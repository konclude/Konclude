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

#include "CMemoryPoolContainerAllocationManager.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {



			CMemoryPoolContainerAllocationManager::CMemoryPoolContainerAllocationManager(CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider) {
				mMemoryPoolContainer = memoryPoolContainer;
				mMemoryPoolProvider = memoryPoolProvider;
				mStatPoolAllocCount = 0;
				mStatAllocPoolSize = 0;
			}

			CMemoryPoolContainerAllocationManager::~CMemoryPoolContainerAllocationManager() {
			}


			void* CMemoryPoolContainerAllocationManager::allocate(size_t size) {
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

			CMemoryAllocationManager* CMemoryPoolContainerAllocationManager::release(void* address) {
				// not supported by this pool memory manager, stored all memory in memory pool container
				return this;
			}

			void* CMemoryPoolContainerAllocationManager::allocateAligned(size_t size, int alignment) {
				void* memPtr = 0;
				char* memBlock = getMemoryBlock(size*sizeof(char),alignment);
				memPtr = memBlock;
				return memPtr;
			}

			char* CMemoryPoolContainerAllocationManager::getMemoryBlock(cint64 memoryBlockSize) {
#ifdef KONCLUDE_DIRECT_NEW_MEMORY_ALLOCATION
				return new char[memoryBlockSize];
#endif // KONCLUDE_DIRECT_NEW_MEMORY_ALLOCATION
				char* memBlock = 0;
				bool allocated = false;
				CMemoryPool* memoryPool = mMemoryPoolContainer->getMemoryPools();
				while (!allocated) {
					if (!memoryPool) {
						memoryPool = mMemoryPoolProvider->acquireMemoryPool(memoryBlockSize);
						++mStatPoolAllocCount;
						if (memoryPool) {
							mStatAllocPoolSize += memoryPool->getMemoryBlockSize();
							mMemoryPoolContainer->appendMemoryPool(memoryPool);
						}
					}
					if (memoryPool) {
						char* memBlockEnd = memoryPool->getMemoryBlockEnd();
						char* memBlockPointer = memoryPool->getMemoryBlockPointer();
						if (memBlockPointer+memoryBlockSize < memBlockEnd) {
							memBlock = memBlockPointer;
							memoryPool->setMemoryBlockPointer(memBlockPointer+memoryBlockSize);
							allocated = true;
						} else {
							memoryPool = nullptr;
						}
					}
				}
				return memBlock;
			}


			cint64 CMemoryPoolContainerAllocationManager::getOptimizedMoreAllocationSize(cint64 size) {
				CMemoryPool* memoryPool = mMemoryPoolContainer->getMemoryPools();
				if (memoryPool) {
					cint64 availableSizeThisPool = memoryPool->getMemoryBlockEnd() - memoryPool->getMemoryBlockData();
					if (availableSizeThisPool >= size) {
						return qMax(availableSizeThisPool,size<<2);
					} else if (memoryPool->getMemoryBlockSize() >= size) {
						return qMax(memoryPool->getMemoryBlockSize(),size<<2);
					}
				}
				return size;
			}



			char* CMemoryPoolContainerAllocationManager::getMemoryBlock(cint64 memoryBlockSize, cint64 alignment) {
#ifdef KONCLUDE_DIRECT_NEW_MEMORY_ALLOCATION
				char* data = new char[memoryBlockSize + alignment];
				cint64 alignmentMask = alignment - 1;
				if (alignmentMask & cint64(data)) {
					data = (char*)((cint64(data) & ~alignmentMask) + alignment);
				}
				return data;
#endif // KONCLUDE_DIRECT_NEW_MEMORY_ALLOCATION
				char* memBlock = 0;
				bool allocated = false;
				CMemoryPool* memoryPool = mMemoryPoolContainer->getMemoryPools();
				while (!allocated) {
					if (!memoryPool) {
						memoryPool = mMemoryPoolProvider->acquireMemoryPool(memoryBlockSize+alignment);
						++mStatPoolAllocCount;
						if (memoryPool) {
							mStatAllocPoolSize += memoryPool->getMemoryBlockSize();
							mMemoryPoolContainer->appendMemoryPool(memoryPool);
						}
					}
					if (memoryPool) {
						char* memBlockEnd = memoryPool->getMemoryBlockEnd();
						char* memBlockPointer = memoryPool->getMemoryBlockPointer();
						cint64 maxBlockAlloc = (cint64)(memBlockPointer)+(cint64)(memoryBlockSize+alignment);
						if (maxBlockAlloc < (cint64)memBlockEnd) {
							// alignment is 2^N
							cint64 alignmentMask = alignment-1;
							memBlock = memBlockPointer;
							if (alignmentMask & cint64(memBlock)) {
								memBlock = (char*)((cint64(memBlock) & ~alignmentMask) + alignment);
							}
							memoryPool->setMemoryBlockPointer(memBlock+memoryBlockSize);
							allocated = true;
						} else {
							memoryPool = nullptr;
						}
					}
				}
				return memBlock;
			}


			void* CMemoryPoolContainerAllocationManager::allocateMemoryToContainer(size_t size, CMemoryPoolContainer* memoryPoolContainer, CMemoryPoolProvider* memoryPoolProvider) {
				cint64 memoryBlockSize = size;
				char* memBlock = 0;
				bool allocated = false;
				CMemoryPool* memoryPool = memoryPoolContainer->getMemoryPools();
				while (!allocated) {
					if (!memoryPool) {
						memoryPool = memoryPoolProvider->acquireMemoryPool(memoryBlockSize);
						memoryPoolContainer->appendMemoryPool(memoryPool);
					}
					if (memoryPool) {
						char* memBlockEnd = memoryPool->getMemoryBlockEnd();
						char* memBlockPointer = memoryPool->getMemoryBlockPointer();
						if (memBlockPointer+memoryBlockSize < memBlockEnd) {
							memBlock = memBlockPointer;
							memoryPool->setMemoryBlockPointer(memBlockPointer+memoryBlockSize);
							allocated = true;
						} else {
							memoryPool = nullptr;
						}
					}
				}
				return memBlock;
			}

		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude
