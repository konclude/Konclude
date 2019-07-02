/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CNewCentralizedLimitedAllocationMemoryPoolProvider.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CNewCentralizedLimitedAllocationMemoryPoolProvider::CNewCentralizedLimitedAllocationMemoryPoolProvider(CAllocationLimitator* allocLimitator, cint64 defaultPoolSize) {
				mDefaultPoolSize = defaultPoolSize;
				mStatAllocatedPoolSize = 0;
				mStatAllocatedPoolCount = 0;
				mStatReleasedPoolSize = 0;
				mStatReleasedPoolCount = 0;
				mStatDiffPoolSize = 0;
				mStatDiffPoolCount = 0;
				mAllocLimitator = allocLimitator;
			}

			CNewCentralizedLimitedAllocationMemoryPoolProvider::~CNewCentralizedLimitedAllocationMemoryPoolProvider() {
			}


			CMemoryPool* CNewCentralizedLimitedAllocationMemoryPoolProvider::acquireMemoryPool(cint64 minPoolSize) {
				cint64 memoryBlockSize = qMax(mDefaultPoolSize,minPoolSize);
				mAllocLimitator->addAllocatingMemorySize(memoryBlockSize+sizeof(CMemoryPool));
				CMemoryPool* memoryPool = new CMemoryPool();
				char* memoryBlock = new char[memoryBlockSize];
				mStatAllocatedPoolSize += memoryBlockSize;
				mStatDiffPoolSize += memoryBlockSize;
				++mStatAllocatedPoolCount;
				++mStatDiffPoolCount;
				memoryPool->setMemoryBlockData(memoryBlock,memoryBlockSize);
				return memoryPool;
			}


			CMemoryPool* CNewCentralizedLimitedAllocationMemoryPoolProvider::acquireMemoryPoolConsiderated(cint64 minPoolSize) {
				cint64 memoryBlockSize = qMax(mDefaultPoolSize,minPoolSize);
				if (mAllocLimitator->addAllocatingMemorySize(memoryBlockSize+sizeof(CMemoryPool))) {
					CMemoryPool* memoryPool = new CMemoryPool();
					char* memoryBlock = new char[memoryBlockSize];
					mStatAllocatedPoolSize += memoryBlockSize;
					mStatDiffPoolSize += memoryBlockSize;
					++mStatAllocatedPoolCount;
					++mStatDiffPoolCount;
					memoryPool->setMemoryBlockData(memoryBlock,memoryBlockSize);
					return memoryPool;
				} else {
					throw CMemoryAllocationException();
				}
				return nullptr;			
			}



			CMemoryPoolProvider* CNewCentralizedLimitedAllocationMemoryPoolProvider::releaseMemoryPool(CMemoryPool* memoryPool) {
				while (memoryPool) {
					CMemoryPool* tmpMemoryPool = memoryPool;
					memoryPool = memoryPool->getNext();
					mStatReleasedPoolSize += tmpMemoryPool->getMemoryBlockSize();
					mStatDiffPoolSize -= tmpMemoryPool->getMemoryBlockSize();
					mAllocLimitator->addReleasedMemorySize(tmpMemoryPool->getMemoryBlockSize()+sizeof(CMemoryPool));
					++mStatReleasedPoolCount;
					--mStatReleasedPoolCount;
					char* memoryBlock = tmpMemoryPool->getMemoryBlockData();
					delete [] memoryBlock;
					delete tmpMemoryPool;
				}
				return this;
			}


			cint64 CNewCentralizedLimitedAllocationMemoryPoolProvider::getAllocatedReleaseDifferencePoolCount() {
				return mStatDiffPoolCount;
			}

			cint64 CNewCentralizedLimitedAllocationMemoryPoolProvider::getAllocatedReleaseDifferencePoolSize() {
				return mStatDiffPoolSize;
			}




		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

