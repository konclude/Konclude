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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCLASSIFICATIONMESSAGEANALYSER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCLASSIFICATIONMESSAGEANALYSER_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Classifier/CClassificationMessageDataObserver.h"
#include "Reasoner/Classifier/CClassificationClassSubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationClassSubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationUpdatePossibleClassSubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationInitializePossibleClassSubsumptionMessageData.h"
#include "Reasoner/Classifier/CClassificationMessageData.h"
#include "Reasoner/Classifier/CClassificationSatisfiableCalculationConceptReferenceLinking.h"
#include "Reasoner/Classifier/CClassificationSatisfiablePossibleSubsumptionCalculationConceptReferenceLinking.h"
#include "Reasoner/Classifier/CClassificationPseudoModelIdentifierMessageData.h"

#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CDependencyNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CIndividualSaturationProcessNode.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSatisfiableReferenceLinkingData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Consistiser;
		using namespace Classifier;
		using namespace Ontology;

		namespace Kernel {

			using namespace Task;
			using namespace Process::Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskClassificationMessageAnalyser
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskClassificationMessageAnalyser {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskClassificationMessageAnalyser();

						bool readCalculationConfig(CCalculationConfigurationExtension *config);
						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);

						CIndividualProcessNode* getCorrectedIndividualID(CIndividualProcessNode* baseIndiNode, CIndividualProcessNodeVector* indiNodeVec, bool* nondetMergedFlag);


						void checkCanHaveClashWithModel(CIndividualProcessNode* indiNode, CConcept* concept, bool negated, bool& clashFoundFlag, bool& unknownFlag, bool& clashFreeFlag, cint64 depth, QSet<CIndividualProcessNode*>* testedIndividualsSet, CIndividualProcessNode* lastNode, CIndividualProcessNodeVector* indiNodeVec);
						void verifySubsumptionPossibleWithModel(CIndividualProcessNode* indiNode, CConcept* testingConcept, CClassificationMessageData* possSubsumMessageData, CCLASSSUBSUMPTIONMESSAGELIST<CConcept*>* subsumerList);


					// protected methods
					protected:
						CClassificationMessageData* extractPossibleSubsumptionInformation(CConceptDescriptor* conDes, CIndividualProcessNode* indiNode, CReapplyConceptLabelSet* conSet, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:

						class CSaturatedMergedTestItem {
							public:
								CSaturatedMergedTestItem() {
									mSuccessfullyMerged = false;
									mSatisfiableMerged = false;
								}
							public:
								bool mSuccessfullyMerged;
								bool mSatisfiableMerged;
						};


						class CConceptNegationTriggerItem {
							public:
								CConceptNegationTriggerItem() {
									mTriggerFlag = false;
									mNegationFlag = false;
									mConcept = nullptr;
									mIndiSatNode = nullptr;
								}
							public:
								bool mTriggerFlag;
								bool mNegationFlag;
								CConcept* mConcept;
								CIndividualSaturationProcessNode* mIndiSatNode;
						};



						class CIndividualAnalyseProcessItem {
							public:
								CIndividualAnalyseProcessItem* initIndividualAnalyseProcessItem(cint64 indiID = 0, cint64 rootDistance = 0, CIndividualAnalyseProcessItem* ancItem = nullptr, CIndividualLinkEdge* ancLink = nullptr) {
									mRootDistance = rootDistance;
									mAncItem = ancItem;
									mIndiID = indiID;
									mAncLink = ancLink;
									return this;
								}
							public:
								cint64 mIndiID;
								cint64 mRootDistance;
								CIndividualLinkEdge* mAncLink;
								CIndividualAnalyseProcessItem* mAncItem;
						};



						class CPseudoModelAnalyseProcessItem {
							public:
								CPseudoModelAnalyseProcessItem* initPseudoModelAnalyseProcessItem(cint64 pmModelID = 0, cint64 rootDistance = 0) {
									mPMModelID = pmModelID;
									mRootDistance = rootDistance;
									mNodeLinker = nullptr;
									return this;
								}
								CPseudoModelAnalyseProcessItem* addNodeLinker(CXNegLinker<CIndividualProcessNode*>* nodeLinker) {
									if (nodeLinker) {
										mNodeLinker = nodeLinker->append(mNodeLinker);
									}
									return this;
								}
							public:
								cint64 mPMModelID;
								cint64 mRootDistance;
								CXNegLinker<CIndividualProcessNode*>* mNodeLinker;
						};


						typedef QPair<CConcept*,bool> TConceptNegationPair;


						bool mConfAnalyseSubSumptions;
						bool mConfAnalyseAllSubSumptions;
						bool mConfAnalyseIdentifierOccurence;
						bool mConfAnalysePossibleSubSumptions;
						bool mEquivalentAlternativesSaturationMerging;


						bool mConfStopAnalyseWhenNotChanging;


						cint64 mMaxModelAnalysingDepth;
						cint64 mMaxPossibleSubsumerNegationCheckingDepth;

						cint64 mMaxSaturatedModelMergingTestRecursionDepth;

						CClassPossibleSubsumptionMapIterator* mReusePossSubsumMapIterator;


						CPROCESSINGHASH< CConcept*,CCLASSPOSSIBLESUBSUMPTIONMESSAGELIST<CClassificationInitializePossibleClassSubsumptionData*>* >* mMultiplePossSubsumInitAvoidHash;
						bool mUseAlwaysConRefLinkDataHash;
						QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* mConRefLinkDataHash;
						CTaskMemoryPoolAllocationManager* mTempMemAllocMan;
						CContextBase* mTmpContext;
						CSatisfiableCalculationTask* mStatCalcTask;


						CTaskMemoryPoolAllocationManager* mPMTempMemAllocMan;
						CContextBase* mPMTmpContext;
						cint64 mMaxPseudoModelDepth;
						cint64 mMaxPseudoModelNodes;


						cint64 mStatConsideredOtherNodeCount;
						cint64 mStatExtractedPossibleSubsumerOtherNodeCount;

						cint64 mStatSuccessorModelMergingTestCount;



						CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >* mTestSaturatedMergedHash;
						CIndividualProcessNode* mLastMergingTestNode;

					// private methods
					private:
						CConceptDescriptor* getIndividualProcessNodeConceptWithSingleAncestorDependency(CIndividualProcessNode* indiNode, CIndividualAnalyseProcessItem* indiAnProcItem, CCalculationAlgorithmContext* calcAlgContext);
						bool hasDependencyToAncestor(CIndividualProcessNode* individualNode, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						bool testSubsumerCandidatePossibleWithMergedSaturatedModel(CIndividualProcessNode* indiNode, CConcept* equivConcept, CCalculationAlgorithmContext* calcAlgContext);

						bool testSubsumerCandidatePossibleWithMergedSaturatedModel(CIndividualProcessNode* indiNode, CConcept* testConcept, bool negation, bool& mergeSatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool testConceptSetWithSaturatedModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, bool& mergeClashedFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool testRoleSuccessorsWithSaturatedModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContext* calcAlgContext);
						bool testSaturatedExistentialsModelMergable(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CCalculationAlgorithmContext* calcAlgContext);


						bool collectSuccessorMergingNodesAndConcepts(CIndividualProcessNode* indiNode, CIndividualSaturationProcessNode* saturationIndiNode, CReapplyRoleSuccessorHash* roleSuccHash, CRole* role, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, CCalculationAlgorithmContext* calcAlgContext);
						bool collectSuccessorMergingNodesAndConcepts(CIndividualSaturationProcessNode* excludeSaturationIndiNode, CRole* role, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& nextSuccessorList, CPROCESSINGLIST<TConceptNegationPair>& nextTrivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& nextBackwardRoleSet, CPROCESSINGHASH<CRole*,TConceptNegationPair>& successorInfluenceConceptsHash, CCalculationAlgorithmContext* calcAlgContext);


						bool collectEquivalenceConceptAlternatives(CIndividualProcessNode* indiNode, CConcept* testConcept, bool testConceptNegation, CPROCESSINGSET< QPair<CConcept*,bool> >& alternativesSet, CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >* testSaturatedMergedHash, bool& oneMergeSatisfieableFlag, bool& allMergeUnsatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext);
						bool testEquivalenceConceptAlternatives(CIndividualProcessNode* indiNode, CPROCESSINGSET< QPair<CConcept*,bool> >& alternativesSet, CPROCESSINGHASH< QPair<CConcept*,bool>,CSaturatedMergedTestItem >* testSaturatedMergedHash, bool& oneMergeSatisfieableFlag, bool& allMergeUnsatisfieableFlag, CCalculationAlgorithmContext* calcAlgContext);
						
						CIndividualSaturationProcessNode* getSaturatedIndividualNodeForConcept(CConcept* concept, bool conceptNegation, CCalculationAlgorithmContext* calcAlgContext);
						CIndividualSaturationProcessNode* getExistentialSaturatedIndividualNodeForConcept(CConcept* concept, CCalculationAlgorithmContext* calcAlgContext);

						bool testSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext);
						bool testSingleSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext);
						bool testMultipleSaturatedSuccessorModelMergable(CIndividualSaturationProcessNode* existentialIndiNode, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>* trivialSuccessorPropagatedConceptList, CPROCESSINGSET<CRole*>& backwardRoleSet, cint64 remainingMergingDepth, cint64& remainingMergingCount, CCalculationAlgorithmContext* calcAlgContext);
						



						bool addAutomateTransactionConcepts(CConcept* concept, bool negation, CRole* role, CPROCESSINGLIST<CIndividualSaturationProcessNode*>& successorList, CPROCESSINGLIST<TConceptNegationPair>& trivialConceptTestingList, CCalculationAlgorithmContext* calcAlgContext);
						bool collectTrivialPropagationTestingConcepts(CConcept* concept, bool negation, CPROCESSINGLIST<TConceptNegationPair>& trivialConceptTestingList, CCalculationAlgorithmContext* calcAlgContext);


					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKCLASSIFICATIONMESSAGEANALYSER_H
