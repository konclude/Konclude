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

#include "CMemoryPool.h"

#if !defined(_WIN32)
#include <sys/mman.h>
#include <unistd.h>
#endif


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CMemoryPool::CMemoryPool() : CLinkerBase<CMemoryPool*,CMemoryPool>(this,nullptr) {
				mMemoryBlockBegin = 0;
				mMemoryBlockSize = 0;
				mMemoryBlockPointer = 0;
				mMemoryBlockMapped = false;
			}

			CMemoryPool::~CMemoryPool() {
			}

			char* CMemoryPool::getMemoryBlockData() {
				return mMemoryBlockBegin;
			}

			cint64 CMemoryPool::getMemoryBlockSize() {
				return mMemoryBlockSize;
			}

			char* CMemoryPool::getMemoryBlockPointer() {
				return mMemoryBlockPointer;
			}

			char* CMemoryPool::getMemoryBlockEnd() {
				return mMemoryBlockEnd;
			}

			CMemoryPool* CMemoryPool::setMemoryBlockData(char* memoryBlock, cint64 memoryBlockSize, bool memoryBlockMapped) {
				mMemoryBlockBegin = memoryBlock;
				mMemoryBlockSize = memoryBlockSize;
				mMemoryBlockEnd = mMemoryBlockBegin + mMemoryBlockSize;
				mMemoryBlockPointer = mMemoryBlockBegin;
				mMemoryBlockMapped = memoryBlockMapped;
				return this;
			}

			bool CMemoryPool::isMemoryBlockMapped() {
				return mMemoryBlockMapped;
			}

#if !defined(_WIN32)
			static cint64 roundedMappingSize(cint64 memoryBlockSize) {
				static const cint64 pageSize = (cint64)sysconf(_SC_PAGESIZE);
				return ((memoryBlockSize + pageSize - 1) / pageSize) * pageSize;
			}
#endif

			cint64 CMemoryPool::mappedBlockSize(cint64 memoryBlockSize) {
#if !defined(_WIN32)
				return roundedMappingSize(memoryBlockSize);
#else
				return memoryBlockSize;
#endif
			}

			char* CMemoryPool::allocateMemoryBlock(cint64 memoryBlockSize, bool& memoryBlockMapped) {
#if !defined(_WIN32)
				void* mapping = mmap(nullptr, (size_t)roundedMappingSize(memoryBlockSize), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
				if (mapping != MAP_FAILED) {
					memoryBlockMapped = true;
					return (char*)mapping;
				}
#endif
				memoryBlockMapped = false;
				return new char[memoryBlockSize];
			}

			void CMemoryPool::releaseMemoryBlock(char* memoryBlock, cint64 memoryBlockSize, bool memoryBlockMapped) {
				if (!memoryBlock) {
					return;
				}
#if !defined(_WIN32)
				if (memoryBlockMapped) {
					munmap(memoryBlock, (size_t)roundedMappingSize(memoryBlockSize));
					return;
				}
#endif
				delete [] memoryBlock;
			}

			void CMemoryPool::releaseMemoryBlockData(CMemoryPool* memoryPool) {
				releaseMemoryBlock(memoryPool->getMemoryBlockData(), memoryPool->getMemoryBlockSize(), memoryPool->isMemoryBlockMapped());
			}

			CMemoryPool* CMemoryPool::setMemoryBlockPointer(char* memoryBlockPointer) {
				mMemoryBlockPointer = memoryBlockPointer;
				return this;
			}

			CMemoryPool* CMemoryPool::incMemoryBlockPointer(cint64 pointerInc) {
				mMemoryBlockPointer = (char*)((cint64)mMemoryBlockPointer + pointerInc);
				return this;
			}


			CMemoryPool* CMemoryPool::resetMemoryBlockPointer() {
				mMemoryBlockPointer = mMemoryBlockBegin;
				return this;
			}

			CMemoryPool* CMemoryPool::getNextMemoryPool() {
				return getNext();
			}

			CMemoryPool* CMemoryPool::setNextMemoryPool(CMemoryPool* nextMemoryPool) {
				CLinkerBase<CMemoryPool*,CMemoryPool>::setNext(nextMemoryPool);
				return this;
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

