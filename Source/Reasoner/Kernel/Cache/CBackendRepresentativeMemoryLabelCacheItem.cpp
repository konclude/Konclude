/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CBackendRepresentativeMemoryLabelCacheItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItem::CBackendRepresentativeMemoryLabelCacheItem(CBackendRepresentativeMemoryCacheContext* context) : CLinkerBase<cint64,CBackendRepresentativeMemoryLabelCacheItem>(0,nullptr),mContext(context) {
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::initCacheEntry(cint64 signature, cint64 entryID) {
					setData(entryID);
					mSignature = signature;
					mDetValueLinker = nullptr;
					mDetTagValueHash = nullptr;
					mDetValueCount = 0;
					mCompletelyHandled = false;
					mCompletelySaturated = false;
					return this;
				}


				cint64 CBackendRepresentativeMemoryLabelCacheItem::getCacheEntryID() {
					return getData();
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setCacheEntryID(cint64 entryID) {
					setData(entryID);
					return this;
				}


				cint64 CBackendRepresentativeMemoryLabelCacheItem::getSignature() {
					return mSignature;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setSignature(cint64 signature) {
					mSignature = signature;
					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::addDeterministicCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker) {
					mDetValueCount += linker->getCount();
					mDetValueLinker = linker->append(mDetValueLinker);
					return this;
				}


				CBackendRepresentativeMemoryLabelValueLinker* CBackendRepresentativeMemoryLabelCacheItem::getDeterministicCacheValueLinker() {
					return mDetValueLinker;
				}


				CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* CBackendRepresentativeMemoryLabelCacheItem::getDeterministicTagCacheValueHash(bool create) {
					if (!mDetTagValueHash && create) {
						mDetTagValueHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					}
					return mDetTagValueHash;					
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setDeterministicTagCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* hash) {
					mDetTagValueHash = hash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryLabelCacheItem::getDeterministicCacheValueCount() {
					return mDetValueCount;
				}

				bool CBackendRepresentativeMemoryLabelCacheItem::isCompletelyHandled() {
					return mCompletelyHandled;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setCompletelyHandled(bool completelyHandled) {
					mCompletelyHandled = completelyHandled;
					return this;
				}


				bool CBackendRepresentativeMemoryLabelCacheItem::isCompletelySaturated() {
					return mCompletelySaturated;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setCompletelySaturated(bool completelySaturated) {
					mCompletelySaturated = completelySaturated;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
