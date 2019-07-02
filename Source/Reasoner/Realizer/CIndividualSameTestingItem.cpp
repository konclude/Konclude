/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CIndividualSameTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CIndividualSameTestingItem::CIndividualSameTestingItem(COntologyRealizingItem* preproItem, COptimizedKPSetConceptInstantiatedItem* instantiatedItem1, COptimizedKPSetConceptInstantiatedItem* instantiatedItem2) : CRealizingTestingItem(preproItem) {
				mOntologyPreproItem = preproItem;
				mInstantiatedItem1 = instantiatedItem1;
				mInstantiatedItem2 = instantiatedItem2;
			}


			CRealizingTestingItem::REALIZINGTESTINGTYPE CIndividualSameTestingItem::getRealizingTestingType() {
				return CRealizingTestingItem::INDIVIDUALSAMETESTINGTYPE;
			}


			COptimizedKPSetConceptInstantiatedItem* CIndividualSameTestingItem::getInstantiatedItem1() {
				return mInstantiatedItem1;
			}

			COptimizedKPSetConceptInstantiatedItem* CIndividualSameTestingItem::getInstantiatedItem2() {
				return mInstantiatedItem2;
			}
		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
