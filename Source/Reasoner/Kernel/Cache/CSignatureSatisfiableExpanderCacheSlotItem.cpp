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

#include "CSignatureSatisfiableExpanderCacheSlotItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheSlotItem::CSignatureSatisfiableExpanderCacheSlotItem(CMemoryPool* memoryPool) : CLinkerBase<CSignatureSatisfiableExpanderCacheSlotItem*,CSignatureSatisfiableExpanderCacheSlotItem>(this),CMemoryPoolContainer(memoryPool) {
					mReaderUsing = false;
					mHasherItemHash = nullptr;
					mSigItemHash = nullptr;
				}

				bool CSignatureSatisfiableExpanderCacheSlotItem::incReader() {
					if (mReaderSharingCount.ref()) {
						mReaderUsing = true;
					}
					return true;
				}


				bool CSignatureSatisfiableExpanderCacheSlotItem::incReader(cint64 incCount) {
					for (cint64 i = 0; i < incCount; ++i) {
						incReader();
					}
					return mReaderUsing;
				}


				bool CSignatureSatisfiableExpanderCacheSlotItem::decReader() {
					if (!mReaderSharingCount.deref()) {
						mReaderUsing = false;
					}
					return mReaderUsing;
				}


				CSignatureSatisfiableExpanderCacheSlotItem* CSignatureSatisfiableExpanderCacheSlotItem::setSignatureItemHash(CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* sigItemHash) {
					mSigItemHash = sigItemHash;
					return this;
				}

				CSignatureSatisfiableExpanderCacheSlotItem* CSignatureSatisfiableExpanderCacheSlotItem::setHasherItemHash(CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* hasherItemHash) {
					mHasherItemHash = hasherItemHash;
					return this;
				}

				bool CSignatureSatisfiableExpanderCacheSlotItem::hasCacheReaders() {
					return mReaderUsing;
				}

				CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* CSignatureSatisfiableExpanderCacheSlotItem::getSignatureItemHash() {
					return mSigItemHash;
				}

				CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* CSignatureSatisfiableExpanderCacheSlotItem::getHasherItemHash() {
					return mHasherItemHash;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
