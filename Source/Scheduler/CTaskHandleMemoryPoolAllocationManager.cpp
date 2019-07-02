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

#include "CTaskHandleMemoryPoolAllocationManager.h"


namespace Konclude {

	namespace Scheduler {



		CTaskHandleMemoryPoolAllocationManager::CTaskHandleMemoryPoolAllocationManager(CConsiderateMemoryPoolProvider* memoryPoolProvider) : CMemoryPoolAllocationManager(memoryPoolProvider) {
			mConsideratedMemPoolProvider = memoryPoolProvider;
		}

		CTaskHandleMemoryPoolAllocationManager::~CTaskHandleMemoryPoolAllocationManager() {
		}

		void* CTaskHandleMemoryPoolAllocationManager::allocate(size_t size) {
			return CMemoryPoolAllocationManager::allocate(size);
		}

		CMemoryAllocationManager* CTaskHandleMemoryPoolAllocationManager::release(void* address) {
			return CMemoryPoolAllocationManager::release(address);
		}


		void* CTaskHandleMemoryPoolAllocationManager::allocateAligned(size_t size, int alignment) {
			return CMemoryPoolAllocationManager::allocateAligned(size,alignment);
		}


		CMemoryTemporaryAllocationManager* CTaskHandleMemoryPoolAllocationManager::releaseAllMemory() {
			return CMemoryPoolAllocationManager::releaseAllMemory();
		}


		CMemoryPool* CTaskHandleMemoryPoolAllocationManager::acqireTemporaryMemoryPool(size_t size) {
			return CMemoryPoolAllocationManager::acqireTemporaryMemoryPool(size);
		}

		CMemoryTemporaryAllocationManager* CTaskHandleMemoryPoolAllocationManager::releaseTemporaryMemoryPool(CMemoryPool* memoryPool) {
			return CMemoryPoolAllocationManager::releaseTemporaryMemoryPool(memoryPool);
		}


		cint64 CTaskHandleMemoryPoolAllocationManager::getOptimizedMoreAllocationSize(cint64 size) {
			return CMemoryPoolAllocationManager::getOptimizedMoreAllocationSize(size);
		}


		CMemoryTemporaryAllocationManager* CTaskHandleMemoryPoolAllocationManager::releaseMemoryPoolContainer(CMemoryPoolContainer* memoryPoolContainer) {
			return CMemoryPoolAllocationManager::releaseMemoryPoolContainer(memoryPoolContainer);
		}

		CMemoryPoolProvider* CTaskHandleMemoryPoolAllocationManager::releaseMemoryPool(CMemoryPool* memoryPool) {
			return CMemoryPoolAllocationManager::releaseMemoryPool(memoryPool);
		}


		CMemoryPool* CTaskHandleMemoryPoolAllocationManager::acquireMemoryPool(cint64 minPoolSize) {
			return CMemoryPoolAllocationManager::acquireMemoryPool(minPoolSize);
		}

		CMemoryPool* CTaskHandleMemoryPoolAllocationManager::acquireMemoryPoolConsiderated(cint64 minPoolSize) {
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
