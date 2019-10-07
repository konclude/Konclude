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

#include "COptimizedKPSetRoleNeighbourInstancesHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetRoleNeighbourInstancesHash::COptimizedKPSetRoleNeighbourInstancesHash() {
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHash::getRoleSuccessorInstanceItemData(COptimizedKPSetRoleInstancesItem* item, COptimizedKPSetIndividualItem* successorItem, bool create) {
				if (create) {
					COptimizedKPSetRoleNeighbourInstancesHashData& hashData = QHash<COptimizedKPSetRoleInstancesItem*, COptimizedKPSetRoleNeighbourInstancesHashData>::operator[](item);
					return hashData.getRoleSuccessorInstanceItemData(successorItem);
				} else {
					COptimizedKPSetRoleNeighbourInstancesHashData hashData = QHash<COptimizedKPSetRoleInstancesItem*, COptimizedKPSetRoleNeighbourInstancesHashData>::value(item);
					return hashData.getRoleSuccessorInstanceItemData(successorItem);
				}
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleNeighbourInstancesHash::getRolePredecessorInstanceItemData(COptimizedKPSetRoleInstancesItem* item, COptimizedKPSetIndividualItem* predecessorItem, bool create) {
				if (create) {
					COptimizedKPSetRoleNeighbourInstancesHashData& hashData = QHash<COptimizedKPSetRoleInstancesItem*, COptimizedKPSetRoleNeighbourInstancesHashData>::operator[](item);
					return hashData.getRolePredecessorInstanceItemData(predecessorItem);
				} else {
					COptimizedKPSetRoleNeighbourInstancesHashData hashData = QHash<COptimizedKPSetRoleInstancesItem*, COptimizedKPSetRoleNeighbourInstancesHashData>::value(item);
					return hashData.getRolePredecessorInstanceItemData(predecessorItem);
				}
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
