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

#include "CCompletionGraphCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCompletionGraphCacheHandler::CCompletionGraphCacheHandler() {
					mLastOntology = nullptr;
					mLastLoadedCachedCompletionGraph = false;
					mPreviousConsistencyCachedCompletionGraph = false;
				}


				CCompletionGraphCacheHandler::~CCompletionGraphCacheHandler() {
				}



				bool CCompletionGraphCacheHandler::loadConsistenceModelData(CCalculationAlgorithmContext* calcAlgContext) {
					CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
					mCalcTask = calcAlgContext->getUsedSatisfiableCalculationTask();
					mIndiProcNodeVec = mCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
					if (mLastOntology != ontology) {
						mLastOntology = ontology;
						mIncrementalExpansionCaching = false;
						mLastLoadedCachedCompletionGraph = false;
						mPreviousConsistencyCachedCompletionGraph = false;
						mCurrentIncrementalExpansionID = 0;
						CConsistence* consistence = ontology->getConsistence();
						if (consistence) {
							CConsistenceData* consData = consistence->getConsistenceModelData();
							if (consData) {
								CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
								if (consTaskData) {
									mDetSatCalcTask = consTaskData->getDeterministicSatisfiableTask();
									mCompGraphCachedCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
									if (mDetSatCalcTask && mCompGraphCachedCalcTask) {
										mDetSatProcNodeVec = mDetSatCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
										mCompGraphCachedProcNodeVec = mCompGraphCachedCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
										mLastLoadedCachedCompletionGraph = true;
										mDetLocalizationTag = mDetSatCalcTask->getProcessingDataBox()->getProcessContext()->getProcessTagger()->getCurrentLocalizationTag();
									}
								}
							}
						}
						if (!mLastLoadedCachedCompletionGraph) {
							CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsTestAd = mCalcTask->getSatisfiableTaskIncrementalConsistencyTestingAdapter();
							if (incConsTestAd) {
								CConcreteOntology* prevConsOntology = incConsTestAd->getPreviousConsistentOntology();
								CConsistence* prevConsistence = prevConsOntology->getConsistence();
								if (prevConsistence) {
									CConsistenceData* prevConsData = prevConsistence->getConsistenceModelData();
									if (prevConsData) {
										CConsistenceTaskData* prevConsTaskData = dynamic_cast<CConsistenceTaskData*>(prevConsData);
										if (prevConsTaskData) {
											mDetSatCalcTask = prevConsTaskData->getDeterministicSatisfiableTask();
											mCompGraphCachedCalcTask = prevConsTaskData->getCompletionGraphCachedSatisfiableTask();
											if (mDetSatCalcTask && mCompGraphCachedCalcTask) {
												mDetSatProcNodeVec = mDetSatCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
												mCompGraphCachedProcNodeVec = mCompGraphCachedCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
												mPreviousConsistencyCachedCompletionGraph = true;
												mIncrementalExpansionCaching = true;
												mCurrentIncrementalExpansionID = mCalcTask->getProcessingDataBox()->getIncrementalExpansionID();
												mDetLocalizationTag = mDetSatCalcTask->getProcessingDataBox()->getProcessContext()->getProcessTagger()->getCurrentLocalizationTag();
											}
										}
									}
								}
							}
						}
					}
					return mLastLoadedCachedCompletionGraph;
				}


				bool CCompletionGraphCacheHandler::getReactivationIndividuals(CIndividualProcessNode* individualNode, CIndividualReactivationProcessingQueue* reactProcQueue, CCalculationAlgorithmContext* calcAlgContext) {
					loadConsistenceModelData(calcAlgContext);
					bool addedReactivationIndi = false;
					cint64 indiID = individualNode->getIndividualID();
					CIndividualProcessNode* compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(indiID);
					CIndividualProcessNode* detCompGraphCachedIndiNode = mDetSatProcNodeVec->getData(indiID);

					if (compGraphCachedIndiNode && compGraphCachedIndiNode != detCompGraphCachedIndiNode) {
						cint64 detSatIndiCount = mDetSatProcNodeVec->getItemCount()-1;

						CPROCESSINGLIST<cint64> reactivateComplGraphCachedIndiList(calcAlgContext->getTaskProcessorContext());
						CPROCESSINGSET<cint64> reactivateComplGraphCachedIndiSet(calcAlgContext->getTaskProcessorContext());

						CIndividualMergingHash* mergingHash = compGraphCachedIndiNode->getIndividualMergingHash(false);
						if (mergingHash) {
							for (CIndividualMergingHash::const_iterator it = mergingHash->constBegin(), itEnd = mergingHash->constEnd(); it != itEnd; ++it) {
								CIndividual* mergedIndi = it.key();
								cint64 mergedIndiID = mergedIndi->getIndividualID();
								CIndividualProcessNode* compGraphCachedMergedIndiNode = mCompGraphCachedProcNodeVec->getData(mergedIndiID);
								CIndividualProcessNode* detSatMergedIndiNode = nullptr;
								if (mergedIndiID <= detSatIndiCount) {
									detSatMergedIndiNode = mDetSatProcNodeVec->getData(mergedIndiID);
								}
								if (compGraphCachedMergedIndiNode != detSatMergedIndiNode && detSatMergedIndiNode) {
									addedReactivationIndi |= reactProcQueue->insertReactivationIndiviudal(detSatMergedIndiNode,true);
								}
							}
						}


						CConnectionSuccessorSetIterator connSuccSetIt(compGraphCachedIndiNode->getConnectionSuccessorIterator());
						while (connSuccSetIt.hasNext()) {
							cint64 connIndiID = connSuccSetIt.next();
							CIndividualProcessNode* detSatConnIndiNode = nullptr;
							if (connIndiID <= detSatIndiCount) {
								detSatConnIndiNode = mDetSatProcNodeVec->getData(connIndiID);
							}
							if (detSatConnIndiNode) {
								CIndividualProcessNode* compGraphCachedConnIndiNode = mCompGraphCachedProcNodeVec->getData(connIndiID);
								if (compGraphCachedConnIndiNode != detSatConnIndiNode) {
									addedReactivationIndi |= reactProcQueue->insertReactivationIndiviudal(detSatConnIndiNode,false);
								}
							} else {
								reactivateComplGraphCachedIndiList.append(connIndiID);
								reactivateComplGraphCachedIndiSet.insert(connIndiID);
							}
						}

						while (!reactivateComplGraphCachedIndiList.isEmpty()) {
							cint64 connIndiID = reactivateComplGraphCachedIndiList.takeFirst();
							CIndividualProcessNode* detSatConnIndiNode = nullptr;
							if (connIndiID <= detSatIndiCount) {
								detSatConnIndiNode = mDetSatProcNodeVec->getData(connIndiID);
							}
							if (!detSatConnIndiNode) {
								CIndividualProcessNode* compGraphCachedConnIndiNode = mCompGraphCachedProcNodeVec->getData(connIndiID);
								CConnectionSuccessorSetIterator connSuccSetIt(compGraphCachedConnIndiNode->getConnectionSuccessorIterator());
								while (connSuccSetIt.hasNext()) {
									cint64 connIndiID = connSuccSetIt.next();
									if (!reactivateComplGraphCachedIndiSet.contains(connIndiID)) {
										reactivateComplGraphCachedIndiSet.insert(connIndiID);
										reactivateComplGraphCachedIndiList.append(connIndiID);
									}
								}
							} else {
								addedReactivationIndi |= reactProcQueue->insertReactivationIndiviudal(detSatConnIndiNode,true);
							}
						}
					}
					return addedReactivationIndi;
				}


				bool CCompletionGraphCacheHandler::isIndividualNodeCompletionGraphConsistenceBlocked(CIndividualProcessNode* individualNode, bool& conceptSetExtended, CCalculationAlgorithmContext* calcAlgContext) {
					conceptSetExtended = false;
					if (loadConsistenceModelData(calcAlgContext)) {
						if (mIncrementalExpansionCaching) {
							if (individualNode->getIncrementalExpansionID() >= mCurrentIncrementalExpansionID) {
								return false;
							}
						}
						cint64 detSatIndiCount = mDetSatProcNodeVec->getItemCount()-1;
						cint64 indiID = individualNode->getIndividualID();
						if (indiID <= detSatIndiCount) {
							CIndividualProcessNode* detSatIndiNode = mDetSatProcNodeVec->getData(indiID);
							if (detSatIndiNode) {
								CIndividualProcessNode* compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(indiID);
								while (compGraphCachedIndiNode->getMergedIntoIndividualNodeID() != compGraphCachedIndiNode->getIndividualID()) {
									compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(compGraphCachedIndiNode->getMergedIntoIndividualNodeID());
								}
								if (testCompletionGraphCached(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext)) {
									conceptSetExtended = testConceptLabelExtended(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext);
									return true;
								}
							}
						}
					}
					return false;
				}


				bool CCompletionGraphCacheHandler::testConceptLabelExtended(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = individualNode->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* detCachedConSet = detSatIndiNode->getReapplyConceptLabelSet(false);
					if (conSet->getAddingSortedConceptDescriptionLinker() != detCachedConSet->getAddingSortedConceptDescriptionLinker()) {
						return true;
					}
					return false;
				}



				bool CCompletionGraphCacheHandler::testCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					bool cached = true;
					if (cached) {
						cached &= testConceptLabelSubSetCompletionGraphCached(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext);
					}
					if (cached) {
						cached &= testDistinctSubSetCompletionGraphCached(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext);
					}
					if (cached) {
						cached &= testMergingSubSetCompletionGraphCached(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext);
					}
					if (cached) {
						cached &= testBindingSubSetCompletionGraphCached(individualNode,detSatIndiNode,compGraphCachedIndiNode,calcAlgContext);
					}
					return cached;
				}



				bool CCompletionGraphCacheHandler::testMergingSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					return true;
				}


				bool CCompletionGraphCacheHandler::testDistinctSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDistinctHash* subDistinctHash = individualNode->getDistinctHash(false);
					CDistinctHash* superDistinctHash = compGraphCachedIndiNode->getDistinctHash(false);
					if (subDistinctHash) {
						if (!superDistinctHash) {
							return false;
						}
						for (CDistinctHash::const_iterator it = subDistinctHash->constBegin(), itEnd = subDistinctHash->constEnd(); it != itEnd; ++it) {
							cint64 disIndiID = it.key();
							if (!superDistinctHash->isIndividualDistinct(disIndiID)) {
								return false;
							}
						}
					}
					return true;
				}



				bool CCompletionGraphCacheHandler::testBindingSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {

					CReapplyConceptLabelSet* subConceptSet = individualNode->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true,false,false);
					while (subConSetIt.hasValue()) {
						CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
						CConcept* concept = subConDes->getConcept();

						CConceptOperator* conOperator = concept->getConceptOperator();
						if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_BIND_TYPE)) {
							CConceptPropagationBindingSetHash* indiPropBindSetHash = individualNode->getConceptPropagationBindingSetHash(false);
							if (indiPropBindSetHash) {
								CPropagationBindingSet* indiPropBindSet = indiPropBindSetHash->getPropagationBindingSet(concept,false);
								if (indiPropBindSet) {
									CPropagationBindingMap* indiPropBindMap = indiPropBindSet->getPropagationBindingMap();
									if (!indiPropBindMap->empty()) {
										CConceptPropagationBindingSetHash* compGraphCachedPropBindSetHash = compGraphCachedIndiNode->getConceptPropagationBindingSetHash(false);
										if (!compGraphCachedPropBindSetHash) {
											return true;
										}
										CPropagationBindingSet* compGraphCachedPropBindSet = compGraphCachedPropBindSetHash->getPropagationBindingSet(concept,false);
										if (!compGraphCachedPropBindSet) {
											return true;
										}
										CPropagationBindingMap* compGraphCachedPropBindMap = compGraphCachedPropBindSet->getPropagationBindingMap();

										if (!indiPropBindMap->isKeySubSetOf(compGraphCachedPropBindMap)) {
											return true;
										}
									}
								}
							}
						}
						if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_VARIABLE_BIND_TYPE)) {
							CConceptVariableBindingPathSetHash* indiVarBindSetHash = individualNode->getConceptVariableBindingPathSetHash(false);
							if (indiVarBindSetHash) {
								CVariableBindingPathSet* indiVarBindSet = indiVarBindSetHash->getVariableBindingPathSet(concept,false);
								if (indiVarBindSet) {
									CVariableBindingPathMap* indiVarBindMap = indiVarBindSet->getVariableBindingPathMap();
									if (!indiVarBindMap->empty()) {
										CConceptVariableBindingPathSetHash* compGraphCachedVarBindSetHash = compGraphCachedIndiNode->getConceptVariableBindingPathSetHash(false);
										if (!compGraphCachedVarBindSetHash) {
											return true;
										}
										CVariableBindingPathSet* compGraphCachedVarBindSet = compGraphCachedVarBindSetHash->getVariableBindingPathSet(concept,false);
										if (!compGraphCachedVarBindSet) {
											return true;
										}
										CVariableBindingPathMap* compGraphCachedVarBindMap = compGraphCachedVarBindSet->getVariableBindingPathMap();

										if (!indiVarBindMap->isKeySubSetOf(compGraphCachedVarBindMap)) {
											return true;
										}
									}
								}
							}
							CConceptRepresentativePropagationSetHash* indiVarRepBindSetHash = individualNode->getConceptRepresentativePropagationSetHash(false);
							if (indiVarRepBindSetHash) {
								CRepresentativePropagationSet* indiVarRepBindSet = indiVarRepBindSetHash->getRepresentativePropagationSet(concept,false);
								if (indiVarRepBindSet) {
									CRepresentativePropagationMap* indiVarRepBindMap = indiVarRepBindSet->getRepresentativePropagationMap();
									if (!indiVarRepBindMap->empty()) {
										CConceptRepresentativePropagationSetHash* compGraphCachedVarRepBindSetHash = compGraphCachedIndiNode->getConceptRepresentativePropagationSetHash(false);
										if (!compGraphCachedVarRepBindSetHash) {
											return true;
										}
										CRepresentativePropagationSet* compGraphCachedVarRepBindSet = compGraphCachedVarRepBindSetHash->getRepresentativePropagationSet(concept,false);
										if (!compGraphCachedVarRepBindSet) {
											return true;
										}
										CRepresentativePropagationMap* compGraphCachedVarRepBindMap = compGraphCachedVarRepBindSet->getRepresentativePropagationMap();

										if (!indiVarRepBindMap->isKeySubSetOf(compGraphCachedVarRepBindMap)) {
											return true;
										}
									}
								}
							}
						}

						subConSetIt.moveNext();
					}
					return true;
				}


				bool CCompletionGraphCacheHandler::testConceptLabelSubSetCompletionGraphCached(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* subConceptSet = individualNode->getReapplyConceptLabelSet(false);
					CReapplyConceptLabelSet* superConceptSet = compGraphCachedIndiNode->getReapplyConceptLabelSet(false);
					cint64 subConSetCount = subConceptSet->getConceptCount();
					cint64 superConSetCount = superConceptSet->getConceptCount();
					if (subConSetCount > superConSetCount) {
						return false;
					}
					if (superConSetCount == 0) {
						return true;
					}
					CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true,false,false);
					CReapplyConceptLabelSetIterator superConSetIt = superConceptSet->getConceptLabelSetIterator(true,false,false);
					CConceptDescriptor* superConDes = superConSetIt.getConceptDescriptor();
					cint64 superConTag = superConDes->getConceptTag();
					superConSetIt.moveNext();
					bool contained = false;
					while (subConSetIt.hasValue()) {
						CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
						cint64 subConTag = subConDes->getConceptTag();

						while (superConTag < subConTag) {
							if (!superConSetIt.hasValue()) {
								return false;
							}
							if (testCriticalConcept(individualNode,detSatIndiNode,compGraphCachedIndiNode,superConDes,contained,calcAlgContext)) {
								return false;
							}
							contained = false;
							superConDes = superConSetIt.getConceptDescriptor();
							superConTag = superConDes->getConceptTag();
							superConSetIt.moveNext();
						}
						if (subConTag != superConTag) {
							return false;
						} else {
							if (subConDes->isNegated() != superConDes->isNegated()) {
								return false;
							}
							contained = true;
						}
						subConSetIt.moveNext();
					}

					while (superConSetIt.hasValue()) {
						if (testCriticalConcept(individualNode,detSatIndiNode,compGraphCachedIndiNode,superConDes,contained,calcAlgContext)) {
							return false;
						}
						contained = false;
						superConDes = superConSetIt.getConceptDescriptor();
						superConTag = superConDes->getConceptTag();
						superConSetIt.moveNext();
					}
					if (testCriticalConcept(individualNode,detSatIndiNode,compGraphCachedIndiNode,superConDes,contained,calcAlgContext)) {
						return false;
					}

					return true;
				}


				bool CCompletionGraphCacheHandler::testCriticalConcept(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CConceptDescriptor* conDes, bool containedFlag, CCalculationAlgorithmContext* calcAlgContext) {
					CConcept* concept = conDes->getConcept();
					bool conNegation = conDes->getNegation();
					cint64 opCode = concept->getOperatorCode();
					CRole* role = concept->getRole();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (!containedFlag) {
						if (!conNegation && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) || conNegation && opCode == CCSOME) {
							CRoleSuccessorLinkIterator succLinkIt = individualNode->getRoleSuccessorLinkIterator(role);
							while (succLinkIt.hasNext()) {
								CIndividualLinkEdge* link = succLinkIt.next();
								CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
								if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
									succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
								}
								if (succIndi->getLocalizationTag() > mDetLocalizationTag && !succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)
											|| succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {

									CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
									while (opConLinkerIt) {
										bool opNegated = opConLinkerIt->isNegated()^conNegation;
										CConcept* opCon = opConLinkerIt->getData();
										CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
										if (!succConSet || !succConSet->containsConcept(opCon,opNegated)) {
											return true;
										}
										if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_PBINDAQALL | CConceptOperator::CCF_PBINDALL)) {
											CConceptPropagationBindingSetHash* compGraphCachedPropBindSetHash = compGraphCachedIndiNode->getConceptPropagationBindingSetHash(false);
											if (compGraphCachedPropBindSetHash) {
												CPropagationBindingSet* compGraphCachedPropBindSet = compGraphCachedPropBindSetHash->getPropagationBindingSet(concept,false);
												if (compGraphCachedPropBindSet) {
													CPropagationBindingMap* compGraphCachedPropBindMap = compGraphCachedPropBindSet->getPropagationBindingMap();
													if (!compGraphCachedPropBindMap->empty()) {
														CConceptPropagationBindingSetHash* succPropBindSetHash = succIndi->getConceptPropagationBindingSetHash(false);
														if (!succPropBindSetHash) {
															return true;
														}
														CPropagationBindingSet* succPropBindSet = succPropBindSetHash->getPropagationBindingSet(opCon,false);
														if (!succPropBindSet) {
															return true;
														}
														CPropagationBindingMap* succPropBindMap = succPropBindSet->getPropagationBindingMap();

														if (!compGraphCachedPropBindMap->isKeySubSetOf(succPropBindMap)) {
															return true;
														}
													}
												}
											}
										}
										if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_VARBINDAQALL | CConceptOperator::CCF_VARBINDALL)) {
											CConceptVariableBindingPathSetHash* compGraphCachedVarBindSetHash = compGraphCachedIndiNode->getConceptVariableBindingPathSetHash(false);
											if (compGraphCachedVarBindSetHash) {
												CVariableBindingPathSet* compGraphCachedVarBindSet = compGraphCachedVarBindSetHash->getVariableBindingPathSet(concept,false);
												if (compGraphCachedVarBindSet) {
													CVariableBindingPathMap* compGraphCachedVarBindMap = compGraphCachedVarBindSet->getVariableBindingPathMap();
													if (!compGraphCachedVarBindMap->empty()) {
														CConceptVariableBindingPathSetHash* succVarBindSetHash = succIndi->getConceptVariableBindingPathSetHash(false);
														if (!succVarBindSetHash) {
															return true;
														}
														CVariableBindingPathSet* succVarBindSet = succVarBindSetHash->getVariableBindingPathSet(opCon,false);
														if (!succVarBindSet) {
															return true;
														}
														CVariableBindingPathMap* succVarBindMap = succVarBindSet->getVariableBindingPathMap();

														if (!compGraphCachedVarBindMap->isKeySubSetOf(succVarBindMap)) {
															return true;
														}
													}
												}
											}
											CConceptRepresentativePropagationSetHash* compGraphCachedVarRepBindSetHash = compGraphCachedIndiNode->getConceptRepresentativePropagationSetHash(false);
											if (compGraphCachedVarRepBindSetHash) {
												CRepresentativePropagationSet* compGraphCachedVarRepBindSet = compGraphCachedVarRepBindSetHash->getRepresentativePropagationSet(concept,false);
												if (compGraphCachedVarRepBindSet) {
													CRepresentativePropagationMap* compGraphCachedVarRepBindMap = compGraphCachedVarRepBindSet->getRepresentativePropagationMap();
													if (!compGraphCachedVarRepBindMap->empty()) {
														CConceptRepresentativePropagationSetHash* succVarRepBindSetHash = succIndi->getConceptRepresentativePropagationSetHash(false);
														if (!succVarRepBindSetHash) {
															return true;
														}
														CRepresentativePropagationSet* succVarRepBindSet = succVarRepBindSetHash->getRepresentativePropagationSet(opCon,false);
														if (!succVarRepBindSet) {
															return true;
														}
														CRepresentativePropagationMap* succVarRepBindMap = succVarRepBindSet->getRepresentativePropagationMap();

														if (!compGraphCachedVarRepBindMap->isKeySubSetOf(succVarRepBindMap)) {
															return true;
														}
													}
												}
											}
										}
										opConLinkerIt = opConLinkerIt->getNext();
									}
								}
							}
						} else if (!conNegation && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE))) {
							return testCriticalB2AutomateTransitionOperands(individualNode,detSatIndiNode,compGraphCachedIndiNode,concept,calcAlgContext);
						}

					}
					if (!conNegation && opCode == CCATMOST || conNegation && opCode == CCATLEAST) {
						// check cardinality
						cint64 cardinality = concept->getParameter() - 1*conNegation;

						bool nominalIndiNode = individualNode->isNominalIndividual();

						cint64 updatedCardinality = 0;
						CRoleSuccessorLinkIterator succLinkIt = individualNode->getRoleSuccessorLinkIterator(role);
						while (updatedCardinality <= cardinality && succLinkIt.hasNext()) {
							CIndividualLinkEdge* link = succLinkIt.next();
							CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
							if (!opConLinkerIt) {

								CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
								if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
									succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
								}
								//CIndividualProcessNode* corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(succIndi->getIndividualID());
								//while (corrCachedSuccIndi && corrCachedSuccIndi->getMergedIntoIndividualNodeID() != corrCachedSuccIndi->getIndividualID()) {
								//	corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(corrCachedSuccIndi->getMergedIntoIndividualNodeID());
								//}
								if (succIndi->getLocalizationTag() > mDetLocalizationTag) {
									if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
										CIndividualProcessNode* corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(succIndi->getIndividualID());
										if (corrCachedSuccIndi->getMergedIntoIndividualNodeID() != succIndi->getIndividualID() || corrCachedSuccIndi->getLastMergedIntoIndividualNode() != succIndi->getLastMergedIntoIndividualNode()) {
											++updatedCardinality;
										}
									}
								}

								if (nominalIndiNode) {
									if (link->getCreatorIndividualID() != individualNode->getIndividualID()) {
										if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
											succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
										}
										if (succIndi->isBlockableIndividual()) {
											// requires NN-rule
											return true;
										}
									}
								}
							} else {
								CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
								if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
									succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
								}
								if (nominalIndiNode) {
									if (link->getCreatorIndividualID() != individualNode->getIndividualID()) {
										if (succIndi->isBlockableIndividual()) {
											// requires NN-rule
											return true;
										}
									}
								}
								if (succIndi->getLocalizationTag() > mDetLocalizationTag) {
									if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
										bool containsSomeOps = false;
										while (opConLinkerIt && !containsSomeOps) {
											bool opNegated = opConLinkerIt->isNegated()^conNegation;
											CConcept* opCon = opConLinkerIt->getData();
											bool containsNegated = false;
											CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
											if (succConSet) {
												if (succConSet->containsConcept(opCon,&containsNegated)) {
													if (containsNegated == opNegated) {
														containsSomeOps = true;
													}
												} else {
													containsSomeOps = true;
												}
											}
											opConLinkerIt = opConLinkerIt->getNext();
										}
										if (containsSomeOps) {
											CIndividualProcessNode* corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(succIndi->getIndividualID());
											if (corrCachedSuccIndi->getMergedIntoIndividualNodeID() != succIndi->getIndividualID() || corrCachedSuccIndi->getLastMergedIntoIndividualNode() != succIndi->getLastMergedIntoIndividualNode()) {
												++updatedCardinality;
											}
										}

										//CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
										//CIndividualProcessNode* corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(succIndi->getIndividualID());
										//while (corrCachedSuccIndi && corrCachedSuccIndi->getMergedIntoIndividualNodeID() != corrCachedSuccIndi->getIndividualID()) {
										//	corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(corrCachedSuccIndi->getMergedIntoIndividualNodeID());
										//}
										//if (!corrCachedSuccIndi) {
										//	++updatedCardinality;
										//} else {
										//	bool cachedContainsSomeOps = false;
										//	CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
										//	while (opConLinkerIt && !cachedContainsSomeOps) {
										//		bool opNegated = opConLinkerIt->isNegated()^conNegation;
										//		CConcept* opCon = opConLinkerIt->getData();
										//		bool containsNegated = false;
										//		CReapplyConceptLabelSet* cachedSuccConSet = corrCachedSuccIndi->getReapplyConceptLabelSet(false);
										//		if (cachedSuccConSet) {
										//			if (cachedSuccConSet->containsConcept(opCon,&containsNegated)) {
										//				if (containsNegated == opNegated) {
										//					cachedContainsSomeOps = true;
										//				}
										//			} else {
										//				cachedContainsSomeOps = true;
										//			}
										//		}
										//		opConLinkerIt = opConLinkerIt->getNext();
										//	}
										//	if (!cachedContainsSomeOps) {
										//		++updatedCardinality;
										//	}
										//}
									}
								}
							}
						}

						if (updatedCardinality > cardinality) {
							return true;
						}

						CReapplyRoleSuccessorHash* cachedSuccHash = compGraphCachedIndiNode->getReapplyRoleSuccessorHash(false);
						if (cachedSuccHash) {
							cint64 linkCount = 0;
							CRoleSuccessorLinkIterator succLinkIt = cachedSuccHash->getRoleSuccessorLinkIterator(role,&linkCount);
							if (cardinality - linkCount < updatedCardinality) {

								cint64 cachedCardinality = 0;
								while (succLinkIt.hasNext()) {
									CIndividualLinkEdge* link = succLinkIt.next();
									CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
									if (!opConLinkerIt) {
										++cachedCardinality;
									} else {
										CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
										if (link->isLocalizationTagUpdated(mCompGraphCachedCalcTask->getProcessingDataBox()->getProcessContext()->getUsedProcessTagger())) {
											succIndi = mCompGraphCachedProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
										}
										bool containsSomeOps = false;
										while (opConLinkerIt && !containsSomeOps) {
											bool opNegated = opConLinkerIt->isNegated();
											CConcept* opCon = opConLinkerIt->getData();
											CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
											if (succConSet && succConSet->containsConcept(opCon,opNegated)) {
												containsSomeOps = true;
											}
											opConLinkerIt = opConLinkerIt->getNext();
										}
										if (containsSomeOps) {
											++cachedCardinality;
										}
									}
									if (updatedCardinality + cachedCardinality > cardinality) {
										return true;
									}
								}

							}
						}

					}
					return false;
				}

				bool CCompletionGraphCacheHandler::testCriticalB2AutomateTransitionOperands(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CConcept* concept, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 opCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						// recursive
						CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
						while (opLinkerIt) {
							CConcept* opConcept = opLinkerIt->getData();
							if (testCriticalB2AutomateTransitionOperands(individualNode,detSatIndiNode,compGraphCachedIndiNode,opConcept,calcAlgContext)) {
								return true;
							}
							opLinkerIt = opLinkerIt->getNext();
						}
					} else if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQALL_TYPE)) {
						CRole* conRole = concept->getRole();
						CRoleSuccessorLinkIterator succLinkIt = individualNode->getRoleSuccessorLinkIterator(conRole);
						while (succLinkIt.hasNext()) {
							CIndividualLinkEdge* link = succLinkIt.next();
							CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
							if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
								succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
							}
							if (succIndi->getLocalizationTag() > mDetLocalizationTag) {
								CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
								while (opLinkerIt) {
									CConcept* opConcept = opLinkerIt->getData();
									CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
									if (succConSet) {
										if (!succConSet->containsConcept(opConcept,false)) {
											return true;
										}
									}
									opLinkerIt = opLinkerIt->getNext();
								}
							}
						}
					}
					return true;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
