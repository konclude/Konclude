/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CDatatypeStringValueDataExclusion.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeStringValueDataExclusion::CDatatypeStringValueDataExclusion() {
				}


				CDatatypeStringValueDataExclusion* CDatatypeStringValueDataExclusion::initDataExclusion(CDatatypeStringValueDataExclusion* value) {
					if (value) {
						mPosExclusionType = value->mPosExclusionType;
						mNegExclusionType = value->mNegExclusionType;
						mPosExclusionDepTrackPoint = value->mPosExclusionDepTrackPoint;
						mNegExclusionDepTrackPoint = value->mNegExclusionDepTrackPoint;
						mExcluded = value->mExcluded;
						mExcludedDepTrackPoint = value->mExcludedDepTrackPoint;
					} else {
						mPosExclusionType = CDatatypeStringValueExclusionType::SVET_NOTHING;
						mNegExclusionType = CDatatypeStringValueExclusionType::SVET_ALL;
						mPosExclusionDepTrackPoint = nullptr;
						mNegExclusionDepTrackPoint = nullptr;
						mExcludedDepTrackPoint = nullptr;
						mExcluded = false;
					}
					return this;
				}


				bool CDatatypeStringValueDataExclusion::isExcluded() {
					return mExcluded;
				}

				CDependencyTrackPoint* CDatatypeStringValueDataExclusion::getExcludedDependencyTrackPoint() {
					return mExcludedDepTrackPoint;
				}

				CDatatypeStringValueDataExclusion* CDatatypeStringValueDataExclusion::setExcluded(bool excluded) {
					mExcluded = excluded;
					return this;
				}

				CDatatypeStringValueDataExclusion* CDatatypeStringValueDataExclusion::setExcludedDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint) {
					mExcludedDepTrackPoint = depTrackPoint;
					return this;
				}



				bool CDatatypeStringValueDataExclusion::isSomethingExcluded() {
					if (isExcluded()) {
						return true;
					}
					if (mPosExclusionType != CDatatypeStringValueExclusionType::SVET_NOTHING || mNegExclusionType != CDatatypeStringValueExclusionType::SVET_ALL) {
						return true;
					} else {
						return false;
					}
				}

				bool CDatatypeStringValueDataExclusion::isEverythingExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mNegExclusionType <= (cint64)mPosExclusionType) {
						return true;
					}
					if (mPosExclusionType == CDatatypeStringValueExclusionType::SVET_ALL || mNegExclusionType == CDatatypeStringValueExclusionType::SVET_NOTHING) {
						return true;
					}
					return false;
				}



				bool CDatatypeStringValueDataExclusion::areOnlyLanguageTagsAllowed() {
					if (isExcluded()) {
						return false;
					}
					if ((cint64)mPosExclusionType == (cint64)CDatatypeStringValueExclusionType::SVET_NOTHING 
								&& (cint64)mNegExclusionType == (cint64)CDatatypeStringValueExclusionType::SVET_LANGUAGE_TAGS_COMPLETELY) {

						return true;
					}
					return false;
				}


				bool CDatatypeStringValueDataExclusion::areLanguageTagsExcluded() {
					if (isExcluded()) {
						return true;
					}
					if ((cint64)mPosExclusionType >= (cint64)CDatatypeStringValueExclusionType::SVET_LANGUAGE_TAGS_COMPLETELY) {
						return true;
					}
					if ((cint64)mNegExclusionType < (cint64)CDatatypeStringValueExclusionType::SVET_LANGUAGE_TAGS_COMPLETELY) {
						return true;
					}
					return false;
				}


				bool CDatatypeStringValueDataExclusion::isExcluded(CDatatypeStringValueExclusionType* type) {
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


				CDatatypeStringValueDataExclusion* CDatatypeStringValueDataExclusion::setExclusion(CDatatypeStringValueExclusionType* type, CDependencyTrackPoint* depTrackPoint) {
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

				CDependencyTrackPoint* CDatatypeStringValueDataExclusion::getExclusionDependencyTrackPoint(CDatatypeStringValueExclusionType* type) {
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



				CDependencyTrackPoint* CDatatypeStringValueDataExclusion::getPositiveExclusionDependencyTrackPoint() {
					return mPosExclusionDepTrackPoint;
				}

				CDependencyTrackPoint* CDatatypeStringValueDataExclusion::getNegativeExclusionDependencyTrackPoint() {
					return mNegExclusionDepTrackPoint;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
