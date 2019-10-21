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
				mTestingPossibleInstancesCount = 0;
				mTestingInitializationPropagationCount = 0;
				mAllSuccProcessedFlag = false;
				mToProcessFlag = false;
				mPossibleProcessingQueuedFlag = false;
				mSelfSuccsCompletedFlag = false;
				mRequiresCandidateInitializationFlag = true;
				mUninitializedSuccItemCount = 0;
				mAllSuccInitializedFlag = false;
				mInitializingQueuedFlag = false;
				mInitializationStartedFlag = false;
				mInitializationCompletedFlag = false;
				mToInitializeFlag = false;
				mTmpPropagationConcept = nullptr;
				mTmpMarkerConcept = nullptr;
				mTmpInversePropagationConcept = nullptr;
				mTmpInverseMarkerConcept = nullptr;
				mInvRole = nullptr;
				mRoleHierNode = nullptr;
				mInvRoleHierNode = nullptr;
				mInvRedirectedItem = nullptr;
				mComplexIndiCandidateIterator = nullptr;
				mOnlyTransitiveComplexRoleCandidates = true;
				mOnlyDeterministicComplexRoleStarterCandidatesUsage = true;

				mPropagationDirectionDetermined = false;
				mPreferredPropagationDirection = NONE;
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

			CRolePropertiesHierarchyNode* COptimizedKPSetRoleInstancesItem::getRoleHierarchyNode() {
				return mRoleHierNode;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setRoleHierarchyNode(CRolePropertiesHierarchyNode* roleHierNode) {
				mRoleHierNode = roleHierNode;
				return this; 
			}


			CRolePropertiesHierarchyNode* COptimizedKPSetRoleInstancesItem::getInverseRoleHierarchyNode() {
				return mInvRoleHierNode;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInverseRoleHierarchyNode(CRolePropertiesHierarchyNode* roleHierNode) {
				mInvRoleHierNode = roleHierNode;
				return this; 
			}


			COptimizedKPSetRoleInstancesRedirectionItem* COptimizedKPSetRoleInstancesItem::getInverseRoleRedirectedItem() {
				return mInvRedirectedItem;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setInverseRoleRedirectedItem(COptimizedKPSetRoleInstancesRedirectionItem* redirectedItem) {
				mInvRedirectedItem = redirectedItem;
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






			cint64 COptimizedKPSetRoleInstancesItem::getTestingComplexCandidateInstancesCount() {
				return mTestingPossibleInstancesCount;
			}

			bool COptimizedKPSetRoleInstancesItem::hasTestingComplexCandidateInstancesCount() {
				return mTestingPossibleInstancesCount > 0;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTestingComplexCandidateInstancesCount(cint64 testPossIntCount) {
				mTestingPossibleInstancesCount = testPossIntCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::incTestingComplexCandidateInstancesCount(cint64 incCount) {
				mTestingPossibleInstancesCount += incCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::decTestingComplexCandidateInstancesCount(cint64 decCount) {
				mTestingPossibleInstancesCount -= decCount;
				return this;
			}







			cint64 COptimizedKPSetRoleInstancesItem::getTestingInitializationPropagationCount() {
				return mTestingInitializationPropagationCount;
			}

			bool COptimizedKPSetRoleInstancesItem::hasTestingInitializationPropagationCount() {
				return mTestingInitializationPropagationCount > 0;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setTestingInitializationPropagationCount(cint64 testPossIntCount) {
				mTestingInitializationPropagationCount = testPossIntCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::incTestingInitializationPropagationCount(cint64 incCount) {
				mTestingInitializationPropagationCount += incCount;
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::decTestingInitializationPropagationCount(cint64 decCount) {
				mTestingInitializationPropagationCount -= decCount;
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
				return mPossibleProcessingQueuedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setPossibleInstancesProcessingQueuedFlag(bool processingQueued) {
				mPossibleProcessingQueuedFlag = processingQueued;
				return this;
			}


			bool COptimizedKPSetRoleInstancesItem::hasComplexCandidateInstancesProcessingQueuedFlag() {
				return mComplexCandidateProcessingQueuedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setComplexCandidateProcessingQueuedFlag(bool processingQueued) {
				mComplexCandidateProcessingQueuedFlag = processingQueued;
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
				return mInitializationStartedFlag && mInitializationCompletedFlag;
			}

			bool COptimizedKPSetRoleInstancesItem::isCandidateSuccessorInitializationStarted() {
				return mInitializationStartedFlag;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setCandidateSuccessorInitializationStarted(bool initStarted) {
				mInitializationStartedFlag = initStarted;
				return this;
			}


			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setCandidateSuccessorInitializationCompleted(bool initCompleted) {
				mInitializationCompletedFlag = initCompleted;
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
				mKnownInstancesSet.insert(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mKnownInstancesSet.remove(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasKnownInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mKnownInstancesSet.contains(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
			}


			bool COptimizedKPSetRoleInstancesItem::hasPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				return mPossibleInstancesSet.contains(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addPossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInstancesSet.insert(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removePossibleInverseInstance(const COptimizedKPSetIndividualItemPair& itemPair) {
				mPossibleInstancesSet.remove(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				return this;
			}




			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					addKnownInstance(COptimizedKPSetIndividualItemPair(itemPair.second,itemPair.first));
				} else {
					addKnownInstance(itemPair);
				}
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					removeKnownInstance(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				} else {
					removeKnownInstance(itemPair);
				}
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasKnownInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					return hasKnownInstance(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				} else {
					return hasKnownInstance(itemPair);
				}
			}


			bool COptimizedKPSetRoleInstancesItem::hasPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					return hasPossibleInstance(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				} else {
					return hasPossibleInstance(itemPair);
				}
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addPossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					addPossibleInstance(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
				} else {
					addPossibleInstance(itemPair);
				}
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removePossibleInstance(const COptimizedKPSetIndividualItemPair& itemPair, bool inverse) {
				if (inverse) {
					removePossibleInstance(COptimizedKPSetIndividualItemPair(itemPair.second, itemPair.first));
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




			bool COptimizedKPSetRoleInstancesItem::hasOnlyTransitiveComplexRoleCandidates() {
				return mOnlyTransitiveComplexRoleCandidates;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setOnlyTransitiveComplexRoleCandidates(bool onlyTransitiveCandidates) {
				mOnlyTransitiveComplexRoleCandidates = onlyTransitiveCandidates;
				return this;
			}




			QSet<TRoleItemInversionPair>* COptimizedKPSetRoleInstancesItem::getOnlyTransitiveComplexSubRoleItems() {
				return &mOnlyTransitiveComplexSubRoleItems;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addOnlyTransitiveComplexSubRoleItem(const TRoleItemInversionPair& item) {
				mOnlyTransitiveComplexSubRoleItems.insert(item);
				return this;
			}




			bool COptimizedKPSetRoleInstancesItem::hasOnlyDeterministicComplexRoleStarterCandidatesUsage() {
				return mOnlyDeterministicComplexRoleStarterCandidatesUsage;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setOnlyDeterministicComplexRoleStarterCandidatesUsage(bool onlyDeterministicComplexRoleStarterCandidatesUsage) {
				mOnlyDeterministicComplexRoleStarterCandidatesUsage = onlyDeterministicComplexRoleStarterCandidatesUsage;
				return this;
			}




			COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelHash* COptimizedKPSetRoleInstancesItem::getCombinedNeighbourCacheLabelItemDataHash(bool inversed) {
				if (!inversed) {
					return &mCombinedNeighbourLabelDataHash;
				} else {
					return &mInverseCombinedNeighbourLabelDataHash;
				}
			}


			bool COptimizedKPSetRoleInstancesItem::hasCombinedNeighbourCacheLabelItemInversed(CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem) {
				return mInverseCombinedNeighbourLabelDataHash.contains(cacheLabelItem);
			}


			QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* COptimizedKPSetRoleInstancesItem::getIndividualIdComplexRoleDataHash() {
				return &mIndividualIdComplexRoleDataHash;
			}

			bool COptimizedKPSetRoleInstancesItem::hasComplexRoleData() {
				return mIndividualIdComplexRoleDataHash.size() > 0;
			}


			COptimizedKPSetIndividualComplexRoleData* COptimizedKPSetRoleInstancesItem::getIndividualIdComplexRoleData(cint64 individualId, bool createIfNotExists) {
				if (createIfNotExists) {
					COptimizedKPSetIndividualComplexRoleData*& complexRoleData = mIndividualIdComplexRoleDataHash[individualId];
					if (!complexRoleData) {
						complexRoleData = new COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData();
					}
					return complexRoleData;
				} else {
					return mIndividualIdComplexRoleDataHash.value(individualId);
				}				
			}



			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addComplexStarterCandidateCombinedNeighbourRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem) {
				if (!inversed) {
					mComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet.insert(labelCacheItem);
				} else {
					mComplexInverseStarterCandidateCombinedNeighbourRoleLabelCacheItemSet.insert(labelCacheItem);
				}
				return this;
			}



			QSet<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetRoleInstancesItem::getComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet(bool inversed) {
				if (inversed) {
					return &mComplexInverseStarterCandidateCombinedNeighbourRoleLabelCacheItemSet;
				} else {
					return &mComplexStarterCandidateCombinedNeighbourRoleLabelCacheItemSet;
				}
			}





			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addComplexStarterCandidateCombinedExistentialRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem) {
				if (!inversed) {
					mComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet.insert(labelCacheItem);
				} else {
					mComplexInverseStarterCandidateCombinedExistentialRoleLabelCacheItemSet.insert(labelCacheItem);
				}
				return this;
			}



			QSet<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetRoleInstancesItem::getComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet(bool inversed) {
				if (inversed) {
					return &mComplexInverseStarterCandidateCombinedExistentialRoleLabelCacheItemSet;
				} else {
					return &mComplexStarterCandidateCombinedExistentialRoleLabelCacheItemSet;
				}
			}




			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addExistentialRoleLabelCacheItem(bool inversed, CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem) {
				if (!inversed) {
					mExistentialRoleLabelCacheItemSet.insert(labelCacheItem);
				} else {
					mExistentialInverseRoleLabelCacheItemSet.insert(labelCacheItem);
				}
				return this;
			}

			QSet<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedKPSetRoleInstancesItem::getExistentialRoleLabelCacheItemSet(bool inversed) {
				if (!inversed) {
					return &mExistentialRoleLabelCacheItemSet;
				} else {
					return &mExistentialInverseRoleLabelCacheItemSet;
				}
			}			


			COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* COptimizedKPSetRoleInstancesItem::getIndividualComplexCandidateIterator() {
				return mComplexIndiCandidateIterator;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setIndividualComplexCandidateIterator(COptimizedRepresentativeKPSetCacheMultipleTypeLabelSetItemIterator* iterator) {
				mComplexIndiCandidateIterator = iterator;
				return this;
			}



			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::addComplexCandidateInstance(const COptimizedKPSetIndividualItemReferencePair& itemRefPair, bool inverse) {
				COptimizedKPSetIndividualItemReferencePair corrItemRefPair = itemRefPair;
				if (inverse) {
					corrItemRefPair = COptimizedKPSetIndividualItemReferencePair(corrItemRefPair.second, corrItemRefPair.first);
				}
				mComplexRoleCandidateInstancesSet.insert(corrItemRefPair);
				return this;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::removeComplexCandidateInstance(const COptimizedKPSetIndividualItemReferencePair& itemRefPair, bool inverse) {
				COptimizedKPSetIndividualItemReferencePair corrItemRefPair = itemRefPair;
				if (inverse) {
					corrItemRefPair = COptimizedKPSetIndividualItemReferencePair(corrItemRefPair.second, corrItemRefPair.first);
				}
				mComplexRoleCandidateInstancesSet.remove(corrItemRefPair);
				return this;
			}

			bool COptimizedKPSetRoleInstancesItem::hasComplexInstanceCandidates() {
				return !mComplexRoleCandidateInstancesSet.isEmpty();
			}

			COptimizedKPSetIndividualItemReferencePair COptimizedKPSetRoleInstancesItem::takeNextTestingComplexInstanceCandidate() {
				COptimizedKPSetIndividualItemReferencePair nextItem = COptimizedKPSetIndividualItemReferencePair(CRealizationIndividualInstanceItemReference(), CRealizationIndividualInstanceItemReference());
				if (hasPossibleInstances()) {
					QSet<COptimizedKPSetIndividualItemReferencePair>::iterator it = mComplexRoleCandidateInstancesSet.begin();
					nextItem = *it;
					mComplexRoleCandidateInstancesSet.erase(it);
				}
				return nextItem;
			}


			bool COptimizedKPSetRoleInstancesItem::hasPropagationDirectionDetermined() {
				return mPropagationDirectionDetermined;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setPropagationDirectionDetermined(bool determined) {
				mPropagationDirectionDetermined = determined;
				return this;
			}

			COptimizedKPSetRoleInstancesItem::RolePreferredPropagationDirection COptimizedKPSetRoleInstancesItem::getPreferredPropagationDirection() {
				return mPreferredPropagationDirection;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedKPSetRoleInstancesItem::setPreferredPropagationDirection(RolePreferredPropagationDirection direction) {
				mPreferredPropagationDirection = direction;
				return this;
			}




		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
