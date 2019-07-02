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

#include "CProcessingDataBox.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CProcessingDataBox::CProcessingDataBox(CProcessContext* processContext) : mProcessContext(processContext) {
					mIndiProcessVector = CObjectParameterizingAllocator< CIndividualProcessNodeVector,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					mOntologyTopConcept = nullptr;
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
					mIndiReactProQueue = nullptr;
					mSignatureBlockingReviewSet = nullptr;
					mReusingReviewSet = nullptr;
					mUseSignatureBlockingReviewSet = nullptr;
					mUseIndiReactProQueue = nullptr;
					mUseReusingReviewSet = nullptr;
					mPrevSignatureBlockingReviewSet = nullptr;
					mPrevIndiReactProQueue = nullptr;
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
					mConstructedIndiNode = nullptr;
					mLastConDesIndiReapplication = false;
					mLastProcessingIndiNode = nullptr;
					mLastProcessingConDes = nullptr;
					mIndiProcessNodeLinker = nullptr;
					mIndiPilingProcessNodeLinker = nullptr;
					mRemConPilProcessLinker = nullptr;
					mRemConPilDes = nullptr;
					mIndiPilingProcessVector = nullptr;
					mIndiSaturationProcessNodeLinker = nullptr;
					mIndiSaturationCompletionNodeLinker = nullptr;
					mNominalDelayedIndiSaturationProcessNodeLinker = nullptr;
					mDisjunctCommonConceptExtractProcessingLinker = nullptr;
					mRemConSatProcessLinker = nullptr;
					mRemSatUpdateLinker = nullptr;
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
					mConstructedIndiNodeInitialized = false;
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

					mIndiDepthProcessingQueue = nullptr;
					mUseIndiDepthProcessingQueue = nullptr;
					mPrevIndiDepthProcessingQueue = nullptr;

					mNominalProcessingQueue = nullptr;
					mUseNominalProcessingQueue = nullptr;
					mPrevNominalProcessingQueue = nullptr;

					mIndiDepthDetExpPreProcessingQueue = nullptr;
					mUseIndiDepthDetExpPreProcessingQueue = nullptr;
					mPrevIndiDepthDetExpPreProcessingQueue = nullptr;
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
					mClashedDescriptorLinker = nullptr;
				}


				CProcessingDataBox* CProcessingDataBox::initProcessingDataBox(CConcreteOntology* ontology) {
					mOntology = ontology;
					mOntologyTopConcept = mOntology->getTBox()->getTopConcept();
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
					mLastConDesIndiReapplication = false;
					mIndiProcessNodeLinker = nullptr;
					mIndiPilingProcessNodeLinker = nullptr;
					mRemConPilProcessLinker = nullptr;
					mRemConPilDes = nullptr;
					mIndiPilingProcessVector = nullptr;
					mRemSatUpdateLinker = nullptr;
					mRemConSatDes = nullptr;
					mRemRoleSatProcessLinker = nullptr;

					mIndiSaturationProcessNodeLinker = nullptr;
					mIndiSaturationCompletionNodeLinker = nullptr;
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
					mNextIndividualNodeID = 0;
					mNextSatResSuccExtIndividualNodeID = -1;
					mNextPropagationID = 1;
					mNextVariableID = 1;
					mNextRepVariableID = 1;
					mInsufficientNodeOccured = false;
					mDelayedNominalProcessingOccured = false;
					mProblematicEQCandidateNodeOccured = false;
					if (processingDataBox) {
						mUseIndiProcessQueue = processingDataBox->mUseIndiProcessQueue;
						prevIndiProcVec = processingDataBox->mIndiProcessVector;
						mOntology = processingDataBox->mOntology;
						mPrevIndiDepthProcessingQueue = processingDataBox->mUseIndiDepthProcessingQueue;
						mUseIndiDepthProcessingQueue = mPrevIndiDepthProcessingQueue;
						mPrevNominalProcessingQueue = processingDataBox->mUseNominalProcessingQueue;
						mUseNominalProcessingQueue = mPrevNominalProcessingQueue;
						mPrevIndiDepthDetExpPreProcessingQueue = processingDataBox->mUseIndiDepthDetExpPreProcessingQueue;
						mUseIndiDepthDetExpPreProcessingQueue = mPrevIndiDepthDetExpPreProcessingQueue;
						mPrevIndiSignatureBlockingUpdateProcessingQueue = processingDataBox->mUseIndiBlockedReactivationProcessingQueue;
						mUseIndiBlockedReactivationProcessingQueue = mPrevIndiSignatureBlockingUpdateProcessingQueue;
						mPrevVarBindConceptBatchProcessQueue = processingDataBox->mUseVarBindConceptBatchProcessQueue;
						mUseVarBindConceptBatchProcessQueue = mPrevVarBindConceptBatchProcessQueue;
						mPrevIndiBlockedReactivationProcessingQueue = processingDataBox->mUseIndiSignatureBlockingUpdateProcessingQueue;
						mUseIndiSignatureBlockingUpdateProcessingQueue = mPrevIndiBlockedReactivationProcessingQueue;
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
						mPrevIndiReactProQueue = processingDataBox->mUseIndiReactProQueue;
						mUseIndiReactProQueue = mPrevIndiReactProQueue;
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
						mPrevBranchingTree = processingDataBox->mUseBranchingTree;
						mUseBranchingTree = mPrevBranchingTree;
						mOntologyTopConcept = processingDataBox->mOntologyTopConcept;
						mIndividualNodeCacheTestingLinker = processingDataBox->mIndividualNodeCacheTestingLinker;
						mSortedNominalNonDetProcessingNodeLinker = processingDataBox->mSortedNominalNonDetProcessingNodeLinker;
						mSortedNominalNonDetProcessingNodesSorted = processingDataBox->mSortedNominalNonDetProcessingNodesSorted;
						mNominalNonDetProcessingCount = processingDataBox->mNominalNonDetProcessingCount;
						mIndividualNodeResolveLinker = processingDataBox->mIndividualNodeResolveLinker;
						mConstructedIndiNode = processingDataBox->mConstructedIndiNode;
						mLastProcessingIndiNode = processingDataBox->mLastProcessingIndiNode;
						mLastProcessingConDes = processingDataBox->mLastProcessingConDes;
						mIndiProcessNodeLinker = processingDataBox->mIndiProcessNodeLinker;
						mIndiPilingProcessNodeLinker = processingDataBox->mIndiPilingProcessNodeLinker;
						mRemConPilProcessLinker = processingDataBox->mRemConPilProcessLinker;
						mRemConPilDes = processingDataBox->mRemConPilDes;
						mIndiPilingProcessVector = processingDataBox->mIndiPilingProcessVector;
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


						mIndiSaturationProcessNodeLinker = processingDataBox->mIndiSaturationProcessNodeLinker;
						mIndiSaturationCompletionNodeLinker = processingDataBox->mIndiSaturationCompletionNodeLinker;
						mDisjunctCommonConceptExtractProcessingLinker = processingDataBox->mDisjunctCommonConceptExtractProcessingLinker;
						mRemConSatProcessLinker = processingDataBox->mRemConSatProcessLinker;
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
					clearBlockingUpdateReviewProcessingQueue();
					clearBlockedReactivationProcessingQueue();
					clearIndividualReactivationProcessingQueue();
					clearReusingReviewData();
					clearBlockedResolveIndividualNodes();
					clearSortedNominalNonDeterministicProcessingNodeLinker();
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

				CConcept* CProcessingDataBox::getOntologyTopConcept() {
					return mOntologyTopConcept;
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

				CIndividualReactivationProcessingQueue* CProcessingDataBox::getIndividualReactivationProcessingQueue(bool create) {
					if (!mIndiReactProQueue && create) {
						mIndiReactProQueue = CObjectParameterizingAllocator< CIndividualReactivationProcessingQueue,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIndiReactProQueue->initProcessingQueue(mPrevIndiReactProQueue);
						mUseIndiReactProQueue = mIndiReactProQueue;
					}
					return mUseIndiReactProQueue;
				}

				CProcessingDataBox* CProcessingDataBox::clearIndividualReactivationProcessingQueue() {
					mIndiReactProQueue = nullptr;
					mUseIndiReactProQueue = nullptr;
					mPrevIndiReactProQueue = nullptr;
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
						linker = mIndividualNodeResolveLinker->append(linker);
					}
					return this;
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


				CIndividualPilingProcessNodeVector* CProcessingDataBox::getIndividualPilingProcessNodeVector(bool create) {
					if (!mIndiPilingProcessVector && create) {
						mIndiPilingProcessVector = CObjectParameterizingAllocator< CIndividualPilingProcessNodeVector,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					}
					return mIndiPilingProcessVector;
				}


				CIndividualPilingProcessNodeLinker* CProcessingDataBox::getIndividualPilingProcessNodeLinker() {
					return mIndiPilingProcessNodeLinker;
				}

				CIndividualPilingProcessNodeLinker* CProcessingDataBox::takeIndividualPilingProcessNodeLinker() {
					CIndividualPilingProcessNodeLinker* indiProcNodeLinker = mIndiPilingProcessNodeLinker;
					if (mIndiPilingProcessNodeLinker) {
						mIndiPilingProcessNodeLinker = mIndiPilingProcessNodeLinker->getNext();
						indiProcNodeLinker->clearNext();
					}
					return indiProcNodeLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* indiProcessNodeLinker) {
					mIndiPilingProcessNodeLinker = indiProcessNodeLinker;
					return this;
				}


				CProcessingDataBox* CProcessingDataBox::addIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* indiProcessNodeLinker) {
					mIndiPilingProcessNodeLinker = indiProcessNodeLinker->append(mIndiPilingProcessNodeLinker);
					return this;
				}

				CConceptPilingProcessLinker* CProcessingDataBox::getRemainingConceptPilingProcessLinker() {
					return mRemConPilProcessLinker;
				}

				CConceptPilingProcessLinker* CProcessingDataBox::takeRemainingConceptPilingProcessLinker() {
					CConceptPilingProcessLinker* conPilProLinker = mRemConPilProcessLinker;
					if (mRemConPilProcessLinker) {
						mRemConPilProcessLinker = mRemConPilProcessLinker->getNext();
						conPilProLinker->clearNext();
					}
					return conPilProLinker;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcessLinker) {
					mRemConPilProcessLinker = conPilProcessLinker;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcessLinker) {
					mRemConPilProcessLinker = conPilProcessLinker->append(mRemConPilProcessLinker);
					return this;
				}

				CConceptPilingDescriptor* CProcessingDataBox::getRemainingConceptPilingDescriptor() {
					return mRemConPilDes;
				}

				CConceptPilingDescriptor* CProcessingDataBox::takeRemainingConceptPilingDescriptor() {
					CConceptPilingDescriptor* conPilDes = mRemConPilDes;
					if (mRemConPilDes) {
						mRemConPilDes = mRemConPilDes->getNext();
						conPilDes->clearNext();
					}
					return conPilDes;
				}

				CProcessingDataBox* CProcessingDataBox::setRemainingConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes) {
					mRemConPilDes = conPilDes;
					return this;
				}

				CProcessingDataBox* CProcessingDataBox::addRemainingConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes) {
					mRemConPilDes = conPilDes->append(mRemConPilDes);
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


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
