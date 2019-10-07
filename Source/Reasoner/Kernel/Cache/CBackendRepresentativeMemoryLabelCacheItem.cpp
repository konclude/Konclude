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

#include "CBackendRepresentativeMemoryLabelCacheItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItem::CBackendRepresentativeMemoryLabelCacheItem(CBackendRepresentativeMemoryCacheContext* context) : CLinkerBase<cint64,CBackendRepresentativeMemoryLabelCacheItem>(0,nullptr),mContext(context) {
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::initCacheEntry(cint64 signature, cint64 entryID, LABEL_CACHE_ITEM_TYPE type) {
					initCachingStatusFlags();
					setData(entryID);
					mSignature = signature;
					mValueLinker = nullptr;
					mTagValueHash = nullptr;
					mValueCount = 0;
					mCacheItemType = type;

					mIndiAssociationCount = 0;

					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItemExtensionData::LABEL_CACHE_ITEM_EXTENSION_TYPE_COUNT; ++i) {
						mExtensionData[i] = nullptr;
					}

					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE CBackendRepresentativeMemoryLabelCacheItem::getLabelType() {
					return mCacheItemType;
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


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::addCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker) {
					mValueCount += linker->getCount();
					mValueLinker = linker->append(mValueLinker);
					return this;
				}


				CBackendRepresentativeMemoryLabelValueLinker* CBackendRepresentativeMemoryLabelCacheItem::getCacheValueLinker() {
					return mValueLinker;
				}


				CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* CBackendRepresentativeMemoryLabelCacheItem::getTagCacheValueHash(bool create) {
					if (!mTagValueHash && create) {
						mTagValueHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>,CContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(),mContext);
					}
					return mTagValueHash;					
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setTagCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* hash) {
					mTagValueHash = hash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryLabelCacheItem::getCacheValueCount() {
					return mValueCount;
				}

				bool CBackendRepresentativeMemoryLabelCacheItem::hasCachedTagValue(cint64 tag) {
					if (mTagValueHash) {
						return mTagValueHash->contains(tag);
					}
					return false;
				}


				CBackendRepresentativeMemoryLabelCacheItemExtensionData* CBackendRepresentativeMemoryLabelCacheItem::getExtensionData(cint64 extensionType) {
					return mExtensionData[extensionType];
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::setExtensionData(cint64 extensionType, CBackendRepresentativeMemoryLabelCacheItemExtensionData* extensionData) {
					mExtensionData[extensionType] = extensionData;
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::incIndividualAssociationCount(cint64 count) {
					mIndiAssociationCount += count;
					return this;
				}
				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItem::decIndividualAssociationCount(cint64 count) {
					mIndiAssociationCount -= count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryLabelCacheItem::getIndividualAssociationCount() {
					return mIndiAssociationCount;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
