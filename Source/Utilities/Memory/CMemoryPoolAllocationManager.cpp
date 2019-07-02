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

#include "CMemoryPoolAllocationManager.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CMemoryPoolAllocationManager::CMemoryPoolAllocationManager(CMemoryPoolProvider* memoryPoolProvider) {
				mMemoryPoolProvider = memoryPoolProvider;
				mFreeMemoryPoolList = nullptr;
				mUsedMemoryPoolList = nullptr;
				mUsingMemoryPool = nullptr;
				mFreeMemoryPoolCount = 0;
				mStatMemToUsingCount = 0;
				mStatMemToFreeCount = 0;
			}

			CMemoryPoolAllocationManager::~CMemoryPoolAllocationManager() {
				// release all memory blocks
				while (mFreeMemoryPoolList) {
					CMemoryPool* tmpMemPool = mFreeMemoryPoolList;
					mFreeMemoryPoolList = mFreeMemoryPoolList->getNextMemoryPool();
					tmpMemPool->clearNext();
					mMemoryPoolProvider->releaseMemoryPool(tmpMemPool);
				}
				while (mUsedMemoryPoolList) {
					CMemoryPool* tmpMemPool = mUsedMemoryPoolList;
					mUsedMemoryPoolList = mUsedMemoryPoolList->getNextMemoryPool();
					tmpMemPool->clearNext();
					mMemoryPoolProvider->releaseMemoryPool(tmpMemPool);
				}
				if (mUsingMemoryPool) {
					mMemoryPoolProvider->releaseMemoryPool(mUsingMemoryPool);
				}
				mFreeMemoryPoolCount = 0;
			}

			void* CMemoryPoolAllocationManager::allocate(size_t size) {
				void* memPtr = 0;
				char* memBlock = 0;
				if (size >= 8) {
					memBlock = getMemoryBlock(size*sizeof(char),8);
				} else if (size >= 4) {
					memBlock = getMemoryBlock(size*sizeof(char),4);
				} else if (size >= 2) {
					memBlock = getMemoryBlock(size*sizeof(char),2);
				} else {
					memBlock = getMemoryBlock(size*sizeof(char));
				}
				memPtr = memBlock;
				return memPtr;
			}

			CMemoryAllocationManager* CMemoryPoolAllocationManager::release(void* address) {
				// not supported by this pool memory manager, use releaseAllMemory()
				return this;
			}

			void* CMemoryPoolAllocationManager::allocateAligned(size_t size, int alignment) {
				void* memPtr = 0;
				char* memBlock = getMemoryBlock(size*sizeof(char),alignment);
				memPtr = memBlock;
				return memPtr;
			}

			char* CMemoryPoolAllocationManager::getMemoryBlock(cint64 memoryBlockSize) {
				char* memBlock = 0;
				bool allocated = false;
				while (!allocated) {
					if (!mUsingMemoryPool) {
						if (!mFreeMemoryPoolList) {
							mFreeMemoryPoolList = mMemoryPoolProvider->acquireMemoryPool(memoryBlockSize);
							++mFreeMemoryPoolCount;
						}
						mUsingMemoryPool = mFreeMemoryPoolList;
						mFreeMemoryPoolList = mFreeMemoryPoolList->getNextMemoryPool();
						mUsingMemoryPool->clearNext();
						--mFreeMemoryPoolCount;
						++mStatMemToUsingCount;
					}
					if (mUsingMemoryPool) {
						char* memBlockEnd = mUsingMemoryPool->getMemoryBlockEnd();
						char* memBlockPointer = mUsingMemoryPool->getMemoryBlockPointer();
						if (memBlockPointer+memoryBlockSize <= memBlockEnd) {
							memBlock = memBlockPointer;
							mUsingMemoryPool->setMemoryBlockPointer(memBlockPointer+memoryBlockSize);
							allocated = true;
						} else {
							// move pool to used
							CMemoryPool* tmpMemPool = mUsingMemoryPool;
							mUsedMemoryPoolList = tmpMemPool->setNextMemoryPool(mUsedMemoryPoolList);
							mUsingMemoryPool = nullptr;
						}
					}
				}
				return memBlock;
			}


			char* CMemoryPoolAllocationManager::getMemoryBlock(cint64 memoryBlockSize, cint64 alignment) {
				char* memBlock = 0;
				bool allocated = false;
				while (!allocated) {
					if (!mUsingMemoryPool) {
						if (!mFreeMemoryPoolList) {
							mFreeMemoryPoolList = mMemoryPoolProvider->acquireMemoryPool(memoryBlockSize+alignment);
							++mFreeMemoryPoolCount;
						}
						mUsingMemoryPool = mFreeMemoryPoolList;
						mFreeMemoryPoolList = mFreeMemoryPoolList->getNextMemoryPool();
						mUsingMemoryPool->clearNext();
						--mFreeMemoryPoolCount;
						++mStatMemToUsingCount;
					}
					if (mUsingMemoryPool) {
						char* memBlockEnd = mUsingMemoryPool->getMemoryBlockEnd();
						char* memBlockPointer = mUsingMemoryPool->getMemoryBlockPointer();
						cint64 maxBlockAlloc = (cint64)memBlockPointer+(cint64)memoryBlockSize+alignment;
						if (maxBlockAlloc <= (cint64)memBlockEnd) {
							// alignment is 2^N
							cint64 alignmentMask = alignment-1;
							memBlock = memBlockPointer;
							if (alignmentMask & cint64(memBlock)) {
								memBlock = (char*)((cint64(memBlock) & ~alignmentMask) + alignment);
							}
							mUsingMemoryPool->setMemoryBlockPointer(memBlock+memoryBlockSize);
							allocated = true;
						} else {
							// move pool to used
							CMemoryPool* tmpMemPool = mUsingMemoryPool;
							mUsedMemoryPoolList = tmpMemPool->setNextMemoryPool(mUsedMemoryPoolList);
							mUsingMemoryPool = nullptr;
						}
					}
				}
				return memBlock;
			}


			CMemoryTemporaryAllocationManager* CMemoryPoolAllocationManager::releaseAllMemory() {
				// move all used memory pools to free list
				CMemoryPool* tmpMemPool = nullptr;
				while (mUsedMemoryPoolList) {
					tmpMemPool = mUsedMemoryPoolList;
					tmpMemPool->resetMemoryBlockPointer();
					mUsedMemoryPoolList = mUsedMemoryPoolList->getNextMemoryPool();
					mFreeMemoryPoolList = tmpMemPool->setNextMemoryPool(mFreeMemoryPoolList);
					++mFreeMemoryPoolCount;
					++mStatMemToFreeCount;
				}
				if (mUsingMemoryPool) {
					mUsingMemoryPool->resetMemoryBlockPointer();
					mFreeMemoryPoolList = mUsingMemoryPool->setNextMemoryPool(mFreeMemoryPoolList);
					++mFreeMemoryPoolCount;
					++mStatMemToFreeCount;
					mUsingMemoryPool = nullptr;
				}
				return this;
			}



			CMemoryPool* CMemoryPoolAllocationManager::acqireTemporaryMemoryPool(size_t size) {
				CMemoryPool* tmpMemoryPool = nullptr;
				if (mFreeMemoryPoolList) {
					if (mFreeMemoryPoolList->getMemoryBlockSize() >= size) {
						tmpMemoryPool = mFreeMemoryPoolList;
						mFreeMemoryPoolList = mFreeMemoryPoolList->getNextMemoryPool();
						--mFreeMemoryPoolCount;
					}
				}
				if (!tmpMemoryPool) {
					tmpMemoryPool = mMemoryPoolProvider->acquireMemoryPool(size);
				}
				if (tmpMemoryPool) {
					++mStatMemToUsingCount;
					tmpMemoryPool->clearNext();
				}
				return tmpMemoryPool;
			}


			CMemoryTemporaryAllocationManager* CMemoryPoolAllocationManager::releaseTemporaryMemoryPool(CMemoryPool* memoryPool) {
				memoryPool->resetMemoryBlockPointer();
				memoryPool->setNextMemoryPool(mFreeMemoryPoolList);
				mFreeMemoryPoolList = memoryPool;
				++mFreeMemoryPoolCount;
				++mStatMemToFreeCount;
				return this;
			}



			cint64 CMemoryPoolAllocationManager::getOptimizedMoreAllocationSize(cint64 size) {
				if (mUsingMemoryPool) {
					cint64 availableSizeThisPool = mUsingMemoryPool->getMemoryBlockEnd() - mUsingMemoryPool->getMemoryBlockData();
					if (availableSizeThisPool >= size) {
						return qMax(availableSizeThisPool,size<<2);
					} else if (mUsingMemoryPool->getMemoryBlockSize() >= size) {
						return qMax(mUsingMemoryPool->getMemoryBlockSize(),size<<2);
					}
				} else if (mFreeMemoryPoolList) {
					cint64 availableSizeThisPool = mFreeMemoryPoolList->getMemoryBlockEnd() - mFreeMemoryPoolList->getMemoryBlockData();
					if (availableSizeThisPool >= size) {
						return qMax(availableSizeThisPool,size<<2);
					} else if (mUsingMemoryPool->getMemoryBlockSize() >= size) {
						return qMax(mUsingMemoryPool->getMemoryBlockSize(),size<<2);
					}
				}
				return size;
			}


			CMemoryTemporaryAllocationManager* CMemoryPoolAllocationManager::releaseMemoryPoolContainer(CMemoryPoolContainer* memoryPoolContainer) {
				CMemoryPool* memoryPoolLinker = memoryPoolContainer->takeMemoryPools();
				return releaseTemporaryMemoryPools(memoryPoolLinker);
			}


			cint64 CMemoryPoolAllocationManager::getFreeMemoryPoolCount() {
				return mFreeMemoryPoolCount;
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

