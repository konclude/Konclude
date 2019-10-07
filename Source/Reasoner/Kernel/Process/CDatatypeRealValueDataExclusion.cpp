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

#include "CDatatypeRealValueDataExclusion.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeRealValueDataExclusion::CDatatypeRealValueDataExclusion() {
				}


				CDatatypeRealValueDataExclusion* CDatatypeRealValueDataExclusion::initDataExclusion(CDatatypeRealValueDataExclusion* value) {
					if (value) {
						mPosExclusionType = value->mPosExclusionType;
						mNegExclusionType = value->mNegExclusionType;
						mPosExclusionDepTrackPoint = value->mPosExclusionDepTrackPoint;
						mNegExclusionDepTrackPoint = value->mNegExclusionDepTrackPoint;
						mExcluded = value->mExcluded;
						mExcludedDepTrackPoint = value->mExcludedDepTrackPoint;
					} else {
						mPosExclusionType = CDatatypeRealValueExclusionType::RVET_NOTHING;
						mNegExclusionType = CDatatypeRealValueExclusionType::RVET_ALL;
						mPosExclusionDepTrackPoint = nullptr;
						mNegExclusionDepTrackPoint = nullptr;
						mExcludedDepTrackPoint = nullptr;
						mExcluded = false;
					}
					return this;
				}


				bool CDatatypeRealValueDataExclusion::isExcluded() {
					return mExcluded;
				}

				CDependencyTrackPoint* CDatatypeRealValueDataExclusion::getExcludedDependencyTrackPoint() {
					return mExcludedDepTrackPoint;
				}

				CDatatypeRealValueDataExclusion* CDatatypeRealValueDataExclusion::setExcluded(bool excluded) {
					mExcluded = excluded;
					return this;
				}

				CDatatypeRealValueDataExclusion* CDatatypeRealValueDataExclusion::setExcludedDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint) {
					mExcludedDepTrackPoint = depTrackPoint;
					return this;
				}



				bool CDatatypeRealValueDataExclusion::isSomethingExcluded() {
					if (isExcluded()) {
						return true;
					}
					if (mPosExclusionType != CDatatypeRealValueExclusionType::RVET_NOTHING || mNegExclusionType != CDatatypeRealValueExclusionType::RVET_ALL) {
						return true;
					} else {
						return false;
					}
				}

				bool CDatatypeRealValueDataExclusion::isEverythingExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mNegExclusionType <= (cint64)mPosExclusionType) {
						return true;
					}
					if (mPosExclusionType == CDatatypeRealValueExclusionType::RVET_ALL || mNegExclusionType == CDatatypeRealValueExclusionType::RVET_NOTHING) {
						return true;
					}
					return false;
				}



				bool CDatatypeRealValueDataExclusion::areOnlyIntegerValuesAllowed() {
					if (isExcluded()) {
						return false;
					}
					if ((cint64)mPosExclusionType == (cint64)CDatatypeRealValueExclusionType::RVET_NOTHING 
								&& (cint64)mNegExclusionType == (cint64)CDatatypeRealValueExclusionType::RVET_INTEGER) {

						return true;
					}
					return false;
				}


				bool CDatatypeRealValueDataExclusion::areIntegerValuesExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mPosExclusionType >= (cint64)CDatatypeRealValueExclusionType::RVET_INTEGER) {
						return true;
					}
					if ((cint64)mNegExclusionType < (cint64)CDatatypeRealValueExclusionType::RVET_INTEGER) {
						return true;
					}
					return false;
				}


				bool CDatatypeRealValueDataExclusion::areDecimalValuesExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mPosExclusionType >= (cint64)CDatatypeRealValueExclusionType::RVET_DECIMAL) {
						return true;
					}
					if ((cint64)mNegExclusionType < (cint64)CDatatypeRealValueExclusionType::RVET_DECIMAL) {
						return true;
					}
					return false;
				}


				bool CDatatypeRealValueDataExclusion::areRationalValuesExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mPosExclusionType >= (cint64)CDatatypeRealValueExclusionType::RVET_RATIONAL) {
						return true;
					}
					if ((cint64)mNegExclusionType < (cint64)CDatatypeRealValueExclusionType::RVET_RATIONAL) {
						return true;
					}
					return false;
				}


				bool CDatatypeRealValueDataExclusion::areRealValuesExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mPosExclusionType >= (cint64)CDatatypeRealValueExclusionType::RVET_REAL) {
						return true;
					}
					if ((cint64)mNegExclusionType < (cint64)CDatatypeRealValueExclusionType::RVET_REAL) {
						return true;
					}
					return false;
				}


				bool CDatatypeRealValueDataExclusion::isExcluded(CDatatypeRealValueExclusionType* type) {
					if (isExcluded()) {
						return true;
					}
					if (type->getExclusionNegation()) {
						if ((cint64)type->getExclusionType() >= (cint64)mNegExclusionType) {
							return true;
						}
					} else {
						if ((cint64)type->getExclusionType() <= (cint64)mPosExclusionType) {
							return true;
						}
					}
					return false;
				}

				CDatatypeRealValueDataExclusion* CDatatypeRealValueDataExclusion::setExclusion(CDatatypeRealValueExclusionType* type, CDependencyTrackPoint* depTrackPoint) {
					if (!isExcluded(type)) {
						if (type->getExclusionNegation()) {
							mNegExclusionType = type->getExclusionType();
							mNegExclusionDepTrackPoint = depTrackPoint;
						} else {
							mPosExclusionType = type->getExclusionType();
							mPosExclusionDepTrackPoint = depTrackPoint;
						}
					}
					return this;
				}

				CDependencyTrackPoint* CDatatypeRealValueDataExclusion::getExclusionDependencyTrackPoint(CDatatypeRealValueExclusionType* type) {
					if (isExcluded()) {
						return getExcludedDependencyTrackPoint();
					}
					if (!isExcluded(type)) {
						return nullptr;
					} else if (type->getExclusionNegation()) {
						return mNegExclusionDepTrackPoint;
					} else {
						return mPosExclusionDepTrackPoint;
					}
					return nullptr;
				}



				CDependencyTrackPoint* CDatatypeRealValueDataExclusion::getPositiveExclusionDependencyTrackPoint() {
					return mPosExclusionDepTrackPoint;
				}

				CDependencyTrackPoint* CDatatypeRealValueDataExclusion::getNegativeExclusionDependencyTrackPoint() {
					return mNegExclusionDepTrackPoint;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
