/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData(CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleLabelCacheItem) {
				mCombinedNeighbourRoleLabelCacheItem = combinedNeighbourRoleLabelCacheItem;
				mPossibleInstances = false;
			}


			bool COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::hasPossibleInstances() {
				return mPossibleInstances;
			}


			CBackendRepresentativeMemoryLabelCacheItem* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getCombinedNeighbourRoleLabelCacheItem() {
				return mCombinedNeighbourRoleLabelCacheItem;
			}


			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::setPossibleInstances(bool possibleInstances) {
				mPossibleInstances = possibleInstances;
				return this;
			}

			QHash<COptimizedKPSetRoleInstancesItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*>* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getInstancesItemDataHash() {
				return &mInstancesItemDataHash;
			}

			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getInstancesItemData(COptimizedKPSetRoleInstancesItem* instanceItem, bool createIfNotExists) {
				COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*& instancesData = mInstancesItemDataHash[instanceItem];
				if (!instancesData && createIfNotExists) {
					instancesData = new COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData();
				}
				return instancesData;
			}

			QSet<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getKnownInstancesItemSet(bool inverse) {
				if (!inverse) {
					return &mKnownInstancesItemSet;
				} else {
					return &mInverseKnownInstancesItemSet;
				}
			}

			QSet<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getPossibleInstancesItemSet(bool inverse) {
				if (!inverse) {
					return &mPossibleInstancesItemSet;
				} else {
					return &mInversePossibleInstancesItemSet;
				}
			}


			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::addKnownInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse) {
				if (!inverse) {
					mKnownInstancesItemSet.insert(roleItem);
				} else {
					mInverseKnownInstancesItemSet.insert(roleItem);
				}
				return this;
			}

			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::addPossibleInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse) {
				if (!inverse) {
					mPossibleInstancesItemSet.insert(roleItem);
				} else {
					mInversePossibleInstancesItemSet.insert(roleItem);
				}
				return this;
			}


			QSet<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::getComplexCandiateInstancesItemSet(bool inverse) {
				if (inverse) {
					return &mInverseComplexCandiateInstancesItemSet;
				} else {
					return &mComplexCandiateInstancesItemSet;
				}
			}

			COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::addComplexCandiateInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse) {
				if (inverse) {
					mInverseComplexCandiateInstancesItemSet.insert(roleItem);
				} else {
					mComplexCandiateInstancesItemSet.insert(roleItem);
				}
				return this;
			}


			bool COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData::hasComplexCandiateInstancesItems() {
				return !mComplexCandiateInstancesItemSet.isEmpty() || !mInverseComplexCandiateInstancesItemSet.isEmpty();
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
