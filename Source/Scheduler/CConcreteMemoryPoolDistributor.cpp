/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CConcreteMemoryPoolDistributor.h"


namespace Konclude {

	namespace Scheduler {



		CConcreteMemoryPoolDistributor::CConcreteMemoryPoolDistributor(CMemoryPoolProvider* alternateMemoryPoolProvider) {
			mAlternateMemoryPoolProvider = alternateMemoryPoolProvider;
			mDeleteAlternateMemoryPoolProvider = nullptr;
			if (!mAlternateMemoryPoolProvider) {
				mDeleteAlternateMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
				mAlternateMemoryPoolProvider = mDeleteAlternateMemoryPoolProvider;
			}
			mProviderLinker = nullptr;
			mAddedProviderLinker = nullptr;
			mLastAddedProviderLinker = nullptr;
			mTmpStoredMemoryPools = nullptr;
		}

		CConcreteMemoryPoolDistributor::~CConcreteMemoryPoolDistributor() {
			delete mDeleteAlternateMemoryPoolProvider;
		}


		CDistributionMemoryPoolProvider* CConcreteMemoryPoolDistributor::createDistributionMemoryPoolProvider(CDistributionMemoryPoolReleaser* distributionMemoryReleaser) {
			CDistributionMemoryPoolProvider* provider = nullptr;
			mCreateProviderSyncLock.lock();
			provider = new CDistributionMemoryPoolProvider(mAlternateMemoryPoolProvider,distributionMemoryReleaser);
			mAddedProviderLinker = provider->setNext(mAddedProviderLinker);
			mCreateProviderSyncLock.unlock();
			return provider;
		}


		CConcreteMemoryPoolDistributor* CConcreteMemoryPoolDistributor::distributeMemoryPools(CMemoryPool* memoryPools) {
			CDistributionMemoryPoolProvider* tmpProvLinker = mAddedProviderLinker;
			CDistributionMemoryPoolProvider* nextLastAddedProvLinker = tmpProvLinker;
			while (tmpProvLinker && tmpProvLinker != mLastAddedProviderLinker) {
				CDistributionMemoryPoolProvider* tmpTakeProvLinker = tmpProvLinker;
				tmpProvLinker = tmpProvLinker->getNext();

				mProviderLinker = tmpTakeProvLinker->setNext(mProviderLinker);
			}
			mLastAddedProviderLinker = nextLastAddedProvLinker;

			tmpProvLinker = mProviderLinker;
			while (tmpProvLinker) {
				tmpProvLinker->updatedRoundAcquiredMemoryPoolCounts();
				tmpProvLinker = tmpProvLinker->getNext();
			}

			CDistributionMemoryPoolProvider* sortedProviderLinker = nullptr;

			tmpProvLinker = mProviderLinker;
			while (tmpProvLinker) {
				CDistributionMemoryPoolProvider* tmpSortProvLinker = tmpProvLinker;
				tmpProvLinker = tmpProvLinker->getNext();

				tmpSortProvLinker->clearNext();
				if (sortedProviderLinker) {
					sortedProviderLinker = sortedProviderLinker->insertNextSorted(tmpSortProvLinker);
				} else {
					sortedProviderLinker = tmpSortProvLinker;
				}
			}

			if (mTmpStoredMemoryPools) {
				memoryPools = mTmpStoredMemoryPools->getLastListLink()->setNextMemoryPool(memoryPools);
				mTmpStoredMemoryPools = nullptr;
			}

			while (memoryPools) {
				CMemoryPool* tmpMemoryPool = memoryPools;
				memoryPools = memoryPools->getNext();
				tmpMemoryPool->clearNext();

				if (sortedProviderLinker->checkAppendableDistributedMemoryPools(tmpMemoryPool)) {
					sortedProviderLinker->appendDistributedMemoryPools(tmpMemoryPool);
					sortedProviderLinker->updateMissingMemoryPoolRatio();

					// update sorted list
					CDistributionMemoryPoolProvider* nextSortedProviderLinker = sortedProviderLinker->getNext();
					if (nextSortedProviderLinker && sortedProviderLinker->getMissingMemoryPoolRatio() > nextSortedProviderLinker->getMissingMemoryPoolRatio()) {					
						sortedProviderLinker->clearNext();
						sortedProviderLinker = nextSortedProviderLinker->insertNextSorted(sortedProviderLinker);
					}
				} else {
					mTmpStoredMemoryPools = tmpMemoryPool->setNextMemoryPool(mTmpStoredMemoryPools);
				}
			}
			mProviderLinker = sortedProviderLinker;
			return this;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude

