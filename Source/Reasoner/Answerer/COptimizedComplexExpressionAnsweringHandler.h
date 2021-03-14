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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONANSWERINGHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONANSWERINGHANDLER_H

// Libraries includes
#include <QList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringHandler.h"
#include "CComplexQueryExpressionProcessingData.h"
#include "COptimizedComplexExpressionOntologyAnsweringItem.h"
#include "CAnsweringMessageDataCalculationCompletedSatisfiable.h"
#include "CAnswererContext.h"
#include "CAnsweringCalculationHandler.h"
#include "CAnsweringMessageDataCalculationAdapter.h"
#include "CAnsweringMessageDataCalculationClassSubsumers.h"
#include "CAnsweringMessageDataCalculationPossibleClassSubsumers.h"
#include "CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass.h"
#include "CAnsweringMessageDataCalculationCompletedClassSubsumption.h"
#include "CAnsweringMessageDataCalculationCompletedSubsumptionSubClass.h"
#include "CAnsweringMessageDataCalculationCompletedInstanceIndividual.h"
#include "CAnsweringMessageDataCalculationCompletedInstanceItem.h"
#include "COptimizedComplexConceptItemVisitor.h"
#include "CAnsweringMessageDataRequirementCompleted.h"
#include "CAnsweringMessageDataRequirementCompletedRealization.h"
#include "CAnsweringMessageDataRequirementCompletedSubClassRealization.h"
#include "CEntailmentQueryProcessingData.h"
#include "CAnsweringMessageDataRequirementCompletedConceptAssertionEntailment.h"
#include "CAnsweringMessageDataRequirementCompletedRoleAssertionEntailment.h"
#include "CAnsweringMessageDataCalculationCompletedInstanceEntailment.h"
#include "COptimizedComplexVariableConceptBaseItem.h"
#include "COptimizedComplexVariableRolePropagationItem.h"
#include "COptimizedComplexVariableRolePropagationJoiningItem.h"
#include "COptimizedComplexVariableRolePropagationReplacementItem.h"
#include "COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler.h"
#include "COptimizedComplexVariableReductionItem.h"
#include "COptimizedComplexVariableJoiningItem.h"
#include "COptimizedComplexBuildingIndividualVariableCompositionsItem.h"
#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"
#include "COptimizedComplexVariableJoiningHash.h"
#include "CEntailmentVariableBuildingItemData.h"
#include "CAnsweringMessageDataRequirementCompletedRoleTargetsRealization.h"
#include "COptimizedComplexVariableDataLiteralExtensionItem.h"
#include "COptimizedComplexVariableDataLiteralBaseItem.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "CAnsweringMessageDataCalculationCompletedVariableBindingPropagations.h"
#include "CAnsweringPropagationSteeringAbsorptionExtensionItemController.h"
#include "CAnsweringPropagationSteeringAbsorptionExtensionItemEntailmentController.h"
#include "CAnsweringMessageDataVariableBindingPropagations.h"
#include "CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation.h"
#include "CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController.h"
#include "CAnsweringMessageDataCalculationCompletedVariableBindingEntailment.h"
#include "CAnsweringPropagationSteeringAbsorptionEntailmentController.h"
#include "CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem.h"
#include "CAnsweringMessageDataInstancePropagationsData.h"
#include "CAnsweringMessageDataInstancePossiblePropagationsData.h"
#include "CAnsweringMessageDataCalculationMaterializedConcepts.h"
#include "CAnsweringMessageDataInstanceCertainPropagationsData.h"
#include "CAnsweringProgress.h"
#include "COptimizedComplexVariableIndividualBindingsCardinality.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"
#include "CAnsweringMessageDataCalculationCompletedQueryMaterialization.h"
#include "COptimizedComplexVariableIndividualMappingsHash.h"
#include "COptimizedComplexVariableIndividualMappingsMultiHash.h"
#include "COptimizedComplexVariableExtractionItem.h"
#include "CAnsweringPropagationSteeringAbsorptionPropagationController.h"
#include "COptimizedComplexBuildingClassVariableCompositionsItem.h"
#include "COptimizedComplexVariableConceptSubSuperItem.h"
#include "COptimizedComplexVariableSubSuperClassesPropagationItem.h"
#include "COptimizedComplexBuildingObjectPropertyVariableCompositionsItem.h"
#include "COptimizedComplexVariableRoleSubSuperItem.h"
#include "COptimizedComplexVariableSubSuperPropertiesPropagationItem.h"
#include "COptimizedComplexBuildingIndividualMixedVariableCompositionsItem.h"
#include "COptimizedComplexVariableIndividualTypeExtensionItem.h"
#include "COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem.h"

// Other includes
#include "Reasoner/Answerer/Composition/CAbstractVariableMappingsCompositionJoinComputator.h"
#include "Reasoner/Answerer/Composition/CSequentialVariableMappingsCompositionJoinComputator.h"
#include "Reasoner/Answerer/Composition/CQtConcurrentVariableMappingsCompositionJoinComputator.h"
#include "Reasoner/Answerer/Composition/CAbstractVariableMappingsCompositionPropagationComputator.h"
#include "Reasoner/Answerer/Composition/CSequentialVariableMappingsCompositionPropagationComputator.h"
#include "Reasoner/Answerer/Composition/CAbstractVariableMappingsCompositionPropagationComputator.h"
#include "Reasoner/Answerer/Composition/CQtConcurrentVariableMappingsCompositionPropagationComputator.h"

#include "Reasoner/Answerer/Conclusion/CAbstractComplexQueryFinishingHandler.h"
#include "Reasoner/Answerer/Conclusion/CSequentialStreamingComplexQueryFinishingHandler.h"
#include "Reasoner/Answerer/Conclusion/CQtConcurrentStreamingComplexQueryFinishingHandler.h"


#include "Context/CContextBase.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"
#include "Reasoner/Ontology/COntologyProcessingConceptRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingRoleRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingIteratorRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingInitializedRealizationRequirement.h"
#include "Reasoner/Ontology/COntologyTextFormater.h"

#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CClassSynsetsResult.h"
#include "Reasoner/Query/CClassSetResult.h"
#include "Reasoner/Query/CIndividualsResult.h"
#include "Reasoner/Query/CIndividualSynsetsResult.h"
#include "Reasoner/Query/CIndividualsResultVisitorGenerator.h"
#include "Reasoner/Query/CIndividualSynsetsResultVisitorGenerator.h"
#include "Reasoner/Query/CIndividualSynsetResultVisitorGenerator.h"
#include "Reasoner/Query/CComplexConceptAnsweringQuery.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CVariableBindingsAnswersListResult.h"
#include "Reasoner/Query/CVariableBindingsListAnswerResult.h"
#include "Reasoner/Query/CVariableBindingStringResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersSetResult.h"
#include "Reasoner/Query/CVariableBindingStringDataResult.h"
#include "Reasoner/Query/CVariableBindingsAnswerResultMapOrdering.h"
#include "Reasoner/Query/CVariableBindingsAnswersOrderedMapResult.h"
#include "Reasoner/Query/CVariableBindingFilteringLiteralComparison.h"
#include "Reasoner/Query/CQueryUnknownEntityReferenceError.h"
#include "Reasoner/Query/CVariableBindingsAnswersStreamingResult.h"
#include "Reasoner/Query/CQueryUnspecifiedStringError.h"
#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringExtendibleQuery.h"
#include "Reasoner/Query/CComplexAxiomsClassVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsObjectPropertyVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsDataPropertyVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsIndividualMixedVariablesAnsweringQuery.h"

#include "Reasoner/Preprocess/CPreProcessContextBase.h"
#include "Reasoner/Preprocess/CRoleChainAutomataTransformationPreProcess.h"
#include "Reasoner/Preprocess/CDataLiteralNormalizerPreProcess.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CConcreteOntologyRebuildingPreparationBuilder.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererBindingPropagationAdapter.h"
#include "Reasoner/Kernel/Task/CSatisfiableTaskAnswererQueryingMaterializationAdapter.h"

#include "Reasoner/Realizer/COptimizedKPSetIndividualItem.h"
#include "Reasoner/Realizer/COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator.h"
#include "Reasoner/Realizer/COptimizedRepresentativeKPSetIntersectionCombinationIterator.h"

#include "Reasoner/Realization/CConceptRealizationInstantiatedToHierarchyNodeVisitor.h"
#include "Reasoner/Realization/CRoleRealizationInstantiatedToHierarchyNodeVisitor.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"
#include "Utilities/CVariantTrible.hpp"
#include "Utilities/CVariantQuad.hpp"

#include "Config/CConfigDataReader.h"


#include "KoncludeSettings.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

#include <iostream>

using namespace std;


namespace Konclude {

	using namespace Config;
	using namespace Context;
	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Revision;
		using namespace Ontology;
		using namespace Generator;
		using namespace Preprocess;
		using namespace Query;
		using namespace Realization;
		using namespace Realizer;

		using namespace Kernel;
		using namespace Task;

		namespace Answerer {

			using namespace Composition;
			using namespace Conclusion;



			typedef QPair<COptimizedComplexConceptItem*, QSet<COptimizedComplexConceptItem*>*> TConceptItemPredecessorSet;
			typedef QPair<CRole*, CIndividual*> TRoleIndividualPair;

			typedef QPair<CIndividualVariableExpression*, double> TRemainingVariableMappingCountSortingItem;
			typedef QPair<CObjectPropertyAssertionExpression*, double> TRemainingVariableCompositionItemObjectPropertySortingItem;

			typedef QPair<cint64, CAxiomExpression*> TDepthTermPair;
			typedef QPair<QPair<cint64, CHierarchyNode*>, QPair<CConcept*, CClassTermExpression*>> TDepthHierarchyNodeConceptClassTermExpressionTuple;

			/*! 
			 *
			 *		\class		COptimizedComplexConceptAnsweringHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexExpressionAnsweringHandler : public CAbstractVariableMappingsCompositionItemRequirementProcessor, public CAnsweringHandler, public CLogDomain {
				// public methods
				public:
					//! Constructor
					COptimizedComplexExpressionAnsweringHandler(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem);


					//! Destructor
					virtual ~COptimizedComplexExpressionAnsweringHandler();


					virtual bool addAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback = nullptr);
					virtual bool canAnsweringComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery);
					virtual bool canAnsweringComplexQuery(bool compositionQuery);


					virtual bool initializeProcessing(CAnswererContext* answererContext);
					virtual bool prepareProcessing(CAnswererContext* answererContext);

					virtual bool continueCalculationCreation(CAnswererContext* answererContext);



					virtual bool processCalculationMessage(CAnsweringMessageData* message, CAnswererContext* answererContext);
					virtual bool processRequirementMessage(CAnsweringMessageData* message, CAnswererContext* answererContext);


					virtual bool isAnsweringFinished(CAnswererContext* answererContext);

					virtual CAnsweringProgress* getAnsweringProgress();

					virtual CAnsweringHandler* addAnsweringStatistics(CAnsweringStatistics* stats, bool lastStoredDiff);
					virtual CAnsweringHandler* storeAnsweringStatistics();

				// protected methods
				protected:
					class CEntailmentSubConceptAxiomHasher : public CVariantQuad<CConcept*, bool, CConcept*, bool> {
					public:
						CEntailmentSubConceptAxiomHasher(CConcept* subConcept, bool subNegation, CConcept* superConcept, bool superNegation) : CVariantQuad(subConcept, subNegation, superConcept, superNegation) {
						}

						CConcept* getSubConcept() {
							return getFirst();
						}

						bool getSubConceptNegation() {
							return getSecond();
						}

						CConcept* getSuperConcept() {
							return getThird();
						}

						bool getSuperConceptNegation() {
							return getFourth();
						}


					};					
					
					class CEntailmentConceptAssertionHasher : public CVariantTrible<CIndividual*, CConcept*, bool> {
					public:
						CEntailmentConceptAssertionHasher(CConcept* concept, bool negation, CIndividual* individual) : CVariantTrible(individual,concept,negation ) {
						}

						CConcept* getConcept() {
							return getSecond();
						}

						bool getNegation() {
							return getThird();
						}

						CIndividual* getIndividual() {
							return getFirst();
						}
					};

					class CEntailmentRoleAssertionHasher : public CVariantTrible<CRole*, CIndividual*, CIndividual*> {
					public:
						CEntailmentRoleAssertionHasher(CRole* role, CIndividual* individualSource, CIndividual* individualDest) : CVariantTrible(role, individualSource, individualDest) {
						}

						CRole* getRole() {
							return getFirst();
						}

						CIndividual* getIndividualSource() {
							return getSecond();
						}

						CIndividual* getIndividualDestination() {
							return getThird();
						}
					};

					class CRoleTargetsRealizationHasher : public CVariantTrible<CRole*, CIndividualReference, bool> {
					public:
						CRoleTargetsRealizationHasher(CRole* role, const CIndividualReference& individualSource, bool inversed) : CVariantTrible(role, individualSource, inversed) {
						}

						CRole* getRole() {
							return getFirst();
						}

						CIndividualReference getIndividualSourceReference() {
							return getSecond();
						}

						bool getInversed() {
							return getThird();
						}
					};
					bool getWorkerCountConcurrentActivation(CCalculationConfigurationExtension* calcConfig, const QString& configName);


					bool createComplexConceptItemCalculation(bool processing, COptimizedComplexConceptStepAnsweringItem* conSatProcStepItem, CAnswererContext* answererContext, COptimizedComplexConceptStepAnsweringItem* conSuperProcStepItem, COptimizedComplexConceptStepAnsweringItem* conSubProcStepItem, COptimizedComplexConceptStepAnsweringItem* conSubRealStepItem, COptimizedComplexConceptStepAnsweringItem* conEqProcStepItem, COptimizedComplexConceptStepAnsweringItem* instancesProcStepItem);

					bool createIndividualMixedVariableCompositionItemsComputation(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext);

					bool createIndividualTypeExtension(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, CClassVariableExpression* typeExtVar, COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiVarCompItemIndexMapping, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* lastIndiVarCompItem, bool processing, CExpressionVariable* lastIndiVarExp, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash);
					bool createIndividualPropertyExtension(COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, CPropertyVariableExpression* typeExtVar, COptimizedComplexVariableCompositionItemVariableIndexMapping* lastIndiVarCompItemIndexMapping, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* lastIndiVarCompItem, bool processing, CExpressionVariable* lastIndiVarExp, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash);

					bool createPropertyVariableCompositionItemsComputation(COptimizedComplexBuildingPropertyVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext);
					bool createClassVariableCompositionItemsComputation(COptimizedComplexBuildingClassVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext);
					bool createIndividualVariableCompositionItemsComputation(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, bool processing, bool& continueProcessingBuildingVarItem, QList<COptimizedComplexVariableCompositionItem *>* compositionVarItemList, CAnswererContext* answererContext);

					bool generateOrdinaryVariableBuiltItemEvaluationFromAbsorptionBasedPart(CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext, QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData *>* absorbingQueryPartsList, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, bool processing);



					bool processCompositionItemRequirements(const QList<COntologyProcessingRequirement*>& reqList, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext);


					COptimizedComplexVariableIndividualBindings* createBindingsForVariableCompositionItems(cint64 bindingSize, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					COptimizedComplexVariableIndividualBindingsCardinality* createBindingsCardinalitesForVariableCompositionItems(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* createBindingsLinkerForVariableCompositionItems(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinality, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);



					void computeVariableCompositionItemDataLiteralExtension(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);

					void configureDependentComputationIncreaseFactor(bool dependentItemRescheduled, bool mappingProcessed, COptimizedComplexVariableCompositionItem* compVarItem);

					void computeVariableCompositionItemAbsorptionBasedExtension(COptimizedComplexVariableCompositionItem* compVarItem, bool &processing, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem);
					void computeVariableCompositionItemFromDataLiteralBase(COptimizedComplexVariableCompositionItem* compVarItem, CAnswererContext* answererContext, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemFromConceptItemBase(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, bool& processing);
					void computeVariableCompositionItemReduction(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemExtraction(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);


					void computeVariableCompositionItemFromConceptItemSubSuperCLasses(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemFromRoleSubSuperProperties(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemSubSuperClassesPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemSubSuperPropertiesPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem);
					void computeVariableCompositionItemIndividualTypeExtension(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool& processing, bool& subBuildItemProcessing);
					void computeVariableCompositionItemIndividualPropertyExtension(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* buildingVarItem, bool& processing, bool& subBuildItemProcessing);


					bool expectedUpdatedReschedulingDependentItemComputation(COptimizedComplexVariableCompositionItemDependence* baseItemDep, COptimizedComplexVariableIndividualMappings* reducedVarMapping, COptimizedComplexVariableCompositionItem* reductionItem, COptimizedComplexVariableCompositionItem* baseItem, COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexBuildingVariableCompositionsItem* rescheduleBuildingVarItem = nullptr);

					COptimizedComplexExpressionAnsweringHandler* computeVariableCompositionItemPropagation(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing);
					COptimizedComplexExpressionAnsweringHandler* computeVariableCompositionItemJoin(COptimizedComplexVariableCompositionItem* compVarItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, bool& processing);



					QList<CAxiomExpression*> eliminateRedundantQueryTerms(const QList<CAxiomExpression*>& assExps, CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext);
					QList<CExpressionVariable*> getRedundantlyEmbeddedVariables(QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext);
					bool tryEmbedVariableRestrictions(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext);
					bool tryEmbedVariablesObjectPropertyAssertionList(CExpressionVariable* varTesting, CExpressionVariable* varEmbedding, const QList<CObjectPropertyAssertionExpression*>& remainingEmbedObjPropList, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeObjectPropAssHash, const QHash<CRolePropertiesHierarchyNode*, CObjectPropertyAssertionExpression*>& embeddingHierNodeInversObjectPropAssHash, QHash<CExpressionVariable*, CExpressionVariable*>& varExpEmbeddingHash, const QHash<CExpressionVariable*, CAxiomExpression*>& varExpAxiomExpHash, const QSet<CExpressionVariable*>& anonymousIndiVariableSet, const QSet<CExpressionVariable*>& answerIndiVariableSet, CAnswererContext* answererContext);


					
					COptimizedComplexExpressionAnsweringHandler* readConfig();
					COptimizedComplexExpressionAnsweringHandler* initializeQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, CAnswererContext* answererContext);

					CObjectPropertyTermExpression* getInverseResolvedObjectPropertyTermExpression(CObjectPropertyTermExpression* exp, bool& inversed);
					CRole* getInverseResolvedObjectPropertyRole(CObjectPropertyTermExpression* exp);
					CRole* getResolvedDataPropertyRole(CDataPropertyTermExpression* exp);

					COptimizedComplexBuildingObjectPropertyVariableCompositionsItem* initComplexAxiomsObjectPropertyVariablesQuery(CComplexAxiomsObjectPropertyVariablesAnsweringQuery* compAxPropVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing);
					void initComplexAxiomsDataPropertyVariablesQuery(CComplexAxiomsDataPropertyVariablesAnsweringQuery* compAxPropVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing);
					COptimizedComplexBuildingClassVariableCompositionsItem* initComplexAxiomsClassVariablesQuery(CComplexAxiomsClassVariablesAnsweringQuery* compAxClassVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* initComplexAssertionsIndividualVariablesQuery(CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing, bool forceBuildingItem = false);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* initComplexAxiomsIndividualMixedVariablesQuery(CComplexAxiomsIndividualMixedVariablesAnsweringQuery* compAxIndMixedVarQuery, CAnswererContext* answererContext, CComplexQueryExpressionProcessingData* queryProcessingData, bool &queryCompleted, CConcreteOntology* testingOnto, bool &queryProcessing);

					void createIndividualMixedVariableConnectedSplitSubQueries(QSet<CExpressionVariable *> extensionClassVarExpSet, QList<CAxiomExpression *> subQueryClassAxExps, QSet<CExpressionVariable *> &disVarSet, QSet<CExpressionVariable *> &extensionIndiVarExpSet, function<void(QSet<CAxiomExpression *> nextSubQueryClassAxExps, QList<CExpressionVariable *> subQueryDisClassVarList, QSet<CExpressionVariable *> handledVarExpSet)> classSubQueryHandlerFunc);


					QSet<TConceptNegPair>* getRoleDomainRangeImpliedConceptSet(CRole* role, bool domain, CAnswererContext* answererContext);
					bool isConceptRoleDomainRangeImplied(CConcept* concept, bool negation, CRole* role, bool domain, CAnswererContext* answererContext);

					bool areConceptItemConceptsImpliedByPropertyDomains(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem, const QList<CObjectPropertyAssertionExpression*>& propAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext);


					bool checkExistentiallyOccuringRoles(CAnswererContext* answererContext);
					bool tryAnonymousToIndividualVariablesConversion(QSet<CExpressionVariable *> &anonymousIndiVariableSet, QSet<CExpressionVariable *> &varExpSet, bool distinct, CAnswererContext* answererContext, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, bool& allAnonymousVariables, QSet<CExpressionVariable*>& convertedVariableSet);

					bool finishConceptAndAbsorptionItemsGeneration(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *>& rolledVarExpSet, const QSet<CExpressionVariable *>& anonymousIndiVariableSet, const QHash<CExpressionVariable *, CBuildExpression *>& rebuiltVarClassTermExp, const QHash<CExpressionVariable *, CAxiomExpression *>& varExpAxiomExpHash,
						bool allAnonymousVariables, const QSet<CExpressionVariable *>& initialAnonymousIndiVariableSet, const QList<CDataPropertyAssertionExpression *>& bothVarDataPropAssPropagationList, const QList<CObjectPropertyAssertionExpression *>& bothVarPropAssPropagationList, const QList<CDataPropertyAssertionExpression *>& onlyDataVarDataPropAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext);

					bool schedulingConceptItemsProcessingForBuildItem(const QList<QPair<CIndividualVariableExpression *, TConceptNegPair>> &varConNegInstTestExpList, CComplexAssertionsIndividualVariablesAnsweringQuery* compAssIndVarQuery, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem);

					QList<QPair<CIndividualVariableExpression*, TConceptNegPair>> upgradeExistTopToIndividualTriggers(QList<QPair<CIndividualVariableExpression *, CClassTermExpression *>> varClassInstTestExpList, QHash<CExpressionVariable *, CBuildExpression *> individualTriggerUpdatableHash, CAnswererContext* answererContext);

					bool identifyNonInstantiation(const QList<CAxiomExpression *>& assExps, const QSet<CExpressionVariable *> &answerIndiVariableSet, CAnswererContext* answererContext);


					bool checkNonTrivialAbsorptionPropagationExpression(CBuildExpression* expression);
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData *> generateAbsorptionBasedQueryParts(const QSet<CExpressionVariable *> &anonymousIndiVariableSet, bool allAnonymousVariables, QSet<CExpressionVariable *> &reductionForbiddenVarSet, const QSet<CExpressionVariable *> &rolledVarExpSet,
						const QSet<CExpressionVariable *>& initialAnonymousIndiVariableSet, const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *>& prepareIndiVarSet, const QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash, const QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash);

					QHash<CExpressionVariable*, CBuildExpression*> rebuildVariablesClassTermExpressions(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *> &rolledVarExpSet, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash);


					QList<QPair<CIndividualVariableExpression *, CClassTermExpression *>> generateConceptItemExpressions(const QSet<CExpressionVariable *>& varExpSet, const QSet<CExpressionVariable *> &rolledVarExpSet, const QSet<CExpressionVariable *> &anonymousIndiVariableSet, const QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash,
						const QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, QHash<CExpressionVariable *, CBuildExpression *> &individualTriggerUpdatableHash, QSet<CExpressionVariable *> &prepareIndiVarSet, CComplexQueryExpressionProcessingData* queryProcessingData);

					void expressionsRollingUp(QSet<CExpressionVariable *> &anonymousIndiVariableSet, QSet<CExpressionVariable *> &rolledVarExpSet, QHash<CExpressionVariable *, CAxiomExpression *> &varExpAxiomExpHash, CConcreteOntology* testingOnto, QHash<CExpressionVariable *, CBuildExpression *> &varRolledUpClassExpHash);

					void absorbExistentialQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data, CAnswererContext* answererContext);
					CConcept* getAbsorbExistentialQueryPartRestrictedTopPropagationConcept(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data, CAnswererContext* answererContext);

					bool checkUsesQueryOnlyKnownEntities(CConcreteOntology* expressionOntology);


					bool initializeComplexConceptQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CConcept* testingConcept, bool conceptNegation, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem);
					bool initializeComplexConceptQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CClassTermExpression* classTermExpOfInt, bool satisfiableComputationRequired, bool superClassNodesComputationRequired, bool subClassNodesComputationRequired, bool equivalentClassNodesComputationRequired, bool subClassRealizationRequired, cint64 instanceComputationRequiredCount, COptimizedComplexConceptItem** assocConceptItem);



					bool checkHasNewTopRolePropagation(CConcept* testingConcept, bool testingNegation);

					bool initializeQueryProcessingStep(CComplexConceptStepComputationProcess* compStep, COptimizedComplexConceptItem* conceptItem, CComplexQueryExpressionProcessingData* queryProcessingData = nullptr, COptimizedComplexBuildingVariableCompositionsItem* buildVarItem = nullptr);

					bool queueConceptItemProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep);
					bool requeueConceptItemNextProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep);

					bool finishQueryProcessingAsUnsatisfaible(CComplexQueryExpressionProcessingData* queryProcessingData);
					bool finishQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData);
					bool failQueryProcessing(CComplexQueryExpressionProcessingData* queryProcessingData, CQueryError* queryError);

					bool createSatisfiabilityTest(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);
					bool createSuperClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext);
					bool createSubClassSubsumptionTest(COptimizedComplexConceptItem* conceptItem, CHierarchyNode* testingNode, CAnswererContext* answererContext);
					bool createInvidiualInstanceTest(COptimizedComplexConceptItem* conceptItem, const CIndividualReference& testingIndiRef, CAnswererContext* answererContext);
					bool createInvidiualItemInstanceTest(COptimizedComplexConceptItem* conceptItem, const CRealizationIndividualInstanceItemReference& testingItemRef, CAnswererContext* answererContext);
					bool createCandidatePropagationInstanceTest(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CAnswererContext* answererContext);

					bool canSplitActivationIndividualPropagation(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CAnswererContext* answererContext);
					CConcept* getSplitActivationIndividualPropagationConcept(COptimizedComplexConceptItem* conceptItem, CConceptOfInterestActivationTriggeringData activationTriggeringData, CVariable*& propVar, double& splitCompSize, CAnswererContext* answererContext);
					double getNextRoleActivationPropagationMultiplicationEstimation(double currentExpectedCount, const QSet<CRealizationIndividualInstanceItemReference>& currentSample, CConcept* currentPropConcept, QSet<CRealizationIndividualInstanceItemReference>& nextSample, CConcept*& nextPropagationConcept, CAnswererContext* answererContext);

					CConcreteOntology* createQueryMaterializationData(QSet<CExpressionVariable *> varExpSet, QSet<CExpressionVariable *> rolledVarExpSet, QSet<CExpressionVariable *> anonymousIndiVariableSet, QHash<CExpressionVariable *, CBuildExpression *> rebuiltVarClassTermExp, QHash<CExpressionVariable *, CAxiomExpression *> varExpAxiomExpHash,
						bool allAnonymousVariables, QSet<CExpressionVariable *> initialAnonymousIndiVariableSet, QList<CDataPropertyAssertionExpression *> bothVarDataPropAssPropagationList, QList<CObjectPropertyAssertionExpression *> bothVarPropAssPropagationList, QList<CDataPropertyAssertionExpression *> onlyDataVarDataPropAssList, COptimizedComplexBuildingIndividualVariableCompositionsItem* varBuildItem, CAnswererContext* answererContext);

					bool createQueryMaterializationTest(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext);

					bool visitAllIndividuals(function<bool(const CIndividualReference& indiRef)> visitFunc);


					bool continueCalculationJobFromNondeterministicCachedGraph(CSatisfiableCalculationJob* satCalcJob, CAnswererContext* answererContext);


					bool createVariableBindingPropagationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem);

					CAnsweringSplitPropagationTestControllingItem* createSplitPropagationTestControllingItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, QSet<CIndividualReference>* splitIndiSet, bool initializeBindingSets = true);

					bool createVariableBindingConfirmationTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker, CSameRealization* sameRealization, CAnswererContext* answererContext);
					bool createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);
					bool createAbsorbedQueryPartEntailmentTest(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool &continueProcessingBuildingVarItem);


					bool processCalculationJob(CAnswererContext* answererContext, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageDataCalculationCompleted* completedMessage);
					bool processRequirements(CAnswererContext* answererContext, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CAnsweringMessageDataRequirementCompleted* completedMessage);



					QList<CIndividualVariableExpression*> getSortedRemainingVariableExpressionList(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, QSet<CIndividualVariableExpression*>* remainVarExpSet);
					QList<CObjectPropertyAssertionExpression*> getVariableRemainingSortedObjectPropertyList(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, const QList<CObjectPropertyAssertionExpression*>& propAssList);

					void getVariableCompositionItemPropertyAssertionHandlingScores(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* remainVarExp, COptimizedComplexVariableCompositionItem* minJoiningVarItem, CObjectPropertyAssertionExpression* propAssExp, double minExpectedBindingSize, double &propInOutModificationFactor, double &propTotalScore);

					double getVariableCompositionItemRoleSampledInOutRatio(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, CExpressionVariable* otherVarExp, COptimizedComplexVariableCompositionItem* varItem, CRole* outRole, CRole* inRole, CAnswererContext* answererContext);
					cint64 getVariableCompositionItemVariablePosition(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varItem);
					double getVariableCompositionItemRoleAverageSampledOutCount(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varItem, CRole* role, CAnswererContext* answererContext);


					bool processVariableBindingsPropagationItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingPropagations* message, bool computationError, CAnswererContext* answererContext);
					bool processVariableBindingsConfirmationCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation* message, CAnswererContext* answererContext);
					bool processVariableBindingsEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedVariableBindingEntailment* message, CAnswererContext* answererContext);
					bool processSatisfiableCalculationCompleted(CAnsweringMessageDataCalculationCompletedSatisfiable* message, CAnswererContext* answererContext);
					bool processSuperClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSuperClass* message, CAnswererContext* answererContext);
					bool processSubClassSubsumptionCalculationCompleted(CAnsweringMessageDataCalculationCompletedSubsumptionSubClass* message, CAnswererContext* answererContext);
					bool processIndividualInstanceCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceIndividual* message, CAnswererContext* answererContext);
					bool processInstanceEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceEntailment* message, CAnswererContext* answererContext);
					bool processClassSumsumptionEntailmentCalculationCompleted(CAnsweringMessageDataCalculationCompletedClassSubsumption* message, CAnswererContext* answererContext);
					bool processIndividualItemCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceItem* message, CAnswererContext* answererContext);
					bool processIndividualCandidatePropagationCalculationCompleted(CAnsweringMessageDataCalculationCompletedInstanceCandidatePropagationItem* message, CAnswererContext* answererContext);
					bool processQueryMaterializationCalculationCompleted(CAnsweringMessageDataCalculationCompletedQueryMaterialization* message, CAnswererContext* answererContext);
					bool processExtractedClassSubsumers(CAnsweringMessageDataCalculationClassSubsumers* message, CAnswererContext* answererContext);
					bool processExtractedPossibleClassSubsumers(CAnsweringMessageDataCalculationPossibleClassSubsumers* message, CAnswererContext* answererContext);
					bool processExtractedVariableBindingPropagations(CAnsweringMessageDataVariableBindingPropagations* variableBindingsPropagationMessage, CAnswererContext* answererContext);
					bool processExtractedInstanceCertainPropagations(CAnsweringMessageDataInstanceCertainPropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext);
					bool processExtractedInstanceCandidatePossiblePropagations(CAnsweringMessageDataInstancePossiblePropagationsData* instanceCandidatePropagationMessage, CAnswererContext* answererContext);
					bool processExtractedMaterializationConcept(CAnsweringMessageDataCalculationMaterializedConcepts* matConceptsMessage, CAnswererContext* answererContext);

					CSatisfiableCalculationJob* extendProcessingByTopPropagation(CSatisfiableCalculationJobGenerator& satCalcJobGen, CSatisfiableCalculationJob* satCalcJob, bool hasTopObjectPropertyPropagation, CAnswererContext* answererContext);


					QSet<CHierarchyNode*>* getDirectReducedSuperNodeSet(const QSet<CHierarchyNode*>& superClassesSet);

					bool finishCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext);

					bool notifyWaitingItemData(CComputedItemDataNotificationLinker* notLinker, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext);

					bool updateComputedInstancesCount(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepInstanceComputationProcess* compStep, CAnswererContext* answererContext);
					bool queueComputedInstancesCacheMemoryConsumptionUpdate(COptimizedComplexConceptItem* conceptItem);


					bool extractAtomicSubsumerExpressions(COptimizedComplexConceptItem* conceptItem);
					bool reorderConceptItemParentAddition(COptimizedComplexConceptItem* conceptItem, COptimizedComplexConceptItem* newParentConceptItem);
					bool reorderConceptClassItems();

					bool searchTrivialSuperSubConceptItems(COptimizedComplexConceptItem* conceptItem);
					bool isTriviallySubsumedConceptItem(COptimizedComplexConceptItem* subsumedConceptItem, COptimizedComplexConceptItem* subsumerConceptItem);
					bool visitSuperSubConceptItems(COptimizedComplexConceptItem* startConceptItem, bool visitSubItems, bool visitSuperItems, COptimizedComplexConceptItemVisitor* visitor);


					bool searchSuperSubSatisfiabilityResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext);
					bool searchSuperSubSuperClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext);
					bool searchSuperSubSubClassesResult(COptimizedComplexConceptItem* startConceptItem, CAnswererContext* answererContext);


					bool finishSubClassCalculationStepProcessing(COptimizedComplexConceptItem* conceptItem, CComplexConceptStepComputationProcess* compStep, CAnswererContext* answererContext);



					QSet<COptimizedComplexConceptItem*>* collectComplexConceptItemPredecessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* predItemSet = nullptr);
					QSet<COptimizedComplexConceptItem*>* collectComplexConceptItemSucessors(COptimizedComplexConceptItem* conceptItem, QSet<COptimizedComplexConceptItem*>* succItemSet = nullptr);


					bool searchKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);
					bool neighboursPrunePossibleInstances(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);


					bool initializeRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);
					bool initializeRealizationInstancesIterators(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext);
					bool initializeKnownPossibleInstances(COptimizedComplexConceptItem* conceptItem, cint64 requiredInstancesCount, CAnswererContext* answererContext);

					bool schedulePossibleInstancesNeighourRealization(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);

					bool checkConceptInstancesCompletelyResolveable(COptimizedComplexConceptItem* conceptItem, QSet<TConceptNegPair>* remainingConNegPair);
					bool checkPossibleInstancesCompletelyHandled(COptimizedComplexConceptItem* conceptItem, CAnswererContext* answererContext);
					TConceptNegPair upgradeExistTopToIndividualTrigger(CClassTermExpression* classTermExp, CExpressionVariable* varTermExp, QHash<CExpressionVariable*, CBuildExpression*>* individualTriggerUpdatableHash, CAnswererContext* answererContext);


					bool checkEntailmentConceptAssertion(CConcept* concept, bool negation, CIndividual* individual, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);
					bool checkEntailmentRoleAssertion(CRole* role, CIndividual* individualSource, CIndividual* individualDest, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);

					bool checkEntailmentConceptSubClassAxiom(CConcept* subConcept, bool subConNegation, CConcept* superConcept, bool superConNegation, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);
					bool checkEntailmentRoleSubObjectPropertyAxiom(CRole* subRole, CRole* superRole, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);
					bool checkEntailmentRoleSubDataPropertyAxiom(CRole* subRole, CRole* superRole, CComplexQueryExpressionProcessingData* procData, CAnswererContext* answererContext);



					QList<QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*>> reuseJoinedVariableCompositionItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp);

					bool finishVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, bool allMappingsComputed);
					bool rescheduleVariableCompositionItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItem* depSchedulingCompItem, cint64 additionalRequiredMappingCount, COptimizedComplexBuildingVariableCompositionsItem* rescheduleBuildingVarItem = nullptr);


					QHash<CExpressionVariable*, cint64> getPropagationReducableVariables(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss);
					QHash<CExpressionVariable*, cint64> getReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* propVarExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping);
					QSet<CExpressionVariable*> getJoinReducableVariables(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* destVarExp, COptimizedComplexVariableCompositionItem* minLeftItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping, COptimizedComplexVariableCompositionItem* maxRightItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping, bool itemSameVarReduction);

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildAbsorbedPartHandlingItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionHanldingQueryPart, QHash<CExpressionVariable *, QSet<COptimizedComplexVariableCompositionItem *> *>* varExpVarComItemHash, bool* processing);
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableReductionItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool allopReductionForVar, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, bool* processing = nullptr);
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildQueryTransferringVariableExtractionItem(COptimizedComplexBuildingVariableCompositionsItem* extractionBuildingVarItem, CExpressionVariable* extractionVarExp, COptimizedComplexVariableCompositionItem* extractionVarCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* extractionItemIndexMapping, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool* processing = nullptr);


					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildJoinedVariableJoiningItems(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool* processing, bool lastComputationItem = false);
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = nullptr, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = nullptr, bool* processing = nullptr, bool lastComputationItem = false);

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* destVarExp, bool itemSameVarReduction, CExpressionVariable* varExp1, COptimizedComplexVariableCompositionItem* joiningItem1, CExpressionVariable* varExp2, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1 = nullptr, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2 = nullptr, bool* processing = nullptr, bool lastComputationItem = false);

					cint64 fillJoiningBindingPositionMapping(COptimizedComplexVariableCompositionItemVariableIndexMapping* minLeftItemIndexMapping, COptimizedComplexVariableJoiningBindingPositionMapping &joiningBindingPositionMapping, COptimizedComplexVariableCompositionItemVariableIndexMapping* maxRightItemIndexMapping, bool itemSameVarReduction, QSet<CExpressionVariable *> &reductionVarExpSet);


					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableSubSuperPropagationItem(COptimizedComplexBuildingPropertyVariableCompositionsItem* buildingVarItem, CPropertyVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CPropertyVariableExpression* otherVariable, bool superPropagation, bool* processing, CAnswererContext* answererContext);
					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableSubSuperPropagationItem(COptimizedComplexBuildingClassVariableCompositionsItem* buildingVarItem, CClassVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CClassVariableExpression* otherVariable, bool superPropagation, bool* processing, CAnswererContext* answererContext);

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, QHash<CExpressionVariable*, cint64>& reductionVarExpIndHash, CObjectPropertyAssertionExpression* propAss = nullptr, bool* processing = nullptr, CAnswererContext* answererContext = nullptr);

					QPair<COptimizedComplexVariableCompositionItem*, COptimizedComplexVariableCompositionItemVariableIndexMapping*> buildVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CDataLiteralVariableExpression* dataVariable, CDataPropertyTermExpression* dataPropTerm = nullptr, bool* processing = nullptr, CAnswererContext* answererContext = nullptr);


					bool tryReuseVariableRolePropagationItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CIndividualVariableExpression* varExp, COptimizedComplexVariableCompositionItem* varCompItem, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping, CObjectPropertyAssertionExpression* propAss = nullptr);

					bool hasVariableJoiningItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, bool itemSameVarReduction, COptimizedComplexVariableCompositionItem* joiningItem1, COptimizedComplexVariableCompositionItem* joiningItem2, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping1, COptimizedComplexVariableCompositionItemVariableIndexMapping* itemIndexMapping2);
					bool queueVariableItemComputation(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);

					bool ensureRoleTargetsRealization(CRole* role, bool inversed, const QSet<CIndividualReference>& individualSourceSet, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext);

					bool canRetrieveInstantiatingDataValueIndividuals(CRole* dataRole, CDataLiteral* dataLiteral);
					QMap<cint64, IndividualIdCandidateData>* getInstantiatingDataValueIndividualIdMap(CRole* dataRole, CDataLiteral* dataLiteral, QMap<cint64, IndividualIdCandidateData>* instantiatingDataValueIndividualIdMap, CAnswererContext* answererContext);
					QSet<CDataLiteral*>* getInstantiatedDataLiterals(const CIndividualReference& indiRef, CRole* dataRole, QSet<CDataLiteral*>* dataLiteralSet, CAnswererContext* answererContext);
					CIndividualProcessNode* getNominalIndividualProcessNode(const CIndividualReference& indiRef, CIndividualProcessNodeVector* indiProcVector, bool* nondeterministicNodeFlag, CAnswererContext* answererContext);
					CIndividualProcessNode* getMergeCorrectedIndividualProcessNode(CIndividualProcessNode* indiProcNode, CIndividualProcessNodeVector* indiProcVector, bool* nonDeterministicallyMergedFlag, CAnswererContext* answererContext);




					QStringList debugGetItemVariableNames(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem);
					bool debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CExpressionVariable* varExp, COptimizedComplexVariableCompositionItem* varCompItem);
					bool debugCheckVariableMappingContainsSolution(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);

					bool debugWriteVariableMappingToFile(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);
					QSet<QString> debugGetVariableMappingStringSet(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, cint64** sortArray = nullptr);
					bool debugCheckVariableMappingFromFile(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);
					QString debugGetVariableMappingsHeaderString(COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, cint64** sortArray = nullptr);

					bool debugWriteVariableMappingToFileWithHeader(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem, const QString& suffix = QString());



					bool installQueryExtendible(CComplexQueryExtendibleData* extendibleData, CComplexQueryExpressionProcessingData* queryProcessingData);




					bool updateAnswersCacheConsumptionForItem(COptimizedComplexConceptItem* varCompItem);
					bool updateAnswersCacheConsumptionForItem(COptimizedComplexVariableCompositionItem* varCompItem);
					bool cleanUpAnswersCache(bool queryProcessingFinished);
					bool cleanUpAnswersCacheItem(COptimizedComplexVariableCompositionItem* varCompItem, cint64& clearedVarCompItemCount);
					bool cleanUpAnswersCacheItem(COptimizedComplexConceptItem* comConItem, cint64& clearedVarCompItemCount);

					bool addUsedVariableCompositionItem(COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableCompositionItem* varCompItem);



					bool queueAnswersCacheItemUpdate(COptimizedComplexVariableCompositionItem* varCompItem);

					bool updateQueuedAnswersCacheItems();

					bool increaseUsageWeight();

					bool startMeasuringVariableCompositionItemComputationCost(COptimizedComplexVariableCompositionItem* varCompItem);
					bool endMeasuringVariableCompositionItemComputationCost(COptimizedComplexVariableCompositionItem* varCompItem);


					bool startMeasuringComplexConceptItemComputationCost(COptimizedComplexConceptItem* compConItem);
					bool endMeasuringComplexConceptItemComputationCost(COptimizedComplexConceptItem* compConItem);

					QString getCacheSizeString(cint64 bytes);

				// protected variables
				protected:
					QList<CComplexQueryExpressionProcessingData*> mPendingQueryProcessingList;
					COptimizedComplexExpressionOntologyAnsweringItem* mOntoAnsweringItem;


					QHash<CEntailmentSubConceptAxiomHasher, CEntailmentQueryProcessingData*> mSubConEntProcDataHash;
					QHash<CEntailmentConceptAssertionHasher, CEntailmentQueryProcessingData*> mConAssEntProcDataHash;
					QHash<CEntailmentRoleAssertionHasher, CEntailmentQueryProcessingData*> mRoleAssEntProcDataHash;
					QHash<CRoleTargetsRealizationHasher, CEntailmentVariableBuildingItemData*> mRoleTargetsProcDataHash;

#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
					QStringList debugCreationStringList;
					QString debugCreationString;
#endif
					cint64 nextBindingsFileId = 0;


					class CInstanceBindingIndividualCollectionVisitor : public CSameRealizationIndividualVisitor {
					public:
						QList<CIndividualReference>* mIndividualReferenceList;

						CInstanceBindingIndividualCollectionVisitor(QList<CIndividualReference>* individualReferenceList) {
							mIndividualReferenceList = individualReferenceList;
						}

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							mIndividualReferenceList->append(indiRef);
							return true;
						}
					};


					class CInstanceBindingIndividualCountingVisitor : public CSameRealizationIndividualVisitor {
					public:
						cint64 individualCount = 0;

						bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization) {
							individualCount++;;
							return true;
						}
					};

					bool mConfConcurrentAnswerGeneration;

					bool mConfAutomaticConcurrentComputationActivation;
					bool mConfConcurrentJoining;
					cint64 mConcurrentJoinComputationTaskCount = 50;
					CAbstractVariableMappingsCompositionJoinComputator* mJoinComputer;

					bool mConfConcurrentPropagation;
					cint64 mConcurrentPropagationComputationTaskCount = 50;
					CAbstractVariableMappingsCompositionPropagationComputator* mPropagationComputer;


					bool mConfInterpretQueriesAsDistinct;

					bool mConfInterpretNonAnswerIndividualVariablesAsAnonymousVariables;
					QString mConfNonAnswerIndividualVariablesAsAnonymousVariablesInterpretingPrefixString;

					bool mConfDebugTestingOntologyWriting;
					bool mConfFailOnUnknownEntity;

					bool mConfDirectPossibleRealizationRequesting;

					bool mConfVariablePreAbsorption;
					bool mConfVariableAbsorptionOverFullQuery;


					bool mConfUnsatisfiableNonInstanceStatisticsChecking;
					bool mConfAnonymousToDistinguishedVariablesConversion;
					bool mConfAnonymousToDistinguishedVariablesOnlyFullConversion;
					bool mConfOccurrenceStatisticsCollection;
					bool mConfRedundantTermReduction;
					bool mConfRedundantlyRestrictedVariablesElimination;
					bool mConfPropagationJoining;
					bool mConfPropagationReplacement;
					bool mConfDebugWriteVariableCompositionItems;
					bool mConfBindingReducingForNonDistinctAnswerQueries;
					bool mConfDetailedMissingEntityReporting;

					bool mConfDistinctUnecessaryVariablesReduction;
					bool mConfAlwaysUnecessaryVariablesReduction;

					cint64 mConfMaximumBatchMappingsComputationSize;
					double mConfBatchMappingsComputationSizeIncreasingFactor;
					cint64 mConfFirstBatchMappingsComputationSize;
					cint64 mConfMinimalMappingsComputationSize;
					double mConfMappingsRepeatedlyInsufficientDependencyComputationIncreasingFactor;
					cint64 mConfMappingsComputationUnlimitedInterpretationSize;
					bool mConfContinueMappingsCompuationWhenResultsNotWriteable;

					bool mConfAllowBindingReduction;
					cint64 mConfMappingSizeBindingReduction;
					bool mConfAllowJoiningBindingReduction;


					cint64 mConfConceptItemExpectedCountSamplingSize;
					double mConfConceptItemExpectedCountSamplingRetrievedPercentageLimit;


					cint64 mConfSamplingBasedJoinMappingSize;

					cint64 mConfMaxVariableBindingPropagationsInitializationIndividualCount;
					cint64 mConfMaxCandidateInstancePropagationsInitializationIndividualCount;

					cint64 mConfQueryEntailmentTestRestrictedTopPropertyPropagationSplitting;

					bool mConfLazyExistentialPartAbsorption;
					bool mConfBooleanQueryExistentialPartOrdinaryEvaluation;


					bool mConfExtendedLogging;

					double mConfConceptItemInstanceCandiateRetrievingSizeIncreaseFactor;
					double mConfConceptItemInstanceCandiateRetrievingMaxSize;

					double mConfSplitVariableMappingPropagationTestMaxIntegrationLimit;
					double mConfSplitVariableMappingPropagationTestInitialVariableIntegrationLimit;
					double mConfSplitVariableMappingPropagationTestInitialTotalIntegrationLimit;
					double mConfSplitVariableMappingPropagationTestIntegrationLimitIncreaseFactor;


					double mConfNextVariablePropagationHandlingReducibleFactor = 0.8;
					bool mConfNextVariablePropagationHandlingReducibleFactorIntegration = true;
					bool mConfNextVariablePropagationHandlingPropertyScoresIntegration = true;
					double mConfNextVariablePropagationHandlingPropertyTotalScoreIntegrationPercent = 5;
					double mConfNextVariablePropagationHandlingPropertyInOutFactorIntegrationPercent = 50;
					bool mConfNextVariablePropagationHandlingPropertyPriorization = true;
					bool mConfVariableCompositionItemRoleSampling = true;
					bool mConfForceAlwaysVariableCompositionItemRoleSampling = false;
					cint64 mConfVariableCompositionItemRoleSamplingCount = 20;
					cint64 mConfVariableCompositionItemRoleSamplingMappingsIncreaseSize = 10000;


					bool mConfFullQueryMaterialization;

					bool mConfNonDeterministicSatisfiableCalculationContinuation;

					bool mConfAllowLastStepSplitComputation;
					cint64 mConfLastStepSplitComputationMappingsRequirement;



					cint64 mConfAnswersCacheMaximumSizeBytes;
					cint64 mConfAnswersCacheMaximumCount;
					double mConfAnswersCacheUsageWeightIncrease;
					double mConfAnswersCacheCostBaseValue = 100;

					bool mConfSkipConceptItemsIntegrationForPropagationOverVariables = true;



					bool mConfAbsorbedIndividualActivationInstanceCandidatePropagationSplitting = true;
					cint64 mConfAbsorbedIndividualActivationInstanceCandidatePropagationDirectSplittingSize = 100;
					cint64 mConfAbsorbedIndividualActivationInstanceCandidatePropagationMinimumSplittingSize = 20;
					cint64 mConfAbsorbedIndividualActivationInstanceCandidatePropagationMaximumSplittingCount = 10000;


					COntologyQueryExtendedConfigDependedPreProcesser* mTestingOntologyPreprocessor;
					CConcreteOntologyRebuildingPreparationBuilder* mTestingOntologyBuilder;

					cint64 mTotalPossibleInstancesCount = 0;
					cint64 mTestedPossibleInstancesCount = 0;
					CAnsweringProgress mAnsweringProgress;
					QTime mAnsweringStartTime;


					CRealizationIndividualSorting mDefaultRealizationSorting;
					CRealizationIndividualSorting mRoleFillerRealizationSorting;
					CAnswererContext* mAnswererContext;

					QString mDebugOneExistTypeOccurRoleNamesString;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXEXPRESSIONANSWERINGHANDLER_H
