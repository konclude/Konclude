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

#include "CLimitedReserveMemoryPoolAllocationManager.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CLimitedReserveMemoryPoolAllocationManager::CLimitedReserveMemoryPoolAllocationManager(CMemoryPoolProvider* memoryPoolProvider, cint64 freeReserveLimit, cint64 freeReleaseBlockSize) : CMemoryPoolAllocationManager(memoryPoolProvider) {
				mFreeReserveLimit = freeReserveLimit;
				mFreeReleaseBlockSize = freeReleaseBlockSize;
			}

			CLimitedReserveMemoryPoolAllocationManager::~CLimitedReserveMemoryPoolAllocationManager() {
			}


			CMemoryTemporaryAllocationManager* CLimitedReserveMemoryPoolAllocationManager::releaseTemporaryMemoryPool(CMemoryPool* memoryPool) {
				memoryPool->resetMemoryBlockPointer();
				memoryPool->setNextMemoryPool(mFreeMemoryPoolList);
				mFreeMemoryPoolList = memoryPool;
				++mStatMemToFreeCount;
				++mFreeMemoryPoolCount;
				if (mFreeMemoryPoolCount > mFreeReserveLimit) {
					CMemoryPool* tmpMemoryPoolIt = mFreeMemoryPoolList;
					CMemoryPool* releaseMemoryPool = mFreeMemoryPoolList;
					for (cint64 cnt = 0; cnt < mFreeReleaseBlockSize && tmpMemoryPoolIt->hasNext(); ++cnt) {
						tmpMemoryPoolIt = tmpMemoryPoolIt->getNext();
						--mFreeMemoryPoolCount;
					}
					--mFreeMemoryPoolCount;
					mFreeMemoryPoolList = tmpMemoryPoolIt->getNext();
					tmpMemoryPoolIt->clearNext();
					mMemoryPoolProvider->releaseMemoryPool(releaseMemoryPool);
				}
				return this;
			}



		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

