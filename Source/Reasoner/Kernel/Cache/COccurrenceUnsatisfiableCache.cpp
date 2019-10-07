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
					CThread::postEvent(new CWriteUnsatisfiableCacheEntryEvent(itemList));
				}




				void COccurrenceUnsatisfiableCache::threadStarted() {
					primarCacheEntry = new COccurrenceUnsatisfiableCacheEntry(CCacheValue(0,0,CCacheValue::CACHEVALCONCEPTONTOLOGYTAG),0,updateSlotCount,0);
					container.append(primarCacheEntry);

					for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
						updatesSlotItemVector[idx] = new COccurrenceUnsatisfiableCacheUpdateSlotItem(idx);
						notusedUpdatesSlotsList.append(updatesSlotItemVector[idx]);
					}
				}

				void COccurrenceUnsatisfiableCache::threadStopped() {
					for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
						delete updatesSlotItemVector[idx];
					}
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

					foreach (COccurrenceUnsatisfiableCacheEntry *entry, container) {
						COccurrenceUnsatisfiableCacheEntriesHash *prevDelHash = entry->updateSlotCacheHashGetPrevious(slotIndex);
						if (prevDelHash) {
							updateSlot->addCacheEntriesHash(prevDelHash);
						}
					}

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
							if (!updatedCacheHash) {
								if (cacheHash) {
									updatedCacheHash = new COccurrenceUnsatisfiableCacheEntriesHash(*cacheHash);
								} else {
									updatedCacheHash = new COccurrenceUnsatisfiableCacheEntriesHash();
								}
								updateSlot->addCacheEntry(cache);
							}

							COccurrenceUnsatisfiableCacheEntry *nextCache = new COccurrenceUnsatisfiableCacheEntry(cacheValue,0,updateSlotCount,slotIndex);
							container.append(nextCache);

							qint64 tag = cacheValue.getTag();
							updatedCacheHash->insert(cacheValue,nextCache);
							cache->setMinimumCandidate(tag);
							cache->setMaximumCandidate(tag);

							COccurrenceUnsatisfiableCacheEntriesHash *prevDelHash = cache->setCacheEntriesHashSlotGetPrevious(slotIndex,updatedCacheHash);
							if (prevDelHash) {
								updateSlot->addCacheEntriesHash(prevDelHash);
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


						if (!lastUpdateSlot || !testAlreadyCached(lastUpdateSlot,cEL)) {
							cacheWritingRequested = true;

							if (waitCacheWritePrepared()) {
								COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot = notusedUpdatesSlotsList.takeFirst();

								writeOperationsCount++;

								writeCacheValues(updateSlot,cEL);
								mCachStat.incCacheEntriesCount();

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

							cacheWritingRequested = false;
						}


						return true;
					}
					return false;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
