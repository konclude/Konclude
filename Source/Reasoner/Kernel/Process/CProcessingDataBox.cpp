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
					mIndiSaturationAnalysingNodeLinker = nullptr;
					mNominalDelayedIndiSaturationProcessNodeLinker = nullptr;
					mDisjunctCommonConceptExtractProcessingLinker = nullptr;
					mRemConSatProcessLinker = nullptr;
					mRemSatUpdateLinker = nullptr;
					mRemSatIndiNodeLinker = nullptr;
					mRemSatIndiSuccLinkDataLinker = nullptr;
					mRemConSatDes = nullptr;
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

					mIndiDepthProcessingQueue = nullptr;
					mUseIndiDepthProcessingQueue = nullptr;
					mPrevIndiDepthProcessingQueue = nullptr;

					mNominalProcessingQueue = nullptr;
					mUseNominalProcessingQueue = nullptr;
					mPrevNominalProcessingQueue = nullptr;

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

					mSeparatedSaturationConAssResolveNode = nullptr;
					mIncrementalExpID = 0;
					mMaxIncPrevCompGraphNodeID = 0;
					mNextIncrementalIndiExpID = 1;
					mNextRoleAssertionCreationID = 1;

					mReferredIndiTrackVec = nullptr;
					mIndiDepTrackingRequired = false;
				}


				CProcessingDataBox* CProcessingDataBox::initProcessingDataBox(CConcreteOntology* ontology) {
					mOntology = ontology;
					mOntologyTopConcept = mOntology->getTBox()->getTopConcept();
					mOntologyTopDataRangeConcept = mOntology->getTBox()->getTopDataRangeConcept();
					mUseExtendedConceptVector = mOntology->getTBox()->getConceptVector(false);
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
					mRemRoleSatProcessLinker = nullptr;

					mIndiSaturationProcessNodeLinker = nullptr;
					mIndiSaturationCompletionNodeLinker = nullptr;
					mIndiSaturationAnalysingNodeLinker = nullptr;
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
					if (processingDataBox) {
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
						mPrevIndiSignatureBlockingUpdateProcessingQueue = processingDataBox->mUseIndiBlockedReactivationProcessingQueue;
						mUseIndiBlockedReactivationProcessingQueue = mPrevIndiSignatureBlockingUpdateProcessingQueue;
						mPrevVarBindConceptBatchProcessQueue = processingDataBox->mUseVarBindConceptBatchProcessQueue;
						mUseVarBindConceptBatchProcessQueue = mPrevVarBindConceptBatchProcessQueue;
						mPrevIndiBlockedReactivationProcessingQueue = processingDataBox->mUseIndiSignatureBlockingUpdateProcessingQueue;
						mUseIndiSignatureBlockingUpdateProcessingQueue = mPrevIndiBlockedReactivationProcessingQueue;
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
						mPrevRoleAssertionProcessQueue = processingDataBox->mUseRoleAssertionProcessQueue;
						mUseRoleAssertionProcessQueue = mPrevRoleAssertionProcessQueue;
						mPrevBackendSyncRetestProcessQueue = processingDataBox->mUseBackendSyncRetestProcessQueue;
						mUseBackendSyncRetestProcessQueue = mPrevBackendSyncRetestProcessQueue;
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
						mIndiSaturationAnalysingNodeLinker = processingDataBox->mIndiSaturationAnalysingNodeLinker;
						mDisjunctCommonConceptExtractProcessingLinker = processingDataBox->mDisjunctCommonConceptExtractProcessingLinker;
						mNominalDelayedIndiSaturationProcessNodeLinker = processingDataBox->mNominalDelayedIndiSaturationProcessNodeLinker;

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
					clearRoleAssertionProcessingQueue();
					clearIncrementalExpansionInitializingProcessingQueue();
					clearIncrementalCompatibilityCheckingQueue();
					clearIncrementalExpansionIProcessingQueue();
					clearBackendCacheSynchronizationProcessingQueue();
					return this;
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



				CIndividualUnsortedProcessingQueue* CProcessingDataBox::getRoleAssertionProcessingQueue(bool create) {
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




			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
