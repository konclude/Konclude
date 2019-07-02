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

#include "COptimizedKPSetRolePossibleSubsumptionMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRolePossibleSubsumptionMap::COptimizedKPSetRolePossibleSubsumptionMap() : QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>() {
				mRequirePossibleSubsumptionUpdate = false;
				mRemainingPossibleSubsumptionCount = 0;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMap::isPossibleSubsumptionUpdateRequired() {
				return mRequirePossibleSubsumptionUpdate;
			}

			cint64 COptimizedKPSetRolePossibleSubsumptionMap::getRemainingPossibleSubsumptionCount() {
				return mRemainingPossibleSubsumptionCount;
			}



			COptimizedKPSetRolePossibleSubsumptionMap* COptimizedKPSetRolePossibleSubsumptionMap::setRemainingPossibleSubsumptionCount(cint64 possSubsumCount) {
				mRemainingPossibleSubsumptionCount = possSubsumCount;
				return this;
			}

			COptimizedKPSetRolePossibleSubsumptionMap* COptimizedKPSetRolePossibleSubsumptionMap::decRemainingPossibleSubsumptionCount(cint64 decCount) {
				mRemainingPossibleSubsumptionCount -= decCount;
				return this;
			}

			COptimizedKPSetRolePossibleSubsumptionMap* COptimizedKPSetRolePossibleSubsumptionMap::incRemainingPossibleSubsumptionCount(cint64 incCount) {
				mRemainingPossibleSubsumptionCount += incCount;
				return this;
			}

			COptimizedKPSetRolePossibleSubsumptionMap* COptimizedKPSetRolePossibleSubsumptionMap::setPossibleSubsumptionUpdateRequired(bool requiredUpdate) {
				mRequirePossibleSubsumptionUpdate = requiredUpdate;
				return this;
			}

			bool COptimizedKPSetRolePossibleSubsumptionMap::hasRemainingPossibleSubsumptions() {
				return mRemainingPossibleSubsumptionCount > 0;
			}

			CClassPossibleSubsumptionMapIterator* COptimizedKPSetRolePossibleSubsumptionMap::getIterator(CClassPossibleSubsumptionMapIterator* reuseIterator, CContext* context) {
				if (reuseIterator) {
					COptimizedKPSetRolePossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = (COptimizedKPSetRolePossibleSubsumptionMapIterator*)reuseIterator;
					*optKPSetClassPossSubMapIterator = COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::begin(),QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				} else {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					COptimizedKPSetRolePossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = CObjectAllocator<COptimizedKPSetRolePossibleSubsumptionMapIterator>::allocateAndConstruct(memMan);
					*optKPSetClassPossSubMapIterator = COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::begin(),QMap<CConceptTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				}
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
