/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CExtractRelevantConceptRolePreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CExtractRelevantConceptRolePreProcess::CExtractRelevantConceptRolePreProcess() {
			}


			CExtractRelevantConceptRolePreProcess::~CExtractRelevantConceptRolePreProcess() {
			}


			CConcreteOntology* CExtractRelevantConceptRolePreProcess::preprocess(CConcreteOntology* ontology, CPreProcessContext* context) {
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();
				
				if (!nonELConstructsUsed) {
					CMBox* mBox = ontology->getDataBoxes()->getMBox();
					CTBox* tBox = ontology->getDataBoxes()->getTBox();
					CRBox* rBox = ontology->getDataBoxes()->getRBox();
					CABox* aBox = ontology->getDataBoxes()->getABox();

					CConceptVector* conceptVec = tBox->getConceptVector();
					cint64 conceptCount = tBox->getConceptCount();
					CRoleVector* roleVec = rBox->getRoleVector();

					cint64 markedPropagationIntoCreationDirectionConceptCount = 0;

					QHash<CRole*,bool> creationRoleHash;
					for (cint64 conIdx = 0; conIdx < conceptCount; ++conIdx) {
						CConcept* concept = conceptVec->getData(conIdx);
						if (concept) {
							CConceptOperator* conOperator = concept->getConceptOperator();
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) {
								CRole* role = concept->getRole();
								CRoleProcessData* roleData = (CRoleProcessData*)role->getRoleData();
								if (roleData) {
									roleData->setInferRelevantFlag(true);
								}
							}
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ABSORPTION_RELEVANT_TYPE)) {
								CConceptProcessData* conceptData = (CConceptProcessData*)concept->getConceptData();
								if (conceptData) {
									conceptData->setInferRelevantFlag(true);
								}
								for (CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList(); opLinkerIt; opLinkerIt = opLinkerIt->getNext()) {
									CConcept* opConcept = opLinkerIt->getData();
									CConceptProcessData* opConceptData = (CConceptProcessData*)opConcept->getConceptData();
									if (opConceptData) {
										opConceptData->setInferRelevantFlag(true);
									}
								}
							}
						}
					}


					for (cint64 conIdx = 0; conIdx < conceptCount; ++conIdx) {
						CConcept* concept = conceptVec->getData(conIdx);
						if (concept) {
							CConceptOperator* conOperator = concept->getConceptOperator();
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE | CConceptOperator::CCFS_POSSIBLE_ROLE_CREATION_TYPE)) {
								CRole* role = concept->getRole();
								if (creationRoleHash.contains(role)) {
									CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
									if (conProData) {
										++markedPropagationIntoCreationDirectionConceptCount;
										conProData->setPropagationIntoCreationDirection(true);
									}
								}
							}
						}
					}


					LOG(INFO,"::Konclude::Reasoner::Preprocess::PropagationIntoCreationDirectionExtractor",logTr("Marked %1 concepts with possibly propagation into creation direction.").arg(markedPropagationIntoCreationDirectionConceptCount),this);
				}
				return ontology;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
