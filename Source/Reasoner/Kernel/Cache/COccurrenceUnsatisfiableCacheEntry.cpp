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

#include "COccurrenceUnsatisfiableCacheEntry.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceUnsatisfiableCacheEntry::COccurrenceUnsatisfiableCacheEntry(const CCacheValue &cacheValue, COccurrenceUnsatisfiableCacheEntry *prevUnsatCacheEntry, qint64 writeUpdateSlotCount, qint64 currentActiveSlot, qint64 readCountVecSize) {
					prevEntry = prevUnsatCacheEntry;
					cacheVal = cacheValue;
					updateSlotCount = writeUpdateSlotCount;
					activeSlot = currentActiveSlot;
					lastCacheEntriesHash = 0;
					unsatTerm = false;
					maxItemEntry = Q_INT64_C(0x8000000000000000);
					minItemEntry = Q_INT64_C(0x7FFFFFFFFFFFFFFF);

					cacheEntriesHashes = new COccurrenceUnsatisfiableCacheEntriesHash *[updateSlotCount];
					for (qint64 idx = 0; idx < updateSlotCount; ++idx) {
						cacheEntriesHashes[idx] = 0;
					}

					readCountCount = readCountVecSize;
					if (readCountCount > 0) {
						readCountVec = new qint64[readCountCount];
					} else {
						readCountVec = 0;
					}

					serialized = false;
					cacheTermValuesList = 0;
				}


				COccurrenceUnsatisfiableCacheEntry::~COccurrenceUnsatisfiableCacheEntry() {
					delete cacheEntriesHashes;
					if (readCountVec) {
						delete readCountVec;
					}
					if (cacheTermValuesList) {
						delete cacheTermValuesList;
					}
				}


				bool COccurrenceUnsatisfiableCacheEntry::isSerialized() {
					return serialized;
				}


				COccurrenceUnsatisfiableCacheEntry *COccurrenceUnsatisfiableCacheEntry::getPreviousUnsatisfiableCacheEntry() {
					return prevEntry;
				}


				COccurrenceUnsatisfiableCacheEntriesHash *COccurrenceUnsatisfiableCacheEntry::setCacheEntriesHashSlotGetPrevious(qint64 slotIndex, COccurrenceUnsatisfiableCacheEntriesHash *entriesHash) {
					COccurrenceUnsatisfiableCacheEntriesHash *tmpHash = cacheEntriesHashes[slotIndex];
					cacheEntriesHashes[slotIndex] = entriesHash;
					lastCacheEntriesHash = entriesHash;
					return tmpHash;
				}


				COccurrenceUnsatisfiableCacheEntriesHash *COccurrenceUnsatisfiableCacheEntry::updateSlotCacheHashGetPrevious(qint64 slotIndex) {
					COccurrenceUnsatisfiableCacheEntriesHash *prevHash = 0;
					if (cacheEntriesHashes[slotIndex] != lastCacheEntriesHash) {
						prevHash = cacheEntriesHashes[slotIndex];
						cacheEntriesHashes[slotIndex] = lastCacheEntriesHash;
						lastCacheEntriesHash->incReferenceCount();
					}
					return prevHash;
				}


				void COccurrenceUnsatisfiableCacheEntry::removeSlotCacheEntriesHash(qint64 slotIndex) {
					cacheEntriesHashes[slotIndex] = 0;
				}


				void COccurrenceUnsatisfiableCacheEntry::setActiveSlot(qint64 slotIndex) {
					activeSlot = slotIndex;
				}

				void COccurrenceUnsatisfiableCacheEntry::setMinimumCandidate(qint64 value) {
					minItemEntry = qMin(minItemEntry,value);
				}

				void COccurrenceUnsatisfiableCacheEntry::setMaximumCandidate(qint64 value) {
					maxItemEntry = qMax(maxItemEntry,value);
				}


				qint64 COccurrenceUnsatisfiableCacheEntry::getMaxTag() {
					return maxItemEntry;
				}

				qint64 COccurrenceUnsatisfiableCacheEntry::getMinTag() {
					return minItemEntry;
				}

					

				COccurrenceUnsatisfiableCacheEntriesHash *COccurrenceUnsatisfiableCacheEntry::getCacheEntriesHash() {
					return lastCacheEntriesHash;
				}


				COccurrenceUnsatisfiableCacheEntriesHash *COccurrenceUnsatisfiableCacheEntry::getSlotCacheEntriesHash(qint64 slotIndex) {
					return cacheEntriesHashes[slotIndex];
				}


				bool COccurrenceUnsatisfiableCacheEntry::isUnsatisfiableTermination() {
					return unsatTerm;
				}


				CCacheValue COccurrenceUnsatisfiableCacheEntry::getCacheValue() {
					return cacheVal;
				}


				qint64 COccurrenceUnsatisfiableCacheEntry::getTotalReadCount() {
					qint64 readCount = 0;
					for (qint64 i = 0; i < readCountCount; ++i) {
						readCount += readCountVec[i];
					}
					return readCount;
				}

				void COccurrenceUnsatisfiableCacheEntry::incReadCount(qint64 readCountIndex) {
					if (readCountIndex >= 0 && readCountIndex < readCountCount) {
						readCountVec[readCountIndex] = readCountVec[readCountIndex]+1;
					}
				}

				COccurrenceUnsatisfiableCacheEntry *COccurrenceUnsatisfiableCacheEntry::setCacheTerminationValuesList(QList<CCacheValue> *cacheTerminationValuesList) {
					if (cacheTermValuesList) {
						delete cacheTermValuesList;
					}
					cacheTermValuesList = cacheTerminationValuesList;
					unsatTerm = cacheTermValuesList != 0;
					return this;
				}

				COccurrenceUnsatisfiableCacheEntry *COccurrenceUnsatisfiableCacheEntry::copyCacheTerminationValuesList(QList<CCacheValue> *cacheTerminationValuesList) {
					if (cacheTermValuesList) {
						delete cacheTermValuesList;
					}
					if (cacheTerminationValuesList) {
						cacheTermValuesList = new QList<CCacheValue>(*cacheTerminationValuesList);
					} else {
						cacheTermValuesList = 0;
					}
					unsatTerm = cacheTermValuesList != 0;
					return this;
				}

				QList<CCacheValue> *COccurrenceUnsatisfiableCacheEntry::getCacheTerminationValuesList() {
					return cacheTermValuesList;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
