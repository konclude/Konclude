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

#include "COptimizedKPSetRolePossibleSubsumptionMapIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRolePossibleSubsumptionMapIterator::COptimizedKPSetRolePossibleSubsumptionMapIterator() {
			}


			COptimizedKPSetRolePossibleSubsumptionMapIterator::COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itBegin, QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itEnd) {
				mItBegin = itBegin;
				mItEnd = itEnd;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::hasNext() {
				return mItBegin != mItEnd;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::isSubsumptionConfirmed() {
				return mItBegin.value()->isSubsumptionConfirmed();
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::isSubsumptionInvalided() {
				return mItBegin.value()->isSubsumptionInvalided();
			}

			CRole* COptimizedKPSetRolePossibleSubsumptionMapIterator::getSubsumptionRole() {
				return mItBegin.key().getRole();
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::invalidateSubsumption() {
				bool wasInvalid = isSubsumptionInvalided();
				mItBegin.value()->setSubsumptionInvalid(true);
				return wasInvalid;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::confirmSubsumption() {
				bool wasConfirmed = isSubsumptionConfirmed();
				mItBegin.value()->setSubsumptionConfirmed(true);
				return wasConfirmed;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMapIterator::moveNext() {
				++mItBegin;
				return true;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
