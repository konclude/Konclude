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

#include "CProcessingDataBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessingDataBox::CProcessingDataBox(CProcessContext* processContext) : mProcessContext(processContext) {
					mIndiProcessVector = CObjectParameterizingAllocator< CIndividualProcessNodeVector,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					mOntologyTopConcept = nullptr;
					mOntologyTopDataRangeConcept = nullptr;
					mSignatureBlockingCandidateHash = nullptr;
					mUseSignatureBlockingCandidateHash = nullptr;
					mPrevSignatureBlockingCandidateHash = nullptr;

					mSignatureNominalDelayingCandidateHash = nullptr;
					mUseSignatureNominalDelayingCandidateHash = nullptr;
					mPrevSignatureNominalDelayingCandidateHash = nullptr;

					mBlockingIndiNodeCandidateHash = nullptr;
					mUseBlockingIndiNodeCandidateHash = nullptr;
					mPrevBlockingIndiNodeCandidateHash = nullptr;
					mBlockingIndiNodeLinkedCandidateHash = nullptr;
					mUseBlockingIndiNodeLinkedCandidateHash = nullptr;
					mPrevBlockingIndiNodeLinkedCandidateHash = nullptr;
					mEarlyIndiReactProQueue = nullptr;
					mLateIndiReactProQueue = nullptr;
					mSignatureBlockingReviewSet = nullptr;
					mReusingReviewSet = nullptr;
					mUseSignatureBlockingReviewSet = nullptr;
					mUseEarlyIndiReactProQueue = nullptr;
					mUseLateIndiReactProQueue = nullptr;
					mUseReusingReviewSet = nullptr;
					mPrevSignatureBlockingReviewSet = nullptr;
					mPrevEarlyIndiReactProQueue = nullptr;
					mPrevReusingReviewSet = nullptr;
					mNodeSwitchHistory = nullptr;
					mUseNodeSwitchHistory = nullptr;
					mPrevNodeSwitchHistory = nullptr;
					mBranchingTree = nullptr;
					mUseBranchingTree = nullptr;
					mPrevBranchingTree = nullptr;
					mIndividualNodeCacheTestingLinker = nullptr;
					mSortedNominalNonDetProcessingNodeLinker = nullptr;
					mSortedNominalNonDetProcessingNodesSorted = false;
					mNominalNonDetProcessingCount = 0;
					mIndividualNodeResolveLinker = nullptr;
					mBlockableIndividualNodeUpdatedLinker = nullptr;
					mConstructedIndiNode = nullptr;
					mLastConDesIndiReapplication = false;
					mLastProcessingIndiNode = nullptr;
					mLastProcessingConDes = nullptr;
					mIndiProcessNodeLinker = nullptr;
					mIndiSaturationProcessNodeLinker = nullptr;
					mIndiSaturationCompletionNodeLinker = nullptr;
					mIndiSaturationCompletedNodeLinker = nullptr;
					mIndiSaturationAnalysingNodeLinker = nullptr;
					mNominalDelayedIndiSaturationProcessNodeLinker = nullptr;
					mSaturationATMOSTMergingProcessLinker = nullptr;
					mDisjunctCommonConceptExtractProcessingLinker = nullptr;
					mRemConSatProcessLinker = nullptr;
					mRemSatUpdateLinker = nullptr;
					mRemSatIndiNodeLinker = nullptr;
					mRemSatIndiSuccLinkDataLinker = nullptr;
					mRemConSatDes = nullptr;
					mRemConDes = nullptr;
					mRemRoleSatProcessLinker = nullptr;
					mIndiSaturationProcessVector = nullptr;
					mSatCriticalIndiNodeProcQueue = nullptr;
					mSatSuccExtIndNodeProcQueue = nullptr;
					mSatCriticalIndiNodeConTestSet = nullptr;
					mSatNominalDependentNodeHash = nullptr;
					mSatInfluencedNominalSet = nullptr;
					mIndiDepthFirstProcessQueue = nullptr;
					mUseIndiDepthFirstProcessQueue = nullptr;
					mPrevIndiDepthFirstProcessQueue = nullptr;
					mIndiImmProcessQueue = nullptr;
					mUseIndiImmProcessQueue = nullptr;
					mPrevIndiImmProcessQueue = nullptr;
					mDelayNomProcessQueue = nullptr;
					mUseDelayNomProcessQueue = nullptr;
					mPrevDelayNomProcessQueue = nullptr;
					mCachingLossReactivationProcessQueue = nullptr;
					mUseCachingLossReactivationProcessQueue = nullptr;
					mPrevCachingLossReactivationProcessQueue = nullptr;
					mRoleAssertionProcessQueue = nullptr;
					mUseRoleAssertionProcessQueue = nullptr;
					mPrevRoleAssertionProcessQueue = nullptr;
					mBackendSyncRetestProcessQueue = nullptr;
					mUseBackendSyncRetestProcessQueue = nullptr;
					mPrevBackendSyncRetestProcessQueue = nullptr;
					mBackendIndirectCompatibilityExpansionQueue = nullptr;
					mUseBackendIndirectCompatibilityExpansionQueue = nullptr;
					mPrevBackendIndirectCompatibilityExpansionQueue = nullptr;

					mBackendIndividualReuseExpansionQueue = nullptr;
					mUseBackendIndividualReuseExpansionQueue = nullptr;
					mPrevBackendIndividualReuseExpansionQueue = nullptr;
					mBackendLateIndividualReuseExpansionQueue = nullptr;
					mUseBackendLateIndividualReuseExpansionQueue = nullptr;
					mPrevBackendLateIndividualReuseExpansionQueue = nullptr;
					mBackendIndividualNeighbourExpansionQueue = nullptr;
					mUseBackendIndividualNeighbourExpansionQueue = nullptr;
					mPrevBackendIndividualNeighbourExpansionQueue = nullptr;

					mBackendIndividualLateReuseExpansionActivated = false;


					mBackendDirectInfluenceExpansionQueue = nullptr;
					mUseBackendDirectInfluenceExpansionQueue = nullptr;
					mPrevBackendDirectInfluenceExpansionQueue = nullptr;
					mConstructedIndiNodeInitialized = false;
					mMultipleConstructionIndiNodes = false;
					mMaximumDeterministicBranchTag = 0;
					mNextIndividualNodeID = 0;
					mNextSatResSuccExtIndividualNodeID = -1;
					mNextPropagationID = 1;
					mNextVariableID = 1;
					mNextRepVariableID = 1;
					mLocExtendedConceptVector = nullptr;
					mUseExtendedConceptVector = nullptr;
					mUseGroundingHash = nullptr;
					mLocGroundingHash = nullptr;
					mLocVarBindingPathMergingHash = nullptr;
					mUseVarBindingPathMergingHash = nullptr;
					mLocRepVarBindPathSetHash = nullptr;
					mUseRepVarBindPathSetHash = nullptr;
					mLocRepVarBindPathHash = nullptr;
					mUseRepVarBindPathHash = nullptr;
					mLocRepVarBindPathJoiningKeyHash = nullptr;
					mUseRepVarBindPathJoiningKeyHash = nullptr;
					mLocRepJoiningHash = nullptr;
					mUseRepJoiningHash = nullptr;
					mUseIndiProcessQueue = nullptr;
					mLocIndiProcessQueue = nullptr;
					mUseNomCachingLossReactHash = nullptr;
					mLocNomCachingLossReactHash = nullptr;
					mUseMarkerIndiNodeHash = nullptr;
					mLocMarkerIndiNodeHash = nullptr;
					mUseBackendLoadedAssociationHash = nullptr;
					mLocBackendLoadedAssociationHash = nullptr;
					mUseBackendConceptSetLabelProcessingHash = nullptr;
					mLocBackendConceptSetLabelProcessingHash = nullptr;
					mUseBackendNeighbourExpansionControllingData = nullptr;
					mLocBackendNeighbourExpansionControllingData = nullptr;

					mDelayedBackendInitProcQueue = nullptr;
					mUseDelayedBackendInitProcQueue = nullptr;
					mPrevDelayedBackendInitProcQueue = nullptr;

					mBackendNeighbourExpansionQueue = nullptr;
					mUseBackendNeighbourExpansion = nullptr;
					mPrevBackendNeighbourExpansion = nullptr;

					mIndiDepthProcessingQueue = nullptr;
					mUseIndiDepthProcessingQueue = nullptr;
					mPrevIndiDepthProcessingQueue = nullptr;

					mNominalProcessingQueue = nullptr;
					mUseNominalProcessingQueue = nullptr;
					mPrevNominalProcessingQueue = nullptr;

					mNominalDeterministicProcessingQueue = nullptr;
					mUseNominalDeterministicProcessingQueue = nullptr;
					mPrevNominalDeterministicProcessingQueue = nullptr;

					mIncrementalExansionInitializingProcessingQueue = nullptr;
					mUseIncrementalExansionInitializingProcessingQueue = nullptr;
					mPrevIncrementalExansionInitializingProcessingQueue = nullptr;


					mIncrementalExansionProcessingQueue = nullptr;
					mUseIncrementalExansionProcessingQueue = nullptr;
					mPrevIncrementalExansionProcessingQueue = nullptr;


					mIncrementalCompatibilityCheckingQueue = nullptr;
					mUseIncrementalCompatibilityCheckingQueue = nullptr;
					mPrevIncrementalCompatibilityCheckingQueue = nullptr;

					mIndiDepthDetExpPreProcessingQueue = nullptr;
					mUseIndiDepthDetExpPreProcessingQueue = nullptr;
					mPrevIndiDepthDetExpPreProcessingQueue = nullptr;

					mIndiDepthFirstDetExpPreProcessingQueue = nullptr;
					mUseIndiDepthFirstDetExpPreProcessingQueue = nullptr;
					mPrevIndiDepthFirstDetExpPreProcessingQueue = nullptr;


					mIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					mUseIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					mPrevIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					mIndiBlockedReactivationProcessingQueue = nullptr;
					mUseIndiBlockedReactivationProcessingQueue = nullptr;
					mPrevIndiBlockedReactivationProcessingQueue = nullptr;
					mVarBindConceptBatchProcessQueue = nullptr;
					mUseVarBindConceptBatchProcessQueue = nullptr;
					mPrevVarBindConceptBatchProcessQueue = nullptr;
					mInsufficientNodeOccured = false;
					mDelayedNominalProcessingOccured = false;
					mProblematicEQCandidateNodeOccured = false;
					mIncrementalExpansionInitialized = false;
					mIncrementalExpansionCompatibleMerged = false;
					mIncrementalExpansionCachingMerged = false;
					mClashedDescriptorLinker = nullptr;
					mValueSpaceTriggeringProcessingQueue = nullptr;
					mUseValueSpaceTriggeringProcessingQueue = nullptr;
					mPrevValueSpaceTriggeringProcessingQueue = nullptr;
					mDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;
					mUseDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;
					mPrevDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;

					mLocalIndiVector = nullptr;
					mUseIndiVector = nullptr;

					mSeparatedSaturationConAssResolveNode = nullptr;
					mIncrementalExpID = 0;
					mMaxIncPrevCompGraphNodeID = 0;
					mNextIncrementalIndiExpID = 1;
					mNextRoleAssertionCreationID = 1;

					mReferredIndiTrackVec = nullptr;
					mIndiDepTrackingRequired = false;

					mBranchingInstruction = nullptr;
					mRemainingPossibleInstanceIndividualMergingLimit = -1;
					mPossibleInstanceIndividualMergingSize = 1;
					mPossibleInstanceIndividualMergedCount = 0;
					mPossibleInstanceIndividualCurrentMergingCount = 0;
					mPossibleInstanceIndividualMergingStopped = false;

					mCurrentMergedPossibleInstanceIndividualLinkersLinker = nullptr;
					mLastMergedPossibleInstanceIndividualLinker = nullptr;
					mLastBackendCacheIntegratedIndiNodeLinker = nullptr;
					mBackendCacheIntegratedIndividualNodeCount = 0;
					mBackendCacheIntegratedSameIndividualNodeCount = 0;

					mBackendCacheUpdateIndividualsInitialized = false;
					mRepresentativeNeighbourExpansionIndividualNodeLinker = nullptr;
				}


				CProcessingDataBox* CProcessingDataBox::initProcessingDataBox(CConcreteOntology* ontology) {
					mOntology = ontology;
					mOntologyTopConcept = mOntology->getTBox()->getTopConcept();
					mOntologyTopDataRangeConcept = mOntology->getTBox()->getTopDataRangeConcept();
					mUseExtendedConceptVector = mOntology->getTBox()->getConceptVector(false);
					mUseIndiVector = mOntology->getABox()->getIndividualVector(false);
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::setProcessingOntology(CConcreteOntology* ontology) {
					mOntology = ontology;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::initProcessingDataBox(CProcessingDataBox* processingDataBox) {
					CIndividualProcessNodeVector* prevIndiProcVec = nullptr;
					mUseIndiProcessQueue = nullptr;
					mLocIndiProcessQueue = nullptr;
					mOntology = nullptr;
					mIndividualNodeCacheTestingLinker = nullptr;
					mSortedNominalNonDetProcessingNodeLinker = nullptr;
					mSortedNominalNonDetProcessingNodesSorted = false;
					mNominalNonDetProcessingCount = 0;
					mIndividualNodeResolveLinker = nullptr;
					mBlockableIndividualNodeUpdatedLinker = nullptr;
					mLastConDesIndiReapplication = false;
					mIndiProcessNodeLinker = nullptr;
					mRemSatUpdateLinker = nullptr;
					mRemSatIndiNodeLinker = nullptr;
					mRemSatIndiSuccLinkDataLinker = nullptr;
					mRemConSatDes = nullptr;
					mRemConDes = nullptr;
					mRemRoleSatProcessLinker = nullptr;

					mIndiSaturationProcessNodeLinker = nullptr;
					mIndiSaturationCompletionNodeLinker = nullptr;
					mIndiSaturationCompletedNodeLinker = nullptr;
					mIndiSaturationAnalysingNodeLinker = nullptr;
					mSaturationATMOSTMergingProcessLinker = nullptr;
					mNominalDelayedIndiSaturationProcessNodeLinker = nullptr;
					mDisjunctCommonConceptExtractProcessingLinker = nullptr;
					mRemConSatProcessLinker = nullptr;
					mIndiSaturationProcessVector = nullptr;
					mSatCriticalIndiNodeProcQueue = nullptr;
					mSatSuccExtIndNodeProcQueue = nullptr;
					mSatCriticalIndiNodeConTestSet = nullptr;
					mSatNominalDependentNodeHash = nullptr;
					mSatInfluencedNominalSet = nullptr;
					mConstructedIndiNodeInitialized = false;
					mMultipleConstructionIndiNodes = false;
					mLocExtendedConceptVector = nullptr;
					mUseExtendedConceptVector = nullptr;
					mUseGroundingHash = nullptr;
					mLocGroundingHash = nullptr;
					mLocVarBindingPathMergingHash = nullptr;
					mUseVarBindingPathMergingHash = nullptr;
					mLocRepVarBindPathSetHash = nullptr;
					mUseRepVarBindPathSetHash = nullptr;
					mLocRepVarBindPathHash = nullptr;
					mUseRepVarBindPathHash = nullptr;
					mLocRepVarBindPathJoiningKeyHash = nullptr;
					mUseRepVarBindPathJoiningKeyHash = nullptr;
					mLocRepJoiningHash = nullptr;
					mUseRepJoiningHash = nullptr;
					mLocNomCachingLossReactHash = nullptr;
					mUseNomCachingLossReactHash = nullptr;
					mUseMarkerIndiNodeHash = nullptr;
					mLocMarkerIndiNodeHash = nullptr;
					mUseBackendLoadedAssociationHash = nullptr;
					mLocBackendLoadedAssociationHash = nullptr;
					mUseBackendConceptSetLabelProcessingHash = nullptr;
					mLocBackendConceptSetLabelProcessingHash = nullptr;
					mUseBackendNeighbourExpansionControllingData = nullptr;
					mLocBackendNeighbourExpansionControllingData = nullptr;
					mNextIndividualNodeID = 0;
					mNextSatResSuccExtIndividualNodeID = -1;
					mNextPropagationID = 1;
					mNextVariableID = 1;
					mNextRepVariableID = 1;
					mInsufficientNodeOccured = false;
					mDelayedNominalProcessingOccured = false;
					mProblematicEQCandidateNodeOccured = false;
					mIncrementalExpansionInitialized = false;
					mIncrementalExpansionCompatibleMerged = false;
					mIncrementalExpansionCachingMerged = false;
					mSeparatedSaturationConAssResolveNode = nullptr;
					mBranchingInstruction = nullptr;
					mRemainingPossibleInstanceIndividualMergingLimit = -1;
					mPossibleInstanceIndividualMergingStopped = false;
					mPossibleInstanceIndividualMergingSize = 1;
					mPossibleInstanceIndividualMergedCount = 0;
					mPossibleInstanceIndividualCurrentMergingCount = 0;
					mLastMergedPossibleInstanceIndividualLinker = nullptr;
					mCurrentMergedPossibleInstanceIndividualLinkersLinker = nullptr;
					mLastBackendCacheIntegratedIndiNodeLinker = nullptr;
					mBackendCacheIntegratedIndividualNodeCount = 0;
					mBackendCacheIntegratedSameIndividualNodeCount = 0;
					mLocalIndiVector = nullptr;
					mBackendCacheUpdateIndividualsInitialized = false;
					mRepresentativeNeighbourExpansionIndividualNodeLinker = nullptr;
					if (processingDataBox) {
						mPossibleInstanceIndividualMergingStopped = processingDataBox->mPossibleInstanceIndividualMergingStopped;
						mPossibleInstanceIndividualMergingSize = processingDataBox->mPossibleInstanceIndividualMergingSize;
						mPossibleInstanceIndividualMergedCount = processingDataBox->mPossibleInstanceIndividualMergedCount;
						mPossibleInstanceIndividualCurrentMergingCount = processingDataBox->mPossibleInstanceIndividualCurrentMergingCount;
						mRemainingPossibleInstanceIndividualMergingLimit = processingDataBox->mRemainingPossibleInstanceIndividualMergingLimit;
						mLastMergedPossibleInstanceIndividualLinker = processingDataBox->mLastMergedPossibleInstanceIndividualLinker;
						mCurrentMergedPossibleInstanceIndividualLinkersLinker = processingDataBox->mCurrentMergedPossibleInstanceIndividualLinkersLinker;
						mLastBackendCacheIntegratedIndiNodeLinker = processingDataBox->mLastBackendCacheIntegratedIndiNodeLinker;
						mBackendCacheIntegratedIndividualNodeCount = processingDataBox->mBackendCacheIntegratedIndividualNodeCount;
						mBackendCacheIntegratedSameIndividualNodeCount = processingDataBox->mBackendCacheIntegratedSameIndividualNodeCount;
						mBranchingInstruction = processingDataBox->mBranchingInstruction;
						mUseIndiProcessQueue = processingDataBox->mUseIndiProcessQueue;
						prevIndiProcVec = processingDataBox->mIndiProcessVector;
						mOntology = processingDataBox->mOntology;
						mPrevIndiDepthProcessingQueue = processingDataBox->mUseIndiDepthProcessingQueue;
						mUseIndiDepthProcessingQueue = mPrevIndiDepthProcessingQueue;
						mPrevNominalProcessingQueue = processingDataBox->mUseNominalProcessingQueue;
						mUseNominalProcessingQueue = mPrevNominalProcessingQueue;
						mPrevIncrementalExansionInitializingProcessingQueue = processingDataBox->mUseIncrementalExansionInitializingProcessingQueue;
						mUseIncrementalExansionInitializingProcessingQueue = mPrevIncrementalExansionInitializingProcessingQueue;
						mPrevIncrementalCompatibilityCheckingQueue = processingDataBox->mUseIncrementalCompatibilityCheckingQueue;
						mUseIncrementalCompatibilityCheckingQueue = mPrevIncrementalCompatibilityCheckingQueue;
						mPrevIndiDepthDetExpPreProcessingQueue = processingDataBox->mUseIndiDepthDetExpPreProcessingQueue;
						mUseIndiDepthDetExpPreProcessingQueue = mPrevIndiDepthDetExpPreProcessingQueue;
						mPrevIndiDepthFirstDetExpPreProcessingQueue = processingDataBox->mUseIndiDepthFirstDetExpPreProcessingQueue;
						mUseIndiDepthFirstDetExpPreProcessingQueue = mPrevIndiDepthFirstDetExpPreProcessingQueue;
						mPrevIndiBlockedReactivationProcessingQueue = processingDataBox->mUseIndiBlockedReactivationProcessingQueue;
						mUseIndiBlockedReactivationProcessingQueue = mPrevIndiBlockedReactivationProcessingQueue;
						mPrevVarBindConceptBatchProcessQueue = processingDataBox->mUseVarBindConceptBatchProcessQueue;
						mUseVarBindConceptBatchProcessQueue = mPrevVarBindConceptBatchProcessQueue;
						mPrevIndiSignatureBlockingUpdateProcessingQueue = processingDataBox->mUseIndiSignatureBlockingUpdateProcessingQueue;
						mUseIndiSignatureBlockingUpdateProcessingQueue = mPrevIndiSignatureBlockingUpdateProcessingQueue;
						mPrevValueSpaceTriggeringProcessingQueue = processingDataBox->mUseValueSpaceTriggeringProcessingQueue;
						mUseValueSpaceTriggeringProcessingQueue = mPrevValueSpaceTriggeringProcessingQueue;
						mPrevDistinctValueSpaceSatisfiabilityCheckingQueue = processingDataBox->mUseDistinctValueSpaceSatisfiabilityCheckingQueue;
						mUseDistinctValueSpaceSatisfiabilityCheckingQueue = mPrevDistinctValueSpaceSatisfiabilityCheckingQueue;
						mPrevSignatureBlockingCandidateHash = processingDataBox->mUseSignatureBlockingCandidateHash;
						mUseSignatureBlockingCandidateHash = mPrevSignatureBlockingCandidateHash;

						mPrevSignatureNominalDelayingCandidateHash = processingDataBox->mUseSignatureNominalDelayingCandidateHash;
						mUseSignatureNominalDelayingCandidateHash = mPrevSignatureNominalDelayingCandidateHash;

						mPrevBlockingIndiNodeCandidateHash = processingDataBox->mUseBlockingIndiNodeCandidateHash;
						mUseBlockingIndiNodeCandidateHash = mPrevBlockingIndiNodeCandidateHash;
						mPrevBlockingIndiNodeLinkedCandidateHash = processingDataBox->mUseBlockingIndiNodeLinkedCandidateHash;
						mUseBlockingIndiNodeLinkedCandidateHash = mPrevBlockingIndiNodeLinkedCandidateHash;
						mPrevSignatureBlockingReviewSet = processingDataBox->mUseSignatureBlockingReviewSet;
						mUseSignatureBlockingReviewSet = mPrevSignatureBlockingReviewSet;
						mPrevEarlyIndiReactProQueue = processingDataBox->mUseEarlyIndiReactProQueue;
						mUseEarlyIndiReactProQueue = mPrevEarlyIndiReactProQueue;
						mPrevLateIndiReactProQueue = processingDataBox->mUseLateIndiReactProQueue;
						mUseLateIndiReactProQueue = mPrevLateIndiReactProQueue;
						mPrevReusingReviewSet = processingDataBox->mUseReusingReviewSet;
						mUseReusingReviewSet = mPrevReusingReviewSet;
						mPrevNodeSwitchHistory = processingDataBox->mUseNodeSwitchHistory;
						mUseNodeSwitchHistory = mPrevNodeSwitchHistory;
						mPrevIndiDepthFirstProcessQueue = processingDataBox->mUseIndiDepthFirstProcessQueue;
						mUseIndiDepthFirstProcessQueue = mPrevIndiDepthFirstProcessQueue;
						mPrevIndiImmProcessQueue = processingDataBox->mUseIndiImmProcessQueue;
						mUseIndiImmProcessQueue = mPrevIndiImmProcessQueue;
						mPrevDelayNomProcessQueue = processingDataBox->mUseDelayNomProcessQueue;
						mUseDelayNomProcessQueue = mPrevDelayNomProcessQueue;
						mPrevCachingLossReactivationProcessQueue = processingDataBox->mUseCachingLossReactivationProcessQueue;
						mUseCachingLossReactivationProcessQueue = mPrevCachingLossReactivationProcessQueue;

						mPrevDelayedBackendInitProcQueue = processingDataBox->mUseDelayedBackendInitProcQueue;
						mUseDelayedBackendInitProcQueue = mPrevDelayedBackendInitProcQueue;

						mPrevBackendNeighbourExpansion = processingDataBox->mUseBackendNeighbourExpansion;
						mUseBackendNeighbourExpansion = mPrevBackendNeighbourExpansion;

						mPrevRoleAssertionProcessQueue = processingDataBox->mUseRoleAssertionProcessQueue;
						mUseRoleAssertionProcessQueue = mPrevRoleAssertionProcessQueue;
						mPrevBackendSyncRetestProcessQueue = processingDataBox->mUseBackendSyncRetestProcessQueue;
						mUseBackendSyncRetestProcessQueue = mPrevBackendSyncRetestProcessQueue;
						mPrevBackendIndirectCompatibilityExpansionQueue = processingDataBox->mUseBackendIndirectCompatibilityExpansionQueue;
						mUseBackendIndirectCompatibilityExpansionQueue = mPrevBackendIndirectCompatibilityExpansionQueue;
						mPrevBackendDirectInfluenceExpansionQueue = processingDataBox->mUseBackendDirectInfluenceExpansionQueue;
						mUseBackendDirectInfluenceExpansionQueue = mPrevBackendDirectInfluenceExpansionQueue;

						mPrevBackendIndividualReuseExpansionQueue = processingDataBox->mUseBackendIndividualReuseExpansionQueue;
						mUseBackendIndividualReuseExpansionQueue = mPrevBackendIndividualReuseExpansionQueue;
						mBackendIndividualLateReuseExpansionActivated = processingDataBox->mBackendIndividualLateReuseExpansionActivated;
						mPrevBackendLateIndividualReuseExpansionQueue = processingDataBox->mUseBackendLateIndividualReuseExpansionQueue;
						mUseBackendLateIndividualReuseExpansionQueue = mPrevBackendLateIndividualReuseExpansionQueue;
						mPrevBackendIndividualNeighbourExpansionQueue = processingDataBox->mUseBackendIndividualNeighbourExpansionQueue;
						mUseBackendIndividualNeighbourExpansionQueue = mPrevBackendIndividualNeighbourExpansionQueue;

						mPrevBranchingTree = processingDataBox->mUseBranchingTree;
						mUseBranchingTree = mPrevBranchingTree;
						mOntologyTopConcept = processingDataBox->mOntologyTopConcept;
						mOntologyTopDataRangeConcept = processingDataBox->mOntologyTopDataRangeConcept;
						mIndividualNodeCacheTestingLinker = processingDataBox->mIndividualNodeCacheTestingLinker;
						mSortedNominalNonDetProcessingNodeLinker = processingDataBox->mSortedNominalNonDetProcessingNodeLinker;
						mSortedNominalNonDetProcessingNodesSorted = processingDataBox->mSortedNominalNonDetProcessingNodesSorted;
						mNominalNonDetProcessingCount = processingDataBox->mNominalNonDetProcessingCount;
						mIndividualNodeResolveLinker = processingDataBox->mIndividualNodeResolveLinker;
						mBlockableIndividualNodeUpdatedLinker = processingDataBox->mBlockableIndividualNodeUpdatedLinker;
						mConstructedIndiNode = processingDataBox->mConstructedIndiNode;
						mLastProcessingIndiNode = processingDataBox->mLastProcessingIndiNode;
						mLastProcessingConDes = processingDataBox->mLastProcessingConDes;
						mIndiProcessNodeLinker = processingDataBox->mIndiProcessNodeLinker;
						mMultipleConstructionIndiNodes = processingDataBox->mMultipleConstructionIndiNodes;
						mConstructedIndiNodeInitialized = processingDataBox->mConstructedIndiNodeInitialized;
						mMaximumDeterministicBranchTag = processingDataBox->mMaximumDeterministicBranchTag;
						mNextPropagationID = processingDataBox->mNextPropagationID;
						mNextIndividualNodeID = processingDataBox->mNextIndividualNodeID;
						mNextSatResSuccExtIndividualNodeID = processingDataBox->mNextSatResSuccExtIndividualNodeID;
						mNextVariableID = processingDataBox->mNextVariableID;
						mNextRepVariableID = processingDataBox->mNextRepVariableID;
						mUseExtendedConceptVector = processingDataBox->mUseExtendedConceptVector;
						mUseGroundingHash = processingDataBox->mUseGroundingHash;
						mUseVarBindingPathMergingHash = processingDataBox->mUseVarBindingPathMergingHash;
						mUseRepVarBindPathSetHash = processingDataBox->mUseRepVarBindPathSetHash;
						mUseRepVarBindPathHash = processingDataBox->mUseRepVarBindPathHash;
						mUseRepVarBindPathJoiningKeyHash = processingDataBox->mUseRepVarBindPathJoiningKeyHash;
						mUseRepJoiningHash = processingDataBox->mUseRepJoiningHash;
						mInsufficientNodeOccured = processingDataBox->mInsufficientNodeOccured;
						mDelayedNominalProcessingOccured = processingDataBox->mDelayedNominalProcessingOccured;
						mProblematicEQCandidateNodeOccured = processingDataBox->mProblematicEQCandidateNodeOccured;
						mUseNomCachingLossReactHash = processingDataBox->mUseNomCachingLossReactHash;
						mUseMarkerIndiNodeHash = processingDataBox->mUseMarkerIndiNodeHash;
						mUseBackendConceptSetLabelProcessingHash = processingDataBox->mUseBackendConceptSetLabelProcessingHash;
						mUseBackendLoadedAssociationHash = processingDataBox->mUseBackendLoadedAssociationHash;
						mUseBackendNeighbourExpansionControllingData = processingDataBox->mUseBackendNeighbourExpansionControllingData;
						mIncrementalExpansionInitialized = processingDataBox->mIncrementalExpansionInitialized;
						mNextIncrementalIndiExpID = processingDataBox->mNextIncrementalIndiExpID;
						mNextRoleAssertionCreationID = processingDataBox->mNextRoleAssertionCreationID;
						mIncrementalExpID = processingDataBox->mIncrementalExpID;
						mMaxIncPrevCompGraphNodeID = processingDataBox->mMaxIncPrevCompGraphNodeID;
						mIncrementalExpansionCompatibleMerged = processingDataBox->mIncrementalExpansionCompatibleMerged;
						mIncrementalExpansionCachingMerged = processingDataBox->mIncrementalExpansionCachingMerged;
						mReferredIndiTrackVec = processingDataBox->mReferredIndiTrackVec;
						mIndiDepTrackingRequired = processingDataBox->mIndiDepTrackingRequired;


						mIndiSaturationProcessNodeLinker = processingDataBox->mIndiSaturationProcessNodeLinker;
						mIndiSaturationCompletionNodeLinker = processingDataBox->mIndiSaturationCompletionNodeLinker;
						mIndiSaturationCompletedNodeLinker = processingDataBox->mIndiSaturationCompletedNodeLinker;
						mIndiSaturationAnalysingNodeLinker = processingDataBox->mIndiSaturationAnalysingNodeLinker;
						mDisjunctCommonConceptExtractProcessingLinker = processingDataBox->mDisjunctCommonConceptExtractProcessingLinker;
						mNominalDelayedIndiSaturationProcessNodeLinker = processingDataBox->mNominalDelayedIndiSaturationProcessNodeLinker;
						mSaturationATMOSTMergingProcessLinker = processingDataBox->mSaturationATMOSTMergingProcessLinker;
						mUseIndiVector = processingDataBox->mUseIndiVector;

						mBackendCacheUpdateIndividualsInitialized = processingDataBox->mBackendCacheUpdateIndividualsInitialized;
						mRepresentativeNeighbourExpansionIndividualNodeLinker = processingDataBox->mRepresentativeNeighbourExpansionIndividualNodeLinker;

						if (processingDataBox->mIndiSaturationProcessVector) {
							getIndividualSaturationProcessNodeVector(true)->referenceVector(processingDataBox->mIndiSaturationProcessVector);
						}
						if (processingDataBox->mSatInfluencedNominalSet) {
							getSaturationInfluencedNominalSet(true)->initInfluencedNominalSet(processingDataBox->mSatInfluencedNominalSet);
						}
						if (processingDataBox->mSatNominalDependentNodeHash) {
							getSaturationNominalDependentNodeHash(true)->initNominalDependentNodeHash(processingDataBox->mSatNominalDependentNodeHash);
						}
						if (processingDataBox->mSatCriticalIndiNodeConTestSet) {
							getSaturationCriticalIndividualNodeConceptTestSet(true)->initIndividualNodeConceptTestSet(processingDataBox->mSatCriticalIndiNodeConTestSet);
						}
						if (processingDataBox->mSatSuccExtIndNodeProcQueue) {
							getSaturationSucessorExtensionIndividualNodeProcessingQueue(true)->initProcessingQueue(processingDataBox->mSatSuccExtIndNodeProcQueue);
						}
						if (processingDataBox->mSatCriticalIndiNodeProcQueue) {
							getSaturationCriticalIndividualNodeProcessingQueue(true)->initProcessingQueue(processingDataBox->mSatCriticalIndiNodeProcQueue);
						}

					}
					mIndiProcessVector->referenceVector(prevIndiProcVec);
					return this;
				}

				bool CProcessingDataBox::hasClashedDescriptorLinker() {
					return mClashedDescriptorLinker != nullptr;
				}

				CClashedDependencyDescriptor* CProcessingDataBox::getClashedDescriptorLinker() {
					return mClashedDescriptorLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setClashedDescriptorLinker(CClashedDependencyDescriptor* clashDesLinker) {
					mClashedDescriptorLinker = clashDesLinker;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::clearIndividualProcessingQueues() {
					clearRoleAssertionProcessingQueue();
					clearDelayingNominalProcessingQueue();
					clearIndividualDepthFirstProcessingQueue();
					clearIndividualImmediatelyProcessingQueue();
					clearVariableBindingConceptBatchProcessingQueue();
					clearIndividualDepthProcessingQueue();
					clearNominalDeterministicProcessingQueue();
					clearNominalProcessingQueue();
					clearIndividualDepthDeterministicExpansionPreprocessingQueue();
					clearIndividualDepthFirstDeterministicExpansionProcessingQueue();
					clearBlockingUpdateReviewProcessingQueue();
					clearBlockedReactivationProcessingQueue();
					clearEarlyIndividualReactivationProcessingQueue();
					clearReusingReviewData();
					clearBlockedResolveIndividualNodes();
					clearSortedNominalNonDeterministicProcessingNodeLinker();
					clearValueSpaceTriggeringProcessingQueue();
					clearDistinctValueSpaceSatisfiabilityCheckingQueue();
					clearIncrementalExpansionInitializingProcessingQueue();
					clearIncrementalCompatibilityCheckingQueue();
					clearIncrementalExpansionIProcessingQueue();
					clearBackendCacheSynchronizationProcessingQueue();
					clearBackendDirectInfluenceExpansionQueue();
					clearBackendIndirectCompatibilityExpansionQueue();
					clearDelayedBackendConceptSetLabelProcessingInitializationQueue();
					clearBackendNeighbourExpansionQueue();
					clearBackendIndividualNeighbourExpansionQueue();
					clearBackendIndividualReuseExpansionQueue();
					clearBackendLateIndividualNeighbourExpansionQueue();
					return this;
				}


				CIndividualVector* CProcessingDataBox::getIndividualVector(bool localize) {
					if (!mLocalIndiVector && localize) {
						mLocalIndiVector = CObjectParameterizingAllocator<CIndividualVector, CContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext), mProcessContext);
						mLocalIndiVector->referenceVector(mUseIndiVector);
						mUseIndiVector = mLocalIndiVector;
					}
					return mUseIndiVector;
				}


				CConceptVector* CProcessingDataBox::getExtendedConceptVector(bool forceLocalisation) {
					if (forceLocalisation && !mLocExtendedConceptVector) {
						mLocExtendedConceptVector = CObjectParameterizingAllocator<CConceptVector,CContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocExtendedConceptVector->referenceVector(mUseExtendedConceptVector);
						mUseExtendedConceptVector = mLocExtendedConceptVector;
					}
					return mUseExtendedConceptVector;
				}


				CConceptNominalSchemaGroundingHash* CProcessingDataBox::getConceptNominalSchemaGroundingHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocGroundingHash) {
						mLocGroundingHash = CObjectParameterizingAllocator<CConceptNominalSchemaGroundingHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocGroundingHash->initConceptNominalSchemaGroundingHash(mUseGroundingHash);
						mUseGroundingHash = mLocGroundingHash;
					}
					return mUseGroundingHash;
				}



				CVariableBindingPathMergingHash* CProcessingDataBox::getVariableBindingPathMergingHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocVarBindingPathMergingHash) {
						mLocVarBindingPathMergingHash = CObjectParameterizingAllocator<CVariableBindingPathMergingHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocVarBindingPathMergingHash->initVariableBindingPathMergingHash(mUseVarBindingPathMergingHash);
						mUseVarBindingPathMergingHash = mLocVarBindingPathMergingHash;
					}
					return mUseVarBindingPathMergingHash;
				}


				CRepresentativeVariableBindingPathSetHash* CProcessingDataBox::getRepresentativeVariableBindingPathSetHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocRepVarBindPathSetHash) {
						mLocRepVarBindPathSetHash = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathSetHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocRepVarBindPathSetHash->initRepresentativeVariableBindingPathSetHash(mUseRepVarBindPathSetHash);
						mUseRepVarBindPathSetHash = mLocRepVarBindPathSetHash;
					}
					return mUseRepVarBindPathSetHash;
				}

				CRepresentativeVariableBindingPathHash* CProcessingDataBox::getRepresentativeVariableBindingPathHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocRepVarBindPathHash) {
						mLocRepVarBindPathHash = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocRepVarBindPathHash->initRepresentativeVariableBindingPathHash(mUseRepVarBindPathHash);
						mUseRepVarBindPathHash = mLocRepVarBindPathHash;
					}
					return mUseRepVarBindPathHash;
				}



				CRepresentativeVariableBindingPathJoiningKeyHash* CProcessingDataBox::getRepresentativeVariableBindingPathJoiningKeyHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocRepVarBindPathJoiningKeyHash) {
						mLocRepVarBindPathJoiningKeyHash = CObjectParameterizingAllocator<CRepresentativeVariableBindingPathJoiningKeyHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocRepVarBindPathJoiningKeyHash->initRepresentativeVariableBindingPathJoiningKeyHash(mUseRepVarBindPathJoiningKeyHash);
						mUseRepVarBindPathJoiningKeyHash = mLocRepVarBindPathJoiningKeyHash;
					}
					return mUseRepVarBindPathJoiningKeyHash;
				}

				CRepresentativeJoiningHash* CProcessingDataBox::getRepresentativeJoiningHash(bool forceLocalisation) {
					if (forceLocalisation && !mLocRepJoiningHash) {
						mLocRepJoiningHash = CObjectParameterizingAllocator<CRepresentativeJoiningHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocRepJoiningHash->initRepresentativeJoiningHash(mUseRepJoiningHash);
						mUseRepJoiningHash = mLocRepJoiningHash;
					}
					return mUseRepJoiningHash;
				}


				CNominalCachingLossReactivationHash* CProcessingDataBox::getNominalCachingLossReactivationHash(bool createOrForceLocalisation) {
					if (createOrForceLocalisation && !mLocNomCachingLossReactHash) {
						mLocNomCachingLossReactHash = CObjectParameterizingAllocator<CNominalCachingLossReactivationHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocNomCachingLossReactHash->initNominalDependentNodeHash(mUseNomCachingLossReactHash);
						mUseNomCachingLossReactHash = mLocNomCachingLossReactHash;
					}
					return mUseNomCachingLossReactHash;
				}


				CMarkerIndividualNodeHash* CProcessingDataBox::getMarkerIndividualNodeHash(bool createOrForceLocalisation) {
					if (createOrForceLocalisation && !mLocMarkerIndiNodeHash) {
						mLocMarkerIndiNodeHash = CObjectParameterizingAllocator<CMarkerIndividualNodeHash,CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext),mProcessContext);
						mLocMarkerIndiNodeHash->initMarkerIndividualNodeHash(mUseMarkerIndiNodeHash);
						mUseMarkerIndiNodeHash = mLocMarkerIndiNodeHash;
					}
					return mUseMarkerIndiNodeHash;
				}


				cint64 CProcessingDataBox::getNextSaturationResolvedSuccessorExtensionIndividualNodeID(bool incremetNextID) {					
					if (mNextSatResSuccExtIndividualNodeID == -1) {
						mNextSatResSuccExtIndividualNodeID = mIndiSaturationProcessVector->getItemCount();
						mNextSatResSuccExtIndividualNodeID = qMax(mNextSatResSuccExtIndividualNodeID, mOntology->getABox()->getIndividualCount());
						COntologyTriplesAssertionsAccessor* triplesAccessor = mOntology->getOntologyTriplesData()->getTripleAssertionAccessor();
						if (triplesAccessor) {
							mNextSatResSuccExtIndividualNodeID = qMax(mNextSatResSuccExtIndividualNodeID, mOntology->getOntologyTriplesData()->getTripleAssertionAccessor()->getMaxIndexedIndividualId());
						}
					}
					cint64 nextPropID = mNextSatResSuccExtIndividualNodeID;
					if (incremetNextID) {
						++mNextSatResSuccExtIndividualNodeID;
					}
					return nextPropID;
				}

				cint64 CProcessingDataBox::getNextIndividualNodeID(bool incremetNextID) {
					cint64 nextPropID = mNextIndividualNodeID;
					if (incremetNextID) {
						++mNextIndividualNodeID;
					}
					return nextPropID;
				}

				CProcessingDataBox* CProcessingDataBox::setFirstPossibleIndividualNodeID(cint64 indiID) {
					mNextIndividualNodeID = indiID;
					return this;
				}


				cint64 CProcessingDataBox::getNextBindingPropagationID(bool incremetNextID) {
					cint64 nextPropID = mNextPropagationID;
					if (incremetNextID) {
						++mNextPropagationID;
					}
					return nextPropID;
				}

				cint64 CProcessingDataBox::getNextVariableBindingPathID(bool incremetNextID) {
					cint64 nextPropID = mNextVariableID;
					if (incremetNextID) {
						++mNextVariableID;
					}
					return nextPropID;
				}

				cint64 CProcessingDataBox::getNextRepresentativeVariableBindingPathID(bool incremetNextID) {
					cint64 nextPropID = mNextRepVariableID;
					if (incremetNextID) {
						++mNextRepVariableID;
					}
					return nextPropID;
				}

				CIndividualProcessingQueue* CProcessingDataBox::getIndividualProcessingQueue(bool create) {
					if (!mLocIndiProcessQueue) {
						mLocIndiProcessQueue = CObjectParameterizingAllocator< CIndividualProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mLocIndiProcessQueue->initProcessingQueue(mUseIndiProcessQueue);
						mUseIndiProcessQueue = mLocIndiProcessQueue;
					}
					return mUseIndiProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIndividualProcessingQueue() {
					mUseIndiProcessQueue = nullptr;
					mLocIndiProcessQueue = nullptr;
					return this;
				}


				CIndividualProcessNodeVector* CProcessingDataBox::getIndividualProcessNodeVector() {
					return mIndiProcessVector;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualProcessNodeVector(CIndividualProcessNodeVector* indiNodeVec) {
					mIndiProcessVector = indiNodeVec;
					return this;
				}


				CConcept* CProcessingDataBox::getOntologyTopConcept() {
					return mOntologyTopConcept;
				}

				CConcept* CProcessingDataBox::getOntologyTopDataRangeConcept() {
					return mOntologyTopDataRangeConcept;
				}

				CConcreteOntology* CProcessingDataBox::getOntology() {
					return mOntology;
				}


				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getIndividualDepthFirstProcessingQueue(bool create) {
					if (!mIndiDepthFirstProcessQueue && create) {
						mIndiDepthFirstProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiDepthFirstProcessQueue->initProcessingQueue(mPrevIndiDepthFirstProcessQueue);
						mUseIndiDepthFirstProcessQueue = mIndiDepthFirstProcessQueue;
					}
					return mUseIndiDepthFirstProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIndividualDepthFirstProcessingQueue() {		
					mIndiDepthFirstProcessQueue = nullptr;
					mUseIndiDepthFirstProcessQueue = nullptr;
					mPrevIndiDepthFirstProcessQueue = nullptr;
					return this;
				}



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getIndividualImmediatelyProcessingQueue(bool create) {
					if (!mIndiImmProcessQueue && create) {
						mIndiImmProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiImmProcessQueue->initProcessingQueue(mPrevIndiImmProcessQueue);
						mUseIndiImmProcessQueue = mIndiImmProcessQueue;
					}
					return mUseIndiImmProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIndividualImmediatelyProcessingQueue() {		
					mIndiImmProcessQueue = nullptr;
					mUseIndiImmProcessQueue = nullptr;
					mPrevIndiImmProcessQueue = nullptr;
					return this;
				}



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getRoleAssertionExpansionProcessingQueue(bool create) {
					if (!mRoleAssertionProcessQueue && create) {
						mRoleAssertionProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mRoleAssertionProcessQueue->initProcessingQueue(mPrevRoleAssertionProcessQueue);
						mUseRoleAssertionProcessQueue = mRoleAssertionProcessQueue;
					}
					return mUseRoleAssertionProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearRoleAssertionProcessingQueue() {		
					mRoleAssertionProcessQueue = nullptr;
					mUseRoleAssertionProcessQueue = nullptr;
					mPrevRoleAssertionProcessQueue = nullptr;
					return this;
				}




				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getBackendCacheSynchronizationProcessingQueue(bool create) {
					if (!mBackendSyncRetestProcessQueue && create) {
						mBackendSyncRetestProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBackendSyncRetestProcessQueue->initProcessingQueue(mPrevBackendSyncRetestProcessQueue);
						mUseBackendSyncRetestProcessQueue = mBackendSyncRetestProcessQueue;
					}
					return mUseBackendSyncRetestProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendCacheSynchronizationProcessingQueue() {		
					mBackendSyncRetestProcessQueue = nullptr;
					mUseBackendSyncRetestProcessQueue = nullptr;
					mPrevBackendSyncRetestProcessQueue = nullptr;
					return this;
				}




				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getBackendDirectInfluenceExpansionQueue(bool create) {
					if (!mBackendDirectInfluenceExpansionQueue && create) {
						mBackendDirectInfluenceExpansionQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendDirectInfluenceExpansionQueue->initProcessingQueue(mPrevBackendDirectInfluenceExpansionQueue);
						mUseBackendDirectInfluenceExpansionQueue = mBackendDirectInfluenceExpansionQueue;
					}
					return mUseBackendDirectInfluenceExpansionQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendDirectInfluenceExpansionQueue() {
					mBackendDirectInfluenceExpansionQueue = nullptr;
					mUseBackendDirectInfluenceExpansionQueue = nullptr;
					mPrevBackendDirectInfluenceExpansionQueue = nullptr;
					return this;
				}



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getBackendIndirectCompatibilityExpansionQueue(bool create) {
					if (!mBackendIndirectCompatibilityExpansionQueue && create) {
						mBackendIndirectCompatibilityExpansionQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendIndirectCompatibilityExpansionQueue->initProcessingQueue(mPrevBackendIndirectCompatibilityExpansionQueue);
						mUseBackendIndirectCompatibilityExpansionQueue = mBackendIndirectCompatibilityExpansionQueue;
					}
					return mUseBackendIndirectCompatibilityExpansionQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendIndirectCompatibilityExpansionQueue() {
					mBackendIndirectCompatibilityExpansionQueue = nullptr;
					mUseBackendIndirectCompatibilityExpansionQueue = nullptr;
					mPrevBackendIndirectCompatibilityExpansionQueue = nullptr;
					return this;
				}








				bool CProcessingDataBox::isBackendIndividualLateReuseExpansionActivated() {
					return mBackendIndividualLateReuseExpansionActivated;
				}

				CProcessingDataBox* CProcessingDataBox::setBackendIndividualLateReuseExpansionActivated(bool activated) {
					mBackendIndividualLateReuseExpansionActivated = activated;
					return this;
				}



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getBackendIndividualReuseExpansionQueue(bool create) {
					if (!mBackendLateIndividualReuseExpansionQueue && create) {
						mBackendLateIndividualReuseExpansionQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendLateIndividualReuseExpansionQueue->initProcessingQueue(mPrevBackendLateIndividualReuseExpansionQueue);
						mUseBackendLateIndividualReuseExpansionQueue = mBackendLateIndividualReuseExpansionQueue;
					}
					return mUseBackendLateIndividualReuseExpansionQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendIndividualReuseExpansionQueue() {
					mBackendLateIndividualReuseExpansionQueue = nullptr;
					mUseBackendLateIndividualReuseExpansionQueue = nullptr;
					mPrevBackendLateIndividualReuseExpansionQueue = nullptr;
					return this;
				}








				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getBackendLateIndividualNeighbourExpansionQueue(bool create) {
					if (!mBackendIndividualReuseExpansionQueue && create) {
						mBackendIndividualReuseExpansionQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendIndividualReuseExpansionQueue->initProcessingQueue(mPrevBackendIndividualReuseExpansionQueue);
						mUseBackendIndividualReuseExpansionQueue = mBackendIndividualReuseExpansionQueue;
					}
					return mUseBackendIndividualReuseExpansionQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendLateIndividualNeighbourExpansionQueue() {
					mBackendIndividualReuseExpansionQueue = nullptr;
					mUseBackendIndividualReuseExpansionQueue = nullptr;
					mPrevBackendIndividualReuseExpansionQueue = nullptr;
					return this;
				}













				CIndividualLinkerRotationProcessingQueue* CProcessingDataBox::getBackendIndividualNeighbourExpansionQueue(bool create) {
					if (!mBackendIndividualNeighbourExpansionQueue && create) {
						mBackendIndividualNeighbourExpansionQueue = CObjectParameterizingAllocator< CIndividualLinkerRotationProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendIndividualNeighbourExpansionQueue->initProcessingQueue(mPrevBackendIndividualNeighbourExpansionQueue);
						mUseBackendIndividualNeighbourExpansionQueue = mBackendIndividualNeighbourExpansionQueue;
					}
					return mUseBackendIndividualNeighbourExpansionQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendIndividualNeighbourExpansionQueue() {
					mBackendIndividualNeighbourExpansionQueue = nullptr;
					mUseBackendIndividualNeighbourExpansionQueue = nullptr;
					mPrevBackendIndividualNeighbourExpansionQueue = nullptr;
					return this;
				}





				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getDelayingNominalProcessingQueue(bool create) {
					if (!mDelayNomProcessQueue && create) {
						mDelayNomProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mDelayNomProcessQueue->initProcessingQueue(mPrevDelayNomProcessQueue);
						mUseDelayNomProcessQueue = mDelayNomProcessQueue;
					}
					return mUseDelayNomProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearDelayingNominalProcessingQueue() {		
					mDelayNomProcessQueue = nullptr;
					mUseDelayNomProcessQueue = nullptr;
					mPrevDelayNomProcessQueue = nullptr;
					return this;
				}



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getNominalCachingLossReactivationProcessingQueue(bool create) {
					if (!mCachingLossReactivationProcessQueue && create) {
						mCachingLossReactivationProcessQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mCachingLossReactivationProcessQueue->initProcessingQueue(mPrevCachingLossReactivationProcessQueue);
						mUseCachingLossReactivationProcessQueue = mCachingLossReactivationProcessQueue;
					}
					return mUseCachingLossReactivationProcessQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearNominalCachingLossReactivationProcessingQueue() {		
					mCachingLossReactivationProcessQueue = nullptr;
					mUseCachingLossReactivationProcessQueue = nullptr;
					mPrevCachingLossReactivationProcessQueue = nullptr;
					return this;
				}


				CIndividualConceptBatchProcessingQueue* CProcessingDataBox::getVariableBindingConceptBatchProcessingQueue(bool create) {
					if (!mVarBindConceptBatchProcessQueue && create) {
						mVarBindConceptBatchProcessQueue = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mVarBindConceptBatchProcessQueue->initProcessingQueue(mPrevVarBindConceptBatchProcessQueue);
						mUseVarBindConceptBatchProcessQueue = mVarBindConceptBatchProcessQueue;
					}
					return mUseVarBindConceptBatchProcessQueue;
				}

				CProcessingDataBox* CProcessingDataBox::clearVariableBindingConceptBatchProcessingQueue() {
					mVarBindConceptBatchProcessQueue = nullptr;
					mUseVarBindConceptBatchProcessQueue = nullptr;
					mPrevVarBindConceptBatchProcessQueue = nullptr;
					return this;
				}



				CIndividualDepthProcessingQueue* CProcessingDataBox::getIndividualDepthProcessingQueue(bool create) {
					if (!mIndiDepthProcessingQueue && create) {
						mIndiDepthProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiDepthProcessingQueue->initProcessingQueue(mPrevIndiDepthProcessingQueue);
						mUseIndiDepthProcessingQueue = mIndiDepthProcessingQueue;
					}
					return mUseIndiDepthProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIndividualDepthProcessingQueue() {
					mIndiDepthProcessingQueue = nullptr;
					mUseIndiDepthProcessingQueue = nullptr;
					mPrevIndiDepthProcessingQueue = nullptr;
					return this;
				}







				CIndividualDepthProcessingQueue* CProcessingDataBox::getNominalDeterministicProcessingQueue(bool create) {
					if (!mNominalDeterministicProcessingQueue && create) {
						mNominalDeterministicProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mNominalDeterministicProcessingQueue->initProcessingQueue(mPrevNominalDeterministicProcessingQueue);
						mUseNominalDeterministicProcessingQueue = mNominalDeterministicProcessingQueue;
					}
					return mUseNominalDeterministicProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearNominalDeterministicProcessingQueue() {
					mNominalDeterministicProcessingQueue = nullptr;
					mUseNominalDeterministicProcessingQueue = nullptr;
					mPrevNominalDeterministicProcessingQueue = nullptr;
					return this;
				}




				CIndividualDepthProcessingQueue* CProcessingDataBox::getNominalProcessingQueue(bool create) {
					if (!mNominalProcessingQueue && create) {
						mNominalProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mNominalProcessingQueue->initProcessingQueue(mPrevNominalProcessingQueue);
						mUseNominalProcessingQueue = mNominalProcessingQueue;
					}
					return mUseNominalProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearNominalProcessingQueue() {
					mNominalProcessingQueue = nullptr;
					mUseNominalProcessingQueue = nullptr;
					mPrevNominalProcessingQueue = nullptr;
					return this;
				}






				CIndividualDepthProcessingQueue* CProcessingDataBox::getIncrementalExpansionInitializingProcessingQueue(bool create) {
					if (!mIncrementalExansionInitializingProcessingQueue && create) {
						mIncrementalExansionInitializingProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIncrementalExansionInitializingProcessingQueue->initProcessingQueue(mPrevIncrementalExansionInitializingProcessingQueue);
						mUseIncrementalExansionInitializingProcessingQueue = mIncrementalExansionInitializingProcessingQueue;
					}
					return mUseIncrementalExansionInitializingProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIncrementalExpansionInitializingProcessingQueue() {
					mIncrementalExansionInitializingProcessingQueue = nullptr;
					mUseIncrementalExansionInitializingProcessingQueue = nullptr;
					mPrevIncrementalExansionInitializingProcessingQueue = nullptr;
					return this;
				}





				CIndividualCustomPriorityProcessingQueue* CProcessingDataBox::getIncrementalExpansionProcessingQueue(bool create) {
					if (!mIncrementalExansionProcessingQueue && create) {
						mIncrementalExansionProcessingQueue = CObjectParameterizingAllocator< CIndividualCustomPriorityProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIncrementalExansionProcessingQueue->initProcessingQueue(mPrevIncrementalExansionProcessingQueue);
						mUseIncrementalExansionProcessingQueue = mIncrementalExansionProcessingQueue;
					}
					return mUseIncrementalExansionProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIncrementalExpansionIProcessingQueue() {
					mIncrementalExansionProcessingQueue = nullptr;
					mUseIncrementalExansionProcessingQueue = nullptr;
					mPrevIncrementalExansionProcessingQueue = nullptr;
					return this;
				}





				CIndividualDepthProcessingQueue* CProcessingDataBox::getIncrementalCompatibilityCheckingQueue(bool create) {
					if (!mIncrementalCompatibilityCheckingQueue && create) {
						mIncrementalCompatibilityCheckingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIncrementalCompatibilityCheckingQueue->initProcessingQueue(mPrevIncrementalCompatibilityCheckingQueue);
						mUseIncrementalCompatibilityCheckingQueue = mIncrementalCompatibilityCheckingQueue;
					}
					return mUseIncrementalCompatibilityCheckingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIncrementalCompatibilityCheckingQueue() {
					mIncrementalCompatibilityCheckingQueue = nullptr;
					mUseIncrementalCompatibilityCheckingQueue = nullptr;
					mPrevIncrementalCompatibilityCheckingQueue = nullptr;
					return this;
				}




				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getIndividualDepthFirstDeterministicExpansionProcessingQueue(bool create) {
					if (!mIndiDepthFirstDetExpPreProcessingQueue && create) {
						mIndiDepthFirstDetExpPreProcessingQueue = CObjectParameterizingAllocator< CIndividualUnsortedProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiDepthFirstDetExpPreProcessingQueue->initProcessingQueue(mPrevIndiDepthFirstDetExpPreProcessingQueue);
						mUseIndiDepthFirstDetExpPreProcessingQueue = mIndiDepthFirstDetExpPreProcessingQueue;
					}
					return mUseIndiDepthFirstDetExpPreProcessingQueue;
				}

				CProcessingDataBox* CProcessingDataBox::clearIndividualDepthFirstDeterministicExpansionProcessingQueue() {
					mIndiDepthFirstDetExpPreProcessingQueue = nullptr;
					mUseIndiDepthFirstDetExpPreProcessingQueue = nullptr;
					mPrevIndiDepthFirstDetExpPreProcessingQueue = nullptr;
					return this;
				}



				CIndividualDepthProcessingQueue* CProcessingDataBox::getIndividualDepthDeterministicExpansionPreprocessingQueue(bool create) {
					if (!mIndiDepthDetExpPreProcessingQueue && create) {
						mIndiDepthDetExpPreProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiDepthDetExpPreProcessingQueue->initProcessingQueue(mPrevIndiDepthDetExpPreProcessingQueue);
						mUseIndiDepthDetExpPreProcessingQueue = mIndiDepthDetExpPreProcessingQueue;
					}
					return mUseIndiDepthDetExpPreProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearIndividualDepthDeterministicExpansionPreprocessingQueue() {
					mIndiDepthDetExpPreProcessingQueue = nullptr;
					mUseIndiDepthDetExpPreProcessingQueue = nullptr;
					mPrevIndiDepthDetExpPreProcessingQueue = nullptr;
					return this;
				}



				CIndividualDepthProcessingQueue* CProcessingDataBox::getBlockingUpdateReviewProcessingQueue(bool create) {
					if (!mIndiSignatureBlockingUpdateProcessingQueue && create) {
						mIndiSignatureBlockingUpdateProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiSignatureBlockingUpdateProcessingQueue->initProcessingQueue(mPrevIndiSignatureBlockingUpdateProcessingQueue);
						mUseIndiSignatureBlockingUpdateProcessingQueue = mIndiSignatureBlockingUpdateProcessingQueue;
					}
					return mUseIndiSignatureBlockingUpdateProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBlockingUpdateReviewProcessingQueue() {
					mIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					mUseIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					mPrevIndiSignatureBlockingUpdateProcessingQueue = nullptr;
					return this;
				}


				CIndividualDepthProcessingQueue* CProcessingDataBox::getBlockedReactivationProcessingQueue(bool create) {
					if (!mIndiBlockedReactivationProcessingQueue && create) {
						mIndiBlockedReactivationProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiBlockedReactivationProcessingQueue->initProcessingQueue(mPrevIndiBlockedReactivationProcessingQueue);
						mUseIndiBlockedReactivationProcessingQueue = mIndiBlockedReactivationProcessingQueue;
					}
					return mUseIndiBlockedReactivationProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearBlockedReactivationProcessingQueue() {
					mIndiBlockedReactivationProcessingQueue = nullptr;
					mUseIndiBlockedReactivationProcessingQueue = nullptr;
					mPrevIndiBlockedReactivationProcessingQueue = nullptr;
					return this;
				}




				CIndividualDepthProcessingQueue* CProcessingDataBox::getValueSpaceTriggeringProcessingQueue(bool create) {
					if (!mValueSpaceTriggeringProcessingQueue && create) {
						mValueSpaceTriggeringProcessingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mValueSpaceTriggeringProcessingQueue->initProcessingQueue(mPrevValueSpaceTriggeringProcessingQueue);
						mUseValueSpaceTriggeringProcessingQueue = mValueSpaceTriggeringProcessingQueue;
					}
					return mUseValueSpaceTriggeringProcessingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearValueSpaceTriggeringProcessingQueue() {
					mValueSpaceTriggeringProcessingQueue = nullptr;
					mUseValueSpaceTriggeringProcessingQueue = nullptr;
					mPrevValueSpaceTriggeringProcessingQueue = nullptr;
					return this;
				}






				CIndividualDepthProcessingQueue* CProcessingDataBox::getDistinctValueSpaceSatisfiabilityCheckingQueue(bool create) {
					if (!mDistinctValueSpaceSatisfiabilityCheckingQueue && create) {
						mDistinctValueSpaceSatisfiabilityCheckingQueue = CObjectParameterizingAllocator< CIndividualDepthProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mDistinctValueSpaceSatisfiabilityCheckingQueue->initProcessingQueue(mPrevDistinctValueSpaceSatisfiabilityCheckingQueue);
						mUseDistinctValueSpaceSatisfiabilityCheckingQueue = mDistinctValueSpaceSatisfiabilityCheckingQueue;
					}
					return mUseDistinctValueSpaceSatisfiabilityCheckingQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearDistinctValueSpaceSatisfiabilityCheckingQueue() {
					mDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;
					mUseDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;
					mPrevDistinctValueSpaceSatisfiabilityCheckingQueue = nullptr;
					return this;
				}










				CProcessingDataBox* CProcessingDataBox::setSignatureBlockingCandidateHash(CSignatureBlockingCandidateHash* signatureHash) {
					mSignatureBlockingCandidateHash = signatureHash;
					mUseSignatureBlockingCandidateHash = signatureHash;
					return this;
				}


				CSignatureBlockingCandidateHash* CProcessingDataBox::getSignatureBlockingCandidateHash(bool create) {
					if (!mSignatureBlockingCandidateHash && create) {
						mSignatureBlockingCandidateHash = CObjectParameterizingAllocator< CSignatureBlockingCandidateHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSignatureBlockingCandidateHash->initSignatureBlockingCandidateHash(mPrevSignatureBlockingCandidateHash);
						mUseSignatureBlockingCandidateHash = mSignatureBlockingCandidateHash;
					}
					return mUseSignatureBlockingCandidateHash;
				}



				CSignatureBlockingCandidateHash* CProcessingDataBox::getSignatureNominalDelayingCandidateHash(bool create) {
					if (!mSignatureNominalDelayingCandidateHash && create) {
						mSignatureNominalDelayingCandidateHash = CObjectParameterizingAllocator< CSignatureBlockingCandidateHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSignatureNominalDelayingCandidateHash->initSignatureBlockingCandidateHash(mPrevSignatureNominalDelayingCandidateHash);
						mUseSignatureNominalDelayingCandidateHash = mSignatureNominalDelayingCandidateHash;
					}
					return mUseSignatureNominalDelayingCandidateHash;
				}



				CBlockingIndividualNodeCandidateHash* CProcessingDataBox::getBlockingIndividualNodeCandidateHash(bool create) {
					if (!mBlockingIndiNodeCandidateHash && create) {
						mBlockingIndiNodeCandidateHash = CObjectParameterizingAllocator< CBlockingIndividualNodeCandidateHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBlockingIndiNodeCandidateHash->initBlockingIndividualNodeCandidateHash(mPrevBlockingIndiNodeCandidateHash);
						mUseBlockingIndiNodeCandidateHash = mBlockingIndiNodeCandidateHash;
					}
					return mUseBlockingIndiNodeCandidateHash;
				}

				CBlockingIndividualNodeLinkedCandidateHash* CProcessingDataBox::getBlockingIndividualNodeLinkedCandidateHash(bool create) {
					if (!mBlockingIndiNodeLinkedCandidateHash && create) {
						mBlockingIndiNodeLinkedCandidateHash = CObjectParameterizingAllocator< CBlockingIndividualNodeLinkedCandidateHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBlockingIndiNodeLinkedCandidateHash->initBlockingIndividualNodeCandidateHash(mPrevBlockingIndiNodeLinkedCandidateHash);
						mUseBlockingIndiNodeLinkedCandidateHash = mBlockingIndiNodeLinkedCandidateHash;
					}
					return mUseBlockingIndiNodeLinkedCandidateHash;
				}

				CSignatureBlockingReviewSet* CProcessingDataBox::getSignatureBlockingReviewSet(bool create) {
					if (!mSignatureBlockingReviewSet && create) {
						mSignatureBlockingReviewSet = CObjectParameterizingAllocator< CSignatureBlockingReviewSet,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSignatureBlockingReviewSet->initSignatureBlockingReviewSet(mPrevSignatureBlockingReviewSet);
						mUseSignatureBlockingReviewSet = mSignatureBlockingReviewSet;
					}
					return mUseSignatureBlockingReviewSet;
				}

				CIndividualReactivationProcessingQueue* CProcessingDataBox::getEarlyIndividualReactivationProcessingQueue(bool create) {
					if (!mEarlyIndiReactProQueue && create) {
						mEarlyIndiReactProQueue = CObjectParameterizingAllocator< CIndividualReactivationProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mEarlyIndiReactProQueue->initProcessingQueue(mPrevEarlyIndiReactProQueue);
						mUseEarlyIndiReactProQueue = mEarlyIndiReactProQueue;
					}
					return mUseEarlyIndiReactProQueue;
				}

				CProcessingDataBox* CProcessingDataBox::clearEarlyIndividualReactivationProcessingQueue() {
					mEarlyIndiReactProQueue = nullptr;
					mUseEarlyIndiReactProQueue = nullptr;
					mPrevEarlyIndiReactProQueue = nullptr;
					return this;
				}





				CIndividualReactivationProcessingQueue* CProcessingDataBox::getLateIndividualReactivationProcessingQueue(bool create) {
					if (!mLateIndiReactProQueue && create) {
						mLateIndiReactProQueue = CObjectParameterizingAllocator< CIndividualReactivationProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mLateIndiReactProQueue->initProcessingQueue(mPrevLateIndiReactProQueue);
						mUseLateIndiReactProQueue = mLateIndiReactProQueue;
					}
					return mUseLateIndiReactProQueue;
				}

				CProcessingDataBox* CProcessingDataBox::clearLateIndividualReactivationProcessingQueue() {
					mLateIndiReactProQueue = nullptr;
					mUseLateIndiReactProQueue = nullptr;
					mPrevLateIndiReactProQueue = nullptr;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::clearSignatureBlockingReviewSet() {
					mUseSignatureBlockingReviewSet = nullptr;
					mSignatureBlockingReviewSet = nullptr;
					mPrevSignatureBlockingReviewSet = nullptr;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::clearReusingReviewData() {
					mReusingReviewSet = nullptr;
					mUseReusingReviewSet = nullptr;
					mPrevReusingReviewSet = nullptr;
					return this;
				}

				CReusingReviewData* CProcessingDataBox::getReusingReviewData(bool create) {
					if (!mReusingReviewSet && create) {
						mReusingReviewSet = CObjectParameterizingAllocator< CReusingReviewData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mReusingReviewSet->initReviewData(mPrevReusingReviewSet);
						mUseReusingReviewSet = mReusingReviewSet;
					}
					return mUseReusingReviewSet;
				}

				CNodeSwitchHistory* CProcessingDataBox::getNodeSwitchHistory(bool create) {
					if (!mNodeSwitchHistory && create) {
						mNodeSwitchHistory = CObjectParameterizingAllocator< CNodeSwitchHistory,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mNodeSwitchHistory->initSwitchHistory(mPrevNodeSwitchHistory);
						mUseNodeSwitchHistory = mNodeSwitchHistory;
					}
					return mUseNodeSwitchHistory;
				}

				CBranchingTree* CProcessingDataBox::getBranchingTree(bool create) {
					if (!mBranchingTree && create) {
						mBranchingTree = CObjectParameterizingAllocator< CBranchingTree,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBranchingTree->initBranchingTree(mPrevBranchingTree);
						mUseBranchingTree = mBranchingTree;
					}
					return mUseBranchingTree;
				}

				bool CProcessingDataBox::hasCacheTestingIndividualNodes() {
					return mIndividualNodeCacheTestingLinker != nullptr;
				}

				CIndividualProcessNode* CProcessingDataBox::takeNextCacheTestingIndividualNode() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeCacheTestingLinker = mIndividualNodeCacheTestingLinker;
					if (mIndividualNodeCacheTestingLinker) {
						mIndividualNodeCacheTestingLinker = mIndividualNodeCacheTestingLinker->getNext();
					}
					return tmpIndividualNodeCacheTestingLinker->getData();
				}


				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::takeIndividualNodeCacheTestingLinker() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeCacheTestingLinker = mIndividualNodeCacheTestingLinker;
					mIndividualNodeCacheTestingLinker = nullptr;
					return tmpIndividualNodeCacheTestingLinker;
				}

				CProcessingDataBox* CProcessingDataBox::addIndividualNodeCacheTestingLinker(CXLinker<CIndividualProcessNode*>* linker) {
					if (linker) {
						mIndividualNodeCacheTestingLinker = linker->append(mIndividualNodeCacheTestingLinker);
					}
					return this;
				}






				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::getSortedNominalNonDeterministicProcessingNodeLinker() {
					return mSortedNominalNonDetProcessingNodeLinker;
				}


				bool CProcessingDataBox::hasSortedNominalNonDeterministicProcessingNodes() {
					return mSortedNominalNonDetProcessingNodeLinker != nullptr;
				}

				CIndividualProcessNode* CProcessingDataBox::takeSortedNominalNonDeterministicProcessingNode() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeCacheTestingLinker = mSortedNominalNonDetProcessingNodeLinker;
					if (mSortedNominalNonDetProcessingNodeLinker) {
						--mNominalNonDetProcessingCount;
						mSortedNominalNonDetProcessingNodeLinker = mSortedNominalNonDetProcessingNodeLinker->getNext();
					}
					return tmpIndividualNodeCacheTestingLinker->getData();
				}


				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::takeSortedNominalNonDeterministicProcessingNodeLinker() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeCacheTestingLinker = mSortedNominalNonDetProcessingNodeLinker;
					mSortedNominalNonDetProcessingNodeLinker = nullptr;
					return tmpIndividualNodeCacheTestingLinker;
				}

				CProcessingDataBox* CProcessingDataBox::addSortedNominalNonDeterministicProcessingNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					if (linker) {
						mNominalNonDetProcessingCount += linker->getCount();
						mSortedNominalNonDetProcessingNodeLinker = linker->append(mSortedNominalNonDetProcessingNodeLinker);
					}
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::setSortedNominalNonDeterministicProcessingNodeLinker(CXLinker<CIndividualProcessNode*>* linker) {
					mNominalNonDetProcessingCount = linker->getCount();
					mSortedNominalNonDetProcessingNodeLinker = linker;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::clearSortedNominalNonDeterministicProcessingNodeLinker() {
					mSortedNominalNonDetProcessingNodeLinker = nullptr;
					mNominalNonDetProcessingCount = 0;
					return this;
				}


				bool CProcessingDataBox::hasNominalNonDeterministicProcessingNodesSorted() {
					return mSortedNominalNonDetProcessingNodesSorted;
				}

				CProcessingDataBox* CProcessingDataBox::setNominalNonDeterministicProcessingNodesSorted(bool sorted) {
					mSortedNominalNonDetProcessingNodesSorted = sorted;
					return this;
				}






				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::takeNextIndividualNodeBlockedResolveLinker() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeBlockResolveLinker = mIndividualNodeResolveLinker;
					if (mIndividualNodeResolveLinker) {
						mIndividualNodeResolveLinker = mIndividualNodeResolveLinker->getNext();
					}
					return tmpIndividualNodeBlockResolveLinker;
				}

				bool CProcessingDataBox::hasBlockedResolveIndividualNodes() {
					return mIndividualNodeResolveLinker != nullptr;
				}

				CProcessingDataBox* CProcessingDataBox::clearBlockedResolveIndividualNodes() {
					mIndividualNodeResolveLinker = nullptr;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addIndividualNodeBlockedResolveLinker(CXLinker<CIndividualProcessNode*>* linker) {
					if (linker) {
						mIndividualNodeResolveLinker = linker->append(mIndividualNodeResolveLinker);
					}
					return this;
				}








				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::getBlockableIndividualNodeUpdatedLinker() {
					CXLinker<CIndividualProcessNode*>* tmpIndividualNodeBlockResolveLinker = mBlockableIndividualNodeUpdatedLinker;
					if (mBlockableIndividualNodeUpdatedLinker) {
						mBlockableIndividualNodeUpdatedLinker = mBlockableIndividualNodeUpdatedLinker->getNext();
					}
					return tmpIndividualNodeBlockResolveLinker;
				}

				bool CProcessingDataBox::hasBlockableIndividualNodeUpdatedLinker() {
					return mBlockableIndividualNodeUpdatedLinker != nullptr;
				}

				CProcessingDataBox* CProcessingDataBox::clearBlockableIndividualNodeUpdatedLinker() {
					mBlockableIndividualNodeUpdatedLinker = nullptr;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addBlockableIndividualNodeUpdatedLinker(CXLinker<CIndividualProcessNode*>* linker) {
					if (linker) {
						mBlockableIndividualNodeUpdatedLinker = linker->append(mBlockableIndividualNodeUpdatedLinker);
					}
					return this;
				}





				CProcessingDataBox* CProcessingDataBox::setMultipleConstructionIndividualNodes(bool multiple) {
					mMultipleConstructionIndiNodes = multiple;
					return this;
				}

				bool CProcessingDataBox::hasMultipleConstructionIndividualNodes() {
					return mMultipleConstructionIndiNodes;
				}



				CIndividualProcessNode* CProcessingDataBox::getConstructedIndividualNode() {
					return mConstructedIndiNode;
				}

				CProcessingDataBox* CProcessingDataBox::setConstructedIndividualNode(CIndividualProcessNode* indiNode) {
					mConstructedIndiNode = indiNode;
					return this;
				}


				bool CProcessingDataBox::hasConstructedIndividualNodeInitialized() {
					return mConstructedIndiNodeInitialized;
				}

				CProcessingDataBox* CProcessingDataBox::setConstructedIndividualNodeInitialized(bool initialized) {
					mConstructedIndiNodeInitialized = initialized;
					return this;
				}


				CProcessContext* CProcessingDataBox::getProcessContext() {
					return mProcessContext;
				}

				CProcessingDataBox* CProcessingDataBox::setLastProcessingIndividualNodeAndConceptDescriptor(CIndividualProcessNode* indiNode, CConceptProcessDescriptor* conDes) {
					mLastProcessingIndiNode = indiNode;
					mLastProcessingConDes = conDes;
					return this;
				}

				bool CProcessingDataBox::getLastProcessingIndividualNodeAndConceptDescriptor(CIndividualProcessNode*& indiNode, CConceptProcessDescriptor*& conDes) {
					if (mLastProcessingIndiNode && mLastProcessingConDes) {
						indiNode = mLastProcessingIndiNode;
						conDes = mLastProcessingConDes;
						return true;
					}
					return false;
				}

				bool CProcessingDataBox::hasLastProcessingIndividualNodeAndConceptDescriptor() {
					return mLastProcessingConDes && mLastProcessingIndiNode;
				}

				bool CProcessingDataBox::isReapplicationLastConceptDesciptorOnLastIndividualNodeRequired() {
					return mLastConDesIndiReapplication;
				}

				CProcessingDataBox* CProcessingDataBox::setReapplicationLastConceptDesciptorOnLastIndividualNodeRequired(bool requiresReapplication) {
					mLastConDesIndiReapplication = requiresReapplication;
					return this;
				}

				CIndividualProcessNodeLinker* CProcessingDataBox::getIndividualProcessNodeLinker() {
					return mIndiProcessNodeLinker;
				}

				CIndividualProcessNodeLinker* CProcessingDataBox::takeIndividualProcessNodeLinker() {
					CIndividualProcessNodeLinker* indiProcNodeLinker = mIndiProcessNodeLinker;
					if (mIndiProcessNodeLinker) {
						mIndiProcessNodeLinker = mIndiProcessNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualProcessNodeLinker(CIndividualProcessNodeLinker* indiProcessNodeLinker) {
					mIndiProcessNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualProcessNodeLinker(CIndividualProcessNodeLinker* indiProcessNodeLinker) {
					mIndiProcessNodeLinker = indiProcessNodeLinker->append(mIndiProcessNodeLinker);
					return this;
				}



				CProcessingDataBox* CProcessingDataBox::setMaximumDeterministicBranchTag(cint64 branchTag) {
					mMaximumDeterministicBranchTag = branchTag;
					return this;
				}

				cint64 CProcessingDataBox::getMaximumDeterministicBranchTag() {
					return mMaximumDeterministicBranchTag;
				}




				CIndividualSaturationProcessNodeVector* CProcessingDataBox::getIndividualSaturationProcessNodeVector(bool create) {
					if (!mIndiSaturationProcessVector && create) {
						mIndiSaturationProcessVector = CObjectParameterizingAllocator< CIndividualSaturationProcessNodeVector,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mIndiSaturationProcessVector;
				}

				bool CProcessingDataBox::hasIndividualSaturationProcessNodeLinker() {
					return mIndiSaturationProcessNodeLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getIndividualSaturationProcessNodeLinker() {
					return mIndiSaturationProcessNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeIndividualSaturationProcessNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mIndiSaturationProcessNodeLinker;
					if (mIndiSaturationProcessNodeLinker) {
						mIndiSaturationProcessNodeLinker = mIndiSaturationProcessNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationProcessNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationProcessNodeLinker = indiProcessNodeLinker->append(mIndiSaturationProcessNodeLinker);
					return this;
				}




				bool CProcessingDataBox::hasIndividualDisjunctCommonConceptExtractProcessLinker() {
					return mDisjunctCommonConceptExtractProcessingLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getIndividualDisjunctCommonConceptExtractProcessLinker() {
					return mDisjunctCommonConceptExtractProcessingLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeIndividualDisjunctCommonConceptExtractProcessLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mDisjunctCommonConceptExtractProcessingLinker;
					if (mDisjunctCommonConceptExtractProcessingLinker) {
						mDisjunctCommonConceptExtractProcessingLinker = mDisjunctCommonConceptExtractProcessingLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualDisjunctCommonConceptExtractProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mDisjunctCommonConceptExtractProcessingLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualDisjunctCommonConceptExtractProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mDisjunctCommonConceptExtractProcessingLinker = indiProcessNodeLinker->append(mDisjunctCommonConceptExtractProcessingLinker);
					return this;
				}




				CConceptSaturationProcessLinker* CProcessingDataBox::getRemainingConceptSaturationProcessLinker() {
					return mRemConSatProcessLinker;
				}

				CConceptSaturationProcessLinker* CProcessingDataBox::takeRemainingConceptSaturationProcessLinker() {
					CConceptSaturationProcessLinker* conSatProLinker = mRemConSatProcessLinker;
					if (mRemConSatProcessLinker) {
						mRemConSatProcessLinker = mRemConSatProcessLinker->getNext();
						conSatProLinker->clearNext();
					}
					return conSatProLinker;
				}

				CIndividualSaturationProcessNodeStatusUpdateLinker* CProcessingDataBox::getRemainingIndividualSaturationUpdateLinker() {
					return mRemSatUpdateLinker;
				}

				CIndividualSaturationProcessNodeStatusUpdateLinker* CProcessingDataBox::takeRemainingIndividualSaturationUpdateLinker() {
					CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdLinker = mRemSatUpdateLinker;
					if (mRemSatUpdateLinker) {
						mRemSatUpdateLinker = mRemSatUpdateLinker->getNext();
						conSatUpdLinker->clearNext();
					}
					return conSatUpdLinker;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* indiSaturationUpdateLinker) {
					mRemSatUpdateLinker = indiSaturationUpdateLinker->append(mRemSatUpdateLinker);
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* indiSaturationUpdateLinker) {
					mRemSatUpdateLinker = indiSaturationUpdateLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::setRemainingConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conSatProcessLinker) {
					mRemConSatProcessLinker = conSatProcessLinker;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conSatProcessLinker) {
					mRemConSatProcessLinker = conSatProcessLinker->append(mRemConSatProcessLinker);
					return this;
				}

				CConceptDescriptor* CProcessingDataBox::getRemainingConceptDescriptor() {
					return mRemConDes;
				}

				CConceptDescriptor* CProcessingDataBox::takeRemainingConceptDescriptor() {
					CConceptDescriptor* conDes = mRemConDes;
					if (mRemConDes) {
						mRemConDes = mRemConDes->getNext();
						conDes->clearNext();
					}
					return conDes;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingConceptDescriptor(CConceptDescriptor* conDes) {
					mRemConDes = conDes;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingConceptDescriptor(CConceptDescriptor* conDes) {
					mRemConDes = conDes->append(mRemConDes);
					return this;
				}






				CConceptSaturationDescriptor* CProcessingDataBox::getRemainingConceptSaturationDescriptor() {
					return mRemConSatDes;
				}

				CConceptSaturationDescriptor* CProcessingDataBox::takeRemainingConceptSaturationDescriptor() {
					CConceptSaturationDescriptor* conSatDes = mRemConSatDes;
					if (mRemConSatDes) {
						mRemConSatDes = mRemConSatDes->getNext();
						conSatDes->clearNext();
					}
					return conSatDes;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes) {
					mRemConSatDes = conSatDes;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes) {
					mRemConSatDes = conSatDes->append(mRemConSatDes);
					return this;
				}



				CRoleSaturationProcessLinker* CProcessingDataBox::getRemainingRoleSaturationProcessLinker() {
					return mRemRoleSatProcessLinker;
				}

				CRoleSaturationProcessLinker* CProcessingDataBox::takeRemainingRoleSaturationProcessLinker() {
					CRoleSaturationProcessLinker* roleProDes = mRemRoleSatProcessLinker;
					if (mRemRoleSatProcessLinker) {
						mRemRoleSatProcessLinker = mRemRoleSatProcessLinker->getNext();
						roleProDes->clearNext();
					}
					return roleProDes;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					mRemRoleSatProcessLinker = roleProcessLinker;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker) {
					mRemRoleSatProcessLinker = roleProcessLinker->append(mRemRoleSatProcessLinker);
					return this;
				}








				CCriticalIndividualNodeConceptTestSet* CProcessingDataBox::getSaturationCriticalIndividualNodeConceptTestSet(bool create) {
					if (!mSatCriticalIndiNodeConTestSet && create) {
						mSatCriticalIndiNodeConTestSet = CObjectParameterizingAllocator< CCriticalIndividualNodeConceptTestSet,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSatCriticalIndiNodeConTestSet->initIndividualNodeConceptTestSet(nullptr);
					}
					return mSatCriticalIndiNodeConTestSet;
				}




				CSaturationNominalDependentNodeHash* CProcessingDataBox::getSaturationNominalDependentNodeHash(bool create) {
					if (!mSatNominalDependentNodeHash && create) {
						mSatNominalDependentNodeHash = CObjectParameterizingAllocator< CSaturationNominalDependentNodeHash,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSatNominalDependentNodeHash->initNominalDependentNodeHash(nullptr);
					}
					return mSatNominalDependentNodeHash;
				}



				CSaturationInfluencedNominalSet* CProcessingDataBox::getSaturationInfluencedNominalSet(bool create) {
					if (!mSatInfluencedNominalSet && create) {
						mSatInfluencedNominalSet = CObjectParameterizingAllocator< CSaturationInfluencedNominalSet,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSatInfluencedNominalSet->initInfluencedNominalSet(nullptr);
					}
					return mSatInfluencedNominalSet;
				}




				CSaturationSuccessorExtensionIndividualNodeProcessingQueue* CProcessingDataBox::getSaturationSucessorExtensionIndividualNodeProcessingQueue(bool create) {
					if (!mSatSuccExtIndNodeProcQueue && create) {
						mSatSuccExtIndNodeProcQueue = CObjectParameterizingAllocator< CSaturationSuccessorExtensionIndividualNodeProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSatSuccExtIndNodeProcQueue->initProcessingQueue(nullptr);
					}
					return mSatSuccExtIndNodeProcQueue;
				}

				CCriticalIndividualNodeProcessingQueue* CProcessingDataBox::getSaturationCriticalIndividualNodeProcessingQueue(bool create) {
					if (!mSatCriticalIndiNodeProcQueue && create) {
						mSatCriticalIndiNodeProcQueue = CObjectParameterizingAllocator< CCriticalIndividualNodeProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mSatCriticalIndiNodeProcQueue->initProcessingQueue(nullptr);
					}
					return mSatCriticalIndiNodeProcQueue;
				}


				bool CProcessingDataBox::isInsufficientNodeOccured() {
					return mInsufficientNodeOccured;
				}

				CProcessingDataBox* CProcessingDataBox::setInsufficientNodeOccured(bool insufficientNodeOccured) {
					mInsufficientNodeOccured = insufficientNodeOccured;
					return this;
				}

				bool CProcessingDataBox::isProblematicEQCandidateOccured() {
					return mProblematicEQCandidateNodeOccured;
				}

				CProcessingDataBox* CProcessingDataBox::setProblematicEQCandidateOccured(bool problematicNodeOccured) {
					mProblematicEQCandidateNodeOccured = problematicNodeOccured;
					return this;
				}


				bool CProcessingDataBox::isDelayedNominalProcessingOccured() {
					return mDelayedNominalProcessingOccured;
				}

				CProcessingDataBox* CProcessingDataBox::setDelayedNominalProcessingOccured(bool delayedNominalProcessingOccured) {
					mDelayedNominalProcessingOccured = delayedNominalProcessingOccured;
					return this;
				}



				bool CProcessingDataBox::hasIndividualSaturationCompletionNodeLinker() {
					return mIndiSaturationCompletionNodeLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getIndividualSaturationCompletionNodeLinker() {
					return mIndiSaturationCompletionNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeIndividualSaturationCompletionNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mIndiSaturationCompletionNodeLinker;
					if (mIndiSaturationCompletionNodeLinker) {
						mIndiSaturationCompletionNodeLinker = mIndiSaturationCompletionNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationCompletionNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationCompletionNodeLinker = indiProcessNodeLinker->append(mIndiSaturationCompletionNodeLinker);
					return this;
				}







				bool CProcessingDataBox::hasIndividualSaturationCompletedNodeLinker() {
					return mIndiSaturationCompletedNodeLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getIndividualSaturationCompletedNodeLinker() {
					return mIndiSaturationCompletedNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeIndividualSaturationCompletedNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mIndiSaturationCompletedNodeLinker;
					if (mIndiSaturationCompletedNodeLinker) {
						mIndiSaturationCompletedNodeLinker = mIndiSaturationCompletedNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualSaturationCompletedNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationCompletedNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualSaturationCompletedNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationCompletedNodeLinker = indiProcessNodeLinker->append(mIndiSaturationCompletedNodeLinker);
					return this;
				}







				bool CProcessingDataBox::hasIndividualSaturationAnalysationNodeLinker() {
					return mIndiSaturationAnalysingNodeLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getIndividualSaturationAnalysationNodeLinker() {
					return mIndiSaturationAnalysingNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeIndividualSaturationAnalysationNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mIndiSaturationAnalysingNodeLinker;
					if (mIndiSaturationAnalysingNodeLinker) {
						mIndiSaturationAnalysingNodeLinker = mIndiSaturationAnalysingNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualSaturationAnalysationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationAnalysingNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualSaturationAnalysationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mIndiSaturationAnalysingNodeLinker = indiProcessNodeLinker->append(mIndiSaturationAnalysingNodeLinker);
					return this;
				}








				bool CProcessingDataBox::hasNominalDelayedIndividualSaturationProcessNodeLinker() {
					return mNominalDelayedIndiSaturationProcessNodeLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getNominalDelayedIndividualSaturationProcessNodeLinker() {
					return mNominalDelayedIndiSaturationProcessNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeNominalDelayedIndividualSaturationProcessNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mNominalDelayedIndiSaturationProcessNodeLinker;
					if (mNominalDelayedIndiSaturationProcessNodeLinker) {
						mNominalDelayedIndiSaturationProcessNodeLinker = mNominalDelayedIndiSaturationProcessNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mNominalDelayedIndiSaturationProcessNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mNominalDelayedIndiSaturationProcessNodeLinker = indiProcessNodeLinker->append(mNominalDelayedIndiSaturationProcessNodeLinker);
					return this;
				}









				bool CProcessingDataBox::hasSaturationATMOSTMergingProcessLinker() {
					return mSaturationATMOSTMergingProcessLinker != nullptr;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getSaturationATMOSTMergingProcessLinker() {
					return mSaturationATMOSTMergingProcessLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeSaturationATMOSTMergingProcessLinker() {
					CIndividualSaturationProcessNodeLinker* indiProcNodeLinker = mSaturationATMOSTMergingProcessLinker;
					if (mSaturationATMOSTMergingProcessLinker) {
						mSaturationATMOSTMergingProcessLinker = mSaturationATMOSTMergingProcessLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setSaturationATMOSTMergingProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mSaturationATMOSTMergingProcessLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addSaturationATMOSTMergingProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mSaturationATMOSTMergingProcessLinker = indiProcessNodeLinker->append(mSaturationATMOSTMergingProcessLinker);
					return this;
				}












				CIndividualSaturationProcessNode* CProcessingDataBox::getSeparatedSaturationConceptAssertionResolveNode() {
					return mSeparatedSaturationConAssResolveNode;
				}

				CProcessingDataBox* CProcessingDataBox::setSeparatedSaturationConceptAssertionResolveNode(CIndividualSaturationProcessNode* node) {
					mSeparatedSaturationConAssResolveNode = node;
					return this;
				}


				bool CProcessingDataBox::isIncrementalExpansionInitialised() {
					return mIncrementalExpansionInitialized;
				}

				CProcessingDataBox* CProcessingDataBox::setIncrementalExpansionInitialised(bool initialized) {
					mIncrementalExpansionInitialized = initialized;
					return this;
				}

				cint64 CProcessingDataBox::getIncrementalExpansionID() {
					return mIncrementalExpID;
				}

				CProcessingDataBox* CProcessingDataBox::setIncrementalExpansionID(cint64 incExpID) {
					mIncrementalExpID = incExpID;
					return this;
				}


				cint64 CProcessingDataBox::getMaxIncrementalPreviousCompletionGraphNodeID() {
					return mMaxIncPrevCompGraphNodeID;
				}

				CProcessingDataBox* CProcessingDataBox::setMaxIncrementalPreviousCompletionGraphNodeID(cint64 nodeID) {
					mMaxIncPrevCompGraphNodeID = nodeID;
					return this;
				}

				cint64 CProcessingDataBox::getNextIncrementalIndividualExpansionID(bool incrementNextID) {
					cint64 nextID = mNextIncrementalIndiExpID;
					if (incrementNextID) {
						++mNextIncrementalIndiExpID;
					}
					return nextID;
				}

				cint64 CProcessingDataBox::getNextRoleAssertionCreationID(bool incrementNextID) {
					cint64 nextID = mNextRoleAssertionCreationID;
					if (incrementNextID) {
						++mNextRoleAssertionCreationID;
					}
					return nextID;
				}


				bool CProcessingDataBox::isIncrementalExpansionCompatibleMerged() {
					return mIncrementalExpansionCompatibleMerged;
				}

				CProcessingDataBox* CProcessingDataBox::setIncrementalExpansionCompatibleMerged(bool compatibleMerged) {
					mIncrementalExpansionCompatibleMerged = compatibleMerged;
					return this;
				}

				bool CProcessingDataBox::isIncrementalExpansionCachingMerged() {
					return mIncrementalExpansionCachingMerged;
				}

				CProcessingDataBox* CProcessingDataBox::setIncrementalExpansionCachingMerged(bool cachingMerged) {
					mIncrementalExpansionCachingMerged = cachingMerged;
					return this;
				}

				CReferredIndividualTrackingVector* CProcessingDataBox::getReferredIndividualTrackingVector() {
					return mReferredIndiTrackVec;
				}


				CProcessingDataBox* CProcessingDataBox::setReferredIndividualTrackingVector(CReferredIndividualTrackingVector* refIndiTrackVec) {
					mReferredIndiTrackVec = refIndiTrackVec;
					return this;
				}

				bool CProcessingDataBox::isIndividualDependenceTrackingRequired() {
					return mIndiDepTrackingRequired;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired) {
					mIndiDepTrackingRequired = indiDepTrackingRequired;
					return this;
				}







				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::getRemainingIndividualSaturationNodeLinker() {
					return mRemSatIndiNodeLinker;
				}

				CIndividualSaturationProcessNodeLinker* CProcessingDataBox::takeRemainingIndividualSaturationNodeLinker() {
					CIndividualSaturationProcessNodeLinker* indiNodeSatLinker = mRemSatIndiNodeLinker;
					if (mRemSatIndiNodeLinker) {
						mRemSatIndiNodeLinker = mRemSatIndiNodeLinker->getNext();
						indiNodeSatLinker->clearNext();
					}
					return indiNodeSatLinker;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingIndividualSaturationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mRemSatIndiNodeLinker = indiProcessNodeLinker->append(mRemSatIndiNodeLinker);
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker) {
					mRemSatIndiNodeLinker = indiProcessNodeLinker;
					return this;
				}









				CIndividualSaturationSuccessorLinkDataLinker* CProcessingDataBox::getRemainingIndividualSuccessorLinkDataLinker() {
					return mRemSatIndiSuccLinkDataLinker;
				}

				CIndividualSaturationSuccessorLinkDataLinker* CProcessingDataBox::takeRemainingIndividualSuccessorLinkDataLinker() {
					CIndividualSaturationSuccessorLinkDataLinker* satSuccLinkDataLinker = mRemSatIndiSuccLinkDataLinker;
					if (mRemSatIndiSuccLinkDataLinker) {
						mRemSatIndiSuccLinkDataLinker = mRemSatIndiSuccLinkDataLinker->getNext();
						satSuccLinkDataLinker->clearNext();
					}
					return satSuccLinkDataLinker;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingIndividualSuccessorLinkDataLinker(CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker) {
					mRemSatIndiSuccLinkDataLinker = succLinkDataLinker->append(mRemSatIndiSuccLinkDataLinker);
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingIndividualSuccessorLinkDataLinker(CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker) {
					mRemSatIndiSuccLinkDataLinker = succLinkDataLinker;
					return this;
				}


				bool CProcessingDataBox::hasBranchingInstruction() {
					return mBranchingInstruction != nullptr;
				}

				CBranchingInstruction* CProcessingDataBox::getBranchingInstruction() {
					return mBranchingInstruction;
				}

				CProcessingDataBox* CProcessingDataBox::setBranchingInstruction(CBranchingInstruction* branchingInstruction) {
					mBranchingInstruction = branchingInstruction;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::clearBranchingInstruction() {
					mBranchingInstruction = nullptr;
					return this;
				}

				CXLinker<cint64>* CProcessingDataBox::getLastMergedPossibleInstanceIndividualLinker() {
					return mLastMergedPossibleInstanceIndividualLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setLastMergedPossibleInstanceIndividualLinker(CXLinker<cint64>* lastMergedPossibleInstanceLinker) {
					mLastMergedPossibleInstanceIndividualLinker = lastMergedPossibleInstanceLinker;
					return this;
				}



				CXLinker<CXLinker<cint64>*>* CProcessingDataBox::getCurrentMergedPossibleInstanceIndividualLinkersLinker() {
					return mCurrentMergedPossibleInstanceIndividualLinkersLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setCurrentMergedPossibleInstanceIndividualLinkersLinker(CXLinker<CXLinker<cint64>*>* mergedPossibleInstanceLinker) {
					mCurrentMergedPossibleInstanceIndividualLinkersLinker = mergedPossibleInstanceLinker;
					return this;
				}


				cint64 CProcessingDataBox::getRemainingPossibleInstanceIndividualMergingLimit() {
					return mRemainingPossibleInstanceIndividualMergingLimit;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingPossibleInstanceIndividualMergingLimit(cint64 limit) {
					mRemainingPossibleInstanceIndividualMergingLimit = limit;
					return this;
				}





				cint64 CProcessingDataBox::getPossibleInstanceIndividualMergedCount() {
					return mPossibleInstanceIndividualMergedCount;
				}

				CProcessingDataBox* CProcessingDataBox::setPossibleInstanceIndividualMergedCount(cint64 count) {
					mPossibleInstanceIndividualMergedCount = count;
					return this;
				}



				cint64 CProcessingDataBox::getPossibleInstanceIndividualCurrentMergingCount() {
					return mPossibleInstanceIndividualCurrentMergingCount;
				}

				CProcessingDataBox* CProcessingDataBox::setPossibleInstanceIndividualCurrentMergingCount(cint64 count) {
					mPossibleInstanceIndividualCurrentMergingCount = count;
					return this;
				}




				cint64 CProcessingDataBox::getPossibleInstanceIndividualMergingSize() {
					return mPossibleInstanceIndividualMergingSize;
				}

				CProcessingDataBox* CProcessingDataBox::setPossibleInstanceIndividualMergingSize(cint64 limit) {
					mPossibleInstanceIndividualMergingSize = limit;
					return this;
				}




				bool CProcessingDataBox::isPossibleInstanceIndividualMergingStopped() {
					return mPossibleInstanceIndividualMergingStopped;
				}

				CProcessingDataBox* CProcessingDataBox::setPossibleInstanceIndividualMergingStopped(bool stopped) {
					mPossibleInstanceIndividualMergingStopped = stopped;
					return this;
				}


				cint64 CProcessingDataBox::getBackendCacheIntegratedIndividualNodeCount() {
					return mBackendCacheIntegratedIndividualNodeCount;
				}

				CProcessingDataBox* CProcessingDataBox::incBackendCacheIntegratedIndividualNodeCount(cint64 count) {
					mBackendCacheIntegratedIndividualNodeCount += count;
					return this;
				}




				cint64 CProcessingDataBox::getBackendCacheIntegratedSameIndividualNodeCount() {
					return mBackendCacheIntegratedSameIndividualNodeCount;
				}

				CProcessingDataBox* CProcessingDataBox::incBackendCacheIntegratedSameIndividualNodeCount(cint64 count) {
					mBackendCacheIntegratedSameIndividualNodeCount += count;
					return this;
				}



				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::getLastBackendCacheIntegratedIndividualNodeLinker() {
					return mLastBackendCacheIntegratedIndiNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setLastBackendCacheIntegratedIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mLastBackendCacheIntegratedIndiNodeLinker = indiLinker;
					mBackendCacheIntegratedIndividualNodeCount = indiLinker->getCount();
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addLastBackendCacheIntegratedIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mBackendCacheIntegratedIndividualNodeCount += indiLinker->getCount();
					mLastBackendCacheIntegratedIndiNodeLinker = indiLinker->append(mLastBackendCacheIntegratedIndiNodeLinker);
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::setBackendCacheLoadedAssociationHash(CIndividualRepresentativeBackendCacheLoadedAssociationHash* backendCacheLoadedAssociationHash) {
					mUseBackendLoadedAssociationHash = backendCacheLoadedAssociationHash;
					mLocBackendLoadedAssociationHash = backendCacheLoadedAssociationHash;
					return this;
				}


				CIndividualRepresentativeBackendCacheLoadedAssociationHash* CProcessingDataBox::getBackendCacheLoadedAssociationHash(bool createOrForceLocalisation) {
					if (createOrForceLocalisation && !mLocBackendLoadedAssociationHash) {
						mLocBackendLoadedAssociationHash = CObjectParameterizingAllocator<CIndividualRepresentativeBackendCacheLoadedAssociationHash, CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext), mProcessContext);
						mLocBackendLoadedAssociationHash->initIndividualRepresentativeBackendCacheLoadedAssociationHash(mUseBackendLoadedAssociationHash);
						mUseBackendLoadedAssociationHash = mLocBackendLoadedAssociationHash;
					}
					return mUseBackendLoadedAssociationHash;
				}


				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHash* CProcessingDataBox::getBackendCacheConceptSetLabelProcessingHash(bool createOrForceLocalisation) {
					if (createOrForceLocalisation && !mLocBackendConceptSetLabelProcessingHash) {
						mLocBackendConceptSetLabelProcessingHash = CObjectParameterizingAllocator<CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHash, CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext), mProcessContext);
						mLocBackendConceptSetLabelProcessingHash->initIndividualRepresentativeBackendCacheConceptSetLabelProcessingHash(mUseBackendConceptSetLabelProcessingHash);
						mUseBackendConceptSetLabelProcessingHash = mLocBackendConceptSetLabelProcessingHash;
					}
					return mUseBackendConceptSetLabelProcessingHash;
				}




				CIndividualDelayedBackendInitializationProcessingQueue* CProcessingDataBox::getDelayedBackendConceptSetLabelProcessingInitializationQueue(bool create) {
					if (!mDelayedBackendInitProcQueue && create) {
						mDelayedBackendInitProcQueue = CObjectParameterizingAllocator< CIndividualDelayedBackendInitializationProcessingQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mDelayedBackendInitProcQueue->initProcessingQueue(mPrevDelayedBackendInitProcQueue);
						mUseDelayedBackendInitProcQueue = mDelayedBackendInitProcQueue;
					}
					return mUseDelayedBackendInitProcQueue;
				}


				CProcessingDataBox* CProcessingDataBox::clearDelayedBackendConceptSetLabelProcessingInitializationQueue() {
					mDelayedBackendInitProcQueue = nullptr;
					mUseDelayedBackendInitProcQueue = nullptr;
					mPrevDelayedBackendInitProcQueue = nullptr;
					return this;
				}


				CBackendNeighbourExpansionControllingData* CProcessingDataBox::getBackendNeighbourExpansionControllingData(bool createOrLocalize) {
					if (createOrLocalize && !mLocBackendNeighbourExpansionControllingData) {
						mLocBackendNeighbourExpansionControllingData = CObjectParameterizingAllocator<CBackendNeighbourExpansionControllingData, CProcessContext*>::allocateAndConstructAndParameterize(CContext::getMemoryAllocationManager(mProcessContext), mProcessContext);
						mLocBackendNeighbourExpansionControllingData->initExpansionControllingData(mUseBackendNeighbourExpansionControllingData);
						mUseBackendNeighbourExpansionControllingData = mLocBackendNeighbourExpansionControllingData;
					}
					return mUseBackendNeighbourExpansionControllingData;
				}



				CBackendNeighbourExpansionQueue* CProcessingDataBox::getBackendNeighbourExpansionQueue(bool create) {
					if (!mBackendNeighbourExpansionQueue && create) {
						mBackendNeighbourExpansionQueue = CObjectParameterizingAllocator< CBackendNeighbourExpansionQueue, CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(), mProcessContext);
						mBackendNeighbourExpansionQueue->initBackendNeighbourExpansionQueue(mPrevBackendNeighbourExpansion);
						mUseBackendNeighbourExpansion = mBackendNeighbourExpansionQueue;
					}
					return mUseBackendNeighbourExpansion;
				}


				CProcessingDataBox* CProcessingDataBox::clearBackendNeighbourExpansionQueue() {
					mBackendNeighbourExpansionQueue = nullptr;
					mUseBackendNeighbourExpansion = nullptr;
					mPrevBackendNeighbourExpansion = nullptr;
					return this;
				}

				bool CProcessingDataBox::hasBackendCacheUpdateIndividualsInitialized() {
					return mBackendCacheUpdateIndividualsInitialized;
				}

				CProcessingDataBox* CProcessingDataBox::setBackendCacheUpdateIndividualsInitialized(bool initialized) {
					mBackendCacheUpdateIndividualsInitialized = initialized;
					return this;
				}


				CXLinker<CIndividualProcessNode*>* CProcessingDataBox::getRepresentativeNeighbourExpansionIndividualNodeLinker() {
					return mRepresentativeNeighbourExpansionIndividualNodeLinker;
				}
				CProcessingDataBox* CProcessingDataBox::addRepresentativeNeighbourExpansionIndividualNodeLinker(CXLinker<CIndividualProcessNode*>* indiLinker) {
					mRepresentativeNeighbourExpansionIndividualNodeLinker = indiLinker->append(mRepresentativeNeighbourExpansionIndividualNodeLinker);
					return this;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
