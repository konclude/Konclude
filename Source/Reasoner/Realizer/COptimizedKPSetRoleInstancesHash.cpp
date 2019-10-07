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

#include "COptimizedKPSetRoleInstancesHash.h"
#include "COptimizedKPSetIndividualItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetRoleInstancesHash::COptimizedKPSetRoleInstancesHash() {
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleInstancesHash::getInstanceItemData(COptimizedKPSetIndividualItem* item) {
				return getInstanceItemData(item->getIndividualId());
			}

			COptimizedKPSetRoleInstancesData* COptimizedKPSetRoleInstancesHash::getInstanceItemData(cint64 indiId, bool create) {
				if (create) {
					COptimizedKPSetRoleInstancesHashData& hashData = QHash<cint64, COptimizedKPSetRoleInstancesHashData>::operator[](indiId);
					return hashData.mInstanceItemData;
				} else {
					COptimizedKPSetRoleInstancesHashData hashData = QHash<cint64, COptimizedKPSetRoleInstancesHashData>::value(indiId);
					return hashData.mInstanceItemData;
				}
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
