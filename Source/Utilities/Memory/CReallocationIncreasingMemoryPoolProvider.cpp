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

#include "CReallocationIncreasingMemoryPoolProvider.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CReallocationIncreasingMemoryPoolProvider::CReallocationIncreasingMemoryPoolProvider(CMemoryAllocationManager* reallocatingMemMan, cint64 defaultPoolSize, cint64 maxPoolSize, double increasingFactor) : CConsiderateMemoryPoolProvider() {
				mReallocatingMemMan = reallocatingMemMan;
				mDefaultPoolSize = defaultPoolSize;
				mStatAllocatedPoolSize = 0;
				mStatAllocatedPoolCount = 0;
				mStatReleasedPoolSize = 0;
				mStatReleasedPoolCount = 0;
				mStatDiffPoolSize = 0;
				mStatDiffPoolCount = 0;
				mMaxPoolSize = maxPoolSize;
				mIncreasingFactor = increasingFactor;
			}

			CReallocationIncreasingMemoryPoolProvider::~CReallocationIncreasingMemoryPoolProvider() {
			}



			CMemoryPool* CReallocationIncreasingMemoryPoolProvider::acquireMemoryPoolConsiderated(cint64 minPoolSize) {
				return acquireMemoryPool(minPoolSize);
			}


			CMemoryPool* CReallocationIncreasingMemoryPoolProvider::acquireMemoryPool(cint64 minPoolSize) {
				mDefaultPoolSize *= mIncreasingFactor;
				mDefaultPoolSize = qMin(mDefaultPoolSize, mMaxPoolSize);
				CMemoryPool* memoryPool = CObjectAllocator<CMemoryPool>::allocateAndConstruct(mReallocatingMemMan);
				if (!memoryPool) {
					// not enough memory even for the log message
					//LOG(CATASTROPHIC,"::Konclude::Utilities::NewAllocationMemoryPoolProvider",logTr("Memory allocation failed."),this);
				}
				cint64 memoryBlockSize = qMax(mDefaultPoolSize, minPoolSize);
				char* memoryBlock = CObjectAllocator<char>::allocateAndConstructArray(mReallocatingMemMan, memoryBlockSize);
				if (!memoryBlock) {
					LOG(CATASTROPHIC, "::Konclude::Utilities::NewAllocationMemoryPoolProvider", logTr("Memory allocation failed."), this);
				}
				mStatAllocatedPoolSize += memoryBlockSize;
				mStatDiffPoolSize += memoryBlockSize;
				++mStatAllocatedPoolCount;
				++mStatDiffPoolCount;
				memoryPool->setMemoryBlockData(memoryBlock, memoryBlockSize);
				return memoryPool;
			}


			CMemoryPoolProvider* CReallocationIncreasingMemoryPoolProvider::releaseMemoryPool(CMemoryPool* memoryPool) {
				return this;
			}


			cint64 CReallocationIncreasingMemoryPoolProvider::getAllocatedReleaseDifferencePoolCount() {
				return mStatDiffPoolCount;
			}

			cint64 CReallocationIncreasingMemoryPoolProvider::getAllocatedReleaseDifferencePoolSize() {
				return mStatDiffPoolSize;
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

