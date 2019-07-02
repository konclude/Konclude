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

#include "CBackendRepresentativeMemoryCache.h"
#include "CBackendRepresentativeMemoryCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCache::CBackendRepresentativeMemoryCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread) {
					mWriteDataCount = 0;
					mNextWriteCollectCount = 0;
					mCollectCount = 0;
					mStartWriteCollectCount = 1000;

					mNextEntryID = 1;
					mConfig = config;

					mSlotLinker = nullptr;
					mReaderLinker = nullptr;
					mCollectMemoryPools = nullptr;
					mCollectWriteData = nullptr;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					mSigLabelItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mSigCardItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					
					startThread(QThread::HighestPriority);
				}


				CBackendRepresentativeMemoryCache::~CBackendRepresentativeMemoryCache() {
				}



				CCacheStatistics* CBackendRepresentativeMemoryCache::getCacheStatistics() {
					return &mCacheStat;
				}

				void CBackendRepresentativeMemoryCache::createReaderSlotUpdate(CBackendRepresentativeMemoryCacheContext* context) {
					CMemoryPoolProvider* memProv = context->getMemoryPoolAllocationManager();
					CBackendRepresentativeMemoryCacheSlotItem* slot = CObjectMemoryPoolAllocator<CBackendRepresentativeMemoryCacheSlotItem>::allocateAndConstructWithMemroyPool(memProv);
					CMemoryPoolContainerAllocationManager slotMemMan(slot,memProv);
					CMemoryAllocationManager* memMan = &slotMemMan;
					CContextBase tmpContext(memMan);
					CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* slotSigLabelItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&tmpContext);
					CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* slotSigCardItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&tmpContext);
					*slotSigLabelItemHash = *mSigLabelItemHash;
					*slotSigCardItemHash = *mSigCardItemHash;
					slotSigLabelItemHash->detach();
					slotSigCardItemHash->detach();
					slot->setSignatureLabelItemHash(slotSigLabelItemHash);
					slot->setSignatureCardinalityItemHash(slotSigCardItemHash);
					if (mSlotLinker) {
						mSlotLinker->append(slot);
					} else {
						mSlotLinker = slot;
					}
					CBackendRepresentativeMemoryCacheReader* readerLinkerIt = mReaderLinker;
					while (readerLinkerIt) {
						slot->incReader();
						readerLinkerIt->updateSlot(slot);
						readerLinkerIt = readerLinkerIt->getNext();
					}
				}


				void CBackendRepresentativeMemoryCache::cleanUnusedSlots(CBackendRepresentativeMemoryCacheContext* context) {
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
							CMemoryPool* memoryPools = tmpSlotLinker->getMemoryPools();
							memMan->releaseTemporaryMemoryPools(memoryPools);
						} else {
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


				CBackendRepresentativeMemoryCacheWriter* CBackendRepresentativeMemoryCache::createCacheWriter() {
					CBackendRepresentativeMemoryCacheWriter* writer = new CBackendRepresentativeMemoryCacheWriter(this);
					return writer;
				}



				CBackendRepresentativeMemoryCache* CBackendRepresentativeMemoryCache::writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteBackendAssociationCachedEvent(writeData,memoryPools));
					return this;
				}




				void CBackendRepresentativeMemoryCache::installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context) {
					for (CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinkerIt = tempCardWriteDataLinker; tempCardWriteDataLinkerIt; tempCardWriteDataLinkerIt = tempCardWriteDataLinkerIt->getNext()) {
						cint64 signature = tempCardWriteDataLinkerIt->getSignature();
						CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = tempCardWriteDataLinkerIt->getCardinalityCacheValueLinker();
						cint64 cardValueCount = tempCardWriteDataLinkerIt->getCardinalityCacheValueCount();
						CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*& sigResolveItem = (*mSigCardItemHash)[signature];
						if (!sigResolveItem) {
							sigResolveItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem,CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
						}

						CBackendRepresentativeMemoryCardinalityCacheItem* refLabelItem = nullptr;
						for (CBackendRepresentativeMemoryCardinalityCacheItem* cardItemLinkerIt = sigResolveItem->getCardinalityCacheItems(); cardItemLinkerIt && !refLabelItem; cardItemLinkerIt = cardItemLinkerIt->getNext()) {
							if (cardItemLinkerIt->getCardinalityCacheValueCount() == cardValueCount) {
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* cardValueHash = cardItemLinkerIt->getTagCardinalityCacheValueHash(false);
								bool identical = true;
								for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt && identical; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
									const CCacheValue& cacheValue = cardValueLinkerIt->getCacheValue();
									cint64 valueTag = cacheValue.getTag();
									CBackendRepresentativeMemoryCardinalityValueLinker* cardValue = cardValueHash->value(valueTag);
									if (!cardValue || cardValue->getCacheValue() != cacheValue || cardValue->getFreeCardinality() != cardValueLinkerIt->getFreeCardinality()) {
										identical = false;
									}
								}

								if (identical) {
									refLabelItem = cardItemLinkerIt;
								}
							}
						}

						if (!refLabelItem) {
							refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCardinalityCacheItem,CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
							refLabelItem->initCacheEntry(signature,mNextEntryID++);

							CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* cardValueHash = refLabelItem->getTagCardinalityCacheValueHash(true);
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {

								CBackendRepresentativeMemoryCardinalityValueLinker* newCardValueLinker = CObjectAllocator< CBackendRepresentativeMemoryCardinalityValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								newCardValueLinker->initCardinalityValueLinker(cardValueLinkerIt->getCacheValue(),cardValueLinkerIt->getFreeCardinality());
								cardValueHash->insert(cardValueLinkerIt->getCacheValue().getTag(),newCardValueLinker);
								refLabelItem->addCardinalityCacheValueLinker(newCardValueLinker);
							}

							sigResolveItem->appendCardinalityCacheItem(refLabelItem);
						}

						tempCardWriteDataLinkerIt->setTemporaryData(refLabelItem);
					}					
				}






				void CBackendRepresentativeMemoryCache::installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context) {
					for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinkerIt = tempLabelWriteDataLinker; tempLabelWriteDataLinkerIt; tempLabelWriteDataLinkerIt = tempLabelWriteDataLinkerIt->getNext()) {
						cint64 signature = tempLabelWriteDataLinkerIt->getSignature();
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = tempLabelWriteDataLinkerIt->getDeterministicCacheValueLinker();
						cint64 labelValueCount = tempLabelWriteDataLinkerIt->getDeterministicCacheValueCount();
						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*& sigResolveItem = (*mSigLabelItemHash)[signature];
						if (!sigResolveItem) {
							sigResolveItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelSignatureResolveCacheItem,CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
						}

						CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = nullptr;
						for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem->getLabelItems(); labelItemLinkerIt && !refLabelItem; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
							if (labelItemLinkerIt->getDeterministicCacheValueCount() == labelValueCount) {
								CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getDeterministicTagCacheValueHash(false);
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
							refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem,CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(),context);
							refLabelItem->initCacheEntry(signature,mNextEntryID++);

							CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getDeterministicTagCacheValueHash(true);
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

								CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
								labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(),newValueLinker);
								refLabelItem->addDeterministicCacheValueLinker(newValueLinker);
							}

							sigResolveItem->appendLabelItem(refLabelItem);
						}

						if (!refLabelItem->isCompletelyHandled() && tempLabelWriteDataLinkerIt->isCompletelyHandled()) {
							refLabelItem->setCompletelyHandled(true);
						}
						if (!refLabelItem->isCompletelySaturated() && tempLabelWriteDataLinkerIt->isCompletelySaturated()) {
							refLabelItem->setCompletelySaturated(true);
						}

						tempLabelWriteDataLinkerIt->setTemporaryData(refLabelItem);
					}

				}



				bool CBackendRepresentativeMemoryCache::installAssociations(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context) {
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();


						CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = tempAssWriteDataLinkerIt->getReferredLabelData();
						if (!referredLabelCacheItem) {
							CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = tempAssWriteDataLinkerIt->getReferredTemporaryLabelData();
							referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
						}

						CBackendRepresentativeMemoryCardinalityCacheItem* referredCardCacheItem = tempAssWriteDataLinkerIt->getReferredCardinalityData();
						if (!referredCardCacheItem && tempAssWriteDataLinkerIt->getReferredTemporaryCardinalityData()) {
							CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tmpRefferedCardItem = tempAssWriteDataLinkerIt->getReferredTemporaryCardinalityData();
							referredCardCacheItem = (CBackendRepresentativeMemoryCardinalityCacheItem*)tmpRefferedCardItem->getTemporaryData();
						}

						CIndividualProcessData* indiProcData = (CIndividualProcessData*)individual->getIndividualData();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = (CBackendRepresentativeMemoryCacheIndividualAssociationData*)indiProcData->getBackendCachingData();


						if (!associationData || associationData->getBackendLabelCacheEntry() != referredLabelCacheItem || 
								associationData->getBackendCardinalityCacheEntry() != referredCardCacheItem || 
								associationData->isCompletelyHandled() != tempAssWriteDataLinkerIt->isCompletelyHandled() || 
								associationData->isCompletelySaturated() != tempAssWriteDataLinkerIt->isCompletelySaturated()) {

							CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
							locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(context->getMemoryAllocationManager());
							if (associationData) {
								locAssociationData->initAssociationData(associationData);
							} else {
								locAssociationData->initAssociationData(individual);
							}

							
							bool movedRepLabel = false;
							if (!associationData || locAssociationData->getBackendLabelCacheEntry() != referredLabelCacheItem || 
									associationData->getBackendCardinalityCacheEntry() != referredCardCacheItem) {

								locAssociationData->setBackendLabelCacheEntry(referredLabelCacheItem);
								locAssociationData->setBackendCardinalityCacheEntry(referredCardCacheItem);
								movedRepLabel = true;

								locAssociationData->setCompletelyHandled(tempAssWriteDataLinkerIt->isCompletelyHandled());
								locAssociationData->setCompletelySaturated(tempAssWriteDataLinkerIt->isCompletelySaturated());
								associationsUpdated = true;

							} else {
								if (!locAssociationData->isCompletelyHandled() && tempAssWriteDataLinkerIt->isCompletelyHandled()) {
									locAssociationData->setCompletelyHandled(true);
									associationsUpdated = true;
								}
								if (!locAssociationData->isCompletelySaturated() && tempAssWriteDataLinkerIt->isCompletelySaturated()) {
									locAssociationData->setCompletelySaturated(true);
									associationsUpdated = true;
								}
							}

							if (locAssociationData->isCompletelyHandled()) {
								if (locAssociationData->isIncompletelyMarked()) {
									mIncompletelyAssociatedIndividualSet.remove(individual);
									locAssociationData->setIncompletelyMarked(false);
								}
							} else {
								if (!locAssociationData->isIncompletelyMarked()) {
									mIncompletelyAssociatedIndividualSet.insert(individual);
									locAssociationData->setIncompletelyMarked(true);
								}
							}

							indiProcData->setBackendCachingData(locAssociationData);

						}
					}
					return associationsUpdated;
				}


				bool CBackendRepresentativeMemoryCache::getIncompletlyAssociationCachedIndividuals(QSet<CIndividual*>* individualSet) {
					CBlockingCallbackData callbackData;
					postEvent(new CRetrieveIncompletelyAssociationCachedEvent(&callbackData,individualSet));
					callbackData.waitForCallback();
					return true;
				}


				bool CBackendRepresentativeMemoryCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;

					} else if (type == EVENTRETRIEVEINCOMPLETELYASSOCIATIONCACHED) {
						CRetrieveIncompletelyAssociationCachedEvent* iace = (CRetrieveIncompletelyAssociationCachedEvent*)event;
						QSet<CIndividual*>* individualSet = iace->getIndividualSet();
						if (individualSet) {
							*individualSet = mIncompletelyAssociatedIndividualSet;
						}
						CCallbackData* callbackData = iace->getCallback();
						if (callbackData) {
							callbackData->doCallback();
						}
						return true;					


					} else if (type == EVENTWRITEBACKENDASSOCIATIONENTRY) {
						CWriteBackendAssociationCachedEvent* wcde = (CWriteBackendAssociationCachedEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();
						CBackendRepresentativeMemoryCacheWriteData* newWriteData = (CBackendRepresentativeMemoryCacheWriteData*)wcde->getWriteData();
						cint64 dataWriteCount = newWriteData->getCount();

						mCollectCount += dataWriteCount;
						mCollectMemoryPools = memoryPools->append(mCollectMemoryPools);
						mCollectWriteData = (CBackendRepresentativeMemoryCacheWriteData*)newWriteData->append(mCollectWriteData);

						if (mCollectCount >= mNextWriteCollectCount) {
							if (mWriteDataCount > mStartWriteCollectCount) {
								++mNextWriteCollectCount;
							}

							CBackendRepresentativeMemoryCacheWriteData* dataLinkerIt = mCollectWriteData; 

							bool allCachingSuccess = true;
							bool oneCachingSuccess = false;
							bool oneCachingExpSuccess = false;
							while (dataLinkerIt) {
								if (dataLinkerIt->getCacheWriteDataType() == CBackendRepresentativeMemoryCacheWriteData::BACKENDASSOCIATIONWRITEDATATYPE) {
									CBackendRepresentativeMemoryCacheLabelAssociationWriteData* baAsWrDa = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)dataLinkerIt;

									CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker = baAsWrDa->getTemporaryAssociationWriteDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = baAsWrDa->getTemporaryLabelWriteDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = baAsWrDa->getTemporaryCardinaltyWriteDataLinker();

									installTemporaryLabels(tempLabelWriteDataLinker,&mContext);
									installTemporaryCardinalities(tempCardWriteDataLinker,&mContext);
									bool cached = installAssociations(tempAssWriteDataLinker,&mContext);

									allCachingSuccess &= cached;
									oneCachingSuccess |= cached;
									oneCachingExpSuccess |= cached;
								}
								dataLinkerIt = (CBackendRepresentativeMemoryCacheWriteData*)dataLinkerIt->getNext();
							}

							if (oneCachingExpSuccess) {
								createReaderSlotUpdate(&mContext);
								cleanUnusedSlots(&mContext);
							}

							mContext.releaseTemporaryMemoryPools(mCollectMemoryPools);
							mCacheStat.setMemoryConsumption(mContext.getMemoryConsumption());

							mCollectWriteData = nullptr;
							mCollectMemoryPools = nullptr;
							mCollectCount = 0;
						}

						++mWriteDataCount;
						return true;
					}
					return false;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
