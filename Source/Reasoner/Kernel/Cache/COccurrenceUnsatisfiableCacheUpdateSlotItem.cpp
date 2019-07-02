/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COccurrenceUnsatisfiableCacheUpdateSlotItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				COccurrenceUnsatisfiableCacheUpdateSlotItem::COccurrenceUnsatisfiableCacheUpdateSlotItem(qint64 updateSlotIndex) {
					slotIndex = updateSlotIndex;
					mReaderUsing = false;
				}


				COccurrenceUnsatisfiableCacheUpdateSlotItem::~COccurrenceUnsatisfiableCacheUpdateSlotItem() {
					cleanSlotUpdateItems();
				}


				bool COccurrenceUnsatisfiableCacheUpdateSlotItem::incReader() {
					if (mReaderSharingCount.ref()) {
						mReaderUsing = true;
					}
					return true;
				}


				bool COccurrenceUnsatisfiableCacheUpdateSlotItem::incReader(cint64 incCount) {
					for (cint64 i = 0; i < incCount; ++i) {
						incReader();
					}
					return mReaderUsing;
				}


				bool COccurrenceUnsatisfiableCacheUpdateSlotItem::decReader() {
					if (!mReaderSharingCount.deref()) {
						mReaderUsing = false;
					}
					return mReaderUsing;
				}


				void COccurrenceUnsatisfiableCacheUpdateSlotItem::addCacheEntry(COccurrenceUnsatisfiableCacheEntry *cacheEntry) {
					updatedEntrySet.insert(cacheEntry);
				}

				void COccurrenceUnsatisfiableCacheUpdateSlotItem::addCacheEntriesHash(COccurrenceUnsatisfiableCacheEntriesHash *prevDelHash) {
					updatedHashesList.append(prevDelHash);
				}


				void COccurrenceUnsatisfiableCacheUpdateSlotItem::activateSlotUpdateItems() {
					foreach (COccurrenceUnsatisfiableCacheEntry *entry, updatedEntrySet) {
						entry->setActiveSlot(slotIndex);
					}
				}


				void COccurrenceUnsatisfiableCacheUpdateSlotItem::cleanSlotUpdateItems() {
					foreach (COccurrenceUnsatisfiableCacheEntriesHash *hash, updatedHashesList) {
						if (hash->decReferenceCountReturnHasToBeDeleted()) {
							delete hash;
						}
					}
					updatedHashesList.clear();
					updatedEntrySet.clear();
				}


				qint64 COccurrenceUnsatisfiableCacheUpdateSlotItem::getSlotIndex() {
					return slotIndex;
				}

				bool COccurrenceUnsatisfiableCacheUpdateSlotItem::hasCacheReaders() {
					return mReaderUsing;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
