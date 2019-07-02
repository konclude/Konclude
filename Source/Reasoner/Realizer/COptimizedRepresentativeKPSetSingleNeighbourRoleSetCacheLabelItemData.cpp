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

#include "COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData() {
				mPossibleInstances = false;
			}


			bool COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::hasPossibleInstances() {
				return mPossibleInstances;
			}

			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::setPossibleInstances(bool possibleInstances) {
				mPossibleInstances = possibleInstances;
				return this;
			}

			QHash<COptimizedKPSetRoleInstancesItem*, COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*>* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::getInstancesItemDataHash() {
				return &mInstancesItemDataHash;
			}

			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::getInstancesItemData(COptimizedKPSetRoleInstancesItem* instanceItem, bool createIfNotExists) {
				COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData*& instancesData = mInstancesItemDataHash[instanceItem];
				if (!instancesData && createIfNotExists) {
					instancesData = new COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemInstancesData();
				}
				return instancesData;
			}

			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::getKnownInstancesItemList(bool inverse) {
				if (!inverse) {
					return &mKnownInstancesItemList;
				} else {
					return &mKnownInverseInstancesItemList;
				}
			}

			QList<COptimizedKPSetRoleInstancesItem*>* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::getPossibleInstancesItemList(bool inverse) {
				if (!inverse) {
					return &mPossibleInstancesItemList;
				} else {
					return &mPossibleInverseInstancesItemList;
				}
			}


			QList<COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData*>* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::getReferencingCombinedNeighbourRoleLabelItemDataList() {
				return &mReferencingCombinedNeighbourRoleLabelItemDataList;
			}

			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::addReferencingCombinedNeighbourRoleLabelDataItem(COptimizedRepresentativeKPSetCombinedNeighbourRoleSetCacheLabelItemData* itemData) {
				mReferencingCombinedNeighbourRoleLabelItemDataList.append(itemData);
				return this;
			}


			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::addKnownInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse) {
				if (!inverse) {
					mKnownInstancesItemList.append(roleItem);
				} else {
					mKnownInverseInstancesItemList.append(roleItem);
				}
				return this;
			}


			COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData* COptimizedRepresentativeKPSetSingleNeighbourRoleSetCacheLabelItemData::addPossibleInstancesItem(COptimizedKPSetRoleInstancesItem* roleItem, bool inverse) {
				if (!inverse) {
					mPossibleInstancesItemList.append(roleItem);
				} else {
					mPossibleInverseInstancesItemList.append(roleItem);
				}
				return this;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
