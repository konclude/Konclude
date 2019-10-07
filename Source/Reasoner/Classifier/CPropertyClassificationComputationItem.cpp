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

#include "CPropertyClassificationComputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CPropertyClassificationComputationItem::CPropertyClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CRole *satisfiableTestedRole) {
				mSatCalcJob = satCalcJob;
				mTestValid = true;
				mRoleSatisfiableTest = true;
				mRoleSubsumptionTest = false;
				mRole1 = satisfiableTestedRole;
				mRole2 = nullptr;
			}

			CPropertyClassificationComputationItem::CPropertyClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CRole *subsumerTestedRole, CRole *subsumedTestedRole) {
				mSatCalcJob = satCalcJob;
				mTestValid = true;
				mRoleSatisfiableTest = false;
				mRoleSubsumptionTest = true;
				mRole1 = subsumerTestedRole;
				mRole2 = subsumedTestedRole;
			}



			bool CPropertyClassificationComputationItem::isRoleSatisfiableTest() {
				return mRoleSatisfiableTest;
			}

			bool CPropertyClassificationComputationItem::isRoleSubsumptionTest() {
				return mRoleSubsumptionTest;
			}


			CRole *CPropertyClassificationComputationItem::getSatisfiableTestedRole() {
				if (mRoleSatisfiableTest) {
					return mRole1;
				} 
				return nullptr;
			}

			CRole *CPropertyClassificationComputationItem::getSubsumerTestedRole() {
				if (mRoleSubsumptionTest) {
					return mRole1;
				} 
				return nullptr;
			}

			CRole *CPropertyClassificationComputationItem::getSubsumedTestedRole() {
				if (mRoleSubsumptionTest) {
					return mRole2;
				} 
				return nullptr;
			}

			CSatisfiableCalculationJob* CPropertyClassificationComputationItem::getSatisfiableCalculationJob() {
				return mSatCalcJob;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
