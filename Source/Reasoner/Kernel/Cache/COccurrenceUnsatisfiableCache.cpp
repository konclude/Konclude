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

#include "COccurrenceUnsatisfiableCache.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceUnsatisfiableCache::COccurrenceUnsatisfiableCache(qint64 writeUpdateSlotCount, const QString &threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName,watchDogThread),updatesSlotItemVector(writeUpdateSlotCount,0) {
					cacheWritingRequested = false;
					updateSlotCount = writeUpdateSlotCount;

					writeOperationsCount = 0;
					mCachingTag = 0;
					lastUpdateSlot = nullptr;
					mBatchUpdateSlot = nullptr;
					mBatchWriteCount = 0;

					startThread();
				}


				COccurrenceUnsatisfiableCache::~COccurrenceUnsatisfiableCache() {
					qDeleteAll(cacheReaderList);
					qDeleteAll(cacheWriterList);
				}



				COccurrenceUnsatisfiableCacheReader *COccurrenceUnsatisfiableCache::getCacheReader() {
					cacheReaderSyncMutex.lock();
					COccurrenceUnsatisfiableCacheReader *reader = new COccurrenceUnsatisfiableCacheReader(this);
					cacheReaderList.append(reader);
					cacheReaderSyncMutex.unlock();
					return reader;
				}


				COccurrenceUnsatisfiableCacheWriter *COccurrenceUnsatisfiableCache::getCacheWriter() {
					cacheWriterSyncMutex.lock();
					COccurrenceUnsatisfiableCacheWriter *writer = new COccurrenceUnsatisfiableCacheWriter(this);
					cacheWriterList.append(writer);
					cacheWriterSyncMutex.unlock();
					return writer;
				}


				bool COccurrenceUnsatisfiableCache::isCacheWritePending() {
					return cacheWritingRequested;
				}


				cint64 COccurrenceUnsatisfiableCache::getCurrentCachingTag() {
					return mCachingTag;
				}

				CCacheStatistics* COccurrenceUnsatisfiableCache::getCacheStatistics() {
					return &mCachStat;
				}


				COccurrenceUnsatisfiableCacheEntry *COccurrenceUnsatisfiableCache::getPrimarCacheEntry() {
					if (!CThread::isThreadRunning()) {
						CThread::waitSynchronization();
					}
					return primarCacheEntry;
				}


				void COccurrenceUnsatisfiableCache::addUnsatisfiableCacheEntry(QList<CCacheValue> &itemList) {
					// item list is and has to be copied in event constructor!
					mQueuedWriteCount.fetchAndAddOrdered(1);
					CThread::postEvent(new CWriteUnsatisfiableCacheEntryEvent(itemList));
				}




				void COccurrenceUnsatisfiableCache::threadStarted() {
					primarCacheEntry = new COccurrenceUnsatisfiableCacheEntry(CCacheValue(0,0,CCacheValue::CACHEVALCONCEPTONTOLOGYTAG),0,updateSlotCount,0);
					container.append(primarCacheEntry);
					mSlotOutdatedEntries.resize(updateSlotCount);

					for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
						updatesSlotItemVector[idx] = new COccurrenceUnsatisfiableCacheUpdateSlotItem(idx);
						notusedUpdatesSlotsList.append(updatesSlotItemVector[idx]);
					}
				}

				void COccurrenceUnsatisfiableCache::threadStopped() {
					for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
						delete updatesSlotItemVector[idx];
					}
					// the update slots release the entries hashes they replaced; the ones the entries still
					// refer to in their slots hold a reference each that nothing releases, so they are
					// deleted here, each once, before the entries (issue #45)
					QSet<COccurrenceUnsatisfiableCacheEntriesHash*> entriesHashSet;
					foreach (COccurrenceUnsatisfiableCacheEntry* entry, container) {
						for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
							COccurrenceUnsatisfiableCacheEntriesHash* entriesHash = entry->getSlotCacheEntriesHash(idx);
							if (entriesHash) {
								entriesHashSet.insert(entriesHash);
							}
						}
					}
					qDeleteAll(entriesHashSet);
					primarCacheEntry = 0;
					qDeleteAll(container);
					container.clear();
				}


				bool COccurrenceUnsatisfiableCache::waitCacheWritePrepared() {
					for (qint64 i = usedUpdatesSlotsList.count(); i > 0; --i) {
						COccurrenceUnsatisfiableCacheUpdateSlotItem *slotItem = usedUpdatesSlotsList.takeFirst();
						if (!slotItem->hasCacheReaders()) {
							slotItem->cleanSlotUpdateItems();
							notusedUpdatesSlotsList.append(slotItem);
						} else {
							usedUpdatesSlotsList.append(slotItem);
						}
					}
					while (notusedUpdatesSlotsList.isEmpty()) {
						// wait until one update slot is not longer used
						QThread::msleep(10);
						for (qint64 i = usedUpdatesSlotsList.count(); i > 0; --i) {
							COccurrenceUnsatisfiableCacheUpdateSlotItem *slotItem = usedUpdatesSlotsList.takeFirst();
							if (!slotItem->hasCacheReaders()) {
								slotItem->cleanSlotUpdateItems();
								notusedUpdatesSlotsList.append(slotItem);
							} else {
								usedUpdatesSlotsList.append(slotItem);
							}
						}
					}
					return true;
				}



				bool COccurrenceUnsatisfiableCache::activateCacheUpdate(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot) {

					qint64 slotIndex = updateSlot->getSlotIndex();

					updateSlot->activateSlotUpdateItems();
					usedUpdatesSlotsList.append(updateSlot);

					// only the entries changed since this slot was last activated have an outdated hash in it
					QSet<COccurrenceUnsatisfiableCacheEntry*>& outdatedEntries = mSlotOutdatedEntries[slotIndex];
					foreach (COccurrenceUnsatisfiableCacheEntry *entry, outdatedEntries) {
						COccurrenceUnsatisfiableCacheEntriesHash *prevDelHash = entry->updateSlotCacheHashGetPrevious(slotIndex);
						if (prevDelHash) {
							updateSlot->addCacheEntriesHash(prevDelHash);
						}
					}
					outdatedEntries.clear();
					// the hashes of the batch are now visible to readers and have to be copied before further changes
					mBatchCreatedHashes.clear();

					lastUpdateSlot = updateSlot;

					cacheReaderSyncMutex.lock();
					foreach (COccurrenceUnsatisfiableCacheReader *reader, cacheReaderList) {
						reader->changeUpdateSlot(updateSlot);
					}
					cacheReaderSyncMutex.unlock();

					return true;
				}


				bool COccurrenceUnsatisfiableCache::writeCacheTags(CCacheValue* cacheValue, cint64 cachingTag, cint64 cachedTag, cint64 cachingSize) {
					CCacheValue::CACHEVALUEIDENTIFIER valId = cacheValue->getCacheValueIdentifier();
					bool hasConcept = false;
					bool conceptNeg = false;
					if (valId == CCacheValue::CACHEVALTAGANDCONCEPT) {
						hasConcept = true;
						conceptNeg = false;
					} else if (valId == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT) {
						hasConcept = true;
						conceptNeg = true;
					}
					if (hasConcept) {
						cint64 identification = cacheValue->getIdentification();
						CConcept* concept = (CConcept*)identification;
						if (concept) {
							CConceptProcessData* conProData = (CConceptProcessData*)concept->getConceptData();
							if (conProData) {
								CUnsatisfiableCachingTags* unsatCachingTags = conProData->getUnsatisfiableCachingTags(conceptNeg);
								if (!unsatCachingTags) {
									unsatCachingTags = new CUnsatisfiableCachingTags();
									conProData->setUnsatisfiableCachingTags(conceptNeg,unsatCachingTags);
								}
								unsatCachingTags->updateCachingTags(cachedTag,cachingTag,cachingSize);
								return true;
							}
						}
					}
					return false;
				}


				bool COccurrenceUnsatisfiableCache::writeCacheValues(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot, QList<CCacheValue> *cacheValueList) {
					qint64 slotIndex = updateSlot->getSlotIndex();
					COccurrenceUnsatisfiableCacheEntry *cache = primarCacheEntry;

					cint64 cachingTag = mCachingTag+1;
					cint64 cachedTag = mCacheTaggingPool.takeNextTag();

					cint64 cachingSize = cacheValueList->count();

					foreach (CCacheValue cacheValue, *cacheValueList) {

						COccurrenceUnsatisfiableCacheEntriesHash *cacheHash = cache->getCacheEntriesHash();
						if (!cacheHash || !cacheHash->contains(cacheValue)) {

							COccurrenceUnsatisfiableCacheEntriesHash *updatedCacheHash = 0;
							bool hashCreated = false;
							if (cacheHash && mBatchCreatedHashes.contains(cacheHash)) {
								// created by an earlier write of the open batch and only referenced by its update slot,
								// which no reader uses yet, so it is extended in place instead of being copied again
								updatedCacheHash = cacheHash;
							} else {
								if (cacheHash) {
									updatedCacheHash = new COccurrenceUnsatisfiableCacheEntriesHash(*cacheHash);
								} else {
									updatedCacheHash = new COccurrenceUnsatisfiableCacheEntriesHash();
								}
								updateSlot->addCacheEntry(cache);
								mBatchCreatedHashes.insert(updatedCacheHash);
								hashCreated = true;
							}

							COccurrenceUnsatisfiableCacheEntry *nextCache = new COccurrenceUnsatisfiableCacheEntry(cacheValue,0,updateSlotCount,slotIndex);
							container.append(nextCache);

							qint64 tag = cacheValue.getTag();
							updatedCacheHash->insert(cacheValue,nextCache);
							cache->setMinimumCandidate(tag);
							cache->setMaximumCandidate(tag);

							if (hashCreated) {
								COccurrenceUnsatisfiableCacheEntriesHash *prevDelHash = cache->setCacheEntriesHashSlotGetPrevious(slotIndex,updatedCacheHash);
								if (prevDelHash) {
									updateSlot->addCacheEntriesHash(prevDelHash);
								}
								// the other slots still have an older hash of this entry
								for (qint64 otherSlotIndex = 0; otherSlotIndex < updateSlotCount; ++otherSlotIndex) {
									if (otherSlotIndex != slotIndex) {
										mSlotOutdatedEntries[otherSlotIndex].insert(cache);
									}
								}
							}

							cache = nextCache;
						} else {
							cache = cacheHash->value(cacheValue);
						}

						writeCacheTags(&cacheValue,cachingTag,cachedTag,cachingSize);
					}
					if (cache) {
						cache->copyCacheTerminationValuesList(cacheValueList);
					}
					++mCachingTag;						
					
					return true;
				}



				bool COccurrenceUnsatisfiableCache::testAlreadyCached(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot, QList<CCacheValue> *cacheValueList) {
					qint64 slotIndex = updateSlot->getSlotIndex();
					COccurrenceUnsatisfiableCacheEntry *cache = primarCacheEntry;
					foreach (CCacheValue cacheValue, *cacheValueList) {

						if (cache->isUnsatisfiableTermination()) {
							return true;
						}

						COccurrenceUnsatisfiableCacheEntriesHash *cacheHash = cache->getCacheEntriesHash();

						if (!cacheHash) {
							return false;
						}

						cache = cacheHash->value(cacheValue);
						if (!cache) {
							return false;
						}
					}
					if (cache->isUnsatisfiableTermination()) {
						return true;
					}
					return false;
				}



				QString COccurrenceUnsatisfiableCache::getCachingConceptsDebugString(QList<CCacheValue> &itemList) {
					QString cachingConString;
					for (QList<CCacheValue>::const_iterator it = itemList.constBegin(), itEnd = itemList.constEnd(); it != itEnd; ++it) {
						CCacheValue cacheValue(*it);
						QString conceptString;
						if (cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDCONCEPT) {							
							conceptString = CConceptTextFormater::getConceptString((CConcept*)cacheValue.getIdentification(),false);
						} else if (cacheValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT) {							
							conceptString = CConceptTextFormater::getConceptString((CConcept*)cacheValue.getIdentification(),true);
						}
						if (!cachingConString.isEmpty()) {
							cachingConString += QString(", ");
						}
						cachingConString += conceptString;
					}
					return cachingConString;
				}


				bool COccurrenceUnsatisfiableCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type,event)) {
						return true;
					} if (type == EVENTWRITEUNSATISFIABLECACHEENTRY) {
						CWriteUnsatisfiableCacheEntryEvent *wuc = (CWriteUnsatisfiableCacheEntryEvent *)event;

						QList<CCacheValue> *cEL = wuc->getCacheEntryList();


						mQueuedWriteCount.fetchAndAddOrdered(-1);

						COccurrenceUnsatisfiableCacheUpdateSlotItem* testSlot = mBatchUpdateSlot ? mBatchUpdateSlot : lastUpdateSlot;
						if (!testSlot || !testAlreadyCached(testSlot,cEL)) {
							cacheWritingRequested = true;

							if (!mBatchUpdateSlot && waitCacheWritePrepared()) {
								mBatchUpdateSlot = notusedUpdatesSlotsList.takeFirst();
							}
							if (mBatchUpdateSlot) {
								writeOperationsCount++;

								writeCacheValues(mBatchUpdateSlot,cEL);
								mCachStat.incCacheEntriesCount();
								++mBatchWriteCount;
							}

							cacheWritingRequested = false;
						}

						// the writes are collected in one update slot while more are queued and activated together once
						// none is left (or after a maximum number), so that a backlog costs one slot switch and one copy
						// of each changed hash instead of one per write
						if (mBatchUpdateSlot && (mQueuedWriteCount.loadAcquire() <= 0 || mBatchWriteCount >= 1000)) {
							COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot = mBatchUpdateSlot;
							mBatchUpdateSlot = nullptr;
							mBatchWriteCount = 0;
							{
								activateCacheUpdate(updateSlot);

								for (qint64 i = usedUpdatesSlotsList.count(); i > 0; --i) {
									COccurrenceUnsatisfiableCacheUpdateSlotItem *slotItem = usedUpdatesSlotsList.takeFirst();
									if (!slotItem->hasCacheReaders()) {
										slotItem->cleanSlotUpdateItems();
										notusedUpdatesSlotsList.append(slotItem);
									} else {
										usedUpdatesSlotsList.append(slotItem);
									}
								}

								//KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(mCachingString = getCachingConceptsDebugString(*cEL));
								//KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(mCachedStringList.append(mCachingString));
								//KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(std::cout<<"\nUnsatisfiable cached concepts: "<<mCachingString.toLocal8Bit().data()<<"\n\n");
								//KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(LOG(WARNING,"::Konclude::Reasoner::Cache::OccurenceUnsatisfiableCache",logTr("Unsatisfiable cached concepts: %1").arg(mCachingString),this));

							}
						}


						return true;
					}
					return false;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
