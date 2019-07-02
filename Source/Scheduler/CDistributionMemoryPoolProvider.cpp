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

#include "CDistributionMemoryPoolProvider.h"


namespace Konclude {

	namespace Scheduler {



		CDistributionMemoryPoolProvider::CDistributionMemoryPoolProvider(CMemoryPoolProvider* alternateMemoryPoolProvider, CDistributionMemoryPoolReleaser* distributionMemoryReleaser) : CSortedLinkerBase<CDistributionMemoryPoolProvider*,CDistributionMemoryPoolProvider,CDistributionMemoryPoolProvider>(this,nullptr) {
			mLastPickedDistributedPoolLinker = nullptr;
			mDistributedPoolLinker = nullptr;
			mAvailablePoolLinker = nullptr;
			mAlternateMemoryPoolProvider = alternateMemoryPoolProvider;
			mDistributionMemoryReleaser = distributionMemoryReleaser;

			mDistributedPoolCount = 0;
			mPickedPoolCount = 0;
			mAvailablePoolCount = 0;
			mAquiredPoolCount = 0;

			mLastAcquiredPoolCount = 0;
			mLastAcquiredPoolDiff = 0;

			mLastSavedMissRatio = 0.;

			mLastNewDiffRatio = 0.6;
		}

		CDistributionMemoryPoolProvider::~CDistributionMemoryPoolProvider() {
		}


		CDistributionMemoryPoolProvider* CDistributionMemoryPoolProvider::updateMissingMemoryPoolRatio() {
			mLastSavedMissRatio = (double)getMemoryPoolSupplyCount()/(double)mLastAcquiredPoolCount;
			return this;
		}

		double CDistributionMemoryPoolProvider::getMissingMemoryPoolRatio() {
			return mLastSavedMissRatio;
		}

		cint64 CDistributionMemoryPoolProvider::getMissingMemoryPoolCount() {
			return mLastAcquiredPoolDiff - getMemoryPoolSupplyCount();
		}

		cint64 CDistributionMemoryPoolProvider::getMemoryPoolSupplyCount() {
			return mDistributedPoolCount-mPickedPoolCount+mAvailablePoolCount;
		}

		CDistributionMemoryPoolProvider* CDistributionMemoryPoolProvider::updatedRoundAcquiredMemoryPoolCounts() {
			cint64 roundAcquiredPoolDiff = mAquiredPoolCount - mLastAcquiredPoolCount;
			mLastAcquiredPoolCount = mAquiredPoolCount;
			mLastAcquiredPoolDiff = roundAcquiredPoolDiff * (1. - mLastNewDiffRatio) + mLastAcquiredPoolDiff * mLastNewDiffRatio;
			updateMissingMemoryPoolRatio();
			return this;
		}


		CDistributionMemoryPoolProvider* CDistributionMemoryPoolProvider::appendDistributedMemoryPools(CMemoryPool* memoryPools) {
			while (memoryPools) {
				CMemoryPool* tmpMemoryPool = memoryPools;
				memoryPools = memoryPools->getNext();

				if (mDistributedPoolLinker != tmpMemoryPool) {
					mDistributedPoolLinker = tmpMemoryPool->setNext(mDistributedPoolLinker);
				} else {
					// write access to mLastPickedDistributedPoolLinker is safe only in this case
					mDistributedPoolLinker = tmpMemoryPool->clearNext();
					mLastPickedDistributedPoolLinker = nullptr;
				}
				++mDistributedPoolCount;
			}
			return this;
		}


		bool CDistributionMemoryPoolProvider::checkAppendableDistributedMemoryPools(CMemoryPool* memoryPools) {
			CMemoryPool* memoryPoolIt = memoryPools;
			while (memoryPoolIt) {
				if (memoryPoolIt == mLastPickedDistributedPoolLinker) {
					return false;
				}
				memoryPoolIt = memoryPoolIt->getNextMemoryPool();
			}
			return true;
		}



		bool CDistributionMemoryPoolProvider::pickDistributedMemoryPools() {
			CMemoryPool* tmpMemoryPoolLinker = mDistributedPoolLinker;
			if (tmpMemoryPoolLinker && mLastPickedDistributedPoolLinker != tmpMemoryPoolLinker) {
				CMemoryPool* nextTmpMemoryPoolLinker = tmpMemoryPoolLinker;
				while (nextTmpMemoryPoolLinker && mLastPickedDistributedPoolLinker != nextTmpMemoryPoolLinker) {
					CMemoryPool* addTmpMemoryPoolLinker = nextTmpMemoryPoolLinker;
					nextTmpMemoryPoolLinker = nextTmpMemoryPoolLinker->getNext();
					mAvailablePoolLinker = addTmpMemoryPoolLinker->setNext(mAvailablePoolLinker);
					++mPickedPoolCount;
					++mAvailablePoolCount;
				}
				mLastPickedDistributedPoolLinker = tmpMemoryPoolLinker;
				return true;
			}
			return false;
		}


		CMemoryPool* CDistributionMemoryPoolProvider::acquireMemoryPool(cint64 minPoolSize) {
			CMemoryPool* tmpMemoryPool = nullptr;
			++mAquiredPoolCount;
			pickDistributedMemoryPools();
			if (mAvailablePoolLinker) {
				if (mAvailablePoolLinker->getMemoryBlockSize() > minPoolSize) {
					tmpMemoryPool = mAvailablePoolLinker;
					mAvailablePoolLinker = mAvailablePoolLinker->getNext();
					--mAvailablePoolCount;
				}
			}
			if (!tmpMemoryPool) {
				tmpMemoryPool = mAlternateMemoryPoolProvider->acquireMemoryPool(minPoolSize);
			}
			if (tmpMemoryPool) {
				tmpMemoryPool->clearNext();
			}
			return tmpMemoryPool;
		}

		CDistributionMemoryPoolProvider* CDistributionMemoryPoolProvider::releaseMemoryPool(CMemoryPool* memoryPool) {
			if (mDistributionMemoryReleaser) {
				mDistributionMemoryReleaser->distributeMemoryPools(memoryPool);
			} else {
				mAlternateMemoryPoolProvider->releaseMemoryPool(memoryPool);
			}
			return this;
		}


		cint64 CDistributionMemoryPoolProvider::getAllocatedReleaseDifferencePoolCount() {
			return 0;
		}

		cint64 CDistributionMemoryPoolProvider::getAllocatedReleaseDifferencePoolSize() {
			return 0;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude

