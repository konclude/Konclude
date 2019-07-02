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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUSATURATIONTASKHANDLEALGORITHM_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUSATURATIONTASKHANDLEALGORITHM_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContextBase.h"
#include "CCalculationStopProcessingException.h"
#include "CCalculationErrorProcessingException.h"
#include "CCalculationClashProcessingException.h"
#include "CSatisfiableTaskConsistencyPreyingAnalyser.h"
#include "CSatisfiableTaskMultiSubsumptionIdentifierAnalyser.h"
#include "CTrackedClashedDependencyLine.h"
#include "CTrackedClashedDescriptor.h"
#include "CTrackedClashedDescriptorHasher.h"
#include "CDependencyFactory.h"
#include "CClashDescriptorFactory.h"
#include "CIndividualNodeManager.h"
#include "CUnsatisfiableCacheHandler.h"
#include "CSatisfiableExpanderCacheHandler.h"
#include "CCompletionGraphCacheHandler.h"
#include "CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser.h"

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

#include "Reasoner/Kernel/Process/CClashedConceptDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedIndividualLinkDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedIndividualDistinctDescriptor.h"
#include "Reasoner/Kernel/Process/CClashedNegationDisjointLinkDescriptor.h"

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


#include "Reasoner/Kernel/Strategy/CConceptProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CIndividualProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CTaskProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CConcreteConceptProcessingOperatorPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CIndividualAncestorDepthMaximumConceptProcessingPriorityStrategy.h"
#include "Reasoner/Kernel/Strategy/CEqualDepthTaskProcessingPriorityStrategy.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Reasoner/Kernel/Cache/CExpanderBranchedLinker.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheEntry.h"

#include "Scheduler/CTask.h"
#include "Scheduler/CTaskHandleAlgorithm.h"


#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CMemoryAllocationException.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

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

#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

#define KONCLUCE_TASK_ALGORITHM_SATURATION_MODEL_STRING_INSTRUCTION(a)

#else

//#define KONCLUCE_TASK_ALGORITHM_SATURATION_MODEL_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_SATURATION_MODEL_STRING_INSTRUCTION(a)

#endif

				/*! 
				 *
				 *		\class		CCalculationTableauSaturationTaskHandleAlgorithm
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationTableauSaturationTaskHandleAlgorithm : public CTaskHandleAlgorithm {
					// public methods
					public:
						//! Constructor
						CCalculationTableauSaturationTaskHandleAlgorithm();

						//! Destructor
						virtual ~CCalculationTableauSaturationTaskHandleAlgorithm();

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
						CCalculationAlgorithmContextBase* createCalculationAlgorithmContext(CTaskProcessorContext *processorContext, CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask);
					
						bool individualNodeInitializing(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void individualNodeConclusion(CIndividualProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyTableauSaturationRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);

						void applyAutomatChooseRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						void applyANDRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						void applySOMERule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						void applyALLRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						void applyIMPLICATIONRule(CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						

						void applyBackwardPropagationConcepts(CIndividualProcessNode*& processIndi, CBackwardPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext);

						void addIndividualToProcessingQueue(CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						void addConceptsToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CConceptAssertionLinker* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);

						void addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualProcessNode*& processIndi, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);
						bool insertConceptToIndividualConceptSet(CConceptDescriptor* conceptDescriptor, CIndividualProcessNode*& processIndi, CReapplyConceptLabelSet* conLabelSet, CCondensedReapplyQueueIterator* reapplyIt, bool nodeInitialization, CCalculationAlgorithmContextBase* calcAlgContext);
						void applyReapplyQueueConcepts(CIndividualProcessNode*& processIndi, CCondensedReapplyQueueIterator* reapplyQueueIt, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptToProcessingQueue(CConceptDescriptor *conceptDescriptor, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptRestrictedToProcessingQueue(CConceptDescriptor *conceptDescriptor, CProcessingRestrictionSpecification* procRest, CIndividualProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);

						void initializeInitializationConcepts(CIndividualProcessNode*& indiProcNode, CXSortedNegLinker<CConcept*>* initConceptLinkerIt, CCalculationAlgorithmContextBase* calcAlgContext);

						bool hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext);


						QString generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID = 0, cint64 lastIndiID = -1);

					// protected variables
					protected:
						CCalculationAlgorithmContextBase* mCalcAlgContext;
						CProcessingDataBox* mProcessingDataBox;


						CConceptProcessingPriorityStrategy* mConceptPriorityStrategy;
						CIndividualProcessingPriorityStrategy* mIndividualPriorityStrategy;
						CTaskProcessingPriorityStrategy* mTaskProcessingStrategy;
						CUnsatisfiableCacheRetrievalStrategy* mUnsatCachRetStrategy;

						CClashDescriptorFactory* mClashDesFactory;
						CDependencyFactory* mDependencyFactory;
						CIndividualNodeManager* mIndiNodeManager;
						CUnsatisfiableCacheHandler* mUnsatCacheHandler;
						CSatisfiableExpanderCacheHandler* mSatExpCacheHandler;
						CCompletionGraphCacheHandler* mCompGraphCacheHandler;
						CSaturationNodeExpansionCacheHandler* mSatNodeCacheHandler;


						typedef void (CCalculationTableauSaturationTaskHandleAlgorithm::*TableauRuleFunction) (CIndividualProcessNode*& processIndi, CConceptProcessLinker* conProLinker);
						const static cint64 mRuleFuncCount = 31;
						TableauRuleFunction* mPosJumpFuncVec;
						TableauRuleFunction* mNegJumpFuncVec;

						TableauRuleFunction mPosTableauRuleJumpFuncVec[mRuleFuncCount];
						TableauRuleFunction mNegTableauRuleJumpFuncVec[mRuleFuncCount];

						bool mConfAncestorBlockingSearch;

						CCalculationConfigurationExtension *mLastConfig;


						CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser mSubsumAnalyser;


						struct TConceptDescriptorProcessLinkerPairStruct {
							CConceptDescriptor conceptDescriptor;
							CConceptProcessLinker conceptProcessLinker;
						};


						// for debugging
						cint64 mAppliedALLRuleCount;
						cint64 mAppliedSOMERuleCount;
						cint64 mAppliedANDRuleCount;
						cint64 mAppliedORRuleCount;
						cint64 mAppliedATLEASTRuleCount;
						cint64 mAppliedATMOSTRuleCount;
						cint64 mAppliedTOTALRuleCount;

						cint64 mAddedALLConcepts;
						cint64 mAddedSOMEConcepts;
						cint64 mAddedIMPLConcepts;
						cint64 mAddedTRIGGConcepts;
						cint64 mAddedSUBConcepts;
						cint64 mAddedELSEConcepts;

						cint64 mSubstituitedIndiNodeCount;

						QStringList mDebugIndiModelStringList;
						QString mDebugIndiModelString;

						QString mRuleBeginDebugIndiModelString;
						QString mRuleEndDebugIndiModelString;
						QString mEndSaturationDebugIndiModelString;


					// private methods
					private:

					// private variables
					private:

				};



			}; // end namespace Algorithm

		}; // end namespace Kernel 

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUSATURATIONTASKHANDLEALGORITHM_H
