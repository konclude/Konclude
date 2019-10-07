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

#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualAssociationData::CBackendRepresentativeMemoryCacheIndividualAssociationData() {
				}



				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::initAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData) {
					initCachingStatusFlags(assData->getStatusFlags());
					mIndiID = assData->mIndiID;
					mRepresentativeSameIndiId = assData->mRepresentativeSameIndiId;
					mIncompletelyMarked = assData->mIncompletelyMarked;
					mIndirectlyConnectedNominalIndividual = assData->mIndirectlyConnectedNominalIndividual;
					mIndirectlyConnectedIndividualIntegration = assData->mIndirectlyConnectedIndividualIntegration;
					mCardinalityCacheEntry = assData->mCardinalityCacheEntry;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						mLabelCacheEntries[i] = assData->mLabelCacheEntries[i];
					}
					mNeighbourRoleSetHash = nullptr;
					mRoleSetNeighbourArray = nullptr;
					mAssociationDataUpdateId = assData->mAssociationDataUpdateId + 1;
					mCacheUpdateId = 0;
					mLastIntegratedIndirectlyConnectedIndividualsChangeId = assData->mLastIntegratedIndirectlyConnectedIndividualsChangeId;
					prevData = assData;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::initAssociationData(cint64 indiId) {
					initCachingStatusFlags();
					mAssociationDataUpdateId = 1;
					mIndiID = indiId;
					mRepresentativeSameIndiId = indiId;
					mIncompletelyMarked = false;
					mIndirectlyConnectedNominalIndividual = false;
					mIndirectlyConnectedIndividualIntegration = false;
					mCardinalityCacheEntry = nullptr;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						mLabelCacheEntries[i] = nullptr;
					}
					mNeighbourRoleSetHash = nullptr;
					mRoleSetNeighbourArray = nullptr;
					mCacheUpdateId = 0;
					mLastIntegratedIndirectlyConnectedIndividualsChangeId = 0;
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheIndividualAssociationData::getDeterministicConceptSetLabelCacheEntry() {
					return getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL);
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setDeterministicConceptSetLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem* cacheEntry) {
					setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL, cacheEntry);
					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheIndividualAssociationData::getLabelCacheEntry(cint64 labelType) {
					return mLabelCacheEntries[labelType];
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setLabelCacheEntry(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* cacheEntry) {
					mLabelCacheEntries[labelType] = cacheEntry;
					return this;
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCacheIndividualAssociationData::getBackendCardinalityCacheEntry() {
					return mCardinalityCacheEntry;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setBackendCardinalityCacheEntry(CBackendRepresentativeMemoryCardinalityCacheItem* cacheEntry) {
					mCardinalityCacheEntry = cacheEntry;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::isIncompletelyMarked() {
					return mIncompletelyMarked;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setIncompletelyMarked(bool marked) {
					mIncompletelyMarked = marked;
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* CBackendRepresentativeMemoryCacheIndividualAssociationData::getNeighbourRoleSetHash() {
					return mNeighbourRoleSetHash;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setNeighbourRoleSetHash(CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash) {
					mNeighbourRoleSetHash = neighbourRoleSetHash;
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* CBackendRepresentativeMemoryCacheIndividualAssociationData::getRoleSetNeighbourArray() {
					return mRoleSetNeighbourArray;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setRoleSetNeighbourArray(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray) {
					mRoleSetNeighbourArray = roleSetNeighbourArray;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::getAssociationDataUpdateId() {
					return mAssociationDataUpdateId;
				}

				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::getCacheUpdateId() {
					return mCacheUpdateId;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setCacheUpdateId(cint64 updateId) {
					mCacheUpdateId = updateId;
					return this;
				}




				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::getLastIntegratedIndirectlyConnectedIndividualsChangeId() {
					return mLastIntegratedIndirectlyConnectedIndividualsChangeId;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setLastIntegratedIndirectlyConnectedIndividualsChangeId(cint64 lastIntegratedChangeId) {
					mLastIntegratedIndirectlyConnectedIndividualsChangeId = lastIntegratedChangeId;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::isIndirectlyConnectedNominalIndividual() {
					return mIndirectlyConnectedNominalIndividual;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setIndirectlyConnectedNominalIndividual(bool indirectlyConnected) {
					mIndirectlyConnectedNominalIndividual = indirectlyConnected;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::hasIndirectlyConnectedIndividualIntegration() {
					return mIndirectlyConnectedIndividualIntegration;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setIndirectlyConnectedIndividualIntegration(bool indirectlyConnectedIndividualIntegration) {
					mIndirectlyConnectedIndividualIntegration = indirectlyConnectedIndividualIntegration;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::getRepresentativeSameIndividualId() {
					return mRepresentativeSameIndiId;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setRepresentativeSameIndividualId(cint64 indiId) {
					mRepresentativeSameIndiId = indiId;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::getAssociatedIndividualId() {
					return mIndiID;
				}

				cint64 CBackendRepresentativeMemoryCacheIndividualAssociationData::hasRepresentativeSameIndividualMerging() {
					return mIndiID != mRepresentativeSameIndiId;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
