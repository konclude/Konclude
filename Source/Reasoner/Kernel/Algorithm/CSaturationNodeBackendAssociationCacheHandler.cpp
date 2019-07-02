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

#include "CSaturationNodeBackendAssociationCacheHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {

				CSaturationNodeBackendAssociationCacheHandler::CSaturationNodeBackendAssociationCacheHandler(CBackendRepresentativeMemoryCacheReader* backAssCacheReader, CBackendRepresentativeMemoryCacheWriter* backAssCacheWriter) {
					mAssBackCacheReader = backAssCacheReader;
					mAssBackCacheWriter = backAssCacheWriter;

					mStatCreatedTempLabels = 0;
					mStatReusedTempLabels = 0;
					mStatReusedCachedLabels = 0;
					mStatReusedHashedIndiLabels = 0;
					mStatReusedHashedSignatureLabels = 0;

					mTmpContext = nullptr;
					mMemAllocMan = nullptr;
					mWriteData = nullptr;

					mConfDirectCardinalityInsufficient = false;
				}




				CBackendRepresentativeMemoryCacheIndividualAssociationData* CSaturationNodeBackendAssociationCacheHandler::getIndividualAssociation(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext) {
					return mAssBackCacheReader->getIndividualAssociation(individual);
				}



				bool CSaturationNodeBackendAssociationCacheHandler::prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (!mMemAllocMan) {
						CTaskMemoryPoolAllocationManager satCacheMemManCreaterMemMan(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mMemAllocMan = CObjectAllocator<CTaskMemoryPoolAllocationManager>::allocateAndConstruct(&satCacheMemManCreaterMemMan);
						mMemAllocMan->initTaskMemoryPoolAllocationManager(&mMemPoolCon,calcAlgContext->getTemporaryMemoryAllocationManager());
						mTmpContext = CObjectParameterizingAllocator< CContextBase,CMemoryAllocationManager* >::allocateAndConstructAndParameterize(mMemAllocMan,mMemAllocMan);
						return true;
					}
					return false;
				}

				bool CSaturationNodeBackendAssociationCacheHandler::commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* commitWriteData = nullptr;
						CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeDataIt = mWriteData;
						while (writeDataIt) {
							CBackendRepresentativeMemoryCacheLabelAssociationWriteData* tmpWriteData = writeDataIt;
							writeDataIt = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)writeDataIt->getNext();
							tmpWriteData->clearNext();
							if (commitWriteData) {
								commitWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)tmpWriteData->append(commitWriteData);
							} else {
								commitWriteData = tmpWriteData;
							}
						}
						mAssBackCacheWriter->writeCachedData(commitWriteData,mMemPoolCon.takeMemoryPools());
						mWriteData = nullptr;
						mMemAllocMan = nullptr;
						mTmpContext = nullptr;
						return true;
					}
					return false;
				}



				bool CSaturationNodeBackendAssociationCacheHandler::addCacheMessages(CBackendRepresentativeMemoryCacheLabelAssociationWriteData* backAssWriteData, CCalculationAlgorithmContext* calcAlgContext) {
					if (mWriteData) {
						mWriteData = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)backAssWriteData->append(mWriteData);
					} else {
						mWriteData = backAssWriteData;
					}
					return true;
				}











				bool CSaturationNodeBackendAssociationCacheHandler::findAssociationBackendCardinality(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {

					CIndividualSaturationProcessNodeStatusFlags* directIndFlags = satNode->getIndirectStatusFlags();
					CIndividualSaturationProcessNodeStatusFlags* indirectIndFlags = satNode->getIndirectStatusFlags();
					CIndividualSaturationProcessNodeStatusFlags* dirFlags = satNode->getDirectStatusFlags();
					bool insufficient = indirectIndFlags->hasInsufficientFlag() || !indirectIndFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag();
					if (!insufficient && directIndFlags->hasCardinalityRestrictedFlag()) {

						if (mConfDirectCardinalityInsufficient) {
							tmpAssWriteDataLinker->setCompletelyHandled(false);
							tmpAssWriteDataLinker->setCompletelySaturated(false);
						} else {

							CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
							if (!conSet) {
								return false;
							}
							CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
							if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker()) {
								return findAssociationBackendCardinality(copiedNode,tmpAssWriteDataLinker,calcAlgContext);
							}

							QPair<CBackendRepresentativeMemoryCardinalityCacheItem*,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*>* hashedRefPair = nullptr;
							if (satNode->getNominalIndividual() == nullptr) {
								QPair<CBackendRepresentativeMemoryCardinalityCacheItem*,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*>& refPair = (*mIndiNodeRefCardHash)[satNode];
								if (refPair.first) {
									tmpAssWriteDataLinker->setReferredCardinalityData(refPair.first);
									++mStatReusedHashedIndiLabels;
									return true;
								} else if (refPair.second) {
									tmpAssWriteDataLinker->setReferredTemporaryCardinalityData(refPair.second);
									++mStatReusedHashedIndiLabels;
									return true;
								}
								hashedRefPair = &refPair;
							}

							CPROCESSHASH<CRole*,cint64>* roleFreeCardHash = CObjectParameterizingAllocator< CPROCESSHASH<CRole*,cint64>,CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

							for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
								CConcept* concept = conSatDesIt->getConcept();
								bool negation = conSatDesIt->isNegated();
								cint64 conOpCode = concept->getOperatorCode();
								if (!negation && conOpCode == CCATMOST || negation && conOpCode == CCATLEAST) {
									CRole* role = concept->getRole();
									cint64 cardinality = concept->getParameter();
									if (negation) {
										cardinality -= 1;
									}
									cint64 freeCardinality = 0;

									CLinkedRoleSaturationSuccessorHash* linkedSuccHash = satNode->getLinkedRoleSuccessorHash(false);
									if (linkedSuccHash) {
										cint64 foundCardinality = 0;
										CPROCESSHASH<CRole*,CLinkedRoleSaturationSuccessorData*>* succHash = linkedSuccHash->getLinkedRoleSuccessorHash();
										CLinkedRoleSaturationSuccessorData* succData = succHash->value(role);
										if (succData && succData->mSuccCount > 0) {
											freeCardinality = cardinality - succData->mSuccCount;

											if (freeCardinality < 0) {
												freeCardinality = 0;
											}
											cint64& roleCard = (*roleFreeCardHash)[role];
											if (roleCard > freeCardinality || roleCard < 0) {
												roleCard = freeCardinality;
											}
										}
									}
								}
							}

							cint64 roleCardSigValue = 0;
							for (CPROCESSHASH<CRole*,cint64>::const_iterator it = roleFreeCardHash->constBegin(), itEnd = roleFreeCardHash->constEnd(); it != itEnd; ++it) {
								CRole* role(it.key());
								cint64 freeCard(it.value());
								roleCardSigValue += role->getRoleTag();
								roleCardSigValue += (cint64)role;
								roleCardSigValue += freeCard;
							}


							CBackendRepresentativeMemoryCardinalityCacheItem* cachedCardItem = mAssBackCacheReader->getCardinalityCacheEntry(roleCardSigValue,roleFreeCardHash);
							if (cachedCardItem) {
								++mStatReusedCachedLabels;
								if (hashedRefPair) {
									hashedRefPair->first = cachedCardItem;
								}
								tmpAssWriteDataLinker->setReferredCardinalityData(cachedCardItem);
								return true;
							}


							CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*& prevTempRefCards = (*mSignatureTmpRefCardHash)[roleCardSigValue];
							if (prevTempRefCards) {
								for (CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* prevTempRefCardIt = prevTempRefCards; prevTempRefCardIt; prevTempRefCardIt = prevTempRefCardIt->getNext()) {
									if (prevTempRefCardIt->getCardinalityCacheValueCount() == roleFreeCardHash->size()) {
										bool compatibleCards = true;
										for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = prevTempRefCardIt->getCardinalityCacheValueLinker(); cardValueLinkerIt && compatibleCards; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
											CCacheValue& cacheValue = cardValueLinkerIt->getCacheValue();
											CRole* role = (CRole*)cacheValue.getIdentification();

											if (roleFreeCardHash->contains(role)) {
												if (roleFreeCardHash->value(role) != cardValueLinkerIt->getFreeCardinality()) {
													compatibleCards = false;
												}
											} else {
												compatibleCards = false;
											}
										}
										if (compatibleCards) {
											++mStatReusedHashedSignatureLabels;
											++mStatReusedTempLabels;
											if (hashedRefPair) {
												hashedRefPair->second = prevTempRefCardIt;
											}
											tmpAssWriteDataLinker->setReferredTemporaryCardinalityData(prevTempRefCardIt);
											return true;
										}
									}
								}
							}


							CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* newTempRefCard = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							newTempRefCard->initLabelWriteData(roleCardSigValue);
							for (CPROCESSHASH<CRole*,cint64>::const_iterator it = roleFreeCardHash->constBegin(), itEnd = roleFreeCardHash->constEnd(); it != itEnd; ++it) {
								CRole* role(it.key());
								cint64 freeCard(it.value());
								CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = CObjectAllocator< CBackendRepresentativeMemoryCardinalityValueLinker >::allocateAndConstruct(mMemAllocMan);
								cardValueLinker->initCardinalityValueLinker(getCacheValue(role),freeCard);
								newTempRefCard->appendCardinalityCacheValueLinker(cardValueLinker);
							}
							mNewTmpCardList->append(newTempRefCard);
							++mStatCreatedTempLabels;

							if (hashedRefPair) {
								hashedRefPair->second = newTempRefCard;
							}
							tmpAssWriteDataLinker->setReferredTemporaryCardinalityData(newTempRefCard);
							prevTempRefCards = newTempRefCard->append(prevTempRefCards);
						}
						return true;
					}
					return false;
				}














				bool CSaturationNodeBackendAssociationCacheHandler::findAssociationBackendLabel(CIndividualSaturationProcessNode* satNode, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptSaturationLabelSet* conSet = satNode->getReapplyConceptSaturationLabelSet(false);
					if (!conSet) {
						return false;
					}
					CIndividualSaturationProcessNode* copiedNode = satNode->getCopyIndividualNode();
					if (copiedNode && copiedNode->getReapplyConceptSaturationLabelSet(false) && copiedNode->getReapplyConceptSaturationLabelSet(false)->getConceptSaturationDescriptionLinker() == conSet->getConceptSaturationDescriptionLinker()) {
						return findAssociationBackendLabel(copiedNode,tmpAssWriteDataLinker,calcAlgContext);
					}

					QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>* hashedRefPair = nullptr;
					if (satNode->getNominalIndividual() == nullptr) {
						QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>& refPair = (*mIndiNodeRefLabelHash)[satNode];
						if (refPair.first) {
							tmpAssWriteDataLinker->setReferredLabelData(refPair.first);
							++mStatReusedHashedIndiLabels;
							return true;
						} else if (refPair.second) {
							tmpAssWriteDataLinker->setReferredTemporaryLabelData(refPair.second);
							++mStatReusedHashedIndiLabels;
							return true;
						}
						hashedRefPair = &refPair;
					}

					CConceptSetSignature conSetSignature;
					for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
						CConcept* concept = conSatDesIt->getConcept();
						bool negation = conSatDesIt->isNegated();
						conSetSignature.addConceptSignature(concept,negation);						
					}
					cint64 conSetSigValue = conSetSignature.getSignatureValue();

					
					CBackendRepresentativeMemoryLabelCacheItem* cachedLabelItem = mAssBackCacheReader->getLabelCacheEntry(conSetSigValue,conSet->getConceptCount(),conSet->getConceptSaturationDescriptionLinker());
					if (cachedLabelItem) {
						++mStatReusedCachedLabels;
						if (hashedRefPair) {
							hashedRefPair->first = cachedLabelItem;
						}
						tmpAssWriteDataLinker->setReferredLabelData(cachedLabelItem);
						return true;
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*& prevTempRefLabels = (*mSignatureTmpRefLabelHash)[conSetSigValue];
					if (prevTempRefLabels) {
						for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* prevTempRefLabelIt = prevTempRefLabels; prevTempRefLabelIt; prevTempRefLabelIt = prevTempRefLabelIt->getNext()) {
							if (prevTempRefLabelIt->getDeterministicCacheValueCount() == conSet->getConceptCount()) {
								bool compatibleConcepts = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = prevTempRefLabelIt->getDeterministicCacheValueLinker(); labelValueLinkerIt && compatibleConcepts; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
									CConcept* concept = (CConcept*)cacheValue.getIdentification();
									bool negation = cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT;
									if (!conSet->hasConcept(concept,negation)) {
										compatibleConcepts = false;
									}
								}
								if (compatibleConcepts) {
									++mStatReusedHashedSignatureLabels;
									++mStatReusedTempLabels;
									if (hashedRefPair) {
										hashedRefPair->second = prevTempRefLabelIt;
									}
									tmpAssWriteDataLinker->setReferredTemporaryLabelData(prevTempRefLabelIt);
									return true;
								}
							}
						}
					}


					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTempRefLabel = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
					newTempRefLabel->initLabelWriteData(conSetSigValue);
					CIndividualSaturationProcessNodeStatusFlags* indFlags = satNode->getIndirectStatusFlags();
					CIndividualSaturationProcessNodeStatusFlags* dirFlags = satNode->getDirectStatusFlags();
					bool insufficient = indFlags->hasInsufficientFlag() || !indFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag();
					if (!insufficient) {
						newTempRefLabel->setCompletelySaturated(true);
						newTempRefLabel->setCompletelyHandled(true);
					}
					for (CConceptSaturationDescriptor* conSatDesIt = conSet->getConceptSaturationDescriptionLinker(); conSatDesIt; conSatDesIt = conSatDesIt->getNext()) {
						CConcept* concept = conSatDesIt->getConcept();
						bool negation = conSatDesIt->isNegated();
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(mMemAllocMan);
						labelValueLinker->initLabelValueLinker(getCacheValue(concept,negation));
						newTempRefLabel->appendDeterministicCacheValueLinker(labelValueLinker);
					}
					mNewTmpLabelList->append(newTempRefLabel);
					++mStatCreatedTempLabels;

					if (hashedRefPair) {
						hashedRefPair->second = newTempRefLabel;
					}
					tmpAssWriteDataLinker->setReferredTemporaryLabelData(newTempRefLabel);
					prevTempRefLabels = newTempRefLabel->append(prevTempRefLabels);
					return true;
				}


				const CCacheValue CSaturationNodeBackendAssociationCacheHandler::getCacheValue(CConcept* concept, bool negation) {
					return mAssBackCacheReader->getCacheValue(concept,negation);
				}



				const CCacheValue CSaturationNodeBackendAssociationCacheHandler::getCacheValue(CRole* role) {
					return mAssBackCacheReader->getCacheValue(role);
				}


				bool CSaturationNodeBackendAssociationCacheHandler::tryAssociateNodesWithBackendCache(CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinker, CCalculationAlgorithmContext* calcAlgContext) {

					mIndiNodeRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH< CIndividualSaturationProcessNode*,QPair<CBackendRepresentativeMemoryLabelCacheItem*,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*> >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpRefLabelHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					mIndiNodeRefCardHash = CObjectParameterizingAllocator< CPROCESSHASH< CIndividualSaturationProcessNode*,QPair<CBackendRepresentativeMemoryCardinalityCacheItem*,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*> >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
					mSignatureTmpRefCardHash = CObjectParameterizingAllocator< CPROCESSHASH<cint64,CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker*>, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());


					mNewTmpLabelList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());
					mNewTmpCardList = CObjectParameterizingAllocator< CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >, CContext* >::allocateAndConstructAndParameterize(calcAlgContext->getUsedTemporaryMemoryAllocationManager(),calcAlgContext->getUsedTaskProcessorContext());

					prepareCacheMessages(calcAlgContext);

					CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tmpAssWriteDataLinker = nullptr;

					for (CIndividualSaturationProcessNodeLinker* indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinker; indiSaturationAnalysingNodeLinkerIt; indiSaturationAnalysingNodeLinkerIt = indiSaturationAnalysingNodeLinkerIt->getNext()) {
						CIndividualSaturationProcessNode* satNode = indiSaturationAnalysingNodeLinkerIt->getProcessingIndividual();
						CIndividual* nominalIndividual = satNode->getNominalIndividual();
						if (nominalIndividual) {

							CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* newAssWriteDataLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker >::allocateAndConstruct(mMemAllocMan);
							newAssWriteDataLinker->initAccociationWriteData(nominalIndividual);

							findAssociationBackendLabel(satNode,newAssWriteDataLinker,calcAlgContext);
							findAssociationBackendCardinality(satNode,newAssWriteDataLinker,calcAlgContext);

							CIndividualSaturationProcessNodeStatusFlags* indFlags = satNode->getIndirectStatusFlags();
							CIndividualSaturationProcessNodeStatusFlags* dirFlags = satNode->getDirectStatusFlags();
							bool insufficient = indFlags->hasInsufficientFlag() || !indFlags->hasCompletedFlag() || !dirFlags->hasCompletedFlag();
							if (insufficient) {
								newAssWriteDataLinker->setCompletelySaturated(false);
								newAssWriteDataLinker->setCompletelyHandled(false);
							}

							tmpAssWriteDataLinker = newAssWriteDataLinker->append(tmpAssWriteDataLinker);
						}
					}

					CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = nullptr;
					for (CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* >::const_iterator it = mNewTmpLabelList->constBegin(), itEnd = mNewTmpLabelList->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* newTmpLabelLinker = (*it);
						newTmpLabelLinker->clearNext();
						tempLabelWriteDataLinker = newTmpLabelLinker->append(tempLabelWriteDataLinker);
					}
					CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = nullptr;
					for (CPROCESSLIST< CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* >::const_iterator it = mNewTmpCardList->constBegin(), itEnd = mNewTmpCardList->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* newTmpCardLinker = (*it);
						newTmpCardLinker->clearNext();
						tempCardWriteDataLinker = newTmpCardLinker->append(tempCardWriteDataLinker);
					}

					CBackendRepresentativeMemoryCacheLabelAssociationWriteData* writeData = CObjectAllocator< CBackendRepresentativeMemoryCacheLabelAssociationWriteData >::allocateAndConstruct(mMemAllocMan);
					writeData->initWriteData(tmpAssWriteDataLinker,tempLabelWriteDataLinker,tempCardWriteDataLinker);
					addCacheMessages(writeData,calcAlgContext);
					commitCacheMessages(calcAlgContext);

					return false;
				}










			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
