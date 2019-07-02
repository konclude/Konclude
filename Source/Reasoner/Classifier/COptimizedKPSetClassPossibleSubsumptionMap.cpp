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

#include "COptimizedKPSetClassPossibleSubsumptionMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetClassPossibleSubsumptionMap::COptimizedKPSetClassPossibleSubsumptionMap() : QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>() {
				mRequirePossibleSubsumptionUpdate = false;
				mRemainingPossibleSubsumptionCount = 0;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMap::isPossibleSubsumptionUpdateRequired() {
				return mRequirePossibleSubsumptionUpdate;
			}

			cint64 COptimizedKPSetClassPossibleSubsumptionMap::getRemainingPossibleSubsumptionCount() {
				return mRemainingPossibleSubsumptionCount;
			}



			COptimizedKPSetClassPossibleSubsumptionMap* COptimizedKPSetClassPossibleSubsumptionMap::setRemainingPossibleSubsumptionCount(cint64 possSubsumCount) {
				mRemainingPossibleSubsumptionCount = possSubsumCount;
				return this;
			}

			COptimizedKPSetClassPossibleSubsumptionMap* COptimizedKPSetClassPossibleSubsumptionMap::decRemainingPossibleSubsumptionCount(cint64 decCount) {
				mRemainingPossibleSubsumptionCount -= decCount;
				return this;
			}

			COptimizedKPSetClassPossibleSubsumptionMap* COptimizedKPSetClassPossibleSubsumptionMap::incRemainingPossibleSubsumptionCount(cint64 incCount) {
				mRemainingPossibleSubsumptionCount += incCount;
				return this;
			}

			COptimizedKPSetClassPossibleSubsumptionMap* COptimizedKPSetClassPossibleSubsumptionMap::setPossibleSubsumptionUpdateRequired(bool requiredUpdate) {
				mRequirePossibleSubsumptionUpdate = requiredUpdate;
				return this;
			}

			bool COptimizedKPSetClassPossibleSubsumptionMap::hasRemainingPossibleSubsumptions() {
				return mRemainingPossibleSubsumptionCount > 0;
			}

			CClassPossibleSubsumptionMapIterator* COptimizedKPSetClassPossibleSubsumptionMap::getIterator(CClassPossibleSubsumptionMapIterator* reuseIterator, CContext* context) {
				if (reuseIterator) {
					COptimizedKPSetClassPossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = (COptimizedKPSetClassPossibleSubsumptionMapIterator*)reuseIterator;
					*optKPSetClassPossSubMapIterator = COptimizedKPSetClassPossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::begin(),QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				} else {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					COptimizedKPSetClassPossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = CObjectAllocator<COptimizedKPSetClassPossibleSubsumptionMapIterator>::allocateAndConstruct(memMan);
					*optKPSetClassPossSubMapIterator = COptimizedKPSetClassPossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::begin(),QMap<CConceptTagComparer,COptimizedKPSetClassPossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				}
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
