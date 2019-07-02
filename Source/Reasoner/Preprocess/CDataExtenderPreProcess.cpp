/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CDataExtenderPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CDataExtenderPreProcess::CDataExtenderPreProcess() {
			}


			CDataExtenderPreProcess::~CDataExtenderPreProcess() {
			}


			CConcreteOntology *CDataExtenderPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.ProcessingDataExtender.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();

				CMBox *mBox = ontology->getDataBoxes()->getMBox();
				CTBox *tBox = ontology->getDataBoxes()->getTBox();
				CRBox *rBox = ontology->getDataBoxes()->getRBox();
				CABox *aBox = ontology->getDataBoxes()->getABox();

				CConceptVector* conceptVector = tBox->getConceptVector();
				CRoleVector* roleVector = rBox->getRoleVector();
				CBranchingTriggerVector* branchTriggVec = mBox->getBranchingTriggerVector();
				CImplicationReplacementVector* impRepVec = mBox->getImplicationReplacementVector();

				CRoleVector *roles = rBox->getRoleVector();

				CConcept *topConcept = conceptVector->getLocalData(1);

				mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();


				mStatConceptDataExtended = 0;
				mStatRoleDataExtended = 0;


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

				cint64 conCount = tBox->getConceptCount();
				for (cint64 conIdx = 0; conIdx < conCount; ++conIdx) {
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

				LOG(INFO,"::Konclude::Reasoner::Preprocess::ProcessDataExtender",logTr("Extended %1 concepts and %2 roles for processing.").arg(mStatConceptDataExtended).arg(mStatRoleDataExtended),this);
				return ontology;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
