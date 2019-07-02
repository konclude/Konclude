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

#include "COptimizedKPSetRoleInstancesItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetRoleInstancesItem::COptimizedKPSetRoleInstancesItem() : COptimizedKPSetRoleInstancesRedirectionItem(this,nullptr,false) {
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::initInstancesItem(CRole* role) {
				mRole = role;
				mUnprocessedSuccItemCount = 0;
				mTestingPossInstanceCount = 0;
				mAllSuccProcessedFlag = false;
				mToProcessFlag = false;
				mProcessingQueuedFlag = false;
				mSelfSuccsCompletedFlag = false;
				mRequiresCandidateInitializationFlag = true;
				mUninitializedSuccItemCount = 0;
				mAllSuccInitializedFlag = false;
				mInitializingQueuedFlag = false;
				mInitializationStartedFlag = false;
				mToInitializeFlag = false;
				mTmpPropagationConcept = nullptr;
				mTmpMarkerConcept = nullptr;
				mTmpInversePropagationConcept = nullptr;
				mTmpInverseMarkerConcept = nullptr;
				mInvRole = nullptr;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInitialUnprocessedSuccessorItemCount() {
				mUnprocessedSuccItemCount = mSuccessorItemList.count();
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInitialUninitializedSuccessorItemCount() {
				mUninitializedSuccItemCount = mSuccessorItemList.count();
				return this;
			}

			CRole* COptimizedKPSetRoleInstancesItem::getRole() {
				return mRole;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setRole(CRole* role) {
				mRole = role;
				return this;
			}

			QSet<COptimizedKPSetIndividualItemPair>* COptimizedKPSetRoleInstancesItem::getKnownInstancesSet() {
				return &mKnownInstancesSet;
			}

			QSet<COptimizedKPSetIndividualItemPair>* COptimizedKPSetRoleInstancesItem::getPossibleInstancesSet() {
				return &mPossibleInstancesSet;
			}

			QList<TRoleItemInversionPair>* COptimizedKPSetRoleInstancesItem::getParentItemList() {
				return &mParentItemList;
			}

			QList<TRoleItemInversionPair>* COptimizedKPSetRoleInstancesItem::getSuccessorItemList() {
				return &mSuccessorItemList;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mKnownInstancesSet.insert(itemPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mKnownInstancesSet.remove(itemPair);
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mKnownInstancesSet.contains(itemPair);
			}


			bool COptimizedKPSetRoleInstancesItem::hasPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mPossibleInstancesSet.contains(itemPair);
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInstancesSet.insert(itemPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removePossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInstancesSet.remove(itemPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addParentItem(COptimizedKPSetRoleInstancesItem* item, bool inversed) {
				mParentItemList.append(TRoleItemInversionPair(item,inversed));
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addSuccessorItem(COptimizedKPSetRoleInstancesItem* item, bool inversed) {
				mSuccessorItemList.append(TRoleItemInversionPair(item,inversed));
				return this;
			}

			cint64 COptimizedKPSetRoleInstancesItem::getUnprocessedSuccessorItemCount() {
				return mUnprocessedSuccItemCount;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setUnprocessedSuccessorItemCount(cint64 unpSuccCount) {
				mUnprocessedSuccItemCount = unpSuccCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::incUnprocessedSuccessorItemCount(cint64 incCount) {
				mUnprocessedSuccItemCount += incCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::decUnprocessedSuccessorItemCount(cint64 decCount) {
				mUnprocessedSuccItemCount -= decCount;
				return this;
			}

			cint64 COptimizedKPSetRoleInstancesItem::getTestingPossibleInstancesCount() {
				return mTestingPossInstanceCount;
			}

			bool COptimizedKPSetRoleInstancesItem::hasTestingPossibleInstances() {
				return mTestingPossInstanceCount > 0;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTestingPossibleInstancesCount(cint64 testPossIntCount) {
				mTestingPossInstanceCount = testPossIntCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::incTestingPossibleInstancesCount(cint64 incCount) {
				mTestingPossInstanceCount += incCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::decTestingPossibleInstancesCount(cint64 decCount) {
				mTestingPossInstanceCount -= decCount;
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasAllSuccessorProcessedFlag() {
				return mAllSuccProcessedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setAllSuccessorProcessedFlag(bool allSuccProcessed) {
				mAllSuccProcessedFlag = allSuccProcessed;
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasToProcessPossibleInstancesFlag() {
				return mToProcessFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setToProcessPossibleInstancesFlag(bool toProcess) {
				mToProcessFlag = toProcess;
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasPossibleInstancesProcessingQueuedFlag() {
				return mProcessingQueuedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setPossibleInstancesProcessingQueuedFlag(bool processingQueued) {
				mProcessingQueuedFlag = processingQueued;
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasSelfSuccessorProcessedFlag() {
				return mSelfSuccsCompletedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setSelfSuccessorProcessedFlag(bool selfSuccProcessed) {
				mSelfSuccsCompletedFlag = selfSuccProcessed;
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasPossibleInstances() {
				return !mPossibleInstancesSet.isEmpty();
			}

			COptimizedKPSetIndividualItemPair COptimizedKPSetRoleInstancesItem::takeNextTestingPossibleInstance() {
				COptimizedKPSetIndividualItemPair nextItem(nullptr,nullptr);
				if (hasPossibleInstances()) {
					QSet<COptimizedKPSetIndividualItemPair>::iterator it = mPossibleInstancesSet.begin();
					nextItem = *it;
					mPossibleInstancesSet.erase(it);
				}
				return nextItem;
			}


			bool COptimizedKPSetRoleInstancesItem::requiresCandidateInitialization() {
				return mRequiresCandidateInitializationFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setRequiresCandidateInitialization(bool requiresInit) {
				mRequiresCandidateInitializationFlag = requiresInit;
				return this;
			}


			bool COptimizedKPSetRoleInstancesItem::hasRemainingCandidateInitializationSuccessors() {
				return !mRemainingCandidateInitializationSuccessorList.isEmpty();
			}

			QList<COptimizedKPSetIndividualItem*>* COptimizedKPSetRoleInstancesItem::getRemainingCandidateInitializationSuccessorList() {
				return &mRemainingCandidateInitializationSuccessorList;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetRoleInstancesItem::takeNextRemainingCandidateInitializationSuccessor() {
				if (!mRemainingCandidateInitializationSuccessorList.isEmpty()) {
					return mRemainingCandidateInitializationSuccessorList.takeFirst();
				}
				return nullptr;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addRemainingCandidateInitializationSuccessor(COptimizedKPSetIndividualItem* item) {
				mRemainingCandidateInitializationSuccessorList.append(item);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addRemainingCandidateInitializationSuccessors(QList<COptimizedKPSetIndividualItem*>* itemList) {
				mRemainingCandidateInitializationSuccessorList.append(*itemList);
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::isCandidateSuccessorInitializationCompleted() {
				return mInitializationStartedFlag && mRemainingCandidateInitializationSuccessorList.isEmpty();
			}

			bool COptimizedKPSetRoleInstancesItem::isCandidateSuccessorInitializationStarted() {
				return mInitializationStartedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setCandidateSuccessorInitializationStarted(bool initStarted) {
				mInitializationStartedFlag = initStarted;
				return this;
			}



			cint64 COptimizedKPSetRoleInstancesItem::getUninitializedSuccessorItemCount() {
				return mUninitializedSuccItemCount;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setUninitializedSuccessorItemCount(cint64 unintSuccCount) {
				mUninitializedSuccItemCount = unintSuccCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::incUninitializedSuccessorItemCount(cint64 incCount) {
				mUninitializedSuccItemCount += incCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::decUninitializedSuccessorItemCount(cint64 decCount) {
				mUninitializedSuccItemCount -= decCount;
				return this;
			}


			bool COptimizedKPSetRoleInstancesItem::hasAllSuccessorInitializedFlag() {
				return mAllSuccInitializedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setAllSuccessorInitializedFlag(bool allSuccInitialized) {
				mAllSuccInitializedFlag = allSuccInitialized;
				return this;
			}


			bool COptimizedKPSetRoleInstancesItem::hasInitializingQueuedFlag() {
				return mInitializingQueuedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInitializingQueuedFlag(bool initializingQueued) {
				mInitializingQueuedFlag = initializingQueued;
				return this;
			}


			bool COptimizedKPSetRoleInstancesItem::hasToInitializeCandidatesFlag() {
				return mToInitializeFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setToInitializeCandidatesFlag(bool toInitialize) {
				mToInitializeFlag = toInitialize;
				return this;
			}

			CConcept* COptimizedKPSetRoleInstancesItem::getTemporaryPropagationConcept() {
				return mTmpPropagationConcept;
			}

			CConcept* COptimizedKPSetRoleInstancesItem::getTemporaryMarkerConcept() {
				return mTmpMarkerConcept;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTemporaryPropagationConcept(CConcept* propagationConcept) {
				mTmpPropagationConcept = propagationConcept;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTemporaryMarkerConcept(CConcept* markerConcept) {
				mTmpMarkerConcept = markerConcept;
				return this;
			}





			CConcept* COptimizedKPSetRoleInstancesItem::getTemporaryInversePropagationConcept() {
				return mTmpInversePropagationConcept;
			}

			CConcept* COptimizedKPSetRoleInstancesItem::getTemporaryInverseMarkerConcept() {
				return mTmpInverseMarkerConcept;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTemporaryInversePropagationConcept(CConcept* propagationConcept) {
				mTmpInversePropagationConcept = propagationConcept;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTemporaryInverseMarkerConcept(CConcept* markerConcept) {
				mTmpInverseMarkerConcept = markerConcept;
				return this;
			}



			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mKnownInverseInstancesSet.insert(itemPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mKnownInverseInstancesSet.remove(itemPair);
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mKnownInverseInstancesSet.contains(itemPair);
			}


			bool COptimizedKPSetRoleInstancesItem::hasPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mPossibleInverseInstancesSet.contains(itemPair);
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInverseInstancesSet.insert(itemPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removePossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInverseInstancesSet.remove(itemPair);
				return this;
			}




			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					addKnownInverseInstance(itemPair);
				} else {
					addKnownInstance(itemPair);
				}
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					removeKnownInverseInstance(itemPair);
				} else {
					removeKnownInstance(itemPair);
				}
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					return hasKnownInverseInstance(itemPair);
				} else {
					return hasKnownInstance(itemPair);
				}
			}


			bool COptimizedKPSetRoleInstancesItem::hasPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					return hasPossibleInverseInstance(itemPair);
				} else {
					return hasPossibleInstance(itemPair);
				}
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					addPossibleInverseInstance(itemPair);
				} else {
					addPossibleInstance(itemPair);
				}
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removePossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					removePossibleInverseInstance(itemPair);
				} else {
					removePossibleInstance(itemPair);
				}
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasInverseRole() {
				return mInvRole;
			}

			CRole* COptimizedKPSetRoleInstancesItem::getInverseRole() {
				return mInvRole;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInverseRole(CRole* role) {
				mInvRole = role;
				return this;
			}

			QSet<CRole*>* COptimizedKPSetRoleInstancesItem::getComplexRoleStarterCandidateSet() {
				return &mComplexRoleStarterCandidateSet;
			}

			QSet<CRole*>* COptimizedKPSetRoleInstancesItem::getComplexInverseRoleStarterCandidateSet() {
				return &mComplexInverseRoleStarterCandidateSet;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addComplexRoleStarterCandidate(CRole* role) {
				mComplexRoleStarterCandidateSet.insert(role);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addComplexInverseRoleStarterCandidate(CRole* role) {
				mComplexInverseRoleStarterCandidateSet.insert(role);
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasComplexRoleStarterCandidate(CRole* role) {
				return mComplexRoleStarterCandidateSet.contains(role);
			}

			bool COptimizedKPSetRoleInstancesItem::hasComplexInverseRoleStarterCandidate(CRole* role) {
				return mComplexInverseRoleStarterCandidateSet.contains(role);
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
