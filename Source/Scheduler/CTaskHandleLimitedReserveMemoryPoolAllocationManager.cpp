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

#include "CTaskHandleLimitedReserveMemoryPoolAllocationManager.h"


namespace Konclude {

	namespace Scheduler {



		CTaskHandleLimitedReserveMemoryPoolAllocationManager::CTaskHandleLimitedReserveMemoryPoolAllocationManager(CConsiderateMemoryPoolProvider* memoryPoolProvider, cint64 freeReserveLimit, cint64 freeReleaseBlockSize) : CLimitedReserveMemoryPoolAllocationManager(memoryPoolProvider,freeReserveLimit,freeReleaseBlockSize) {
			mConsideratedMemPoolProvider = memoryPoolProvider;
		}

		CTaskHandleLimitedReserveMemoryPoolAllocationManager::~CTaskHandleLimitedReserveMemoryPoolAllocationManager() {
		}

		void* CTaskHandleLimitedReserveMemoryPoolAllocationManager::allocate(size_t size) {
			return CLimitedReserveMemoryPoolAllocationManager::allocate(size);
		}

		CMemoryAllocationManager* CTaskHandleLimitedReserveMemoryPoolAllocationManager::release(void* address) {
			return CLimitedReserveMemoryPoolAllocationManager::release(address);
		}


		void* CTaskHandleLimitedReserveMemoryPoolAllocationManager::allocateAligned(size_t size, int alignment) {
			return CLimitedReserveMemoryPoolAllocationManager::allocateAligned(size,alignment);
		}


		CMemoryTemporaryAllocationManager* CTaskHandleLimitedReserveMemoryPoolAllocationManager::releaseAllMemory() {
			return CLimitedReserveMemoryPoolAllocationManager::releaseAllMemory();
		}


		CMemoryPool* CTaskHandleLimitedReserveMemoryPoolAllocationManager::acqireTemporaryMemoryPool(size_t size) {
			return CLimitedReserveMemoryPoolAllocationManager::acqireTemporaryMemoryPool(size);
		}

		CMemoryTemporaryAllocationManager* CTaskHandleLimitedReserveMemoryPoolAllocationManager::releaseTemporaryMemoryPool(CMemoryPool* memoryPool) {
			return CLimitedReserveMemoryPoolAllocationManager::releaseTemporaryMemoryPool(memoryPool);
		}


		cint64 CTaskHandleLimitedReserveMemoryPoolAllocationManager::getOptimizedMoreAllocationSize(cint64 size) {
			return CLimitedReserveMemoryPoolAllocationManager::getOptimizedMoreAllocationSize(size);
		}


		CMemoryTemporaryAllocationManager* CTaskHandleLimitedReserveMemoryPoolAllocationManager::releaseMemoryPoolContainer(CMemoryPoolContainer* memoryPoolContainer) {
			return CLimitedReserveMemoryPoolAllocationManager::releaseMemoryPoolContainer(memoryPoolContainer);
		}


		CMemoryPoolProvider* CTaskHandleLimitedReserveMemoryPoolAllocationManager::releaseMemoryPool(CMemoryPool* memoryPool) {
			return CLimitedReserveMemoryPoolAllocationManager::releaseMemoryPool(memoryPool);
		}


		CMemoryPool* CTaskHandleLimitedReserveMemoryPoolAllocationManager::acquireMemoryPool(cint64 minPoolSize) {
			return CLimitedReserveMemoryPoolAllocationManager::acquireMemoryPool(minPoolSize);
		}

		CMemoryPool* CTaskHandleLimitedReserveMemoryPoolAllocationManager::acquireMemoryPoolConsiderated(cint64 minPoolSize) {
			CMemoryPool* tmpMemoryPool = nullptr;
			if (mFreeMemoryPoolList) {
				if (mFreeMemoryPoolList->getMemoryBlockSize() >= minPoolSize) {
					tmpMemoryPool = mFreeMemoryPoolList;
					mFreeMemoryPoolList = mFreeMemoryPoolList->getNextMemoryPool();
					--mFreeMemoryPoolCount;
				}
			}
			if (!tmpMemoryPool) {
				tmpMemoryPool = mConsideratedMemPoolProvider->acquireMemoryPoolConsiderated(minPoolSize);
			}
			if (tmpMemoryPool) {
				++mStatMemToUsingCount;
				tmpMemoryPool->clearNext();
			}
			return tmpMemoryPool;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
