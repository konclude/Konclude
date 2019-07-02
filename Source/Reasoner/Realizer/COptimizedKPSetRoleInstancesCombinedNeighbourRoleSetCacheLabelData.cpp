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

#include "COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData() {
			}


			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::getKnownInstancesLabelItemDataHash() {
				return &mKnownInstancesLabelItemsData;
			}

			QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::getPossibleInstancesLabelItemDataHash() {
				return &mPossibleInstancesLabelItemsData;
			}


			COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::addKnownInstancesLabelItemData(COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData* labelItemData) {
				mKnownInstancesLabelItemsData.insert(labelItemData->getSingleNeighbourRoleSetCacheLabelItem(), labelItemData);
				return this;
			}

			COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::addPossibleInstancesLabelItemData(COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData* labelItemData) {
				mPossibleInstancesLabelItemsData.insert(labelItemData->getSingleNeighbourRoleSetCacheLabelItem(), labelItemData);
				return this;
			}

			bool COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::hasKnownInstancesLabelItems() {
				return !mKnownInstancesLabelItemsData.isEmpty();
			}

			bool COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::hasPossibleInstancesLabelItems() {
				return !mPossibleInstancesLabelItemsData.isEmpty();
			}

			bool COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::hasKnownInstancesLabelItem(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem) {
				return mKnownInstancesLabelItemsData.contains(cacheLabelItem);
			}

			bool COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData::hasPossibleInstancesLabelItem(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem) {
				return mPossibleInstancesLabelItemsData.contains(cacheLabelItem);
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
