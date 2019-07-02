/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "COptimizedKPSetRolePossibleSubsumptionMapIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRolePossibleSubsumptionMapIterator::COptimizedKPSetRolePossibleSubsumptionMapIterator() {
			}


			COptimizedKPSetRolePossibleSubsumptionMapIterator::COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itBegin, QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::iterator itEnd) {
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

			CConcept* COptimizedKPSetRolePossibleSubsumptionMapIterator::getSubsumptionConcept() {
				return mItBegin.key().getConcept();
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
