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

#include "CBackendRepresentativeMemoryCache.h"
#include "CBackendRepresentativeMemoryCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCache::CBackendRepresentativeMemoryCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName, watchDogThread) {

					mConfig = config;

					mSlotLinker = nullptr;
					mLastUpdatedSlotLinker = nullptr;
					mReaderLinker = nullptr;

					mNextIndiUpdateId = 1;
					mNextNomConnUpdateId = 1;

					mReaderSlotUpdateCount = 0;
					mOntologyDataUpdateCount = 0;
					mOntologyDataReleasedCount = 0;
					mOntologyDataReleasedWhileNewCreationCount = 0;
					mOntologyDataReleasedWhileSlotUpdateCount = 0;
					mReaderSlotReleasedCount = 0;
					mCheckingRemainingIncompletelyHandledCount = 0;
					mEmptyWriteDataCount = 0;
					mWriteDataCount = 0;
					mStartWriteCollectCount = 0;
					mNextWriteCollectCount = 0;


					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					//mIndiIdAssoDataHash = nullptr;

					mOntologyIdentifierDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>, CBackendRepresentativeMemoryCacheBaseContext* >::allocateAndConstructAndParameterize(memMan, &mContext);

					startThread(QThread::HighestPriority);
				}


				CBackendRepresentativeMemoryCache::~CBackendRepresentativeMemoryCache() {
				}



				CCacheStatistics* CBackendRepresentativeMemoryCache::getCacheStatistics() {
					return &mCacheStat;
				}

				void CBackendRepresentativeMemoryCache::createReaderSlotUpdate(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, CBackendRepresentativeMemoryCacheBaseContext* context) {
					CMemoryPoolProvider* memProv = context->getMemoryPoolAllocationManager();
					CBackendRepresentativeMemoryCacheSlotItem* slot = CObjectMemoryPoolAllocator<CBackendRepresentativeMemoryCacheSlotItem>::allocateAndConstructWithMemroyPool(memProv);
					CMemoryPoolContainerAllocationManager slotMemMan(slot, memProv);
					CMemoryAllocationManager* memMan = &slotMemMan;
					CContextBase tmpContext(memMan);


					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>* ontologyIdentifierDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>, CContext* >::allocateAndConstructAndParameterize(memMan, &tmpContext);
					*ontologyIdentifierDataHash = *mOntologyIdentifierDataHash;
					ontologyIdentifierDataHash->detach();
					slot->setOntologyIdentifierDataHash(ontologyIdentifierDataHash);

					mReaderSlotUpdateCount++;

					mLastUpdatedSlotLinker = slot;
					if (mSlotLinker) {
						mSlotLinker->append(slot);
					} else {
						mSlotLinker = slot;
					}


					for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = ontologyIdentifierDataHash->constBegin(), itEnd = ontologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
						ontologyData->incUsageCount();
					}

					CBackendRepresentativeMemoryCacheReader* readerLinkerIt = mReaderLinker;
					while (readerLinkerIt) {
						slot->incReader();
						readerLinkerIt->updateSlot(slot);
						readerLinkerIt = readerLinkerIt->getNext();
					}
				}






				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCache::prepareOntologyDataUpdate(cint64 ontologyIdentifier) {
					CBackendRepresentativeMemoryCacheOntologyData*& ontologyData = (*mOntologyIdentifierDataHash)[ontologyIdentifier];
					CBackendRepresentativeMemoryCacheOntologyData* prevOntologyData = ontologyData;

					ontologyData = new CBackendRepresentativeMemoryCacheOntologyData(&mContext);
					ontologyData->initOntologyData(ontologyIdentifier);

					mOntologyDataUpdateCount++;

					CBackendRepresentativeMemoryCacheContext* tmpContext = ontologyData->getTemporaryContext();
					CMemoryAllocationManager* tmpMemMan = tmpContext->getMemoryAllocationManager();
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* slotSigLabelItemHashArray[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* ontoSigLabelItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(tmpMemMan, tmpContext);
						slotSigLabelItemHashArray[i] = ontoSigLabelItemHash;
						ontologyData->setSignatureLabelItemHash(i, ontoSigLabelItemHash);

						if (prevOntologyData) {
							*ontoSigLabelItemHash = *prevOntologyData->getSignatureLabelItemHash(i);
							ontoSigLabelItemHash->detach();
						}
					}

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* ontologyNominalIndiIdIndirectConnectionDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(tmpMemMan, tmpContext);
					if (prevOntologyData) {
						*ontologyNominalIndiIdIndirectConnectionDataHash = *prevOntologyData->getNominaIIndividualdIndirectConnectionDataHash();
						ontologyNominalIndiIdIndirectConnectionDataHash->detach();
					}
					ontologyData->setNominaIIndividualdIndirectConnectionDataHash(ontologyNominalIndiIdIndirectConnectionDataHash);



					if (!prevOntologyData) {
						CBackendRepresentativeMemoryCacheOntologyContext* ontologyContext = new CBackendRepresentativeMemoryCacheOntologyContext(&mContext);
						ontologyData->setOntologyContext(ontologyContext);
					} else {
						ontologyData->setOntologyContext(prevOntologyData->getOntologyContext());
					}



					if (!prevOntologyData) {
						cint64 indiIdAssoDataVectorSize = 1000000;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(ontologyData->getOntologyContext()->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
						for (cint64 i = 0; i < indiIdAssoDataVectorSize; ++i) {
							indiIdAssoDataVector[i] = nullptr;
						}
						ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
					} else {
						cint64 indiIdAssoDataVectorSize = prevOntologyData->getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = prevOntologyData->getIndividualIdAssoiationDataVector();
						ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
					}


					if (prevOntologyData) {
						ontologyData->setSameIndividualsMergings(prevOntologyData->hasSameIndividualsMergings());
						ontologyData->setMaxIndividualAssociationDataUpdateCount(prevOntologyData->getMaxIndividualAssociationDataUpdateCount());
						ontologyData->setZeroIncompletelyHandledIndividualIdCountDebugWritten(prevOntologyData->getZeroIncompletelyHandledIndividualIdCountDebugWritten());
						ontologyData->setIncompletelyHandledIndividualIdCount(prevOntologyData->getIncompletelyHandledIndividualIdCount());
						ontologyData->setLastMinIncompletelyHandledIndvidualiId(prevOntologyData->getLastMinIncompletelyHandledIndvidualiId());
						ontologyData->setNextEntryID(prevOntologyData->getNextEntryID(false));
					}


					ontologyData->incUsageCount();
					if (prevOntologyData) {
						prevOntologyData->decUsageCount();
						if (prevOntologyData->getUsageCount() <= 0) {
							++mOntologyDataReleasedCount;
							++mOntologyDataReleasedWhileNewCreationCount;
							CMemoryPoolAllocationManager* memMan = mContext.getMemoryPoolAllocationManager();
							CMemoryPool* memoryPools = prevOntologyData->getTemporaryContext()->getMemoryPoolContainer()->getMemoryPools();
							memMan->releaseTemporaryMemoryPools(memoryPools);
							delete prevOntologyData;
						}

					}

					return ontologyData;
				}






				void CBackendRepresentativeMemoryCache::cleanUnusedSlots(CBackendRepresentativeMemoryCacheBaseContext* context) {
					CMemoryPoolAllocationManager* memMan = context->getMemoryPoolAllocationManager();
					CBackendRepresentativeMemoryCacheSlotItem* slotLinkerIt = mSlotLinker;
					CBackendRepresentativeMemoryCacheSlotItem* lastSlotLinker = nullptr;
					while (slotLinkerIt) {
						CBackendRepresentativeMemoryCacheSlotItem* tmpSlotLinker = nullptr;
						if (!slotLinkerIt->hasCacheReaders()) {
							tmpSlotLinker = slotLinkerIt;
						}
						CBackendRepresentativeMemoryCacheSlotItem* nextSlotLinker = slotLinkerIt->getNext();
						if (tmpSlotLinker) {
							if (!lastSlotLinker) {
								mSlotLinker = nextSlotLinker;
							} else {
								lastSlotLinker->setNext(nextSlotLinker);
							}

							CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>* ontologyIdentifierDataHash = slotLinkerIt->getOntologyIdentifierDataHash();
							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = ontologyIdentifierDataHash->constBegin(), itEnd = ontologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
								CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
								ontologyData->decUsageCount();
								if (ontologyData->getUsageCount() <= 0) {
									++mOntologyDataReleasedCount;
									++mOntologyDataReleasedWhileSlotUpdateCount;

									CMemoryPool* memoryPools = ontologyData->getTemporaryContext()->getMemoryPoolContainer()->getMemoryPools();
									memMan->releaseTemporaryMemoryPools(memoryPools);
									delete ontologyData;
								}
							}

							CMemoryPool* memoryPools = tmpSlotLinker->getMemoryPools();
							memMan->releaseTemporaryMemoryPools(memoryPools);
							++mReaderSlotReleasedCount;
						}
						else {
							lastSlotLinker = slotLinkerIt;
						}
						slotLinkerIt = nextSlotLinker;
					}
				}



				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCache::createCacheReader() {
					CBackendRepresentativeMemoryCacheReader* reader = new CBackendRepresentativeMemoryCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = reader->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return reader;
				}


				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCache::createOntologyFixedCacheReader(cint64 ontologyIdentifier) {
					CBackendRepresentativeMemoryCacheReader* reader = new CBackendRepresentativeMemoryCacheReader();
					mFixedOntologyIdentifierDataHashLock.lockForRead();
					CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mFixedOntologyIdentifierDataHash.value(ontologyIdentifier);
					if (ontologyData) {
						ontologyData->incUsageCount();
					}
					reader->fixOntologyData(ontologyData);
					mFixedOntologyIdentifierDataHashLock.unlock();
					return reader;
				}


				CBackendRepresentativeMemoryCacheWriter* CBackendRepresentativeMemoryCache::createCacheWriter() {
					CBackendRepresentativeMemoryCacheWriter* writer = new CBackendRepresentativeMemoryCacheWriter(this);
					return writer;
				}



				CBackendRepresentativeMemoryCache* CBackendRepresentativeMemoryCache::writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteBackendAssociationCachedEvent(writeData, memoryPools));
					return this;
				}




				void CBackendRepresentativeMemoryCache::installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					for (CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinkerIt = tempCardWriteDataLinker; tempCardWriteDataLinkerIt; tempCardWriteDataLinkerIt = tempCardWriteDataLinkerIt->getNext()) {

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* labelWriteDataLinker = tempCardWriteDataLinkerIt->getLabelWriteDataLinker();
						CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)labelWriteDataLinker->getTemporaryData();

						CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = tempCardWriteDataLinkerIt->getCardinalityCacheValueLinker();

						bool handled = false;
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)labelCacheItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						if (extensionData) {
							handled = true;
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
								cint64 roleTag = cardValueLinkerIt->getRoleTag();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* roleCardData = extensionData->getRoleCardinalityData(roleTag);
								if (roleCardData) {
									roleCardData->updateExistentialMaxUsedCardinality(cardValueLinkerIt->getExistentialMaxUsedCardinality());
									roleCardData->updateMinimumRestrictingCardinality(cardValueLinkerIt->getMinimalRestrictingCardinality());
								} else {
									handled = false;
								}
							}
						}

						if (!handled) {
							CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* newExtensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newExtensionData->initCardinalityExtensionData();
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
								cint64 roleTag = cardValueLinkerIt->getRoleTag();
								cint64 cardCount = cardValueLinkerIt->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardValueLinkerIt->getMinimalRestrictingCardinality();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
								cardData->initCardinalityData(cardCount, minRestCount);
								if (extensionData) {
									CBackendRepresentativeMemoryLabelCacheItemCardinalityData* roleCardData = extensionData->getRoleCardinalityData(roleTag);
									cardData->updateExistentialMaxUsedCardinality(roleCardData->getExistentialMaxUsedCardinality());
									cardData->updateMinimumRestrictingCardinality(roleCardData->getMinimumRestrictingCardinality());
								}
								newExtensionData->setRoleCardinalityData(roleTag, cardData);
							}

							labelCacheItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH, newExtensionData);
						}
					}
				}






				void CBackendRepresentativeMemoryCache::installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinkerIt = tempLabelWriteDataLinker; tempLabelWriteDataLinkerIt; tempLabelWriteDataLinkerIt = tempLabelWriteDataLinkerIt->getNext()) {

						cint64 signature = tempLabelWriteDataLinkerIt->getSignature();
						if (tempLabelWriteDataLinkerIt->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
							signature = 0;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = tempLabelWriteDataLinkerIt->getCacheValueLinker(); labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
								if (cacheValue.getCacheValueIdentifier() == CCacheValue::CACHE_VALUE_TAG_AND_TEMPORARY_ENTRY) {
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpLabelWriteData = (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*)cacheValue.getIdentification();
									CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpLabelWriteData->getTemporaryData();
									cacheValue.initCacheValue(labelCacheItem->getCacheEntryID(), (cint64)labelCacheItem, CCacheValue::CACHE_VALUE_TAG_AND_ENTRY);
									labelValueLinkerIt->setCacheValue(cacheValue);
								}
								signature = CBackendRepresentativeMemoryCacheUtilities::getSignatureExtensionByCacheValue(signature, cacheValue);
							}
							// update signature
							tempLabelWriteDataLinkerIt->setSignature(signature);
						}

						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = tempLabelWriteDataLinkerIt->getCacheValueLinker();
						cint64 labelValueCount = tempLabelWriteDataLinkerIt->getCacheValueCount();
						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(tempLabelWriteDataLinkerIt->getLabelType()))[signature];

						CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = nullptr;
						for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt && !refLabelItem; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
							if (labelItemLinkerIt->getCacheValueCount() == labelValueCount) {
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
								bool identical = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
									cint64 valueTag = cacheValue.getTag();
									CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
									if (!labelValue || labelValue->getCacheValue() != cacheValue) {
										identical = false;
									}
								}

								if (identical) {
									refLabelItem = labelItemLinkerIt;
								}
							}
						}

						if (!refLabelItem) {
							refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)tempLabelWriteDataLinkerIt->getLabelType());

							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

								CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
								labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
								refLabelItem->addCacheValueLinker(newValueLinker);
							}

							sigResolveItem.appendLabelItem(refLabelItem);
						}

						if (!refLabelItem->isCompletelyHandled() && tempLabelWriteDataLinkerIt->isCompletelyHandled()) {
							refLabelItem->setCompletelyHandled(true);
						}
						if (!refLabelItem->isCompletelySaturated() && tempLabelWriteDataLinkerIt->isCompletelySaturated()) {
							refLabelItem->setCompletelySaturated(true);
						}
						if (!refLabelItem->hasNondeterministicElements() && tempLabelWriteDataLinkerIt->hasNondeterministicElements()) {
							refLabelItem->setNondeterministicElements(true);
						}


						if (tempLabelWriteDataLinkerIt->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
							getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
						}

						tempLabelWriteDataLinkerIt->setTemporaryData(refLabelItem);
					}

				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCache::getReducedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, function<bool(const CCacheValue& cacheValue)> reduceCheckFunction, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = label->getCacheValueLinker();
					cint64 signature = 0;
					cint64 newCount = 0;
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
						CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
						if (!reduceCheckFunction(cacheValue)) {
							// extend signature/hash value
							signature += qHash((qint64)cacheValue.getTag());
							newCount++;
						}
					}


					if (newCount == label->getCount()) {
						return label;
					}

					// test with signature whether reduced label already exists
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(labelType))[signature];

					for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
						if (labelItemLinkerIt->getCacheValueCount() == newCount) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
							bool identical = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
								if (!reduceCheckFunction(cacheValue)) {
									cint64 valueTag = cacheValue.getTag();
									CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
									if (!labelValue || labelValue->getCacheValue() != cacheValue) {
										identical = false;
									}
								}
							}

							if (identical) {
								return labelItemLinkerIt;
							}
						}
					}


					// create new label item
					CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
					refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)labelType);

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

						const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
						if (!reduceCheckFunction(cacheValue)) {
							CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
							newValueLinker->initLabelValueLinker(cacheValue);
							labelValueHash->insert(cacheValue.getTag(), newValueLinker);
							refLabelItem->addCacheValueLinker(newValueLinker);
						}
					}

					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
					}


					sigResolveItem.appendLabelItem(refLabelItem);
					return refLabelItem;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCache::getAdditionMergedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* additionLabel, CBackendRepresentativeMemoryLabelCacheItem* associatedlabel, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					if (additionLabel == associatedlabel) {
						return associatedlabel;
					}
					CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinker = additionLabel->getCacheValueLinker();
					CBackendRepresentativeMemoryLabelValueLinker* associatedLabelValueLinker = associatedlabel->getCacheValueLinker();
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* associatedLabelValueHash = associatedlabel->getTagCacheValueHash(false);
					// check whether all values are already included
					bool valuesAlreadyIncluded = true;
					cint64 signature = associatedlabel->getSignature();
					cint64 newCount = associatedlabel->getCount();
					for (CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinkerIt = additionLabelValueLinker; additionLabelValueLinkerIt; additionLabelValueLinkerIt = additionLabelValueLinkerIt->getNext()) {
						CCacheValue cacheValue = additionLabelValueLinkerIt->getCacheValue();
						if (!associatedLabelValueHash || !associatedLabelValueHash->contains(cacheValue.getTag())) {
							valuesAlreadyIncluded = false;
							// extend signature/hash value
							signature += qHash((qint64)cacheValue.getTag());
							newCount++;
						}
					}
					if (valuesAlreadyIncluded) {
						return associatedlabel;
					}
					// test with signature whether extended label already exists
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(labelType))[signature];

					for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
						if (labelItemLinkerIt->getCacheValueCount() == additionLabel->getCacheValueCount()) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
							bool identical = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = associatedLabelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
								cint64 valueTag = cacheValue.getTag();
								CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
								if (!labelValue || labelValue->getCacheValue() != cacheValue) {
									identical = false;
								}
							}
							for (CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinkerIt = additionLabelValueLinker; additionLabelValueLinkerIt && identical; additionLabelValueLinkerIt = additionLabelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = additionLabelValueLinkerIt->getCacheValue();
								cint64 valueTag = cacheValue.getTag();
								CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
								if (!labelValue || labelValue->getCacheValue() != cacheValue) {
									identical = false;
								}
							}

							if (identical) {
								return labelItemLinkerIt;
							}
						}
					}


					// create new label item
					CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
					refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)labelType);

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = associatedLabelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

						CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
						newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
						labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
						refLabelItem->addCacheValueLinker(newValueLinker);
					}

					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = additionLabelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
						CBackendRepresentativeMemoryLabelValueLinker*& newValueLinker = (*labelValueHash)[labelValueLinkerIt->getCacheValue().getTag()];
						if (!newValueLinker) {
							newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
							newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
							labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
							refLabelItem->addCacheValueLinker(newValueLinker);
						}
					}

					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
					}
					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {


						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData1 = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)additionLabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData2 = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)associatedlabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						if (extensionData1 || extensionData2) {


							CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* newExtensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newExtensionData->initCardinalityExtensionData();
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* newExtensionDataHash = newExtensionData->getRoleCardinalityDataHash();

							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = extensionData1->getRoleCardinalityDataHash()->constBegin(), itEnd = extensionData1->getRoleCardinalityDataHash()->constEnd(); it != itEnd; ++it) {
								cint64 roleTag = it.key();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
								cint64 cardCount = cardData->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardData->getMinimumRestrictingCardinality();

								CBackendRepresentativeMemoryLabelCacheItemCardinalityData*& roleCardData = (*newExtensionDataHash)[roleTag];
								if (!roleCardData) {
									roleCardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
									roleCardData->initCardinalityData(cardCount, minRestCount);
								} else {
									roleCardData->updateExistentialMaxUsedCardinality(cardCount);
									roleCardData->updateMinimumRestrictingCardinality(minRestCount);
								}
							}
							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = extensionData2->getRoleCardinalityDataHash()->constBegin(), itEnd = extensionData2->getRoleCardinalityDataHash()->constEnd(); it != itEnd; ++it) {
								cint64 roleTag = it.key();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
								cint64 cardCount = cardData->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardData->getMinimumRestrictingCardinality();

								CBackendRepresentativeMemoryLabelCacheItemCardinalityData*& roleCardData = (*newExtensionDataHash)[roleTag];
								if (!roleCardData) {
									roleCardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
									roleCardData->initCardinalityData(cardCount, minRestCount);
								} else {
									roleCardData->updateExistentialMaxUsedCardinality(cardCount);
									roleCardData->updateMinimumRestrictingCardinality(minRestCount);
								}
							}

							refLabelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH, newExtensionData);


						}	

					}

					sigResolveItem.appendLabelItem(refLabelItem);
					return refLabelItem;

				}





				bool CBackendRepresentativeMemoryCache::checkAssociationUsage(CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinkerIt = tempAssUseDataLinker; tempAssUseDataLinkerIt; tempAssUseDataLinkerIt = tempAssUseDataLinkerIt->getNext()) {
						cint64 individualID = tempAssUseDataLinkerIt->getIndividualID();
						cint64 usedUpateID = tempAssUseDataLinkerIt->getUsedAssociationUpdateId();

						CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
						if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
							associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
						}

						mCheckedIndiCount++;

						if (!associationData || (associationData->getAssociationDataUpdateId() != usedUpateID && associationData->isCompletelyHandled())) {

							associationsUpdated = markIndividualAssociationIncompletelyHandled(individualID, associationData, ontologyData);

							mCheckIncompatibleIndiCount++;


						}
					}
					return associationsUpdated;
				}









				bool CBackendRepresentativeMemoryCache::markIndividualAssociationIncompletelyHandled(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;

					if (!associationData || associationData->isCompletelyHandled()) {

						CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
						locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(context->getMemoryAllocationManager());
						if (associationData) {
							locAssociationData->initAssociationData(associationData);
						} else {
							locAssociationData->initAssociationData(individualID);
						}
						locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);


						if (associationData && associationData->getNeighbourRoleSetHash()) {

							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* prevNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();

							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* prevArray = associationData->getRoleSetNeighbourArray();

							newArray->initNeighbourArray(prevArray);
							newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash);

							locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
							locAssociationData->setRoleSetNeighbourArray(newArray);
						}



						locAssociationData->setCompletelyHandled(false);
						if (!locAssociationData->isIncompletelyMarked()) {
							ontologyData->incIncompletelyHandledIndividualIdCount();
							ontologyData->setLastMinIncompletelyHandledIndvidualiId(qMin(ontologyData->getLastMinIncompletelyHandledIndvidualiId(), individualID));

							locAssociationData->setIncompletelyMarked(true);


							//if (mIncompletelyAssociatedIndividualSet.contains(CIndividualReference(individualID))) {
							//	bool debug = true;
							//}
							//mIncompletelyAssociatedIndividualSet.insert(CIndividualReference(individualID));

						}
						associationsUpdated = true;

						setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);

					}
					return associationsUpdated;
				}








				bool CBackendRepresentativeMemoryCache::installAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();


						bool labelAddition = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION;
						bool labelReplacement = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT;
						bool labelRemoval = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REMOVAL;


						bool linksAddition = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION;
						bool linksReplacement = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT;
						bool linksRemoval = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REMOVAL;


						cint64 integratedIndirectlyConnectedIndividualsChangeId = tempAssWriteDataLinkerIt->getIntegratedIndirectlyConnectedIndividualsChangeId();


						CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
						if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
							associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
						}

						bool incompatibleChanges = false;
						cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();
						if (associationData && associationData->getAssociationDataUpdateId() != usedUpdateId) {
							labelAddition = true;
							labelRemoval = false;
							labelReplacement = false;
							linksAddition = true;
							linksRemoval = false;
							linksReplacement = false;
							incompatibleChanges = true;
						}

						CBackendRepresentativeMemoryLabelCacheItem* referredLabels[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT];
						for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
							CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = tempAssWriteDataLinkerIt->getReferredLabelData(i);
							if (!referredLabelCacheItem) {
								CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = tempAssWriteDataLinkerIt->getReferredTemporaryLabelData(i);
								if (tmpRefferedLabelItem) {
									referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
								}
							}
							referredLabels[i] = referredLabelCacheItem;
						}






						bool repLabelUpdateRequired = false;
						for (cint64 i = 0; associationData && !repLabelUpdateRequired && i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
							if ((referredLabels[i] || labelReplacement || labelRemoval) && (!associationData || referredLabels[i] != associationData->getLabelCacheEntry(i))) {
								repLabelUpdateRequired = true;
							}
						}


						bool statusFlagsUpdateRequired = false;
						if ((tempAssWriteDataLinkerIt->getStatusFlags() != 0 || labelReplacement || labelRemoval) && (!associationData || associationData->getStatusFlags() != tempAssWriteDataLinkerIt->getStatusFlags())) {
							statusFlagsUpdateRequired = true;
						}



						bool linksUpdateRequired = false;
						if ((tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker() != nullptr || linksReplacement || linksRemoval)) {
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* existingNeighbourRoleSetHash = nullptr;
							if (associationData) {
								existingNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();
							}
							cint64 updateLinksCount = 0;
							for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker(); !linksUpdateRequired && roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
								CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
								cint64 neighbourId = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference().getIndividualID();
								CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
								if (!referredLabelCacheItem) {
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
									if (tmpRefferedLabelItem) {
										referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
									}
								}
								if (linksAddition) {
									if (!existingNeighbourRoleSetHash) {
										linksUpdateRequired = true;
									} else if (existingNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId) != referredLabelCacheItem) {
										linksUpdateRequired = true;
									}
								}
								if (linksRemoval) {
									if (existingNeighbourRoleSetHash && existingNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId)) {
										linksUpdateRequired = true;
									}
								}
								updateLinksCount++;
							}
							if (!linksUpdateRequired && (updateLinksCount > 0 && !existingNeighbourRoleSetHash || existingNeighbourRoleSetHash && updateLinksCount != existingNeighbourRoleSetHash->getNeighbourCount())) {
								linksUpdateRequired = true;
							}
						}


						bool integrationUpdated = false;
						if (!associationData || (associationData && (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration() && !associationData->hasIndirectlyConnectedIndividualIntegration() || 
																	tempAssWriteDataLinkerIt->isIndirectlyConnectedNominalIndividual() && !associationData->isIndirectlyConnectedNominalIndividual() ||
																	integratedIndirectlyConnectedIndividualsChangeId > 0 && integratedIndirectlyConnectedIndividualsChangeId != associationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId()))) {
							integrationUpdated = true;
						}


						if (!associationData || repLabelUpdateRequired || statusFlagsUpdateRequired || linksUpdateRequired || integrationUpdated) {

							++mUpdatedIndiCount;

							CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
							locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(context->getMemoryAllocationManager());
							if (associationData) {
								locAssociationData->initAssociationData(associationData);
							} else {
								locAssociationData->initAssociationData(individualID);
							}
							locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
							ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

							locAssociationData->setRepresentativeSameIndividualId(qMax(tempAssWriteDataLinkerIt->getRepresentativeSameIndividualId(), locAssociationData->getRepresentativeSameIndividualId()));

							if (locAssociationData->getRepresentativeSameIndividualId() != locAssociationData->getAssociatedIndividualId()) {
								ontologyData->setSameIndividualsMergings(true);
							}


							bool labelsUpdated = false;
							if (!associationData || repLabelUpdateRequired) {

								for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
									if (referredLabels[i] || labelReplacement || labelRemoval) {
										if (locAssociationData->getLabelCacheEntry(i) && labelAddition) {
											CBackendRepresentativeMemoryLabelCacheItem* mergedLabel = getAdditionMergedLabel(i, referredLabels[i], locAssociationData->getLabelCacheEntry(i), ontologyData);
											locAssociationData->setLabelCacheEntry(i, mergedLabel);
										} else {
											locAssociationData->setLabelCacheEntry(i, referredLabels[i]);
										}
									}
									bool exactIndiAssocTracking = requiresIndividualAssociations(i);
									CBackendRepresentativeMemoryLabelCacheItem* prevLabelItem = nullptr;
									CBackendRepresentativeMemoryLabelCacheItem* newLabelItem = locAssociationData->getLabelCacheEntry(i);
									if (associationData) {
										prevLabelItem = associationData->getLabelCacheEntry(i);
									}

									if (prevLabelItem != newLabelItem) {
										if (prevLabelItem) {
											prevLabelItem->decIndividualAssociationCount();
											if (exactIndiAssocTracking) {
												CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(prevLabelItem, ontologyData);
												indiAssoExtData->removeIndividualIdAssociation(associationData);
											}
										}
										if (newLabelItem) {
											newLabelItem->incIndividualAssociationCount();
											if (exactIndiAssocTracking) {
												CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(newLabelItem, ontologyData);
												indiAssoExtData->addIndividualIdAssociation(locAssociationData);
											}
										}
									}
								}
								labelsUpdated = true;

								associationsUpdated = true;
								if (associationData) {
									CBackendRepresentativeMemoryLabelCacheItem* indConnNomLabelItem = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
									if (indConnNomLabelItem) {
										mTmpIndiIndirectlyConnNomLabelItemHash.insert(individualID, indConnNomLabelItem);
									}
								}
							}

							if (integrationUpdated) {
								if (integratedIndirectlyConnectedIndividualsChangeId > 0) {
									locAssociationData->setLastIntegratedIndirectlyConnectedIndividualsChangeId(integratedIndirectlyConnectedIndividualsChangeId);
								}
								if (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration()) {
									locAssociationData->setIndirectlyConnectedIndividualIntegration(true);
								}
								if (tempAssWriteDataLinkerIt->isIndirectlyConnectedNominalIndividual()) {
									locAssociationData->setIndirectlyConnectedNominalIndividual(true);
								}
								associationsUpdated = true;
							}

							if (statusFlagsUpdateRequired) {
								locAssociationData->setStatusFlags(tempAssWriteDataLinkerIt->getStatusFlags());
								associationsUpdated = true;
							}


							CBackendRepresentativeMemoryLabelCacheItem* newNeighbourRoleSetCompLabel = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
							CBackendRepresentativeMemoryLabelCacheItem* prevNeighbourRoleSetCompLabel = nullptr;
							if (associationData) {
								prevNeighbourRoleSetCompLabel = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
							}

							if (!linksUpdateRequired && prevNeighbourRoleSetCompLabel && newNeighbourRoleSetCompLabel != prevNeighbourRoleSetCompLabel) {
								linksUpdateRequired = true;
								associationsUpdated = true;
							}


							// update neighbour data
							if (linksUpdateRequired) {
								CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinker = tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker();
								// create role set neighbour array and neighbour role set hash, if addition, then copy previous data


								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* newArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(newNeighbourRoleSetCompLabel, ontologyData);

								CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* prevNeighbourRoleSetHash = nullptr;
								if (associationData) {
									prevNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();
								}

								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* prevArray = nullptr;
								if (associationData) {
									prevArray = associationData->getRoleSetNeighbourArray();
								}

								QSet< cint64 >* updateOrRemovalNeighbourIndiIdSet = nullptr;
								if ((linksRemoval || linksReplacement) && prevArray) {
									updateOrRemovalNeighbourIndiIdSet = new QSet< cint64 >();
								}

								for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinker; roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
									CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
									CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
									if (!referredLabelCacheItem) {
										CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
										if (tmpRefferedLabelItem) {
											referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
										}
									}
									if (referredLabelCacheItem) {
										if (prevNeighbourRoleSetHash && !updateOrRemovalNeighbourIndiIdSet) {
											if (prevNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID())) {
												updateOrRemovalNeighbourIndiIdSet = new QSet< cint64 >();
											}
										}
									}
								}



								if (prevNeighbourRoleSetCompLabel == newNeighbourRoleSetCompLabel && prevArray && linksAddition && !updateOrRemovalNeighbourIndiIdSet) {
									newArray->initNeighbourArray(prevArray);
									newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash);
								} else {
									newArray->initNeighbourArray(newArrayIndexData);
									// copy data from previous array
									if (linksAddition && prevArray && !updateOrRemovalNeighbourIndiIdSet) {
										CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArray->getIndexData();
										for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
											cint64 prevIndex = prevArrayIndexData->getIndex(newArrayIndexData->getNeighbourRoleSetLabel(i));
											if (prevIndex >= 0) {
												newArray->at(i) = prevArray->at(prevIndex);
											}
										}
										newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash);
									} else {
										newNeighbourRoleSetHash->initNeighbourRoleSetHash(nullptr);
									}
								}


								// add/update new data
								for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinker; roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
									CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
									CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
									if (!referredLabelCacheItem) {
										CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
										if (tmpRefferedLabelItem) {
											referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
										}
									}
									if (referredLabelCacheItem) {
										cint64 index = newArrayIndexData->getIndex(referredLabelCacheItem);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
										newLinker->initIndividualIdLinker(neighbourIndiRef.getIndividualID());
										newArray->at(index).addIndividualIdLinker(newLinker);
										newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID(), referredLabelCacheItem);
										if (updateOrRemovalNeighbourIndiIdSet) {
											updateOrRemovalNeighbourIndiIdSet->insert(neighbourIndiRef.getIndividualID());
										}
									}
								}

								bool needsNeighCompLabelCleanUp = false;
								// copy previous data
								if (updateOrRemovalNeighbourIndiIdSet && !linksReplacement) {
									CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArray->getIndexData();
									for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
										CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = newArrayIndexData->getNeighbourRoleSetLabel(i);
										cint64 prevIndex = prevArrayIndexData->getIndex(referredLabelCacheItem);
										if (prevIndex >= 0) {
											prevArray->at(prevIndex).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {
												if (!updateOrRemovalNeighbourIndiIdSet->contains(neighbourIndiId)) {
													CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
													newLinker->initIndividualIdLinker(neighbourIndiId);
													newArray->at(i).addIndividualIdLinker(newLinker);
													newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiId, referredLabelCacheItem);
												}
												return true;
											});
										}
									}
									delete updateOrRemovalNeighbourIndiIdSet;



									bool neighbourRoleSetCompLabelReductionRequired = false;
									for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
										if (!newArray->at(i).getIndividualIdLinker()) {
											neighbourRoleSetCompLabelReductionRequired = true;
										}
									}
									if (neighbourRoleSetCompLabelReductionRequired) {
										++mReducedNeighbourArrayCount;
										CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCombinationLabelSet = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

										CBackendRepresentativeMemoryLabelCacheItem* reducedNeighbourRoleSetCombinationLabelSet = getReducedLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, neighbourRoleSetCombinationLabelSet, [&](const CCacheValue& cacheValue)->bool {
											CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel = (CBackendRepresentativeMemoryLabelCacheItem*)cacheValue.getIdentification();
											cint64 index = newArrayIndexData->getIndex(neighbourRoleSetLabel);
											if (index < 0) {
												return true;
											}
											if (!newArray->at(index).getIndividualIdLinker()) {
												return true;
											}
											return false;
										}, ontologyData);

										if (neighbourRoleSetCombinationLabelSet != reducedNeighbourRoleSetCombinationLabelSet) {
											locAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, reducedNeighbourRoleSetCombinationLabelSet);


											CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* reducedArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(reducedNeighbourRoleSetCombinationLabelSet, ontologyData);
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* reducedArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
											reducedArray->initNeighbourArray(reducedArrayIndexData);
											// copy data into reduced array
											for (cint64 i = 0; i < reducedArrayIndexData->getArraySize(); ++i) {
												cint64 prevIndex = newArrayIndexData->getIndex(reducedArrayIndexData->getNeighbourRoleSetLabel(i));
												if (prevIndex >= 0) {
													reducedArray->at(i) = newArray->at(prevIndex);
												}
											}
											newArray = reducedArray;


											bool exactIndiAssocTracking = requiresIndividualAssociations(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

											if (neighbourRoleSetCombinationLabelSet) {
												neighbourRoleSetCombinationLabelSet->decIndividualAssociationCount();
												if (exactIndiAssocTracking) {
													CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(neighbourRoleSetCombinationLabelSet, ontologyData);
													indiAssoExtData->removeIndividualIdAssociation(locAssociationData);
												}
											}
											if (reducedNeighbourRoleSetCombinationLabelSet) {
												reducedNeighbourRoleSetCombinationLabelSet->incIndividualAssociationCount();
												if (exactIndiAssocTracking) {
													CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(reducedNeighbourRoleSetCombinationLabelSet, ontologyData);
													indiAssoExtData->addIndividualIdAssociation(locAssociationData);
												}
											}

										}
										
									}


								}

								locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
								locAssociationData->setRoleSetNeighbourArray(newArray);



							} else if (associationData && associationData->getNeighbourRoleSetHash()) {
								CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								newArray->initNeighbourArray(associationData->getRoleSetNeighbourArray());
								newNeighbourRoleSetHash->initNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash());
								locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
								locAssociationData->setRoleSetNeighbourArray(newArray);
								associationsUpdated = true;
							}


							if (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration()) {
								CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData*& nomConnData = (*ontologyData->getNominaIIndividualdIndirectConnectionDataHash())[individualID];
								if (nomConnData || integratedIndirectlyConnectedIndividualsChangeId > 0) {
									if (integratedIndirectlyConnectedIndividualsChangeId != nomConnData->getLastChangeId()) {
										locAssociationData->setCompletelyHandled(false);
										associationsUpdated = true;
									}
								}
							}


							if (incompatibleChanges && locAssociationData->isCompletelyHandled() && labelsUpdated) {
								mCheckIncompatibleIndiCount++;
								locAssociationData->setCompletelyHandled(false);
								associationsUpdated = true;
							}

							if (locAssociationData->isCompletelyPropagated() && !tempAssWriteDataLinkerIt->isCompletelyPropagated()) {
								locAssociationData->setCompletelyPropagated(false);
								associationsUpdated = true;
							}



							if (locAssociationData->isCompletelyHandled()) {
								if (locAssociationData->isIncompletelyMarked()) {
									ontologyData->decIncompletelyHandledIndividualIdCount();
									//if (!mIncompletelyAssociatedIndividualSet.contains(CIndividualReference(individualID))) {
									//	bool debug = true;
									//}
									//mIncompletelyAssociatedIndividualSet.remove(CIndividualReference(individualID));
									locAssociationData->setIncompletelyMarked(false);
									associationsUpdated = true;
								}
							} else {
								ontologyData->setLastMinIncompletelyHandledIndvidualiId(qMin(ontologyData->getLastMinIncompletelyHandledIndvidualiId(), individualID));
								if (!locAssociationData->isIncompletelyMarked()) {
									ontologyData->incIncompletelyHandledIndividualIdCount();
									//if (mIncompletelyAssociatedIndividualSet.contains(CIndividualReference(individualID))) {
									//	bool debug = true;
									//}
									//mIncompletelyAssociatedIndividualSet.insert(CIndividualReference(individualID));
									locAssociationData->setIncompletelyMarked(true);
									associationsUpdated = true;
								}
							}


							//if (mIncompletelyHandledIndiIdCount != mIncompletelyAssociatedIndividualSet.size()) {
							//	bool debug = true;
							//}
							//if (locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) && locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL)->getCacheValueCount() > 0) {
							//	if (!locAssociationData->getRoleSetNeighbourArray()) {
							//		bool debug = true;
							//	}
							//}


							if (associationsUpdated) {
								setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);
							}




						}
					}
					return associationsUpdated;
				}



				bool CBackendRepresentativeMemoryCache::installNominalIndirectConncetionUpdates(CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool connectionDataUpdated = false;

					for (CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinkerIt = tempNomIndirectConnDataLinker; tempNomIndirectConnDataLinkerIt; tempNomIndirectConnDataLinkerIt = tempNomIndirectConnDataLinkerIt->getNext()) {
						cint64 nomIndiId = tempNomIndirectConnDataLinkerIt->getIndividualID();
						CXLinker<cint64>* indirectlyConnectedIndividualIdLinker = tempNomIndirectConnDataLinkerIt->getIndirectlyConnectedIndividualIdLinker();
						cint64 lastChangeIntegrationId = tempNomIndirectConnDataLinkerIt->getLastChangeIntegrationId();

						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData*& nomConnData = (*ontologyData->getNominaIIndividualdIndirectConnectionDataHash())[nomIndiId];
						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* prevNomConnData = nomConnData;


						CBackendRepresentativeMemoryCacheIndividualAssociationData* nominalAssociationData = nullptr;
						if (ontologyData->getIndividualIdAssoiationDataVector() && nomIndiId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
							nominalAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[nomIndiId];
						}
						if (nominalAssociationData && nominalAssociationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId() != lastChangeIntegrationId && lastChangeIntegrationId > 0 ||
								nominalAssociationData && prevNomConnData && lastChangeIntegrationId == 0 && nominalAssociationData->hasIndirectlyConnectedIndividualIntegration() && prevNomConnData->getLastChangeId() != nominalAssociationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId()) {
							// mark as incompletely handled
							connectionDataUpdated |= markIndividualAssociationIncompletelyHandled(nomIndiId, nominalAssociationData, ontologyData);
						}


						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* newNomConnData = CObjectAllocator< CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData >::allocateAndConstruct(context->getMemoryAllocationManager());
						newNomConnData->initNominalIndividualIndirectConnectionData(prevNomConnData);
						nomConnData = newNomConnData;
						newNomConnData->setLastChangeId(mNextNomConnUpdateId++);

						for (CXLinker<cint64>* indirectlyConnectedIndividualIdLinkerIt = indirectlyConnectedIndividualIdLinker; indirectlyConnectedIndividualIdLinkerIt; indirectlyConnectedIndividualIdLinkerIt = indirectlyConnectedIndividualIdLinkerIt->getNext()) {
							cint64 indiId = indirectlyConnectedIndividualIdLinkerIt->getData();
							// avoid duplicates

							bool alreadyPresent = false;
							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && indiId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
							}

							CBackendRepresentativeMemoryLabelCacheItem* indiConnNomIndiIdSetLabel = nullptr;
							if (associationData) {
								if (associationData->getCacheUpdateId() < mTmpIndiAssocPrevUpdateId) {
									indiConnNomIndiIdSetLabel = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
								} else {
									indiConnNomIndiIdSetLabel = mTmpIndiIndirectlyConnNomLabelItemHash.value(indiId);
								}
							}

							if (indiConnNomIndiIdSetLabel && indiConnNomIndiIdSetLabel->hasCachedTagValue(nomIndiId)) {
								alreadyPresent = true;
							}


							if (!alreadyPresent) {
								CXLinker<cint64>* indiLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(context->getMemoryAllocationManager());
								indiLinker->initLinker(indiId);
								nomConnData->addIndirectlyConnectedIndividualIdLinker(indiLinker);
								connectionDataUpdated = true;
							}

						}
					}

					return connectionDataUpdated;
				}



				void CBackendRepresentativeMemoryCache::setUpdatedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = ontologyData->getIndividualIdAssoiationDataVector();
					cint64 indiIdAssoDataVectorSize = ontologyData->getIndividualIdAssoiationDataVectorSize();
					if (individualID >= indiIdAssoDataVectorSize) {
						cint64 prevSize = indiIdAssoDataVectorSize;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** prevIndiIdAssoDataVector = indiIdAssoDataVector;
						indiIdAssoDataVectorSize = qMax(indiIdAssoDataVectorSize * 10, individualID);
						indiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(context->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
						for (cint64 i = 0; i < prevSize; ++i) {
							indiIdAssoDataVector[i] = prevIndiIdAssoDataVector[i];
						}
						for (cint64 i = prevSize; i < indiIdAssoDataVectorSize; ++i) {
							indiIdAssoDataVector[i] = nullptr;
						}
						ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
					}
					indiIdAssoDataVector[individualID] = locAssociationData;
					//mIndiIdAssoDataHash->insert(individualID, locAssociationData);
				}


				bool CBackendRepresentativeMemoryCache::getIncompletlyAssociationCachedIndividuals(cint64 ontologyIdentifier, QSet<CIndividualReference>* individualSet, cint64 limit) {
					CBlockingCallbackData callbackData;
					postEvent(new CRetrieveIncompletelyAssociationCachedEvent(&callbackData, ontologyIdentifier, individualSet, limit));
					callbackData.waitForCallback();
					return true;
				}


				bool CBackendRepresentativeMemoryCache::requiresIndividualAssociations(cint64 labelType) {
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL ||
						labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL ||
						labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						return true;
					}
					return false;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryCache::getIndividualAssociationsExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP, extensionData);
					}
					return extensionData;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* CBackendRepresentativeMemoryCache::getIndividualNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX, extensionData);
						extensionData->initNeighbourArrayIndexData(labelItem);
					}
					return extensionData;
				}


				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* CBackendRepresentativeMemoryCache::getNeighbourArrayRoleTagResolvingLabelExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH, extensionData);
						extensionData->initTagLabelResolvingExtensionData();

						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexExtensionData = getIndividualNeighbourArrayIndexExtensionData(labelItem, ontologyData);
						for (cint64 i = 0; i < indexExtensionData->getArraySize(); ++i) {
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexExtensionData->getNeighbourRoleSetLabel(i);

							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = neighbourRoleSetLabelItem->getCacheValueLinker(); labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								CCacheValue cacheValue = labelValueLinkerIt->getData();


								CCacheValue::CACHEVALUEIDENTIFIER identifier = cacheValue.getCacheValueIdentifier();
								bool nondeterministc = identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE ||
									identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;


								CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								linker->initTagLabelResolvingData(neighbourRoleSetLabelItem, i, !nondeterministc);
								extensionData->appendTagLabelResolvingDataLinker(cacheValue.getTag(), linker);
							}
						}
						
					}
					return extensionData;
				}

				bool CBackendRepresentativeMemoryCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type, event)) {
						return true;

					}
					else if (type == EVENTRETRIEVEINCOMPLETELYASSOCIATIONCACHED) {
						CRetrieveIncompletelyAssociationCachedEvent* iace = (CRetrieveIncompletelyAssociationCachedEvent*)event;
						cint64 limit = iace->getRetrievalLimit();
						cint64 ontologyIdentifier = iace->getOntologyIdentifier();

						++mCheckingRemainingIncompletelyHandledCount;

						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mOntologyIdentifierDataHash->value(ontologyIdentifier);
						if (ontologyData) {
							cint64 prevLastMinIncompletelyHandledIndiId = ontologyData->getLastMinIncompletelyHandledIndvidualiId();
							QSet<CIndividualReference>* individualSet = iace->getIndividualSet();
							if (individualSet) {
								cint64 count = 0;
								for (cint64 i = ontologyData->getLastMinIncompletelyHandledIndvidualiId(); i < ontologyData->getIndividualIdAssoiationDataVectorSize() && count < ontologyData->getIncompletelyHandledIndividualIdCount() && (count < limit || limit < 0); ++i) {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[i];
									if (indiAssData && !indiAssData->isCompletelyHandled()) {
										individualSet->insert(i);
										count++;
									} else if (i == ontologyData->getLastMinIncompletelyHandledIndvidualiId()) {
										ontologyData->incLastMinIncompletelyHandledIndvidualiId();
									}
								}

								/*if (limit < 0 || limit > mIncompletelyAssociatedIndividualSet.size()) {
									*individualSet = mIncompletelyAssociatedIndividualSet;
								} else {
									cint64 count = 0;
									for (QSet<CIndividualReference>::const_iterator it = mIncompletelyAssociatedIndividualSet.constBegin(), itEnd = mIncompletelyAssociatedIndividualSet.constEnd(); it != itEnd && count < limit; ++it) {
										individualSet->insert(*it);
										++count;
									}
								}*/
							}



							//if (mIncompletelyHandledIndiIdCount != mIncompletelyAssociatedIndividualSet.size()) {
							//	bool debug = true;
							//}

							//if (mIncompletelyHandledIndiIdCount > 0 && individualSet->isEmpty()) {
							//	cint64 count = 0;
							//	for (cint64 i = 0; i < mIndiIdAssoDataVectorSize && count < mIncompletelyHandledIndiIdCount && count < limit; ++i) {
							//		CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mIndiIdAssoDataVector[i];
							//		if (indiAssData && !indiAssData->isCompletelyHandled()) {
							//			individualSet->insert(i);
							//			count++;
							//		}
							//	}

							//	count = count;
							//	bool debug = true;
							//}


							if (ontologyData->getIncompletelyHandledIndividualIdCount() <= 0 && !ontologyData->getZeroIncompletelyHandledIndividualIdCountDebugWritten()) {
								ontologyData->setZeroIncompletelyHandledIndividualIdCountDebugWritten(true);
								mFixedOntologyIdentifierDataHashLock.lockForWrite();
								ontologyData->incUsageCount();
								mFixedOntologyIdentifierDataHash.insert(ontologyIdentifier, ontologyData);
								mFixedOntologyIdentifierDataHashLock.unlock();
#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
								writeStringifiedRepresentativeCacheToFile();
#endif
							}
						}


						CCallbackData* callbackData = iace->getCallback();
						if (callbackData) {
							callbackData->doCallback();
						}
						return true;


					}
					else if (type == EVENTWRITEBACKENDASSOCIATIONENTRY) {
						CWriteBackendAssociationCachedEvent* wcde = (CWriteBackendAssociationCachedEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();
						CBackendRepresentativeMemoryCacheWriteData* newWriteData = (CBackendRepresentativeMemoryCacheWriteData*)wcde->getWriteData();
						cint64 dataWriteCount = newWriteData->getCount();

						CBackendRepresentativeMemoryCacheWriteData* dataLinkerIt = newWriteData;
						++mWriteDataCount;

						cint64 ontologyIdentifier = dataLinkerIt->getOntologyIdentifier();
						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = prepareOntologyDataUpdate(ontologyIdentifier);

						cint64 prevUpdatedIndiCount = mUpdatedIndiCount;
						bool allCachingSuccess = true;
						bool oneCachingSuccess = false;
						bool oneCachingExpSuccess = false;
						while (dataLinkerIt) {
							if (dataLinkerIt->getCacheWriteDataType() == CBackendRepresentativeMemoryCacheWriteData::BACKENDASSOCIATIONWRITEDATATYPE) {
								CBackendRepresentativeMemoryCacheLabelAssociationWriteData* baAsWrDa = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)dataLinkerIt;

								CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker = baAsWrDa->getTemporaryAssociationWriteDataLinker();
								CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = baAsWrDa->getTemporaryLabelWriteDataLinker();
								CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = baAsWrDa->getTemporaryCardinaltyWriteDataLinker();
								CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker = baAsWrDa->getTemporaryAssociationUseDataLinker();
								CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker = baAsWrDa->getTemporaryNominalIndirectConnectionDataLinker();

								installTemporaryLabels(tempLabelWriteDataLinker, ontologyData);
								installTemporaryCardinalities(tempCardWriteDataLinker, ontologyData);

								mTmpIndiAssocPrevUpdateId = mNextIndiUpdateId;
								mTmpIndiIndirectlyConnNomLabelItemHash.clear();

								bool cached = installAssociationUpdates(tempAssWriteDataLinker, ontologyData);
								cached |= installNominalIndirectConncetionUpdates(tempNomIndirectConnDataLinker, ontologyData);
								cached |= checkAssociationUsage(tempAssUseDataLinker, ontologyData);

								allCachingSuccess &= cached;
								oneCachingSuccess |= cached;
								oneCachingExpSuccess |= cached;
							}
							dataLinkerIt = (CBackendRepresentativeMemoryCacheWriteData*)dataLinkerIt->getNext();
						}

						if (!oneCachingSuccess) {
							++mEmptyWriteDataCount;
						}

						createReaderSlotUpdate(ontologyData, &mContext);
						cleanUnusedSlots(&mContext);


						mContext.releaseTemporaryMemoryPools(memoryPools);
						mCacheStat.setMemoryConsumption(mContext.getMemoryConsumption());

						cint64 updatedIndiCount = mUpdatedIndiCount - prevUpdatedIndiCount;
						LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Update %1 of representative cache, %2 updated individual association data, %3 remaining insufficiently handled individuals.").arg(mWriteDataCount).arg(updatedIndiCount).arg(ontologyData->getIncompletelyHandledIndividualIdCount()), this);

#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
						if (mWriteDataCount <= 50 || mWriteDataCount % 500 == 0) {
							//writeStringifiedRepresentativeCacheToFile();
						}
#endif

						return true;
					}
					return false;
				}







				void CBackendRepresentativeMemoryCache::writeStringifiedRepresentativeCacheToFile() {
					QString modelString = getRepresentativeCacheString();
					QByteArray data = modelString.toUtf8();
					QFile modelFileLatest("./Debugging/RepresentativeCache/individual-label-association-data.txt");
					if (modelFileLatest.open(QIODevice::WriteOnly)) {
						modelFileLatest.write(data);
					}
					QFile modelFile(QString("./Debugging/RepresentativeCache/individual-label-association-data-%1.txt").arg(mWriteDataCount));
					if (modelFile.open(QIODevice::WriteOnly)) {
						modelFile.write(data);
					}
				}



				QString CBackendRepresentativeMemoryCache::getRepresentativeCacheLabelItemString(cint64 labelType) {
					QString labelTypeString;
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_CONCEPT_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_CONCEPT_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {
						labelTypeString = "FULL_CONCEPT_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						labelTypeString = "NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL";
					}
					return labelTypeString;
				}

				QString CBackendRepresentativeMemoryCache::getRepresentativeCacheString() {
					QString repCacheString;
					for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = mOntologyIdentifierDataHash->constBegin(), itEnd = mOntologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
						cint64 ontId = it.key();
						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
						QString ontologyIdDataString = QString("Ontology: %1\r\n\r\n\r\n%2\r\n\r\n\r\n").arg(ontId).arg(getRepresentativeCacheString(ontologyData));
						repCacheString += ontologyIdDataString;
					}
					return repCacheString;
				}


				QString CBackendRepresentativeMemoryCache::getRepresentativeCacheString(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					QStringList labelStringList;
					for (cint64 labelType = 0; labelType < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++labelType) {
						QString labelTypeString = getRepresentativeCacheLabelItemString(labelType);
						labelTypeString = QString("\r\n\r\n%1:\r\n").arg(labelTypeString);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigLabelItemHash = ontologyData->getSignatureLabelItemHash(labelType);
						for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>::const_iterator it = sigLabelItemHash->constBegin(), itEnd = sigLabelItemHash->constEnd(); it != itEnd; ++it) {
							cint64 signature = it.key();
							CBackendRepresentativeMemoryLabelSignatureResolveCacheItem sigResolveItem = it.value();
							for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
								cint64 itemId = labelItemLinkerIt->getCacheEntryID();
								QString labelString = QString("Label %1 (with signature %2, association count %3): ").arg(itemId).arg(signature).arg(labelItemLinkerIt->getIndividualAssociationCount());
								CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = labelItemLinkerIt->getCacheValueLinker();
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
									if (labelValueLinkerIt != labelValueLinker) {
										labelString += ", ";
									}
									labelString += QString("%1(%2)").arg(cacheValue.getTag()).arg((cint64)cacheValue.getCacheValueIdentifier());
								}
								if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {
									CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* cardExtData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)labelItemLinkerIt->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
									if (cardExtData) {
										QString cardDataString;
										CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* roleTagCardDataHash = cardExtData->getRoleCardinalityDataHash();
										for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = roleTagCardDataHash->constBegin(), itEnd = roleTagCardDataHash->constEnd(); it != itEnd; ++it) {
											cint64 roleTag = it.key();
											CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
											if (!cardDataString.isEmpty()) {
												cardDataString += "; ";
											}
											cardDataString += QString("%1 -> # %2 with <= %3").arg(roleTag).arg(cardData->getExistentialMaxUsedCardinality()).arg(cardData->getMinimumRestrictingCardinality());
										}
										labelString += QString("\r\n\t\t%1").arg(cardDataString);
									}
								}




								labelTypeString += QString("%1\r\n").arg(labelString);
							}
						}
						labelStringList.append(labelTypeString);
					}


					QStringList indiStringList;
					for (cint64 i = 0; i < ontologyData->getIndividualIdAssoiationDataVectorSize(); ++i) {
					//for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>::const_iterator it = mIndiIdAssoDataHash->constBegin(), itEnd = mIndiIdAssoDataHash->constEnd(); it != itEnd; ++it) {
						cint64 indiId = i;
						CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
						if (indiAssData) {
							QString indiAssString = QString("\r\nIndividual %1:\r\n").arg(indiId);


							QString updateString = QString("\tupdate ids: individual association change: %1, total association change: %2, last indirectly connected individuals integration: %3\r\n").arg(indiAssData->getAssociationDataUpdateId()).arg(indiAssData->getCacheUpdateId()).arg(indiAssData->getLastIntegratedIndirectlyConnectedIndividualsChangeId());
							indiAssString += updateString;


							QStringList statusStrings;
							if (indiAssData->isCompletelySaturated()) {
								statusStrings += "completely saturated";
							} else {
								statusStrings += "incompletely saturated";
							}

							if (indiAssData->isCompletelyPropagated()) {
								statusStrings += "completely propagated";
							} else {
								statusStrings += "incompletely propagated";
							}

							if (indiAssData->isCompletelyHandled()) {
								statusStrings += "completely handled";
							} else {
								statusStrings += "incompletely handled";
							}

							if (indiAssData->hasIndirectlyConnectedIndividualIntegration()) {
								statusStrings += "indirectly connected individuals integration";
							}


							if (indiAssData->isIndirectlyConnectedNominalIndividual()) {
								statusStrings += "indirectly connected nominal";
							}


							QString statusString = QString("\tstatus: %1\r\n").arg(statusStrings.join(", "));
							indiAssString += statusString;


							if (indiAssData->getRepresentativeSameIndividualId() != indiAssData->getAssociatedIndividualId()) {
								indiAssString += QString("\trepresentative same individual id: %1\r\n").arg(indiAssData->getRepresentativeSameIndividualId());
							}


							for (cint64 labelType = 0; labelType < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++labelType) {
								CBackendRepresentativeMemoryLabelCacheItem* labelItem = indiAssData->getLabelCacheEntry(labelType);
								if (labelItem) {
									cint64 itemId = labelItem->getCacheEntryID();
									QString labelTypeString = getRepresentativeCacheLabelItemString(labelType);
									QString indiLabelAssString = QString("\tassociated %1 label: %2\r\n").arg(labelTypeString).arg(itemId);
									indiAssString += indiLabelAssString;
								}
							}


							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
							if (roleSetNeighbourArray) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
								for (cint64 i = 0; i < indexData->getArraySize(); ++i) {
									CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexData->getNeighbourRoleSetLabel(i);
									QString roleSetNeighbourIndiIdsString;
									roleSetNeighbourArray->at(i).visitNeighbourIndividualIds([&](cint64 id)->bool {
										if (!roleSetNeighbourIndiIdsString.isEmpty()) {
											roleSetNeighbourIndiIdsString += ", ";
										}
										roleSetNeighbourIndiIdsString += QString::number(id);
										return true;
									});
									QString indiArrayAssString = QString("\tneighbouring ids for label %1: %2\r\n").arg(neighbourRoleSetLabelItem->getCacheEntryID()).arg(roleSetNeighbourIndiIdsString);
									indiAssString += indiArrayAssString;
								}
							}

							CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* indConnData = ontologyData->getNominaIIndividualdIndirectConnectionDataHash()->value(indiId);
							if (indConnData) {
								QStringList indConnIndiIds;
								for (CXLinker<cint64>* indiLinkerIt = indConnData->getIndirectlyConnectedIndividualIdLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
									cint64 indiId = indiLinkerIt->getData();
									indConnIndiIds.append(QString::number(indiId));
								}
								QString indiIndConnDatatring = QString("\tindirectly connected individuals with change id %1: %2\r\n").arg(indConnData->getLastChangeId()).arg(indConnIndiIds.join(", "));
								indiAssString += indiIndConnDatatring;
							}




							indiStringList.append(indiAssString);



						}

					}

					QString statusString = QString("\tcache writes: %1,\r\n\ttotal association changes: %2,\r\n\tincompletely handled individuals: %3,\r\n\tincompatible association changes: %4,\r\n\tincompatible association checks: %5,\r\n\tmax individual association changes: %6\r\n")
						.arg(mWriteDataCount).arg(mNextIndiUpdateId).arg(ontologyData->getIncompletelyHandledIndividualIdCount()).arg(mUpdateIncompatibleIndiCount).arg(mCheckIncompatibleIndiCount).arg(ontologyData->getMaxIndividualAssociationDataUpdateCount());

					return QString("STATISTICS:\r\n\r\n%1\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nREPRESENATIVE LABELS:\r\n\r\n%2\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nINDIVIDUAL LABEL ASSOCIATIONS:\r\n%3\r\n").arg(statusString).arg(labelStringList.join("\r\n")).arg(indiStringList.join("\r\n"));
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
