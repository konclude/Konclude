/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CSatisfiableTaskSaturationOccurrenceStatisticsCollector.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSatisfiableTaskSaturationOccurrenceStatisticsCollector::CSatisfiableTaskSaturationOccurrenceStatisticsCollector(COccurrenceStatisticsCacheWriter* occStatsCacheWriter) {
					mOccStatsCacheWriter = occStatsCacheWriter;
					mCollectedOccStatsNodeCount = 0;
					mCollectedOccStatsSuccNodeCount = 0;
					mCollectedOccStatsConceptCount = 0;
					mCollectedOccStatsRoleCount = 0;
				}


				CSatisfiableTaskSaturationOccurrenceStatisticsCollector::~CSatisfiableTaskSaturationOccurrenceStatisticsCollector() {
				}


				bool CSatisfiableTaskSaturationOccurrenceStatisticsCollector::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					mOntology = statCalcTask->getProcessingDataBox()->getOntology();
					mOntologyTag = mOntology->getOntologyID();
					return collectOccurrenceStatistics(statCalcTask->getProcessingDataBox(), calcAlgContext);
				}





				CIndividualSaturationProcessNode* CSatisfiableTaskSaturationOccurrenceStatisticsCollector::getSuccessorNode(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode* satNode, CCalculationAlgorithmContext* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNegation = conDes->isNegated();
					CRole* role = concept->getRole();
					CConceptData* conceptData = concept->getConceptData();
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* extSatCalcRefLinkData = confSatRefLinkingData->getExistentialSuccessorConceptSaturationReferenceLinkingData();
							if (extSatCalcRefLinkData) {
								CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)extSatCalcRefLinkData->getIndividualProcessNodeForConcept();
								if (existIndiNode) {
									return existIndiNode;
								}
							}
						}
					}
					CSortedNegLinker<CConcept*>* conceptOpLinkerIt = concept->getOperandList();
					while (conceptOpLinkerIt) {
						CConcept* opConcept = conceptOpLinkerIt->getData();
						bool opConNegation = conceptOpLinkerIt->isNegated() ^ conNegation;
						CConceptData* opConceptData = opConcept->getConceptData();
						if (opConceptData) {
							CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
							CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
							if (conRefLinking) {
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
								CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
								if (satCalcRefLinkData) {
									CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
									if (existIndiNode) {
										return existIndiNode;
									}
								}
							}
						}
						conceptOpLinkerIt = conceptOpLinkerIt->getNext();
					}
					CConcept* baseTopConcept = nullptr;
					if (!role->isDataRole()) {
						baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopConcept();
					} else {
						baseTopConcept = calcAlgContext->getUsedProcessingDataBox()->getOntologyTopDataRangeConcept();
					}
					CConcept* opConcept = baseTopConcept;
					bool opConNegation = false;
					CConceptData* opConceptData = opConcept->getConceptData();
					if (opConceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)opConceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(opConNegation);
							if (satCalcRefLinkData) {
								CIndividualSaturationProcessNode* existIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
								if (existIndiNode) {
									return existIndiNode;
								}
							}
						}
					}
					return nullptr;
				}




				bool CSatisfiableTaskSaturationOccurrenceStatisticsCollector::visitNodeSuccessors(CIndividualSaturationProcessNode* satNode,
					bool visitedExistentialSuccessessors, bool visitNominalSuccessors, bool visitDataSuccessors, bool analyseConcepts, bool analyseSuccessorLinks,
					function<bool(CIndividualSaturationProcessNode* succNode, cint64 succId)> visitSuccNodeFunc, function<bool(CRole* role, bool inversed)> visitSuccRoleFunc, CCalculationAlgorithmContext* calcAlgContext) {

					CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
					CLinkedRoleSaturationSuccessorHash* linkedSuccHash = satNode->getLinkedRoleSuccessorHash(false);

					CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
					if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker() && (!linkedSuccHash || copiedNode->getLinkedRoleSuccessorHash(false))) {
						return visitNodeSuccessors(copiedNode, visitedExistentialSuccessessors, visitNominalSuccessors, visitDataSuccessors, analyseConcepts, analyseSuccessorLinks, visitSuccNodeFunc, visitSuccRoleFunc, calcAlgContext);
					}

					bool visited = false;

					if (analyseSuccessorLinks && linkedSuccHash) {
						CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>* linkSuccDatahash = linkedSuccHash->getLinkedRoleSuccessorHash();
						for (CPROCESSHASH<CRole*, CLinkedRoleSaturationSuccessorData*>::const_iterator it = linkSuccDatahash->constBegin(), itEnd = linkSuccDatahash->constEnd(); it != itEnd; ++it) {
							CRole* role = it.key();
							CLinkedRoleSaturationSuccessorData* linkSuccData = it.value();
							if (linkSuccData->mSuccCount > 0) {
								CPROCESSMAP<cint64, CSaturationSuccessorData*>* succNodeMap = linkSuccData->getSuccessorNodeDataMap(false);
								for (CPROCESSMAP<cint64, CSaturationSuccessorData*>::const_iterator succIt = succNodeMap->constBegin(), succItEnd = succNodeMap->constEnd(); succIt != succItEnd; ++succIt) {
									bool roleSuccessorVisited = false;
									CSaturationSuccessorData* succData = succIt.value();
									if (succData->mActiveCount > 0) {
										CIndividualSaturationProcessNode* succSatNode = succData->mSuccIndiNode;
										bool visistSuccessorNode = true;
										if (!succSatNode || succSatNode->isABoxIndividualRepresentationNode()) {
											visistSuccessorNode = false;
										}
										if (succSatNode && succSatNode->getIntegratedNominalIndividual() || succData->mVALUENominalConnection) {
											if (!visitNominalSuccessors) {
												visistSuccessorNode = false;
											}
										} else {
											if (role->isDataRole()) {
												if (!visitDataSuccessors) {
													visistSuccessorNode = false;
												}
											} else {
												if (!visitedExistentialSuccessessors) {
													visistSuccessorNode = false;
												}
											}
										}
										if (visistSuccessorNode) {
											roleSuccessorVisited = true;
											visited |= visitSuccNodeFunc(succSatNode, succSatNode->getIndividualID());
										}
									}

									if (roleSuccessorVisited) {
										bool baseRole = false;
										for (CXNegLinker<CRole*>* creationRoleLinkerIt = succData->mCreationRoleLinker; creationRoleLinkerIt && !baseRole; creationRoleLinkerIt = creationRoleLinkerIt->getNext()) {
											if (!creationRoleLinkerIt->isNegated() && creationRoleLinkerIt->getData() == role) {
												baseRole = true;
											}
										}
										if (baseRole) {				
											for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
												CRole* superRole = superRoleLinkerIt->getData();
												bool superRoleNegation = superRoleLinkerIt->isNegated();
												visited |= visitSuccRoleFunc(superRole, superRoleNegation);
											}
										}
									}
								}
							}							
						}

					} else if (analyseConcepts && conSet) {
						for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
							CConcept* concept = conSatDesIt->getConcept();
							bool conNegation = conSatDesIt->isNegated();
							cint64 conCode = concept->getOperatorCode();
							CRole* role = concept->getRole();
							bool existentialSuccessorCand = false;
							if (!conNegation && conCode == CCVALUE && visitNominalSuccessors) {
								visited |= visitSuccNodeFunc(nullptr, concept->getNominalIndividual()->getIndividualID());
							}
							if (!conNegation && conCode == CCATLEAST || conNegation && conCode == CCATMOST) {
								CIndividualSaturationProcessNode* succSatNode = getSuccessorNode(conSatDesIt, satNode, calcAlgContext);
								CSaturationSuccessorData succData;
								succData.mSuccCount = concept->getParameter();
								if (conNegation) {
									succData.mSuccCount--;
								}
								visited |= visitSuccNodeFunc(succSatNode, succSatNode->getIndividualID());
							}

							if (!conNegation && (conCode == CCSOME || conCode == CCAQSOME) || conNegation && (conCode == CCALL)) {
								existentialSuccessorCand = true;
							}
							if (role && existentialSuccessorCand) {
								CIndividualSaturationProcessNode* succSatNode = getSuccessorNode(conSatDesIt, satNode, calcAlgContext);
								if (succSatNode) {
									bool visistSuccessorNode = true;
									if (succSatNode->getIntegratedNominalIndividual()) {
										if (!visitNominalSuccessors) {
											visistSuccessorNode = false;
										}
									} else {
										if (role->isDataRole()) {
											if (!visitDataSuccessors) {
												visistSuccessorNode = false;
											}
										} else {
											if (!visitedExistentialSuccessessors) {
												visistSuccessorNode = false;
											}
										}
									}

									if (visistSuccessorNode) {
										visited |= visitSuccNodeFunc(succSatNode, succSatNode->getIndividualID());

										for (CSortedNegLinker<CRole*>* superRoleLinkerIt = role->getIndirectSuperRoleList(); superRoleLinkerIt; superRoleLinkerIt = superRoleLinkerIt->getNext()) {
											CRole* superRole = superRoleLinkerIt->getData();
											bool superRoleNegation = superRoleLinkerIt->isNegated();
											visited |= visitSuccRoleFunc(superRole, superRoleNegation);
										}

									}
								}
							}

						}
					}

					return visited;
				}






				bool CSatisfiableTaskSaturationOccurrenceStatisticsCollector::collectOccurrenceStatistics(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContext* calcAlgContext) {
					if (processingDataBox->hasIndividualSaturationCompletedNodeLinker()) {

						CIndividualSaturationProcessNodeLinker* succIdentNodeLinker = nullptr;
						for (CIndividualSaturationProcessNodeLinker* indiNodeLinkerIt = processingDataBox->getIndividualSaturationCompletedNodeLinker(); indiNodeLinkerIt; indiNodeLinkerIt = indiNodeLinkerIt->getNext()) {
							CIndividualSaturationProcessNode* indiProcessNode = indiNodeLinkerIt->getProcessingIndividual();
							if (indiProcessNode->isOccurrenceStatisticsCollectingRequired() && !indiProcessNode->isOccurrenceStatisticsCollected()) {

								CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(calcAlgContext->getTemporaryMemoryAllocationManager());
								indiProcessNodeLinker->initProcessNodeLinker(indiProcessNode, true);
								succIdentNodeLinker = indiProcessNodeLinker->append(succIdentNodeLinker);

								indiProcessNode->setOccurrenceStatisticsCollected(true);
							}
						}

						while (succIdentNodeLinker) {
							CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker = succIdentNodeLinker;
							succIdentNodeLinker = succIdentNodeLinker->getNext();
							indiProcessNodeLinker->clearNext();
							++mCollectedOccStatsNodeCount;

							CIndividualSaturationProcessNode* indiProcessNode = indiProcessNodeLinker->getProcessingIndividual();

							visitNodeSuccessors(indiProcessNode, true, false, true, true, true, [&](CIndividualSaturationProcessNode* succNode, cint64 succIndiId)->bool {
								if (succNode && !succNode->isOccurrenceStatisticsCollected()) {
									succNode->setOccurrenceStatisticsCollectingRequired(true);
									succNode->setOccurrenceStatisticsCollected(true);
									CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker = CObjectAllocator< CIndividualSaturationProcessNodeLinker >::allocateAndConstruct(calcAlgContext->getTemporaryMemoryAllocationManager());
									indiProcessNodeLinker->initProcessNodeLinker(succNode, true);
									succIdentNodeLinker = indiProcessNodeLinker->append(succIdentNodeLinker);		
									++mCollectedOccStatsSuccNodeCount;
								}
								return true;
							}, [&](CRole* role, bool inversed)->bool {
								mOccStatsCacheWriter->incRoleInstanceOccurrencceStatistics(mOntology, role->getRoleTag(), 1, 0, 0, 1, 0, 0);
								++mCollectedOccStatsRoleCount;
								return true;
							}, calcAlgContext);

							collectConceptOccurrenceStatistics(indiProcessNode, calcAlgContext);
						}

					}
					return true;
				}






				bool CSatisfiableTaskSaturationOccurrenceStatisticsCollector::collectConceptOccurrenceStatistics(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* conSet = indiNode->getReapplyConceptSaturationLabelSet(false);
					for (CConceptSaturationDescriptor* conSatDesLinkerIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesLinkerIt; conSatDesLinkerIt = conSatDesLinkerIt->getNext()) {
						CConcept* concept = conSatDesLinkerIt->getConcept();
						mOccStatsCacheWriter->incConceptInstanceOccurrencceStatistics(mOntology, concept->getConceptTag(), 1, 0, 0, 1);
						++mCollectedOccStatsConceptCount;
					}
					return true;
				}



			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
