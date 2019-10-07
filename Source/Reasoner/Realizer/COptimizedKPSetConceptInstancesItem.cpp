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

#include "COptimizedKPSetConceptInstancesItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetConceptInstancesItem::COptimizedKPSetConceptInstancesItem() {
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::initInstancesItem(CHierarchyNode* hierNode) {
				mUnprocessedSuccItemCount = 0;
				mTestingPossInstanceCount = 0;
				mAllSuccProcessedFlag = false;
				mToProcessFlag = false;
				mProcessingQueuedFlag = false;
				mSelfSuccsCompletedFlag = false;
				mHierNode = hierNode;
				mConceptInstancesTestingFinished = false;
				mPossibleInstanceMergingDataContainer = nullptr;
				mIndividualItemPossibleInstanceMergingLinkerHash = nullptr;
				mPrefferedPossibleInstancesSet = nullptr;
				mModelMergingCheckedSet = nullptr;
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setInitialUnprocessedSuccessorItemCount() {
				mUnprocessedSuccItemCount = mSuccessorItemList.count();
				return this;
			}

			CHierarchyNode* COptimizedKPSetConceptInstancesItem::getHierarchyNode() {
				return mHierNode;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setHierarchyNode(CHierarchyNode* hierNode) {
				mHierNode = hierNode;
				return this;
			}

			QMap<cint64, COptimizedKPSetIndividualItem*>* COptimizedKPSetConceptInstancesItem::getKnownInstancesMap() {
				return &mKnownInstancesMap;
			}

			QMap<cint64, COptimizedKPSetIndividualItem*>* COptimizedKPSetConceptInstancesItem::getPossibleInstancesMap() {
				return &mPossibleInstancesMap;
			}

			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetConceptInstancesItem::getParentItemList() {
				return &mParentItemList;
			}

			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetConceptInstancesItem::getSuccessorItemList() {
				return &mSuccessorItemList;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addKnownInstance(COptimizedKPSetIndividualItem* item) {
				mKnownInstancesMap.insert(item->getIndividualId(), item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::removeKnownInstance(COptimizedKPSetIndividualItem* item) {
				mKnownInstancesMap.remove(item->getIndividualId());
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasKnownInstance(COptimizedKPSetIndividualItem* item) {
				return mKnownInstancesMap.contains(item->getIndividualId());
			}


			bool COptimizedKPSetConceptInstancesItem::hasPossibleInstance(COptimizedKPSetIndividualItem* item) {
				return mPossibleInstancesMap.contains(item->getIndividualId());
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addPossibleInstance(COptimizedKPSetIndividualItem* item) {
				mPossibleInstancesMap.insert(item->getIndividualId(), item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addParentItem(COptimizedKPSetConceptInstancesItem* item) {
				mParentItemList.append(item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addSuccessorItem(COptimizedKPSetConceptInstancesItem* item) {
				mSuccessorItemList.append(item);
				return this;
			}

			cint64 COptimizedKPSetConceptInstancesItem::getUnprocessedSuccessorItemCount() {
				return mUnprocessedSuccItemCount;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setUnprocessedSuccessorItemCount(cint64 unpSuccCount) {
				mUnprocessedSuccItemCount = unpSuccCount;
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::incUnprocessedSuccessorItemCount(cint64 incCount) {
				mUnprocessedSuccItemCount += incCount;
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::decUnprocessedSuccessorItemCount(cint64 decCount) {
				mUnprocessedSuccItemCount -= decCount;
				return this;
			}

			cint64 COptimizedKPSetConceptInstancesItem::getTestingPossibleInstancesCount() {
				return mTestingPossInstanceCount;
			}

			bool COptimizedKPSetConceptInstancesItem::hasTestingPossibleInstances() {
				return mTestingPossInstanceCount > 0;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setTestingPossibleInstancesCount(cint64 testPossIntCount) {
				mTestingPossInstanceCount = testPossIntCount;
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::incTestingPossibleInstancesCount(cint64 incCount) {
				mTestingPossInstanceCount += incCount;
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::decTestingPossibleInstancesCount(cint64 decCount) {
				mTestingPossInstanceCount -= decCount;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasAllSuccessorProcessedFlag() {
				return mAllSuccProcessedFlag;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setAllSuccessorProcessedFlag(bool allSuccProcessed) {
				mAllSuccProcessedFlag = allSuccProcessed;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasToProcessPossibleInstancesFlag() {
				return mToProcessFlag;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setToProcessPossibleInstancesFlag(bool toProcess) {
				mToProcessFlag = toProcess;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasPossibleInstancesProcessingQueuedFlag() {
				return mProcessingQueuedFlag;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setPossibleInstancesProcessingQueuedFlag(bool processingQueued) {
				mProcessingQueuedFlag = processingQueued;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasSelfSuccessorProcessedFlag() {
				return mSelfSuccsCompletedFlag;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setSelfSuccessorProcessedFlag(bool selfSuccProcessed) {
				mSelfSuccsCompletedFlag = selfSuccProcessed;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasPossibleInstances() {
				return !mPossibleInstancesMap.isEmpty();
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::removeTestingPossibleInstance(COptimizedKPSetIndividualItem* possInstance) {
				mPossibleInstancesMap.remove(possInstance->getIndividualId());
				if (mPrefferedPossibleInstancesSet) {
					mPrefferedPossibleInstancesSet->remove(possInstance->getIndividualId());
				}
				return this;
			}

			QMap<cint64, COptimizedKPSetIndividualItem*>* COptimizedKPSetConceptInstancesItem::getPrefferedPossibleInstanceTestingSet() {
				return mPrefferedPossibleInstancesSet;
			}


			COptimizedKPSetIndividualItem* COptimizedKPSetConceptInstancesItem::takeNextTestingPossibleInstance() {
				COptimizedKPSetIndividualItem* nextItem = nullptr;
				if (hasPossibleInstances()) {
					if (mIndividualItemPossibleInstanceMergingLinkerHash) {
						if (!mPrefferedPossibleInstancesSet) {
							mPrefferedPossibleInstancesSet = new QMap<cint64, COptimizedKPSetIndividualItem *>(mPossibleInstancesMap);
						}
					}

					if (mPrefferedPossibleInstancesSet && !mPrefferedPossibleInstancesSet->isEmpty()) {
						QMap<cint64, COptimizedKPSetIndividualItem*>::iterator it = mPrefferedPossibleInstancesSet->begin();
						while (it != mPrefferedPossibleInstancesSet->end()) {
							nextItem = *it;
							it = mPrefferedPossibleInstancesSet->erase(it);
							CPossibleInstancesIndividualsMergingLinker* linker = mIndividualItemPossibleInstanceMergingLinkerHash->value(nextItem);
							if (!linker || linker->isSatisfiableMerged()) {
								break;
							}
						}
						mPossibleInstancesMap.remove(nextItem->getIndividualId());

					} else {

						QMap<cint64, COptimizedKPSetIndividualItem*>::iterator it = mPossibleInstancesMap.begin();
						nextItem = *it;
						mPossibleInstancesMap.erase(it);
					}
				}
				return nextItem;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::setConceptInstancesTestingFinished(bool finished) {
				mConceptInstancesTestingFinished = finished;
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::isConceptInstancesTestingFinished() {
				return mConceptInstancesTestingFinished;
			}


			QHash<COptimizedKPSetIndividualItem*, CPossibleInstancesIndividualsMergingLinker*>* COptimizedKPSetConceptInstancesItem::getIndividualItemPossibleInstanceMergingLinkerHash(bool create) {
				if (create && !mIndividualItemPossibleInstanceMergingLinkerHash) {
					mIndividualItemPossibleInstanceMergingLinkerHash = new QHash<COptimizedKPSetIndividualItem *, CPossibleInstancesIndividualsMergingLinker *>();
				}
				return mIndividualItemPossibleInstanceMergingLinkerHash;
			}

			QList<CPossibleInstancesIndividualsMergingData*>* COptimizedKPSetConceptInstancesItem::getPossibleInstanceMergingDataContainer(bool create) {
				if (create && !mPossibleInstanceMergingDataContainer) {
					mPossibleInstanceMergingDataContainer = new QList<CPossibleInstancesIndividualsMergingData *>();
				}
				return mPossibleInstanceMergingDataContainer;
			}


			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::clearPossibleInstanceMergingData() {
				if (mPossibleInstanceMergingDataContainer) {
					qDeleteAll(*mPossibleInstanceMergingDataContainer);
					delete mPossibleInstanceMergingDataContainer;
					mPossibleInstanceMergingDataContainer = nullptr;
				}
				if (mIndividualItemPossibleInstanceMergingLinkerHash) {
					delete mIndividualItemPossibleInstanceMergingLinkerHash;
					mIndividualItemPossibleInstanceMergingLinkerHash = nullptr;
				}
				if (mPrefferedPossibleInstancesSet) {
					delete mPrefferedPossibleInstancesSet;
					mPrefferedPossibleInstancesSet = nullptr;
				}
				if (mModelMergingCheckedSet) {
					delete mModelMergingCheckedSet;
					mModelMergingCheckedSet = nullptr;
				}
				return this;
			}


			QMap<cint64, COptimizedKPSetIndividualItem*>* COptimizedKPSetConceptInstancesItem::getModelMergingCheckedSet(bool create) {
				if (!mModelMergingCheckedSet && create) {
					mModelMergingCheckedSet = new QMap<cint64, COptimizedKPSetIndividualItem*>();
				}
				return mModelMergingCheckedSet;
			}


			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addKnownInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item) {
				mKnownInstancesLabelCacheItemList.append(item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addKnownMostSpecificInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item) {
				mKnownMostSpecificInstancesLabelCacheItemList.append(item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addPossibleInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item) {
				mPossibleInstancesLabelCacheItemList.append(item);
				return this;
			}


			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addPossibleMostSpecificInstancesLabelCacheItem(CBackendRepresentativeMemoryLabelCacheItem* item) {
				mPossibleMostSpecificInstancesLabelCacheItemList.append(item);
				return this;
			}

			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetConceptInstancesItem::getKnownInstancesLabelCacheItemList() {
				return &mKnownInstancesLabelCacheItemList;
			}

			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetConceptInstancesItem::getKnownMostSpecificInstancesLabelCacheItemList() {
				return &mKnownMostSpecificInstancesLabelCacheItemList;
			}

			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetConceptInstancesItem::getPossibleInstancesLabelCacheItemList() {
				return &mPossibleInstancesLabelCacheItemList;
			}

			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetConceptInstancesItem::getPossibleMostSpecificInstancesLabelCacheItemList() {
				return &mPossibleMostSpecificInstancesLabelCacheItemList;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
