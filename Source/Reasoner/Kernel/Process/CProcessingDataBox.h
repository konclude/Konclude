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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGDATABOX_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGDATABOX_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualProcessingQueue.h"
#include "CIndividualProcessNodeVector.h"
#include "CSignatureBlockingCandidateHash.h"
#include "CBlockingIndividualNodeCandidateHash.h"
#include "CNodeSwitchHistory.h"
#include "CSignatureBlockingReviewSet.h"
#include "CIndividualPilingProcessNodeLinker.h"
#include "CIndividualPilingProcessNodeVector.h"
#include "CIndividualPilingProcessNode.h"
#include "CConceptPilingProcessLinker.h"
#include "CIndividualUnsortedProcessingQueue.h"
#include "CIndividualDepthProcessingQueue.h"
#include "CReusingReviewData.h"
#include "CBlockingIndividualNodeLinkedCandidateHash.h"
#include "CConceptNominalSchemaGroundingHash.h"
#include "CVariableBindingPathMergingHash.h"
#include "CIndividualConceptBatchProcessingQueue.h"
#include "CRepresentativeVariableBindingPathSetHash.h"
#include "CRepresentativeVariableBindingPathHash.h"
#include "CRepresentativeVariableBindingPathJoiningKeyHash.h"
#include "CRepresentativeJoiningHash.h"
#include "CIndividualReactivationProcessingQueue.h"
#include "CIndividualSaturationProcessNodeLinker.h"
#include "CIndividualSaturationProcessNodeVector.h"
#include "CIndividualSaturationProcessNode.h"
#include "CIndividualSaturationProcessNodeStatusUpdateLinker.h"
#include "CCriticalIndividualNodeProcessingQueue.h"
#include "CCriticalIndividualNodeConceptTestSet.h"
#include "CSaturationSuccessorExtensionIndividualNodeProcessingQueue.h"
#include "CSaturationNominalDependentNodeHash.h"
#include "CSaturationInfluencedNominalSet.h"
#include "CNominalCachingLossReactivationHash.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Kernel/Process/Dependency/CBranchingTree.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				using namespace Dependency;

				/*! 
				 *
				 *		\class		CProcessingDataBox
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessingDataBox {
					// public methods
					public:
						//! Constructor
						CProcessingDataBox(CProcessContext* processContext = nullptr);

						CProcessingDataBox* initProcessingDataBox(CConcreteOntology* ontology);
						CProcessingDataBox* initProcessingDataBox(CProcessingDataBox* processingDataBox);


						bool hasClashedDescriptorLinker();
						CClashedDependencyDescriptor* getClashedDescriptorLinker();
						CProcessingDataBox* setClashedDescriptorLinker(CClashedDependencyDescriptor* clashDesLinker);


						CIndividualProcessingQueue* getIndividualProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualProcessingQueue();
						CIndividualProcessNodeVector* getIndividualProcessNodeVector();



						CProcessingDataBox* clearIndividualProcessingQueues();




						CIndividualUnsortedProcessingQueue* getRoleAssertionProcessingQueue(bool create = true);
						CProcessingDataBox* clearRoleAssertionProcessingQueue();

						CIndividualUnsortedProcessingQueue* getDelayingNominalProcessingQueue(bool create = true);
						CProcessingDataBox* clearDelayingNominalProcessingQueue();

						CIndividualUnsortedProcessingQueue* getNominalCachingLossReactivationProcessingQueue(bool create = true);
						CProcessingDataBox* clearNominalCachingLossReactivationProcessingQueue();

						CIndividualUnsortedProcessingQueue* getIndividualDepthFirstProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualDepthFirstProcessingQueue();

						CIndividualUnsortedProcessingQueue* getIndividualImmediatelyProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualImmediatelyProcessingQueue();

						CIndividualConceptBatchProcessingQueue* getVariableBindingConceptBatchProcessingQueue(bool create = true);
						CProcessingDataBox* clearVariableBindingConceptBatchProcessingQueue();


						CIndividualDepthProcessingQueue* getIndividualDepthProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualDepthProcessingQueue();

						CIndividualDepthProcessingQueue* getNominalProcessingQueue(bool create = true);
						CProcessingDataBox* clearNominalProcessingQueue();

						CIndividualDepthProcessingQueue* getIndividualDepthDeterministicExpansionPreprocessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualDepthDeterministicExpansionPreprocessingQueue();

						CIndividualDepthProcessingQueue* getBlockingUpdateReviewProcessingQueue(bool create = true);
						CProcessingDataBox* clearBlockingUpdateReviewProcessingQueue();

						CIndividualDepthProcessingQueue* getBlockedReactivationProcessingQueue(bool create = true);
						CProcessingDataBox* clearBlockedReactivationProcessingQueue();


						CIndividualDepthProcessingQueue* getValueSpaceTriggeringProcessingQueue(bool create = true);
						CProcessingDataBox* clearValueSpaceTriggeringProcessingQueue();


						CIndividualDepthProcessingQueue* getDistinctValueSpaceSatisfiabilityCheckingQueue(bool create = true);
						CProcessingDataBox* clearDistinctValueSpaceSatisfiabilityCheckingQueue();


						CConcept* getOntologyTopConcept();
						CConcept* getOntologyTopDataRangeConcept();
						CConcreteOntology* getOntology();

						CSignatureBlockingCandidateHash* getSignatureNominalDelayingCandidateHash(bool create = true);

						CProcessingDataBox* setSignatureBlockingCandidateHash(CSignatureBlockingCandidateHash* signatureHash);
						CSignatureBlockingCandidateHash* getSignatureBlockingCandidateHash(bool create = true);
						CBlockingIndividualNodeCandidateHash* getBlockingIndividualNodeCandidateHash(bool create = true);
						CBlockingIndividualNodeLinkedCandidateHash* getBlockingIndividualNodeLinkedCandidateHash(bool create = true);
						CSignatureBlockingReviewSet* getSignatureBlockingReviewSet(bool create = true);
						CProcessingDataBox* clearSignatureBlockingReviewSet();
						CIndividualReactivationProcessingQueue* getIndividualReactivationProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualReactivationProcessingQueue();


						CReusingReviewData* getReusingReviewData(bool create = true);
						CProcessingDataBox* clearReusingReviewData();

						CNodeSwitchHistory* getNodeSwitchHistory(bool create = true);
						CBranchingTree* getBranchingTree(bool create = true);


						CIndividualProcessNode* takeNextCacheTestingIndividualNode();
						bool hasCacheTestingIndividualNodes();
						CXLinker<CIndividualProcessNode*>* takeIndividualNodeCacheTestingLinker();
						CProcessingDataBox* addIndividualNodeCacheTestingLinker(CXLinker<CIndividualProcessNode*>* linker);




						CXLinker<CIndividualProcessNode*>* getSortedNominalNonDeterministicProcessingNodeLinker();
						CIndividualProcessNode* takeSortedNominalNonDeterministicProcessingNode();
						bool hasSortedNominalNonDeterministicProcessingNodes();
						CXLinker<CIndividualProcessNode*>* takeSortedNominalNonDeterministicProcessingNodeLinker();
						CProcessingDataBox* addSortedNominalNonDeterministicProcessingNodeLinker(CXLinker<CIndividualProcessNode*>* linker);
						CProcessingDataBox* setSortedNominalNonDeterministicProcessingNodeLinker(CXLinker<CIndividualProcessNode*>* linker);
						CProcessingDataBox* clearSortedNominalNonDeterministicProcessingNodeLinker();

						bool hasNominalNonDeterministicProcessingNodesSorted();
						CProcessingDataBox* setNominalNonDeterministicProcessingNodesSorted(bool sorted);



						CXLinker<CIndividualProcessNode*>* takeNextIndividualNodeBlockedResolveLinker();
						CProcessingDataBox* addIndividualNodeBlockedResolveLinker(CXLinker<CIndividualProcessNode*>* linker);
						bool hasBlockedResolveIndividualNodes();
						CProcessingDataBox* clearBlockedResolveIndividualNodes();


						CProcessingDataBox* setMultipleConstructionIndividualNodes(bool multiple);
						bool hasMultipleConstructionIndividualNodes();

						CIndividualProcessNode* getConstructedIndividualNode();
						CProcessingDataBox* setConstructedIndividualNode(CIndividualProcessNode* indiNode);
						bool hasConstructedIndividualNodeInitialized();
						CProcessingDataBox* setConstructedIndividualNodeInitialized(bool initialized);

						CProcessContext* getProcessContext();

						CProcessingDataBox* setLastProcessingIndividualNodeAndConceptDescriptor(CIndividualProcessNode* indiNode, CConceptProcessDescriptor* conDes);
						bool getLastProcessingIndividualNodeAndConceptDescriptor(CIndividualProcessNode*& indiNode, CConceptProcessDescriptor*& conDes);
						bool hasLastProcessingIndividualNodeAndConceptDescriptor();
						bool isReapplicationLastConceptDesciptorOnLastIndividualNodeRequired();
						CProcessingDataBox* setReapplicationLastConceptDesciptorOnLastIndividualNodeRequired(bool requiresReapplication);

						CIndividualProcessNodeLinker* getIndividualProcessNodeLinker();
						CIndividualProcessNodeLinker* takeIndividualProcessNodeLinker();
						CProcessingDataBox* setIndividualProcessNodeLinker(CIndividualProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualProcessNodeLinker(CIndividualProcessNodeLinker* indiProcessNodeLinker);

						CIndividualPilingProcessNodeVector* getIndividualPilingProcessNodeVector(bool create = true);

						CIndividualPilingProcessNodeLinker* getIndividualPilingProcessNodeLinker();
						CIndividualPilingProcessNodeLinker* takeIndividualPilingProcessNodeLinker();
						CProcessingDataBox* setIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualPilingProcessNodeLinker(CIndividualPilingProcessNodeLinker* indiProcessNodeLinker);

						CConceptPilingProcessLinker* getRemainingConceptPilingProcessLinker();
						CConceptPilingProcessLinker* takeRemainingConceptPilingProcessLinker();
						CProcessingDataBox* setRemainingConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcessLinker);
						CProcessingDataBox* addRemainingConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcessLinker);

						CConceptPilingDescriptor* getRemainingConceptPilingDescriptor();
						CConceptPilingDescriptor* takeRemainingConceptPilingDescriptor();
						CProcessingDataBox* setRemainingConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes);
						CProcessingDataBox* addRemainingConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes);

						CIndividualSaturationProcessNodeVector* getIndividualSaturationProcessNodeVector(bool create = true);

						bool hasIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* getIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeIndividualSaturationProcessNodeLinker();
						CProcessingDataBox* setIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);

						bool hasIndividualDisjunctCommonConceptExtractProcessLinker();
						CIndividualSaturationProcessNodeLinker* getIndividualDisjunctCommonConceptExtractProcessLinker();
						CIndividualSaturationProcessNodeLinker* takeIndividualDisjunctCommonConceptExtractProcessLinker();
						CProcessingDataBox* setIndividualDisjunctCommonConceptExtractProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualDisjunctCommonConceptExtractProcessLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);

						CConceptSaturationProcessLinker* getRemainingConceptSaturationProcessLinker();
						CConceptSaturationProcessLinker* takeRemainingConceptSaturationProcessLinker();
						CProcessingDataBox* setRemainingConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conPilProcessLinker);
						CProcessingDataBox* addRemainingConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conPilProcessLinker);

						CConceptSaturationDescriptor* getRemainingConceptSaturationDescriptor();
						CConceptSaturationDescriptor* takeRemainingConceptSaturationDescriptor();
						CProcessingDataBox* setRemainingConceptSaturationDescriptor(CConceptSaturationDescriptor* conPilDes);
						CProcessingDataBox* addRemainingConceptSaturationDescriptor(CConceptSaturationDescriptor* conPilDes);


						CRoleSaturationProcessLinker* getRemainingRoleSaturationProcessLinker();
						CRoleSaturationProcessLinker* takeRemainingRoleSaturationProcessLinker();
						CProcessingDataBox* setRemainingRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);
						CProcessingDataBox* addRemainingRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleProcessLinker);


						CIndividualSaturationProcessNodeStatusUpdateLinker* getRemainingIndividualSaturationUpdateLinker();
						CIndividualSaturationProcessNodeStatusUpdateLinker* takeRemainingIndividualSaturationUpdateLinker();
						CProcessingDataBox* setRemainingIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* indiSaturationUpdateLinker);
						CProcessingDataBox* addRemainingIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* indiSaturationUpdateLinker);


						CSaturationSuccessorExtensionIndividualNodeProcessingQueue* getSaturationSucessorExtensionIndividualNodeProcessingQueue(bool create = true);
						CCriticalIndividualNodeProcessingQueue* getSaturationCriticalIndividualNodeProcessingQueue(bool create = true);
						CCriticalIndividualNodeConceptTestSet* getSaturationCriticalIndividualNodeConceptTestSet(bool create = true);
						CSaturationNominalDependentNodeHash* getSaturationNominalDependentNodeHash(bool create = true);
						CSaturationInfluencedNominalSet* getSaturationInfluencedNominalSet(bool create = true);





						bool hasIndividualSaturationCompletionNodeLinker();
						CIndividualSaturationProcessNodeLinker* getIndividualSaturationCompletionNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeIndividualSaturationCompletionNodeLinker();
						CProcessingDataBox* setIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualSaturationCompletionNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);




						bool hasNominalDelayedIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* getNominalDelayedIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeNominalDelayedIndividualSaturationProcessNodeLinker();
						CProcessingDataBox* setNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);










						bool isInsufficientNodeOccured();
						CProcessingDataBox* setInsufficientNodeOccured(bool insufficientNodeOccured);

						bool isDelayedNominalProcessingOccured();
						CProcessingDataBox* setDelayedNominalProcessingOccured(bool delayedNominalProcessingOccured);

						bool isProblematicEQCandidateOccured();
						CProcessingDataBox* setProblematicEQCandidateOccured(bool problematicNodeOccured);

						CProcessingDataBox* setMaximumDeterministicBranchTag(cint64 branchTag);
						cint64 getMaximumDeterministicBranchTag();

						cint64 getNextSaturationResolvedSuccessorExtensionIndividualNodeID(bool incremetNextID = true);

						cint64 getNextIndividualNodeID(bool incremetNextID = true);

						cint64 getNextBindingPropagationID(bool incremetNextID = true);

						cint64 getNextVariableBindingPathID(bool incremetNextID = true);

						cint64 getNextRepresentativeVariableBindingPathID(bool incremetNextID = true);

						CConceptVector* getExtendedConceptVector(bool forceLocalisation = true);
						CConceptNominalSchemaGroundingHash* getConceptNominalSchemaGroundingHash(bool forceLocalisation = true);

						CVariableBindingPathMergingHash* getVariableBindingPathMergingHash(bool forceLocalisation = true);

						CRepresentativeVariableBindingPathSetHash* getRepresentativeVariableBindingPathSetHash(bool forceLocalisation = true);
						CRepresentativeVariableBindingPathHash* getRepresentativeVariableBindingPathHash(bool forceLocalisation = true);

						CRepresentativeVariableBindingPathJoiningKeyHash* getRepresentativeVariableBindingPathJoiningKeyHash(bool forceLocalisation = true);
						CRepresentativeJoiningHash* getRepresentativeJoiningHash(bool forceLocalisation = true);


						CNominalCachingLossReactivationHash* getNominalCachingLossReactivationHash(bool createOrForceLocalisation = true);

					// protected methods
					protected:

					// protected variables
					protected:
						CProcessContext* mProcessContext;

						CConcept* mOntologyTopConcept;
						CConcept* mOntologyTopDataRangeConcept;
						CConcreteOntology* mOntology;

						CClashedDependencyDescriptor* mClashedDescriptorLinker;

						CIndividualProcessingQueue* mUseIndiProcessQueue;
						CIndividualProcessingQueue* mLocIndiProcessQueue;
						CIndividualProcessNodeVector* mIndiProcessVector;

						CIndividualUnsortedProcessingQueue* mIndiImmProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseIndiImmProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevIndiImmProcessQueue;

						CIndividualUnsortedProcessingQueue* mDelayNomProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseDelayNomProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevDelayNomProcessQueue;


						CIndividualUnsortedProcessingQueue* mRoleAssertionProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseRoleAssertionProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevRoleAssertionProcessQueue;


						CIndividualDepthProcessingQueue* mIndiDepthDetExpPreProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIndiDepthDetExpPreProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIndiDepthDetExpPreProcessingQueue;

						CIndividualConceptBatchProcessingQueue* mVarBindConceptBatchProcessQueue;
						CIndividualConceptBatchProcessingQueue* mUseVarBindConceptBatchProcessQueue;
						CIndividualConceptBatchProcessingQueue* mPrevVarBindConceptBatchProcessQueue;

						CIndividualDepthProcessingQueue* mIndiDepthProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIndiDepthProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIndiDepthProcessingQueue;


						CIndividualDepthProcessingQueue* mNominalProcessingQueue;
						CIndividualDepthProcessingQueue* mUseNominalProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevNominalProcessingQueue;


						CIndividualUnsortedProcessingQueue* mIndiDepthFirstProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseIndiDepthFirstProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevIndiDepthFirstProcessQueue;



						CIndividualUnsortedProcessingQueue* mCachingLossReactivationProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseCachingLossReactivationProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevCachingLossReactivationProcessQueue;


						CIndividualDepthProcessingQueue* mIndiSignatureBlockingUpdateProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIndiSignatureBlockingUpdateProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIndiSignatureBlockingUpdateProcessingQueue;

						CIndividualDepthProcessingQueue* mIndiBlockedReactivationProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIndiBlockedReactivationProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIndiBlockedReactivationProcessingQueue;


						CIndividualDepthProcessingQueue* mValueSpaceTriggeringProcessingQueue;
						CIndividualDepthProcessingQueue* mUseValueSpaceTriggeringProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevValueSpaceTriggeringProcessingQueue;

						CIndividualDepthProcessingQueue* mDistinctValueSpaceSatisfiabilityCheckingQueue;
						CIndividualDepthProcessingQueue* mUseDistinctValueSpaceSatisfiabilityCheckingQueue;
						CIndividualDepthProcessingQueue* mPrevDistinctValueSpaceSatisfiabilityCheckingQueue;


						CSignatureBlockingCandidateHash* mSignatureBlockingCandidateHash;
						CSignatureBlockingCandidateHash* mUseSignatureBlockingCandidateHash;
						CSignatureBlockingCandidateHash* mPrevSignatureBlockingCandidateHash;

						CSignatureBlockingReviewSet* mSignatureBlockingReviewSet;
						CSignatureBlockingReviewSet* mUseSignatureBlockingReviewSet;
						CSignatureBlockingReviewSet* mPrevSignatureBlockingReviewSet;


						CSignatureBlockingCandidateHash* mSignatureNominalDelayingCandidateHash;
						CSignatureBlockingCandidateHash* mUseSignatureNominalDelayingCandidateHash;
						CSignatureBlockingCandidateHash* mPrevSignatureNominalDelayingCandidateHash;


						CIndividualReactivationProcessingQueue* mIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mUseIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mPrevIndiReactProQueue;

						CReusingReviewData* mReusingReviewSet;
						CReusingReviewData* mUseReusingReviewSet;
						CReusingReviewData* mPrevReusingReviewSet;

						CBlockingIndividualNodeCandidateHash* mBlockingIndiNodeCandidateHash;
						CBlockingIndividualNodeCandidateHash* mUseBlockingIndiNodeCandidateHash;
						CBlockingIndividualNodeCandidateHash* mPrevBlockingIndiNodeCandidateHash;

						CBlockingIndividualNodeLinkedCandidateHash* mBlockingIndiNodeLinkedCandidateHash;
						CBlockingIndividualNodeLinkedCandidateHash* mUseBlockingIndiNodeLinkedCandidateHash;
						CBlockingIndividualNodeLinkedCandidateHash* mPrevBlockingIndiNodeLinkedCandidateHash;

						CNodeSwitchHistory* mNodeSwitchHistory;
						CNodeSwitchHistory* mUseNodeSwitchHistory;
						CNodeSwitchHistory* mPrevNodeSwitchHistory;

						CBranchingTree* mBranchingTree;
						CBranchingTree* mUseBranchingTree;
						CBranchingTree* mPrevBranchingTree;

						CConceptVector* mLocExtendedConceptVector;
						CConceptVector* mUseExtendedConceptVector;

						CConceptNominalSchemaGroundingHash* mLocGroundingHash;
						CConceptNominalSchemaGroundingHash* mUseGroundingHash;

						bool mMultipleConstructionIndiNodes;
						CIndividualProcessNode* mConstructedIndiNode;
						bool mConstructedIndiNodeInitialized;
						cint64 mMaximumDeterministicBranchTag;

						CIndividualProcessNode* mLastProcessingIndiNode;
						CConceptProcessDescriptor* mLastProcessingConDes;
						bool mLastConDesIndiReapplication;


						CXLinker<CIndividualProcessNode*>* mSortedNominalNonDetProcessingNodeLinker;
						bool mSortedNominalNonDetProcessingNodesSorted;
						cint64 mNominalNonDetProcessingCount;


						CXLinker<CIndividualProcessNode*>* mIndividualNodeCacheTestingLinker;

						CIndividualProcessNodeLinker* mIndiProcessNodeLinker;
						CIndividualPilingProcessNodeLinker* mIndiPilingProcessNodeLinker;
						CIndividualPilingProcessNodeVector* mIndiPilingProcessVector;
						CConceptPilingProcessLinker* mRemConPilProcessLinker;
						CConceptPilingDescriptor* mRemConPilDes;

						CIndividualSaturationProcessNodeLinker* mDisjunctCommonConceptExtractProcessingLinker;
						CIndividualSaturationProcessNodeLinker* mIndiSaturationProcessNodeLinker;
						CIndividualSaturationProcessNodeLinker* mIndiSaturationCompletionNodeLinker;
						CIndividualSaturationProcessNodeVector* mIndiSaturationProcessVector;
						CIndividualSaturationProcessNodeStatusUpdateLinker* mRemSatUpdateLinker;
						CConceptSaturationProcessLinker* mRemConSatProcessLinker;
						CRoleSaturationProcessLinker* mRemRoleSatProcessLinker;
						CConceptSaturationDescriptor* mRemConSatDes;
						CCriticalIndividualNodeProcessingQueue* mSatCriticalIndiNodeProcQueue;
						CCriticalIndividualNodeConceptTestSet* mSatCriticalIndiNodeConTestSet;
						CSaturationNominalDependentNodeHash* mSatNominalDependentNodeHash;
						CSaturationInfluencedNominalSet* mSatInfluencedNominalSet;
						bool mInsufficientNodeOccured;
						bool mDelayedNominalProcessingOccured;
						bool mProblematicEQCandidateNodeOccured;
						CSaturationSuccessorExtensionIndividualNodeProcessingQueue* mSatSuccExtIndNodeProcQueue;
						CIndividualSaturationProcessNodeLinker* mNominalDelayedIndiSaturationProcessNodeLinker;

						CXLinker<CIndividualProcessNode*>* mIndividualNodeResolveLinker;

						cint64 mNextSatResSuccExtIndividualNodeID;
						cint64 mNextIndividualNodeID;
						cint64 mNextPropagationID;
						cint64 mNextVariableID;
						cint64 mNextRepVariableID;

						CVariableBindingPathMergingHash* mUseVarBindingPathMergingHash;
						CVariableBindingPathMergingHash* mLocVarBindingPathMergingHash;

						CRepresentativeVariableBindingPathSetHash* mUseRepVarBindPathSetHash;
						CRepresentativeVariableBindingPathSetHash* mLocRepVarBindPathSetHash;

						CRepresentativeVariableBindingPathJoiningKeyHash* mUseRepVarBindPathJoiningKeyHash;
						CRepresentativeVariableBindingPathJoiningKeyHash* mLocRepVarBindPathJoiningKeyHash;

						CRepresentativeJoiningHash* mUseRepJoiningHash;
						CRepresentativeJoiningHash* mLocRepJoiningHash;

						CRepresentativeVariableBindingPathHash* mUseRepVarBindPathHash;
						CRepresentativeVariableBindingPathHash* mLocRepVarBindPathHash;


						CNominalCachingLossReactivationHash* mUseNomCachingLossReactHash;
						CNominalCachingLossReactivationHash* mLocNomCachingLossReactHash;

					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGDATABOX_H
