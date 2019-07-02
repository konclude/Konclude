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
#include "CMarkerIndividualNodeHash.h"
#include "CIndividualCustomPriorityProcessingQueue.h"
#include "CReferredIndividualTrackingVector.h"
#include "CIndividualSaturationSuccessorLinkDataLinker.h"

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
						CProcessingDataBox* setIndividualProcessNodeVector(CIndividualProcessNodeVector* indiNodeVec);



						CProcessingDataBox* clearIndividualProcessingQueues();




						CIndividualUnsortedProcessingQueue* getRoleAssertionProcessingQueue(bool create = true);
						CProcessingDataBox* clearRoleAssertionProcessingQueue();

						CIndividualUnsortedProcessingQueue* getBackendCacheSynchronizationProcessingQueue(bool create = true);
						CProcessingDataBox* clearBackendCacheSynchronizationProcessingQueue();


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

						CIndividualUnsortedProcessingQueue* getIndividualDepthFirstDeterministicExpansionProcessingQueue(bool create = true);
						CProcessingDataBox* clearIndividualDepthFirstDeterministicExpansionProcessingQueue();

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

						CIndividualDepthProcessingQueue* getIncrementalExpansionInitializingProcessingQueue(bool create = true);
						CProcessingDataBox* clearIncrementalExpansionInitializingProcessingQueue();

						CIndividualCustomPriorityProcessingQueue* getIncrementalExpansionProcessingQueue(bool create = true);
						CProcessingDataBox* clearIncrementalExpansionIProcessingQueue();

						CIndividualDepthProcessingQueue* getIncrementalCompatibilityCheckingQueue(bool create = true);
						CProcessingDataBox* clearIncrementalCompatibilityCheckingQueue();


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
						CIndividualReactivationProcessingQueue* getEarlyIndividualReactivationProcessingQueue(bool create = true);
						CProcessingDataBox* clearEarlyIndividualReactivationProcessingQueue();
						CIndividualReactivationProcessingQueue* getLateIndividualReactivationProcessingQueue(bool create = true);
						CProcessingDataBox* clearLateIndividualReactivationProcessingQueue();


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




						CXLinker<CIndividualProcessNode*>* getBlockableIndividualNodeUpdatedLinker();
						CProcessingDataBox* addBlockableIndividualNodeUpdatedLinker(CXLinker<CIndividualProcessNode*>* linker);
						bool hasBlockableIndividualNodeUpdatedLinker();
						CProcessingDataBox* clearBlockableIndividualNodeUpdatedLinker();



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





						CIndividualSaturationProcessNodeLinker* getRemainingIndividualSaturationNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeRemainingIndividualSaturationNodeLinker();
						CProcessingDataBox* setRemainingIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addRemainingIndividualSaturationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);





						CIndividualSaturationSuccessorLinkDataLinker* getRemainingIndividualSuccessorLinkDataLinker();
						CIndividualSaturationSuccessorLinkDataLinker* takeRemainingIndividualSuccessorLinkDataLinker();
						CProcessingDataBox* setRemainingIndividualSuccessorLinkDataLinker(CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker);
						CProcessingDataBox* addRemainingIndividualSuccessorLinkDataLinker(CIndividualSaturationSuccessorLinkDataLinker* succLinkDataLinker);








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


						bool hasIndividualSaturationAnalysationNodeLinker();
						CIndividualSaturationProcessNodeLinker* getIndividualSaturationAnalysationNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeIndividualSaturationAnalysationNodeLinker();
						CProcessingDataBox* setIndividualSaturationAnalysationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addIndividualSaturationAnalysationNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);




						bool hasNominalDelayedIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* getNominalDelayedIndividualSaturationProcessNodeLinker();
						CIndividualSaturationProcessNodeLinker* takeNominalDelayedIndividualSaturationProcessNodeLinker();
						CProcessingDataBox* setNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);
						CProcessingDataBox* addNominalDelayedIndividualSaturationProcessNodeLinker(CIndividualSaturationProcessNodeLinker* indiProcessNodeLinker);



						CIndividualSaturationProcessNode* getSeparatedSaturationConceptAssertionResolveNode();
						CProcessingDataBox* setSeparatedSaturationConceptAssertionResolveNode(CIndividualSaturationProcessNode* node);







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
						CProcessingDataBox* setFirstPossibleIndividualNodeID(cint64 indiID);

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

						CMarkerIndividualNodeHash* getMarkerIndividualNodeHash(bool createOrForceLocalisation = true);


						bool isIncrementalExpansionInitialised();
						CProcessingDataBox* setIncrementalExpansionInitialised(bool initialized);


						cint64 getIncrementalExpansionID();
						CProcessingDataBox* setIncrementalExpansionID(cint64 incExpID);

						cint64 getMaxIncrementalPreviousCompletionGraphNodeID();
						CProcessingDataBox* setMaxIncrementalPreviousCompletionGraphNodeID(cint64 nodeID);


						cint64 getNextIncrementalIndividualExpansionID(bool incrementNextID = true);

						bool isIncrementalExpansionCompatibleMerged();
						CProcessingDataBox* setIncrementalExpansionCompatibleMerged(bool compatibleMerged);

						bool isIncrementalExpansionCachingMerged();
						CProcessingDataBox* setIncrementalExpansionCachingMerged(bool cachingMerged);

						cint64 getNextRoleAssertionCreationID(bool incrementNextID = true);


						CReferredIndividualTrackingVector* getReferredIndividualTrackingVector();
						CProcessingDataBox* setReferredIndividualTrackingVector(CReferredIndividualTrackingVector* refIndiTrackVec);
						bool isIndividualDependenceTrackingRequired();
						CProcessingDataBox* setIndividualDependenceTrackingRequired(bool indiDepTrackingRequired);


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

						CIndividualUnsortedProcessingQueue* mBackendSyncRetestProcessQueue;
						CIndividualUnsortedProcessingQueue* mUseBackendSyncRetestProcessQueue;
						CIndividualUnsortedProcessingQueue* mPrevBackendSyncRetestProcessQueue;

						CIndividualDepthProcessingQueue* mIndiDepthDetExpPreProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIndiDepthDetExpPreProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIndiDepthDetExpPreProcessingQueue;

						CIndividualUnsortedProcessingQueue* mIndiDepthFirstDetExpPreProcessingQueue;
						CIndividualUnsortedProcessingQueue* mUseIndiDepthFirstDetExpPreProcessingQueue;
						CIndividualUnsortedProcessingQueue* mPrevIndiDepthFirstDetExpPreProcessingQueue;

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



						CIndividualDepthProcessingQueue* mIncrementalExansionInitializingProcessingQueue;
						CIndividualDepthProcessingQueue* mUseIncrementalExansionInitializingProcessingQueue;
						CIndividualDepthProcessingQueue* mPrevIncrementalExansionInitializingProcessingQueue;


						CIndividualCustomPriorityProcessingQueue* mIncrementalExansionProcessingQueue;
						CIndividualCustomPriorityProcessingQueue* mUseIncrementalExansionProcessingQueue;
						CIndividualCustomPriorityProcessingQueue* mPrevIncrementalExansionProcessingQueue;



						CIndividualDepthProcessingQueue* mIncrementalCompatibilityCheckingQueue;
						CIndividualDepthProcessingQueue* mUseIncrementalCompatibilityCheckingQueue;
						CIndividualDepthProcessingQueue* mPrevIncrementalCompatibilityCheckingQueue;



						CSignatureBlockingCandidateHash* mSignatureBlockingCandidateHash;
						CSignatureBlockingCandidateHash* mUseSignatureBlockingCandidateHash;
						CSignatureBlockingCandidateHash* mPrevSignatureBlockingCandidateHash;

						CSignatureBlockingReviewSet* mSignatureBlockingReviewSet;
						CSignatureBlockingReviewSet* mUseSignatureBlockingReviewSet;
						CSignatureBlockingReviewSet* mPrevSignatureBlockingReviewSet;


						CSignatureBlockingCandidateHash* mSignatureNominalDelayingCandidateHash;
						CSignatureBlockingCandidateHash* mUseSignatureNominalDelayingCandidateHash;
						CSignatureBlockingCandidateHash* mPrevSignatureNominalDelayingCandidateHash;


						CIndividualReactivationProcessingQueue* mEarlyIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mUseEarlyIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mPrevEarlyIndiReactProQueue;

						CIndividualReactivationProcessingQueue* mLateIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mUseLateIndiReactProQueue;
						CIndividualReactivationProcessingQueue* mPrevLateIndiReactProQueue;

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

						CIndividualSaturationProcessNodeLinker* mDisjunctCommonConceptExtractProcessingLinker;
						CIndividualSaturationProcessNodeLinker* mIndiSaturationProcessNodeLinker;
						CIndividualSaturationProcessNodeLinker* mIndiSaturationCompletionNodeLinker;
						CIndividualSaturationProcessNodeLinker* mIndiSaturationAnalysingNodeLinker;
						CIndividualSaturationProcessNodeVector* mIndiSaturationProcessVector;
						CIndividualSaturationProcessNodeStatusUpdateLinker* mRemSatUpdateLinker;
						CIndividualSaturationProcessNodeLinker* mRemSatIndiNodeLinker;
						CIndividualSaturationSuccessorLinkDataLinker* mRemSatIndiSuccLinkDataLinker;
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

						CIndividualSaturationProcessNode* mSeparatedSaturationConAssResolveNode;

						CXLinker<CIndividualProcessNode*>* mIndividualNodeResolveLinker;
						CXLinker<CIndividualProcessNode*>* mBlockableIndividualNodeUpdatedLinker;

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


						CMarkerIndividualNodeHash* mUseMarkerIndiNodeHash;
						CMarkerIndividualNodeHash* mLocMarkerIndiNodeHash;


						bool mIncrementalExpansionInitialized;
						cint64 mNextIncrementalIndiExpID;
						cint64 mIncrementalExpID;
						bool mIncrementalExpansionCompatibleMerged;
						bool mIncrementalExpansionCachingMerged;
						cint64 mMaxIncPrevCompGraphNodeID;


						cint64 mNextRoleAssertionCreationID;


						CReferredIndividualTrackingVector* mReferredIndiTrackVec;
						bool mIndiDepTrackingRequired;

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
