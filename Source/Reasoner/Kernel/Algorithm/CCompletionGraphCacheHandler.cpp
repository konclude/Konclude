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

#include "CCompletionGraphCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CCompletionGraphCacheHandler::CCompletionGraphCacheHandler(CIndividualNodeBackendCacheHandler* backendCacheHandler) {
					mLastOntology = nullptr;
					mLastLoadedCachedCompletionGraph = false;
					mPreviousConsistencyCachedCompletionGraph = false;
					mLastConfig = nullptr;
					mBackendCacheHandler = backendCacheHandler;
				}


				CCompletionGraphCacheHandler::~CCompletionGraphCacheHandler() {
				}



				bool CCompletionGraphCacheHandler::loadConsistenceModelData(CCalculationAlgorithmContext* calcAlgContext) {
					CConcreteOntology* ontology = calcAlgContext->getProcessingDataBox()->getOntology();
					mCalcTask = calcAlgContext->getUsedSatisfiableCalculationTask();
					mIndiProcNodeVec = mCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector();
					CConfiguration* config = calcAlgContext->getSatisfiableCalculationTask()->getCalculationConfiguration();
					if (mLastOntology != ontology || mLastConfig != config) {
						mLastOntology = ontology;
						mLastConfig = config;
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
					bool addedReactivationIndi = false;
					if (loadConsistenceModelData(calcAlgContext)) {
						cint64 indiID = individualNode->getIndividualNodeID();
						CIndividualProcessNode* compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(indiID);
						CIndividualProcessNode* detCompGraphCachedIndiNode = mDetSatProcNodeVec->getData(indiID);

						if (compGraphCachedIndiNode && compGraphCachedIndiNode != detCompGraphCachedIndiNode) {
							cint64 detSatIndiCount = mDetSatProcNodeVec->getItemMaxIndex();

							CPROCESSINGLIST<cint64> reactivateComplGraphCachedIndiList(calcAlgContext->getTaskProcessorContext());
							CPROCESSINGSET<cint64> reactivateComplGraphCachedIndiSet(calcAlgContext->getTaskProcessorContext());

							CIndividualMergingHash* mergingHash = compGraphCachedIndiNode->getIndividualMergingHash(false);
							if (mergingHash) {
								for (CIndividualMergingHash::const_iterator it = mergingHash->constBegin(), itEnd = mergingHash->constEnd(); it != itEnd; ++it) {
									if (it.value().isMergedWithIndividual()) {
										cint64 mergedIndiID = it.key();
										CIndividualProcessNode* compGraphCachedMergedIndiNode = mCompGraphCachedProcNodeVec->getData(-mergedIndiID);
										CIndividualProcessNode* detSatMergedIndiNode = nullptr;
										if (-mergedIndiID <= detSatIndiCount) {
											detSatMergedIndiNode = mDetSatProcNodeVec->getData(-mergedIndiID);
										}
										if (compGraphCachedMergedIndiNode != detSatMergedIndiNode && detSatMergedIndiNode) {
											addedReactivationIndi |= reactProcQueue->insertReactivationIndiviudal(detSatMergedIndiNode, true);
										}
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
						cint64 detSatIndiCount = mDetSatProcNodeVec->getItemMaxIndex();
						cint64 indiID = individualNode->getIndividualNodeID();
						if (indiID <= detSatIndiCount) {
							CIndividualProcessNode* detSatIndiNode = mDetSatProcNodeVec->getData(indiID);
							if (detSatIndiNode) {
								CIndividualProcessNode* compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(indiID);
								while (compGraphCachedIndiNode->getMergedIntoIndividualNodeID() != compGraphCachedIndiNode->getIndividualNodeID()) {
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


				bool CCompletionGraphCacheHandler::isIndividualNodeCompletionGraphConsistencePresentNonBlocked(CIndividualProcessNode* individualNode, CCalculationAlgorithmContext* calcAlgContext) {
					if (loadConsistenceModelData(calcAlgContext)) {
						if (mIncrementalExpansionCaching) {
							if (individualNode->getIncrementalExpansionID() >= mCurrentIncrementalExpansionID) {
								return false;
							}
						}
						cint64 nonDetSatIndiCount = mCompGraphCachedProcNodeVec->getItemMaxIndex();
						cint64 indiID = individualNode->getIndividualNodeID();
						if (indiID <= nonDetSatIndiCount) {
							CIndividualProcessNode* compGraphCachedIndiNode = mCompGraphCachedProcNodeVec->getData(indiID);
							if (compGraphCachedIndiNode) {
								bool blocked = compGraphCachedIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFDIRECTBLOCKED | CIndividualProcessNode::PRFINDIRECTBLOCKED | CIndividualProcessNode::PRFPROCESSINGBLOCKED);
								return !blocked;
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

					CConceptRepresentativePropagationSetHash* compGraphCachedVarRepBindSetHash = compGraphCachedIndiNode->getConceptRepresentativePropagationSetHash(false);
					CConceptPropagationBindingSetHash* compGraphCachedPropBindSetHash = compGraphCachedIndiNode->getConceptPropagationBindingSetHash(false);
					CConceptVariableBindingPathSetHash* compGraphCachedVarBindSetHash = compGraphCachedIndiNode->getConceptVariableBindingPathSetHash(false);


					CConceptRepresentativePropagationSetHash* indiVarRepBindSetHash = individualNode->getConceptRepresentativePropagationSetHash(false);
					CConceptPropagationBindingSetHash* indiPropBindSetHash = individualNode->getConceptPropagationBindingSetHash(false);
					CConceptVariableBindingPathSetHash* indiVarBindSetHash = individualNode->getConceptVariableBindingPathSetHash(false);

					if (indiVarRepBindSetHash || indiPropBindSetHash || indiVarRepBindSetHash) {
						CReapplyConceptLabelSet* subConceptSet = individualNode->getReapplyConceptLabelSet(false);
						CReapplyConceptLabelSetIterator subConSetIt = subConceptSet->getConceptLabelSetIterator(true, false, false);
						while (subConSetIt.hasValue()) {
							CConceptDescriptor* subConDes = subConSetIt.getConceptDescriptor();
							CConcept* concept = subConDes->getConcept();

							CConceptOperator* conOperator = concept->getConceptOperator();
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_PROPAGATION_BIND_TYPE)) {
								if (indiPropBindSetHash) {
									CPropagationBindingSet* indiPropBindSet = indiPropBindSetHash->getPropagationBindingSet(concept, false);
									if (indiPropBindSet) {
										CPropagationBindingMap* indiPropBindMap = indiPropBindSet->getPropagationBindingMap();
										if (!indiPropBindMap->empty()) {
											if (!compGraphCachedPropBindSetHash) {
												return false;
											}
											CPropagationBindingSet* compGraphCachedPropBindSet = compGraphCachedPropBindSetHash->getPropagationBindingSet(concept, false);
											if (!compGraphCachedPropBindSet) {
												return false;
											}
											CPropagationBindingMap* compGraphCachedPropBindMap = compGraphCachedPropBindSet->getPropagationBindingMap();

											if (!indiPropBindMap->isKeySubSetOf(compGraphCachedPropBindMap)) {
												return false;
											}
										}
									}
								}
							}
							if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_VARIABLE_BIND_TYPE)) {
								if (indiVarBindSetHash) {
									CVariableBindingPathSet* indiVarBindSet = indiVarBindSetHash->getVariableBindingPathSet(concept, false);
									if (indiVarBindSet) {
										CVariableBindingPathMap* indiVarBindMap = indiVarBindSet->getVariableBindingPathMap();
										if (!indiVarBindMap->empty()) {
											if (!compGraphCachedVarBindSetHash) {
												return false;
											}
											CVariableBindingPathSet* compGraphCachedVarBindSet = compGraphCachedVarBindSetHash->getVariableBindingPathSet(concept, false);
											if (!compGraphCachedVarBindSet) {
												return false;
											}
											CVariableBindingPathMap* compGraphCachedVarBindMap = compGraphCachedVarBindSet->getVariableBindingPathMap();

											if (!indiVarBindMap->isKeySubSetOf(compGraphCachedVarBindMap)) {
												return false;
											}
										}
									}
								}
								if (indiVarRepBindSetHash) {
									CRepresentativePropagationSet* indiVarRepBindSet = indiVarRepBindSetHash->getRepresentativePropagationSet(concept, false);
									if (indiVarRepBindSet) {
										CRepresentativePropagationMap* indiVarRepBindMap = indiVarRepBindSet->getRepresentativePropagationMap();
										if (!indiVarRepBindMap->empty()) {
											if (!compGraphCachedVarRepBindSetHash) {
												return false;
											}
											CRepresentativePropagationSet* compGraphCachedVarRepBindSet = compGraphCachedVarRepBindSetHash->getRepresentativePropagationSet(concept, false);
											if (!compGraphCachedVarRepBindSet) {
												return false;
											}
											CRepresentativePropagationMap* compGraphCachedVarRepBindMap = compGraphCachedVarRepBindSet->getRepresentativePropagationMap();

											if (!indiVarRepBindMap->isKeySubSetOf(compGraphCachedVarRepBindMap)) {
												return false;
											}
										}
									}
								}
							}

							subConSetIt.moveNext();
						}
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
						if (!conNegation && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) || conNegation && opCode == CCSOME || !conNegation && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE))) {
							return testCriticalB2AutomateTransitionOperands(individualNode,detSatIndiNode,compGraphCachedIndiNode,concept, conNegation,calcAlgContext);
						}
					}
					if (!conNegation && opCode == CCATMOST || conNegation && opCode == CCATLEAST) {
						// check cardinality
						cint64 cardinality = concept->getParameter() - 1*conNegation;

						bool nominalIndiNode = individualNode->isNominalIndividualNode();

						cint64 updatedCardinality = 0;
						CRoleSuccessorLinkIterator succLinkIt = individualNode->getRoleSuccessorLinkIterator(role);
						while (updatedCardinality <= cardinality && succLinkIt.hasNext()) {
							CIndividualLinkEdge* link = succLinkIt.next();
							CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
							CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
							if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
								succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
							}
							if (nominalIndiNode) {
								if (link->getCreatorIndividualID() != individualNode->getIndividualNodeID()) {
									if (succIndi->isBlockableIndividual()) {
										// requires NN-rule
										return true;
									}
								}
							}
							if (succIndi->getLocalizationTag() > mDetLocalizationTag) {
								if (succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
									bool containsSomeOps = true;
									if (opConLinkerIt) {
										containsSomeOps = false;
										while (opConLinkerIt && !containsSomeOps) {
											bool opNegated = opConLinkerIt->isNegated() ^ conNegation;
											CConcept* opCon = opConLinkerIt->getData();
											bool containsNegated = false;
											CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
											if (succConSet) {
												if (succConSet->containsConcept(opCon, &containsNegated)) {
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
											CIndividualProcessNode* corrCachedSuccIndi = mCompGraphCachedProcNodeVec->getData(succIndi->getIndividualNodeID());
											if (!corrCachedSuccIndi || corrCachedSuccIndi->getMergedIntoIndividualNodeID() != succIndi->getIndividualNodeID() || corrCachedSuccIndi->getLastMergedIntoIndividualNode() != succIndi->getLastMergedIntoIndividualNode()) {
												++updatedCardinality;
											}
										}
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
							CRoleSuccessorLinkIterator succLinkIt = cachedSuccHash->getRoleSuccessorLinkIterator(role, &linkCount);

							// check whether some links are lazily expanded from backend cache
							if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKENNEIGHBOURDPARTIALEXPANSION)) {

								visitMergedIndividualsBackendSynchronisationData(individualNode, [&](CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* mergedDataIndiNode) -> bool {
									CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* mergedBackendSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)mergedDataIndiNode->getIndividualBackendCacheSynchronisationData(false);
									if (mergedBackendSyncData) {
										CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedAssocData = mergedBackendSyncData->getAssocitaionData();
										mBackendCacheHandler->visitNeighbourIndividualIdsForRole(mergedAssocData, role, [&](cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)->bool {

											CIndividualProcessNode* neighbourIndiNode = mIndiProcNodeVec->getData(-neighbourIndividualId);
											while (neighbourIndiNode->getMergedIntoIndividualNodeID() != neighbourIndiNode->getIndividualNodeID()) {
												neighbourIndiNode = mIndiProcNodeVec->getData(neighbourIndiNode->getMergedIntoIndividualNodeID());
											}

											if (!neighbourIndiNode || neighbourIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {

												CIndividualProcessNode* neighbourCompCachedIndiNode = mCompGraphCachedProcNodeVec->getData(-neighbourIndividualId);
												while (neighbourCompCachedIndiNode && neighbourCompCachedIndiNode->getMergedIntoIndividualNodeID() != neighbourCompCachedIndiNode->getIndividualNodeID()) {
													neighbourCompCachedIndiNode = mCompGraphCachedProcNodeVec->getData(neighbourCompCachedIndiNode->getMergedIntoIndividualNodeID());
												}

												if (neighbourCompCachedIndiNode && compGraphCachedIndiNode->hasRoleSuccessorToIndividual(role, neighbourCompCachedIndiNode, false)) {

													bool containsSomeOps = true;
													CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
													if (opConLinkerIt) {
														containsSomeOps = false;
														while (opConLinkerIt && !containsSomeOps) {
															bool opNegated = opConLinkerIt->isNegated();
															CConcept* opCon = opConLinkerIt->getData();
															CReapplyConceptLabelSet* succConSet = neighbourCompCachedIndiNode->getReapplyConceptLabelSet(false);
															if (succConSet && succConSet->containsConcept(opCon, opNegated)) {
																containsSomeOps = true;
															}
															opConLinkerIt = opConLinkerIt->getNext();
														}
													}
													if (containsSomeOps && !neighbourIndiNode || !individualNode->hasRoleSuccessorToIndividual(role, neighbourIndiNode, false)) {
														++updatedCardinality;
													}

												}
											}

											return updatedCardinality <= cardinality;
										}, true, calcAlgContext);
									}

									return updatedCardinality <= cardinality;
								}, calcAlgContext);


								if (updatedCardinality > cardinality) {
									return true;
								}
							}

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







				bool CCompletionGraphCacheHandler::visitMergedIndividualsBackendSynchronisationData(CIndividualProcessNode* indiNode, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* mergedDataIndiNode)> visitFunc, CCalculationAlgorithmContext* calcAlgContext) {
					bool visited = false;
					bool continueVisiting = true;
					CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backendSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)indiNode->getIndividualBackendCacheSynchronisationData(false);
					if (backendSyncData) {
						continueVisiting = visitFunc(indiNode, indiNode);
						visited = true;
					}
					CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData = backendSyncData->getAssocitaionData();
					CBackendRepresentativeMemoryLabelCacheItem* detSameIndiLabel = assocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					CIndividualMergingHash* mergingHash = indiNode->getIndividualMergingHash(false);
					if (mergingHash) {
						for (CXLinker<cint64>* newIndiMergedLinkerIt = mergingHash->getMergedIndividualLinker(); newIndiMergedLinkerIt && continueVisiting; newIndiMergedLinkerIt = newIndiMergedLinkerIt->getNext()) {
							cint64 newIndiMergedId = newIndiMergedLinkerIt->getData();

							if (!mBackendCacheHandler->hasIndividualIdsInAssociatedIndividualSetLabel(assocData, detSameIndiLabel, newIndiMergedId)) {

								CIndividualProcessNode* backendSyncDataIndiNode = mIndiProcNodeVec->getData(-newIndiMergedId);

								if (backendSyncDataIndiNode->getIndividualBackendCacheSynchronisationData(false)) {
									continueVisiting = visitFunc(indiNode, backendSyncDataIndiNode);
									visited = true;
								}
							}
						}
					}
					return visited;
				}




				bool CCompletionGraphCacheHandler::testCriticalB2AutomateTransitionOperands(CIndividualProcessNode* individualNode, CIndividualProcessNode* detSatIndiNode, CIndividualProcessNode* compGraphCachedIndiNode, CConcept* concept, bool conNegation, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 opCode = concept->getOperatorCode();
					CConceptOperator* conOperator = concept->getConceptOperator();
					if (!conNegation && conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_AQAND_TYPE)) {
						// recursive
						CSortedNegLinker<CConcept*>* opLinkerIt = concept->getOperandList();
						while (opLinkerIt) {
							CConcept* opConcept = opLinkerIt->getData();
							if (testCriticalB2AutomateTransitionOperands(individualNode,detSatIndiNode,compGraphCachedIndiNode,opConcept, opLinkerIt->isNegated() ^ conNegation,calcAlgContext)) {
								return true;
							}
							opLinkerIt = opLinkerIt->getNext();
						}
					} else if (!conNegation && (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCFS_ALL_AQALL_TYPE)) || conNegation && opCode == CCSOME) {
						CRole* conRole = concept->getRole();
						CRoleSuccessorLinkIterator succLinkIt = individualNode->getRoleSuccessorLinkIterator(conRole);
						while (succLinkIt.hasNext()) {
							CIndividualLinkEdge* link = succLinkIt.next();
							CIndividualProcessNode* succIndi = link->getOppositeIndividual(individualNode);
							if (link->isLocalizationTagUpdated(calcAlgContext->getUsedProcessTagger())) {
								succIndi = mIndiProcNodeVec->getData(link->getOppositeIndividualID(individualNode));
							}
							if (succIndi->getLocalizationTag() > mDetLocalizationTag && !succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)
								|| succIndi->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {
								return checkPropagatedConceptsMissing(concept, conNegation, succIndi, conOperator, compGraphCachedIndiNode);
							}

						}



						if (individualNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKENNEIGHBOURDPARTIALEXPANSION)) {

							bool criticalMissingLink = false;
							visitMergedIndividualsBackendSynchronisationData(individualNode, [&](CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* mergedDataIndiNode) -> bool {
								CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* mergedBackendSyncData = (CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData*)mergedDataIndiNode->getIndividualBackendCacheSynchronisationData(false);
								if (mergedBackendSyncData) {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* mergedAssocData = mergedBackendSyncData->getAssocitaionData();
									mBackendCacheHandler->visitNeighbourIndividualIdsForRole(mergedAssocData, conRole, [&](cint64 neighbourIndividualId, CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel, bool nondeterministic)->bool {


										CIndividualProcessNode* neighbourIndiNode = mIndiProcNodeVec->getData(-neighbourIndividualId);
										while (neighbourIndiNode && neighbourIndiNode->getMergedIntoIndividualNodeID() != neighbourIndiNode->getIndividualNodeID()) {
											neighbourIndiNode = mIndiProcNodeVec->getData(neighbourIndiNode->getMergedIntoIndividualNodeID());
										}


										if (!neighbourIndiNode || neighbourIndiNode->hasPartialProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED | CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID)) {

											CIndividualProcessNode* neighbourCompCachedIndiNode = mCompGraphCachedProcNodeVec->getData(-neighbourIndividualId);
											while (neighbourCompCachedIndiNode && neighbourCompCachedIndiNode->getMergedIntoIndividualNodeID() != neighbourCompCachedIndiNode->getIndividualNodeID()) {
												neighbourCompCachedIndiNode = mCompGraphCachedProcNodeVec->getData(neighbourCompCachedIndiNode->getMergedIntoIndividualNodeID());
											}

											if (neighbourCompCachedIndiNode && compGraphCachedIndiNode->hasRoleSuccessorToIndividual(conRole, neighbourCompCachedIndiNode, false)) {

												if (!neighbourIndiNode) {
													criticalMissingLink = true;
												} else if (!individualNode->hasRoleSuccessorToIndividual(conRole, neighbourIndiNode, false)) {
													if (checkPropagatedConceptsMissing(concept, conNegation, neighbourIndiNode, conOperator, neighbourCompCachedIndiNode)) {
														criticalMissingLink = true;
													}
												}


											}
										}

										return !criticalMissingLink;
									}, true, calcAlgContext);
								}

								return !criticalMissingLink;
							}, calcAlgContext);


							if (criticalMissingLink) {
								return true;
							}
						}
					}
					return true;
				}


				bool CCompletionGraphCacheHandler::checkPropagatedConceptsMissing(CConcept* concept, bool conNegation, CIndividualProcessNode* succIndi, CConceptOperator* conOperator, CIndividualProcessNode* compGraphCachedIndiNode) {
					CSortedNegLinker<CConcept*>* opConLinkerIt = concept->getOperandList();
					while (opConLinkerIt) {
						bool opNegated = opConLinkerIt->isNegated() ^ conNegation;
						CConcept* opCon = opConLinkerIt->getData();
						CReapplyConceptLabelSet* succConSet = succIndi->getReapplyConceptLabelSet(false);
						if (!succConSet || !succConSet->containsConcept(opCon, opNegated)) {
							return true;
						}
						if (conOperator->hasPartialOperatorCodeFlag(CConceptOperator::CCF_PBINDAQALL | CConceptOperator::CCF_PBINDALL)) {
							CConceptPropagationBindingSetHash* compGraphCachedPropBindSetHash = compGraphCachedIndiNode->getConceptPropagationBindingSetHash(false);
							if (compGraphCachedPropBindSetHash) {
								CPropagationBindingSet* compGraphCachedPropBindSet = compGraphCachedPropBindSetHash->getPropagationBindingSet(concept, false);
								if (compGraphCachedPropBindSet) {
									CPropagationBindingMap* compGraphCachedPropBindMap = compGraphCachedPropBindSet->getPropagationBindingMap();
									if (!compGraphCachedPropBindMap->empty()) {
										CConceptPropagationBindingSetHash* succPropBindSetHash = succIndi->getConceptPropagationBindingSetHash(false);
										if (!succPropBindSetHash) {
											return true;
										}
										CPropagationBindingSet* succPropBindSet = succPropBindSetHash->getPropagationBindingSet(opCon, false);
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
								CVariableBindingPathSet* compGraphCachedVarBindSet = compGraphCachedVarBindSetHash->getVariableBindingPathSet(concept, false);
								if (compGraphCachedVarBindSet) {
									CVariableBindingPathMap* compGraphCachedVarBindMap = compGraphCachedVarBindSet->getVariableBindingPathMap();
									if (!compGraphCachedVarBindMap->empty()) {
										CConceptVariableBindingPathSetHash* succVarBindSetHash = succIndi->getConceptVariableBindingPathSetHash(false);
										if (!succVarBindSetHash) {
											return true;
										}
										CVariableBindingPathSet* succVarBindSet = succVarBindSetHash->getVariableBindingPathSet(opCon, false);
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
								CRepresentativePropagationSet* compGraphCachedVarRepBindSet = compGraphCachedVarRepBindSetHash->getRepresentativePropagationSet(concept, false);
								if (compGraphCachedVarRepBindSet) {
									CRepresentativePropagationMap* compGraphCachedVarRepBindMap = compGraphCachedVarRepBindSet->getRepresentativePropagationMap();
									if (!compGraphCachedVarRepBindMap->empty()) {
										CConceptRepresentativePropagationSetHash* succVarRepBindSetHash = succIndi->getConceptRepresentativePropagationSetHash(false);
										if (!succVarRepBindSetHash) {
											return true;
										}
										CRepresentativePropagationSet* succVarRepBindSet = succVarRepBindSetHash->getRepresentativePropagationSet(opCon, false);
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
					return false;
				}

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
