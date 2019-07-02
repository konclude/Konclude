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

#include "COptimizedKPSetClassPossibleSubsumptionData.h"
#include "COptimizedKPSetClassTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetClassPossibleSubsumptionData::COptimizedKPSetClassPossibleSubsumptionData(COptimizedKPSetClassTestingItem* item) {
				mSubsumptionUpdatedFlag = false;
				mSubsumptionConfirmedFlag = false;
				mSubsumptionInvalidFlag = false;
				mClassItem = item;
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isUpdateRequired() {
				return !mSubsumptionUpdatedFlag && (mSubsumptionConfirmedFlag || mSubsumptionInvalidFlag);
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isSubsumptionUpdated() {
				return mSubsumptionUpdatedFlag;
			}

			COptimizedKPSetClassPossibleSubsumptionData* COptimizedKPSetClassPossibleSubsumptionData::setSubsumptionUpdated(bool updated) {
				mSubsumptionUpdatedFlag = updated;
				return this;
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isSubsumptionConfirmed() {
				return mSubsumptionConfirmedFlag;
			}

			COptimizedKPSetClassPossibleSubsumptionData* COptimizedKPSetClassPossibleSubsumptionData::setSubsumptionConfirmed(bool confirmedSubsumption) {
				mSubsumptionConfirmedFlag = confirmedSubsumption;
				return this;
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isSubsumptionInvalided() {
				return mSubsumptionInvalidFlag;
			}

			COptimizedKPSetClassPossibleSubsumptionData* COptimizedKPSetClassPossibleSubsumptionData::setSubsumptionInvalid(bool invalidSubsumption) {
				//QString iriClassNameString1 = CIRIName::getRecentIRIName(mClassItem->getTestingConcept()->getClassNameLinker());
				//if (iriClassNameString1 == "#EntireMolecularEntity") {
				//	bool bug = true;
				//}
				mSubsumptionInvalidFlag = invalidSubsumption;
				return this;
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isSubsumptionKnown() {
				return mSubsumptionInvalidFlag || mSubsumptionConfirmedFlag;
			}

			bool COptimizedKPSetClassPossibleSubsumptionData::isSubsumptionUnknown() {
				return !mSubsumptionConfirmedFlag && !mSubsumptionInvalidFlag;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassPossibleSubsumptionData::getClassItem() {
				return mClassItem;
			}

			COptimizedKPSetClassPossibleSubsumptionData* COptimizedKPSetClassPossibleSubsumptionData::setClassItem(COptimizedKPSetClassTestingItem* item) {
				mClassItem = item;
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
