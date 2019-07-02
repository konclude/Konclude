/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CDataExtenderPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CDataExtenderPreProcess::CDataExtenderPreProcess() {
				mOntology = nullptr;
				mLastConceptId = 0;
			}


			CDataExtenderPreProcess::~CDataExtenderPreProcess() {
			}


			CConcreteOntology *CDataExtenderPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				mOntology = ontology;

				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.ProcessingDataExtender.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				CMBox *mBox = ontology->getDataBoxes()->getMBox();
				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CABox *aBox = ontology->getDataBoxes()->getABox();

				CConceptVector* conceptVector = tBox->getConceptVector();
				CRoleVector* roleVector = rBox->getRoleVector();
				CIndividualVector* indiVector = aBox->getIndividualVector();
				CBranchingTriggerVector* branchTriggVec = mBox->getBranchingTriggerVector();
				CImplicationReplacementVector* impRepVec = mBox->getImplicationReplacementVector();

				CRoleVector *roles = rBox->getRoleVector();

				CConcept *topConcept = conceptVector->getLocalData(1);

				mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


				mStatConceptDataExtended = 0;
				mStatRoleDataExtended = 0;
				mStatIndiDataExtended = 0;


				cint64 roleCount = rBox->getRoleCount();
				for (cint64 roleIdx = 0; roleIdx < roleCount; ++roleIdx) {
					CRole* role = roleVector->getLocalData(roleIdx);
					if (role) {
						if (!role->hasRoleData()) {
							CRoleProcessData* roleData = CObjectAllocator< CRoleProcessData >::allocateAndConstruct(mMemMan);
							roleData->initRoleProcessExtensionData();
							role->setRoleData(roleData);
							++mStatRoleDataExtended;
						}
					}
				}

				cint64 indiCount = aBox->getIndividualCount();
				for (cint64 indiIdx = 0; indiIdx < indiCount; ++indiIdx) {
					CIndividual* indi = indiVector->getLocalData(indiIdx);
					if (indi) {
						if (!indi->hasIndividualData()) {
							CIndividualProcessData* indiData = CObjectAllocator< CIndividualProcessData >::allocateAndConstruct(mMemMan);
							indiData->initIndividualProcessExtensionData();
							indi->setIndividualData(indiData);
							++mStatIndiDataExtended;
						}
					}
				}


				cint64 conCount = tBox->getConceptCount();
				for (cint64 conIdx = mLastConceptId; conIdx < conCount; ++conIdx) {
					CConcept* concept = conceptVector->getLocalData(conIdx);
					if (concept) {
						if (!concept->hasConceptData()) {
							CConceptProcessData* conceptData = CObjectAllocator< CConceptProcessData >::allocateAndConstruct(mMemMan);
							if (nonELConstructsUsed || !skipForELOntologies) {
								conceptData->initConceptProcessExtensionData(branchTriggVec->getData(conIdx),impRepVec->getData(conIdx));
							} else {
								conceptData->initConceptProcessExtensionData(nullptr,nullptr);
							}
							concept->setConceptData(conceptData);
							++mStatConceptDataExtended;
						}
					}
				}
				mLastConceptId = conCount;

				LOG(INFO,"::Konclude::Reasoner::Preprocess::ProcessDataExtender",logTr("Extended %1 concepts and %2 roles for processing.").arg(mStatConceptDataExtended).arg(mStatRoleDataExtended),this);
				return ontology;
			}


			CConcreteOntology* CDataExtenderPreProcess::continuePreprocessing() {
				if (mOntology) {
					CMBox *mBox = mOntology->getDataBoxes()->getMBox();
					CTBox *tBox = mOntology->getDataBoxes()->getTBox();

					CBranchingTriggerVector* branchTriggVec = mBox->getBranchingTriggerVector();
					CImplicationReplacementVector* impRepVec = mBox->getImplicationReplacementVector();

					CConceptVector* conceptVector = tBox->getConceptVector();
					cint64 conCount = tBox->getConceptCount();
					for (cint64 conIdx = mLastConceptId; conIdx < conCount; ++conIdx) {
						CConcept* concept = conceptVector->getLocalData(conIdx);
						if (concept) {
							if (!concept->hasConceptData()) {
								CConceptProcessData* conceptData = CObjectAllocator< CConceptProcessData >::allocateAndConstruct(mMemMan);
								conceptData->initConceptProcessExtensionData(branchTriggVec->getData(conIdx), impRepVec->getData(conIdx));
								concept->setConceptData(conceptData);
								++mStatConceptDataExtended;
							}
						}
					}
					mLastConceptId = conCount;

				}
				return mOntology;
			}




		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
