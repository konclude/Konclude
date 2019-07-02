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

#include "CIndividualProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {




				CIndividualProcessNode::CIndividualProcessNode(CProcessContext* processContext) : mProcessContext(processContext) {

					mMemAllocMan = CContext::getMemoryAllocationManager(mProcessContext);
					mPrevIndividual = nullptr;

					mConceptProcessingQueue = nullptr;
					mUseConceptProcessingQueue = nullptr;
					mPrevConceptProcessingQueue = nullptr;
					mReapplyConLabelSet = nullptr;
					mUseReapplyConLabelSet = nullptr;
					mPrevReapplyConLabelSet = nullptr;

					mReapplyRoleSuccHash = nullptr;
					mUseReapplyRoleSuccHash = nullptr;
					mPrevReapplyRoleSuccHash = nullptr;

					mLastAddedLink = nullptr;

					mConceptPropBindingSetHash = nullptr;
					mUseConceptPropBindingSetHash = nullptr;
					mPrevConceptPropBindingSetHash = nullptr;

					mConceptVarBindPathSetHash = nullptr;
					mUseConceptVarBindPathSetHash = nullptr;
					mPrevConceptVarBindPathSetHash = nullptr;

					mConceptRepPropSetHash = nullptr;
					mUseConceptRepPropSetHash = nullptr;
					mPrevConceptRepPropSetHash = nullptr;

					mSuccRoleHash = nullptr;
					mUseSuccRoleHash = nullptr;
					mPrevSuccRoleHash = nullptr;

					mConnSuccSet = nullptr;
					mUseConnSuccSet = nullptr;
					mPrevConnSuccSet = nullptr;

					mDisjointSuccRoleHash = nullptr;
					mUseDisjointSuccRoleHash = nullptr;
					mPrevDisjointSuccRoleHash = nullptr;
					mDisjointRoleConnections = false;


					mDistinctHash = nullptr;
					mUseDistinctHash = nullptr;
					mPrevDistinctHash = nullptr;

					mBackendSyncData = nullptr;
					mPrevBackendSyncData = nullptr;

					indiModel = nullptr;
					mSatCacheRetData = nullptr;
					mPrevSatCacheRetData = nullptr;
					mSatCacheStoringData = nullptr;
					mPrevSatCacheStoringData = nullptr;
					mIndiBlock = nullptr;
					mPrevIndiBlock = nullptr;
					mAncestorLink = nullptr;

					mIndiSatBlockData = nullptr;
					mPrevIndiSatBlockData = nullptr;

					mIndiUnsatCacheRet = nullptr;
					mPrevIndiUnsatCacheRet = nullptr;

					mSigBlockConExpData = nullptr;
					mPrevSigBlockConExpData = nullptr;

					mReusingConExpData = nullptr;
					mPrevReusingConExpData = nullptr;

					mInitializingConceptLinkerIt = nullptr;
					mProcessInitializingConceptLinkerIt = nullptr;
					mAssertionConceptLinkerIt = nullptr;
					mAssertionRoleLinkerIt = nullptr;
					mBlockedIndividualsLinker = nullptr;
					mReverseAssertionRoleLinkerIt = nullptr;
					mAdditionalRoleAssertionsLinker = nullptr;
					mRoleAssertionsInitialized = false;
					mReverseRoleAssertionsInitialized = false;

					mInitConceptDescriptor = nullptr;

					mSatCachedAbsorbedDisjunctionsReapplyConDes = nullptr;
					mSatCachedAbsorbedSuccessorReapplyConDes = nullptr;

					mSigBlockIndExplData = nullptr;
					mPrevSigBlockIndExplData = nullptr;
					mUseSigBlockIndExplData = nullptr;

					mSigBlockFollowSet = nullptr;
					mUseSigBlockFollowSet = nullptr;
					mPrevSigBlockFollowSet = nullptr;

					mLastConceptCountCachedBlockerCandidate = 0;
					mLastConceptCountSearchBlockerCandidate = 0;
					mLastSearchBlockerCandidateCount = 0;
					mLastSearchBlockerCandidateSignature = 0;
					mBlockingCachingSavedCandidateCount = 0;

					indiAncDepth = 0;
					mNominalLevel = 0;
					mMergeIntoID = 0;
					mIndiID = 0;
					mNomIndi = nullptr;
					mMergedDepTrackPoint = nullptr;

					mInvalidSignatureBlocking = false;
					mProcessingQueued = false;
					mExtendedQueueProcessing = false;
					mImmediatelyProcessingQueued = false;
					mDetExpProcessingQueued = false;
					mDepthProcessingQueued = false;
					mBlockedReactProcessingQueued = false;
					mDelayedNominalProcessingQueued = false;
					mNominalProcessingDelayingChecked = false;
					mBackendSynchronRetestProcessingQueued = false;
					mIncrementalCompatibilityCheckingQueued = false;
					mIncrementalExpansionQueued = false;

					mAssertionInitialisationSignatureValue = 0;

					mProcessingBlockedIndi = nullptr;
					mProcessingBlockedIndividualsLinker = nullptr;
					mSuccessorIndiNodeBackwardDependencyLinker = nullptr;
					mBackwardDependencyToAncestorIndividualNode = false;
					mIndiType = BLOCKABLEINDIVIDUALTYPE;

					mProcessingRestrictionFlags = 0;

					mRoleBackPropHash = nullptr;
					mIndiProcessLinker = nullptr;
					mConceptProcessLinker = nullptr;
					mRequiredBackProp = false;
					mSubstituteIndiNode = nullptr;
					setLocalizationTag(mProcessContext->getUsedProcessTagger());

					mDebugBlockerLastConceptDes = nullptr;
					mCachingLossNodeReactivationInstalled = false;

					mLocReactivationData = nullptr;
					mUseReactivationData = nullptr;

					mLocNominalConnectionSet = nullptr;
					mUseNominalConnectionSet = nullptr;
					mUseDatatypesValueSpaceData = nullptr;
					mUseIncExpData = nullptr;
					mIncExpID = 0;
					mBlockerIndiNode = nullptr;
					mFollowingIndiNode = nullptr;
					mLocDatatypesValueSpaceData = nullptr;
					mLocIncExpData = nullptr;
					mLocIndividualMergingHash = nullptr;
					mUseIndividualMergingHash = nullptr;
					mRoleAssertionCreationID = 0;
					mLastMergedIntoIndividualNode = nullptr;
				}


				CIndividualProcessNode* CIndividualProcessNode::initIndividualProcessNode(CIndividualProcessNode* prevIndividual) {
					mRoleBackPropHash = nullptr;
					mIndiProcessLinker = nullptr;
					mConceptProcessLinker = nullptr;
					mRequiredBackProp = false;
					mSubstituteIndiNode = nullptr;
					mLocReactivationData = nullptr;
					mLocNominalConnectionSet = nullptr;
					mLocDatatypesValueSpaceData = nullptr;
					mLocIncExpData = nullptr;
					mLocIndividualMergingHash = nullptr;
					mBackendSyncData = nullptr;
					setBlockedTestTag(prevIndividual->getBlockedTestTag());
					mPrevIndividual = prevIndividual;
					mDebugBlockerLastConceptDes = prevIndividual->mDebugBlockerLastConceptDes;
					mPrevConceptProcessingQueue = prevIndividual->mUseConceptProcessingQueue;
					mUseConceptProcessingQueue = mPrevConceptProcessingQueue;
					mPrevReapplyConLabelSet = prevIndividual->mUseReapplyConLabelSet;
					mUseReapplyConLabelSet = mPrevReapplyConLabelSet;
					mPrevReapplyRoleSuccHash = prevIndividual->mUseReapplyRoleSuccHash;
					mUseReapplyRoleSuccHash = mPrevReapplyRoleSuccHash;
					mLastAddedLink = prevIndividual->mLastAddedLink;
					mPrevConceptPropBindingSetHash = prevIndividual->mUseConceptPropBindingSetHash;
					mUseConceptPropBindingSetHash = mPrevConceptPropBindingSetHash;
					mPrevConceptVarBindPathSetHash = prevIndividual->mUseConceptVarBindPathSetHash;
					mUseConceptVarBindPathSetHash = mPrevConceptVarBindPathSetHash;
					mPrevConceptRepPropSetHash = prevIndividual->mUseConceptRepPropSetHash;
					mUseConceptRepPropSetHash = mPrevConceptRepPropSetHash;
					mPrevSuccRoleHash = prevIndividual->mUseSuccRoleHash;
					mUseSuccRoleHash = mPrevSuccRoleHash;
					mPrevDisjointSuccRoleHash = prevIndividual->mUseDisjointSuccRoleHash;
					mUseDisjointSuccRoleHash = mPrevDisjointSuccRoleHash;
					mDisjointRoleConnections = prevIndividual->mDisjointRoleConnections;
					mPrevConnSuccSet = prevIndividual->mUseConnSuccSet;
					mUseConnSuccSet = mPrevConnSuccSet;
					mPrevDistinctHash = prevIndividual->mUseDistinctHash;
					mUseDistinctHash = mPrevDistinctHash;
					mInitializingConceptLinkerIt = prevIndividual->mInitializingConceptLinkerIt;
					mProcessInitializingConceptLinkerIt = prevIndividual->mProcessInitializingConceptLinkerIt;
					mAssertionRoleLinkerIt = prevIndividual->mAssertionRoleLinkerIt;
					mReverseAssertionRoleLinkerIt = prevIndividual->mReverseAssertionRoleLinkerIt;
					mAdditionalRoleAssertionsLinker = prevIndividual->mAdditionalRoleAssertionsLinker;
					mRoleAssertionsInitialized = prevIndividual->mRoleAssertionsInitialized;
					mReverseRoleAssertionsInitialized = prevIndividual->mReverseRoleAssertionsInitialized;
					mBlockedIndividualsLinker = prevIndividual->mBlockedIndividualsLinker;
					mSuccessorIndiNodeBackwardDependencyLinker = prevIndividual->mSuccessorIndiNodeBackwardDependencyLinker;
					mBackwardDependencyToAncestorIndividualNode = prevIndividual->mBackwardDependencyToAncestorIndividualNode;
					mAncestorLink = prevIndividual->mAncestorLink;
					indiModel = prevIndividual->indiModel;
					mPrevIndiBlock = prevIndividual->mPrevIndiBlock;
					mPrevIndiSatBlockData = prevIndividual->mPrevIndiSatBlockData;
					mPrevIndiUnsatCacheRet = prevIndividual->mPrevIndiUnsatCacheRet;
					mPrevSigBlockConExpData = prevIndividual->mPrevSigBlockConExpData;
					mPrevReusingConExpData = prevIndividual->mPrevReusingConExpData;
					mPrevSatCacheRetData = prevIndividual->mPrevSatCacheRetData;
					mPrevBackendSyncData = prevIndividual->mPrevBackendSyncData;
					mPrevSigBlockIndExplData = prevIndividual->mUseSigBlockIndExplData;
					mUseSigBlockIndExplData = mPrevSigBlockIndExplData;
					mPrevSigBlockFollowSet = prevIndividual->mUseSigBlockFollowSet;
					mUseSigBlockFollowSet = mPrevSigBlockFollowSet;
					mPrevSatCacheStoringData = prevIndividual->mPrevSatCacheStoringData;
					indiAncDepth = prevIndividual->indiAncDepth;
					mNominalLevel = prevIndividual->mNominalLevel;
					mMergeIntoID = prevIndividual->mMergeIntoID;
					mMergedDepTrackPoint = prevIndividual->mMergedDepTrackPoint;
					mIndiID = prevIndividual->mIndiID;
					mIndiType = prevIndividual->mIndiType;
					mProcessingBlockedIndi = prevIndividual->mProcessingBlockedIndi;
					mProcessingBlockedIndividualsLinker = prevIndividual->mProcessingBlockedIndividualsLinker;
					mInitConceptDescriptor = prevIndividual->mInitConceptDescriptor;
					mProcessingRestrictionFlags = prevIndividual->mProcessingRestrictionFlags;
					mNomIndi = prevIndividual->mNomIndi;
					mInvalidSignatureBlocking = prevIndividual->mInvalidSignatureBlocking;
					mProcessingQueued = prevIndividual->mProcessingQueued;
					mExtendedQueueProcessing = prevIndividual->mExtendedQueueProcessing;
					mImmediatelyProcessingQueued = prevIndividual->mImmediatelyProcessingQueued;
					mDetExpProcessingQueued = prevIndividual->mDetExpProcessingQueued;
					mDepthProcessingQueued = prevIndividual->mDepthProcessingQueued;
					mBlockedReactProcessingQueued = prevIndividual->mBlockedReactProcessingQueued;
					mBackendSynchronRetestProcessingQueued = prevIndividual->mBackendSynchronRetestProcessingQueued;
					mIncrementalCompatibilityCheckingQueued = prevIndividual->mIncrementalCompatibilityCheckingQueued;
					mIncrementalExpansionQueued = prevIndividual->mIncrementalExpansionQueued;
					mDelayedNominalProcessingQueued = prevIndividual->mDelayedNominalProcessingQueued;
					mNominalProcessingDelayingChecked = prevIndividual->mNominalProcessingDelayingChecked;
					mAssertionInitialisationSignatureValue = prevIndividual->mAssertionInitialisationSignatureValue;
					mLastProcessingPriority = prevIndividual->mLastProcessingPriority;
					mDependencyTrackPoint = prevIndividual->mDependencyTrackPoint;
					mSatCachedAbsorbedDisjunctionsReapplyConDes = prevIndividual->mSatCachedAbsorbedDisjunctionsReapplyConDes;
					mSatCachedAbsorbedSuccessorReapplyConDes = prevIndividual->mSatCachedAbsorbedSuccessorReapplyConDes;
					mLastConceptCountCachedBlockerCandidate = prevIndividual->mLastConceptCountCachedBlockerCandidate;
					mLastConceptCountSearchBlockerCandidate = prevIndividual->mLastConceptCountSearchBlockerCandidate;
					mBlockingCachingSavedCandidateCount = prevIndividual->mBlockingCachingSavedCandidateCount;
					mLastSearchBlockerCandidateCount = prevIndividual->mLastSearchBlockerCandidateCount;
					mLastSearchBlockerCandidateSignature = prevIndividual->mLastSearchBlockerCandidateSignature;
					mCachingLossNodeReactivationInstalled = prevIndividual->mCachingLossNodeReactivationInstalled;
					prevIndividual->setRelocalized();
					mUseReactivationData = prevIndividual->mUseReactivationData;
					mUseNominalConnectionSet = prevIndividual->mUseNominalConnectionSet;
					mUseDatatypesValueSpaceData = prevIndividual->mUseDatatypesValueSpaceData;
					mUseIncExpData = prevIndividual->mUseIncExpData;
					mIncExpID = prevIndividual->mIncExpID;
					mRoleAssertionCreationID = prevIndividual->mRoleAssertionCreationID;
					mUseIndividualMergingHash = prevIndividual->mUseIndividualMergingHash;
					mBlockerIndiNode = prevIndividual->mBlockerIndiNode;
					mFollowingIndiNode = prevIndividual->mFollowingIndiNode;
					mLastMergedIntoIndividualNode = prevIndividual->mLastMergedIntoIndividualNode;
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::initIndividualProcessNodeCopy(CIndividualProcessNode* prevIndividual, bool adobtConceptLabels, bool adobtRoleSuccessors, bool adobtStatus) {
					mRoleBackPropHash = nullptr;
					mIndiProcessLinker = nullptr;
					mConceptProcessLinker = nullptr;
					mRequiredBackProp = false;
					mSubstituteIndiNode = nullptr;
					mLocReactivationData = nullptr;
					mLocNominalConnectionSet = nullptr;
					mLocDatatypesValueSpaceData = nullptr;
					mLocIncExpData = nullptr;
					mLocIndividualMergingHash = nullptr;
					prevIndividual->setRelocalized();
					if (adobtStatus) {
						setBlockedTestTag(prevIndividual->getBlockedTestTag());
					}
					mBackendSyncData = nullptr;
					mPrevIndividual = prevIndividual;
					mPrevConceptProcessingQueue = prevIndividual->mUseConceptProcessingQueue;
					mUseConceptProcessingQueue = mPrevConceptProcessingQueue;
					mDependencyTrackPoint = prevIndividual->mDependencyTrackPoint;
					mDebugBlockerLastConceptDes = prevIndividual->mDebugBlockerLastConceptDes;
					mCachingLossNodeReactivationInstalled = prevIndividual->mCachingLossNodeReactivationInstalled;
					mLastMergedIntoIndividualNode = prevIndividual->mLastMergedIntoIndividualNode;

					if (adobtConceptLabels) {
						mPrevReapplyConLabelSet = prevIndividual->mUseReapplyConLabelSet;
						mUseReapplyConLabelSet = mPrevReapplyConLabelSet;
					}
					if (adobtRoleSuccessors) {
						mPrevReapplyRoleSuccHash = prevIndividual->mUseReapplyRoleSuccHash;
						mUseReapplyRoleSuccHash = mPrevReapplyRoleSuccHash;
						mLastAddedLink = prevIndividual->mLastAddedLink;
						mPrevConceptPropBindingSetHash = prevIndividual->mUseConceptPropBindingSetHash;
						mUseConceptPropBindingSetHash = mPrevConceptPropBindingSetHash;
						mPrevConceptVarBindPathSetHash = prevIndividual->mUseConceptVarBindPathSetHash;
						mUseConceptVarBindPathSetHash = mPrevConceptVarBindPathSetHash;
						mPrevConceptRepPropSetHash = prevIndividual->mUseConceptRepPropSetHash;
						mUseConceptRepPropSetHash = mPrevConceptRepPropSetHash;
						mPrevSuccRoleHash = prevIndividual->mUseSuccRoleHash;
						mUseSuccRoleHash = mPrevSuccRoleHash;
						mPrevDisjointSuccRoleHash = prevIndividual->mUseDisjointSuccRoleHash;
						mUseDisjointSuccRoleHash = mPrevDisjointSuccRoleHash;
						mDisjointRoleConnections = prevIndividual->mDisjointRoleConnections;
						mPrevConnSuccSet = prevIndividual->mUseConnSuccSet;
						mUseConnSuccSet = mPrevConnSuccSet;
						mPrevDistinctHash = prevIndividual->mUseDistinctHash;
						mUseDistinctHash = mPrevDistinctHash;
						mUseReactivationData = prevIndividual->mUseReactivationData;
						mUseNominalConnectionSet = prevIndividual->mUseNominalConnectionSet;
						mNominalProcessingDelayingChecked = prevIndividual->mNominalProcessingDelayingChecked;
						mUseDatatypesValueSpaceData = prevIndividual->mUseDatatypesValueSpaceData;
						mUseIndividualMergingHash = prevIndividual->mUseIndividualMergingHash;
					}
					if (adobtStatus) {
						mRoleAssertionCreationID = prevIndividual->mRoleAssertionCreationID;
						mIncExpID = prevIndividual->mIncExpID;
						mUseIncExpData = prevIndividual->mUseIncExpData;
						mFollowingIndiNode = prevIndividual->mFollowingIndiNode;
						mBlockerIndiNode = prevIndividual->mBlockerIndiNode;
						mInitializingConceptLinkerIt = prevIndividual->mInitializingConceptLinkerIt;
						mProcessInitializingConceptLinkerIt = prevIndividual->mProcessInitializingConceptLinkerIt;
						mAssertionRoleLinkerIt = prevIndividual->mAssertionRoleLinkerIt;
						mReverseAssertionRoleLinkerIt = prevIndividual->mReverseAssertionRoleLinkerIt;
						mAdditionalRoleAssertionsLinker = prevIndividual->mAdditionalRoleAssertionsLinker;
						mRoleAssertionsInitialized = prevIndividual->mRoleAssertionsInitialized;
						mReverseRoleAssertionsInitialized = prevIndividual->mReverseRoleAssertionsInitialized;
						mBlockedIndividualsLinker = prevIndividual->mBlockedIndividualsLinker;
						mSuccessorIndiNodeBackwardDependencyLinker = prevIndividual->mSuccessorIndiNodeBackwardDependencyLinker;
						mBackwardDependencyToAncestorIndividualNode = prevIndividual->mBackwardDependencyToAncestorIndividualNode;
						mAncestorLink = prevIndividual->mAncestorLink;
						indiModel = prevIndividual->indiModel;
						mPrevIndiBlock = prevIndividual->mPrevIndiBlock;
						mPrevIndiSatBlockData = prevIndividual->mPrevIndiSatBlockData;
						mPrevIndiUnsatCacheRet = prevIndividual->mPrevIndiUnsatCacheRet;
						mPrevSigBlockConExpData = prevIndividual->mPrevSigBlockConExpData;
						mPrevReusingConExpData = prevIndividual->mPrevReusingConExpData;
						mPrevSatCacheRetData = prevIndividual->mPrevSatCacheRetData;
						mPrevBackendSyncData = prevIndividual->mPrevBackendSyncData;
						mPrevSigBlockIndExplData = prevIndividual->mUseSigBlockIndExplData;
						mUseSigBlockIndExplData = mPrevSigBlockIndExplData;
						mPrevSigBlockFollowSet = prevIndividual->mUseSigBlockFollowSet;
						mUseSigBlockFollowSet = mPrevSigBlockFollowSet;
						mPrevSatCacheStoringData = prevIndividual->mPrevSatCacheStoringData;
						mProcessingBlockedIndi = prevIndividual->mProcessingBlockedIndi;
						mProcessingBlockedIndividualsLinker = prevIndividual->mProcessingBlockedIndividualsLinker;
						mInitConceptDescriptor = prevIndividual->mInitConceptDescriptor;
						mProcessingRestrictionFlags = prevIndividual->mProcessingRestrictionFlags;
						mIndiID = prevIndividual->mIndiID;
						mMergeIntoID = prevIndividual->mMergeIntoID;
						mMergedDepTrackPoint = prevIndividual->mMergedDepTrackPoint;
						mNomIndi = prevIndividual->mNomIndi;
						mSatCachedAbsorbedDisjunctionsReapplyConDes = prevIndividual->mSatCachedAbsorbedDisjunctionsReapplyConDes;
						mSatCachedAbsorbedSuccessorReapplyConDes = prevIndividual->mSatCachedAbsorbedSuccessorReapplyConDes;
						mLastConceptCountCachedBlockerCandidate = prevIndividual->mLastConceptCountCachedBlockerCandidate;
						mLastConceptCountSearchBlockerCandidate = prevIndividual->mLastConceptCountSearchBlockerCandidate;
						mBlockingCachingSavedCandidateCount = prevIndividual->mBlockingCachingSavedCandidateCount;
						mLastSearchBlockerCandidateCount = prevIndividual->mLastSearchBlockerCandidateCount;
						mLastSearchBlockerCandidateSignature = prevIndividual->mLastSearchBlockerCandidateSignature;
						mInvalidSignatureBlocking = prevIndividual->mInvalidSignatureBlocking;
						mProcessingQueued = prevIndividual->mProcessingQueued;
						mExtendedQueueProcessing = prevIndividual->mExtendedQueueProcessing;
						mLastProcessingPriority = prevIndividual->mLastProcessingPriority;
						mImmediatelyProcessingQueued = prevIndividual->mImmediatelyProcessingQueued;
						mDetExpProcessingQueued = prevIndividual->mDetExpProcessingQueued;
						mDepthProcessingQueued = prevIndividual->mDepthProcessingQueued;
						mBlockedReactProcessingQueued = prevIndividual->mBlockedReactProcessingQueued;
						mBackendSynchronRetestProcessingQueued = prevIndividual->mBackendSynchronRetestProcessingQueued;
						mIncrementalCompatibilityCheckingQueued = prevIndividual->mIncrementalCompatibilityCheckingQueued;
						mIncrementalExpansionQueued = prevIndividual->mIncrementalExpansionQueued;
						mDelayedNominalProcessingQueued = prevIndividual->mDelayedNominalProcessingQueued;
						mAssertionInitialisationSignatureValue = prevIndividual->mAssertionInitialisationSignatureValue;
					}
					indiAncDepth = prevIndividual->indiAncDepth;
					mNominalLevel = prevIndividual->mNominalLevel;
					mIndiType = prevIndividual->mIndiType;
					return this;
				}



				CIndividualProcessNode* CIndividualProcessNode::getProcessingBlockTestIndividual() {
					return mProcessingBlockedIndi;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearProcessingBlockTestIndividual() {
					mProcessingBlockedIndi = nullptr;
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::setProcessingBlockTestIndividual(CIndividualProcessNode* processingBlockTestIndi) {
					mProcessingBlockedIndi = processingBlockTestIndi;
					return this;
				}


				bool CIndividualProcessNode::hasInitializingConcepts() {
					return mInitializingConceptLinkerIt != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearProcessInitializingConcepts() {
					mProcessInitializingConceptLinkerIt = nullptr;
					return this;
				}


				CXSortedNegLinker<CConcept*>* CIndividualProcessNode::getProcessInitializingConceptLinker() {
					return mProcessInitializingConceptLinkerIt;
				}

				CXSortedNegLinker<CConcept*>* CIndividualProcessNode::getInitializingConceptLinker() {
					return mInitializingConceptLinkerIt;
				}

				CIndividualProcessNode* CIndividualProcessNode::setInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinkerIt) {
					mInitializingConceptLinkerIt = initializingConceptLinkerIt;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::addInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinkerIt) {
					if (initializingConceptLinkerIt) {
						mInitializingConceptLinkerIt = initializingConceptLinkerIt->append(mInitializingConceptLinkerIt);
						mProcessInitializingConceptLinkerIt = mInitializingConceptLinkerIt;
					}
					return this;
				}

				bool CIndividualProcessNode::hasAssertionConcepts() {
					return mAssertionConceptLinkerIt != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearAssertionConcepts() {
					mAssertionConceptLinkerIt = nullptr;
					return this;
				}


				CConceptAssertionLinker* CIndividualProcessNode::getAssertionConceptLinker() {
					return mAssertionConceptLinkerIt;
				}

				CIndividualProcessNode* CIndividualProcessNode::setAssertionConceptLinker(CConceptAssertionLinker* assertionConceptLinkerIt) {
					mAssertionConceptLinkerIt = assertionConceptLinkerIt;
					return this;
				}




				bool CIndividualProcessNode::hasAssertionRoles() {
					return mAssertionRoleLinkerIt != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearAssertionRoles() {
					mAssertionRoleLinkerIt = nullptr;
					return this;
				}

				CRoleAssertionLinker* CIndividualProcessNode::getAssertionRoleLinker() {
					return mAssertionRoleLinkerIt;
				}

				CIndividualProcessNode* CIndividualProcessNode::setAssertionRoleLinker(CRoleAssertionLinker* assertionRoleLinkerIt) {
					mAssertionRoleLinkerIt = assertionRoleLinkerIt;
					return this;
				}



				cint64 CIndividualProcessNode::getRoleAssertionCreationID() {
					return mRoleAssertionCreationID;
				}

				CIndividualProcessNode* CIndividualProcessNode::setRoleAssertionCreationID(cint64 creationID) {
					mRoleAssertionCreationID = creationID;
					return this;
				}



				bool CIndividualProcessNode::hasReverseAssertionRoles() {
					return mReverseAssertionRoleLinkerIt != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearReverseAssertionRoles() {
					mReverseAssertionRoleLinkerIt = nullptr;
					return this;
				}

				CReverseRoleAssertionLinker* CIndividualProcessNode::getReverseAssertionRoleLinker() {
					return mReverseAssertionRoleLinkerIt;
				}

				CIndividualProcessNode* CIndividualProcessNode::setReverseAssertionRoleLinker(CReverseRoleAssertionLinker* reverseAssertionRoleLinkerIt) {
					mReverseAssertionRoleLinkerIt = reverseAssertionRoleLinkerIt;
					return this;
				}








				bool CIndividualProcessNode::hasAdditionalRoleAssertionsLinker() {
					return mAdditionalRoleAssertionsLinker != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearAdditionalRoleAssertionsLinker() {
					mAdditionalRoleAssertionsLinker = nullptr;
					return this;
				}

				CAdditionalProcessRoleAssertionsLinker* CIndividualProcessNode::getAdditionalRoleAssertionsLinker() {
					return mAdditionalRoleAssertionsLinker;
				}

				CIndividualProcessNode* CIndividualProcessNode::setAdditionalRoleAssertionsLinker(CAdditionalProcessRoleAssertionsLinker* reverseRoleAssertionsLinker) {
					mAdditionalRoleAssertionsLinker = reverseRoleAssertionsLinker;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::addAdditionalRoleAssertionsLinker(CAdditionalProcessRoleAssertionsLinker* reverseRoleAssertionsLinker) {
					mAdditionalRoleAssertionsLinker = reverseRoleAssertionsLinker->append(mAdditionalRoleAssertionsLinker);
					return this;
				}





				bool CIndividualProcessNode::hasRoleAssertionsInitialized() {
					return mRoleAssertionsInitialized;
				}

				CIndividualProcessNode* CIndividualProcessNode::setRoleAssertionsInitialized(bool initialized) {
					mRoleAssertionsInitialized = initialized;
					return this;
				}



				bool CIndividualProcessNode::hasReverseRoleAssertionsInitialized() {
					return mReverseRoleAssertionsInitialized;
				}

				CIndividualProcessNode* CIndividualProcessNode::setReverseRoleAssertionsInitialized(bool initialized) {
					mReverseRoleAssertionsInitialized = initialized;
					return this;
				}


				cint64 CIndividualProcessNode::getIndividualID() {
					return mIndiID;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualID(cint64 indiID) {
					if (mMergeIntoID == mIndiID) {
						mMergeIntoID = indiID;
					}
					mIndiID = indiID;
					return this;
				}

				CIndividualProcessNode::CIndividualType CIndividualProcessNode::getIndividualType() {
					return mIndiType;
				}


				CIndividualProcessNode* CIndividualProcessNode::setIndividualType(CIndividualProcessNode::CIndividualType indiType) {
					mIndiType = indiType;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setReapplyConceptLabelSet(CReapplyConceptLabelSet* reapplyConSet) {
					mUseReapplyConLabelSet = reapplyConSet;
					mReapplyConLabelSet = reapplyConSet;
					mPrevReapplyConLabelSet = reapplyConSet;
					return this;
				}

				CReapplyConceptLabelSet* CIndividualProcessNode::getReapplyConceptLabelSet(bool create) {
					if (create && !mReapplyConLabelSet) {
						mReapplyConLabelSet = CObjectParameterizingAllocator< CReapplyConceptLabelSet,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mReapplyConLabelSet->initConceptLabelSet(mPrevReapplyConLabelSet);
						mUseReapplyConLabelSet = mReapplyConLabelSet;
					}
					return mUseReapplyConLabelSet;
				}



				CConnectionSuccessorSet* CIndividualProcessNode::getConnectionSuccessorSet(bool create) {
					if (create && !mConnSuccSet) {
						mConnSuccSet = CObjectParameterizingAllocator< CConnectionSuccessorSet,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mConnSuccSet->initConnectionSuccessorSet(mPrevConnSuccSet);
						mUseConnSuccSet = mConnSuccSet;
					}
					return mUseConnSuccSet;
				}

				CReapplyRoleSuccessorHash* CIndividualProcessNode::getReapplyRoleSuccessorHash(bool create) {
					if (create && !mReapplyRoleSuccHash) {
						mReapplyRoleSuccHash = CObjectParameterizingAllocator< CReapplyRoleSuccessorHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mReapplyRoleSuccHash->initRoleSuccessorHash(mPrevReapplyRoleSuccHash);
						mUseReapplyRoleSuccHash = mReapplyRoleSuccHash;
					}
					return mUseReapplyRoleSuccHash;
				}


				CConceptPropagationBindingSetHash* CIndividualProcessNode::getConceptPropagationBindingSetHash(bool create) {
					if (create && !mConceptPropBindingSetHash) {
						mConceptPropBindingSetHash = CObjectParameterizingAllocator< CConceptPropagationBindingSetHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mConceptPropBindingSetHash->initConceptPropagationBindingSetHash(mPrevConceptPropBindingSetHash);
						mUseConceptPropBindingSetHash = mConceptPropBindingSetHash;
					}
					return mUseConceptPropBindingSetHash;
				}

				CConceptVariableBindingPathSetHash* CIndividualProcessNode::getConceptVariableBindingPathSetHash(bool create) {
					if (create && !mConceptVarBindPathSetHash) {
						mConceptVarBindPathSetHash = CObjectParameterizingAllocator< CConceptVariableBindingPathSetHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mConceptVarBindPathSetHash->initConceptVariableBindingPathSetHash(mPrevConceptVarBindPathSetHash);
						mUseConceptVarBindPathSetHash = mConceptVarBindPathSetHash;
					}
					return mUseConceptVarBindPathSetHash;
				}

				CConceptRepresentativePropagationSetHash* CIndividualProcessNode::getConceptRepresentativePropagationSetHash(bool create) {
					if (create && !mConceptRepPropSetHash) {
						mConceptRepPropSetHash = CObjectParameterizingAllocator< CConceptRepresentativePropagationSetHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mConceptRepPropSetHash->initConceptRepresentativePropagationSetHash(mPrevConceptRepPropSetHash);
						mUseConceptRepPropSetHash = mConceptRepPropSetHash;
					}
					return mUseConceptRepPropSetHash;
				}


				CRoleSuccessorLinkIterator CIndividualProcessNode::getRoleSuccessorLinkIterator(CRole* role) {
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->getRoleSuccessorLinkIterator(role);
					} else {
						return CRoleSuccessorLinkIterator();
					}
				}

				cint64 CIndividualProcessNode::getRoleSuccessorCount(CRole* role) {
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->getRoleSuccessorCount(role);
					} else {
						return 0;
					}
				}


				CRoleSuccessorLinkIterator CIndividualProcessNode::getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink) {
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->getRoleSuccessorHistoryLinkIterator(role,lastLink);
					} else {
						return CRoleSuccessorLinkIterator();
					}
				}


				bool CIndividualProcessNode::hasRoleSuccessorToIndividual(CRole* role, cint64 destinationIndiID, bool locateable) {
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->hasRoleSuccessorToIndividual(role,getIndividualID(),destinationIndiID,locateable && mReapplyRoleSuccHash);
					}
					return false;
				}


				bool CIndividualProcessNode::hasRoleSuccessorToIndividual(CRole* role, CIndividualProcessNode* desIndi, bool locateable) {
					return hasRoleSuccessorToIndividual(role,desIndi->getIndividualID(),locateable);
				}

				CIndividualLinkEdge* CIndividualProcessNode::getRoleSuccessorToIndividualLink(CRole* role, cint64 destinationIndiID, bool locateable) {
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->getRoleSuccessorToIndividualLink(role,getIndividualID(),destinationIndiID,locateable && mReapplyRoleSuccHash);
					}
					return false;
				}

				CIndividualLinkEdge* CIndividualProcessNode::getRoleSuccessorToIndividualLink(CRole* role, CIndividualProcessNode* desIndi, bool locateable) {
					return getRoleSuccessorToIndividualLink(role,desIndi->getIndividualID(),locateable);
				}



				bool CIndividualProcessNode::hasNegationDisjointToIndividual(CRole* role, cint64 destinationIndiID) {
					if (mUseDisjointSuccRoleHash) {
						return mUseDisjointSuccRoleHash->hasDisjointSuccessorRoleLink(destinationIndiID,role);
					}
					return false;
				}


				bool CIndividualProcessNode::hasNegationDisjointToIndividual(CRole* role, CIndividualProcessNode* desIndi) {
					return hasNegationDisjointToIndividual(role,desIndi->getIndividualID());
				}



				bool CIndividualProcessNode::hasDisjointRoleConnections() {
					return mDisjointRoleConnections;
				}

				CIndividualProcessNode* CIndividualProcessNode::setDisjointRoleConnections(bool disjointRoleConnections) {
					mDisjointRoleConnections = disjointRoleConnections;
					return this;
				}



				CDisjointSuccessorRoleHash* CIndividualProcessNode::getDisjointSuccessorRoleHash(bool create) {
					if (create && !mDisjointSuccRoleHash) {
						mDisjointSuccRoleHash = CObjectParameterizingAllocator< CDisjointSuccessorRoleHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mDisjointSuccRoleHash->initDisjointSuccessorRoleHash(mPrevDisjointSuccRoleHash);
						mUseDisjointSuccRoleHash = mDisjointSuccRoleHash;
					}
					return mUseDisjointSuccRoleHash;
				}


				CIndividualProcessNode* CIndividualProcessNode::installDisjointLink(CNegationDisjointEdge* link) {
					if (!mDisjointSuccRoleHash) {
						getDisjointSuccessorRoleHash(true);
					}
					mUseDisjointSuccRoleHash->insertDisjointSuccessorRoleLink(link->getOppositeIndividualID(mIndiID),link);
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::removeDisjointLinks(cint64 succIndiID) {
					if (mUseDisjointSuccRoleHash) {
						if (!mDisjointSuccRoleHash) {
							getDisjointSuccessorRoleHash(true);
						}
						mUseDisjointSuccRoleHash->removeDisjointSuccessorRoleLinks(succIndiID);
					}
					return this;
				}



				CDisjointSuccessorRoleIterator CIndividualProcessNode::getDisjointSuccessorRoleIterator(cint64 succIndiId) {
					if (mUseDisjointSuccRoleHash) {
						return mUseDisjointSuccRoleHash->getDisjointRoleIterator(succIndiId);
					} else {
						return CDisjointSuccessorRoleIterator();
					}
				}

				CDisjointSuccessorRoleIterator CIndividualProcessNode::getDisjointSuccessorRoleIterator(CIndividualProcessNode* indiNode) {
					return getDisjointSuccessorRoleIterator(indiNode->getIndividualID());
				}


				CRoleSuccessorIterator CIndividualProcessNode::getRoleIterator() {
					if (!mUseReapplyRoleSuccHash) {
						return CRoleSuccessorIterator();
					} else {
						return mUseReapplyRoleSuccHash->getRoleIterator();
					}
				}


				CSuccessorRoleHash* CIndividualProcessNode::getSuccessorRoleHash(bool create) {
					if (create && !mSuccRoleHash) {
						mSuccRoleHash = CObjectParameterizingAllocator< CSuccessorRoleHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mSuccRoleHash->initSuccessorRoleHash(mPrevSuccRoleHash);
						mUseSuccRoleHash = mSuccRoleHash;
					}
					return mUseSuccRoleHash;
				}

				CSuccessorRoleIterator CIndividualProcessNode::getSuccessorRoleIterator(CIndividualProcessNode* indiNode) {
					return getSuccessorRoleIterator(indiNode->getIndividualID());
				}

				CSuccessorRoleIterator CIndividualProcessNode::getSuccessorRoleIterator(cint64 indiID) {
					if (!mUseSuccRoleHash) {
						return CSuccessorRoleIterator();
					} else {
						return mUseSuccRoleHash->getSuccessorRoleIterator(indiID);
					}
				}



				bool CIndividualProcessNode::hasSuccessorIndividualNode(CIndividualProcessNode* indiNode) {
					return hasSuccessorIndividualNode(indiNode->getIndividualID());
				}

				bool CIndividualProcessNode::hasSuccessorIndividualNode(cint64 indiID) {
					if (!mUseSuccRoleHash) {
						return false;
					} else {
						return mUseSuccRoleHash->hasSuccessorIndividualNode(indiID);
					}
				}




				CConceptProcessingQueue* CIndividualProcessNode::getConceptProcessingQueue(bool create) {
					if (create && !mConceptProcessingQueue) {
						mConceptProcessingQueue = CObjectParameterizingAllocator< CConceptProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mConceptProcessingQueue->initProcessingQueue(mPrevConceptProcessingQueue);
						mUseConceptProcessingQueue = mConceptProcessingQueue;
					}
					return mUseConceptProcessingQueue;
				}




				CReapplyQueue* CIndividualProcessNode::getRoleReapplyQueue(CRole* role, bool create) {
					CReapplyQueue* reapplyQueue = nullptr;
					if (create && !mReapplyRoleSuccHash) {
						getReapplyRoleSuccessorHash(true);
					}
					if (mUseReapplyRoleSuccHash) {
						reapplyQueue = mUseReapplyRoleSuccHash->getRoleReapplyQueue(role,create);
					}
					return reapplyQueue;
				}


				CReapplyQueueIterator CIndividualProcessNode::getRoleReapplyIterator(CRole* role, bool clearDynamicReapllyQueue) {
					if (clearDynamicReapllyQueue && !mReapplyRoleSuccHash) {
						getReapplyRoleSuccessorHash(true);
					}
					if (mUseReapplyRoleSuccHash) {
						return mUseReapplyRoleSuccHash->getRoleReapplyIterator(role,clearDynamicReapllyQueue);
					} else {
						return CReapplyQueueIterator(nullptr,nullptr);
					}
				}

				CCondensedReapplyQueue* CIndividualProcessNode::getConceptReapplyQueue(CConcept* concept, bool conceptNegation, bool create) {
					CCondensedReapplyQueue* reapplyQueue = 0;
					if (create && !mReapplyConLabelSet) {
						getReapplyConceptLabelSet(true);
					}
					if (mUseReapplyConLabelSet) {
						reapplyQueue = mUseReapplyConLabelSet->getConceptReapplyQueue(concept,conceptNegation,create);
					}
					return reapplyQueue;
				}


				CCondensedReapplyQueueIterator CIndividualProcessNode::getConceptReapplyIterator(CConcept* concept, bool conceptNegation, bool clearDynamicReapllyQueue) {
					if (clearDynamicReapllyQueue && !mReapplyConLabelSet) {
						getReapplyConceptLabelSet(true);
					}
					if (mUseReapplyConLabelSet) {
						return mUseReapplyConLabelSet->getConceptReapplyIterator(concept,conceptNegation,clearDynamicReapllyQueue);
					} else {
						return CCondensedReapplyQueueIterator(nullptr,conceptNegation);
					}
				}




				CDistinctHash* CIndividualProcessNode::getDistinctHash(bool create) {
					if (create && !mDistinctHash) {
						mDistinctHash = CObjectParameterizingAllocator< CDistinctHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mDistinctHash->initDistinctHash(mPrevDistinctHash);
						mUseDistinctHash = mDistinctHash;
					}
					return mUseDistinctHash;
				}




				CIndividualProcessNode* CIndividualProcessNode::addBlockedIndividualsLinker(CXLinker<CIndividualProcessNode* >* linker) {
					if (linker) {
						mBlockedIndividualsLinker = linker->append(mBlockedIndividualsLinker);
					}
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setBlockedIndividualsLinker(CXLinker<CIndividualProcessNode* >* linker) {
					mBlockedIndividualsLinker = linker;
					return this;
				}

				CXLinker<CIndividualProcessNode* >* CIndividualProcessNode::getBlockedIndividualsLinker() {
					return mBlockedIndividualsLinker;				
				}


				bool CIndividualProcessNode::hasBlockedIndividualsLinker() {
					return mBlockedIndividualsLinker;
				}


				CIndividualProcessNode* CIndividualProcessNode::clearBlockedIndividualsLinker() {
					mBlockedIndividualsLinker = nullptr;
					return this;
				}

				CIndividualLinkEdge* CIndividualProcessNode::getAncestorLink() {
					return mAncestorLink;
				}

				CIndividualProcessNode* CIndividualProcessNode::setAncestorLink(CIndividualLinkEdge* link) {
					mAncestorLink = link;
					return this;
				}

				bool CIndividualProcessNode::isIndividualAncestor(CIndividualProcessNode* individual) {
					if (mAncestorLink) {
						return mAncestorLink->isSourceIndividualID(individual);
					}
					return false;
				}



				bool CIndividualProcessNode::hasIndividualAncestor() {
					return mAncestorLink != nullptr;
				}


				bool CIndividualProcessNode::isBlockableIndividual() {
					return mIndiType == BLOCKABLEINDIVIDUALTYPE;
				}

				bool CIndividualProcessNode::isNominalIndividual() {
					return mIndiType == NOMINALINDIVIDUALTYPE;
				}


				CIndividual* CIndividualProcessNode::getNominalIndividual() {
					return mNomIndi;
				}

				CIndividualProcessNode* CIndividualProcessNode::setNominalIndividual(CIndividual* indi) {
					mNomIndi = indi;
					return this;
				}


				CIndividualLinkEdge* CIndividualProcessNode::getLastAddedRoleLink() {
					return mLastAddedLink;
				}


				CIndividualProcessNode* CIndividualProcessNode::installIndividualLink(CIndividualLinkEdge* link, CReapplyQueueIterator* reapplyQueueIt) {
					if (!mReapplyRoleSuccHash) {
						getReapplyRoleSuccessorHash(true);
					}
					if (!mSuccRoleHash) {
						getSuccessorRoleHash(true);
					}
					mUseReapplyRoleSuccHash->insertRoleSuccessorLink(link->getLinkRole(),link,reapplyQueueIt);
					cint64 oppIndiID = link->getOppositeIndividualID(mIndiID);
					mUseSuccRoleHash->insertSuccessorRoleLink(oppIndiID,link);
					mLastAddedLink = link;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::removeIndividualLink(CIndividualLinkEdge* link) {
					if (!mReapplyRoleSuccHash) {
						getReapplyRoleSuccessorHash(true);
					}
					mUseReapplyRoleSuccHash->removeRoleSuccessorLink(link->getLinkRole(),link);
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::removeIndividualConnection(CIndividualProcessNode* indi) {
					if (!mSuccRoleHash) {
						getSuccessorRoleHash(true);
					}
					mSuccRoleHash->removeSuccessor(indi->getIndividualID());
					if (mUseConnSuccSet) {
						if (!mConnSuccSet) {
							getConnectionSuccessorSet(true);
						}
						mConnSuccSet->removeConnection(indi->getIndividualID());
					}
					return this;
				}



				CSuccessorIterator CIndividualProcessNode::getSuccessorIterator() {
					if (!mUseSuccRoleHash) {
						return CSuccessorIterator();
					}
					return mUseSuccRoleHash->getSuccessorIterator();
				}



				CConnectionSuccessorSetIterator CIndividualProcessNode::getConnectionSuccessorIterator() {
					if (mUseConnSuccSet) {
						return mUseConnSuccSet->getConnectionSuccessorIterator();
					} else {
						return CConnectionSuccessorSetIterator();
					}
				}



				CIndividualNodeModelData* CIndividualProcessNode::getIndividualModelData() {
					return indiModel;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualModelData(CIndividualNodeModelData* individualModel) {
					indiModel = individualModel;
					return this;
				}


				CIndividualNodeSatisfiableCacheRetrievalData* CIndividualProcessNode::getIndividualSatisfiableCacheRetrievalData(bool localCacheData) {
					if (localCacheData) {
						return mSatCacheRetData;
					} else {
						return mPrevSatCacheRetData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualSatisfiableCacheRetrievalData(CIndividualNodeSatisfiableCacheRetrievalData* satCacheRetrievalData) {
					mSatCacheRetData = satCacheRetrievalData;
					mPrevSatCacheRetData = mSatCacheRetData;
					return this;
				}




				CIndividualNodeBackendCacheSynchronisationData* CIndividualProcessNode::getIndividualBackendCacheSynchronisationData(bool localCacheData) {
					if (localCacheData) {
						return mBackendSyncData;
					} else {
						return mPrevBackendSyncData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualBackendCacheSynchronisationData(CIndividualNodeBackendCacheSynchronisationData* backendSyncData) {
					mBackendSyncData = backendSyncData;
					mPrevBackendSyncData = backendSyncData;
					return this;
				}




				CIndividualNodeSatisfiableCacheStoringData* CIndividualProcessNode::getIndividualSatisfiableCacheStoringData(bool localCacheData) {
					if (localCacheData) {
						return mSatCacheStoringData;
					} else {
						return mPrevSatCacheStoringData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualSatisfiableCacheStoringData(CIndividualNodeSatisfiableCacheStoringData* satCacheStoringData) {
					mSatCacheStoringData = satCacheStoringData;
					mPrevSatCacheStoringData = mSatCacheStoringData;
					return this;
				}

				CIndividualNodeUnsatisfiableCacheRetrievalData* CIndividualProcessNode::getIndividualUnsatisfiableCacheRetrievalData(bool localUnsatCacheRetData) {
					if (localUnsatCacheRetData) {
						return mIndiUnsatCacheRet;
					} else {
						return mPrevIndiUnsatCacheRet;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualUnsatisfiableCacheRetrievalData(CIndividualNodeUnsatisfiableCacheRetrievalData* individualUnsatCacheRetData) {
					mIndiUnsatCacheRet = individualUnsatCacheRetData;
					mPrevIndiUnsatCacheRet = mIndiUnsatCacheRet;
					return this;
				}


				CSignatureBlockingIndividualNodeConceptExpansionData* CIndividualProcessNode::getSignatureBlockingIndividualNodeConceptExpansionData(bool localBlockingData) {
					if (localBlockingData) {
						return mSigBlockConExpData;
					} else {
						return mPrevSigBlockConExpData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setSignatureBlockingIndividualNodeConceptExpansionData(CSignatureBlockingIndividualNodeConceptExpansionData* individualBlockingData) {
					mSigBlockConExpData = individualBlockingData;
					mPrevSigBlockConExpData = mSigBlockConExpData;
					return this;
				}


				CReusingIndividualNodeConceptExpansionData* CIndividualProcessNode::getReusingIndividualNodeConceptExpansionData(bool localBlockingData) {
					if (localBlockingData) {
						return mReusingConExpData;
					} else {
						return mPrevReusingConExpData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setReusingIndividualNodeConceptExpansionData(CReusingIndividualNodeConceptExpansionData* individualBlockingData) {
					mReusingConExpData = individualBlockingData;
					mPrevReusingConExpData = mReusingConExpData;
					return this;
				}


				CIndividualNodeAnalizedConceptExpansionData* CIndividualProcessNode::getAnalizedConceptExpansionData(bool createOrLocalize) {
					if (!mSigBlockIndExplData && createOrLocalize) {
						mSigBlockIndExplData = CObjectAllocator<CIndividualNodeAnalizedConceptExpansionData>::allocateAndConstruct(mMemAllocMan);
						mSigBlockIndExplData->initBlockingExplorationData(mPrevSigBlockIndExplData);
						mUseSigBlockIndExplData = mSigBlockIndExplData;
					}
					return mUseSigBlockIndExplData;
				}


				CBlockingFollowSet* CIndividualProcessNode::getBlockingFollowSet(bool createOrLocalize) {
					if (!mSigBlockFollowSet && createOrLocalize) {
						mSigBlockFollowSet = CObjectParameterizingAllocator< CBlockingFollowSet,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
						mSigBlockFollowSet->initBlockingFollowSet(mPrevSigBlockFollowSet);
						mUseSigBlockFollowSet = mSigBlockFollowSet;
					}
					return mUseSigBlockFollowSet;
				}


				bool CIndividualProcessNode::hasBlockingFollower() {
					if (!mUseSigBlockFollowSet) {
						return false;
					} else {
						return !mUseSigBlockFollowSet->empty();
					}
				}


				CIndividualNodeSaturationBlockingData* CIndividualProcessNode::getIndividualSaturationBlockingData(bool localBlockData) {
					if (localBlockData) {
						return mIndiSatBlockData;
					} else {
						return mPrevIndiSatBlockData;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualSaturationBlockingData(CIndividualNodeSaturationBlockingData* individualBlock) {
					mIndiSatBlockData = individualBlock;
					mPrevIndiSatBlockData = individualBlock;
					return this;
				}


				CIndividualNodeBlockData* CIndividualProcessNode::getIndividualBlockData(bool localBlockData) {
					if (localBlockData) {
						return mIndiBlock;
					} else {
						return mPrevIndiBlock;
					}
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualBlockData(CIndividualNodeBlockData* individualBlock) {
					mIndiBlock = individualBlock;
					mPrevIndiBlock = mIndiBlock;
					return this;
				}

				cint64 CIndividualProcessNode::getIndividualAncestorDepth() {
					return indiAncDepth;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualAncestorDepth(cint64 depth) {
					indiAncDepth = depth;
					return this;
				}

				cint64 CIndividualProcessNode::getIndividualNominalLevel() {
					return mNominalLevel;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualNominalLevel(cint64 level) {
					mNominalLevel = level;
					return this;
				}

				cint64 CIndividualProcessNode::getIndividualNominalLevelOrAncestorDepth() {
					if (isNominalIndividual()) {
						return getIndividualNominalLevel();
					}
					return getIndividualAncestorDepth();
				}


				cint64 CIndividualProcessNode::getMergedIntoIndividualNodeID() {
					return mMergeIntoID;
				}

				bool CIndividualProcessNode::hasMergedIntoIndividualNodeID() {
					return mMergeIntoID != mIndiID;
				}

				CIndividualProcessNode* CIndividualProcessNode::setMergedIntoIndividualNodeID(cint64 indiNodeID) {
					mMergeIntoID = indiNodeID;
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::addProcessingBlockedIndividualsLinker(CXLinker<CIndividualProcessNode*>* linker) {
					if (linker) {
						mProcessingBlockedIndividualsLinker = linker->append(mProcessingBlockedIndividualsLinker);
					}
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndirectBlockedIndividualsLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mProcessingBlockedIndividualsLinker = linker;
					return this;
				}

				CXLinker<CIndividualProcessNode*>* CIndividualProcessNode::getProcessingBlockedIndividualsLinker() {
					return mProcessingBlockedIndividualsLinker;				
				}


				CReapplyConceptDescriptor* CIndividualProcessNode::getSatisfiableCachedAbsorbedDisjunctionsLinker() {
					return mSatCachedAbsorbedDisjunctionsReapplyConDes;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearSatisfiableCachedAbsorbedDisjunctionsLinker() {
					mSatCachedAbsorbedDisjunctionsReapplyConDes = nullptr;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::addSatisfiableCachedAbsorbedDisjunctionsLinker(CReapplyConceptDescriptor* disjunctionReapplyConDes) {
					if (!mSatCachedAbsorbedDisjunctionsReapplyConDes) {
						mSatCachedAbsorbedDisjunctionsReapplyConDes = disjunctionReapplyConDes;
					} else {
						disjunctionReapplyConDes->setNext(mSatCachedAbsorbedDisjunctionsReapplyConDes);
						mSatCachedAbsorbedDisjunctionsReapplyConDes = disjunctionReapplyConDes;
					}
					return this;
				}



				CIndividualProcessNode* CIndividualProcessNode::clearSatisfiableCachedAbsorbedGeneratingLinker() {
					mSatCachedAbsorbedSuccessorReapplyConDes = nullptr;
					return this;
				}

				CReapplyConceptDescriptor* CIndividualProcessNode::getSatisfiableCachedAbsorbedGeneratingLinker() {
					return mSatCachedAbsorbedSuccessorReapplyConDes;
				}

				CIndividualProcessNode* CIndividualProcessNode::addSatisfiableCachedAbsorbedGeneratingLinker(CReapplyConceptDescriptor* successorGeneratingReapplyConDes) {
					if (!mSatCachedAbsorbedSuccessorReapplyConDes) {
						mSatCachedAbsorbedSuccessorReapplyConDes = successorGeneratingReapplyConDes;
					} else {
						successorGeneratingReapplyConDes->setNext(mSatCachedAbsorbedSuccessorReapplyConDes);
						mSatCachedAbsorbedSuccessorReapplyConDes = successorGeneratingReapplyConDes;
					}
					return this;
				}



				cint64 CIndividualProcessNode::getProcessingRestrictionFlags() {
					return mProcessingRestrictionFlags;
				}

				bool CIndividualProcessNode::addProcessingRestrictionFlags(cint64 flags) {
					cint64 flagsBackup = mProcessingRestrictionFlags;
					mProcessingRestrictionFlags |= flags;
					return mProcessingRestrictionFlags != flagsBackup;
				}

				bool CIndividualProcessNode::clearProcessingRestrictionFlags(cint64 flags) {
					cint64 flagsBackup = mProcessingRestrictionFlags;
					mProcessingRestrictionFlags &= ~flags;
					return mProcessingRestrictionFlags != flagsBackup;
				}

				bool CIndividualProcessNode::hasProcessingRestrictionFlags(cint64 testFlags) {
					return (mProcessingRestrictionFlags & testFlags) == testFlags;
				}

				bool CIndividualProcessNode::hasPartialProcessingRestrictionFlags(cint64 testFlags) {
					return (mProcessingRestrictionFlags & testFlags) != 0;
				}


				CConceptDescriptor* CIndividualProcessNode::getIndividualInitializationConcept() {
					return mInitConceptDescriptor;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualInitializationConcept(CConceptDescriptor* initConDes) {
					mInitConceptDescriptor = initConDes;
					return this;
				}

				bool CIndividualProcessNode::hasBlockingRetestProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFBLOCKINGRETESTDUEANCESTORMODIFIED | PRFBLOCKINGRETESTDUEDIRECTMODIFIED | PRFBLOCKINGRETESTDUEBLOCKERMODIFIED | PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS | PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED);
				}

				bool CIndividualProcessNode::clearBlockingRetestProcessingRestrictionFlags() {
					return clearProcessingRestrictionFlags(PRFBLOCKINGRETESTDUEANCESTORMODIFIED | PRFBLOCKINGRETESTDUEDIRECTMODIFIED | PRFBLOCKINGRETESTDUEBLOCKERMODIFIED | PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS | PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED);
				}

				bool CIndividualProcessNode::hasBlockedProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFDIRECTBLOCKED | PRFINDIRECTBLOCKED | PRFPROCESSINGBLOCKED);
				}

				bool CIndividualProcessNode::hasDirectBlockedProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFDIRECTBLOCKED);
				}

				bool CIndividualProcessNode::hasProcessingBlockedProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFPROCESSINGBLOCKED);
				}

				bool CIndividualProcessNode::hasPurgedBlockedProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFPURGEDBLOCKED);
				}

				bool CIndividualProcessNode::hasIndirectBlockedProcessingRestrictionFlags() {
					return hasPartialProcessingRestrictionFlags(PRFINDIRECTBLOCKED);
				}

				bool CIndividualProcessNode::clearBlockedProcessingRestrictionFlags() {
					return clearProcessingRestrictionFlags(PRFDIRECTBLOCKED | PRFINDIRECTBLOCKED | PRFPROCESSINGBLOCKED);
				}



				CIndividualProcessNode* CIndividualProcessNode::setLastConceptCountCachedBlockingCandidate(cint64 conCount) {
					mLastConceptCountCachedBlockerCandidate = conCount;
					return this;
				}

				cint64 CIndividualProcessNode::getLastConceptCountCachedBlockingCandidate() {
					return mLastConceptCountCachedBlockerCandidate;
				}

				cint64 CIndividualProcessNode::getLastConceptCountSearchBlockingCandidate() {
					return mLastConceptCountSearchBlockerCandidate;
				}

				CIndividualProcessNode* CIndividualProcessNode::setLastConceptCountSearchBlockingCandidate(cint64 conCount) {
					mLastConceptCountSearchBlockerCandidate = conCount;
					return this;
				}



				cint64 CIndividualProcessNode::getBlockingCachingSavedCandidateCount() {
					return mBlockingCachingSavedCandidateCount;
				}

				CIndividualProcessNode* CIndividualProcessNode::setBlockingCachingSavedCandidateCount(cint64 conCount) {
					mBlockingCachingSavedCandidateCount = conCount;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::incBlockingCachingSavedCandidateCount(cint64 incCount) {
					mBlockingCachingSavedCandidateCount += incCount;
					return this;
				}


				cint64 CIndividualProcessNode::getLastSearchBlockerCandidateCount() {
					return mLastSearchBlockerCandidateCount;
				}

				CIndividualProcessNode* CIndividualProcessNode::setLastSearchBlockerCandidateCount(cint64 canCount) {
					mLastSearchBlockerCandidateCount = canCount;
					return this;
				}

				cint64 CIndividualProcessNode::getLastSearchBlockerCandidateSignature() {
					return mLastSearchBlockerCandidateSignature;
				}

				CIndividualProcessNode* CIndividualProcessNode::setLastSearchBlockerCandidateSignature(cint64 canCount) {
					mLastSearchBlockerCandidateSignature = canCount;
					return this;
				}


				bool CIndividualProcessNode::isInvalidSignatureBlocking() {
					return mInvalidSignatureBlocking;
				}

				CIndividualProcessNode* CIndividualProcessNode::setInvalidSignatureBlocking(bool invalid) {
					mInvalidSignatureBlocking = invalid;
					return this;
				}






				CIndividualProcessNode* CIndividualProcessNode::addSuccessorIndividualNodeBackwardDependencyLinker(CXLinker<CIndividualProcessNode* >* linker) {
					if (linker) {
						mSuccessorIndiNodeBackwardDependencyLinker = linker->append(mSuccessorIndiNodeBackwardDependencyLinker);
					}
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setSuccessorIndividualNodeBackwardDependencyLinker(CXLinker<CIndividualProcessNode* >* linker) {
					mSuccessorIndiNodeBackwardDependencyLinker = linker;
					return this;
				}

				CXLinker<CIndividualProcessNode* >* CIndividualProcessNode::getSuccessorIndividualNodeBackwardDependencyLinker() {
					return mSuccessorIndiNodeBackwardDependencyLinker;				
				}


				bool CIndividualProcessNode::hasSuccessorIndividualNodeBackwardDependencyLinker() {
					return mSuccessorIndiNodeBackwardDependencyLinker;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearSuccessorIndividualNodeBackwardDependencyLinker() {
					mSuccessorIndiNodeBackwardDependencyLinker = nullptr;
					return this;
				}


				bool CIndividualProcessNode::hasBackwardDependencyToAncestorIndividualNode() {
					return mBackwardDependencyToAncestorIndividualNode;
				}

				CIndividualProcessNode* CIndividualProcessNode::setBackwardDependencyToAncestorIndividualNode(bool backwardDependency) {
					mBackwardDependencyToAncestorIndividualNode = backwardDependency;
					return this;
				}


				CRoleBackwardPropagationHash* CIndividualProcessNode::getRoleBackwardPropagationHash(bool create) {
					if (create && !mRoleBackPropHash) {
						mRoleBackPropHash = CObjectParameterizingAllocator< CRoleBackwardPropagationHash,CProcessContext* >::allocateAndConstructAndParameterize(mMemAllocMan,mProcessContext);
					}
					return mRoleBackPropHash;
				}

				CIndividualProcessNodeLinker* CIndividualProcessNode::getIndividualProcessNodeLinker() {
					return mIndiProcessLinker;
				}

				CIndividualProcessNode* CIndividualProcessNode::setIndividualProcessNodeLinker(CIndividualProcessNodeLinker* processNodeLinker) {
					mIndiProcessLinker = processNodeLinker;
					return this;
				}

				CConceptProcessLinker* CIndividualProcessNode::getConceptProcessLinker() {
					return mConceptProcessLinker;
				}

				CConceptProcessLinker* CIndividualProcessNode::takeConceptProcessLinker() {
					CConceptProcessLinker* conProcLinker = mConceptProcessLinker;
					if (mConceptProcessLinker) {
						mConceptProcessLinker = mConceptProcessLinker->getNext();
					}
					return conProcLinker;
				}

				CIndividualProcessNode* CIndividualProcessNode::setConceptProcessLinker(CConceptProcessLinker* conProcessLinker) {
					mConceptProcessLinker = conProcessLinker;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::addConceptProcessLinker(CConceptProcessLinker* conProcessLinker) {
					mConceptProcessLinker = conProcessLinker->append(mConceptProcessLinker);
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::clearConceptProcessLinker() {
					mConceptProcessLinker = nullptr;
					return this;
				}

				bool CIndividualProcessNode::getRequiredBackwardPropagation() {
					return mRequiredBackProp;
				}

				CIndividualProcessNode* CIndividualProcessNode::setRequiredBackwardPropagation(bool requiredBackProp) {
					mRequiredBackProp = requiredBackProp;
					return this;
				}

				bool CIndividualProcessNode::hasSubstituteIndividualNode() {
					return mSubstituteIndiNode != nullptr;
				}

				CIndividualProcessNode* CIndividualProcessNode::getSubstituteIndividualNode() {
					return mSubstituteIndiNode;
				}

				CIndividualProcessNode* CIndividualProcessNode::setSubstituteIndividualNode(CIndividualProcessNode* indiNode) {
					mSubstituteIndiNode = indiNode;
					return this;
				}

				bool CIndividualProcessNode::isProcessingQueued() {
					return mProcessingQueued;
				}

				bool CIndividualProcessNode::isExtendedQueueProcessing() {
					return mExtendedQueueProcessing;
				}

				bool CIndividualProcessNode::isImmediatelyProcessingQueued() {
					return mImmediatelyProcessingQueued;
				}

				bool CIndividualProcessNode::isDeterministicExpandingProcessingQueued() {
					return mDetExpProcessingQueued;
				}

				bool CIndividualProcessNode::isRegularDepthProcessingQueued() {
					return mDepthProcessingQueued;
				}

				bool CIndividualProcessNode::isBlockedReactivationProcessingQueued() {
					return mBlockedReactProcessingQueued;
				}

				CIndividualProcessNode* CIndividualProcessNode::setProcessingQueued(bool processingQueued) {
					mProcessingQueued = processingQueued;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setExtendedQueueProcessing(bool extendedQueueProcessing) {
					mExtendedQueueProcessing = extendedQueueProcessing;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setImmediatelyProcessingQueued(bool immProQue) {
					mImmediatelyProcessingQueued = immProQue;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setDeterministicExpandingProcessingQueued(bool immProQue) {
					mDetExpProcessingQueued = immProQue;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setRegularDepthProcessingQueued(bool depthPro) {
					mDepthProcessingQueued = depthPro;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setBlockedReactivationProcessingQueued(bool depthPro) {
					mBlockedReactProcessingQueued = depthPro;
					return this;
				}



				CIndividualProcessNode* CIndividualProcessNode::setBackendSynchronRetestProcessingQueued(bool backendSyncRetest) {
					mBackendSynchronRetestProcessingQueued = backendSyncRetest;
					return this;
				}


				bool CIndividualProcessNode::isBackendSynchronRetestProcessingQueued() {
					return mBackendSynchronRetestProcessingQueued;
				}





				CIndividualProcessNode* CIndividualProcessNode::setIncrementalCompatibilityCheckingQueued(bool incCompChecking) {
					mIncrementalCompatibilityCheckingQueued = incCompChecking;
					return this;
				}


				bool CIndividualProcessNode::isIncrementalCompatibilityCheckingQueued() {
					return mIncrementalCompatibilityCheckingQueued;
				}




				CIndividualProcessNode* CIndividualProcessNode::setIncrementalExpansionQueued(bool incExpQueued) {
					mIncrementalExpansionQueued = incExpQueued;
					return this;
				}


				bool CIndividualProcessNode::isIncrementalExpansionQueued() {
					return mIncrementalExpansionQueued;
				}


				CIndividualProcessNode* CIndividualProcessNode::clearProcessingQueued() {
					mBlockedReactProcessingQueued = false;
					mProcessingQueued = false;
					mImmediatelyProcessingQueued = false;
					mDetExpProcessingQueued = false;
					mDepthProcessingQueued = false;
					mDelayedNominalProcessingQueued = false;
					mBackendSynchronRetestProcessingQueued = false;
					mIncrementalCompatibilityCheckingQueued = false;
					mIncrementalExpansionQueued = false;
					return this;
				}


				bool CIndividualProcessNode::isDelayedNominalProcessingQueued() {
					return mDelayedNominalProcessingQueued;
				}

				bool CIndividualProcessNode::hasNominalProcessingDelayingChecked() {
					return mNominalProcessingDelayingChecked;
				}

				CIndividualProcessNode* CIndividualProcessNode::setDelayedNominalProcessingQueued(bool delayedProcessingQueued) {
					mDelayedNominalProcessingQueued = delayedProcessingQueued;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::setNominalProcessingDelayingChecked(bool nominalProcessingDelayingChecked) {
					mNominalProcessingDelayingChecked = nominalProcessingDelayingChecked;
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::setAssertionInitialisationSignatureValue(cint64 sigValue) {
					mAssertionInitialisationSignatureValue = sigValue;
					return this;
				}

				cint64 CIndividualProcessNode::getAssertionInitialisationSignatureValue() {
					return mAssertionInitialisationSignatureValue;
				}


				CIndividualProcessNode* CIndividualProcessNode::setLastProcessingPriority(const CIndividualProcessNodePriority& priority) {
					mLastProcessingPriority = priority;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::resetLastProcessingPriority() {
					mLastProcessingPriority.setPriorityToNull();
					return this;
				}

				CIndividualProcessNodePriority CIndividualProcessNode::getLastProcessingPriority() {
					return mLastProcessingPriority;
				}

				bool CIndividualProcessNode::isCachingLossNodeReactivationInstalled() {
					return mCachingLossNodeReactivationInstalled;
				}

				CIndividualProcessNode* CIndividualProcessNode::setCachingLossNodeReactivationInstalled(bool reactivationInstalled) {
					mCachingLossNodeReactivationInstalled = reactivationInstalled;
					return this;
				}



				CNominalCachingLossReactivationData* CIndividualProcessNode::getNominalCachingLossReactivationData(bool create) {
					CNominalCachingLossReactivationData* nominalNodeReactData = nullptr;
					if (!mLocReactivationData && create) {
						CNominalCachingLossReactivationData* nominalNodeReactData = CObjectParameterizingAllocator< CNominalCachingLossReactivationData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						nominalNodeReactData->initNominalCachingLossReactivationData(getIndividualID(),mUseReactivationData);
						mLocReactivationData = nominalNodeReactData;
						mUseReactivationData = nominalNodeReactData;
					}
					return mUseReactivationData;
				}

				CSuccessorConnectedNominalSet* CIndividualProcessNode::getSuccessorNominalConnectionSet(bool create) {
					CSuccessorConnectedNominalSet* nomConnSet = nullptr;
					if (!mLocNominalConnectionSet && create) {
						CSuccessorConnectedNominalSet* nomConnSet = CObjectParameterizingAllocator< CSuccessorConnectedNominalSet,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						nomConnSet->initSuccessorConnectedNominalSet(mUseNominalConnectionSet);
						mLocNominalConnectionSet = nomConnSet;
						mUseNominalConnectionSet = nomConnSet;
					}
					return mUseNominalConnectionSet;
				}

				bool CIndividualProcessNode::hasSuccessorConnectionToNominal(cint64 nominalID) {
					CSuccessorConnectedNominalSet* nomConnSet = getSuccessorNominalConnectionSet(false);
					if (nomConnSet) {
						return nomConnSet->hasSuccessorConnectedNominal(nominalID);
					}
					return false;
				}

				bool CIndividualProcessNode::addSuccessorConnectionToNominal(cint64 nominalID) {
					CSuccessorConnectedNominalSet* nomConnSet = getSuccessorNominalConnectionSet(true);
					if (nomConnSet) {
						return nomConnSet->addSuccessorConnectedNominal(nominalID);
					}
					return false;
				}



				CDatatypesValueSpaceData* CIndividualProcessNode::getDatatypesValueSpaceData(bool create) {
					CDatatypesValueSpaceData* valueSpaceData = nullptr;
					if (!mLocDatatypesValueSpaceData && create) {
						CDatatypesValueSpaceData* valueSpaceData = CObjectParameterizingAllocator< CDatatypesValueSpaceData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						valueSpaceData->initDatatypesValueSpaceData(mUseDatatypesValueSpaceData);
						mLocDatatypesValueSpaceData = valueSpaceData;
						mUseDatatypesValueSpaceData = valueSpaceData;
					}
					return mUseDatatypesValueSpaceData;
				}



				CIndividualNodeIncrementalExpansionData* CIndividualProcessNode::getIncrementalExpansionData(bool create) {
					CIndividualNodeIncrementalExpansionData* incExpData = nullptr;
					if (!mLocIncExpData && create) {
						CIndividualNodeIncrementalExpansionData* incExpData = CObjectParameterizingAllocator< CIndividualNodeIncrementalExpansionData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						incExpData->initIncrementalExpansionData(mUseIncExpData);
						mLocIncExpData = incExpData;
						mUseIncExpData = incExpData;
					}
					return mUseIncExpData;
				}


				CIndividualMergingHash* CIndividualProcessNode::getIndividualMergingHash(bool create) {
					CIndividualMergingHash* indiMergHash = nullptr;
					if (!mLocIndividualMergingHash && create) {
						CIndividualMergingHash* indiMergHash = CObjectParameterizingAllocator< CIndividualMergingHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						indiMergHash->initIndividualMergingHash(mUseIndividualMergingHash);
						mLocIndividualMergingHash = indiMergHash;
						mUseIndividualMergingHash = indiMergHash;
					}
					return mUseIndividualMergingHash;
				}


				CIndividualProcessNode* CIndividualProcessNode::getLastMergedIntoIndividualNode() {
					return mLastMergedIntoIndividualNode;
				}

				CIndividualProcessNode* CIndividualProcessNode::setLastMergedIntoIndividualNode(CIndividualProcessNode* indiNode) {
					mLastMergedIntoIndividualNode = indiNode;
					return this;
				}


				CDependencyTrackPoint* CIndividualProcessNode::getMergedDependencyTrackPoint() {
					return mMergedDepTrackPoint;
				}

				CIndividualProcessNode* CIndividualProcessNode::setMergedDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint) {
					mMergedDepTrackPoint = depTrackPoint;
					return this;
				}


				CIndividualProcessNode* CIndividualProcessNode::setBlockerIndividualNode(CIndividualProcessNode* indiNode) {
					mBlockerIndiNode = indiNode;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::getBlockerIndividualNode() {
					return mBlockerIndiNode;
				}

				CIndividualProcessNode* CIndividualProcessNode::setFollowingIndividualNode(CIndividualProcessNode* indiNode) {
					mFollowingIndiNode = indiNode;
					return this;
				}

				CIndividualProcessNode* CIndividualProcessNode::getFollowingIndividualNode() {
					return mFollowingIndiNode;
				}


				CIndividualProcessNode* CIndividualProcessNode::setIncrementalExpansionID(cint64 incExpID) {
					mIncExpID = incExpID;
					return this;
				}

				cint64 CIndividualProcessNode::getIncrementalExpansionID() {
					return mIncExpID;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

