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

#include "CBackendRepresentativeMemoryCacheSlotItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheSlotItem::CBackendRepresentativeMemoryCacheSlotItem(CMemoryPool* memoryPool) : CLinkerBase<CBackendRepresentativeMemoryCacheSlotItem*,CBackendRepresentativeMemoryCacheSlotItem>(this),CMemoryPoolContainer(memoryPool) {
					mReaderUsing = false;
					mSigLabelItemHash = nullptr;
					mSigCardLabelItemHash = nullptr;
				}

				bool CBackendRepresentativeMemoryCacheSlotItem::incReader() {
					if (mReaderSharingCount.ref()) {
						mReaderUsing = true;
					}
					return true;
				}


				bool CBackendRepresentativeMemoryCacheSlotItem::incReader(cint64 incCount) {
					for (cint64 i = 0; i < incCount; ++i) {
						incReader();
					}
					return mReaderUsing;
				}


				bool CBackendRepresentativeMemoryCacheSlotItem::decReader() {
					if (!mReaderSharingCount.deref()) {
						mReaderUsing = false;
					}
					return mReaderUsing;
				}


				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setSignatureLabelItemHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* sigItemHash) {
					mSigLabelItemHash = sigItemHash;
					return this;
				}


				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setSignatureCardinalityItemHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* sigItemHash) {
					mSigCardLabelItemHash = sigItemHash;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheSlotItem::hasCacheReaders() {
					return mReaderUsing;
				}

				CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* CBackendRepresentativeMemoryCacheSlotItem::getSignatureLabelItemHash() {
					return mSigLabelItemHash;
				}

				CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* CBackendRepresentativeMemoryCacheSlotItem::getSignatureCardinalityItemHash() {
					return mSigCardLabelItemHash;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
