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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUPILINGSATURATIONTASKHANDLEALGORITHM_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUPILINGSATURATIONTASKHANDLEALGORITHM_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContextBase.h"
#include "CCalculationStopProcessingException.h"
#include "CCalculationErrorProcessingException.h"
#include "CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser.h"
#include "CCalculationTableauSaturationTaskHandleAlgorithm.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Kernel/Process/CConceptPilingProcessLinker.h"

#include "Scheduler/CTask.h"
#include "Scheduler/CTaskHandleAlgorithm.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CMemoryAllocationException.h"


#include "Reasoner/Generator/CSatisfiableCalculationTaskFromCalculationJobGenerator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Generator;

		namespace Kernel {

			using namespace Process;
			using namespace Task;

			namespace Algorithm {

#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

#define KONCLUCE_TASK_ALGORITHM_PILING_SATURATION_MODEL_STRING_INSTRUCTION(a)

#else

//#define KONCLUCE_TASK_ALGORITHM_PILING_SATURATION_MODEL_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_PILING_SATURATION_MODEL_STRING_INSTRUCTION(a)

#endif

				/*! 
				 *
				 *		\class		CCalculationTableauPilingSaturationTaskHandleAlgorithm
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationTableauPilingSaturationTaskHandleAlgorithm : public CTaskHandleAlgorithm {
					// public methods
					public:
						//! Constructor
						CCalculationTableauPilingSaturationTaskHandleAlgorithm();

						//! Destructor
						virtual ~CCalculationTableauPilingSaturationTaskHandleAlgorithm();

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

						void addParallelizationPossibilities(CIndividualPilingProcessNode* indiNode, QSet<CIndividualPilingProcessNode*>& currSet, QHash<CIndividualPilingProcessNode*,cint64>& prevHash, cint64& maxParallelDepth, CCalculationAlgorithmContextBase* calcAlgContext);
						void analyseParallelizationPossibilities(CCalculationAlgorithmContextBase* calcAlgContext);

					
						void analyseNodeExpressivityViolation(CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateDirectExpressivityViolations(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateIndirectExpressivityViolations(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool individualNodeInitializing(CIndividualPilingProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void individualNodeConclusion(CIndividualPilingProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void initializeInitializationConcepts(CIndividualPilingProcessNode*& indiProcNode, CXSortedNegLinker<CConcept*>* initConceptLinkerIt, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyTableauSaturationRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);

						void applyAutomatChooseRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);
						void applyANDRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);
						void applySOMERule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);
						void applyALLRule(CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);
						

						void applyBackwardPropagationConcepts(CBackwardPilingPropagationLink* backPropLink, CBackwardPilingPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext);

						void addIndividualToProcessingQueue(CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);




						void addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);


						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);


						void addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, cint64 pilingDepth, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool insertConceptToIndividualConceptSet(CConceptPilingDescriptor* conceptPilingDescriptor, cint64 pilingDepth, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext);


						bool hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext);

						bool updateImplicationReapplyConceptPilingDescriptor(CImplicationReapplyConceptPilingDescriptor* reapplyImpReapplyConPilDes, cint64 prevPilingDepth, CIndividualPilingProcessNode*& rootProcessIndi, CReapplyConceptPilingLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext);

						CConceptPilingDescriptor* createConceptPilingDescriptor(CCalculationAlgorithmContextBase* calcAlgContext);
						CConceptPilingProcessLinker* createConceptPilingProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext);
						CImplicationReapplyConceptPilingDescriptor* createImplicationReapplyConceptPilingDescriptor(CCalculationAlgorithmContextBase* calcAlgContext);

						void releaseConceptPilingDescriptor(CConceptPilingDescriptor* conPilDes, CCalculationAlgorithmContextBase* calcAlgContext);
						void releaseConceptPilingProcessLinker(CConceptPilingProcessLinker* conPilProcLinker, CCalculationAlgorithmContextBase* calcAlgContext);


						QString generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID = 0, cint64 lastIndiID = -1);


						void createDebugSatisfiableSaturationTestingTask(CCalculationAlgorithmContextBase* calcAlgContext);
						void testDebugSaturationTaskContainsConcept(CConcept* concept, bool conNegation, CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void testDebugSaturationTaskContainsConcepts(CIndividualPilingProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);

					// protected variables
					protected:
						CCalculationAlgorithmContextBase* mCalcAlgContext;
						CProcessingDataBox* mProcessingDataBox;




						typedef void (CCalculationTableauPilingSaturationTaskHandleAlgorithm::*TableauRuleFunction) (CIndividualPilingProcessNode*& processIndi, CConceptPilingProcessLinker* conProLinker);
						const static cint64 mRuleFuncCount = 31;
						TableauRuleFunction* mPosJumpFuncVec;
						TableauRuleFunction* mNegJumpFuncVec;

						TableauRuleFunction mPosTableauRuleJumpFuncVec[mRuleFuncCount];
						TableauRuleFunction mNegTableauRuleJumpFuncVec[mRuleFuncCount];

						bool mConfAncestorBlockingSearch;
						bool mConfImplicationAddingSkipping;
						bool mConfCreateSuccessorLinks;

						CCalculationConfigurationExtension *mLastConfig;


						CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser mSubsumAnalyser;




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


						CCalculationTableauSaturationTaskHandleAlgorithm* mDebugSaturationAlgorithm;
						CSatisfiableCalculationTask* mDebugTestingSaturationTask;
						QString mDebugTestSaturationDebugIndiModelString;


					// private methods
					private:

					// private variables
					private:

				};



			}; // end namespace Algorithm

		}; // end namespace Kernel 

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUPILINGSATURATIONTASKHANDLEALGORITHM_H
