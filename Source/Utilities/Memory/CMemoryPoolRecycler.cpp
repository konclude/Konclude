/*
 *		Copyright (C) 2013-2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CMemoryPoolRecycler.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CMemoryPoolRecycler* CMemoryPoolRecycler::getInstance() {
				static CMemoryPoolRecycler instance;
				return &instance;
			}


			CMemoryPoolRecycler::CMemoryPoolRecycler() {
				mPoolList = nullptr;
				mStoredCount = 0;
				mLimit = DEFAULT_MEMORY_POOL_RECYCLE_LIMIT;
			}


			CMemoryPoolRecycler::~CMemoryPoolRecycler() {
				releaseStoredPools();
			}

			void CMemoryPoolRecycler::releaseStoredPools() {
				QMutexLocker locker(&mMutex);
				while (mPoolList) {
					CMemoryPool* memoryPool = mPoolList;
					mPoolList = memoryPool->getNextMemoryPool();
					CMemoryPool::releaseMemoryBlockData(memoryPool);
					delete memoryPool;
				}
				mStoredCount = 0;
			}


			CMemoryPool* CMemoryPoolRecycler::takeMemoryPool(cint64 minPoolSize) {
				if (minPoolSize > DEFAULTMEMORYPOOLSIZE) {
					return nullptr;
				}
				QMutexLocker locker(&mMutex);
				CMemoryPool* memoryPool = mPoolList;
				if (memoryPool) {
					mPoolList = memoryPool->getNextMemoryPool();
					--mStoredCount;
					memoryPool->clearNext();
				}
				return memoryPool;
			}


			bool CMemoryPoolRecycler::giveMemoryPool(CMemoryPool* memoryPool) {
				// the providers allocate the default size, one of them with eight bytes on top, rounded up to whole pages
				cint64 size = memoryPool->getMemoryBlockSize();
				if (size < DEFAULTMEMORYPOOLSIZE || size > CMemoryPool::mappedBlockSize(DEFAULTMEMORYPOOLSIZE + 8)) {
					return false;
				}
				QMutexLocker locker(&mMutex);
				if (mStoredCount >= mLimit) {
					return false;
				}
				memoryPool->resetMemoryBlockPointer();
				memoryPool->setNextMemoryPool(mPoolList);
				mPoolList = memoryPool;
				++mStoredCount;
				return true;
			}


			cint64 CMemoryPoolRecycler::getLimit() {
				return mLimit;
			}


			void CMemoryPoolRecycler::setLimit(cint64 limit) {
				QMutexLocker locker(&mMutex);
				mLimit = limit;
			}


			cint64 CMemoryPoolRecycler::getStoredCount() {
				QMutexLocker locker(&mMutex);
				return mStoredCount;
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude
