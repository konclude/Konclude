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

			QSet<COptimizedKPSetConceptInstantiatedItem*>* COptimizedKPSetConceptInstancesItem::getKnownInstancesSet() {
				return &mKnownInstancesSet;
			}

			QSet<COptimizedKPSetConceptInstantiatedItem*>* COptimizedKPSetConceptInstancesItem::getPossibleInstancesSet() {
				return &mPossibleInstancesSet;
			}

			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetConceptInstancesItem::getParentItemList() {
				return &mParentItemList;
			}

			QList<COptimizedKPSetConceptInstancesItem*>* COptimizedKPSetConceptInstancesItem::getSuccessorItemList() {
				return &mSuccessorItemList;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addKnownInstance(COptimizedKPSetConceptInstantiatedItem* item) {
				mKnownInstancesSet.insert(item);
				return this;
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::removeKnownInstance(COptimizedKPSetConceptInstantiatedItem* item) {
				mKnownInstancesSet.remove(item);
				return this;
			}

			bool COptimizedKPSetConceptInstancesItem::hasKnownInstance(COptimizedKPSetConceptInstantiatedItem* item) {
				return mKnownInstancesSet.contains(item);
			}


			bool COptimizedKPSetConceptInstancesItem::hasPossibleInstance(COptimizedKPSetConceptInstantiatedItem* item) {
				return mPossibleInstancesSet.contains(item);
			}

			COptimizedKPSetConceptInstancesItem* COptimizedKPSetConceptInstancesItem::addPossibleInstance(COptimizedKPSetConceptInstantiatedItem* item) {
				mPossibleInstancesSet.insert(item);
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
				return !mPossibleInstancesSet.isEmpty();
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstancesItem::takeNextTestingPossibleInstance() {
				COptimizedKPSetConceptInstantiatedItem* nextItem = nullptr;
				if (hasPossibleInstances()) {
					QSet<COptimizedKPSetConceptInstantiatedItem*>::iterator it = mPossibleInstancesSet.begin();
					nextItem = *it;
					mPossibleInstancesSet.erase(it);
				}
				return nextItem;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
