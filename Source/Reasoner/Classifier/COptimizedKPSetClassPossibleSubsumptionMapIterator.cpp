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

#include "COptimizedKPSetClassPossibleSubsumptionMapIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetClassPossibleSubsumptionMapIterator::COptimizedKPSetClassPossibleSubsumptionMapIterator() {
			}


			COptimizedKPSetClassPossibleSubsumptionMapIterator::COptimizedKPSetClassPossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator itBegin, QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::iterator itEnd) {
				mItBegin = itBegin;
				mItEnd = itEnd;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::hasNext() {
				return mItBegin != mItEnd;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::isSubsumptionConfirmed() {
				return mItBegin.value()->isSubsumptionConfirmed();
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::isSubsumptionInvalided() {
				return mItBegin.value()->isSubsumptionInvalided();
			}

			CConcept* COptimizedKPSetClassPossibleSubsumptionMapIterator::getSubsumptionConcept() {
				return mItBegin.key().getConcept();
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::invalidateSubsumption() {
				bool wasInvalid = isSubsumptionInvalided();
				mItBegin.value()->setSubsumptionInvalid(true);
				return wasInvalid;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::confirmSubsumption() {
				bool wasConfirmed = isSubsumptionConfirmed();
				mItBegin.value()->setSubsumptionConfirmed(true);
				return wasConfirmed;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMapIterator::moveNext() {
				++mItBegin;
				return true;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
