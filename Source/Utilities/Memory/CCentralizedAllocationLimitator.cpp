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

#include "CCentralizedAllocationLimitator.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CCentralizedAllocationLimitator::CCentralizedAllocationLimitator(CCentralizedAllocationLimitation* limitation) : CLinkerBase<CCentralizedAllocationLimitator*,CCentralizedAllocationLimitator>(this) {
				mMaxMemAllocSize = 0;
				mMemAllocated = 0;
				mReqUpdateFlag = true;
				mAllocLimitation = limitation;
			}

			CCentralizedAllocationLimitator::~CCentralizedAllocationLimitator() {
			}


			cint64 CCentralizedAllocationLimitator::getRemainingMemoryAllocationSize() {
				return mMaxMemAllocSize-mMemAllocated;
			}
			
			cint64 CCentralizedAllocationLimitator::getMaximumMemoryAllocationSize() {
				return mMaxMemAllocSize;
			}

			cint64 CCentralizedAllocationLimitator::getMemoryAllocatedSize() {
				return mMemAllocated;
			}


			bool CCentralizedAllocationLimitator::addAllocatingMemorySize(cint64 allocatedMemorySize) {
				if (mReqUpdateFlag) {
					mReqUpdateFlag = false;
					mMaxMemAllocSize = mAllocLimitation->getUpdatedMemoryAllocationLimit(this);
				}

				if (getRemainingMemoryAllocationSize() >= allocatedMemorySize) {
					mMemAllocated += allocatedMemorySize;
					return true;
				} else {
					while (allocatedMemorySize > 0) {
						mMaxMemAllocSize = mAllocLimitation->getUpdatedMemoryAllocationLimit(this);
						if (getRemainingMemoryAllocationSize() >= allocatedMemorySize) {
							mMemAllocated += allocatedMemorySize;
							return true;
						} else {
							cint64 tmpMemAllocSize = getRemainingMemoryAllocationSize();
							mMemAllocated += tmpMemAllocSize;
							allocatedMemorySize -= tmpMemAllocSize;
						}
					}
				}
				return false;
			}

			CCentralizedAllocationLimitator* CCentralizedAllocationLimitator::addReleasedMemorySize(cint64 allocatedMemorySize) {
				mMemAllocated -= allocatedMemorySize;
				return this;
			}

			CCentralizedAllocationLimitator* CCentralizedAllocationLimitator::notifyRequireUpdateLimit() {
				mReqUpdateFlag = true;
				return this;
			}



		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

