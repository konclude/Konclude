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

#include "CReuseCompletionGraphCacheSlotItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CReuseCompletionGraphCacheSlotItem::CReuseCompletionGraphCacheSlotItem(CMemoryPool* memoryPool) : CLinkerBase<CReuseCompletionGraphCacheSlotItem*,CReuseCompletionGraphCacheSlotItem>(this),CMemoryPoolContainer(memoryPool) {
					mReaderUsing = false;
					mEntryHash = nullptr;
					mEntyCount = 0;
				}

				bool CReuseCompletionGraphCacheSlotItem::incReader() {
					if (mReaderSharingCount.ref()) {
						mReaderUsing = true;
					}
					return true;
				}


				bool CReuseCompletionGraphCacheSlotItem::incReader(cint64 incCount) {
					for (cint64 i = 0; i < incCount; ++i) {
						incReader();
					}
					return mReaderUsing;
				}


				bool CReuseCompletionGraphCacheSlotItem::decReader() {
					if (!mReaderSharingCount.deref()) {
						mReaderUsing = false;
					}
					return mReaderUsing;
				}


				bool CReuseCompletionGraphCacheSlotItem::hasCacheReaders() {
					return mReaderUsing;
				}



				CReuseCompletionGraphCacheSlotItem* CReuseCompletionGraphCacheSlotItem::setEntryHash(CReuseCompletionGraphCompatibilityEntryHash* hash) {
					mEntryHash = hash;
					return this;
				}

				CReuseCompletionGraphCompatibilityEntryHash* CReuseCompletionGraphCacheSlotItem::getEntryHash() {
					return mEntryHash;
				}


				cint64 CReuseCompletionGraphCacheSlotItem::getEntryCount() {
					return mEntyCount;
				}

				CReuseCompletionGraphCacheSlotItem* CReuseCompletionGraphCacheSlotItem::setEntryCount(cint64 entryCount) {
					mEntyCount = entryCount;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
