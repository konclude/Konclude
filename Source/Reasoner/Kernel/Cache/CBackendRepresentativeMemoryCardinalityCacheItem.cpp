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

#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCardinalityCacheItem::CBackendRepresentativeMemoryCardinalityCacheItem(CBackendRepresentativeMemoryCacheContext* context) : CLinkerBase<cint64,CBackendRepresentativeMemoryCardinalityCacheItem>(0,nullptr),mContext(context) {
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalityCacheItem::initCacheEntry(cint64 signature, cint64 entryID) {
					setData(entryID);
					mSignature = signature;
					mCardinalityValueLinker = nullptr;
					mTagCardValueHash = nullptr;
					mCardinalityValueCount = 0;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCardinalityCacheItem::getCacheEntryID() {
					return getData();
				}

				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalityCacheItem::setCacheEntryID(cint64 entryID) {
					setData(entryID);
					return this;
				}


				cint64 CBackendRepresentativeMemoryCardinalityCacheItem::getSignature() {
					return mSignature;
				}

				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalityCacheItem::setSignature(cint64 signature) {
					mSignature = signature;
					return this;
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalityCacheItem::addCardinalityCacheValueLinker(CBackendRepresentativeMemoryCardinalityValueLinker* linker) {
					mCardinalityValueCount += linker->getCount();
					mCardinalityValueLinker = linker->append(mCardinalityValueLinker);
					return this;
				}


				CBackendRepresentativeMemoryCardinalityValueLinker* CBackendRepresentativeMemoryCardinalityCacheItem::getCardinalityCacheValueLinker() {
					return mCardinalityValueLinker;
				}


				CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* CBackendRepresentativeMemoryCardinalityCacheItem::getTagCardinalityCacheValueHash(bool create) {
					if (!mTagCardValueHash && create) {
						mTagCardValueHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					}
					return mTagCardValueHash;					
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCardinalityCacheItem::setTagCardinalityCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* hash) {
					mTagCardValueHash = hash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCardinalityCacheItem::getCardinalityCacheValueCount() {
					return mCardinalityValueCount;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
