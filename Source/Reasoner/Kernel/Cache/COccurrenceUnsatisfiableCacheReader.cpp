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

#include "COccurrenceUnsatisfiableCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceUnsatisfiableCacheReader::COccurrenceUnsatisfiableCacheReader(COccurrenceUnsatisfiableCache *unsatisfiableCache) {
					cache = unsatisfiableCache;
					cacheSlotItem = 0;
					incCacheEntriesLinker = 0;
					incrementUnsatisfiableTestReset();
				}


				COccurrenceUnsatisfiableCacheReader::~COccurrenceUnsatisfiableCacheReader() {
				}



				COccurrenceUnsatisfiableCacheUpdateSlotItem *COccurrenceUnsatisfiableCacheReader::changeUpdateSlot(COccurrenceUnsatisfiableCacheUpdateSlotItem *nextUpdateSlot) {
					COccurrenceUnsatisfiableCacheUpdateSlotItem *tmpSlotItem = 0;
					nextUpdateSlot->incReader();
					tmpSlotItem = mNextCacheSlotItemPointer.fetchAndStoreOrdered(nextUpdateSlot);
					if (tmpSlotItem) {
						tmpSlotItem->decReader();
					}
					return tmpSlotItem;
				}



				bool COccurrenceUnsatisfiableCacheReader::moveToNextSlot() {
                    if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {

						COccurrenceUnsatisfiableCacheUpdateSlotItem* nextSlotItem = mNextCacheSlotItemPointer.fetchAndStoreOrdered(nullptr);
						if (nextSlotItem) {
							if (cacheSlotItem) {
								cacheSlotItem->decReader();
							}
							cacheSlotItem = nextSlotItem;
							return true;
						}						
					}
					return false;
				}



				void COccurrenceUnsatisfiableCacheReader::incrementUnsatisfiableTestReset() {
					while (incCacheEntriesLinker) {
						CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = incCacheEntriesLinker;
						incCacheEntriesLinker = incCacheEntriesLinker->getNext();
						memManCacheEntryList.release(tmpCacheEntry);
					}
					lastUnsatItems.clear();
				}


				bool COccurrenceUnsatisfiableCacheReader::incrementUnsatisfiableTest(CCacheValue *cacheValue, bool *continueTestingUseful) {
					bool unsat = false;
					incCacheEntriesLinker = incrementalUnsatisfiableTest(cacheValue,incCacheEntriesLinker,&unsat,continueTestingUseful);
					return unsat;
				}



				CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *COccurrenceUnsatisfiableCacheReader::incrementalUnsatisfiableTest(CCacheValue *cacheValue, CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *cacheEntries, bool *unsatisfiable, bool *continueTesting) {
					if (!cacheEntries) {
						cacheEntries = memManCacheEntryList.allocate()->init(cache->getPrimarCacheEntry());
					}
					qint64 updateSlot = 0;
                    if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
						moveToNextSlot();
					}
					bool unsatisfiableFound = false;
					bool continueTestingUseful = false;
					if (cacheSlotItem) {
						updateSlot = cacheSlotItem->getSlotIndex();

						CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *nextCacheEntries = 0;
						qint64 itemTag = cacheValue->getTag();

						CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *linkIt = cacheEntries;
						while (linkIt) {

                            if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
								moveToNextSlot();
								if (cacheSlotItem) {
									updateSlot = cacheSlotItem->getSlotIndex();
								}
							}

							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *linkNextIt = linkIt->getNext();
							if (linkNextIt) {
								linkNextIt->removePrev();
							}
							COccurrenceUnsatisfiableCacheEntry *cacheEntry = linkIt->getData();

							qint64 cacheMax = cacheEntry->getMaxTag();
							qint64 cacheMin = cacheEntry->getMinTag();

							COccurrenceUnsatisfiableCacheEntriesHash *cacheHash = cacheEntry->getSlotCacheEntriesHash(updateSlot);

							// test min and max
							if (!cacheHash || itemTag > cacheMax) {
								// cache entry is no longer needed
								memManCacheEntryList.release(linkIt);
							} else {
								if (!cacheEntry->isSerialized()) {
									// cache entry is needed, next round
									nextCacheEntries = linkIt->insertNext(nextCacheEntries);
								} else {
									memManCacheEntryList.release(linkIt);
								}

								// test to add additional cache entries
								if (cacheHash->contains(*cacheValue)) {
									COccurrenceUnsatisfiableCacheEntry *nextAddingCacheEntry = cacheHash->value(*cacheValue);
									if (nextAddingCacheEntry->isUnsatisfiableTermination()) {
										unsatisfiableFound = true;
										linkIt = linkNextIt;
										if (nextAddingCacheEntry->getCacheTerminationValuesList()) {
											lastUnsatItems = *nextAddingCacheEntry->getCacheTerminationValuesList();
										}
										break;
									}
									nextCacheEntries = memManCacheEntryList.allocate()->init(nextAddingCacheEntry,nextCacheEntries);
								}
							}
							linkIt = linkNextIt;
						}

						while (linkIt) {
							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = linkIt;
							linkIt = linkIt->getNext();
							memManCacheEntryList.release(tmpCacheEntry);
						}

						cacheEntries = nextCacheEntries;
						nextCacheEntries = 0;
					} else {
						while (cacheEntries) {
							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = cacheEntries;
							cacheEntries = cacheEntries->getNext();
							memManCacheEntryList.release(tmpCacheEntry);
						}
						cacheEntries = 0;
					}
					if (unsatisfiableFound) {
						while (cacheEntries) {
							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = cacheEntries;
							cacheEntries = cacheEntries->getNext();
							memManCacheEntryList.release(tmpCacheEntry);
						}
						cacheEntries = 0;
					} 
					if (!unsatisfiableFound && cacheEntries) {
						continueTestingUseful = true;
					}

					if (continueTesting) {
						*continueTesting = continueTestingUseful;
					}
					if (unsatisfiable) {
						*unsatisfiable = unsatisfiableFound;
					}
					return cacheEntries;
				}



				bool COccurrenceUnsatisfiableCacheReader::isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count) {
					return isUnsatisfiable(itemVec,count,0);
				}

				bool COccurrenceUnsatisfiableCacheReader::isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count, QList<CCacheValue> *unsatisfiableItemList) {

					bool unsatisfiable = false;

					// list for cache entries
					// TODO: use min and max from cache entries to make list smaller
					CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *cacheEntries = memManCacheEntryList.allocate()->init(cache->getPrimarCacheEntry());


					qint64 firstIndex = 0;
					qint64 lastIndex = count-1;

					// make assumption that list is sorted
					qint64 minItem = itemVec[0].getTag();
					qint64 maxItem = itemVec[lastIndex].getTag();


					qint64 updateSlot = 0;
                    if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
						moveToNextSlot();
					}
					if (cacheSlotItem) {
						updateSlot = cacheSlotItem->getSlotIndex();

						CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *nextCacheEntries = 0;


						// TODO: Skip items which are not in cache
						qint64 index = 0;
						while (index < count && !unsatisfiable && cacheEntries) {
							CCacheValue &item = itemVec[index++];
							qint64 itemTag = item.getTag();
							
							nextCacheEntries = 0;

							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *linkIt = cacheEntries;
							while (linkIt) {

                                if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
									moveToNextSlot();
									if (cacheSlotItem) {
										updateSlot = cacheSlotItem->getSlotIndex();
									}
								}

								CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *linkNextIt = linkIt->getNext();
								if (linkNextIt) {
									linkNextIt->removePrev();
								}
								COccurrenceUnsatisfiableCacheEntry *cacheEntry = linkIt->getData();

								qint64 cacheMax = cacheEntry->getMaxTag();
								qint64 cacheMin = cacheEntry->getMinTag();

								COccurrenceUnsatisfiableCacheEntriesHash *cacheHash = cacheEntry->getSlotCacheEntriesHash(updateSlot);

								// test min and max
								if (!cacheHash || cacheMin > maxItem || itemTag > cacheMax) {
									// cache entry is no longer needed

									memManCacheEntryList.release(linkIt);

								} else {
									// cache entry is needed, next round
									nextCacheEntries = linkIt->insertNext(nextCacheEntries);



									// test to add additional cache entries
									if (cacheHash->contains(item)) {
										
										COccurrenceUnsatisfiableCacheEntry *nextAddingCacheEntry = cacheHash->value(item);

										if (nextAddingCacheEntry->isUnsatisfiableTermination()) {
											unsatisfiable = true;
											if (nextAddingCacheEntry->getCacheTerminationValuesList()) {
												lastUnsatItems = *nextAddingCacheEntry->getCacheTerminationValuesList();
												if (unsatisfiableItemList) {
													*unsatisfiableItemList = *nextAddingCacheEntry->getCacheTerminationValuesList();
												}						
											}
											linkIt = linkNextIt;
											break;
										}

										nextCacheEntries = memManCacheEntryList.allocate()->init(nextAddingCacheEntry,nextCacheEntries);
									}

								}							

								linkIt = linkNextIt;
							}


							while (linkIt) {
								CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = linkIt;
								linkIt = linkIt->getNext();
								memManCacheEntryList.release(tmpCacheEntry);
							}

							cacheEntries = nextCacheEntries;
							nextCacheEntries = 0;
						}
					

						while (cacheEntries) {
							CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *tmpCacheEntry = cacheEntries;
							cacheEntries = cacheEntries->getNext();
							memManCacheEntryList.release(tmpCacheEntry);
						}
					}

                    if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
						moveToNextSlot();
						if (cacheSlotItem) {
							updateSlot = cacheSlotItem->getSlotIndex();
						}
					}
					return unsatisfiable;
				}


				QList<CCacheValue> COccurrenceUnsatisfiableCacheReader::getLastTestedUnsatisfiableItems() {
					return lastUnsatItems;
				}



				QList<CCacheValue> COccurrenceUnsatisfiableCacheReader::getUnsatisfiableItems(QVector<CCacheValue> &itemVec, qint64 count) {
					QList<CCacheValue> unsatList;
					isUnsatisfiable(itemVec,count,&unsatList);
					return unsatList;
				}


				CXLinker<CCacheValue*>* COccurrenceUnsatisfiableCacheReader::getUnsatisfiableItems(CXLinker<CCacheValue*>* cacheValueTestLinker, CMemoryAllocationManager* memMan) {
					CXLinker<CCacheValue*>* unsatLinker = nullptr;



					bool unsatisfiable = false;

					// list for cache entries
					// TODO: use min and max from cache entries to make list smaller
					CXLinker<COccurrenceUnsatisfiableCacheEntry*>* cacheEntries = CObjectAllocator< CXLinker<COccurrenceUnsatisfiableCacheEntry*> >::allocateAndConstruct(memMan)->initLinker(cache->getPrimarCacheEntry());
					//memManCacheEntryList.allocate()->init(cache->getPrimarCacheEntry());


					CCacheValue* firstItem = cacheValueTestLinker->getData();
					CCacheValue* lastItem = cacheValueTestLinker->getLastListLink()->getData();


					CXLinker<CCacheValue*>* testCacheItemIt = cacheValueTestLinker;

					// make assumption that list is sorted
					qint64 minItem = firstItem->getTag();
					qint64 maxItem = lastItem->getTag();


					qint64 updateSlot = 0;
                    if (mNextCacheSlotItemPointer.fetchAndAddRelaxed(0) != nullptr) {
						moveToNextSlot();
					}
					if (cacheSlotItem) {
						updateSlot = cacheSlotItem->getSlotIndex();

						CXLinker<COccurrenceUnsatisfiableCacheEntry *> *nextCacheEntries = nullptr;


						while (testCacheItemIt && cacheEntries) {
							CCacheValue* item = testCacheItemIt->getData();
							qint64 itemTag = item->getTag();

							nextCacheEntries = nullptr;

							CXLinker<COccurrenceUnsatisfiableCacheEntry *> *cacheEntryIt = cacheEntries;
							while (cacheEntryIt) {


								CXLinker<COccurrenceUnsatisfiableCacheEntry *> *cacheEntryLinker = cacheEntryIt;
								cacheEntryIt = cacheEntryIt->getNext();
								cacheEntryLinker->clearNext();


								COccurrenceUnsatisfiableCacheEntry *cacheEntry = cacheEntryLinker->getData();

								qint64 cacheMax = cacheEntry->getMaxTag();
								qint64 cacheMin = cacheEntry->getMinTag();

								COccurrenceUnsatisfiableCacheEntriesHash *cacheHash = cacheEntry->getSlotCacheEntriesHash(updateSlot);

								// test min and max
								if (!cacheHash || cacheMin > maxItem || itemTag > cacheMax) {
									// cache entry is no longer needed

								} else {
									// cache entry is needed, next round
									nextCacheEntries = cacheEntryLinker->append(nextCacheEntries);


									COccurrenceUnsatisfiableCacheEntry *nextAddingCacheEntry = cacheHash->value(*item,nullptr);

									// test to add additional cache entries
									if (nextAddingCacheEntry) {

										if (nextAddingCacheEntry->isUnsatisfiableTermination()) {
											unsatisfiable = true;
											QList<CCacheValue>* unsatValList = nextAddingCacheEntry->getCacheTerminationValuesList();
											if (unsatValList) {

												CXLinker<CCacheValue*>* unsatCacheItemLinker = cacheValueTestLinker;
												CXLinker<CCacheValue*>* lastUnsatCacheItemLinker = nullptr;
												CXLinker<CCacheValue*>* firstUnsatCacheItemLinker = nullptr;
												for (QList<CCacheValue>::const_iterator unsatItemIt = unsatValList->constBegin(), unsatItemItEnd = unsatValList->constEnd(); unsatItemIt != unsatItemItEnd; ++unsatItemIt) {
													while (*unsatCacheItemLinker->getData() != *unsatItemIt) {
														unsatCacheItemLinker = unsatCacheItemLinker->getNext();
													}
													if (!firstUnsatCacheItemLinker) {
														firstUnsatCacheItemLinker = unsatCacheItemLinker;
													} else {
														lastUnsatCacheItemLinker->setNext(unsatCacheItemLinker);
													}
													lastUnsatCacheItemLinker = unsatCacheItemLinker;
												}
												lastUnsatCacheItemLinker->clearNext();
												return firstUnsatCacheItemLinker;
											} else {
												return cacheValueTestLinker;
											}
										}

										nextCacheEntries = CObjectAllocator< CXLinker<COccurrenceUnsatisfiableCacheEntry*> >::allocateAndConstruct(memMan)->initLinker(nextAddingCacheEntry,nextCacheEntries);
									}

								}							

							}

							cacheEntries = nextCacheEntries;
							nextCacheEntries = nullptr;

							testCacheItemIt = testCacheItemIt->getNext();
						}
					}

					return nullptr;
				}

				cint64 COccurrenceUnsatisfiableCacheReader::getCurrentCachingTag() {
					return cache->getCurrentCachingTag();
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
