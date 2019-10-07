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

#include "CSaturationNodeExpansionCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSaturationNodeExpansionCacheHandler::CSaturationNodeExpansionCacheHandler(CSaturationNodeAssociatedExpansionCacheReader* satCacheReader, CSaturationNodeAssociatedExpansionCacheWriter* satCacheWriter) {
					mSatCacheReader = satCacheReader;
					mSatCacheWriter = satCacheWriter;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;
				}



				CIndividualSaturationProcessNode* CSaturationNodeExpansionCacheHandler::getSaturationIndividualNodeForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {

					CConceptData* conceptData = concept->getConceptData();
					CIndividualSaturationProcessNode* saturationIndiNode = nullptr;
					if (conceptData) {
						CConceptProcessData* conProcData = (CConceptProcessData*)conceptData;
						CConceptReferenceLinking* conRefLinking = conProcData->getConceptReferenceLinking();
						if (conRefLinking) {
							CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conRefLinking;
							CSaturationConceptReferenceLinking* satCalcRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(false);
							if (satCalcRefLinkData) {
								saturationIndiNode = (CIndividualSaturationProcessNode*)satCalcRefLinkData->getIndividualProcessNodeForConcept();
							}
						}
					}
					return saturationIndiNode;
				}





				bool CSaturationNodeExpansionCacheHandler::cacheUnsatisfiableConcept(CConcept* concept, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualSaturationProcessNode* saturationIndiNode = getSaturationIndividualNodeForConcept(concept,false,calcAlgContext);

					if (saturationIndiNode && !saturationIndiNode->getIndirectStatusFlags()->hasClashedFlag()) {
						// try caching
						prepareCacheMessages(calcAlgContext);
						CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
						CContextBase* tmpContext = mTmpContext;

						CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData* unsatWriteData = CObjectAllocator< CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData >::allocateAndConstruct(satCacheMemMan);
						unsatWriteData->initUnsatisfiabilityWriteData(saturationIndiNode);
						addCacheMessages(unsatWriteData,calcAlgContext);
						return true;
					}

					return false;				
				}



				bool CSaturationNodeExpansionCacheHandler::isConceptUnsatisfiableCached(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualSaturationProcessNode* saturationIndiNode = getSaturationIndividualNodeForConcept(concept,negation,calcAlgContext);
					if (saturationIndiNode) {
						return saturationIndiNode->getIndirectStatusFlags()->hasClashedFlag();
					}
					return false;
				}


				bool CSaturationNodeExpansionCacheHandler::isNodeSatisfiableCached(CIndividualProcessNode* individualProcessNode, CSaturationNodeAssociatedConceptExpansion*& expansion, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualNodeSaturationBlockingData* satBlockData = individualProcessNode->getIndividualSaturationBlockingData(false);
					CReapplyConceptLabelSet* conSet = individualProcessNode->getReapplyConceptLabelSet(false);
					if (conSet && satBlockData) {
						cint64 totalConceptCount = conSet->getConceptCount();
						CConceptDescriptor* lastAddedConDes = conSet->getAddingSortedConceptDescriptionLinker();


						CIndividualSaturationProcessNode* saturationNode = satBlockData->getSaturationIndividualNode();


						if (satBlockData->getLastConfirmedConceptDescriptior() == lastAddedConDes) {
							CIndividualSaturationProcessNodeStatusFlags* flags = saturationNode->getIndirectStatusFlags();
							if (!flags->hasFlags(CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGINSUFFICIENT | CIndividualSaturationProcessNodeStatusFlags::INDSATFLAGCLASHED,false)) {
								return true;
							}
						}


						CReapplyConceptSaturationLabelSet* satConSet = saturationNode->getReapplyConceptSaturationLabelSet(false);

						CSaturationNodeAssociatedExpansionCacheEntry* cacheEntry = mSatCacheReader->getCacheEntry(saturationNode);
						if (cacheEntry) {
							CConceptDescriptor* lastConDes = satBlockData->getLastConfirmedConceptDescriptior();
							CConceptDescriptor* conDes = conSet->getAddingSortedConceptDescriptionLinker();
							cint64 conSetSignature = conSet->getConceptSignature()->getSignatureValue();


							CSaturationNodeAssociatedDeterministicConceptExpansion* detExpansion = cacheEntry->getDeterministicConceptExpansion();
							if (detExpansion && !detExpansion->requiresNonDeterministicExpansion()) {
								if (testNodeMatchingExpansion(detExpansion,nullptr,conSet,satBlockData,calcAlgContext)) {
									expansion = detExpansion;
									return true;
								}
							}
							CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetExpansionLinker = cacheEntry->getNondeterministicConceptExpansionLinker();
							for (CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetExpansionLinkerIt = nondetExpansionLinker; nondetExpansionLinkerIt; nondetExpansionLinkerIt = nondetExpansionLinkerIt->getNext()) {
								CSaturationNodeAssociatedNondeterministicConceptExpansion* nondetExpansion = nondetExpansionLinkerIt;
								if (testNodeMatchingExpansion(nondetExpansion,detExpansion,conSet,satBlockData,calcAlgContext)) {
									expansion = nondetExpansion;
									return true;
								}
							}
						}


						
					}
					return false;
				}




				bool CSaturationNodeExpansionCacheHandler::getCachedDeterministicExpansion(CIndividualSaturationProcessNode* saturationNode, CSaturationNodeAssociatedDeterministicConceptExpansion*& expansion, CCalculationAlgorithmContext* calcAlgContext) {
					CSaturationNodeAssociatedExpansionCacheEntry* cacheEntry = mSatCacheReader->getCacheEntry(saturationNode);
					if (cacheEntry) {
						CSaturationNodeAssociatedDeterministicConceptExpansion* detExpansion = cacheEntry->getDeterministicConceptExpansion();
						if (detExpansion) {
							expansion = detExpansion;
							return true;
						}
					}
					return false;
				}



				bool CSaturationNodeExpansionCacheHandler::testNodeMatchingExpansion(CSaturationNodeAssociatedConceptExpansion* expansion, CSaturationNodeAssociatedConceptExpansion* alternativeExpansion, CReapplyConceptLabelSet* conSet, CIndividualNodeSaturationBlockingData* satBlockData, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 conSetSignature = conSet->getConceptSignature()->getSignatureValue();
					if (expansion->getConceptSetSignature() != conSetSignature) {
						return false;
					}
					cint64 totalConceptCount = conSet->getConceptCount();
					if (expansion->getTotalConceptCount() != totalConceptCount) {
						return false;
					}
					CConceptDescriptor* lastConDes = satBlockData->getLastConfirmedConceptDescriptior();
					CConceptDescriptor* conDes = conSet->getAddingSortedConceptDescriptionLinker();

					for (CConceptDescriptor* conDesIt = conDes; conDesIt != lastConDes; conDesIt = conDesIt->getNext()) {
						CCacheValue cacheValue(getCacheValueForConcept(conDesIt,calcAlgContext));
						if (!expansion->hasConceptExpansionLinker(&cacheValue)) {
							if (!alternativeExpansion || !alternativeExpansion->hasConceptExpansionLinker(&cacheValue)) {
								return false;
							}
						}
					}

					CSaturationNodeAssociatedDependentNominalSet* dependentNominalSet = expansion->getDependentNominalSet(false);
					if (dependentNominalSet) {
						CIndividualProcessNodeVector* indiVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
						for (CSaturationNodeAssociatedDependentNominalSet::const_iterator it = dependentNominalSet->constBegin(), itEnd = dependentNominalSet->constEnd(); it != itEnd; ++it) {
							cint64 dependentNominalID = -(*it);
							bool nominalStillCached = false;
							CIndividualProcessNode* indiNode = indiVec->getData(dependentNominalID);
							if (indiNode) {
								if (dependentNominalID <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID() && indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
									if (!indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID) && !indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
										nominalStillCached = true;
									}
								} else if (indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKEND)) {
									nominalStillCached = true;
								}
							} else {
								nominalStillCached = true;
							}
							if (!nominalStillCached) {
								return false;
							}
						}
					}

					return true;
				}


				CCacheValue CSaturationNodeExpansionCacheHandler::getCacheValueForConcept(CConceptDescriptor* conDes, CCalculationAlgorithmContext* calcAlgContext) {
					return getCacheValueForConcept(conDes->getConcept(),conDes->isNegated(),calcAlgContext);
				}



				CCacheValue CSaturationNodeExpansionCacheHandler::getCacheValueForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 conTag = concept->getConceptTag();
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = CCacheValue::CACHEVALTAGANDCONCEPT;
					if (negation) {
						cacheValueIdentifier = CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
					}
					CCacheValue cacheValue;
					cacheValue.initCacheValue(conTag,(cint64)concept,cacheValueIdentifier);
					return cacheValue;
				}



				bool CSaturationNodeExpansionCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CSaturationNodeExpansionCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CSaturationNodeAssociatedExpansionCacheWriteData* commitWriteData = nullptr;
						CSaturationNodeAssociatedExpansionCacheWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CSaturationNodeAssociatedExpansionCacheWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CSaturationNodeAssociatedExpansionCacheWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CSaturationNodeAssociatedExpansionCacheWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mSatCacheWriter->writeCacheData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CSaturationNodeExpansionCacheHandler::addCacheMessages(CSaturationNodeAssociatedExpansionCacheWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CSaturationNodeAssociatedExpansionCacheWriteData*)expandWriteData->append(mWriteData);
					} else {
						mWriteData = expandWriteData;
					}
					//commitCacheMessages(calcAlgContext);
					return true;
				}






				bool CSaturationNodeExpansionCacheHandler::testNodeCachingPossible(CIndividualProcessNode* individualProcessNode, bool& onlyIfCompletelyDeterministic, bool& onlyAllNondeterministic, CSaturationNodeAssociatedExpansionCacheEntry*& cacheEntry, CCalculationAlgorithmContext* calcAlgContext) {
					onlyIfCompletelyDeterministic = false;
					cacheEntry = nullptr;
					CIndividualNodeSaturationBlockingData* satBlockData = individualProcessNode->getIndividualSaturationBlockingData(false);
					CReapplyConceptLabelSet* conSet = individualProcessNode->getReapplyConceptLabelSet(false);
					if (conSet && satBlockData) {
						cint64 totalConceptCount = conSet->getConceptCount();
						CConceptDescriptor* lastAddedConDes = conSet->getAddingSortedConceptDescriptionLinker();

						CIndividualSaturationProcessNode* saturationNode = satBlockData->getSaturationIndividualNode();
						CReapplyConceptSaturationLabelSet* satConSet = saturationNode->getReapplyConceptSaturationLabelSet(false);

						if (!saturationNode->isCompleted()) {
							return false;
						}

						cacheEntry = mSatCacheReader->getCacheEntry(saturationNode);
						if (cacheEntry) {
							cint64 conSetSignature = conSet->getConceptSignature()->getSignatureValue();
							CSaturationNodeAssociatedDeterministicConceptExpansion* conExp = cacheEntry->getDeterministicConceptExpansion();
							if (conExp && !conExp->requiresNonDeterministicExpansion()) {
								return false;
							}
							if (!cacheEntry->areMoreNondeterministicExpansionAllowed() || conExp && conExp->getConceptSetSignature() == conSetSignature) {
								onlyIfCompletelyDeterministic = true;
							} else {
								for (CSaturationNodeAssociatedNondeterministicConceptExpansion* conExpIt = cacheEntry->getNondeterministicConceptExpansionLinker(); conExpIt; conExpIt = conExpIt->getNext()) {
									if (conExpIt->getConceptSetSignature() == conSetSignature) {
										onlyIfCompletelyDeterministic = true;
										break;
									}
								}
							}
						}

						// test compatible nominal nodes
						CSuccessorConnectedNominalSet* depNominalSet = individualProcessNode->getSuccessorNominalConnectionSet(false);
						if (depNominalSet) {
							CIndividualProcessNodeVector* indiVec = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
							for (CSuccessorConnectedNominalSet::const_iterator it = depNominalSet->constBegin(), itEnd = depNominalSet->constEnd(); it != itEnd; ++it) {
								cint64 nominalNodeID = *it;
								bool nominalStillCached = false;
								CIndividualProcessNode* indiNode = indiVec->getData(nominalNodeID);
								if (indiNode) {
									if (nominalNodeID <= calcAlgContext->getMaxCompletionGraphCachedIndividualNodeID() && indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHED)) {
										if (!indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALID) && !indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHINGINVALIDATED)) {
											nominalStillCached = true;
											if (indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFCOMPLETIONGRAPHCACHEDNODEEXTENDED)) {
												onlyAllNondeterministic = true;
											}
										}
									} else if (indiNode->hasProcessingRestrictionFlags(CIndividualProcessNode::PRFSYNCHRONIZEDBACKEND)) {
										nominalStillCached = true;
									}
								}
								if (!nominalStillCached) {
									return false;
								}
							}
						}
						return true;
					}
					return false;
				}



				bool CSaturationNodeExpansionCacheHandler::tryNodeSatisfiableCaching(CIndividualProcessNode* individualProcessNode, CCalculationAlgorithmContext* calcAlgContext) {

					// test whether node can be cached

					bool cachingOnlyIfDeterministic = false;
					bool onlyAllNondeterministic = false;
					CSaturationNodeAssociatedExpansionCacheEntry* cacheEntry = nullptr;
					if (testNodeCachingPossible(individualProcessNode,cachingOnlyIfDeterministic,onlyAllNondeterministic,cacheEntry,calcAlgContext)) {
						CIndividualNodeSaturationBlockingData* satBlockData = individualProcessNode->getIndividualSaturationBlockingData(false);
						CConceptDescriptor* lastConDes = satBlockData->getLastConfirmedConceptDescriptior();
						CIndividualSaturationProcessNode* satIndiNode = satBlockData->getSaturationIndividualNode();
						CSaturationConceptDataItem* conceptSatItem = (CSaturationConceptDataItem*)satIndiNode->getSaturationConceptReferenceLinking();;
						if (conceptSatItem) {
							CConcept* saturationConcept = conceptSatItem->getSaturationConcept();

							//if (CIRIName::getRecentIRIName(saturationConcept->getClassNameLinker()) == "http://www.owllink.org/testsuite/galen#Transport") {
							//	bool debug = true;
							//}

							CReapplyConceptLabelSet* conSet = individualProcessNode->getReapplyConceptLabelSet(false);
							cint64 totalConceptCount = conSet->getConceptCount();
							cint64 conSetSignature = conSet->getConceptSignature()->getSignatureValue();


							CConceptDescriptor* conDes = conSet->getAddingSortedConceptDescriptionLinker();
							CConceptDescriptor* lastPossiblyNonDeterministicConDes = nullptr;
							CConceptDescriptor* satConDes = nullptr;
							CDependencyTrackPoint* satConDepTrackPoint = nullptr;
							if (conSet->getConceptDescriptor(saturationConcept,satConDes,satConDepTrackPoint)) {
								for (CConceptDescriptor* conDesIt = conDes; conDesIt != lastConDes; conDesIt = conDesIt->getNext()) {
									CDependencyTrackPoint* depTrackPoint = conDesIt->getDependencyTrackPoint();
									if (onlyAllNondeterministic) {
										lastPossiblyNonDeterministicConDes = conDesIt;
									} else if (!isDeterministicallyDependingOnSaturationConcept(individualProcessNode,depTrackPoint,satConDes,calcAlgContext)) {
										lastPossiblyNonDeterministicConDes = conDesIt;
									}
								}
								if (!cachingOnlyIfDeterministic || lastPossiblyNonDeterministicConDes == nullptr) {

									prepareCacheMessages(calcAlgContext);
									CTaskMemoryPoolAllocationManager* satCacheMemMan = mMemAllocMan;
									CContextBase* tmpContext = mTmpContext;

									CSaturationNodeAssociatedExpansionCacheExpansionWriteData* detExpWriteData = nullptr;
									CSaturationNodeAssociatedExpansionCacheExpansionWriteData* nondetExpWriteData = nullptr;


									bool hasTightAtMostRestriction = false;
									for (CConceptDescriptor* conDesIt = conDes; conDesIt && !hasTightAtMostRestriction; conDesIt = conDesIt->getNext()) {
										CConcept* concept = conDesIt->getConcept();
										bool negation = conDesIt->isNegated();
										cint64 conCode = concept->getConceptOperator()->getOperatorCode();
										if (negation && conCode == CCATLEAST || !negation && conCode == CCATMOST) {
											CRole* role = concept->getRole();
											cint64 parameter = concept->getParameter();
											cint64 cardinality = parameter+1*negation;

											if (individualProcessNode->getRoleSuccessorCount(role) >= cardinality) {
												hasTightAtMostRestriction = true;
											}
										}
									}

									CSaturationNodeAssociatedDependentNominalSet* nomDepSet = nullptr;

									CSuccessorConnectedNominalSet* successorNominalConnSet = individualProcessNode->getSuccessorNominalConnectionSet(false);
									if (successorNominalConnSet) {
										nomDepSet = CObjectParameterizingAllocator< CSaturationNodeAssociatedDependentNominalSet,CContext* >::allocateAndConstructAndParameterize(satCacheMemMan,tmpContext);
										for (CSuccessorConnectedNominalSet::const_iterator it = successorNominalConnSet->constBegin(), itEnd = successorNominalConnSet->constEnd(); it != itEnd; ++it) {
											cint64 nominalID = -(*it);
											nomDepSet->insert(nominalID);
										}
									}


									if (lastPossiblyNonDeterministicConDes) {
										// create non-deterministic expansion data
										nondetExpWriteData = CObjectAllocator< CSaturationNodeAssociatedExpansionCacheExpansionWriteData >::allocateAndConstruct(satCacheMemMan);

										CSaturationNodeAssociatedConceptLinker* nonDetConceptLinker = nullptr;
										CConceptDescriptor* conDesStop = lastPossiblyNonDeterministicConDes->getNext();
										for (CConceptDescriptor* conDesIt = conDes; conDesIt != conDesStop; conDesIt = conDesIt->getNext()) {
											const CCacheValue& cacheValue(getCacheValueForConcept(conDesIt->getConcept(),conDesIt->isNegated(),calcAlgContext));
											CSaturationNodeAssociatedConceptLinker* assConceptLinker = CObjectAllocator< CSaturationNodeAssociatedConceptLinker >::allocateAndConstruct(satCacheMemMan);
											assConceptLinker->initConceptLinker(cacheValue);
											nonDetConceptLinker = assConceptLinker->append(nonDetConceptLinker);
										}

										nondetExpWriteData->initExpansionWriteData(satIndiNode,nonDetConceptLinker);
										nondetExpWriteData->setDeterministicExpansion(false);
										nondetExpWriteData->setTightAtMostRestriction(hasTightAtMostRestriction);
										nondetExpWriteData->setConceptSetSignature(conSetSignature);
										nondetExpWriteData->setTotalConceptCount(totalConceptCount);
										nondetExpWriteData->setDependentNominalSet(nomDepSet);

									}


									if (!lastPossiblyNonDeterministicConDes || lastPossiblyNonDeterministicConDes->getNext() != lastConDes) {

										CSaturationNodeAssociatedDeterministicConceptExpansion* detConExp = nullptr;
										if (cacheEntry) {
											detConExp = cacheEntry->getDeterministicConceptExpansion();
										}
										CSaturationNodeAssociatedConceptLinker* detConceptLinker = nullptr;

										CConceptDescriptor* conDesStart = nullptr;
										if (lastPossiblyNonDeterministicConDes) {
											conDesStart = lastPossiblyNonDeterministicConDes->getNext();
										}
										if (!conDesStart) {
											conDesStart = conDes;
										}
										for (CConceptDescriptor* conDesIt = conDesStart; conDesIt != lastConDes; conDesIt = conDesIt->getNext()) {
											CCacheValue cacheValue(getCacheValueForConcept(conDesIt->getConcept(),conDesIt->isNegated(),calcAlgContext));
											if (!detConExp || !detConExp->hasConceptExpansionLinker(&cacheValue)) {
												CSaturationNodeAssociatedConceptLinker* assConceptLinker = CObjectAllocator< CSaturationNodeAssociatedConceptLinker >::allocateAndConstruct(satCacheMemMan);
												assConceptLinker->initConceptLinker(cacheValue);
												detConceptLinker = assConceptLinker->append(detConceptLinker);
											}
										}

										if (detConceptLinker || !detConExp || detConExp->requiresNonDeterministicExpansion() && !detConceptLinker) {
											detExpWriteData = CObjectAllocator< CSaturationNodeAssociatedExpansionCacheExpansionWriteData >::allocateAndConstruct(satCacheMemMan);
											detExpWriteData->initExpansionWriteData(satIndiNode,detConceptLinker);
											detExpWriteData->setDeterministicExpansion(true);
											if (!lastPossiblyNonDeterministicConDes) {
												detExpWriteData->setRequiresNondeterministicExpansion(false);
												detExpWriteData->setTightAtMostRestriction(hasTightAtMostRestriction);
												detExpWriteData->setConceptSetSignature(conSetSignature);
												detExpWriteData->setTotalConceptCount(totalConceptCount);
												detExpWriteData->setDependentNominalSet(nomDepSet);
											}
										}
									}


									bool wroteCacheData = false;
									if (nondetExpWriteData) {
										wroteCacheData = true;
										addCacheMessages(nondetExpWriteData,calcAlgContext);
									}
									if (detExpWriteData) {
										wroteCacheData = true;
										addCacheMessages(detExpWriteData,calcAlgContext);
									}
									return wroteCacheData;

								}
							}
						}
					}


					return false;
				}






				bool CSaturationNodeExpansionCacheHandler::isDeterministicallyDependingOnSaturationConcept(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, CConceptDescriptor* saturationConceptDescriptor, CCalculationAlgorithmContext* calcAlgContext) {
					CDependencyTrackPoint* satConDepTrackPoint = saturationConceptDescriptor->getDependencyTrackPoint();
					if (!satConDepTrackPoint || !depTrackPoint) {
						return false;
					} else {
						if (satConDepTrackPoint->getBranchingTag() != depTrackPoint->getBranchingTag()) {
							return false;
						}
					}

					cint64 ancDepth = individualNode->getIndividualAncestorDepth();
					if (ancDepth <= 0) {
						if (depTrackPoint->isPointingToIndependentDependencyNode()) {
							return false;
						}
					} else {
						CDependencyNode* depNode = depTrackPoint->getDependencyNode();
						if (depNode->hasAppropriateIndividualNode()) {
							CIndividualProcessNode* appIndiNode = depNode->getAppropriateIndividualNode();
							cint64 appIndiAncDepth = appIndiNode->getIndividualAncestorDepth();
							if (appIndiAncDepth < ancDepth) {
								return false;
							}
							//if (!appIndiNode->isNominalIndividual()) {
							//}
						} else {
							CDependencyNode::DEPENDENCNODEYTYPE depType = depNode->getDependencyType();
							if (depType == CDependencyNode::DNTMERGEDCONCEPT) {
								return false;
							}
						}
					}
					return true;
				}






			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
