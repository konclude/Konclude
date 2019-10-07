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

#include "CPropertyRoleClassification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CPropertyRoleClassification::CPropertyRoleClassification() {
				mRolePropertiesHierarchy = nullptr;
				mStatsColl = nullptr;
				mIndiDepTracking = nullptr;
				mIndiDepTrackingSet = false;
			}

			CPropertyRoleClassification::~CPropertyRoleClassification() {
			}


			bool CPropertyRoleClassification::hasRolePropertiesHierarchy() {
				return mRolePropertiesHierarchy != nullptr;
			}

			CRolePropertiesHierarchy* CPropertyRoleClassification::getRolePropertiesHierarchy() {
				return mRolePropertiesHierarchy;
			}

			CPropertyRoleClassification* CPropertyRoleClassification::setRolePropertiesHierarchy(CRolePropertiesHierarchy* rolePropHierarchy) {
				mRolePropertiesHierarchy = rolePropHierarchy;
				return this;
			}


			bool CPropertyRoleClassification::hasClassificationStatistics() {
				return mStatsColl != nullptr;
			}

			CClassificationStatistics* CPropertyRoleClassification::getClassificationStatistics() {
				return mStatsColl;
			}

			CPropertyRoleClassification* CPropertyRoleClassification::setClassificationStatistics(CClassificationStatistics* statColl) {
				mStatsColl = statColl;
				return this;
			}


			bool CPropertyRoleClassification::hasDependentIndividualsTracking() {
				return mIndiDepTracking != nullptr;
			}

			CIndividualDependenceTracking* CPropertyRoleClassification::getDependentIndividualsTracking() {
				return mIndiDepTracking;
			}

			CPropertyRoleClassification* CPropertyRoleClassification::setDependentIndividualsTracking(CIndividualDependenceTracking* indiDepTracking) {
				mIndiDepTracking = indiDepTracking;
				mIndiDepTrackingSet = true;
				return this;
			}

			bool CPropertyRoleClassification::isIndividualDependentTrackingSet() {
				return mIndiDepTrackingSet;
			}

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude
