/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSNODE_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CProcessContext.h"
#include "CIndividualProcessNodeReference.h"
#include "CReapplyQueue.h"
#include "CConceptLabelSet.h"
#include "CRoleReapplyHash.h"
#include "CConceptReapplyHash.h"
#include "CRoleSuccessorHash.h"
#include "CReapplyRoleSuccessorHash.h"
#include "CReapplyConceptLabelSet.h"
#include "CSuccessorRoleHash.h"
#include "CConnectionSuccessorCorrectionHash.h"
#include "CConnectionSuccessorSet.h"
#include "CConceptProcessingQueue.h"
#include "CLocalizationTag.h"
#include "CBlockedTestTag.h"
#include "CDistinctHash.h"
#include "CDisjointSuccessorRoleHash.h"
#include "CDependencyTracker.h"
#include "CBlockingFollowSet.h"
#include "CRoleBackwardPropagationHash.h"
#include "CIndividualProcessNode.h"
#include "CIndividualProcessNodeLinker.h"
#include "CConceptProcessLinker.h"
#include "CIndividualProcessNodePriority.h"

#include "CIndividualNodeModelData.h"
#include "CIndividualNodeBlockData.h"
#include "CBlockingVariableBindingsAnalogousPropagationData.h"
#include "CIndividualNodeSatisfiableCacheRetrievalData.h"
#include "CIndividualNodeBackendCacheSynchronisationData.h"
#include "CIndividualNodeSatisfiableCacheStoringData.h"
#include "CIndividualNodeUnsatisfiableCacheRetrievalData.h"
#include "CIndividualNodeAnalizedConceptExpansionData.h"
#include "CSignatureBlockingIndividualNodeConceptExpansionData.h"
#include "CReusingIndividualNodeConceptExpansionData.h"
#include "CIndividualNodeSaturationBlockingData.h"
#include "CNominalCachingLossReactivationData.h"
#include "CSuccessorConnectedNominalSet.h"
#include "CSuccessorIndividualATMOSTReactivationData.h"

#include "CPropagationBindingSet.h"
#include "CConceptPropagationBindingSetHash.h"
#include "CConceptVariableBindingPathSetHash.h"
#include "CConceptRepresentativePropagationSetHash.h"
#include "CIndividualNodeIncrementalExpansionData.h"

#include "CDatatypesValueSpaceData.h"
#include "CAdditionalProcessRoleAssertionsLinker.h"
#include "CAdditionalProcessDataAssertionsLinker.h"
#include "CProcessAssertedDataLiteralLinker.h"
#include "CIndividualMergingHash.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CIRIName.h"


#include "Utilities/Memory/CObjectParameterizingAllocator.h"

#include "Utilities/Container/CSortedNegLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {


				/*! 
				 *
				 *		\class		CIndividualProcessNode
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualProcessNode : public CIndividualProcessNodeReference, public CLocalizationTag, public CBlockedTestTag, public CDependencyTracker {
					// public methods
					public:
						//! Constructor
						CIndividualProcessNode(CProcessContext* processContext = nullptr);


						CIndividualProcessNode* initIndividualProcessNode(CIndividualProcessNode* prevIndividual);

						CIndividualProcessNode* initIndividualProcessNodeCopy(CIndividualProcessNode* prevIndividual, bool adobtConceptLabels = true, bool adobtRoleSuccessors = false, bool adobtStatus = false);

						
						CIndividualProcessNode* setReapplyConceptLabelSet(CReapplyConceptLabelSet* reapplyConSet);

						CReapplyConceptLabelSet* getReapplyConceptLabelSet(bool create = true);

						CConceptProcessingQueue* getConceptProcessingQueue(bool create = true);

						CReapplyRoleSuccessorHash* getReapplyRoleSuccessorHash(bool create = true);
						CRoleSuccessorLinkIterator getRoleSuccessorLinkIterator(CRole* role);
						CRoleSuccessorLinkIterator getRoleSuccessorHistoryLinkIterator(CRole* role, CIndividualLinkEdge* lastLink);
						cint64 getRoleSuccessorCount(CRole* role);


						CRoleSuccessorIterator getRoleIterator();

						CSuccessorRoleHash* getSuccessorRoleHash(bool create = true);
						CSuccessorRoleIterator getSuccessorRoleIterator(cint64 indiID);
						CSuccessorRoleIterator getSuccessorRoleIterator(CIndividualProcessNode* indiNode);

						bool hasSuccessorIndividualNode(cint64 indiID);
						bool hasSuccessorIndividualNode(CIndividualProcessNode* indiNode);



						bool hasDisjointRoleConnections();
						CIndividualProcessNode* setDisjointRoleConnections(bool disjointRoleConnections);

						CDisjointSuccessorRoleHash* getDisjointSuccessorRoleHash(bool create = true);
						CDisjointSuccessorRoleIterator getDisjointSuccessorRoleIterator(cint64 succIndiId);
						CDisjointSuccessorRoleIterator getDisjointSuccessorRoleIterator(CIndividualProcessNode* indiNode);


						bool hasNegationDisjointToIndividual(CRole* role, cint64 destinationIndiID);
						bool hasNegationDisjointToIndividual(CRole* role, CIndividualProcessNode* desIndi);

						CIndividualProcessNode* installDisjointLink(CNegationDisjointEdge* link);
						CIndividualProcessNode* removeDisjointLinks(cint64 succIndiID);

						bool hasRoleSuccessorToIndividual(CRole* role, cint64 destinationIndiID, bool locateable);
						bool hasRoleSuccessorToIndividual(CRole* role, CIndividualProcessNode* desIndi, bool locateable);

						CIndividualLinkEdge* getRoleSuccessorToIndividualLink(CRole* role, cint64 destinationIndiID, bool locateable);
						CIndividualLinkEdge* getRoleSuccessorToIndividualLink(CRole* role, CIndividualProcessNode* desIndi, bool locateable);

						CSuccessorIterator getSuccessorIterator();

						CConnectionSuccessorSet* getConnectionSuccessorSet(bool create = true);
						CConnectionSuccessorSetIterator getConnectionSuccessorIterator();

						CReapplyQueue* getRoleReapplyQueue(CRole* role, bool create = true);
						CReapplyQueueIterator getRoleReapplyIterator(CRole* role, bool clearDynamicReapllyQueue = true);


						CCondensedReapplyQueue* getConceptReapplyQueue(CConcept* concept, bool conceptNegation, bool create = true);
						CCondensedReapplyQueueIterator getConceptReapplyIterator(CConcept* concept, bool conceptNegation, bool clearDynamicReapllyQueue = true);

						CDistinctHash* getDistinctHash(bool create = true);


						CIndividualLinkEdge* getLastAddedRoleLink();

						cint64 installIndividualLink(CIndividualLinkEdge* link, CReapplyQueueIterator* reapplyQueueIt = nullptr);
						CIndividualProcessNode* removeIndividualLink(CIndividualLinkEdge* link);
						CIndividualProcessNode* removeIndividualConnection(CIndividualProcessNode* indi);

						CIndividualLinkEdge* getAncestorLink();
						CIndividualProcessNode* setAncestorLink(CIndividualLinkEdge* link);
						bool isIndividualAncestor(CIndividualProcessNode* individual);
						bool hasIndividualAncestor();


						CIndividualProcessNode* addBlockedIndividualsLinker(CXLinker<CIndividualProcessNode* >* linker);
						CXLinker<CIndividualProcessNode*>* getBlockedIndividualsLinker();
						CIndividualProcessNode* setBlockedIndividualsLinker(CXLinker<CIndividualProcessNode* >* linker = nullptr);
						CIndividualProcessNode* clearBlockedIndividualsLinker();
						bool hasBlockedIndividualsLinker();


						CIndividualNodeModelData* getIndividualModelData();
						CIndividualProcessNode* setIndividualModelData(CIndividualNodeModelData* individualModel);

						CIndividualNodeBlockData* getIndividualBlockData(bool localBlockData = false);
						CIndividualProcessNode* setIndividualBlockData(CIndividualNodeBlockData* individualBlock);


						CBlockingVariableBindingsAnalogousPropagationData* getVariableBindingsPropagationBlockingData(bool localBlockData = false);
						CIndividualProcessNode* setVariableBindingsPropagationBlockingData(CBlockingVariableBindingsAnalogousPropagationData* blockData);

						CIndividualNodeSaturationBlockingData* getIndividualSaturationBlockingData(bool localBlockData = false);
						CIndividualProcessNode* setIndividualSaturationBlockingData(CIndividualNodeSaturationBlockingData* individualBlock);

						CIndividualNodeUnsatisfiableCacheRetrievalData* getIndividualUnsatisfiableCacheRetrievalData(bool localUnsatCacheRetData = false);
						CIndividualProcessNode* setIndividualUnsatisfiableCacheRetrievalData(CIndividualNodeUnsatisfiableCacheRetrievalData* individualUnsatCacheRetData);

						CIndividualNodeSatisfiableCacheRetrievalData* getIndividualSatisfiableCacheRetrievalData(bool localCacheData = false);
						CIndividualProcessNode* setIndividualSatisfiableCacheRetrievalData(CIndividualNodeSatisfiableCacheRetrievalData* satCacheRetrievalData);

						CIndividualNodeBackendCacheSynchronisationData* getIndividualBackendCacheSynchronisationData(bool localCacheData = false);
						CIndividualProcessNode* setIndividualBackendCacheSynchronisationData(CIndividualNodeBackendCacheSynchronisationData* backendSyncData);
						
						CIndividualNodeSatisfiableCacheStoringData* getIndividualSatisfiableCacheStoringData(bool localCacheData = false);
						CIndividualProcessNode* setIndividualSatisfiableCacheStoringData(CIndividualNodeSatisfiableCacheStoringData* satCacheStoringData);


						CIndividualNodeAnalizedConceptExpansionData* getAnalizedConceptExpansionData(bool createOrLocalize = true);


						CBlockingFollowSet* getBlockingFollowSet(bool createOrLocalize = true);
						bool hasBlockingFollower();


						CSignatureBlockingIndividualNodeConceptExpansionData* getSignatureBlockingIndividualNodeConceptExpansionData(bool localBlockingData);
						CIndividualProcessNode* setSignatureBlockingIndividualNodeConceptExpansionData(CSignatureBlockingIndividualNodeConceptExpansionData* individualBlockingData);

						CReusingIndividualNodeConceptExpansionData* getReusingIndividualNodeConceptExpansionData(bool localBlockingData);
						CIndividualProcessNode* setReusingIndividualNodeConceptExpansionData(CReusingIndividualNodeConceptExpansionData* individualBlockingData);


						cint64 getIndividualAncestorDepth();
						CIndividualProcessNode* setIndividualAncestorDepth(cint64 depth);

						cint64 getIndividualNominalLevel();
						CIndividualProcessNode* setIndividualNominalLevel(cint64 level);


						cint64 getIndividualNominalLevelOrAncestorDepth();



						cint64 getMergedIntoIndividualNodeID();
						bool hasMergedIntoIndividualNodeID();
						CIndividualProcessNode* setMergedIntoIndividualNodeID(cint64 indiNodeID);





						bool isBlockableIndividual();
						bool isNominalIndividualNode();
						CIndividual* getNominalIndividual();
						CIndividualProcessNode* setNominalIndividual(CIndividual* indi);


						enum CIndividualType { BLOCKABLEINDIVIDUALTYPE, NOMINALINDIVIDUALTYPE };
						CIndividualType getIndividualType();

						CIndividualProcessNode* setIndividualType(CIndividualType indiType);


						cint64 getIndividualNodeID();
						CIndividualProcessNode* setIndividualNodeID(cint64 indiID);

						bool hasInitializingConcepts();
						CIndividualProcessNode* clearProcessInitializingConcepts();
						CXSortedNegLinker<CConcept*>* getInitializingConceptLinker();
						CXSortedNegLinker<CConcept*>* getProcessInitializingConceptLinker();
						CIndividualProcessNode* setInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinkerIt);
						CIndividualProcessNode* addInitializingConceptLinker(CXSortedNegLinker<CConcept*>* initializingConceptLinkerIt);

						bool hasAssertionConcepts();
						CIndividualProcessNode* clearAssertionConcepts();
						CConceptAssertionLinker* getAssertionConceptLinker();
						CIndividualProcessNode* setAssertionConceptLinker(CConceptAssertionLinker* assertionConceptLinkerIt);

						bool hasAssertionData();
						CIndividualProcessNode* clearAssertionData();
						CDataAssertionLinker* getAssertionDataLinker();
						CIndividualProcessNode* setAssertionDataLinker(CDataAssertionLinker* assertionDataLinker);

						CDataAssertionLinker* getLastProcessedAssertionDataLinker();
						CIndividualProcessNode* setLastProcessedAssertionDataLinker(CDataAssertionLinker* assDataLinker);


						bool hasAssertedDataLiterals();
						CProcessAssertedDataLiteralLinker* getAssertedDataLiteralLinker();
						CIndividualProcessNode* setAssertedDataLiteralLinker(CProcessAssertedDataLiteralLinker* dataLiteralLinker);
						CIndividualProcessNode* addAssertedDataLiteralLinker(CProcessAssertedDataLiteralLinker* dataLiteralLinker);
						CIndividualProcessNode* clearAssertedDataLiterals();

						CProcessAssertedDataLiteralLinker* getLastAssertedDataLiteralLinker();
						CIndividualProcessNode* setLastAssertedDataLiteralLinker(CProcessAssertedDataLiteralLinker* dataLiteralLinker);


						bool hasAssertionRoles();
						CIndividualProcessNode* clearAssertionRoles();
						CRoleAssertionLinker* getAssertionRoleLinker();
						CIndividualProcessNode* setAssertionRoleLinker(CRoleAssertionLinker* assertionRoleLinkerIt);


						cint64 getRoleAssertionCreationID();
						CIndividualProcessNode* setRoleAssertionCreationID(cint64 creationID);

						bool hasReverseAssertionRoles();
						CIndividualProcessNode* clearReverseAssertionRoles();
						CReverseRoleAssertionLinker* getReverseAssertionRoleLinker();
						CIndividualProcessNode* setReverseAssertionRoleLinker(CReverseRoleAssertionLinker* reverseAssertionRoleLinkerIt);





						bool hasAdditionalRoleAssertionsLinker();
						CIndividualProcessNode* clearAdditionalRoleAssertionsLinker();
						CAdditionalProcessRoleAssertionsLinker* getAdditionalRoleAssertionsLinker();
						CIndividualProcessNode* setAdditionalRoleAssertionsLinker(CAdditionalProcessRoleAssertionsLinker* reverseRoleAssertionsLinker);
						CIndividualProcessNode* addAdditionalRoleAssertionsLinker(CAdditionalProcessRoleAssertionsLinker* reverseRoleAssertionsLinker);



						bool hasAdditionalDataAssertionsLinker();
						CIndividualProcessNode* clearAdditionalDataAssertionsLinker();
						CAdditionalProcessDataAssertionsLinker* getAdditionalDataAssertionsLinker();
						CIndividualProcessNode* setAdditionalDataAssertionsLinker(CAdditionalProcessDataAssertionsLinker* addDataAssertionsLinker);
						CIndividualProcessNode* addAdditionalDataAssertionsLinker(CAdditionalProcessDataAssertionsLinker* addDataAssertionsLinker);


						CAdditionalProcessDataAssertionsLinker* getLastProcessedAdditionalDataAssertionLinker();
						CIndividualProcessNode* setLastProcessedAdditionalDataAssertionLinker(CAdditionalProcessDataAssertionsLinker* dataLiteralLinker);



						bool hasBaseConceptsInitialized();
						CIndividualProcessNode* setBaseConceptsInitialized(bool initialized);


						bool hasRoleAssertionsInitialized();
						CIndividualProcessNode* setRoleAssertionsInitialized(bool initialized);


						bool hasReverseRoleAssertionsInitialized();
						CIndividualProcessNode* setReverseRoleAssertionsInitialized(bool initialized);


						bool hasNominalIndividualTriplesAssertions();
						CIndividualProcessNode* setNominalIndividualTriplesAssertions(bool hasNominalAssertions);

						bool areNominalIndividualTriplesAssertionsLoaded();
						CIndividualProcessNode* setNominalIndividualTriplesAssertionsLoaded(bool loaded);

						bool isNominalIndividualRepresentativeBackendDataLoaded();
						CIndividualProcessNode* setNominalIndividualRepresentativeBackendDataLoaded(bool loaded);



						CIndividualProcessNode* getProcessingBlockTestIndividual();
						CIndividualProcessNode* clearProcessingBlockTestIndividual();
						CIndividualProcessNode* setProcessingBlockTestIndividual(CIndividualProcessNode* ancestorBlockTestIndi);

						CIndividualProcessNode* addProcessingBlockedIndividualsLinker(CXLinker<CIndividualProcessNode*>* linker);
						CXLinker<CIndividualProcessNode*>* getProcessingBlockedIndividualsLinker();
						CIndividualProcessNode* setIndirectBlockedIndividualsLinker(CXLinker<CIndividualProcessNode*>* linker = nullptr);



						CIndividualProcessNode* clearSatisfiableCachedAbsorbedDisjunctionsLinker();
						CReapplyConceptDescriptor* getSatisfiableCachedAbsorbedDisjunctionsLinker();
						CIndividualProcessNode* addSatisfiableCachedAbsorbedDisjunctionsLinker(CReapplyConceptDescriptor* disjunctionReapplyConDes);

						CIndividualProcessNode* clearSatisfiableCachedAbsorbedGeneratingLinker();
						CReapplyConceptDescriptor* getSatisfiableCachedAbsorbedGeneratingLinker();
						CIndividualProcessNode* addSatisfiableCachedAbsorbedGeneratingLinker(CReapplyConceptDescriptor* successorGeneratingReapplyConDes);

						cint64 getProcessingRestrictionFlags();

						bool addProcessingRestrictionFlags(cint64 flags);
						bool clearProcessingRestrictionFlags(cint64 flags);
						bool hasProcessingRestrictionFlags(cint64 testFlags);
						bool hasPartialProcessingRestrictionFlags(cint64 testFlags);

						CConceptDescriptor* getIndividualInitializationConcept();
						CIndividualProcessNode* setIndividualInitializationConcept(CConceptDescriptor* initConDes);

						cint64 getLastConceptCountCachedBlockingCandidate();
						CIndividualProcessNode* setLastConceptCountCachedBlockingCandidate(cint64 conCount);

						cint64 getLastConceptCountSearchBlockingCandidate();
						CIndividualProcessNode* setLastConceptCountSearchBlockingCandidate(cint64 conCount);

						cint64 getBlockingCachingSavedCandidateCount();
						CIndividualProcessNode* setBlockingCachingSavedCandidateCount(cint64 conCount);
						CIndividualProcessNode* incBlockingCachingSavedCandidateCount(cint64 incCount = 1);


						bool isInvalidSignatureBlocking();
						CIndividualProcessNode* setInvalidSignatureBlocking(bool invalid);

						cint64 getLastSearchBlockerCandidateCount();
						CIndividualProcessNode* setLastSearchBlockerCandidateCount(cint64 canCount);

						cint64 getLastSearchBlockerCandidateSignature();
						CIndividualProcessNode* setLastSearchBlockerCandidateSignature(cint64 signature);

						CIndividualProcessNode* addSuccessorIndividualNodeBackwardDependencyLinker(CXLinker<CIndividualProcessNode* >* linker);
						CXLinker<CIndividualProcessNode*>* getSuccessorIndividualNodeBackwardDependencyLinker();
						CIndividualProcessNode* setSuccessorIndividualNodeBackwardDependencyLinker(CXLinker<CIndividualProcessNode* >* linker = nullptr);
						CIndividualProcessNode* clearSuccessorIndividualNodeBackwardDependencyLinker();
						bool hasSuccessorIndividualNodeBackwardDependencyLinker();

						bool hasBackwardDependencyToAncestorIndividualNode();
						CIndividualProcessNode* setBackwardDependencyToAncestorIndividualNode(bool backwardDependency);



						bool hasBlockingRetestProcessingRestrictionFlags();
						bool clearBlockingRetestProcessingRestrictionFlags();

						bool hasBlockedProcessingRestrictionFlags();
						bool hasDirectBlockedProcessingRestrictionFlags();
						bool hasProcessingBlockedProcessingRestrictionFlags();
						bool hasPurgedBlockedProcessingRestrictionFlags();
						bool hasIndirectBlockedProcessingRestrictionFlags();
						bool clearBlockedProcessingRestrictionFlags();

						CRoleBackwardPropagationHash* getRoleBackwardPropagationHash(bool create = true);
						CIndividualProcessNodeLinker* getIndividualProcessNodeLinker();
						CIndividualProcessNode* setIndividualProcessNodeLinker(CIndividualProcessNodeLinker* processNodeLinker);

						CConceptProcessLinker* getConceptProcessLinker();
						CConceptProcessLinker* takeConceptProcessLinker();
						CIndividualProcessNode* setConceptProcessLinker(CConceptProcessLinker* conProcessLinker);
						CIndividualProcessNode* addConceptProcessLinker(CConceptProcessLinker* conProcessLinker);
						CIndividualProcessNode* clearConceptProcessLinker();

						bool getRequiredBackwardPropagation();
						CIndividualProcessNode* setRequiredBackwardPropagation(bool requiredBackProp);

						bool hasSubstituteIndividualNode();
						CIndividualProcessNode* getSubstituteIndividualNode();
						CIndividualProcessNode* setSubstituteIndividualNode(CIndividualProcessNode* indiNode);

						bool isProcessingQueued();
						bool isExtendedQueueProcessing();
						bool isImmediatelyProcessingQueued();
						bool isDeterministicExpandingProcessingQueued();
						bool isRegularDepthProcessingQueued();
						bool isBlockedReactivationProcessingQueued();
						bool isBackendSynchronRetestProcessingQueued();
						bool isIncrementalCompatibilityCheckingQueued();
						bool isIncrementalExpansionQueued();
						bool isBackendIndirectCompatibilityExpansionQueued();
						bool isBackendDirectInfluenceExpansionQueued();

						CIndividualProcessNode* setProcessingQueued(bool processingQueued);
						CIndividualProcessNode* setExtendedQueueProcessing(bool extendedQueueProcessing);
						CIndividualProcessNode* setImmediatelyProcessingQueued(bool immProQue);
						CIndividualProcessNode* setDeterministicExpandingProcessingQueued(bool detExpPro);
						CIndividualProcessNode* setRegularDepthProcessingQueued(bool depthPro);
						CIndividualProcessNode* setBlockedReactivationProcessingQueued(bool backendSyncRetest);
						CIndividualProcessNode* setBackendSynchronRetestProcessingQueued(bool backendSyncRetest);
						CIndividualProcessNode* setBackendIndirectCompatibilityExpansionQueued(bool queued);
						CIndividualProcessNode* setBackendDirectInfluenceExpansionQueued(bool queued);
						CIndividualProcessNode* setIncrementalCompatibilityCheckingQueued(bool incCompChecking);
						CIndividualProcessNode* setIncrementalExpansionQueued(bool incExpQueued);

						CIndividualProcessNode* clearProcessingQueued();

						bool isDelayedNominalProcessingQueued();
						bool hasNominalProcessingDelayingChecked();

						CIndividualProcessNode* setDelayedNominalProcessingQueued(bool delayedProcessingQueued);
						CIndividualProcessNode* setNominalProcessingDelayingChecked(bool nominalProcessingDelayingChecked);

						CIndividualProcessNode* setAssertionInitialisationSignatureValue(cint64 sigValue);
						cint64 getAssertionInitialisationSignatureValue();


						CIndividualProcessNode* setLastProcessingPriority(const CIndividualProcessNodePriority& priority);
						CIndividualProcessNodePriority getLastProcessingPriority();
						CIndividualProcessNode* resetLastProcessingPriority();

						bool isCachingLossNodeReactivationInstalled();
						CIndividualProcessNode* setCachingLossNodeReactivationInstalled(bool reactivationInstalled);
						CNominalCachingLossReactivationData* getNominalCachingLossReactivationData(bool create = true);

						CSuccessorIndividualATMOSTReactivationData* getSuccessorIndividualATMOSTReactivationData(bool create = true);


						CSuccessorConnectedNominalSet* getSuccessorNominalConnectionSet(bool create = true);
						bool hasSuccessorConnectionToNominal(cint64 nominalID);
						bool addSuccessorConnectionToNominal(cint64 nominalID);


						CDatatypesValueSpaceData* getDatatypesValueSpaceData(bool create = true);

						CIndividualNodeIncrementalExpansionData* getIncrementalExpansionData(bool create = true);

						CIndividualProcessNode* setIncrementalExpansionID(cint64 incExpID);
						cint64 getIncrementalExpansionID();


						CConceptPropagationBindingSetHash* getConceptPropagationBindingSetHash(bool create = true);

						CConceptVariableBindingPathSetHash* getConceptVariableBindingPathSetHash(bool create = true);

						CConceptRepresentativePropagationSetHash* getConceptRepresentativePropagationSetHash(bool create = true);


						CIndividualMergingHash* getIndividualMergingHash(bool create = true);

						CDependencyTrackPoint* getMergedDependencyTrackPoint();
						CIndividualProcessNode* setMergedDependencyTrackPoint(CDependencyTrackPoint* depTrackPoint);

						CIndividualProcessNode* getLastMergedIntoIndividualNode();
						CIndividualProcessNode* setLastMergedIntoIndividualNode(CIndividualProcessNode* indiNode);

						CIndividualProcessNode* setBlockerIndividualNode(CIndividualProcessNode* indiNode);
						CIndividualProcessNode* getBlockerIndividualNode();

						CIndividualProcessNode* setFollowingIndividualNode(CIndividualProcessNode* indiNode);
						CIndividualProcessNode* getFollowingIndividualNode();



						const static cint64 PRFDIRECTBLOCKED										= Q_UINT64_C(0x1);
						const static cint64 PRFPROCESSINGBLOCKED									= Q_UINT64_C(0x2);
						const static cint64 PRFINDIRECTBLOCKED										= Q_UINT64_C(0x4);
						const static cint64 PRFPURGEDBLOCKED										= Q_UINT64_C(0x8);
						const static cint64 PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED					= Q_UINT64_C(0x10);
						const static cint64 PRFINVALIDBLOCKINGORCACHING								= Q_UINT64_C(0x20);

						const static cint64 PRFBLOCKINGRETESTDUEANCESTORMODIFIED					= Q_UINT64_C(0x100);
						const static cint64 PRFBLOCKINGRETESTDUEDIRECTMODIFIED						= Q_UINT64_C(0x200);
						const static cint64 PRFBLOCKINGRETESTDUEBLOCKERMODIFIED						= Q_UINT64_C(0x400);
						const static cint64 PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS					= Q_UINT64_C(0x800);

						const static cint64 PRFPROCESSINGCOMPLETED									= Q_UINT64_C(0x1000);
						const static cint64 PRFANCESTORALLPROCESSED									= Q_UINT64_C(0x2000);

						const static cint64 PRFCONSNODEPREPARATIONINDINODE							= Q_UINT64_C(0x10000);
						const static cint64 PRFCONCRETEDATAINDINODE									= Q_UINT64_C(0x20000);

						const static cint64 PRFSATISFIABLECACHED									= Q_UINT64_C(0x100000);
						const static cint64 PRFANCESTORSATISFIABLECACHED							= Q_UINT64_C(0x200000);
						const static cint64 PRFRETESTSATISFIABLECACHEDDUEDIRECTMODIFIED 			= Q_UINT64_C(0x400000);
						const static cint64 PRFANCESTORSATISFIABLECACHEDABOLISHED					= Q_UINT64_C(0x800000);
						
						const static cint64 PRFSIGNATUREBLOCKINGCACHED								= Q_UINT64_C(0x1000000);
						const static cint64 PRFANCESTORSIGNATUREBLOCKINGCACHED						= Q_UINT64_C(0x2000000);
						const static cint64 PRFRETESTSIGNATUREBLOCKINGCACHEDDUEDIRECTMODIFIED		= Q_UINT64_C(0x4000000);
						const static cint64 PRFANCESTORSIGNATUREBLOCKINGCACHEDABOLISHED				= Q_UINT64_C(0x8000000);

						const static cint64 PRFSATURATIONBLOCKINGCACHED								= Q_UINT64_C(0x10000000);
						const static cint64 PRFANCESTORSATURATIONBLOCKINGCACHED						= Q_UINT64_C(0x20000000);
						const static cint64 PRFRETESTSATURATIONBLOCKINGCACHEDDUEDIRECTMODIFIED		= Q_UINT64_C(0x40000000);
						const static cint64 PRFANCESTORSATURATIONBLOCKINGCACHEDABOLISHED			= Q_UINT64_C(0x80000000);

						const static cint64 PRFSATURATIONSUCCESSORCREATIONBLOCKINGCACHED			= Q_UINT64_C(0x100000000);
						const static cint64 PRFSATURATIONBLOCKINGCACHEDINVALIDATED					= Q_UINT64_C(0x200000000);
						const static cint64 PRFSATURATIONBLOCKINGCACHEDRETESTDUETOMODIFICATION		= Q_UINT64_C(0x400000000);


						const static cint64 PRFCOMPLETIONGRAPHCACHED								= Q_UINT64_C(0x1000000000);
						const static cint64 PRFCOMPLETIONGRAPHCACHINGINVALID						= Q_UINT64_C(0x2000000000);
						const static cint64 PRFCOMPLETIONGRAPHCACHINGINVALIDATED					= Q_UINT64_C(0x4000000000);
						const static cint64 PRFRETESTCOMPLETIONGRAPHCACHEDDUEDIRECTMODIFIED			= Q_UINT64_C(0x8000000000);

						const static cint64 PRFCOMPLETIONGRAPHCACHEDNODELOCATED						= Q_UINT64_C(0x10000000000);
						const static cint64 PRFCOMPLETIONGRAPHCACHEDNODEEXTENDED					= Q_UINT64_C(0x20000000000);


						const static cint64 PRFSUCCESSORNOMINALCONNECTION							= Q_UINT64_C(0x40000000000);
						const static cint64 PRFSUCCESSORNEWNOMINALCONNECTION						= Q_UINT64_C(0x80000000000);

						const static cint64 PRFREUSINGINDIVIDUAL									= Q_UINT64_C(0x100000000000);
						const static cint64 PRFANCESTORREUSINGINDIVIDUALBLOCKED						= Q_UINT64_C(0x200000000000);
						const static cint64 PRFANCESTORREUSINGINDIVIDUALBLOCKEDABOLISHED			= Q_UINT64_C(0x400000000000);

						const static cint64 PRFCACHEDCOMPUTEDTYPESADDED								= Q_UINT64_C(0x1000000000000);


						const static cint64 PRFSYNCHRONIZEDBACKEND									= Q_UINT64_C(0x10000000000000);
						const static cint64 PRFSYNCHRONIZEDBACKENDNEIGHBOUREXPANSIONBLOCKED			= Q_UINT64_C(0x20000000000000);
						const static cint64 PRFSYNCHRONIZEDBACKENDSUCCESSOREXPANSIONBLOCKED			= Q_UINT64_C(0x40000000000000);
						const static cint64 PRFSYNCHRONIZEDBACKENDINDIRECTNOMINALEXPANSIONBLOCKED	= Q_UINT64_C(0x80000000000000);

						const static cint64 PRFRETESTBACKENDSYNCHRONIZATIONDUEDIRECTMODIFIED		= Q_UINT64_C(0x100000000000000);

						const static cint64 PRFSYNCHRONIZEDBACKENNEIGHBOURDPARTIALEXPANSION			= Q_UINT64_C(0x200000000000000);
						const static cint64 PRFSYNCHRONIZEDBACKENNEIGHBOURDFULLEXPANSION			= Q_UINT64_C(0x400000000000000);
						const static cint64 PRFSYNCHRONIZEDBACKENPROCESSINGDELAYING					= Q_UINT64_C(0x800000000000000);

						const static cint64 PRFINCREMENTALEXPANDING									= Q_UINT64_C(0x1000000000000000);
						const static cint64 PRFINCREMENTALEXPANSIONRETESTDUEDIRECTMODIFIED			= Q_UINT64_C(0x2000000000000000);


						const static cint64 PRFBLOCKINGRELATEDFLAGCOMPINATION						= PRFDIRECTBLOCKED | PRFINDIRECTBLOCKED | PRFPROCESSINGBLOCKED |
																										PRFBLOCKINGRETESTDUEANCESTORMODIFIED | PRFBLOCKINGRETESTDUEDIRECTMODIFIED | PRFBLOCKINGRETESTDUEPROCESSINGCOMPLETED |
																										PRFBLOCKINGRETESTDUEBLOCKERMODIFIED | PRFBLOCKINGRETESTDUEINDIRECTBLOCKERLOSS;


						const static cint64 PRFINVALIDATEBLOCKERFLAGSCOMPINATION					= PRFDIRECTBLOCKED | PRFINDIRECTBLOCKED | PRFPROCESSINGBLOCKED | PRFPURGEDBLOCKED | PRFSATISFIABLECACHED | 
																										PRFANCESTORSATISFIABLECACHED | PRFSIGNATUREBLOCKINGCACHED | PRFANCESTORSIGNATUREBLOCKINGCACHED | 
																										PRFREUSINGINDIVIDUAL | PRFANCESTORREUSINGINDIVIDUALBLOCKED | PRFANCESTORSATURATIONBLOCKINGCACHED | PRFCONCRETEDATAINDINODE;

						CConceptDescriptor* mDebugBlockerLastConceptDes;


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mIndiID;
						CIndividualType mIndiType;
						CIndividual* mNomIndi;

						cint64 mProcessingRestrictionFlags;

						cint64 indiAncDepth;
						cint64 mNominalLevel;

						CIndividualLinkEdge* mAncestorLink;

						cint64 mMergeIntoID;

						CProcessContext* mProcessContext;
						CMemoryAllocationManager* mMemAllocMan;
						CIndividualProcessNode* mPrevIndividual;

						CConceptProcessingQueue* mConceptProcessingQueue;
						CConceptProcessingQueue* mUseConceptProcessingQueue;
						CConceptProcessingQueue* mPrevConceptProcessingQueue;


						CReapplyConceptLabelSet* mReapplyConLabelSet;
						CReapplyConceptLabelSet* mUseReapplyConLabelSet;
						CReapplyConceptLabelSet* mPrevReapplyConLabelSet;

						CReapplyRoleSuccessorHash* mReapplyRoleSuccHash;
						CReapplyRoleSuccessorHash* mUseReapplyRoleSuccHash;
						CReapplyRoleSuccessorHash* mPrevReapplyRoleSuccHash;
						CSuccessorRoleHash* mSuccRoleHash;
						CSuccessorRoleHash* mUseSuccRoleHash;
						CSuccessorRoleHash* mPrevSuccRoleHash;

						CIndividualLinkEdge* mLastAddedLink;
						CIndividualProcessNode* mBlockerIndiNode;
						CIndividualProcessNode* mFollowingIndiNode;



						CConnectionSuccessorSet* mConnSuccSet;
						CConnectionSuccessorSet* mUseConnSuccSet;
						CConnectionSuccessorSet* mPrevConnSuccSet;


						CDistinctHash* mDistinctHash;
						CDistinctHash* mUseDistinctHash;
						CDistinctHash* mPrevDistinctHash;

						bool mDisjointRoleConnections;
						CDisjointSuccessorRoleHash* mDisjointSuccRoleHash;
						CDisjointSuccessorRoleHash* mUseDisjointSuccRoleHash;
						CDisjointSuccessorRoleHash* mPrevDisjointSuccRoleHash;

						CIndividualNodeSaturationBlockingData* mIndiSatBlockData;
						CIndividualNodeSaturationBlockingData* mPrevIndiSatBlockData;

						CIndividualNodeBlockData* mIndiBlock;
						CIndividualNodeBlockData* mPrevIndiBlock;

						CBlockingVariableBindingsAnalogousPropagationData* mIndiVarPropBlockData;
						CBlockingVariableBindingsAnalogousPropagationData* mPrevIndiVarPropBlockData;

						CIndividualNodeUnsatisfiableCacheRetrievalData* mIndiUnsatCacheRet;
						CIndividualNodeUnsatisfiableCacheRetrievalData* mPrevIndiUnsatCacheRet;

						CIndividualNodeAnalizedConceptExpansionData* mSigBlockIndExplData;
						CIndividualNodeAnalizedConceptExpansionData* mUseSigBlockIndExplData;
						CIndividualNodeAnalizedConceptExpansionData* mPrevSigBlockIndExplData;

						CSignatureBlockingIndividualNodeConceptExpansionData* mSigBlockConExpData;
						CSignatureBlockingIndividualNodeConceptExpansionData* mPrevSigBlockConExpData;

						CReusingIndividualNodeConceptExpansionData* mReusingConExpData;
						CReusingIndividualNodeConceptExpansionData* mPrevReusingConExpData;

						CBlockingFollowSet* mSigBlockFollowSet;
						CBlockingFollowSet* mUseSigBlockFollowSet;
						CBlockingFollowSet* mPrevSigBlockFollowSet;

						CConceptPropagationBindingSetHash* mConceptPropBindingSetHash;
						CConceptPropagationBindingSetHash* mUseConceptPropBindingSetHash;
						CConceptPropagationBindingSetHash* mPrevConceptPropBindingSetHash;

						CConceptVariableBindingPathSetHash* mConceptVarBindPathSetHash;
						CConceptVariableBindingPathSetHash* mUseConceptVarBindPathSetHash;
						CConceptVariableBindingPathSetHash* mPrevConceptVarBindPathSetHash;

						CConceptRepresentativePropagationSetHash* mConceptRepPropSetHash;
						CConceptRepresentativePropagationSetHash* mUseConceptRepPropSetHash;
						CConceptRepresentativePropagationSetHash* mPrevConceptRepPropSetHash;

						bool mInvalidSignatureBlocking;

						CConceptDescriptor* mInitConceptDescriptor;

						CXSortedNegLinker<CConcept*>* mInitializingConceptLinkerIt;
						CXSortedNegLinker<CConcept*>* mProcessInitializingConceptLinkerIt;
						CConceptAssertionLinker* mAssertionConceptLinkerIt;
						CDataAssertionLinker* mAssertionDataLinkerIt;
						CDataAssertionLinker* mLastProcessedAssertionDataLinker;
						CRoleAssertionLinker* mAssertionRoleLinkerIt;
						CReverseRoleAssertionLinker* mReverseAssertionRoleLinkerIt;
						CAdditionalProcessRoleAssertionsLinker* mAdditionalRoleAssertionsLinker;
						CAdditionalProcessDataAssertionsLinker* mAdditionalDataAssertionsLinker;
						CAdditionalProcessDataAssertionsLinker* mLastProcessedAdditionalDataAssertionsLinker;
						bool mRoleAssertionsInitialized;
						bool mReverseRoleAssertionsInitialized;
						cint64 mRoleAssertionCreationID;

						CProcessAssertedDataLiteralLinker* mAssertedDataLiteralLinker;
						CProcessAssertedDataLiteralLinker* mLastAssertedDataLiteralLinker;

						bool mBaseConceptsInitialized;

						bool mNominalIndiTriplesAssertions;
						bool mLoadedNominalIndiTriplesAssertions;

						bool mLoadedNominalIndiRepresentativeBackendData;

						cint64 mLastConceptCountCachedBlockerCandidate;
						cint64 mLastConceptCountSearchBlockerCandidate;
						cint64 mLastSearchBlockerCandidateCount;
						cint64 mLastSearchBlockerCandidateSignature;
						cint64 mBlockingCachingSavedCandidateCount;

						CXLinker<CIndividualProcessNode*>* mBlockedIndividualsLinker;

						CXLinker<CIndividualProcessNode*>* mSuccessorIndiNodeBackwardDependencyLinker;
						bool mBackwardDependencyToAncestorIndividualNode;

						CIndividualNodeModelData* indiModel;

						CIndividualNodeSatisfiableCacheRetrievalData* mSatCacheRetData;
						CIndividualNodeSatisfiableCacheRetrievalData* mPrevSatCacheRetData;
						CIndividualNodeSatisfiableCacheStoringData* mSatCacheStoringData;
						CIndividualNodeSatisfiableCacheStoringData* mPrevSatCacheStoringData;

						CIndividualNodeBackendCacheSynchronisationData* mBackendSyncData;
						CIndividualNodeBackendCacheSynchronisationData* mPrevBackendSyncData;


						CIndividualProcessNode* mProcessingBlockedIndi;
						CXLinker<CIndividualProcessNode*>* mProcessingBlockedIndividualsLinker;


						CReapplyConceptDescriptor* mSatCachedAbsorbedDisjunctionsReapplyConDes;

						CReapplyConceptDescriptor* mSatCachedAbsorbedSuccessorReapplyConDes;

						CRoleBackwardPropagationHash* mRoleBackPropHash;
						CIndividualProcessNodeLinker* mIndiProcessLinker;
						CConceptProcessLinker* mConceptProcessLinker;
						bool mRequiredBackProp;
						CIndividualProcessNode* mSubstituteIndiNode;

						bool mExtendedQueueProcessing;
						bool mImmediatelyProcessingQueued;
						bool mDetExpProcessingQueued;
						bool mDepthProcessingQueued;
						bool mBlockedReactProcessingQueued;
						bool mBackendSynchronRetestProcessingQueued;
						bool mBackendIndirectCompatibilityExpansionQueued;
						bool mBackendDirectInfluenceExpansionQueued;
						bool mIncrementalCompatibilityCheckingQueued;
						bool mIncrementalExpansionQueued;
						bool mProcessingQueued;
						CIndividualProcessNodePriority mLastProcessingPriority;

						bool mDelayedNominalProcessingQueued;
						bool mNominalProcessingDelayingChecked;
						cint64 mAssertionInitialisationSignatureValue;


						bool mCachingLossNodeReactivationInstalled;
						CNominalCachingLossReactivationData* mUseReactivationData;
						CNominalCachingLossReactivationData* mLocReactivationData;

						CSuccessorIndividualATMOSTReactivationData* mUseSuccIndiATMOSTReactivationData;
						CSuccessorIndividualATMOSTReactivationData* mLocSuccIndiATMOSTReactivationData;


						CSuccessorConnectedNominalSet* mUseNominalConnectionSet;
						CSuccessorConnectedNominalSet* mLocNominalConnectionSet;



						CDatatypesValueSpaceData* mUseDatatypesValueSpaceData;
						CDatatypesValueSpaceData* mLocDatatypesValueSpaceData;


						CIndividualNodeIncrementalExpansionData* mUseIncExpData;
						CIndividualNodeIncrementalExpansionData* mLocIncExpData;
						cint64 mIncExpID;


						CIndividualMergingHash* mUseIndividualMergingHash;
						CIndividualMergingHash* mLocIndividualMergingHash;
						CDependencyTrackPoint* mMergedDepTrackPoint;
						CIndividualProcessNode* mLastMergedIntoIndividualNode;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALPROCESSNODE_H
