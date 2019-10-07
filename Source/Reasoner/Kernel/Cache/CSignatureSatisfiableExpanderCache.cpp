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

#include "CSignatureSatisfiableExpanderCache.h"
#include "CSignatureSatisfiableExpanderCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCache::CSignatureSatisfiableExpanderCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread) {

					mConfig = config;

					mWriteDataCount = 0;
					mNextWriteCollectCount = 0;
					mCollectCount = 0;
					mStartWriteCollectCount = 1000;


					mSlotLinker = nullptr;
					mReaderLinker = nullptr;
					mCollectMemoryPools = nullptr;
					mCollectWriteData = nullptr;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					mSigItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mIncompatibleSigSet = CObjectParameterizingAllocator< CCACHINGSET<cint64>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mAlreadyExpSigSet = CObjectParameterizingAllocator< CCACHINGSET<cint64>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mSignatureReferCountSet = CObjectParameterizingAllocator< CCACHINGHASH<cint64,cint64>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					

					mNextMemoryLevelRequiredSignatureRefCount = CConfigDataReader::readConfigInteger(config,"Konclude.Cache.SatisfiableExpander.InitialMemoryLevelForIncreaseRequiredSignatureReferenceCount",200*1024*1024);
					mNextMemoryLevelIncreaseForRequiredSignatureReferenceCount = CConfigDataReader::readConfigInteger(config,"Konclude.Cache.SatisfiableExpander.NextMemoryLevelIncreaseForIncreaseRequiredSignatureReferenceCount",100*1024*1024);
					mNextCacheEntryRequiredSignatureRefCount = CConfigDataReader::readConfigInteger(config,"Konclude.Cache.SatisfiableExpander.RequiredSignatureReferenceCountIncrease",1);
					mNextCacheEntryRequiredSignatureReferenceCountIncrease = CConfigDataReader::readConfigInteger(config,"Konclude.Cache.SatisfiableExpander.RequiredSignatureReferenceCountIncrease",1);

					startThread(QThread::HighestPriority);
				}


				CSignatureSatisfiableExpanderCache::~CSignatureSatisfiableExpanderCache() {
				}



				CCacheStatistics* CSignatureSatisfiableExpanderCache::getCacheStatistics() {
					return &mCacheStat;
				}

				bool CSignatureSatisfiableExpanderCache::writeExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash) {
					if (canCreateCacheEntryForSignature(newSignature,context)) {
						if (!mSigItemHash->contains(newSignature)) {
							CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
							CSignatureSatisfiableExpanderCacheEntry* entry = nullptr;
							if (prevSignature != 0) {
								CSignatureSatisfiableExpanderCacheRedirectionItem* prevSigItem = mSigItemHash->value(prevSignature);
								if (prevSigItem) {
									entry = prevSigItem->getCacheEntry();
									if (!isCachingDataExpandable(context,entry,prevSignature,cacheValueList)) {
										// not compatible, only identical signatures
										mIncompatibleSigSet->insert(prevSignature);
										return false;
									}
									if (mAlreadyExpSigSet->contains(prevSignature)) {
										entry->setMultipleExpanded(true);
									}
								}
							}

							if (!entry) {
								entry = CObjectParameterizingAllocator< CSignatureSatisfiableExpanderCacheEntry,CSignatureSatisfiableExpanderCacheContext* >::allocateAndConstructAndParameterize(memMan,context);
							}
							
							writeExpanderCachingData(context,entry,prevSignature != 0,cacheValueList,depHash);
							CSignatureSatisfiableExpanderCacheRedirectionItem* sigItem = CObjectAllocator< CSignatureSatisfiableExpanderCacheRedirectionItem >::allocateAndConstruct(memMan);
							sigItem->initRedirectionItem(entry,newSignature,entry->getExpanderCacheValueCount());
							mSigItemHash->insert(newSignature,sigItem);
							mAlreadyExpSigSet->insert(prevSignature);
							//mHasherItemHash.insert(CSignatureSatisfiableExpanderCacheHasher(entry->getExpanderCacheValueLinker(),entry->getExpanderCacheValueCount()),sigItem);
							return true;
						} else {
							mIncompatibleSigSet->insert(newSignature);
						}
					}
					return false;
				}



				bool CSignatureSatisfiableExpanderCache::isCachingDataExpandable(CSignatureSatisfiableExpanderCacheContext* context, CSignatureSatisfiableExpanderCacheEntry* entry, cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList) {
					if (mIncompatibleSigSet->contains(signature) || entry->hasMultipleExpanded()) {
						CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* tagHash = entry->getTagExpanderCacheValueHash();
						cint64 prevCount = entry->getExpanderCacheValueCount();
						while (prevCount-- > 0 && !cacheValueList->isEmpty()) { 
							const CCacheValue& cacheValue = cacheValueList->takeFirst();
							cint64 tag = cacheValue.getTag();
							CExpanderCacheValueLinker* contCacheValue = tagHash->value(tag);
							if (!contCacheValue || *contCacheValue->getCacheValue() != cacheValue) {
								return false;
							}
						}
					} else {
						cint64 prevCount = entry->getExpanderCacheValueCount();
						while (prevCount-- > 0 && !cacheValueList->isEmpty()) { 
							const CCacheValue& cacheValue = cacheValueList->takeFirst();
						}
					}
					return true;
				}


				void CSignatureSatisfiableExpanderCache::writeExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, CSignatureSatisfiableExpanderCacheEntry* entry, bool extending, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash) {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* tagHash = entry->getTagExpanderCacheValueHash();
					CCACHINGHASH<cint64,cint64>::const_iterator itDepEnd = depHash->constEnd();

					CExpanderCacheValueLinker* lastExpCacheValueLinker = nullptr;
					CExpanderCacheValueLinker* firstExpCacheValueLinker = nullptr;


					CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* newTagHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CExpanderCacheValueLinker*>,CContext* >::allocateAndConstructAndParameterize(memMan,context);
					if (tagHash) {
						*newTagHash = *tagHash;
					}

					mCacheStat.incCacheEntriesCount();


					CCACHINGHASH<cint64,CCacheValue>* tagCacheValueHash = nullptr;


					for (CCACHINGLIST<CCacheValue>::const_iterator it = cacheValueList->constBegin(), itEnd = cacheValueList->constEnd(); it != itEnd; ++it) {
						const CCacheValue& cacheValue = *it;
						cint64 tag = cacheValue.getTag();

						if (!newTagHash->contains(tag)) {
							CExpanderCacheValueLinker* expCacheValue = CObjectParameterizingAllocator< CExpanderCacheValueLinker,CSignatureSatisfiableExpanderCacheContext* >::allocateAndConstructAndParameterize(memMan,context);
							expCacheValue->setCacheValue(cacheValue);

							//if (cacheValue.getIdentification() == 0 || tag == 0) {
							//	bool bug = true;
							//}


							CCACHINGHASH<cint64,cint64>::const_iterator itDep = depHash->constFind(tag);
							while (itDep != itDepEnd && itDep.key() == tag) {

								cint64 depTag = itDep.value();
								CExpanderCacheValueLinker* depExpCacheValueLinker = newTagHash->value(depTag);
								if (!depExpCacheValueLinker) {
									if (!tagCacheValueHash) {
										tagCacheValueHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CCacheValue>,CContext* >::allocateAndConstructAndParameterize(memMan,context);
										CCACHINGLIST<CCacheValue>::const_iterator it2 = it;
										for (++it2; it2 != itEnd; ++it2) {
											const CCacheValue& cv = *it2;
											cint64 cvTag = cv.getTag();
											tagCacheValueHash->insert(cvTag,cv);
										}
									}
									const CCacheValue& depCacheValue = tagCacheValueHash->value(depTag);
									addExpanderCachingData(context,depCacheValue,newTagHash,depHash,tagCacheValueHash,lastExpCacheValueLinker,firstExpCacheValueLinker);
									depExpCacheValueLinker = newTagHash->value(depTag);
								}
								expCacheValue->addExpanderDependency(depExpCacheValueLinker);
								++itDep;
							}

							newTagHash->insert(tag,expCacheValue);

							if (firstExpCacheValueLinker) {
								lastExpCacheValueLinker->append(expCacheValue);
								lastExpCacheValueLinker = expCacheValue;
							} else {
								firstExpCacheValueLinker = expCacheValue;
								lastExpCacheValueLinker = firstExpCacheValueLinker;
							}
						}
					}

					entry->setTagExpanderCacheValueHash(newTagHash);
					entry->appendExpanderCacheValueLinker(firstExpCacheValueLinker);

				}




				void CSignatureSatisfiableExpanderCache::addExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, const CCacheValue& cacheValue, CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* newTagHash, CCACHINGHASH<cint64,cint64>* depHash, CCACHINGHASH<cint64,CCacheValue>* tagCacheValueHash, CExpanderCacheValueLinker*& lastExpCacheValueLinker, CExpanderCacheValueLinker*& firstExpCacheValueLinker) {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					CCACHINGHASH<cint64,cint64>::const_iterator itDepEnd = depHash->constEnd();
					cint64 tag = cacheValue.getTag();

					if (!newTagHash->contains(tag)) {
						CExpanderCacheValueLinker* expCacheValue = CObjectParameterizingAllocator< CExpanderCacheValueLinker,CSignatureSatisfiableExpanderCacheContext* >::allocateAndConstructAndParameterize(memMan,context);
						expCacheValue->setCacheValue(cacheValue);

						//if (cacheValue.getIdentification() == 0 || tag == 0) {
						//	bool bug = true;
						//}

						CCACHINGHASH<cint64,cint64>::const_iterator itDep = depHash->constFind(tag);
						while (itDep != itDepEnd && itDep.key() == tag) {
							cint64 depTag = itDep.value();
							CExpanderCacheValueLinker* depExpCacheValueLinker = newTagHash->value(depTag);
							if (!depExpCacheValueLinker) {
								const CCacheValue& depCacheValue = tagCacheValueHash->value(depTag);
								addExpanderCachingData(context,depCacheValue,newTagHash,depHash,tagCacheValueHash,lastExpCacheValueLinker,firstExpCacheValueLinker);
								depExpCacheValueLinker = newTagHash->value(depTag);
							}
							expCacheValue->addExpanderDependency(depExpCacheValueLinker);
							++itDep;
						}

						newTagHash->insert(tag,expCacheValue);

						if (firstExpCacheValueLinker) {
							lastExpCacheValueLinker->append(expCacheValue);
							lastExpCacheValueLinker = expCacheValue;
						} else {
							firstExpCacheValueLinker = expCacheValue;
							lastExpCacheValueLinker = firstExpCacheValueLinker;
						}
					}
				}




				bool CSignatureSatisfiableExpanderCache::writeSatisfiableBranchedCachingData(CSignatureSatisfiableExpanderCacheContext* context, cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedValueList) {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					CSignatureSatisfiableExpanderCacheRedirectionItem* sSigItem = mSigItemHash->value(signature);
					if (sSigItem) {
						CSignatureSatisfiableExpanderCacheEntry* entry = sSigItem->getCacheEntry();

						if (isCachingDataExpandable(context,entry,signature,cacheValueList)) {

							if (!branchedValueList || branchedValueList->isEmpty()) {
								entry->setSatisfiableWithoutBranchedConcepts(true);
							} else {
								CExpanderBranchedLinker* expBranch = CObjectParameterizingAllocator< CExpanderBranchedLinker,CSignatureSatisfiableExpanderCacheContext* >::allocateAndConstructAndParameterize(memMan,context);
								for (CCACHINGLIST<CCacheValue>::const_iterator it = branchedValueList->constBegin(), itEnd = branchedValueList->constEnd(); it != itEnd; ++it) {
									const CCacheValue& cacheValue = *it;
									expBranch->appendCacheValue(cacheValue);
								}
								entry->appendExpanderBranchedLinker(expBranch);
							}

							entry->setSatisfiable(true);
							return true;
						}
					}
					return false;
				}




				void CSignatureSatisfiableExpanderCache::createReaderSlotUpdate(CSignatureSatisfiableExpanderCacheContext* context) {
					CMemoryPoolProvider* memProv = context->getMemoryPoolAllocationManager();
					CSignatureSatisfiableExpanderCacheSlotItem* slot = CObjectMemoryPoolAllocator<CSignatureSatisfiableExpanderCacheSlotItem>::allocateAndConstructWithMemroyPool(memProv);
					CMemoryPoolContainerAllocationManager slotMemMan(slot,memProv);
					CMemoryAllocationManager* memMan = &slotMemMan;
					CContextBase tmpContext(memMan);
					CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* slotSigItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&tmpContext);
					*slotSigItemHash = *mSigItemHash;
					slotSigItemHash->detach();
					slot->setSignatureItemHash(slotSigItemHash);
					// currently not used
					//CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* slotHasherItemHash = CObjectParameterizingAllocator< CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>,CContext* >::allocateAndConstructAndParameterize(memMan,&tmpContext);
					//*slotHasherItemHash = *mHasherItemHash;
					//slotHasherItemHash->detach();
					//slot->setHasherItemHash(slotHasherItemHash);
					if (mSlotLinker) {
						mSlotLinker->append(slot);
					} else {
						mSlotLinker = slot;
					}
					CSignatureSatisfiableExpanderCacheReader* readerLinkerIt = mReaderLinker;
					while (readerLinkerIt) {
						slot->incReader();
						readerLinkerIt->updateSlot(slot);
						readerLinkerIt = readerLinkerIt->getNext();
					}
				}


				void CSignatureSatisfiableExpanderCache::cleanUnusedSlots(CSignatureSatisfiableExpanderCacheContext* context) {
					CMemoryPoolAllocationManager* memMan = context->getMemoryPoolAllocationManager();
					CSignatureSatisfiableExpanderCacheSlotItem* slotLinkerIt = mSlotLinker;
					CSignatureSatisfiableExpanderCacheSlotItem* lastSlotLinker = nullptr;
					while (slotLinkerIt) {
						CSignatureSatisfiableExpanderCacheSlotItem* tmpSlotLinker = nullptr;
						if (!slotLinkerIt->hasCacheReaders()) {
							tmpSlotLinker = slotLinkerIt;
						} 
						CSignatureSatisfiableExpanderCacheSlotItem* nextSlotLinker = slotLinkerIt->getNext();
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



				CSignatureSatisfiableExpanderCacheReader* CSignatureSatisfiableExpanderCache::createCacheReader() {
					CSignatureSatisfiableExpanderCacheReader* reader = new CSignatureSatisfiableExpanderCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = reader->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return reader;
				}


				CSignatureSatisfiableExpanderCacheWriter* CSignatureSatisfiableExpanderCache::createCacheWriter() {
					CSignatureSatisfiableExpanderCacheWriter* writer = new CSignatureSatisfiableExpanderCacheWriter(this);
					return writer;
				}



				CSignatureSatisfiableExpanderCache* CSignatureSatisfiableExpanderCache::writeCachedData(CSignatureSatisfiableExpanderCacheEntryWriteData* writeData, CMemoryPool* memoryPools) {
#ifdef KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING
					CSignatureSatisfiableExpanderCacheEntryWriteData* dataLinkerIt = writeData;
					bool allCachingSuccess = true;
					bool oneCachingSuccess = false;
					bool oneCachingExpSuccess = false;
					while (dataLinkerIt) {
						if (dataLinkerIt->getCacheWriteDataType() == CSignatureSatisfiableExpanderCacheEntryWriteData::SATEXPANDCACHEWRITEDATATYPE) {
							CSignatureSatisfiableExpanderCacheEntryExpandWriteData* exWrDa = (CSignatureSatisfiableExpanderCacheEntryExpandWriteData*)dataLinkerIt;
							cint64 prevSignature = exWrDa->getPrevSignature();
							cint64 newSignature = exWrDa->getNewSignature();
							CCACHINGLIST<CCacheValue>* cacheValueList = exWrDa->getCacheValueList();
							CCACHINGHASH<cint64,cint64>* depHash = exWrDa->getDepHash();

							bool cached = writeExpanderCachingData(&mContext,prevSignature,newSignature,cacheValueList,depHash);
							allCachingSuccess &= cached;
							oneCachingSuccess |= cached;
							oneCachingExpSuccess |= cached;
						} else {
							CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData* saBrWrDa = (CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData*)dataLinkerIt;
							cint64 signature = saBrWrDa->getSignature();
							CCACHINGLIST<CCacheValue>* cacheValueList = saBrWrDa->getCacheValueList();
							CCACHINGLIST<CCacheValue>* branchedValueList = saBrWrDa->getBranchedValueList();

							bool cached = writeSatisfiableBranchedCachingData(&mContext,signature,cacheValueList,branchedValueList);
							allCachingSuccess &= cached;
							oneCachingSuccess |= cached;
						}
						dataLinkerIt = (CSignatureSatisfiableExpanderCacheEntryWriteData*)dataLinkerIt->getNext();
					}

					if (oneCachingExpSuccess) {
						createReaderSlotUpdate(&mContext);
						cleanUnusedSlots(&mContext);
					}
#else 
					postEvent(new CWriteCachedDataEvent(writeData,memoryPools));
#endif
					return this;
				}



				CSignatureSatisfiableExpanderCache* CSignatureSatisfiableExpanderCache::writeExpandCached(cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash, CMemoryPool* memoryPools) {
#ifdef KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING
					if (writeExpanderCachingData(&mContext,prevSignature,newSignature,cacheValueList,depHash)) {
						createReaderSlotUpdate(&mContext);
						cleanUnusedSlots(&mContext);
					}
#else 
					postEvent(new CWriteExpandCachedEvent(prevSignature,newSignature,cacheValueList,depHash,memoryPools));
#endif
					return this;
				}

				CSignatureSatisfiableExpanderCache* CSignatureSatisfiableExpanderCache::writeSatisfiableBranchCached(cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedList, CMemoryPool* memoryPools) {
#ifdef KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING
					writeSatisfiableBranchedCachingData(&mContext,signature,cacheValueList,branchedList);
#else 
					postEvent(new CWriteSatisfiableBranchCachedEvent(signature,cacheValueList,branchedList,memoryPools));
#endif
					return this;
				}


				bool CSignatureSatisfiableExpanderCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;
					} if (type == EVENTWRITEEXPANDCACHEDENTRY) {
						CWriteExpandCachedEvent* wece = (CWriteExpandCachedEvent*)event;
						cint64 prevSignature = wece->getPrevSignature();
						cint64 newSignature = wece->getNewSignature();
						CCACHINGLIST<CCacheValue>* cacheValueList = wece->getCacheValueList();
						CCACHINGHASH<cint64,cint64>* depHash = wece->getDepHash();
						CMemoryPool* memoryPools = wece->getMemoryPools();

						if (writeExpanderCachingData(&mContext,prevSignature,newSignature,cacheValueList,depHash)) {
							createReaderSlotUpdate(&mContext);
							cleanUnusedSlots(&mContext);
						}

						mContext.getMemoryPoolAllocationManager()->releaseTemporaryMemoryPools(memoryPools);
						return true;

					} else if (type == EVENTWRITESATISFIABLEBRANCHCACHEDENTRY) {
						CWriteSatisfiableBranchCachedEvent* wsbce = (CWriteSatisfiableBranchCachedEvent*)event;
						cint64 signature = wsbce->getSignature();
						CCACHINGLIST<CCacheValue>* cacheValueList = wsbce->getCacheValueList();
						CCACHINGLIST<CCacheValue>* branchedValueList = wsbce->getBranchedValueList();
						CMemoryPool* memoryPools = wsbce->getMemoryPools();

						writeSatisfiableBranchedCachingData(&mContext,signature,cacheValueList,branchedValueList);

						mContext.getMemoryPoolAllocationManager()->releaseTemporaryMemoryPools(memoryPools);
						return true;
					} else if (type == EVENTWRITECACHEDDATAENTRY) {
						CWriteCachedDataEvent* wcde = (CWriteCachedDataEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();
						CSignatureSatisfiableExpanderCacheEntryWriteData* newWriteData = (CSignatureSatisfiableExpanderCacheEntryWriteData*)wcde->getWriteData();
						cint64 dataWriteCount = newWriteData->getCount();

						mCollectCount += dataWriteCount;
						mCollectMemoryPools = memoryPools->append(mCollectMemoryPools);
						mCollectWriteData = (CSignatureSatisfiableExpanderCacheEntryWriteData*)newWriteData->append(mCollectWriteData);

						if (mCollectCount >= mNextWriteCollectCount) {
							if (mWriteDataCount > mStartWriteCollectCount) {
								++mNextWriteCollectCount;
							}

							CSignatureSatisfiableExpanderCacheEntryWriteData* dataLinkerIt = mCollectWriteData; 

							bool allCachingSuccess = true;
							bool oneCachingSuccess = false;
							bool oneCachingExpSuccess = false;
							while (dataLinkerIt) {
								if (dataLinkerIt->getCacheWriteDataType() == CSignatureSatisfiableExpanderCacheEntryWriteData::SATEXPANDCACHEWRITEDATATYPE) {
									CSignatureSatisfiableExpanderCacheEntryExpandWriteData* exWrDa = (CSignatureSatisfiableExpanderCacheEntryExpandWriteData*)dataLinkerIt;
									cint64 prevSignature = exWrDa->getPrevSignature();
									cint64 newSignature = exWrDa->getNewSignature();
									CCACHINGLIST<CCacheValue>* cacheValueList = exWrDa->getCacheValueList();
									CCACHINGHASH<cint64,cint64>* depHash = exWrDa->getDepHash();

									bool cached = writeExpanderCachingData(&mContext,prevSignature,newSignature,cacheValueList,depHash);
									allCachingSuccess &= cached;
									oneCachingSuccess |= cached;
									oneCachingExpSuccess |= cached;
								} else {
									CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData* saBrWrDa = (CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData*)dataLinkerIt;
									cint64 signature = saBrWrDa->getSignature();
									CCACHINGLIST<CCacheValue>* cacheValueList = saBrWrDa->getCacheValueList();
									CCACHINGLIST<CCacheValue>* branchedValueList = saBrWrDa->getBranchedValueList();

									bool cached = writeSatisfiableBranchedCachingData(&mContext,signature,cacheValueList,branchedValueList);
									allCachingSuccess &= cached;
									oneCachingSuccess |= cached;
								}
								dataLinkerIt = (CSignatureSatisfiableExpanderCacheEntryWriteData*)dataLinkerIt->getNext();
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




				cint64 CSignatureSatisfiableExpanderCache::getRequiredSignatureReferCountForNextCacheEntryCreation(CSignatureSatisfiableExpanderCacheContext* context) {
					if (context->getMemoryConsumption() >= mNextMemoryLevelRequiredSignatureRefCount) {
						mNextCacheEntryRequiredSignatureRefCount += mNextCacheEntryRequiredSignatureReferenceCountIncrease;
						mNextMemoryLevelRequiredSignatureRefCount += mNextMemoryLevelIncreaseForRequiredSignatureReferenceCount;
					}
					return mNextCacheEntryRequiredSignatureRefCount;
				}

				cint64 CSignatureSatisfiableExpanderCache::canCreateCacheEntryForSignature(cint64 singature, CSignatureSatisfiableExpanderCacheContext* context) {
					cint64& refCount = (*mSignatureReferCountSet)[singature];
					++refCount;
					if (refCount >= getRequiredSignatureReferCountForNextCacheEntryCreation(context)) {
						return true;
					}
					return false;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
