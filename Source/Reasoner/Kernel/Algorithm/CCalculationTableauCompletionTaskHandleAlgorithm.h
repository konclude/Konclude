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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUCOMPLETIONTASKHANDLEALGORITHM_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUCOMPLETIONTASKHANDLEALGORITHM_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContextBase.h"
#include "CCalculationStopProcessingException.h"
#include "CCalculationErrorProcessingException.h"
#include "CCalculationClashProcessingException.h"
#include "CSatisfiableTaskConsistencyPreyingAnalyser.h"
#include "CSatisfiableTaskIncrementalConsistencyPreyingAnalyser.h"
#include "CSatisfiableTaskClassificationMessageAnalyser.h"
#include "CSatisfiableTaskMarkerIndividualPropagationAnalyser.h"
#include "CTrackedClashedDependencyLine.h"
#include "CTrackedClashedDescriptor.h"
#include "CTrackedClashedDescriptorHasher.h"
#include "CDependencyFactory.h"
#include "CClashDescriptorFactory.h"
#include "CIndividualNodeManager.h"
#include "CUnsatisfiableCacheHandler.h"
#include "CSatisfiableExpanderCacheHandler.h"
#include "CReuseCompletionGraphCacheHandler.h"
#include "CCompletionGraphCacheHandler.h"
#include "CConceptNominalSchemaGroundingHandler.h"
#include "CSaturationNodeExpansionCacheHandler.h"
#include "CDatatypeIndividualProcessNodeHandler.h"
#include "CComputedConsequencesCacheHandler.h"
#include "CIndividualNodeBackendCacheHandler.h"
#include "COccurrenceStatisticsCacheHandler.h"
#include "CIncrementalCompletionGraphCompatibleExpansionHandler.h"
#include "CSatisfiableTaskPossibleAssertionCollectingAnalyser.h"
#include "CSatisfiableTaskPropertyClassificationMessageAnalyser.h"
#include "CSatisfiableTaskComplexAnsweringMessageAnalyser.h"
#include "CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser.h"
#include "CIndexedIndividualAssertionConvertionVisitor.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CConceptProcessDescriptor.h"
#include "Reasoner/Kernel/Process/CConceptProcessingQueue.h"
#include "Reasoner/Kernel/Process/CLinkProcessingRestrictionSpecification.h"
#include "Reasoner/Kernel/Process/CBranchingORProcessingRestrictionSpecification.h"
#include "Reasoner/Kernel/Process/CBranchingMergingProcessingRestrictionSpecification.h"
#include "Reasoner/Kernel/Process/CTriggeredImplicationProcessingRestrictionSpecification.h"
#include "Reasoner/Kernel/Process/CIndividualNodeBlockingTestData.h"
#include "Reasoner/Kernel/Process/CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData.h"

#include "Reasoner/Kernel/Process/CClashedConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedIndividualLinkDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedIndividualDistinctDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedNegationDisjointLinkDescriptor.h"

#include "Reasoner/Kernel/Process/CBlockingAlternativeData.h"
#include "Reasoner/Kernel/Process/CBlockingAlternativeSignatureBlockingCandidateData.h"
#include "Reasoner/Kernel/Process/CExtendedCondensedReapplyConceptDescriptorATMOSTReactivation.h"

#include "Reasoner/Kernel/Process/CBranchingInstructionAddIndividualConcepts.h"

#include "Reasoner/Kernel/Process/Dependency/CANDDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CSOMEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CVALUEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CNEGVALUEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CALLDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CATLEASTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CORDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CAUTOMATTRANSACTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CAUTOMATCHOOSEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CSELFDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CDISTINCTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CNOMINALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CFUNCTIONALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CATMOSTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CQUALIFYDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDCONCEPTDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEDLINKDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CIMPLICATIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CCONNECTIONDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSEINDIVIDUALDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSECONCEPTSDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CREUSECOMPLETIONGRAPHDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CORDisjunctDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode.h"


#include "Reasoner/Kernel/Strategy/CConceptProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CIndividualProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CTaskProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CEqualDepthCacheOrientatedProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CConcreteConceptProcessingOperatorPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CEqualDepthTaskProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CGenerativeNonDeterministicUnsatisfiableCacheRetrievalStrategy.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Reasoner/Kernel/Cache/CExpanderBranchedLinker.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntry.h"

#include "Scheduler/CTask.h"
#include "Scheduler/CTaskHandleAlgorithm.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CDisjunctionBranchingStatistics.h"
#include "Reasoner/Ontology/CConceptNegationPair.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CMemoryAllocationException.h"


#include "Test/CCompletionGraphRandomWalkQueryGenerator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	//using namespace Test;
	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Process::Dependency;
			using namespace Task;
			using namespace Strategy;
			using namespace Cache;

			namespace Algorithm {


				/*! 
				 *
				 *		\class		CCalculationTableauCompletionTaskHandleAlgorithm
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationTableauCompletionTaskHandleAlgorithm : public CTaskHandleAlgorithm {
					// public methods
					public:
						//! Constructor
						CCalculationTableauCompletionTaskHandleAlgorithm(CUnsatisfiableCacheHandler* unsatCacheHandler, CSatisfiableExpanderCacheHandler* satExpCacheHandler, CReuseCompletionGraphCacheHandler* reuseCompGraphCacheHandler, CSaturationNodeExpansionCacheHandler* satNodeExpCacheHandler, CComputedConsequencesCacheHandler* compConsCacheHandler, CIndividualNodeBackendCacheHandler* backendCacheHandler, COccurrenceStatisticsCacheHandler* occStatsCacheHandler);

						//! Destructor
						virtual ~CCalculationTableauCompletionTaskHandleAlgorithm();

						virtual bool handleTask(CTaskProcessorContext *processorContext, CTask* task);

						void readCalculationConfig(CSatisfiableCalculationTask* satCalcTask);


						cint64 getAppliedANDRuleCount();
						cint64 getAppliedORRuleCount();
						cint64 getAppliedSOMERuleCount();
						cint64 getAppliedATLEASTRuleCount();
						cint64 getAppliedALLRuleCount();
						cint64 getAppliedATMOSTRuleCount();
						cint64 getAppliedTotalRuleCount();


					// protected methods
					protected:
						typedef void (CCalculationTableauCompletionTaskHandleAlgorithm::*TableauRuleFunction) (CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool areAllDependentFactsUnchanged(CIndividualProcessNode* individualNode, CIndividualProcessNode* backtrackedIndividualNode, CDependencyTrackPoint* prevConDepTrackPoint, CIndividualProcessNodeVector* prevIndiNodeVec, cint64& remBacktrackCount, CCalculationAlgorithmContextBase* calcAlgContext);

						bool initializeIncrementalIndividualExpansion(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividual* getNextIncrementalExpansionIndividual(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* incrementalNodeExpansion(CIndividualProcessNode* expandNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool requiresIncrementalNodeExpansion(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);



						bool incrementalMergeWithPreviousNondeterministicCompletionGraph(CCalculationAlgorithmContextBase* calcAlgContext);



						bool incrementalMergeWithPreviousDeterministicCompletionGraph(CCalculationAlgorithmContextBase* calcAlgContext);
						void pruneIncrementalRemovedSuccessors(CIndividualProcessNode*& indi, CPROCESSINGSET<cint64>* compatibleNominalNodeSet, CPROCESSINGSET<cint64>* pruningNodeSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool identifyCompatibilityChangedNominalIndividualNodes(CPROCESSINGSET<cint64>* nonCompatibleChangedNominalNodeSet, CPROCESSINGSET<cint64>* compatibleNominalNodeSet, CPROCESSINGSET<cint64>* redundantNodeSet, CPROCESSINGSET<cint64>* newNodeSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool checkCompatibilityUpdateDirectlyChangedPropagation(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool propagateDirectlyChangedNeighbourNodeConnection(CIndividualProcessNode* individualNode, bool queueIncrementalExpansion, CCalculationAlgorithmContextBase* calcAlgContext);
						bool establishDirectlyChangedNeighbourConnection(CIndividualProcessNode* individualNode, CIndividualProcessNode* neighbourNodeCandidate, bool queueIncrementalExpansion, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* searchDirectlyChangedNeighbourNodeConnection(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool clearDirectlyChangedNeighbourConnection(CIndividualProcessNode* individualNode, bool queueCompatibilityChecks, CCalculationAlgorithmContextBase* calcAlgContext);
						bool clearPropagatedDirectlyChangedNeighbourConnection(CIndividualProcessNode* individualNode, bool queueCompatibilityChecks, CCalculationAlgorithmContextBase* calcAlgContext);
						bool linkCreationDirectlyChangedNeighbourConnectionUpdate(CIndividualProcessNode* sourceIndi, CIndividualProcessNode* destIndi, bool queueIncrementalExpansion, CCalculationAlgorithmContextBase* calcAlgContext);




						bool findNextPossibleInstanceMergingIndividual(CIndividualProcessNode* processIndi, CPossibleInstancesIndividualsMergingData* possInstanceMergingData, CCalculationAlgorithmContextBase* calcAlgContext, CPROCESSHASH<CBackendRepresentativeMemoryLabelCacheItem*, bool>*& testConceptLabelSetHash, CIndividualProcessNode** foundMergingIndiNode = nullptr, cint64* foundMergingIndiId = nullptr, bool* ruleApplicationFreeMerging = nullptr);
						bool findNextPossibleInstanceMergingIndividual(CIndividualProcessNode* processIndi, CPossibleInstancesIndividualsMergingData* possInstanceMergingData, CCalculationAlgorithmContextBase* calcAlgContext, CPROCESSHASH<CBackendRepresentativeMemoryLabelCacheItem*, bool>*& testConceptLabelSetHash, cint64 individualMaxMergingAttemptCount, CIndividualProcessNode** foundMergingIndiNode = nullptr, cint64* foundMergingIndiId = nullptr, bool* ruleApplicationFreeMerging = nullptr);
						bool tryPossibleInstanceMerging(CIndividualProcessNode* processIndi, CPossibleInstancesIndividualsMergingData* possInstanceMergingData, CCalculationAlgorithmContextBase* calcAlgContext);






						bool trackIndividualReferredDependence(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						bool trackIndividualExtendedDependence(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						bool trackIndividualDependence(cint64 indiID, bool indiReferred, bool indiExtended, CCalculationAlgorithmContextBase* calcAlgContext);









						bool installSaturationCachingReactivation(CIndividualProcessNode* indiProcNode, CSaturationNodeAssociatedDependentNominalSet* nominalSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool tryInstallSaturationCachingReactivation(CIndividualProcessNode* indiProcNode, CSuccessorConnectedNominalSet* nominalSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool reactivateIndividualNodesDueToNominalCachingLoss(CIndividualProcessNode* nominalProcNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool checkIndividualNodesReactivationDueToNominalCachingLoss(CIndividualProcessNode* nominalProcNode, CCalculationAlgorithmContextBase* calcAlgContext);




						bool addCachedComputedTypes(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);






						bool isIndividualNodeBackendCacheSynchronizationProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool detectIndividualNodeBackendCacheSynchronized(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);





						CIndividualProcessNode* takeNextProcessIndividual(CCalculationAlgorithmContextBase* calcAlgContext);



						bool ensurePropagationCutLinksToExpandedIndividual(CIndividualProcessNode* propCutIndiNode, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locPropCutIndiBackendSyncData, CBackendNeighbourExpansionQueueDataLinker* backendNeighbourExpDataLinker, cint64 neighbourIndiId, CCalculationAlgorithmContextBase* calcAlgContext);

						bool continueIndividualProcessing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool tableauRuleProcessing(CIndividualProcessNode*& indiProcNode,CConceptProcessDescriptor*& conProcDes, CCalculationAlgorithmContextBase* calcAlgContext);
						bool individualNodeInitializing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void individualNodeConclusion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void tableauRuleChoice(CIndividualProcessNode*& indiProcNode, CConceptProcessDescriptor*& conProcDes, CCalculationAlgorithmContextBase* calcAlgContext);
						bool initialNodeInitialize(CIndividualProcessNode*& indiProcNode, bool allowPreprocess, CCalculationAlgorithmContextBase* calcAlgContext);

						void eliminiateBlockedIndividuals(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isIndividualNodeCompletionGraphCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool detectIndividualNodeCompletionGraphCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);


						bool isIndividualNodeValidBlocker(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);


						bool rootUnsatisfiabilityWriteCaches(CSatisfiableCalculationTask* task, CCalculationAlgorithmContextBase* calcAlgContext);


						void clashedBacktracking(CClashedDependencyDescriptor* clashes, CCalculationAlgorithmContextBase* calcAlgContext);
						bool cancellationRootTask(CCalculationAlgorithmContextBase* calcAlgContext);
						bool cancellationTask(CSatisfiableCalculationTask* task, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* getFreeTrackedClashedDescriptor(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackFromTrackingLine(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackFromTrackingLineStep(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackNonDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackNonDeterministicBranchingClashedDescriptorFromPreviousIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackNonDeterministicBranchingClashedDescriptor(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* getCollectedFilteredClashedDescriptorsFromBranch(CTrackedClashedDescriptor* nonDetClashedPointingDes, CNonDeterministicDependencyNode* nonDetBranchDepNode, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan = nullptr);
						bool backtrackDeterministicBranchingClashedDescriptorFromCurrentIndividualNodeLevel(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool backtrackDeterministicClashedDescriptorFromPreviousIndividualNodeLevels(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* getBacktrackedDeterministicClashedDescriptorsBeforeProcessingTag(CTrackedClashedDescriptor* trackedClashedDescriptors, cint64 processingTag, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* getBacktrackedDeterministicClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, cint64* minIndiLevel, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* tryGetInvalidSameIndividualNodeLevelBacktrackedDeterministicClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, cint64* minIndiLevel, CCalculationAlgorithmContextBase* calcAlgContext);
						bool initializeTrackingLine(CTrackedClashedDependencyLine* trackingLine, CTrackedClashedDescriptor* trackingClashes, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* createTrackedClashesDescriptors(CClashedDependencyDescriptor* clashes, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan = nullptr, bool copyIndependentConceptDescriptors = false);
						CTrackedClashedDescriptor* createTrackedClashesDescriptor(CClashedDependencyDescriptor* clashDes, CCalculationAlgorithmContextBase* calcAlgContext, CMemoryAllocationManager* tmpMemMan = nullptr, bool copyIndependentConceptDescriptors = false);
						CIndividualProcessNode* getCoresspondingIndividualNodeFromDependency(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getCoresspondingIndividualNodeFromDependency(CDependencyNode* depNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isGeneratingConceptSatisfiableCachedAbsorpable(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext);

						bool reapplySatisfiableCachedAbsorbedGeneratingConcepts(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool reapplySatisfiableCachedAbsorbedDisjunctionConcepts(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);


						void checkValueSpaceDistinctSatisfiability(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void triggerValueSpaceConcepts(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addtriggeredValueSpaceConcepts(CIndividualProcessNode* processIndi, CConceptDescriptor* triggeredConceptLinker, CCalculationAlgorithmContextBase* calcAlgContext);


						void tryCompletionGraphReuse(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext);



						void upgradeSignatureBlockingToIndividualReusing(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void establishIndividualReusing(CIndividualProcessNode* processIndi, CIndividualProcessNode* reuseIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateIndividualReusing(CIndividualProcessNode* processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* searchSignatureReusingIndividualNode(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasCompatibleConceptSetReuse(CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* subConceptSet, CIndividualProcessNode* reuseNodeCand, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addReusingBlockerFollowing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool removeReusingBlockerFollowing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndirectSuccessorReuseBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void reactivateIndirectReuseSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext);
						void removeIndividualReusing(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);


						bool anlyzeIndiviudalNodesConceptExpansion(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isConceptFromDirectOrPredecessorOrNondeterminismusDependent(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, bool* directDependentFlag, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isConceptSignatureBlockingCritical(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						bool getConceptDependenciesToSameIndividualNode(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CXLinker<CConceptDescriptor*>*& depLinker, CCalculationAlgorithmContextBase* calcAlgContext);

						bool hasCompatibleConceptSetSignature(CIndividualProcessNode*& blockingNode, CReapplyConceptLabelSet* conSet, CIndividualProcessNode* compatibleTestNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool detectIndividualNodeSignatureBlockingStatus(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isSignatureBlockedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool establishIndividualNodeSignatureBlocking(CIndividualProcessNode*& blockingIndividualNode, CIndividualProcessNode*& blockerIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool refreshIndividualNodeSignatureBlocking(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool updateSignatureBlockingConceptExpansion(CIndividualProcessNode*& blockingIndividualNode, CSignatureBlockingIndividualNodeConceptExpansionData* sigBlockingData, CIndividualProcessNode*& blockerIndividualNode, CIndividualNodeAnalizedConceptExpansionData* blockerAnalizedConExpData, CCalculationAlgorithmContextBase* calcAlgContext);
					
						bool removeSignatureBlockingBlockerFollowing(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addSignatureBlockingBlockerFollowing(CIndividualProcessNode*& blockingIndividualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void rebuildSignatureBlockingCandidateHash(CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateIndirectSuccessorSignatureBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void reactivateIndirectSignatureBlockedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext);

						bool updateBlockingReviewMarking(CIndividualProcessNode*& blockingIndividualNode, bool isBlocked, CCalculationAlgorithmContextBase* calcAlgContext);

						bool addSignatureIndividualNodeBlockerCandidate(CIndividualProcessNode*& indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						CIndividualProcessNode* searchSignatureIndividualNodeBlocker(CIndividualProcessNode*& blockingNode, CCalculationAlgorithmContextBase* calcAlgContext);


						bool writeClashDescriptorsToCache(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool writeClashDescriptorsToCache(CTrackedClashedDescriptor*& trackedClashedDes, CTrackedClashedDescriptor* additionalTrackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool writeClashDescriptorsToCache(CTrackedClashedDescriptor*& trackedClashedDes, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						bool writeUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext);
						CTrackedClashedDescriptor* getSortedClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isClashedDescriptorSortedBefore(CTrackedClashedDescriptor* trackedClashedDesBefore, CTrackedClashedDescriptor* trackedClashedDesAfter, CCalculationAlgorithmContextBase* calcAlgContext);


						bool addIndiNodeSignatureOfUnsatisfiableClashedDescriptors(CTrackedClashedDescriptor* trackedClashedDes, CCalculationAlgorithmContextBase* calcAlgContext);

						QString generateDebugTrackedClashedDescriptorSummaryString(CTrackedClashedDescriptor* trackedClashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugTrackedClashedDescriptorString(CTrackedClashedDescriptor* trackedClashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugTrackingLineString(CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);
						

						QString writeDebugTrackingLineStringToFile(const QString& debugDataString, const QString& fileNameString, CTrackedClashedDependencyLine* trackingLine, CCalculationAlgorithmContextBase* calcAlgContext);


						QString generateDebugMergingQueueString(CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);


						bool writeSatisfiableCachedIndividualNodesOfUnsatisfiableBranch(CCalculationAlgorithmContextBase* calcAlgContext);
						bool testAllSuccessorsProcessedAndWriteSatisfiableCache(CIndividualProcessNode* indiNode, CPROCESSINGSET<CIndividualProcessNode*>* processedNodeSet, CSatisfiableExpanderCacheHandler* satExpHandler, CCalculationAlgorithmContextBase* calcAlgContext);

						bool cacheSatisfiableIndividualNodes(CCalculationAlgorithmContextBase* calcAlgContext);
						bool isSatisfiableCachedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isSatisfiableCachedCompatible(CIndividualProcessNode*& individualNode, CExpanderBranchedLinker* satBranchLinker, CIndividualProcessNode* ancestorIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isSatisfiableCachedAutomatConceptCompatible(CIndividualProcessNode*& individualNode, CConcept* concept, bool negated, CIndividualProcessNode* ancestorIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool detectIndividualNodeSatisfiableExpandedCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void expandCachedConcepts(CIndividualProcessNode*& individualNode, CSignatureSatisfiableExpanderCacheEntry* entry, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndirectSuccessorSatisfiableCached(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void reactivateIndirectSatisfiableCachedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext);
						void addSatisfiableCachedAbsorbedDisjunctionConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void addSatisfiableCachedAbsorbedGeneratingConcept(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);




						bool analyseBranchingStatistics(CCalculationAlgorithmContextBase* calcAlgContext);




						bool isSaturationCachedProcessingBlocked(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool detectIndividualNodeSaturationCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void reactivateIndirectSaturationCachedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext);





						void markRelevanceForTrackedClashedDescriptors(CTrackedClashedDescriptor* descriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						void markDependencyRelevance(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);


						void addIndividualNodeForCacheUnsatisfiableRetrieval(CIndividualProcessNode*& indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void testIndividualNodeUnsatisfiableCached(CIndividualProcessNode* individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void commitCacheMessages(CCalculationAlgorithmContextBase* calcAlgContext);





						bool hasCommonVariableBindings(CIndividualProcessNode*& processIndi, CRepresentativeVariableBindingPathMap* leftRepVarBindMap, CRepresentativeVariableBindingPathMap* rightRepVarBindMap, CCalculationAlgorithmContextBase* calcAlgContext);

						bool areRepresentativesJoinable(CIndividualProcessNode*& processIndi, CRepresentativeVariableBindingPathSetData* leftRepData, CRepresentativeVariableBindingPathSetData* rightRepData, CSortedLinker<CVariable*>* varLinker, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyREPRESENTATIVEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyREPRESENTATIVEJOINRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyREPRESENTATIVEBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyREPRESENTATIVEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateRepresentativeToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyREPRESENTATIVEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyREPRESENTATIVEANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						bool requiresRepresentativePropagation(CIndividualProcessNode*& processIndi, CRepresentativePropagationDescriptor* repPropDes, CRepresentativePropagationSet* testRepPropSet, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateRepresentative(CIndividualProcessNode*& processIndi, CRepresentativePropagationDescriptor* repPropDes, CRepresentativePropagationSet* repPropSet, CDependencyTrackPoint* nextDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateRepresentativePropagationSet(CIndividualProcessNode*& processIndi, CRepresentativePropagationSet* repPropSet, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void reapplyConceptUpdatedRepresentative(CIndividualProcessNode*& processIndi, CConceptDescriptor* bindingConDes, CDependencyTrackPoint* bindingDepTrackPoint, CReapplyConceptLabelSet* conSet, CCondensedReapplyQueue* reapplyQueue, CCalculationAlgorithmContextBase* calcAlgContext);
						void reapplyConceptUpdatedRepresentative(CIndividualProcessNode*& processIndi, CConceptDescriptor* bindingConDes, CDependencyTrackPoint* bindingDepTrackPoint, cint64 bindingCount, CReapplyConceptLabelSet* conSet, CCondensedReapplyQueue* reapplyQueue, CCalculationAlgorithmContextBase* calcAlgContext);

						CRepresentativeVariableBindingPathSetJoiningKeyMap* getRepresentativeJoiningKeyData(CRepresentativeVariableBindingPathSetData* repVarBindPathSetData, CConcept* joinConcept, CCalculationAlgorithmContextBase* calcAlgContext);

						void createCommonJoiningKeyMap(CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap, CRepresentativeVariableBindingPathSetJoiningKeyMap* firstJoiningKeyMap, CRepresentativeVariableBindingPathSetData* firstRepData,
								CRepresentativeVariableBindingPathSetJoiningKeyMap* secJoiningKeyMap, CRepresentativeVariableBindingPathSetData* secRepData, bool firstLeft, CCalculationAlgorithmContextBase* calcAlgContext);

						void createCommonJoiningAll(CRepresentativeJoiningCommonKeyMap* repJoinCommonKeyMap, CRepresentativeJoiningAllDataExtension* joinAllExtData, CRepresentativeVariableBindingPathSetData* leftRepData, CRepresentativeVariableBindingPathSetData* rightRepData, CCalculationAlgorithmContextBase* calcAlgContext);









						void applyVARIABLEBINDINGANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVARBINDPROPAGATEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVARBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVARBINDPROPAGATEJOINRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool triggerVariableBindingPathJoining(CIndividualProcessNode* processIndi, CVariableBindingPathDescriptor* varBindPathDes, CVariableBindingDescriptor* varBindDes, bool leftTriggered, CVariableBindingTriggerHash* varBindTriggerHash, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool propagateVariableBindingsJoins(CIndividualProcessNode* processIndi, CConceptDescriptor* joiningConDes, CConcept* joinConcept, CVariableBindingPathDescriptor* varBindPathDes, bool leftTriggerPath,  
								CVariableBindingPathJoiningHash* varBindPathJoinHash, CConceptVariableBindingPathSetHash* varBindingPathSetHash, CConceptDescriptor*& joinConDes, CVariableBindingPathSet*& varBindingPathSet, CCalculationAlgorithmContextBase* calcAlgContext);

						CVariableBindingDescriptor* createVariableBindingPathKey(CIndividualProcessNode* processIndi, CSortedLinker<CVariable*>* varLinker, CVariableBindingDescriptor* varBindDes, CCalculationAlgorithmContextBase* calcAlgContext);
						CVariableBindingPath* getJoinedVariableBindingPath(CVariableBindingPath* leftVarBindPath, CVariableBindingPath* rightVarBindPath, CCalculationAlgorithmContextBase* calcAlgContext);
						void forceVariableBindingJoinCreated(CIndividualProcessNode* processIndi, CConceptDescriptor* joiningConDes, CConcept* joinConcept, CConceptDescriptor*& joinConDes, CDependencyTrackPoint* mergedDependencyTrackPoint, CVariableBindingPathSet*& varBindingPathSet, CConceptVariableBindingPathSetHash* varBindingPathSetHash, CCalculationAlgorithmContextBase* calcAlgContext);

						bool propagateInitialVariableBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CDependency* otherDependencies, CConceptVariableBindingPathSetHash* conVarBindingSetHash, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateFreshVariableBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CDependency* otherDependencies, CConceptVariableBindingPathSetHash* conVarBindingSetHash, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateVariableBindingsToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateInitialVariableBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CIndividualLinkEdge* restLink, CConceptVariableBindingPathSetHash* conVarBindingSetHash, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateFreshVariableBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CVariableBindingPathSet* newVarBindingSet, CVariableBindingPathSet* prevVarBindingSet, CIndividualLinkEdge* restLink, CConceptVariableBindingPathSetHash* conVarBindingSetHash, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyVARBINDPROPAGATEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVARBINDPROPAGATEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);


						void applyVARBINDPREPARERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVARBINDFINALIZERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);










						void applyBINDPROPAGATEGROUNDINGRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDPROPAGATECYCLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDPROPAGATEALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDPROPAGATEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDPROPAGATEANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDPROPAGATEANDFLAGALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagatePropagationBindings(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, bool propagateAllFlag, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBINDVARIABLERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						bool propagateInitialPropagationBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateFreshPropagationBindings(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CDependency* otherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagatePropagationBindingsToSuccessor(CIndividualProcessNode* processIndi, CIndividualProcessNode*& succIndi, CSortedNegLinker<CConcept*>* conceptOpLinker, bool negate, CConceptDescriptor* conDes, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateInitialPropagationBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext);
						bool propagateFreshPropagationBindingsToSuccessor(CIndividualProcessNode*& processIndi, CIndividualProcessNode* succIndi, CConceptDescriptor* conDes, CPropagationBindingSet* newPropBindingSet, CPropagationBindingSet* prevPropBindingSet, CIndividualLinkEdge* restLink, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CPropagationBindingReapplyConceptDescriptor* reapplyDesLinker, CCalculationAlgorithmContextBase* calcAlgContext);






						void addRoleAssertion(CIndividualProcessNode*& processIndi, CRoleAssertionLinker* roleAssertionLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void addReverseRoleAssertion(CIndividualProcessNode*& processIndi, CReverseRoleAssertionLinker* reverseRoleAssertionLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						void addDataAssertion(CIndividualProcessNode*& processIndi, CDataAssertionLinker* dataAssertionLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						bool checkBackendCachedNominalConnection(CIndividualProcessNode*& processIndi, CRole* role, cint64 nominalId, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyBOTTOMRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applySOMERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyORRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applySELFRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyAutomatANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyAutomatTransactions(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CConcept* concept, bool negated, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyFUNCTIONALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyATMOSTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyATLEASTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNOMINALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyVALUERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATALITERALRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATARESTRICTIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATATYPERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNOMINALIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATALITERALIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATATYPEIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyDATARESTRICTIONIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);


						void applyNegAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegANDRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegSOMERule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegALLRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegORRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegATMOSTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyNegATLEASTRule(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);


						bool hasIdenticalConceptOperands(CSortedNegLinker<CConcept*>* opConLinker1, CSortedNegLinker<CConcept*>* opConLinker2);




						CREPRESENTATIVEGROUNDINGDependencyNode* createREPRESENTATIVEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CVariableBindingPath* selectVarBindPath, CCalculationAlgorithmContextBase* calcAlgContext);
						CREPRESENTATIVEJOINDependencyNode* createREPRESENTATIVEJOINDependency(CDependencyTrackPoint*& joinContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREPRESENTATIVEBINDVARIABLEDependencyNode* createREPRESENTATIVEBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREPRESENTATIVEIMPLICATIONDependencyNode* createREPRESENTATIVEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CREPRESENTATIVEALLDependencyNode* createREPRESENTATIVEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREPRESENTATIVEANDDependencyNode* createREPRESENTATIVEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CRESOLVEREPRESENTATIVEDependencyNode* createRESOLVEREPRESENTATIVEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CRepresentativeVariableBindingPathMap* resolveVarBindPathMap, CRepresentativePropagationMap* resolveRepPropMap, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* additionalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);


						CVARBINDPROPAGATEJOINDependencyNode* createVARBINDPROPAGATEJOINDependency(CDependencyTrackPoint*& continueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* otherDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVARBINDPROPAGATEALLDependencyNode* createVARBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVARBINDPROPAGATEANDDependencyNode* createVARBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATEVARIABLEBINDINGDependencyNode* createPROPAGATEVARIABLEBINDINGDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATEVARIABLEBINDINGSSUCCESSORDependencyNode* createPROPAGATEVARIABLEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVARBINDVARIABLEDependencyNode* createVARBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVARBINDPROPAGATEGROUNDINGDependencyNode* createVARBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATEVARIABLECONNECTIONDependencyNode* createPROPAGATEVARIABLECONNECTIONDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVARBINDPROPAGATEIMPLICATIONDependencyNode* createVARBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);

						CBINDPROPAGATEGROUNDINGDependencyNode* createBINDPROPAGATEGROUNDINGDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATECONNECTIONAWAYDependencyNode* createPROPAGATECONNECTIONAWAYDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATECONNECTIONDependencyNode* createPROPAGATECONNECTIONDependency(CIndividualProcessNode* processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CBINDPROPAGATECYCLEDependencyNode* createBINDPROPAGATECYCLEDependency(CDependencyTrackPoint*& continueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* triggDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CBINDPROPAGATEALLDependencyNode* createBINDPROPAGATEALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATEBINDINGSSUCCESSORDependencyNode* createPROPAGATEBINDINGSSUCCESSORDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CBINDPROPAGATEIMPLICATIONDependencyNode* createBINDPROPAGATEIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CBINDPROPAGATEANDDependencyNode* createBINDPROPAGATEANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CPROPAGATEBINDINGDependencyNode* createPROPAGATEBINDINGDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CBINDVARIABLEDependencyNode* createBINDVARIABLEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CANDDependencyNode* createANDDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CSOMEDependencyNode* createSOMEDependency(CDependencyTrackPoint*& someContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CSELFDependencyNode* createSELFDependency(CDependencyTrackPoint*& selfContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CVALUEDependencyNode* createVALUEDependency(CDependencyTrackPoint*& valueContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CROLEASSERTIONDependencyNode* createROLEASSERTIONDependency(CDependencyTrackPoint*& roleAssContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CRole* baseAssertionRole, CIndividual* baseAssertionIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						CDATAASSERTIONDependencyNode* createDATAASSERTIONDependency(CDependencyTrackPoint*& roleAssContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CALLDependencyNode* createALLDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CATLEASTDependencyNode* createATLEASTDependency(CDependencyTrackPoint*& atleastContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CAUTOMATTRANSACTIONDependencyNode* createAUTOMATTRANSACTIONDependency(CDependencyTrackPoint*& allContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* linkDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CAUTOMATCHOOSEDependencyNode* createAUTOMATCHOOSEDependency(CDependencyTrackPoint*& andContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CDISTINCTDependencyNode* createDISTINCTDependency(CDependencyTrackPoint*& distinctContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CNEGVALUEDependencyNode* createNEGVALUEDependency(CDependencyTrackPoint*& negValueContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CNOMINALDependencyNode* createNOMINALDependency(CDependencyTrackPoint*& nominalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CFUNCTIONALDependencyNode* createFUNCTIONALDependency(CDependencyTrackPoint*& functionalContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevLink1DependencyTrackPoint, CDependencyTrackPoint* prevLink2DependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						
						CORDependencyNode* createORDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CATMOSTDependencyNode* createATMOSTDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CMERGEDependencyNode* createMERGEDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSEINDIVIDUALDependencyNode* createREUSEINDIVIDUALDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSECOMPLETIONGRAPHDependencyNode* createREUSECOMPLETIONGRAPHDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode* createMERGEPOSSIBLEINSTANCEINDIVIDUALDependencyNode(CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CIndividualProcessNode* mergingIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSECONCEPTSDependencyNode* createREUSECONCEPTSDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CQUALIFYDependencyNode* createQUALIFYDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						CMERGEDCONCEPTDependencyNode* createMERGEDCONCEPTDependency(CDependencyTrackPoint*& mergedConceptContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* conceptPrevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CMERGEDLINKDependencyNode* createMERGEDLINKDependency(CDependencyTrackPoint*& mergedLinkContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* linkPrevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CMERGEDIndividualDependencyNode* createMERGEDINDIVIDUALDependency(CDependencyTrackPoint*& mergedIndividualContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* mergePrevDepTrackPoint, CDependencyTrackPoint* individualPrevDepTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						CORONLYOPTIONDependencyNode* createORONLYOPTIONDependency(CDependencyTrackPoint*& orContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CIMPLICATIONDependencyNode* createIMPLICATIONDependency(CDependencyTrackPoint*& implContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CEXPANDEDDependencyNode* createEXPANDEDDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependency* prevOtherDependencies, CCalculationAlgorithmContextBase* calcAlgContext);
						CCONNECTIONDependencyNode* createCONNECTIONDependency(CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						CSAMEINDIVIDUALSMERGEDependencyNode* createSAMEINDIVIDUALMERGEDependency(CDependencyTrackPoint*& expContinueDepTrackPoint, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* prevOtherDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						CNonDeterministicDependencyTrackPoint* createNonDeterministicDependencyTrackPointBranch(CNonDeterministicDependencyNode* dependencyNode, bool singleBranch, CCalculationAlgorithmContextBase* calcAlgContext);

						CREUSEBACKENDEXPANSIONMODESDependencyNode* createREUSEBACKENDEXPANSIONMODESDependency(CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSEBACKENDFIXEDINDIVIDUALEXPANSIONDependencyNode* createREUSEBACKENDFIXEDINDIVIDUALEXPANSIONDependency(CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSEBACKENDPRIORITIZEDINDIVIDUALEXPANSIONDependencyNode* createREUSEBACKENDPRIORITIZEDINDIVIDUALEXPANSIONDependency(CIndividualProcessNode*& processIndi, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CREUSEBACKENDVALUEDependencyNode* createREUSEBACKENDVALUEDependency(CDependencyTrackPoint*& valueDepTrackPoint, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CDependencyTrackPoint* nominalDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);


						bool isRestrictedTopObjectPropertyPropagation(CIndividualProcessNode*& processIndi, CIndividualProcessNode*& destIndi, CConcept* concept, bool negated, CCalculationAlgorithmContextBase* calcAlgContext);


						bool isConceptAdditionAtomaric(CConcept* addingConcept, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);

						CClashedDependencyDescriptor* createIndividualMergeCausingDescriptors(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CIndividualLinkEdge* link, CSortedNegLinker<CConcept*>* conceptAddLinker, CCalculationAlgorithmContextBase* calcAlgContext);


						CClashedDependencyDescriptor* createClashedIndividualNodeDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						CClashedDependencyDescriptor* createClashedConceptDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualProcessNode*& processIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CClashedDependencyDescriptor* createClashedIndividualLinkDescriptor(CClashedDependencyDescriptor* prevClashes, CIndividualLinkEdge* link, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CClashedDependencyDescriptor* createClashedIndividualDistinctDescriptor(CClashedDependencyDescriptor* prevClashes, CDistinctEdge* distinct, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CClashedDependencyDescriptor* createClashedNegationDisjointDescriptor(CClashedDependencyDescriptor* prevClashes, CNegationDisjointEdge* disjointNegLink, CDependencyTrackPoint* prevDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isNominalIndividualNodeAvailable(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualProcessNode* getCorrectedNominalIndividualNode(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getCorrectedMergedIntoIndividualNode(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getForcedInitializedNominalIndividualNode(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);


						void initializeMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CRoleSuccessorLinkIterator* roleSuccIt, CIndividualLinkEdge* usingLastLink, CSortedNegLinker<CConcept*>* conceptOpLinkerIt, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);
						bool qualifyMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);
						bool mergeMergingIndividualNodes(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, cint64 linkCount, cint64 cardinality, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);
						bool mergeMergingIndividualNodesPairwise(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, cint64 linkCount, cint64 cardinality, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);
						CSatisfiableCalculationTask* createMergeBranchingTask(CIndividualProcessNode*& processIndiNode, CConceptProcessDescriptor*& conProDes, CIndividualProcessNode*& distinctIndiNode, CIndividualProcessNode*& mergingIndiNode, CNonDeterministicDependencyNode* mergeDependencyNode, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);
						CSatisfiableCalculationTask* createDistinctBranchingTask(CIndividualProcessNode*& processIndiNode, CConceptProcessDescriptor*& conProDes, CIndividualProcessNode*& distinctIndiNode, bool createAsNominal, CNonDeterministicDependencyNode* mergeDependencyNode, CBranchingMergingProcessingRestrictionSpecification* branchingMergingProcRest, CCalculationAlgorithmContextBase* calcAlgContext);

						

						bool getAdditionalDisjunctCheckingConcept(CConcept* opConcept, bool opConNegation, CConcept** checkingConcept, bool* checkingNegation, CCalculationAlgorithmContextBase* calcAlgContext);


						bool hasSaturatedClashedFlagForConcept(CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool initializeORProcessing(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification** plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext);
						bool planORProcessing(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification** plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext);
						void executeORBranching(CIndividualProcessNode*& processIndi, CConceptProcessDescriptor*& conProDes, bool negate, CBranchingORProcessingRestrictionSpecification* plannedBranchingProcessRestriction, CCalculationAlgorithmContextBase* calcAlgContext);

						CSatisfiableCalculationTask* createDependendBranchingTaskList(cint64 newTaskCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void prepareBranchedTaskProcessing(CIndividualProcessNode*& individual, CSatisfiableCalculationTask* newTask, CCalculationAlgorithmContextBase* calcAlgContext);

						CConceptRoleBranchingTrigger* searchNextConceptRoleBranchTrigger(CIndividualProcessNode*& processIndi, CConceptRoleBranchingTrigger* triggers, CCalculationAlgorithmContextBase* calcAlgContext);
						void installConceptRoleBranchTrigger(CIndividualProcessNode*& processIndi, CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CProcessingRestrictionSpecification* procRest, CConceptRoleBranchingTrigger* trigger, CCalculationAlgorithmContextBase* calcAlgContext);

						CConceptDescriptor* addConceptToIndividualReturnConceptDescriptor(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToIndividualSkipANDProcessing(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, bool markModification, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CConceptAssertionLinker* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, bool allowPreprocessing, bool allowInitalization, cint64* conceptCount, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool insertConceptsToIndividualConceptSet(CConceptDescriptor* conceptDescriptor, CDependencyTrackPoint* dependencyTrackPoint, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCondensedReapplyQueueIterator* reapplyIt, bool allowInitalization, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addBlockingCoreConcept(CConceptDescriptor* conceptDescriptor, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isConceptUnsatisfiabilitySaturated(CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext);


						bool needsProcessingForConcept(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						void insertConceptProcessDescriptorToProcessingQueue(CConceptProcessDescriptor* conProDes, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void insertConceptProcessDescriptorToProcessingQueue(CConceptProcessDescriptor* conProDes, CConceptProcessingQueue*& conceptProcessingQueue, cint64 bindingCount, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						void addConceptPreprocessedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, cint64 bindingCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptPreprocessedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool allowPreprocessing, CCalculationAlgorithmContextBase* calcAlgContext, TableauRuleFunction skipFunction = nullptr);
						void addConceptToProcessingQueue(CConceptProcessDescriptor* reinsertConProDes, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addCopiedConceptToProcessingQueue(CConceptProcessDescriptor* copyConProDes, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, double priorityOffset, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptRestrictedFixedPriorityToProcessingQueue(CConceptDescriptor *conceptDescriptor, CDependencyTrackPoint* depTrackPoint, CConceptProcessingQueue*& conceptProcessingQueue, CIndividualProcessNode*& processIndi, bool reapplied, CProcessingRestrictionSpecification* procRestriction, double priority, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToProcessingQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToProcessingQueueBasedOnProcessingConcepts(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToBlockingUpdateReviewProcessingQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToBackendSynchronisationRetestQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);

						bool addIndividualToBackendDirectInfluenceExpansionQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToBackendIndirectCompatibilityExpansionQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToBackendReuseExpansionQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToBackendNeighbourExpansionQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);

						bool registerProcessedIndividualForBackendConceptSetLabel(CIndividualProcessNode* individual, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CCalculationAlgorithmContextBase* calcAlgContext);




						bool addIndividualToIncrementalCompatibilityCheckingQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addIndividualToIncrementalExpansionQueue(CIndividualProcessNode* individual, CCalculationAlgorithmContextBase* calcAlgContext);



						bool isConceptInReapplyQueue(CConceptDescriptor* conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isConceptInReapplyQueue(CConceptDescriptor* conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						void addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, CProcessingRestrictionSpecification* procRest, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						void addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CRole* role, CIndividualProcessNode*& processIndi, bool isStaticDes, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CConcept* concept, bool negation, CIndividualProcessNode*& processIndi, bool isStaticDes, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToReapplyQueue(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CConcept* concept, bool negation, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyReapplyQueueConceptsRestricted(CIndividualProcessNode*& processIndi, CReapplyQueueIterator* reapplyQueueIt, CIndividualLinkEdge* restrictedLink, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CCondensedReapplyQueueIterator* reapplyQueueIt, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyExtendedReapplyConceptDescriptor(CIndividualProcessNode*& processIndi, CConcept* concept, bool negation, CCondensedReapplyConceptDescriptor* reapplyConceptDes, CCalculationAlgorithmContextBase* calcAlgContext);


						bool hasRoleSuccessorConcept(CIndividualProcessNode*& processIndi, CRole* role, CConcept* concept, bool conceptNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasRoleSuccessorConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getRoleSuccessorWithConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasDistinctRoleSuccessorConcepts(CIndividualProcessNode*& processIndi, CRole* role, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, cint64 distinctCount, CCalculationAlgorithmContextBase* calcAlgContext);

						bool hasAncestorIndividualNode(CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcept(CIndividualProcessNode*& testIndi, CConcept* conTest, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcept(CReapplyConceptLabelSet* conLabelSet, CConcept* conTest, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool negated, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcepts(CReapplyConceptLabelSet* conLabelSet, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool negated, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcepts(CReapplyConceptLabelSet* conLabelSet, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, bool* containsNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool containsIndividualNodeConcepts(CIndividualProcessNode*& testIndi, CSortedNegLinker<CConcept*>* conTestLinkerIt, CCalculationAlgorithmContextBase* calcAlgContext);



						bool isLabelConceptSubSetIgnoreNominals(CReapplyConceptLabelSet* subConceptSet, CReapplyConceptLabelSet* superConceptSet, bool* clashFlag, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isLabelConceptSubSet(CReapplyConceptLabelSet* subConceptSet, CReapplyConceptLabelSet* superConceptSet, CConceptDescriptor** firstNotEntailedConDes, bool* equalSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptEqualSet(CReapplyConceptLabelSet* conceptSet1, CReapplyConceptLabelSet* conceptSet2, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isPairwiseLabelConceptEqualSet(CReapplyConceptLabelSet* conceptSet1, CReapplyConceptLabelSet* conceptSet1Pair, CReapplyConceptLabelSet* conceptSet2, CReapplyConceptLabelSet* conceptSet2Pair, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptClashSet(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptEqualSetConsiderNominalsForClashOnly(CReapplyConceptLabelSet* conceptSet1, CReapplyConceptLabelSet* conceptSet2, bool* clashFlag, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptClashSet(CReapplyConceptLabelSet* subConceptSet, CReapplyConceptLabelSet* superConceptSet, bool* subSetFlag, bool ignoreNominalsForSubsetChecking, CCalculationAlgorithmContextBase* calcAlgContext);


						bool hasOptimizedBlockingB2AutomateTransitionOperands(CConcept* opConcept, CRole* role, CReapplyConceptLabelSet* vConSet, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptOptimizedBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptSubSetBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptEqualBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isLabelConceptEqualPairwiseBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isIndividualNodeBlocking(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);


						bool isNominalVariablePropagationBindingSubSet(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						
						
						bool areVariablePropagationBindingsCompatible(CVariableBindingPath* varBindPath1, CVariableBindingPath* varBindPath2, CCalculationAlgorithmContextBase* calcAlgContext);
						QSet< QSet<CConcept*> > getIndividualNodeAssociatedConceptsSetFromVariablePropagationBindings(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);
						QSet< QSet<CConcept*> > getIndividualNodeAssociatedConceptsSetFromVariablePropagationBindingsCached(CIndividualProcessNode*& individualNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool collectIndividualNodeVariablePropagationBindings(CIndividualProcessNode*& individualNode, QHash<cint64, CVariableBindingPath*>& collecingPropagationVariableBindingsHash, CCalculationAlgorithmContextBase* calcAlgContext);

						QSet< QList< QSet<CConcept*> > > getIndividualNodesListAssociatedConceptsSetFromVariablePropagationBindings(CIndividualProcessNode*& individualNode, CIndividualProcessNode*& ancestorIndividualNode, const QList<cint64>& dependentNominalIdList, CCalculationAlgorithmContextBase* calcAlgContext);
						QSet<CConcept*> getConceptsForCompatibleVariablePropagationBindings(CIndividualProcessNode*& individualNode, CVariableBindingPath* varBindPath, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isAnonymousVariablePropagationBindingSingleIndividualAnalogousPath(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isAnonymousVariablePropagationBindingAnalogousPath(CIndividualProcessNode*& testIndi, CIndividualProcessNode*& blockingIndi, CIndividualNodeBlockingTestData* blockData, bool testContinueBlocking, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugIndividualNodesListAssociatedConceptsSetString(CIndividualProcessNode*& individualNode, CIndividualProcessNode*& ancestorIndividualNode, const QList<cint64>& dependentNominalIdList, const QSet< QList< QSet<CConcept*> > >& testIndiAllVariableMappingsAssociatedConceptsOverNodesListSet, const QString& nodeNaming, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugIndividualNodeAssociatedConceptsString(cint64 indiNodeId, const QSet<CConcept*>& associatedConcepts, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugIndividualNodeAssociatedConceptsSetString(CIndividualProcessNode*& individualNode, const QSet< QSet<CConcept*> >& allVariableMappingsAssociatedConceptsSet, CCalculationAlgorithmContextBase* calcAlgContext);
						cint64 getBindingsCompatibleConceptSetsHashValue(const QSet< QSet<CConcept*> >& associatedConceptSets, CCalculationAlgorithmContextBase* calcAlgContext);



						CBlockingIndividualNodeCandidateIterator getBlockingIndividualNodeCandidateIterator(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addIndividualNodeCandidateForConcept(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext);
						void addIndividualNodeCandidateForConcept(CIndividualProcessNode*& indi, CSortedNegLinker<CConcept*>* concepts, bool negated, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateIndividualNodeModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void setIndividualNodeConceptLabelSetModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void setIndividualNodeAncestorConnectionModified(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isIndividualNodeConceptLabelSetModified(CIndividualProcessNode*& indi, cint64 modTag, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateProcessingRestrictionToAncestor(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateAddingProcessingRestrictionToAncestor(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateAddingBlockedProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateAddingProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 addRestrictionFlags, bool recursive, cint64 whileNotContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateClearingProcessingRestrictionToSuccessors(CIndividualProcessNode*& indi, cint64 clearRestrictionFlags, bool recursive, cint64 whileContainsFlags, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateIndirectSuccessorBlocking(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void pruneSuccessors(CIndividualProcessNode*& indi, CIndividualProcessNode* ancestorIndi, bool removeNominalLinks, CCalculationAlgorithmContextBase* calcAlgContext);

						bool detectIndividualNodeBlockedStatus(CIndividualProcessNode*& testIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAncestorBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAnywhereBlockingIndividualNode(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAnywhereBlockingIndividualNodeCanidateHashed(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAnywhereBlockingIndividualNodeLinkedCanidateHashed(CIndividualProcessNode* blockingTestIndi, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						void clearBlockingCache(CCalculationAlgorithmContextBase* calcAlgContext);


						bool testAlternativeBlocked(CIndividualProcessNode*& testIndi, CBlockingAlternativeData* blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isIndividualNodeProcessingBlocked(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isIndividualNodeExpansionBlocked(CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool needsIndividualNodeExpansionBlockingTest(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* blockingTestIndi, CCalculationAlgorithmContextBase* calcAlgContext);


						CIndividualProcessNode* getDelayProcessingBlockingNominalNode(CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool tryDelayNominalProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool canDelayNominalProcessing(CConceptProcessDescriptor* conProDes, CIndividualProcessNode* testIndi, CCalculationAlgorithmContextBase* calcAlgContext);



						bool signatureCachedIndividualNodeBlock(CIndividualProcessNode*& indi, CIndividualNodeBlockingTestData* blockData, CIndividualProcessNode*& blockerIndiNode, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool continueIndividualNodeBlock(CIndividualProcessNode*& indi, CIndividualNodeBlockingTestData* blockData, CIndividualProcessNode*& blockerIndiNode, CBlockingAlternativeData** blockAltData, CCalculationAlgorithmContextBase* calcAlgContext);
						void reactivateIndirectBlockedSuccessors(CIndividualProcessNode*& indi, bool recursive, CCalculationAlgorithmContextBase* calcAlgContext);

						bool reactivateBlockedIndividuals(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);



						void propagateIndividualProcessedAndReactivate(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void searchReactivateIndividualsProcessedPropagated(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateIndividualUnprocessed(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndividualUnprocessed(CIndividualProcessNode*& indi, bool requiresConsFlag, CCalculationAlgorithmContextBase* calcAlgContext);

						CCalculationAlgorithmContextBase* createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask);


						void createIndividualNodeDisjointRolesLinks(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CSortedNegLinker<CRole*>* disjointRoleLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void createIndividualNodeNegationLink(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CRole* negationRole, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool areIndividualNodesDisjointRolesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isIndividualNodeDisjointRolesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);


						void propagateIndirectSuccessorSaturationBlocked(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool validateSaturationCachingPossible(CIndividualProcessNode* indi, CIndividualSaturationProcessNode*& saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CConcept* addedConcept, bool addedConceptNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool tryExpansionFromSaturatedData(CIndividualProcessNode*& indi, CIndividualProcessNode* createdSuccIndi, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CIndividualSaturationProcessNode*& saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CCalculationAlgorithmContextBase* calcAlgContext);
						bool tryEstablishSaturationCaching(CIndividualProcessNode*& indi, CIndividualProcessNode* succIndi, CIndividualSaturationProcessNode* saturationIndiNode, bool* satCachingPossible, CConceptDescriptor** lastSatCachPossibleConDes, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getLocalizedSuccessorIndividual(CIndividualProcessNode*& indi, CIndividualLinkEdge* link, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* createSuccessorIndividual(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* tryExtendFunctionalSuccessorIndividual(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void createDistinctSuccessorIndividuals(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CPROCESSINGLIST<CIndividualProcessNode*>& indiList, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, cint64 succCardCount, CCalculationAlgorithmContextBase* calcAlgContext);
						void createNominalsSuccessorIndividuals(CIndividualProcessNode*& indi, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CSortedNegLinker<CConcept*>* conceptLinkerIt, bool negate, CDependencyTrackPoint* depTrackPoint, cint64 succCardCount, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualSaturationProcessNode* getCreationSuccessorSaturationNode(CIndividualProcessNode*& indi, CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualSaturationProcessNode* getSaturationResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CPROCESSINGHASH<cint64, CConceptNegationPair>* conExtensionMap, CCalculationAlgorithmContextBase* calcAlgContext);
						void collectReapplyAutomatTransactionsRestrictions(CIndividualProcessNode*& processIndi, CRole* collectingRole, CConcept* concept, bool negated, CPROCESSINGHASH<cint64, CConceptNegationPair>*& conExtensionMap, CReapplyConceptSaturationLabelSet* conSet, CCalculationAlgorithmContextBase* calcAlgContext);



						bool tryInitalizingFromSaturatedData(CIndividualProcessNode*& indi, CXSortedNegLinker<CConcept*>* initConceptLinker, CDependencyTrackPoint* depTrackPoint, bool allowPreprocess, CCalculationAlgorithmContextBase* calcAlgContext);











						bool hasNondeterministicDependency(CDependencyTrackPoint* dependencyTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);










						bool prepareBackendExpansionReuseBranching(CCalculationAlgorithmContextBase* calcAlgContext);
						bool prepareBackendIndividualFixedReuseExpansion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool prepareBackendIndividualPrioritizedReuseExpansion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool reuseIndividualBackendExpansion(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool checkIndividualBackendExpansionReuseable(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						
						
						CIndividualProcessNode* queuedIndividualBackendNeighbourExpansion(CIndividualProcessNode*& baseIndiNode, CBackendNeighbourExpansionControllingData* expContData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool canDelayRepresentativeNeighbourExpansion(CIndividualProcessNode* expIndiNode, CBackendNeighbourExpansionQueueDataLinker* backendNeighbourExpDataLinker, CPROCESSHASH<CBackendRepresentativeMemoryLabelCacheItem *, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData>* labelNeighbourExpDelayDataHash, CBackendRepresentativeMemoryLabelCacheItem* expandingLabel, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, cint64 arrayPos, cint64 lastCursor, cint64 neighbourIndiId, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData*& labelNeighbourExpansionData, bool& expansionDelaying, bool& representativeExpansion, CCalculationAlgorithmContextBase* calcAlgContext);
						bool delayingRepresentativeNeighbourExpansion(CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, bool expansionDelaying, bool representativeExpansion, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData* labelNeighbourExpansionData, cint64 lastCursor, cint64 neighbourIndiId, CCalculationAlgorithmContextBase* calcAlgContext);

						bool markIndividualNodeBackendNonConceptSetNeighbourLabelRelatedProcessingForDisjointRoles(CIndividualProcessNode* indiNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markIndividualNodeBackendNonConceptSetNeighbourLabelRelatedProcessing(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markIndividualNodeBackendNonConceptSetRelatedAndNeighbourLabelRelatedProcessing(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markIndividualNodeBackendNonConceptSetRelatedAndNeighbourLabelRelatedProcessingForDisjointRoles(CIndividualProcessNode* indiNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);


						CIndividualProcessNode* getLocalizedForcedBackendInitializedNominalIndividualNode(cint64 nominalId, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getLocalizedForcedBackendInitializedNominalIndividualNode(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool loadIndividualNodeDataFromBackendCache(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool initializeIndividualNodeWithBackendCache(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool tryEstablishExpansionBlockingWithBackendCacheSynchronisation(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool tryDelayIndividualNodeInitializationWithBackendConceptSetLabel(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markIndividualNodeBackendNonConceptSetRelatedProcessing(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markIndividualNodeBackendNonConceptSetRelatedProcessingForDisjointRoles(CIndividualProcessNode* indiNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher getIndividualRepresentativeBackendCacheConceptSetLabelProcessingHasher(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool visitIndividualsRelevantMergingsBackendSynchronisationDataIndividuals(CIndividualProcessNode* indiNode, CXLinker<cint64>* mergedIndiLinker, CXLinker<cint64>* lastProcessedMergedIndiLinker, bool localize, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CDependencyTrackPoint* backSyncDepTrackPoint)> visitFunc, CCalculationAlgorithmContextBase* calcAlgContext);
						bool visitIndividualsRelevantBackendSynchronisationDataIndividuals(CIndividualProcessNode* indiNode, bool localize, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CDependencyTrackPoint* backSyncDepTrackPoint)> visitFunc, CCalculationAlgorithmContextBase* calcAlgContext);
						bool visitNewlyMergedIndividualsBackendSynchronisationData(CIndividualProcessNode* indiNode, CPROCESSHASH<CIndividualProcessNode*, CDependencyTrackPoint*>* newIndiMergedHash, bool visitBaseIndividual, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CDependencyTrackPoint* backSyncDepTrackPoint)> visitFunc, CCalculationAlgorithmContextBase* calcAlgContext);
						bool visitNewlyMergedIndividualsBackendSynchronisationData(CIndividualProcessNode* indiNode, CXLinker<CIndividualProcessNode*>* newIndiMergedLinker, CXLinker<CIndividualProcessNode*>* prevIndiMergedLinker, bool visitBaseIndividual, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CDependencyTrackPoint* backSyncDepTrackPoint)> visitFunc, CCalculationAlgorithmContextBase* calcAlgContext);
						bool visitNewlyMergedOnlyDeterministicRepresentativeIndividualsBackendSynchronisationData(CIndividualProcessNode* indiNode, CXLinker<CIndividualProcessNode*>* newIndiMergedLinker, CXLinker<CIndividualProcessNode*>* prevIndiMergedLinker, bool visitBaseIndividual, function<bool(CIndividualProcessNode* baseIndiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CDependencyTrackPoint* backSyncDepTrackPoint)> visitFunc, CCalculationAlgorithmContextBase* calcAlgContext);

						bool validateBackendSynchronisationContinued(CIndividualProcessNode* indi, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backendSyncData, CConcept* addedConcept, bool addedConceptNegation, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testIndividualNodeBackendCacheConceptsSynchronization(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testIndividualNodeBackendCacheNeighbourExpansionBlockingCritical(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testIndividualNodeBackendCacheSameMergedBlockingCritical(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool testIndividualNodeBackendCacheNewMergings(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testIndividualNodeConceptBackendCacheNeighbourExpansionBlockingCritical(CConcept* concept, bool conNegation, bool nondeterministic, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool debugCheckDirectlyInfluencedNeighbourWithPropagationPossible(CConcept* concept, bool conNegation, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool testIndividualNodeBackendCacheExpansionBlockingCriticalCardinality(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testIndividualNodeBackendCacheNominalIndirectConnectionBlockingCritical(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						cint64 getBackendCacheRoleRepresentativeNeighbourCount(CIndividualProcessNode* indiNode, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* backendSyncData, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);

						bool expandDirectlyInfluencedIndividualNeighbourNodesFromBackendCache(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);

						bool expandDirectlyInfluencedNeighboursWithPropagation(CConcept* concept, bool conNegation, bool nondeterministic, CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, CIndividualProcessNode* locBackendSyncDataIndiNode, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CDependencyTrackPoint* backSyncDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						CAnsweringPropagationSteeringController* getPropagationSteeringController(CCalculationAlgorithmContextBase* calcAlgContext);
						bool isNeighbourExpansionWithPropagationAllowed(CIndividualProcessNode* indiNode, CConcept* concept, bool conNegation, cint64 neighbourIndiId, CCalculationAlgorithmContextBase* calcAlgContext);
						bool canExpandDirectlyInfluencedNeighbourWithPropagation(CIndividualProcessNode* indiNode, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CDependencyTrackPoint* backSyncDepTrackPoint, CConcept* concept, bool conNegation, bool nondeterministic, CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData& neighbourExpansionData, cint64 neighbourIndiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool canExpansionPotentiallyInfluenceNeighbourWithPotentialPropagation(CIndividualProcessNode* indiNode, CConcept* concept, bool conNegation, bool nondeterministic, CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, cint64 neighbourIndiId, CCalculationAlgorithmContextBase* calcAlgContext);

						bool expandIndirectlyConnectedIndividuals(CIndividualProcessNode* indiNode, bool checkExpansionRequired, CCalculationAlgorithmContextBase* calcAlgContext);

						bool expandIndirectCompatibleRequiredIndividualNeighbourNodesFromBackendCache(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandIndirectCompatibleRequiredIndividualNeighbourNodesFromBackendCache(CIndividualProcessNode* indi, CIndividualProcessNode* checkingBackendSyncDataIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						
						CPROCESSHASH< QPair<CRole*, bool>, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationRoleNeighbourExpansionData >* getBackendSynchronizationFilledRoleNeighbourExpansionDataHash(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandIndividualInferringNeighboursFromBackendCache(CIndividualProcessNode* indiNode, CIndividualProcessNode* backendSyncDataIndiNode, bool forceExpansion, CDependencyTrackPoint* backSyncDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandIndividualAllNeighboursFromBackendCache(CIndividualProcessNode* indiNode, CIndividualProcessNode* backendSyncDataIndiNode, bool forceExpansion, bool nonDeterministicConsequencesMissingExpansion, CDependencyTrackPoint* backSyncDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandIndividualNeighbourNodeFromBackendCache(CIndividualProcessNode* indiNode, CBackendRepresentativeMemoryCacheIndividualAssociationData* assocData, cint64 neighbourIndiId, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationNeighbourExpansionData& neighbourExpansionData, bool forceExpansion, bool forceProcessing, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationLabelNeighbourExpansionData** representativeLabelExpansionData, CDependencyTrackPoint* baseDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandIndividualNeighbourNodeFromBackendCache(CIndividualProcessNode* indiNode, cint64 neighbourIndiId, CCalculationAlgorithmContextBase* calcAlgContext);

						bool initializeNeighbourExpansionWithPropagation(CIndividualProcessNode* indiNode, CIndividualProcessNode* locBackendSyncDataIndiNode, CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* locBackendSyncData, CDependencyTrackPoint* backSyncDepTrackPoint, cint64 arrayId, CConcept* concept, bool conNegation, bool nondeterministic, CRole* role, bool forceExpansion, bool propCutExpansion, bool nonDeterministicConsequencesMissingExpansion, CCalculationAlgorithmContextBase* calcAlgContext);
						bool ensureBaseLinkExpansion(CIndividualProcessNode* expIndiNode, CIndividualProcessNode* indiNode, cint64 neighbourNodeId, CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualNodeRepresentativeMemoryBackendCacheSynchronisationData* getLocalizedIndividualBackendCacheSnychronisationData(CIndividualProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);


						void installIndividualNodeRoleLink(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CIndividualLinkEdge* individualLink, CCalculationAlgorithmContextBase* calcAlgContext);
						CReapplyQueueIterator installIndividualNodeRoleLinkReapplied(CIndividualProcessNode*& sourceIndi, CIndividualProcessNode*& destinationIndi, CIndividualLinkEdge* individualLink, CCalculationAlgorithmContextBase* calcAlgContext);


						CIndividualProcessNode* createNewEmptyIndividual(CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualProcessNode* createNewIndividual(CDependencyTrackPoint* depTrackPoint, bool dataNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualLinkEdge* createNewIndividualsLink(CIndividualProcessNode*& indiCreator, CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* ancRole, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualLinkEdge* createNewIndividualsLinkReapplyed(CIndividualProcessNode*& indiCreator, CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualLinkEdge* createNewIndividualsLinks(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualLinkEdge* createNewIndividualsLinksReapplyed(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CSortedNegLinker<CRole*>* roleLinkerIt, CRole* ancRole, CDependencyTrackPoint* depTrackPoint, bool checkRoleExisting, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAncestorIndividual(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasIndividualsLink(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, bool locateable, CCalculationAlgorithmContextBase* calcAlgContext);

						void createIndividualsDistinct(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void createIndividualsDistinct(CPROCESSINGLIST<CIndividualProcessNode*>& indiList, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualProcessNode* getLocalizedIndividual(CIndividualProcessNode* indi, bool updateIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getLocalizedIndividual(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getUpToDateIndividual(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getUpToDateIndividual(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getAvailableUpToDateIndividual(cint64 indiID, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividual* createNewTemporaryNominalIndividual(cint64 indiId, CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualLinkEdge* getIndividualNodeLink(CIndividualProcessNode*& indiSource, CIndividualProcessNode*& indiDestination, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualLinkEdge* getLinkProcessingRestriction(CConceptProcessDescriptor*& conProDes);


						void propagateIndividualNodeNewNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndividualNodeNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndividualNodeNeighboursNominalConnectionToAncestors(CIndividualProcessNode*& indi, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void propagateIndividualNodeNominalConnectionFlagsToAncestors(CIndividualProcessNode*& indi, cint64 nominalPropagationFlags, CCalculationAlgorithmContextBase* calcAlgContext);

						void propagateIndividualNodeConnectedNominalToAncestors(CIndividualProcessNode*& indi, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndividualNodeNominalConnectionStatusToAncestors(CIndividualProcessNode*& indi, CIndividualProcessNode* copyFromIndiNode, CCalculationAlgorithmContextBase* calcAlgContext);


						bool expandBackendCacheIndividualNodesNominalMerging(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CCalculationAlgorithmContextBase* calcAlgContext);
						bool expandBackendCacheIndividualNodesNominalMergingNeighbouringConnections(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CCalculationAlgorithmContextBase* calcAlgContext);

						bool isIndividualNodesMergeable(CIndividualProcessNode* indi1, CIndividualProcessNode* indi2, CClashedDependencyDescriptor*& clashDescriptors, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isIndividualNodesMergeableWithoutNewRuleApplications(CIndividualProcessNode* mergeIntoIndi1, CIndividualProcessNode* indi2, bool* mergingPossiblyRequiresRuleApplications, bool cancelOnPossiblyNewRuleApplications, CCalculationAlgorithmContextBase* calcAlgContext);


						CIndividualProcessNode* getMergedIndividualNodes(CIndividualProcessNode*& preferedMergeIntoIndividualNode, CIndividualProcessNode*& individual2, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						void mergeIndividualNodeInto(CIndividualProcessNode*& mergeIntoIndividualNode, CIndividualProcessNode*& individual, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						CIndividualProcessNode* getIntoEmptyMergedIndividualNode(CIndividualProcessNode*& mergingIndividualNode, bool createAsNominal, CIndividualProcessNode* mergerNode, CDependencyTrackPoint* mergeDepTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);



						QString writeGeneratedExtendedDebugIndiModelStringList(const QString& filename, CCalculationAlgorithmContextBase* calcAlgContext);

						QString generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, QStringList* list = nullptr);
						QString generateDebugIndiStatusString(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugDependentNominalsString(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateDebugIncrementalExpansionString(CIndividualProcessNode* indi, CCalculationAlgorithmContextBase* calcAlgContext);

						QString generateDebugDependencyString(CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);
						QStringList generateExtendedDebugConceptSetStringList(CReapplyConceptLabelSet* conSet, CConceptPropagationBindingSetHash* propBindSetHash, CConceptVariableBindingPathSetHash* varBindPathSetHash, CCalculationAlgorithmContextBase* calcAlgContext);
						void debugTestCriticalConceptSet(QStringList& conSetList, CCalculationAlgorithmContextBase* calcAlgContext);


						void analyzeABoxCompressionPossibilities(CCalculationAlgorithmContextBase* calcAlgContext);
						void analyzeBranchingMemoryWasting(CCalculationAlgorithmContextBase* calcAlgContext);

						void analyzeCompletionGraphStatistics(CCalculationAlgorithmContextBase* calcAlgContext);
						void writeCompletionGraphStatistics(CCalculationAlgorithmContextBase* calcAlgContext);
						bool isConceptFromPredecessorDependent(CIndividualProcessNode*& individualNode, CConceptDescriptor* conDes, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContextBase* calcAlgContext);

						void testCompletionGraphCachingAndBlocking(CCalculationAlgorithmContextBase* calcAlgContext, CIndividualProcessNode* exceptIndividualNode = nullptr);
						void testProblematicConceptSet(CCalculationAlgorithmContextBase* calcAlgContext);



						CConceptDescriptor* createConceptDescriptor(CCalculationAlgorithmContextBase* calcAlgContext);
						void releaseConceptDescriptor(CConceptDescriptor* conDes, CCalculationAlgorithmContextBase* calcAlgContext);



					// protected variables
					protected:
						CCalculationAlgorithmContextBase* mCalcAlgContext;
						CProcessingDataBox* mProcessingDataBox;

						CIndividualProcessingQueue* mProcessingQueue;
						CIndividualDepthProcessingQueue* mNominalProcessingQueue;
						CIndividualDepthProcessingQueue* mNominalDeterministicProcessingQueue;

						CIndividualUnsortedProcessingQueue* mDepthFirstProcessingQueue;
						CIndividualUnsortedProcessingQueue* mNominalCachingLossReactivationProcessingQueue;
						CSignatureBlockingReviewSet* mSigBlockRevSet;
						CReusingReviewData* mReusingReviewData;
						CIndividualUnsortedProcessingQueue* mIndiImmediateProQueue;
						CIndividualDepthProcessingQueue* mIndiDetExpProQueue;
						CIndividualUnsortedProcessingQueue* mIndiDetDeptFirstExpProQueue;
						CIndividualDepthProcessingQueue* mIndiBlockReactProQueue;
						CIndividualDepthProcessingQueue* mIndiSigBlockUpdProQueue;
						CIndividualDepthProcessingQueue* mDepthProcessingQueue;
						CIndividualReactivationProcessingQueue* mEarlyIndiReactProcessingQueue;
						CIndividualReactivationProcessingQueue* mLateIndiReactProcessingQueue;
						CIndividualConceptBatchProcessingQueue* mVarBindConBatchProcessingQueue;
						CIndividualUnsortedProcessingQueue* mDelayedNominalProcessingQueue;
						CIndividualUnsortedProcessingQueue* mRoleAssertionProcessingQueue;
						CIndividualDepthProcessingQueue* mValueSpaceTriggeringProQueue;
						CIndividualDepthProcessingQueue* mValueSpaceSatCheckingQueue;
						CIndividualUnsortedProcessingQueue* mBackendSyncRetestProcessingQueue;
						CIndividualUnsortedProcessingQueue* mBackendDirectInfluenceExpansionQueue;
						CIndividualUnsortedProcessingQueue* mBackendIndirectCompatibilityExpansionQueue;
						CIndividualLinkerRotationProcessingQueue* mBackendNeighbourExpansionQueue;
						CIndividualUnsortedProcessingQueue* mBackendLateNeighbourExpansionQueue;
						CIndividualUnsortedProcessingQueue* mBackendReuseExpansionQueue;
						CIndividualDepthProcessingQueue* mIncrementalExpansionInitializingProcessingQueue;
						CIndividualCustomPriorityProcessingQueue* mIncrementalExpansionProcessingQueue;
						CIndividualDepthProcessingQueue* mIncrementalCompatibilityCheckingQueue;

						enum INDINODEQUEUETYPE {
							INQT_NONE, INQT_CACHETEST, INQT_IMMEDIATE, INQT_DELAYEDBACKENDINIT, INQT_ROLEASS, INQT_BACKENDSYNCRETEST, INQT_BACKENDDIRECTINFLUENCEEXPANSION, INQT_BACKENDINDIRECTCOMPATIBILITYEXPANSION, INQT_DETEXP, INQT_VSTSATTESTING, INQT_VSTRIGGERING, INQT_BLOCKREACT, INQT_COMPCACHEDREACT, INQT_BLOCKUP, INQT_DEPTHNORMAL, INQT_NOMINAL, INQT_NOMINALCACHINGLOSSREACTIVATION, INQT_DEPTHFIRST, INQT_OUTDATED, INQT_VARBINDBATCHQUE, INQT_DELAYEDNOMINAL, INQT_BACKENDEXPANSIONREUSE
						};
						INDINODEQUEUETYPE mIndiNodeFromQueueType;

						double mMinConceptProcessingPriorityLevel;
						bool mIndiNodeConcludeUnsatCaching;

						cint64 mCurrentRecProcDepth;
						cint64 mCurrentRecProcDepthLimit;


						CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy* mIndiAncDepthMasConProcPriStr;

						CConceptProcessingPriorityStrategy* mConceptPriorityStrategy;
						CIndividualProcessingPriorityStrategy* mIndividualPriorityStrategy;
						CTaskProcessingPriorityStrategy* mTaskProcessingStrategy;

						CSatisfiableTaskConsistencyPreyingAnalyser mSatTaskConsAnalyser;
						CSatisfiableTaskIncrementalConsistencyPreyingAnalyser mSatTaskIncConsAnalyser;
						CSatisfiableTaskClassificationMessageAnalyser mClassMessAnalyser;
						CSatisfiableTaskMarkerIndividualPropagationAnalyser mMarkerPropRealMessAnalyser;
						CSatisfiableTaskPossibleAssertionCollectingAnalyser mPossAssCollAnalyser;
						CSatisfiableTaskPropertyClassificationMessageAnalyser mSatTaskPropClassAnalyser;
						CSatisfiableTaskComplexAnsweringMessageAnalyser mSatTaskCompAnswerAnalyser;
						CSatisfiableTaskPropagationBindingAnsweringMessageAnalyser mSatTaskPropBindingAnswerAnalyser;


						CClashDescriptorFactory* mClashDesFactory;
						CDependencyFactory* mDependencyFactory;
						CIndividualNodeManager* mIndiNodeManager;
						CUnsatisfiableCacheHandler* mUnsatCacheHandler;
						CSatisfiableExpanderCacheHandler* mSatExpCacheHandler;
						CCompletionGraphCacheHandler* mCompGraphCacheHandler;
						CReuseCompletionGraphCacheHandler* mReuseCompGraphCacheHandler;
						CConceptNominalSchemaGroundingHandler* mGroundingHandler;
						CUnsatisfiableCacheRetrievalStrategy* mUnsatCachRetStrategy;
						CSaturationNodeExpansionCacheHandler* mSatNodeExpCacheHandler;
						CDatatypeIndividualProcessNodeHandler* mDatatypeHandler;
						CComputedConsequencesCacheHandler* mCompConsCacheHandler;
						CIndividualNodeBackendCacheHandler* mBackendCacheHandler;
						CIncrementalCompletionGraphCompatibleExpansionHandler* mIncExpHandler;
						COccurrenceStatisticsCacheHandler* mOccStatsCacheHandler;

						CIndividualProcessNode* mLastUnsatCacheTestedIndiNode;
						cint64 mLastUnsatCacheTestedIndiNodeConceptSetSize;

						const static cint64 mRuleFuncCount = 200;
						TableauRuleFunction* mPosJumpFuncVec;
						TableauRuleFunction* mNegJumpFuncVec;

						TableauRuleFunction mPosTableauRuleJumpFuncVec[mRuleFuncCount];
						TableauRuleFunction mNegTableauRuleJumpFuncVec[mRuleFuncCount];

						bool mOptIncrementalExpansion;
						bool mOptIncrementalCompatibleExpansion;
						bool mOptIncrementalCachingExpansion;

						bool mOptIncrementalDeterministicExpansion;
						bool mOptIncrementalNondeterministicExpansion;

						bool mConfAncestorBlockingSearch;
						bool mConfAnywhereBlockingSearch;
						bool mConfSaveCoreBlockingConceptsCandidates;
						bool mConfAnywhereBlockingLinkedCandidateHashSearch;
						bool mConfAnywhereBlockingCandidateHashSearch;
						bool mConfAnywhereBlockingLazyExactHashing;
						bool mConfAnywhereBlockingSomeInitializationHashing;
						bool mConfSubSetBlocking;
						bool mConfOptimizedSubSetBlocking;
						bool mConfEqualSetBlocking;
						bool mConfPairwiseEqualSetBlocking;
						bool mConfSpecializedAutomateRules;
						bool mConfSemanticBranching;
						bool mConfAtomicSemanticBranching;
						bool mConfBranchTriggering;
						bool mConfStrictIndiNodeProcessing;
						bool mConfIDIndiPriorization;
						bool mConfPropagateNodeProcessed;
						bool mConfDirectRulePreprocessing;
						bool mConfLazyNewNominalGeneration;
						bool mConfConsRestrictedNonStrictIndiNodeProcessing;
						bool mConfUniqueNameAssumption;
						bool mConfPairwiseMerging;
						bool mConfRepresentativePropagationRules;


						bool mConfBuildAllBranchingNodes;
						bool mConfBuildDependencies;
						bool mConfDependencyBacktracking;
						bool mConfDependencyBackjumping;

						bool mConfWriteUnsatCaching;
						bool mConfTestOccurUnsatCached;
						bool mConfTestPrecheckUnsatCached;
						bool mConfMinimizeMerging;
						bool mConfTestedConceptWriteUnsatCaching;

						bool mConfLateBlockingResolving;


						bool mConfSatExpCacheRetrieval;
						bool mConfSatExpCacheConceptExpansion;
						bool mConfSatExpCacheSatisfiableBlocking;
						bool mConfSatExpCacheWriting;

						bool mConfSignatureMirroringBlocking;
						bool mConfSignatureSaving;

						bool mOptSignatureMirroringBlockingForceSubset;
						bool mOptSignatureMirroringBlockingInBlocking;

						bool mConfIndividualReusingFromSignatureBlocking;


						bool mConfSatExpCachedDisjAbsorp;
						bool mConfSatExpCachedMergAbsorp;
						bool mConfSatExpCachedSuccAbsorp;

						bool mConfUnsatCachingUseFullNodeDependency;
						bool mConfUnsatCachingUseNodeSignatureSet;

						bool mConfSkipANDConcepts;

						bool mConfCompletionGraphCaching;
						bool mConfIgnoreBlockingCompletionGraphCachedNonBlockingNodes;

						bool mConfDelayCompletionGraphCachingReactivation;
						bool mConfIndividualsBackendCacheLoading;

						bool mConfDepthOrientatedProcessing;
						bool mConfCurrentIndividualQueuing;

						bool mOptProcessedNodePropagation;
						bool mOptProcessedConsNodePropagation;
						bool mOptConsistenceNodeMarking;

						bool mOptProcessingBlockingTests;
						bool mOptConsNodeProcessingBlockingTests;
						bool mOptNonStrictIndiNodeProcessing;
						bool mOptDetExpPreporcessing;

						bool mConfUnsatBranchSatisfiableCaching;
						bool mConfAtleastAtmostFastClashCheck;


						bool mConfCompGraphReuseCacheRetrieval;
						bool mConfCompGraphDeterministicReuse;
						bool mConfCompGraphNonDeterministicReuse;

						bool mConfSaturationCachingWithNominals;
						bool mConfExactNominalDependencyTracking;
						bool mConfConceptUnsatisfiabilitySaturatedTesting;
						bool mConfSaturationSatisfiabilitiyExpansionCacheWriting;
						bool mConfSaturationExpansionCacheReading;
						bool mConfSaturationCachingTestingDuringBlockingTests;
						bool mConfSaturationConceptUnsatisfiabilitySaturatedCacheWriting;
						bool mConfSaturationIncompleteExpansionFromCache;


						bool mConfCollectCachingUpdatedBlockableIndiNodes;


						bool mConfDatatypeReasoning;


						bool mConfCacheComputedConsequences;
						bool mConfAddCachedComputedConsequences;


						bool mConfAnalogousPropagationPathBlockingWithAnsweringPropagationAdapters;
						bool mOptAnalogousPropagationPathBlocking;


						bool mConfDelayedBackendInitializiation;
						bool mOptDelayedBackendInitializiation;
						bool mOptDelayedBackendInitializiationWithRootLinkers;




						bool mConfAllowBackendSuccessorExpansionBlocking;
						bool mConfAllowBackendNeighbourExpansionBlocking;
						bool mConfOnlyDeterministicRepresentativeBackendIndividualDataConsideration;


						bool mConfOccurrenceStatisticsCollecting;
						bool mOptCollectOccurrenceStatistics;


						bool mConfDebuggingWriteData;
						bool mConfDebuggingWriteDataComplationTasks;
						bool mConfDebuggingWriteDataOnlyOnSatisfiability;
						bool mConfDebuggingWriteDataForConsistencyTests;
						bool mConfDebuggingWriteDataForClassificationTests;
						bool mConfDebuggingWriteDataForIncrementalExpansionTests;
						bool mConfDebuggingWriteDataForRepCacheIndiComputationTests;
						bool mConfDebuggingWriteDataForAnsweringPropagationTests;
						bool mConfDebuggingWriteDataForAllTests;
						bool mDebug = false;
						bool mBacktrackDebug;

						bool mConfBranchingStatisticsAnalysing;
						CBranchTreeNode* mLastAnalysingBranchNodeTree;




						bool mConfExpandCreatedSuccessorsFromSaturation;
						bool mConfSuccessorSaturationExpansionRestrictionsResolving;
						bool mConfCachingBlockingFromSaturation;

						bool mConfMergeConstructedIndividualNode;
						bool mOptMergeConstructedIndividualNode;






						bool mConfVariableBindingSteeringBackendNeighbourExpansion;
						bool mConfBackendExpansionReuse;
						bool mConfBackendExpansionLimitReachedReuseActivation;
						bool mConfBackendExpansionLateDynamicReuseActivation;
						cint64 mConfBackendExpansionSameIndividualCountReuseActivation;
						cint64 mConfBackendExpansionNeighbourIndividualCountReuseActivation;
						bool mConfLimitBackendNeighbourExpansion;
						bool mConfAllProblematicBackendNeighbourDirectExpansion;
						cint64 mConfMinBackendNeighbourDirectExpansionCount;
						cint64 mOptMinBackendNeighbourDirectExpansionCount;

						cint64 mConfMinDirectNeighbourExpansionOverCriticalReductionSize;
						cint64 mOptMinDirectNeighbourExpansionOverCriticalReductionSize;


						cint64 mConfMaxBackendNeighbourTotalExpansionCount;
						cint64 mOptMaxBackendNeighbourTotalExpansionCount;
						cint64 mConfCriticalBackendNeighbourTotalExpansionCount;
						cint64 mOptCriticalBackendNeighbourTotalExpansionCount;
						cint64 mConfQueuedBackendNeighbourExpansionIndisBatchSize;
						cint64 mOptQueuedBackendNeighbourExpansionIndisBatchSize;
						cint64 mConfQueuedBackendNeighbourExpansionRolesBatchCount;
						cint64 mOptQueuedBackendNeighbourExpansionRolesBatchCount;

						bool mConfAtmostAllDirectBackendNeighbourExpansion;
						cint64 mConfDefaultIndividualPrecomputationCount;

						bool mConfNeighbourLabelRepresentativeExpansionDelaying;
						bool mOptNeighbourLabelRepresentativeExpansionDelaying;

						bool mOptLimitBackendNeighbourExpansion;
						bool mOptBackendExpansionReuse;

						bool mConfNewMergingsOnlyInferringExpansion;

						bool mConfExpandDeterministicMergedHandledNeighbours = true;
						bool mConfCardinalityNeighbourExpansionRepresentativeCounting = false;





						bool mConfGenerateQueries;



						cint64 mMaxBlockingCachingSavedCandidateCount;

						cint64 mMapComparisonDirectLookupFactor;


						CCalculationConfigurationExtension *mLastConfig;


						QSet<cint64> mUnsatCachingSignatureSet;


						cint64 mProcessRuleToTaskProcessingVerificationCount;
						cint64 mRemainProcessRuleToTaskProcessingVerification;




						cint64 mStatRepresentativeExpansionTryingNeighbourIndividualCount = 0;
						cint64 mStatRepresentativeExpandedNeighbourIndividualCount = 0;
						cint64 mStatRepresentativeExpansionAlreadyExistingNeighbourIndividualCount = 0;
						cint64 mStatRepresentativeDelayedNeighbourIndividualExpansionCount = 0;



						bool mConfPossibleInstanceIndividualsMerging;



						static const cint64 mDeterministicProcessPriority = 4;
						static const cint64 mImmediatelyProcessPriority = 8;

						// for debugging
						QHash<cint64,cint64> mIndiNodeInitConceptSigCountHash;
						QHash<cint64,cint64> mClosedBranchLevelCountHash;
						QHash<cint64,cint64> mSignatureIndiNodeStatusHash;
						QHash<cint64,cint64> mSignatureIndiNodePredDepHash;
						QMap<cint64,cint64> mIndiNodeCountMap;
						QList<cint64> mIndiNodeCountList;

						QSet<QString> mCriticalConceptSetStringSet;
						bool mFoundCriticalConceptSet;

						static const cint64 mDebugTaskIDVectorSize = 100;
						cint64 mDebugTaskIDVector[mDebugTaskIDVectorSize];
						cint64 mBacktrackingStep;

						TableauRuleFunction mLastJumpFunc;

						CBranchingMergingProcessingRestrictionSpecification* lastBranchingMergingProcRest;

						QStringList mDebugIndiModelStringList;
						QString mDebugIndiModelString;

						QString mBeginTaskDebugIndiModelString;
						QString mBeforeRuleDebugIndiModelString;
						QString mAfterRuleDebugIndiModelString;
						QString mClashedDebugIndiModelString;
						QString mEndTaskDebugIndiModelString;
						QString mIncExpCompIndiModelString;
						QString mIncExpMergedIndiModelString;

						QString mBeforeMergingTaskDebugIndiModelString;
						QString mAfterMergingTaskDebugIndiModelString;


						QStringList mMergedStringList;


						QString mSatTaskDebugIndiModelString;
						QString mBeforeRuleTaskDebugIndiModelString;


						QString mBeginBacktrackingClashString;
						QString mBeginBacktrackingTracklineString;
						QString mEndBacktrackingTracklineString;

						QString mFileBacktrackingStepTracklineString;
						QString mBeginBacktrackingStepTracklineString;
						QString mEndBacktrackingStepTracklineString;

						QString mBeginDetPrevBacktrackingStepTracklineString;
						QString mEndDetPrevBacktrackingStepTracklineString;
						QString mBeginNonDetPrevBacktrackingStepTracklineString;
						QString mEndNonDetPrevBacktrackingStepTracklineString;


						QString mNonDetDependencyTrackPointReasonString;
						QString mNonDetDependencyBeforeProcessedTrackedString;
						QString mNonDetDependencyCollectedTrackedString;

						QString mMergingClashString;
						QString mCachingClashString;
						QString mSortedCachingClashString;


						QString mMergingQueueString;

						QString mBranchLevelClosedCountString;
						QString mBeforeGroundingDebugIndiModelString;
						QString mAfterGroundingDebugIndiModelString;



						QString mAnalogousPropagationBlockingTestingIndiAssociatedConceptsString;
						QString mAnalogousPropagationBlockingBlockingIndiAssociatedConceptsString;

						QString mAnalogousPropagationBlockingTestingIndiAllAssociatedConceptsString;
						QString mAnalogousPropagationBlockingBlockingIndiAllAssociatedConceptsString;



						cint64 mAppliedALLRuleCount;
						cint64 mAppliedSOMERuleCount;
						cint64 mAppliedANDRuleCount;
						cint64 mAppliedORRuleCount;
						cint64 mAppliedATLEASTRuleCount;
						cint64 mAppliedATMOSTRuleCount;
						cint64 mAppliedTOTALRuleCount;


						cint64 mStatVarBindingCreatedCount;
						cint64 mStatVarBindingGroundingCount;
						cint64 mStatVarBindingImplicationCount;
						cint64 mStatVarBindingJoinCombinesCount;
						cint64 mStatVarBindingPropagateSuccCount;
						cint64 mStatVarBindingPropagateSuccFreshCount;
						cint64 mStatVarBindingPropagateSuccInitialCount;
						cint64 mStatVarBindingPropagateCount;
						cint64 mStatVarBindingPropagateFreshCount;
						cint64 mStatVarBindingPropagateInitialCount;


						cint64 mStatRepresentativeCreatedCount;
						cint64 mStatRepresentativeGroundingCount;
						cint64 mStatRepresentativeImplicationCount;
						cint64 mStatRepresentativeJoinCombinesCount;
						cint64 mStatRepresentativeJoinCount;
						cint64 mStatRepresentativeJoinedCount;
						cint64 mStatRepresentativeJoinQuickFailCount;
						cint64 mStatRepresentativePropagateSuccCount;
						cint64 mStatRepresentativePropagateCount;
						cint64 mStatRepresentativePropagateNewRepresentativeCount;
						cint64 mStatRepresentativePropagateReusedRepresentativeCount;
						cint64 mStatRepresentativePropagateUseRepresentativeCount;

						cint64 mStatBackPropActivationCount;


						cint64 mStatConDesInsertionCount;
						cint64 mStatConDesContainedCount;



						cint64 mStatPossibleInstanceMergingTryingCount;
						cint64 mStatPossibleInstanceMergingCount;
						cint64 mStatPossibleInstanceMergingSearchIndiCount;
						cint64 mStatPossibleInstanceMergingFoundIndiCount;
						cint64 mStatPossibleInstanceMergingSkipIndiCount;
						cint64 mStatPossibleInstanceMergingNotMergeableCount;
						cint64 mStatPossibleInstanceMergingMaybeMergeableCount;
						cint64 mStatPossibleInstanceMergingSuccessSubmitCount;
						cint64 mStatPossibleInstanceMergingTriviallySuccessCount;
						cint64 mStatClashCount;
						cint64 mStatSatisfiableCount;
						cint64 mStatStoppedCount;


						QTime mTimerBacktracing;
						QTime mUnsatCacheRetrieval;
						QTime mComplGraphReuseCacheRetrieval;


						bool mNominalMerged;
						cint64 mNominalMergedCount;

						cint64 mOverJumpedNonDeterministicDecisionCount;
						cint64 mRelevantNonDeterministicDecisionCount;



						class IndiAssociatedConceptSetCacheData {
						public:
							QSet< QSet<CConcept*> > mConceptSet;
							bool mCreated = false;
						};
						QHash<CIndividualProcessNode*, IndiAssociatedConceptSetCacheData > mCachedIndiAssociatedConceptSetHash;


						bool mFirstBlockingTestDebugWritten = false;
						bool mFirstBindingCreationDebugWritten = false;



						QSet<cint64> mPropCuttedIndiIds;
						QSet<cint64> mPropCuttedExpandedIndiIds;



						cint64 nextReportingExpansionCount = 1000;
						cint64 lastRecomputationTaskId = 0;
						cint64 lastTaskDepth = 100;
						cint64 debugExpansionCount = 500;


					// private methods
					private:

					// private variables
					private:

				};



			}; // end namespace Algorithm

		}; // end namespace Kernel 

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUCOMPLETIONTASKHANDLEALGORITHM_H
