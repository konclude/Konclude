/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "COptimizedKPSetRolePossibleSubsumptionMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRolePossibleSubsumptionMap::COptimizedKPSetRolePossibleSubsumptionMap() : QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>() {
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

			CPropertyPossibleSubsumptionMapIterator* COptimizedKPSetRolePossibleSubsumptionMap::getIterator(CPropertyPossibleSubsumptionMapIterator* reuseIterator, CContext* context) {
				if (reuseIterator) {
					COptimizedKPSetRolePossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = (COptimizedKPSetRolePossibleSubsumptionMapIterator*)reuseIterator;
					*optKPSetClassPossSubMapIterator = COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::begin(),QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				} else {
					CMemoryAllocationManager* memMan = context->getMemoryAllocationManager();
					COptimizedKPSetRolePossibleSubsumptionMapIterator* optKPSetClassPossSubMapIterator = CObjectAllocator<COptimizedKPSetRolePossibleSubsumptionMapIterator>::allocateAndConstruct(memMan);
					*optKPSetClassPossSubMapIterator = COptimizedKPSetRolePossibleSubsumptionMapIterator(QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::begin(),QMap<CRoleTagComparer,COptimizedKPSetRolePossibleSubsumptionData*>::end());
					return optKPSetClassPossSubMapIterator;
				}
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
