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

#include "CNewAllocationMemoryPoolProvider.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CNewAllocationMemoryPoolProvider::CNewAllocationMemoryPoolProvider(cint64 defaultPoolSize) {
				mDefaultPoolSize = defaultPoolSize;
				mStatAllocatedPoolSize = 0;
				mStatAllocatedPoolCount = 0;
				mStatReleasedPoolSize = 0;
				mStatReleasedPoolCount = 0;
				mStatDiffPoolSize = 0;
				mStatDiffPoolCount = 0;
			}

			CNewAllocationMemoryPoolProvider::~CNewAllocationMemoryPoolProvider() {
			}


			CMemoryPool* CNewAllocationMemoryPoolProvider::acquireMemoryPoolConsiderated(cint64 minPoolSize) {
				return acquireMemoryPool(minPoolSize);
			}


			CMemoryPool* CNewAllocationMemoryPoolProvider::acquireMemoryPool(cint64 minPoolSize) {
				CMemoryPool* memoryPool = new CMemoryPool();
				if (!memoryPool) {
					// not enough memory even for the log message
					//LOG(CATASTROPHIC,"::Konclude::Utilities::NewAllocationMemoryPoolProvider",logTr("Memory allocation failed."),this);
				}
				cint64 memoryBlockSize = qMax(mDefaultPoolSize,minPoolSize);
				char* memoryBlock = new char[memoryBlockSize];
				if (!memoryBlock) {
					LOG(CATASTROPHIC,"::Konclude::Utilities::NewAllocationMemoryPoolProvider",logTr("Memory allocation failed."),this);
				}
				mStatAllocatedPoolSize += memoryBlockSize;
				mStatDiffPoolSize += memoryBlockSize;
				++mStatAllocatedPoolCount;
				++mStatDiffPoolCount;
				memoryPool->setMemoryBlockData(memoryBlock,memoryBlockSize);
				return memoryPool;
			}


			CMemoryPoolProvider* CNewAllocationMemoryPoolProvider::releaseMemoryPool(CMemoryPool* memoryPool) {
				while (memoryPool) {
					CMemoryPool* tmpMemoryPool = memoryPool;
					memoryPool = memoryPool->getNext();
					mStatReleasedPoolSize += tmpMemoryPool->getMemoryBlockSize();
					mStatDiffPoolSize -= tmpMemoryPool->getMemoryBlockSize();
					++mStatReleasedPoolCount;
					char* memoryBlock = tmpMemoryPool->getMemoryBlockData();
					delete [] memoryBlock;
					delete tmpMemoryPool;
				}
				return this;
			}


			cint64 CNewAllocationMemoryPoolProvider::getAllocatedReleaseDifferencePoolCount() {
				return mStatDiffPoolCount;
			}

			cint64 CNewAllocationMemoryPoolProvider::getAllocatedReleaseDifferencePoolSize() {
				return mStatDiffPoolSize;
			}




		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

