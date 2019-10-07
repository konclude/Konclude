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

#include "CIndividualSameTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CIndividualSameTestingItem::CIndividualSameTestingItem(COntologyRealizingItem* preproItem, COptimizedKPSetIndividualItem* instantiatedItem1, COptimizedKPSetIndividualItem* instantiatedItem2, COntologyRealizingDynamicRequirmentProcessingData* procData) : CRealizingTestingItem(preproItem,procData) {
				mOntologyPreproItem = preproItem;
				mInstantiatedItem1 = instantiatedItem1;
				mInstantiatedItem2 = instantiatedItem2;
			}


			CRealizingTestingItem::REALIZINGTESTINGTYPE CIndividualSameTestingItem::getRealizingTestingType() {
				return CRealizingTestingItem::INDIVIDUALSAMETESTINGTYPE;
			}


			COptimizedKPSetIndividualItem* CIndividualSameTestingItem::getInstantiatedItem1() {
				return mInstantiatedItem1;
			}

			COptimizedKPSetIndividualItem* CIndividualSameTestingItem::getInstantiatedItem2() {
				return mInstantiatedItem2;
			}
		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
