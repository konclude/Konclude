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

#include "CIndividualPrecomputationTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CIndividualPrecomputationTestingItem::CIndividualPrecomputationTestingItem(CConsistenceObserver* observer, CIndividualPrecomputationCoordinationHash* indiComCoordHash, COntologyPrecomputationItem* preCompItem, bool* unstatFlagPointer) : CPrecomputationTestingItem(preCompItem),mObserver(observer), mRepresentativeCacheUpdatingAdapter(indiComCoordHash, unstatFlagPointer) {
				mTaskPreyingAdapter.setConsitenceObserver(observer);
				mIndiCompCoordHash = indiComCoordHash;
			}

			CPrecomputationTestingItem::PRECOMPUTATIONTYPE CIndividualPrecomputationTestingItem::getPrecomputationTestingType() {
				return INDIVIDUALPRECOMPUTATIONTYPE;
			}

			CTaskPreyingAdapter* CIndividualPrecomputationTestingItem::getTaskPreyingAdapter() {
				return &mTaskPreyingAdapter;
			}


			CSatisfiableTaskRepresentativeBackendUpdatingAdapter* CIndividualPrecomputationTestingItem::getRepresentativeCacheUpdatingAdapter() {
				return &mRepresentativeCacheUpdatingAdapter;
			}

			QList<CIndividualReference>* CIndividualPrecomputationTestingItem::getIndividualComputationList() {
				return &mIndiComputationList;
			}

			CIndividualPrecomputationTestingItem* CIndividualPrecomputationTestingItem::addIndividualComputations(QList<CIndividualReference>& indiRefList) {
				if (mIndiComputationList.isEmpty()) {
					mIndiComputationList = indiRefList;
				} else {
					mIndiComputationList.append(indiRefList);
				}
				return this;
			}

			CIndividualPrecomputationCoordinationHash* CIndividualPrecomputationTestingItem::getIndividualComputationCoordinationHash() {
				return mIndiCompCoordHash;
			}

			CIndividualPrecomputationTestingItem* CIndividualPrecomputationTestingItem::setIndividualComputationCoordinationHash(CIndividualPrecomputationCoordinationHash* indiComCoordHash) {
				mIndiCompCoordHash = indiComCoordHash;
				return this;
			}



		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
