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

#include "CExtractPropagationIntoCreationDirectionPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CExtractPropagationIntoCreationDirectionPreProcess::CExtractPropagationIntoCreationDirectionPreProcess() {
			}


			CExtractPropagationIntoCreationDirectionPreProcess::~CExtractPropagationIntoCreationDirectionPreProcess() {
			}


			CConcreteOntology* CExtractPropagationIntoCreationDirectionPreProcess::preprocess(CConcreteOntology* ontology, CPreProcessContext* context) {
				bool skipForELOntologies = CConfigDataReader::readConfigBoolean(context->getConfiguration(),"Konclude.Calculation.Preprocessing.ExtractPropagationIntoCreationDirection.SkipForELFragment",true);
				bool nonELConstructsUsed = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getBuildConstructFlags()->isNonELConstructUsed();
				
				if (nonELConstructsUsed || !skipForELOntologies) {
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
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_POSSIBLE_ROLE_CREATION_TYPE)) {
								CRole* role = concept->getRole();
								if (!creationRoleHash.contains(role)) {
									for (CSortedNegLinker<CRole*>* superRoleIt = role->getIndirectSuperRoleList(); superRoleIt; superRoleIt = superRoleIt->getNext()) {
										bool superRoleNegation = superRoleIt->isNegated();
										CRole* superRole = superRoleIt->getData();
										if (!superRoleNegation) {
											bool& roleInserted = creationRoleHash[superRole];
											if (!roleInserted) {
												roleInserted = true;
											}
										}
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
									CRoleProcessData* roleProData = (CRoleProcessData*)role->getRoleData();
									if (roleProData && !roleProData->hasPropagationAndCreationConceptsFlag()) {
										roleProData->setPropagationAndCreationConceptsFlag(true);
									}
								}
							}
						}
					}


					LOG(INFO,"::Konclude::Reasoner::Preprocess::PropagationIntoCreationDirectionExtractor",logTr("Marked %1 concepts with possible propagation into creation direction.").arg(markedPropagationIntoCreationDirectionConceptCount),this);
				}
				return ontology;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
