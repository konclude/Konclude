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

#include "CSignatureSatisfiableExpanderCacheReader.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheReader::CSignatureSatisfiableExpanderCacheReader() : CLinkerBase<CSignatureSatisfiableExpanderCacheReader*,CSignatureSatisfiableExpanderCacheReader>(this) {
					mCurrentSlot = nullptr;
				}


				CSignatureSatisfiableExpanderCacheReader* CSignatureSatisfiableExpanderCacheReader::updateSlot(CSignatureSatisfiableExpanderCacheSlotItem* updatedSlot) {
					CSignatureSatisfiableExpanderCacheSlotItem* prevSlot = mUpdatedSlot.fetchAndStoreOrdered(updatedSlot);
					if (prevSlot != nullptr) {
						prevSlot->decReader();
					}
					return this;
				}


				bool CSignatureSatisfiableExpanderCacheReader::hasUpdatedSlotItem() {
                    return mUpdatedSlot.fetchAndAddRelaxed(0) != nullptr;
				}


				bool CSignatureSatisfiableExpanderCacheReader::switchToUpdatedSlotItem() {
					CSignatureSatisfiableExpanderCacheSlotItem* updatedSlot = mUpdatedSlot.fetchAndStoreOrdered(nullptr);
					if (updatedSlot) {
						CSignatureSatisfiableExpanderCacheSlotItem* prevSlot = mCurrentSlot;
						mCurrentSlot = updatedSlot;
						if (prevSlot != nullptr) {
							prevSlot->decReader();
						}
						return true;
					}
					return false;
				}


				bool CSignatureSatisfiableExpanderCacheReader::hasCacheEntry(cint64 signature) {
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* sigItemHash = mCurrentSlot->getSignatureItemHash();
						if (sigItemHash) {
							return sigItemHash->contains(signature);
						}
					}
					return false;
				}



				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheReader::getCacheEntry(cint64 signature) {
					CSignatureSatisfiableExpanderCacheEntry* entry = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* sigItemHash = mCurrentSlot->getSignatureItemHash();
						if (sigItemHash) {
							CSignatureSatisfiableExpanderCacheRedirectionItem* item = sigItemHash->value(signature,nullptr);
							if (item) {
								entry = item->getCacheEntry();
							}
						}
					}
					return entry;
				}



				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheReader::getCacheEntry(CCACHINGSET<CCacheValue>* cacheValueSet) {
					CSignatureSatisfiableExpanderCacheEntry* entry = nullptr;
					if (hasUpdatedSlotItem()) {
						switchToUpdatedSlotItem();
					}
					if (mCurrentSlot) {
						CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* hasherItemHash = mCurrentSlot->getHasherItemHash();
						if (hasherItemHash) {
							CSignatureSatisfiableExpanderCacheHasher hashValue(cacheValueSet);
							CSignatureSatisfiableExpanderCacheRedirectionItem* item = hasherItemHash->value(hashValue,nullptr);
							if (item) {
								entry = item->getCacheEntry();
							}
						}
					}
					return entry;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
