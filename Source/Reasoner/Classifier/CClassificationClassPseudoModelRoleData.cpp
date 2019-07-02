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

#include "CClassificationClassPseudoModelRoleData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {

			CClassificationClassPseudoModelRoleData::CClassificationClassPseudoModelRoleData() {
				mLowerAtLeast = 0;
				mUpperAtLeast = 0;
				mLowerAtMost = 0;
				mUpperAtMost = 0;
				mSuccessorModel = 0;
			}

			cint64 CClassificationClassPseudoModelRoleData::getLowerAtLeastBound() {
				return mLowerAtLeast;
			}

			cint64 CClassificationClassPseudoModelRoleData::getUpperAtLeastBound() {
				return mUpperAtLeast;
			}


			cint64 CClassificationClassPseudoModelRoleData::getLowerAtMostBound() {
				return mLowerAtMost;
			}

			cint64 CClassificationClassPseudoModelRoleData::getUpperAtMostBound() {
				return mUpperAtMost;
			}


			bool CClassificationClassPseudoModelRoleData::setLowerAtLeastBound(cint64 bound) {
				bool diff = mLowerAtLeast != bound;
				mLowerAtLeast = bound;
				return diff;
			}

			bool CClassificationClassPseudoModelRoleData::setUpperAtLeastBound(cint64 bound) {
				bool diff = mUpperAtLeast != bound;
				mUpperAtLeast = bound;
				return diff;
			}

			bool CClassificationClassPseudoModelRoleData::setLowerAtMostBound(cint64 bound) {
				bool diff = mLowerAtMost != bound;
				mLowerAtMost = bound;
				return diff;
			}

			bool CClassificationClassPseudoModelRoleData::setUpperAtMostBound(cint64 bound) {
				bool diff = mUpperAtMost != bound;
				mUpperAtMost = bound;
				return diff;
			}

			cint64 CClassificationClassPseudoModelRoleData::getSuccessorModelID() const {
				return mSuccessorModel;
			}

			bool CClassificationClassPseudoModelRoleData::setSuccessorModelID(cint64 modelID) {
				bool diff = mSuccessorModel != modelID;
				mSuccessorModel = modelID;
				return diff;
			}

			bool CClassificationClassPseudoModelRoleData::isPossibleSubsumerOf(const CClassificationClassPseudoModelRoleData& possibleSubsumedData) const {
				if (mDeterministicFlag) {
					if (mLowerAtLeast > possibleSubsumedData.mUpperAtLeast) {
						return false;
					}
					if (mUpperAtMost < possibleSubsumedData.mLowerAtMost) {
						return false;
					}
					return true;
				} else {
					return true;
				}
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
