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

#include "CSatisfiableTaskIncrementalConsistencyPreyingAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::CSatisfiableTaskIncrementalConsistencyPreyingAnalyser() {
				}


				CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::~CSatisfiableTaskIncrementalConsistencyPreyingAnalyser() {
				}


				bool CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsAdapter = statCalcTask->getSatisfiableTaskIncrementalConsistencyTestingAdapter();
					if (incConsAdapter) {
						CConsistenceObserver* consObs = nullptr;
						consObs = incConsAdapter->getConsistenceObserver();
						if (consObs) {
							CConcreteOntology* prevConsOntology = incConsAdapter->getPreviousConsistentOntology();
							CConsistenceTaskData* prevConsTaskData = nullptr;
							if (prevConsOntology) {
								CConsistence* prevConsistence = prevConsOntology->getConsistence();
								if (prevConsistence) {
									CConsistenceData* prevConsData = prevConsistence->getConsistenceModelData();
									if (prevConsData) {
										prevConsTaskData = dynamic_cast<CConsistenceTaskData*>(prevConsData);
									}
								}
							}

							CSatisfiableCalculationTask* rootStatCalcTask = (CSatisfiableCalculationTask*)statCalcTask->getRootTask();
							CIncrementalConsistenceTaskData* consTaskData = new CIncrementalConsistenceTaskData(rootStatCalcTask,statCalcTask,prevConsOntology,prevConsTaskData);
							analyseIncrementalChanges(statCalcTask,consTaskData,calcAlgContext);
							consObs->tellConsistenceData(consTaskData);
							return true;
						}
					}
					return false;
				}




				CIndividualProcessNode* CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::getMergedIntoIndividualNode(CIndividualProcessNode* indiNode, CIndividualProcessNodeVector* indiNodeVec, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNode* mergedIntoIndiNode = indiNode;
					while (mergedIntoIndiNode && mergedIntoIndiNode->getMergedIntoIndividualNodeID() != mergedIntoIndiNode->getIndividualNodeID()) {
						mergedIntoIndiNode = indiNodeVec->getData(mergedIntoIndiNode->getMergedIntoIndividualNodeID());
					}
					return mergedIntoIndiNode;
				}




				bool CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::analyseIncrementalChanges(CSatisfiableCalculationTask* statCalcTask, CIncrementalConsistenceTaskData* incConsData, CCalculationAlgorithmContext* calcAlgContext) {

					CConcreteOntology* prevConsOntology = incConsData->getPreviousOntology();
					CConsistenceTaskData* prevConsTaskData = incConsData->getPreviousConsistenceData();
					QSet<cint64>* indirectlyChangedNodeSet = incConsData->getIndirectlyChangedNodeSet();
					QSet<cint64>* deterministicallyChangedNodeSet = incConsData->getDeterministicallyChangedNodeSet();
					QSet<cint64>* changedCompatibleNodeSet = incConsData->getChangedCompatibleNodeSet();
					if (prevConsTaskData) {
						CSatisfiableCalculationTask* prevCompGraphCalcTask = prevConsTaskData->getCompletionGraphCachedSatisfiableTask();			
						CIndividualProcessNodeVector* indiNodeVec = statCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();						
						CIndividualProcessNodeVector* prevIndiNodeVec = prevCompGraphCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
						cint64 incExpID = statCalcTask->getProcessingDataBox()->getIncrementalExpansionID();
						cint64 indiNodeCount = indiNodeVec->getItemCount();
						cint64 indiNodeStartIdx = indiNodeVec->getItemMinIndex();
						cint64 addNodeCount = 0;
						cint64 totalNodeCount = 0;
						cint64 prevNodeCount = 0;
						statCalcTask->getProcessingDataBox()->setMaxIncrementalPreviousCompletionGraphNodeID(prevIndiNodeVec->getItemCount());
						for (cint64 i = indiNodeStartIdx; i < indiNodeCount; ++i) {
							CIndividualProcessNode* indiNode = indiNodeVec->getData(i);
							CIndividualProcessNode* prevIndiNode = prevIndiNodeVec->getData(i);
							if (prevIndiNode) {
								++prevNodeCount;
							}
							if (indiNode) {
								++totalNodeCount;
								if (!prevIndiNode) {
									++addNodeCount;
								}
								if (!indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) || prevIndiNode && !prevIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFPURGEDBLOCKED) && prevIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSUCCESSORNOMINALCONNECTION)) {
									bool indirectlyChanged = false;
									bool deterministicallyChanged = false;
									if (prevIndiNode || indiNode->isNominalIndividualNode()) {
										if (indiNode != prevIndiNode) {
											indirectlyChangedNodeSet->insert(-i);
											indirectlyChanged = true;
										}
										if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFINCREMENTALEXPANDING)) {
											if (indiNode->getIncrementalExpansionID() == incExpID) {
												deterministicallyChangedNodeSet->insert(-i);
												deterministicallyChanged = true;
											}
										}
									}

									if (indirectlyChanged || deterministicallyChanged) {
										bool compatible = false;
										if (indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED) && !indiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
											compatible = true;
										} else {
											CIndividualProcessNode* mergedIntoIndiNode = getMergedIntoIndividualNode(indiNode,indiNodeVec,calcAlgContext);
											if (prevIndiNode) {
												CIndividualProcessNode* prevMergedIntoIndiNode = getMergedIntoIndividualNode(prevIndiNode,prevIndiNodeVec,calcAlgContext);
												if (mergedIntoIndiNode->getIndividualNodeID() == prevMergedIntoIndiNode->getIndividualNodeID()) {
													CReapplyConceptLabelSet* conSet = mergedIntoIndiNode->getReapplyConceptLabelSet(false);
													if (conSet && prevMergedIntoIndiNode) {
														CReapplyConceptLabelSet* prevConSet = prevMergedIntoIndiNode->getReapplyConceptLabelSet(false);
														if (prevConSet && prevConSet->getConceptSignatureValue() == conSet->getConceptSignatureValue() && conSet->getConceptCount() == prevConSet->getConceptCount()) {
															CReapplyConceptLabelSetIterator prevConIt = prevConSet->getConceptLabelSetIterator(true);
															CReapplyConceptLabelSetIterator conIt = conSet->getConceptLabelSetIterator(true);
															bool identicalLabels = true;
															while (prevConIt.hasValue() && conIt.hasValue() && identicalLabels) {
																CConceptDescriptor* conDes = conIt.getConceptDescriptor();
																CConceptDescriptor* prevConDes = prevConIt.getConceptDescriptor();												
																if (conDes && prevConDes) {
																	if (conDes->getConcept() != prevConDes->getConcept()) {
																		identicalLabels = false;
																	}
																	if (conDes->isNegated() != prevConDes->isNegated()) {
																		identicalLabels = false;
																	}
																} else if (conDes || prevConDes) {
																	identicalLabels = false;
																}
																conIt.moveNext();
																prevConIt.moveNext();
															}
															if (conIt.hasNext() || prevConIt.hasNext()) {
																identicalLabels = false;
															}
															if (identicalLabels) {
																compatible = true;
															}
														}
													}
												}
											}			
										}
										if (compatible) {

											// test for problematic at-most cardinality restrictions
											bool problematicConcepts = false;
											CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
											for (CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker(); conDesIt && !problematicConcepts; conDesIt = conDesIt->getNext()) {
												CConcept* concept = conDesIt->getConcept();
												bool negation = conDesIt->isNegated();
												cint64 opCode = concept->getOperatorCode();
												if (!negation && opCode == CCATMOST || negation && opCode == CCATLEAST) {
													problematicConcepts = true;
												}
											}
											

											bool problematicNeighbour = false;
											if (problematicConcepts) {
												CSuccessorIterator succIt(indiNode->getSuccessorIterator());
												while (succIt.hasNext() && !problematicNeighbour) {
													CIndividualLinkEdge* succLink = succIt.nextLink();
													CIndividualProcessNode* succIndi = getSuccessorIndividual(indiNode,succLink,calcAlgContext);
													if (succIndi->isNominalIndividualNode()) {
														if (!prevIndiNode->hasRoleSuccessorToIndividual(succLink->getLinkRole(),succIndi->getIndividualNodeID(),false)) {
															problematicNeighbour = true;
														}
													}
												}
												CConnectionSuccessorSetIterator connIt(indiNode->getConnectionSuccessorIterator());
												while (connIt.hasNext() && !problematicNeighbour) {
													cint64 connID = connIt.next();
													CIndividualProcessNode* connIndiNode = indiNodeVec->getData(connID);
													if (connIndiNode->isNominalIndividualNode()) {
														CIndividualProcessNode* prevConnIndiNode = prevIndiNodeVec->getData(connID);
														if (prevConnIndiNode) {
															CSuccessorRoleIterator connRoleIt(connIndiNode->getSuccessorRoleIterator(indiNode));
															while (connRoleIt.hasNext() && !problematicNeighbour) {
																CIndividualLinkEdge* connLink = connRoleIt.next();
																if (!prevConnIndiNode->hasRoleSuccessorToIndividual(connLink->getLinkRole(),prevIndiNode,false)) {
																	problematicNeighbour = true;
																}
															}
														} else {
															problematicNeighbour = true;
														}
													}
												}
											}


											if (!problematicConcepts || !problematicNeighbour) {
												changedCompatibleNodeSet->insert(-i);
											}
										}
									}
								}
							}
						}
						incConsData->setAddedNodeCount(addNodeCount);
						incConsData->setPreviousNodeCount(prevNodeCount);
						incConsData->setTotalNodeCount(totalNodeCount);
					}

					return true;
				}







				CIndividualProcessNode* CSatisfiableTaskIncrementalConsistencyPreyingAnalyser::getSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNode* succIndi = nullptr;

					if (link->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag())) {
						succIndi = link->getOppositeIndividual(indi);
					} else {
						STATINC(INDINODEUPDATELOADCOUNT,calcAlgContext);
						succIndi = link->getOppositeIndividual(indi);
						if (!succIndi->isLocalizationTagUpToDate(calcAlgContext->getUsedProcessTagger()->getCurrentLocalizationTag()) && succIndi->isRelocalized()) {
							cint64 succIndiId = link->getOppositeIndividualID(indi);
							CIndividualProcessNodeVector* indiProcNodeVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
							succIndi = indiProcNodeVec->getData(succIndiId);
						}
					}
					return succIndi;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
