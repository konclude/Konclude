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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUAPPROXIMATIONSATURATIONTASKHANDLEALGORITHM_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUAPPROXIMATIONSATURATIONTASKHANDLEALGORITHM_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContextBase.h"
#include "CCalculationStopProcessingException.h"
#include "CCalculationErrorProcessingException.h"
#include "CSatisfiableTaskSaturationPreyingAnalyser.h"
#include "CCalculationTableauSaturationTaskHandleAlgorithm.h"

// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Kernel/Process/CConceptSaturationProcessLinker.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNodeStatusUpdateLinker.h"

#include "Scheduler/CTask.h"
#include "Scheduler/CTaskHandleAlgorithm.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CRoleProcessData.h"

#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CMemoryAllocationException.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Consistiser;

		namespace Kernel {

			using namespace Process;
			using namespace Task;

			namespace Algorithm {

#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED

#define KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(a)

#else

//#define KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(a) a
#define KONCLUCE_TASK_ALGORITHM_SATURATION_SATURATION_MODEL_STRING_INSTRUCTION(a)

#endif

				/*! 
				 *
				 *		\class		CCalculationTableauApproximationSaturationTaskHandleAlgorithm
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCalculationTableauApproximationSaturationTaskHandleAlgorithm : public CTaskHandleAlgorithm {
					// public methods
					public:
						//! Constructor
						CCalculationTableauApproximationSaturationTaskHandleAlgorithm();

						//! Destructor
						virtual ~CCalculationTableauApproximationSaturationTaskHandleAlgorithm();

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

						bool hasRemainingProcessingNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasRemainingExtensionProcessingNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext);



						bool completeSaturatedIndividualNodes(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext);
						bool continueNominalDelayedIndividualNodeProcessing(CProcessingDataBox* processingDataBox, CCalculationAlgorithmContextBase* calcAlgContext);


						void testInsufficientALLConcepts(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void testRelevantConceptRoleRatio(CCalculationAlgorithmContextBase* calcAlgContext);

					
						bool individualNodeInitializing(CIndividualSaturationProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void individualNodeConclusion(CIndividualSaturationProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void initializeInitializationConcepts(CIndividualSaturationProcessNode*& indiProcNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void applyTableauSaturationRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);

						void applyAutomatChooseRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyNONERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyDATATYPERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyNotDATATYPERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyANDRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyORRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applySOMERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applySELFRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyALLRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyATMOSTRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyATLEASTRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyIMPLICATIONRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyEQCANDRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyBOTTOMRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyVALUERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						void applyNOMINALRule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);

						void createSuccessorForConcept(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker, cint64 cardinality, CCalculationAlgorithmContextBase* calcAlgContext);

						
						CIndividualProcessNode* getCorrectedNode(cint64 individualID, CIndividualProcessNodeVector* indiVec, CCalculationAlgorithmContextBase* calcAlgContext);

						
						void addNominalDependentIndividualNode(cint64 nominalID, CIndividualSaturationProcessNode* dependentIndiNode, CSaturationNominalDependentNodeData::NOMINALCONNECTIONTYPE connectionType, CCalculationAlgorithmContextBase* calcAlgContext);

						void addInfluencedNominal(cint64 influencedNominalID, CCalculationAlgorithmContextBase* calcAlgContext);



						void delayNominalSaturationConceptProcessing(CIndividualSaturationProcessNode* processIndi, CConceptSaturationProcessLinker* conProLinker, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext);
						void propagateUnloadedABoxCompletionGraphDependentIndividualNodeFlag(CIndividualSaturationProcessNode* processIndi, CConceptSaturationProcessLinker* conProLinker, cint64 nominalID, CCalculationAlgorithmContextBase* calcAlgContext);


						void addAutomateTransitionOperands(CIndividualSaturationProcessNode*& processIndi, CConcept* automatConcept, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						bool testAutomateTransitionOperandsAddable(CIndividualSaturationProcessNode*& processIndi, CConcept* automatConcept, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);


						void applyELSERule(CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);

						void applyBackwardPropagationConcepts(CBackwardSaturationPropagationLink* backPropLink, CBackwardSaturationPropagationReapplyDescriptor* backPropReapplyDesIt, CCalculationAlgorithmContextBase* calcAlgContext);

						void addIndividualToProcessingQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addUninitializedIndividualToProcessingQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addIndividualToCompletionQueue(CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);



						void setDelayedNominalProcessingOccured(CCalculationAlgorithmContextBase* calcAlgContext);
						void setInsufficientNodeOccured(CCalculationAlgorithmContextBase* calcAlgContext);
						void setProblematicEQCandidateOccured(CCalculationAlgorithmContextBase* calcAlgContext);



						void collectLinkedSuccessorNodes(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addLinkedSuccessorNodeForConcept(CConceptSaturationDescriptor* conDes, CLinkedRoleSaturationSuccessorHash* linkedRoleSuccHash, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);





						CSaturationSuccessorExtensionData* getSucessorExtensionData(CLinkedRoleSaturationSuccessorData* succData, bool create, CCalculationAlgorithmContextBase* calcAlgContext);

						void initializeSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateSuccessorRoleALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, const CRoleBackwardSaturationPropagationHashData& backwardPropData, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateSuccessorRoleALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						bool updateSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool addSuccessorExtensionsALLConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CConcept* concept, bool conceptNegation, CSaturationSuccessorALLConceptExtensionData* allConSuccExtData, CCalculationAlgorithmContextBase* calcAlgContext);
						bool processSuccessorALLConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addALLProcessRoleExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						void addProcessExtensionToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);




						void addFUNCTIONALProcessRoleExtensionFunctionalityAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						void addFUNCTIONALProcessRoleExtensionLinkedSuccessorAddedToDependentIndividuals(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						bool processSuccessorFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool updateSuccessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);
						bool updateSuccessorRoleFUNCTIONALConceptsExtensions(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CLinkedRoleSaturationSuccessorData* succData, CCalculationAlgorithmContextBase* calcAlgContext);

						bool installSuccessorRoleFunctionalityConceptsExtension(CIndividualSaturationProcessNode*& indiProcSatNode, CRole* role, CCalculationAlgorithmContextBase* calcAlgContext);


						bool processNextSuccessorExtensions(CCalculationAlgorithmContextBase* calcAlgContext);

						bool addSuccessorExtensionToProcessingQueue(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);




						CIndividualSaturationProcessNode* createResolvedIndividualNode(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode*& copyIndiProcSatNode, bool queueProcessing, CCalculationAlgorithmContextBase* calcAlgContext);

						CIndividualSaturationProcessNode* getResolvedIndividualNodeExtensionSuccessor(CIndividualSaturationProcessNode* indiProcSatNode, CSaturationSuccessorConceptExtensionMap* succConExtMap, CCalculationAlgorithmContextBase* calcAlgContext);
						CSaturationIndividualNodeExtensionResolveData* getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CConcept* concept, bool negation, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						CSaturationIndividualNodeExtensionResolveData* getResolvedIndividualNodeExtension(CSaturationIndividualNodeExtensionResolveData* resolveData, CIndividualSaturationProcessNode* extensionNode, CIndividualSaturationProcessNode*& copyIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void addALLConceptExtensionProcessingRole(CRole* role, CRoleBackwardSaturationPropagationHashData& backPropHashData, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addFUNCTIONALConceptExtensionProcessingRole(CRole* role, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addNewLinkedExtensionProcessingRole(CRole* role, CIndividualSaturationProcessNode*& indiProcSatNode, bool queueALLExtension, bool queueFUNCTIONALExtension, CCalculationAlgorithmContextBase* calcAlgContext);



						void initializeIndividualNodeByCoping(CIndividualSaturationProcessNode* indiProcSatNode, CIndividualSaturationProcessNode* copyFromIndiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);


						void checkNextCriticalConcepts(CCalculationAlgorithmContextBase* calcAlgContext);
						bool hasNextCriticalConcepts(CCalculationAlgorithmContextBase* calcAlgContext);

						void addCriticalConceptDescriptor(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						
						void checkCriticalConceptsForNode(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addCriticalConceptForDependentNodes(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, bool directFlagsCheck, cint64 checkFlags, CCalculationAlgorithmContextBase* calcAlgContext);


						void addCriticalORConceptTestedForDependentNodes(CConceptSaturationDescriptor* conDes, CCriticalSaturationConceptTypeQueues::CRITICALSATURATIONCONCEPTQUEUETYPE conceptType, CIndividualSaturationProcessNode*& indiProcSatNode, CCriticalIndividualNodeConceptTestSet* criticalIndiNodeConTestSet, CCalculationAlgorithmContextBase* calcAlgContext);


						bool isCriticalALLConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isCriticalORConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isCriticalATMOSTConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, bool& ancestorPossiblyCriticalFlag, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isCriticalVALUEConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isCriticalNOMINALConceptDescriptorInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool isCriticalEQCANDConceptDescriptorProblematic(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);

						bool markATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						bool markNominalATMOSTRestrictedAncestorsAsInsufficient(CConceptSaturationDescriptor* conDes, CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void updateExtractDisjunctCommonConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void initializeExtractDisjunctCommonConcept(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void addDisjunctCommonConceptExtractionToProcessingQueue(CIndividualSaturationProcessNode*& indiProcSatNode, CCalculationAlgorithmContextBase* calcAlgContext);


						void addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CXNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptsFilteredToIndividual(CXSortedNegLinker<CConcept*>* conceptAddLinker, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);


						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& processIndi, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						void addConceptFilteredToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& processIndi, CCalculationAlgorithmContextBase* calcAlgContext);


						void addConceptToIndividual(CConcept* addingConcept, bool negate, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, bool updateCopyDependedIndividual, CCalculationAlgorithmContextBase* calcAlgContext);
						bool insertConceptToIndividualConceptSet(CConceptSaturationDescriptor* conceptSaturationDescriptor, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext);

						void processModificationUpdateLinkers(CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CSaturationModifiedProcessUpdateLinker* modProcUpdateLinker, CCalculationAlgorithmContextBase* calcAlgContext);

						bool hasConceptLocalImpact(CConcept* concept, bool conNeg, CCalculationAlgorithmContextBase* calcAlgContext);

						bool updateImplicationReapplyConceptSaturationDescriptor(CImplicationReapplyConceptSaturationDescriptor* reapplyImpReapplyConSatDes, CIndividualSaturationProcessNode*& rootProcessIndi, CReapplyConceptSaturationLabelSet* labelSet, CCalculationAlgorithmContextBase* calcAlgContext);

						CRoleSaturationProcessLinker* createRoleSaturationProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext);
						CConceptSaturationDescriptor* createConceptSaturationDescriptor(CCalculationAlgorithmContextBase* calcAlgContext);
						CConceptSaturationProcessLinker* createConceptSaturationProcessLinker(CCalculationAlgorithmContextBase* calcAlgContext);
						CImplicationReapplyConceptSaturationDescriptor* createImplicationReapplyConceptSaturationDescriptor(CCalculationAlgorithmContextBase* calcAlgContext);

						void releaseConceptSaturationDescriptor(CConceptSaturationDescriptor* conSatDes, CCalculationAlgorithmContextBase* calcAlgContext);
						void releaseConceptSaturationProcessLinker(CConceptSaturationProcessLinker* conSatProcLinker, CCalculationAlgorithmContextBase* calcAlgContext);
						void releaseRoleSaturationProcessLinker(CRoleSaturationProcessLinker* roleSatProcLinker, CCalculationAlgorithmContextBase* calcAlgContext);


						CIndividualSaturationProcessNodeStatusUpdateLinker* createIndividualSaturationUpdateLinker(CCalculationAlgorithmContextBase* calcAlgContext);
						void releaseIndividualSaturationUpdateLinker(CIndividualSaturationProcessNodeStatusUpdateLinker* conSatUpdateLinker, CCalculationAlgorithmContextBase* calcAlgContext);

						CSaturationModifiedProcessUpdateLinker* createModifiedProcessUpdateLinker(CCalculationAlgorithmContextBase* calcAlgContext);


						QString getDebugIndividualConceptName(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);

						QString generateDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext);
						QString generateExtendedDebugIndiModelStringList(CCalculationAlgorithmContextBase* calcAlgContext, cint64 firstIndiID = 0, cint64 lastIndiID = -1);
						QStringList generateStatusFlagsStringList(CIndividualSaturationProcessNodeStatusFlags* statusFlags, CCalculationAlgorithmContextBase* calcAlgContext);


						void testDebugSaturationTaskContainsConcept(CConcept* concept, bool conNegation, CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);
						void testDebugSaturationTaskContainsConcepts(CIndividualSaturationProcessNode* indiNode, CCalculationAlgorithmContextBase* calcAlgContext);

						void updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, cint64 flags, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateDirectNotDependentAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, cint64 flags, CCalculationAlgorithmContextBase* calcAlgContext);

						void updateDirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateDirectNotDependentAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateIndirectAddingIndividualStatusFlags(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						
						bool requiresDirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext);
						bool requiresIndirectAddingIndividualStatusFlagsUpdate(CIndividualSaturationProcessNode* indiNode, CIndividualSaturationProcessNodeStatusFlags* addingFlags, CCalculationAlgorithmContextBase* calcAlgContext);


						bool loadConsistenceModelData(CCalculationAlgorithmContext* calcAlgContext);




						bool requiresAddingSuccessorConnectedNominals(CIndividualSaturationProcessNode* indiNode, cint64 addingNominalID, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateAddingSuccessorConnectedNominal(CIndividualSaturationProcessNode* indiNode, cint64 addingNominalID, CCalculationAlgorithmContextBase* calcAlgContext);
						void updateAddingSuccessorConnectedNominal(CIndividualSaturationProcessNode* indiNode, CSuccessorConnectedNominalSet* succConnNomSet, CCalculationAlgorithmContextBase* calcAlgContext);

					// protected variables
					protected:
						CCalculationAlgorithmContextBase* mCalcAlgContext;
						CProcessingDataBox* mProcessingDataBox;

						CSatisfiableTaskSaturationPreyingAnalyser mSatTaskSaturationPreyAnalyser;


						typedef void (CCalculationTableauApproximationSaturationTaskHandleAlgorithm::*TableauRuleFunction) (CIndividualSaturationProcessNode*& processIndi, CConceptSaturationProcessLinker* conProLinker);
						const static cint64 mRuleFuncCount = 200;
						TableauRuleFunction* mPosJumpFuncVec;
						TableauRuleFunction* mNegJumpFuncVec;

						TableauRuleFunction mPosTableauRuleJumpFuncVec[mRuleFuncCount];
						TableauRuleFunction mNegTableauRuleJumpFuncVec[mRuleFuncCount];

						bool mConfImplicationAddingSkipping;
						bool mConfForceAllConceptInsertion;
						bool mConfForceAllCopyInsteadOfSubstituition;
						bool mConfDebuggingWriteData;

						bool mConfAddCriticalConceptsToQueues;
						bool mConfDirectlyCriticalToInsufficient;
						bool mConfCheckCriticalConcepts;


						bool mConfConceptsExtensionProcessing;
						bool mConfALLConceptsExtensionProcessing;
						bool mConfFUNCTIONALConceptsExtensionProcessing;
						bool mConfNominalProcessing;
						bool mConfCopyNodeFromTopIndividualForManyConcepts;


						CCalculationConfigurationExtension *mLastConfig;



						bool mCachedCompletionGraphMissing;
						bool mCachedCompletionGraphLoaded;
						CIndividualProcessNodeVector* mDetCachedCGIndiVector;
						CIndividualProcessNodeVector* mNonDetCachedCGIndiVector;
						bool mNonDetConsistencyCG;
						bool mDetConsistencyCG;




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
						cint64 mCopiedIndiNodeCount;

						cint64 mDirectUpdatedStatusIndiNodeCount;
						cint64 mIndirectUpdatedStatusIndiNodeCount;

						cint64 mSuccessorConnectedNominalUpdatedCount;

						cint64 mALLSuccExtInitializedCount;


						cint64 mDisjunctionInitializedSkippedCount;


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

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCALCULATIONTABLEAUAPPROXIMATIONSATURATIONTASKHANDLEALGORITHM_H
