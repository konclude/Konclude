/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CReuseCompletionGraphCache.h"
#include "CReuseCompletionGraphCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CReuseCompletionGraphCache::CReuseCompletionGraphCache(QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread) {
					startThread(QThread::HighestPriority);

					mSlotLinker = nullptr;
					mReaderLinker = nullptr;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					mEntyHash = CObjectParameterizingAllocator< CReuseCompletionGraphCompatibilityEntryHash,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mEntyList = CObjectParameterizingAllocator< CCACHINGLIST<CReuseCompletionGraphCacheEntry*>,CContext* >::allocateAndConstructAndParameterize(memMan,&mContext);
					mEntryCount = 0;
				}


				CReuseCompletionGraphCache::~CReuseCompletionGraphCache() {
				}





				void CReuseCompletionGraphCache::createReaderSlotUpdate(CReuseCompletionGraphCacheContext* context) {
					CMemoryPoolProvider* memProv = context->getMemoryPoolAllocationManager();
					CReuseCompletionGraphCacheSlotItem* slot = CObjectMemoryPoolAllocator<CReuseCompletionGraphCacheSlotItem>::allocateAndConstructWithMemroyPool(memProv);
					CMemoryPoolContainerAllocationManager slotMemMan(slot,memProv);
					CMemoryAllocationManager* memMan = &slotMemMan;
					CContextBase tmpContext(memMan);

					CReuseCompletionGraphCompatibilityEntryHash* entyHash = CObjectParameterizingAllocator< CReuseCompletionGraphCompatibilityEntryHash,CContext* >::allocateAndConstructAndParameterize(memMan,&tmpContext);
					*entyHash = *mEntyHash;
					entyHash->detach();
					slot->setEntryHash(entyHash);
					slot->setEntryCount(mEntryCount);

					if (mSlotLinker) {
						mSlotLinker->append(slot);
					} else {
						mSlotLinker = slot;
					}
					CReuseCompletionGraphCacheReader* readerLinkerIt = mReaderLinker;
					while (readerLinkerIt) {
						slot->incReader();
						readerLinkerIt->updateSlot(slot);
						readerLinkerIt = readerLinkerIt->getNext();
					}
				}


				void CReuseCompletionGraphCache::cleanUnusedSlots(CReuseCompletionGraphCacheContext* context) {
					CMemoryPoolAllocationManager* memMan = context->getMemoryPoolAllocationManager();
					CReuseCompletionGraphCacheSlotItem* slotLinkerIt = mSlotLinker;
					CReuseCompletionGraphCacheSlotItem* lastSlotLinker = nullptr;
					while (slotLinkerIt) {
						CReuseCompletionGraphCacheSlotItem* tmpSlotLinker = nullptr;
						if (!slotLinkerIt->hasCacheReaders()) {
							tmpSlotLinker = slotLinkerIt;
						} 
						CReuseCompletionGraphCacheSlotItem* nextSlotLinker = slotLinkerIt->getNext();
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



				CReuseCompletionGraphCacheReader* CReuseCompletionGraphCache::createCacheReader() {
					CReuseCompletionGraphCacheReader* reader = new CReuseCompletionGraphCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = reader->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return reader;
				}


				CReuseCompletionGraphCacheWriter* CReuseCompletionGraphCache::createCacheWriter() {
					CReuseCompletionGraphCacheWriter* writer = new CReuseCompletionGraphCacheWriter(this);
					return writer;
				}


				CReuseCompletionGraphCache* CReuseCompletionGraphCache::writeExpandCache(CReuseCompletionGraphCacheEntryWriteData* writeData, CMemoryPool* memoryPools) {
					postEvent(new CWriteCachedDataEvent(writeData,memoryPools));
					return this;
				}


				void CReuseCompletionGraphCache::writeExpandCacheData(CReuseCompletionGraphCacheContext* context, CCACHINGLIST<CCacheValue>* cacheList, CCACHINGLIST<CCacheValue>* minimalCacheList, CSatisfiableCalculationJobInstantiation* jobInst) {

					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					CReuseCompletionGraphCacheEntry* newEntry = CObjectParameterizingAllocator< CReuseCompletionGraphCacheEntry,CReuseCompletionGraphCacheContext* >::allocateAndConstructAndParameterize(memMan,context);
					newEntry->setJobInstantiation(jobInst);
					mEntyList->append(newEntry);
					newEntry->setEntryID(mEntryCount);

					for (CCACHINGLIST<CCacheValue>::const_iterator it = cacheList->constBegin(), itEnd = cacheList->constEnd(); it != itEnd; ++it) {
						const CCacheValue& cacheValue = *it;
						CReuseCompletionGraphCompatibilityEntryHashData& hashData = (*mEntyHash)[cacheValue];
						CXLinker<CReuseCompletionGraphCacheEntry*>* linker = CObjectAllocator< CXLinker<CReuseCompletionGraphCacheEntry*> >::allocateAndConstruct(memMan);
						linker->initLinker(newEntry);
						hashData.addEntailedEntyLinker(linker);
						newEntry->addEntailedValue(cacheValue);
					}

					for (CCACHINGLIST<CCacheValue>::const_iterator it = minimalCacheList->constBegin(), itEnd = minimalCacheList->constEnd(); it != itEnd; ++it) {
						const CCacheValue& cacheValue = *it;
						newEntry->addMinimalValue(cacheValue);
					}

					++mEntryCount;
				}



				bool CReuseCompletionGraphCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;
					} if (type == CWriteCachedDataEvent::EVENTTYPE) {
						CWriteCachedDataEvent* wcde = (CWriteCachedDataEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();

						CReuseCompletionGraphCacheEntryWriteData* dataLinkerIt = (CReuseCompletionGraphCacheEntryWriteData*)wcde->getWriteData();
						while (dataLinkerIt) {
							if (dataLinkerIt->getCacheWriteDataType() == CReuseCompletionGraphCacheEntryWriteData::REUSECOMPLETIONGRAPHWRITEEXPANDDATATYPE) {
								CReuseCompletionGraphCacheEntryExpandWriteData* writeExpandData = (CReuseCompletionGraphCacheEntryExpandWriteData*)dataLinkerIt;

								CCACHINGLIST<CCacheValue>* cacheList = writeExpandData->getCacheValueList();
								CCACHINGLIST<CCacheValue>* minimalCacheList = writeExpandData->getCacheValueList();
								CSatisfiableCalculationJobInstantiation* jobInst = writeExpandData->getJobInstantiation();

								writeExpandCacheData(&mContext,cacheList,minimalCacheList,jobInst);
							}
							dataLinkerIt = (CReuseCompletionGraphCacheEntryWriteData*)dataLinkerIt->getNext();
						}

						createReaderSlotUpdate(&mContext);
						cleanUnusedSlots(&mContext);

						mContext.getMemoryPoolAllocationManager()->releaseTemporaryMemoryPools(memoryPools);
					}
					return false;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
