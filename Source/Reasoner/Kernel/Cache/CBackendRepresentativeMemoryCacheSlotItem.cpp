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



				CBackendRepresentativeMemoryCacheSlotItem::CBackendRepresentativeMemoryCacheSlotItem(CMemoryPool* memoryPool) : CLinkerBase<CBackendRepresentativeMemoryCacheSlotItem*, CBackendRepresentativeMemoryCacheSlotItem>(this), CMemoryPoolContainer(memoryPool) {
					mReaderUsing = false;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
						mSigLabelItemHash[i] = nullptr;
					}
					mNominaIIndividualdIndirectConnectionDataHash = nullptr;
					mSigCardLabelItemHash = nullptr;
					mIndiIdAssoDataHash = nullptr;
					mIndiIdAssoDataVector = nullptr;
					mIndiIdAssoDataVectorSize = 0;
					mSameIndividalsMergings = false;
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


				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash) {
					mSigLabelItemHash[labelType] = sigItemHash;
					return this;
				}


				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setSignatureCardinalityItemHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* sigItemHash) {
					mSigCardLabelItemHash = sigItemHash;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheSlotItem::hasCacheReaders() {
					return mReaderUsing;
				}

				bool CBackendRepresentativeMemoryCacheSlotItem::hasSameIndividualsMergings() {
					return mSameIndividalsMergings;
				}

				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setSameIndividualsMergings(bool sameIndisMergings) {
					mSameIndividalsMergings = sameIndisMergings;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* CBackendRepresentativeMemoryCacheSlotItem::getSignatureLabelItemHash(cint64 labelType) {
					return mSigLabelItemHash[labelType];
				}

				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* CBackendRepresentativeMemoryCacheSlotItem::getSignatureCardinalityItemHash() {
					return mSigCardLabelItemHash;
				}

				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* CBackendRepresentativeMemoryCacheSlotItem::getIndividualIdAssociationDataHash() {
					return mIndiIdAssoDataHash;
				}

				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash) {
					mIndiIdAssoDataHash = indiIdAssoDataHash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheSlotItem::getIndividualIdAssoiationDataVectorSize() {
					return mIndiIdAssoDataVectorSize;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData** CBackendRepresentativeMemoryCacheSlotItem::getIndividualIdAssoiationDataVector() {
					return mIndiIdAssoDataVector;
				}

				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector) {
					mIndiIdAssoDataVector = indiIdAssoDataVector;
					mIndiIdAssoDataVectorSize = indiIdAssoDataVectorSize;
					return this;
				}


				CBackendRepresentativeMemoryCacheSlotItem* CBackendRepresentativeMemoryCacheSlotItem::setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash) {
					mNominaIIndividualdIndirectConnectionDataHash = hash;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* CBackendRepresentativeMemoryCacheSlotItem::getNominaIIndividualdIndirectConnectionDataHash() {
					return mNominaIIndividualdIndirectConnectionDataHash;
				}




			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
