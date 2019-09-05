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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGVARIABLECOMPOSITIONSITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGVARIABLECOMPOSITIONSITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexVariableCompositionItemVariableExpressionMapping.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "CRequirementWaitingDependencyData.h"
#include "CComplexQueryMaterializationData.h"
#include "CComplexQueryFinishingBuildingVariableCompositionsItemData.h"



// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Parser/Expressions/CIndividualVariableExpression.h"
#include "Parser/Expressions/CObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CDataLiteralVariableExpression.h"
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataPropertyAssertionExpression.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexBuildingVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingVariableCompositionsItem : public CRequirementWaitingDependencyData {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingVariableCompositionsItem(CComplexQueryProcessingData* queryProcessingData);
					~COptimizedComplexBuildingVariableCompositionsItem();



					COptimizedComplexBuildingVariableCompositionsItem* createExtendingBuildingVariableCompositionsItem();





					CComplexQueryMaterializationData* getMaterializationData();
					COptimizedComplexBuildingVariableCompositionsItem* setMaterializationData(CComplexQueryMaterializationData* tmpOnto);


					CComplexQueryFinishingBuildingVariableCompositionsItemData* getBuildingFinishingData();
					COptimizedComplexBuildingVariableCompositionsItem* setBuildingFinishingData(CComplexQueryFinishingBuildingVariableCompositionsItemData* tmpOnto);



					QHash<CExpressionVariable*, CBuildExpression*>* getVariableClassTermExpressionHash();
					COptimizedComplexBuildingVariableCompositionsItem* setVariableClassTermExpressionHash(const QHash<CExpressionVariable*, CBuildExpression*>& hash);


					COptimizedComplexBuildingVariableCompositionsItem* addVariableComplexConceptItem(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem);
					COptimizedComplexBuildingVariableCompositionsItem* addVariablePropertyAssertion(CIndividualVariableExpression* varExp, CObjectPropertyAssertionExpression* propAssExp);

					COptimizedComplexBuildingVariableCompositionsItem* addVariablePropertyAssertions(const QList<CObjectPropertyAssertionExpression*>& propAssExpList);


					QSet<CIndividualVariableExpression*>* getRemainingVariableExpressionSet();

					QList<CObjectPropertyAssertionExpression*> getUnhanledPropertyAssertionsExpressions(CIndividualVariableExpression* varExp);






					bool isWaitingComputationStep();
					COptimizedComplexBuildingVariableCompositionsItem* setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep);
					COptimizedComplexBuildingVariableCompositionsItem* setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep);

					CComplexQueryProcessingData* getQueryProcessingData();


					bool isInitializedBaseConceptVariableItems();
					COptimizedComplexBuildingVariableCompositionsItem* setInitializedBaseConceptVariableItems(bool initialized);


					QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* getVariableConceptItemHash();

					QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* getVariableExpressionVariableCompositionItemHash();



					bool isRemovingPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss);
					bool isRemovingDataPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CDataPropertyAssertionExpression* dataPropAss);

					COptimizedComplexBuildingVariableCompositionsItem* setPropertyAssertionHandled(CObjectPropertyAssertionExpression* propAss);
					COptimizedComplexBuildingVariableCompositionsItem* setVariableExpressionHandled(CIndividualVariableExpression* varEpx);
					COptimizedComplexBuildingVariableCompositionsItem* setVariableExpressionUnhandled(CIndividualVariableExpression* varEpx);


					CIndividualVariableExpression* getOtherVariableExpression(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss);




					bool hasWaitingVariableCompositionItems();
					cint64 getVariableCompositionItemWaitingCount();
					COptimizedComplexBuildingVariableCompositionsItem* incVariableCompositionItemWaitingCount(cint64 incCount = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decVariableCompositionItemWaitingCount(cint64 decCount = 1);



					bool isWaitingSubVariableBuildingItems();
					cint64 getWaitingSubVariableBuildingItemCount();
					COptimizedComplexBuildingVariableCompositionsItem* incWaitingSubVariableBuildingItemCount(cint64 incCount = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decWaitingSubVariableBuildingItemCount(cint64 decCount = 1);




					QList<COptimizedComplexVariableCompositionItem*>* getComputeVariableMappingItemList();
					COptimizedComplexBuildingVariableCompositionsItem* addComputeVariableMappingItem(COptimizedComplexVariableCompositionItem* item);


					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* getVariableItemIndexMappingHash();
					COptimizedComplexVariableCompositionItemVariableExpressionMapping& getVariableItemIndexMapping(CExpressionVariable* varExp);


					bool isProcessingQueued();
					COptimizedComplexBuildingVariableCompositionsItem* setProcessingQueued(bool queued);


					bool isWaitingRoleTargetsRealization();
					cint64 getWaitingRoleTargetsRealizationCount();
					COptimizedComplexBuildingVariableCompositionsItem* incWaitingRoleTargetsRealizationCount(cint64 count = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decWaitingRoleTargetsRealizationCount(cint64 count = 1);



					bool isWaitingVariableBindingsPropagation();
					cint64 getWaitingVariableBindingsPropagationCount();
					COptimizedComplexBuildingVariableCompositionsItem* incVariableBindingsPropagationCount(cint64 count = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decVariableBindingsPropagationCount(cint64 count = 1);


					bool isWaitingVariableBindingsConfirmation();
					cint64 getWaitingVariableBindingsConfirmationCount();
					COptimizedComplexBuildingVariableCompositionsItem* incVariableBindingsConfirmationCount(cint64 count = 1);
					COptimizedComplexBuildingVariableCompositionsItem* decVariableBindingsConfirmationCount(cint64 count = 1);



					bool isWaitingComputation();



					CExpressionVariable* getLastHandledVariableExpression();
					COptimizedComplexBuildingVariableCompositionsItem* setLastHandledVariableExpression(CExpressionVariable* varExp);



					QSet<CIndividualVariableExpression*>* getReuseVariableExpressionComputationsCheckSet();


					QSet<CExpressionVariable*>* getAllVariableSet();
					COptimizedComplexBuildingVariableCompositionsItem* setAllVariableSet(const QSet<CExpressionVariable*>& varSet);


					QSet<CExpressionVariable*>* getDistinguishedVariableSet();
					COptimizedComplexBuildingVariableCompositionsItem* setDistinguishedVariableSet(const QSet<CExpressionVariable*>& varSet);


					QSet<CExpressionVariable*>* getReductionDeniedVariableSet();
					COptimizedComplexBuildingVariableCompositionsItem* setReductionDeniedVariableSet(const QSet<CExpressionVariable*>& varSet);



					COptimizedComplexBuildingVariableCompositionsItem* addBasicDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList);
					QList<CDataPropertyAssertionExpression*>* getBasicDataPropertyAssertionList();
					QSet<CDataLiteralVariableExpression*>* getBasicDataVariableJoiningSet();
					bool isInitializedBasicDataVariableItems();
					COptimizedComplexBuildingVariableCompositionsItem* setInitializedBasicDataVariableItems(bool initialized);

					bool isJoinedBasicDataVariableItems();
					COptimizedComplexBuildingVariableCompositionsItem* setJoinedBasicDataVariableItems(bool initialized);


					COptimizedComplexBuildingVariableCompositionsItem* addVariableDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList);
					COptimizedComplexBuildingVariableCompositionsItem* addVariableDataPropertyAssertion(CIndividualVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp);
					COptimizedComplexBuildingVariableCompositionsItem* addVariableDataPropertyAssertion(CDataLiteralVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp);

					QList<CDataPropertyAssertionExpression*> getUnhanledDataPropertyAssertionsExpressions(CIndividualVariableExpression* varExp);
					QList<CDataPropertyAssertionExpression*> getUnhanledDataPropertyAssertionsExpressions(CDataLiteralVariableExpression* varExp);
					QList<CIndividualVariableExpression*> getUnhandledDataExtensionIndividualTermExpressions(CDataPropertyAssertionExpression* propAssExp, CIndividualVariableExpression* varExp);
					COptimizedComplexBuildingVariableCompositionsItem* setDataPropertyAssertionHandled(CDataPropertyAssertionExpression* propAss);




					QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* getDataLiteralVariableIndidualVariableMappingHash();
					QHash<CIndividualVariableExpression*, QHash<CDataLiteralVariableExpression*, CDataPropertyTermExpression*>>* getIndidualVariableDataLiteralVariableRoleMappingHash();

					bool isInitializedConceptDataVariableExtensionItems();
					COptimizedComplexBuildingVariableCompositionsItem* setInitializedConceptDataVariableExtensionItems(bool initialized);

					COptimizedComplexBuildingVariableCompositionsItem* addIndidualVariableDataLiteralVariableRoleRequiredExtension(CIndividualVariableExpression* indiVar, CDataLiteralVariableExpression* dataLitVar, CDataPropertyTermExpression* dataRole);

					QSet<CIndividualVariableExpression*>* getRemainingExtendingDataVariableSet();



					COptimizedComplexBuildingVariableCompositionsItem* getAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem();
					COptimizedComplexBuildingVariableCompositionsItem* setAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem(COptimizedComplexBuildingVariableCompositionsItem* subItem);

					bool hasAbsorptionBasedQueryPartsOrdinaryEvaluated();
					COptimizedComplexBuildingVariableCompositionsItem* setAbsorptionBasedQueryPartsOrdinaryEvaluated(bool ordnaryEvaluated);

					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbingQueryPartsList();
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbingQueryParts(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbingQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbedQueryPartItemExtensionHandlingList();
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbedBasedQueryPartsItemExtensionHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbedBasedQueryPartItemExtensionHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbedQueryPartEntailmentCheckingHandlingList();
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbedBasedQueryPartsEntailmentCheckingHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorbedBasedQueryPartEntailmentCheckingHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);



					COptimizedComplexVariableCompositionItem* getVariableLastCompositionItem(CExpressionVariable* variableExp);
					CExpressionVariable* getVariableLastCompositionItemAssociatedVariableExpression(CExpressionVariable* variableExp);

					COptimizedComplexBuildingVariableCompositionsItem* updateLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp, COptimizedComplexVariableCompositionItem* lastHandledItem);
					COptimizedComplexBuildingVariableCompositionsItem* clearLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp);


					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* getAbsorptionBasedHandlingExtensionItemList();
					COptimizedComplexBuildingVariableCompositionsItem* addAbsorptionBasedHandlingExtensionItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem);


					bool isSatisfiable();
					COptimizedComplexBuildingVariableCompositionsItem* setSatisfiability(bool satisfiable);


					QSet<CExpressionVariable*>* getVariableSingleCardinalitySet();
					bool hasVariableSingleCardinality(CExpressionVariable* varExp);



					bool isBindingsReducible();
					COptimizedComplexBuildingVariableCompositionsItem* setBindingsReducible(bool reducible);


					QList<COptimizedComplexVariableCompositionItem*>* getUsedComplexVariableCompositionItemList();
					COptimizedComplexBuildingVariableCompositionsItem* addUsedComplexVariableCompositionItem(COptimizedComplexVariableCompositionItem* item);


				// protected methods
				protected:

				// protected variables
				protected:

					QHash<CExpressionVariable*, CBuildExpression*> mVariableClassTermExpressionHash;


					QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*> mVarVarCompItemHash;

					QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*> mVarConItemHash;

					QHash<CIndividualVariableExpression*, CObjectPropertyAssertionExpression*> mVarPropAssHash;

					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping> mVarItemIndexMappingHash;

					QSet<CIndividualVariableExpression*> mRemainingVarExpSet;
					QSet<CIndividualVariableExpression*> mReuseVarExpComputationsCheckSet;
					QSet<CIndividualVariableExpression*> mHandledVarExpSet;
					QSet<CObjectPropertyAssertionExpression*> mHandledPropAssSet;

					cint64 mVarCompItemWaitingCount;
					cint64 mSubVarBuildItemWaitingCount;
					QList<COptimizedComplexVariableCompositionItem*> mComputeMappingItemList;

					bool mAbsorptionBasedQueryPartsOrdinaryEvaluated;
					COptimizedComplexBuildingVariableCompositionsItem* mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem;
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorbingQueryPartsList;


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorptionBasedQueryPartItemExtensionHandlingList;
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorptionBasedQueryPartEntailmentCheckingHandlingList;

					bool mInitializedBaseConceptVariableItems;


					cint64 mWaitingComputationStepsCount;
					cint64 mWaitingVariableBindingsPropagationCount;
					cint64 mWaitingVariableBindingsConfirmationCount;

					CComplexQueryProcessingData* mQueryProcessingData;

					bool mProcessingQueued;

					cint64 mWaitingRoleTargetsRealizationCount;

					CExpressionVariable* mLastHandVarExp;


					QSet<CExpressionVariable*> mAllVarExpSet;
					QSet<CExpressionVariable*> mDistinguishedVarExpSet;
					QSet<CExpressionVariable*> mReductionDeniedVariableSet;


					QList<CDataPropertyAssertionExpression*> mBasicDataPropAssList;
					QSet<CDataLiteralVariableExpression*> mBasicDataVarJoiningSet;
					bool mInitializedBasicDataVariableItems;
					bool mJoinedBasicDataVariableItems;



					QHash<CIndividualVariableExpression*, CDataPropertyAssertionExpression*> mVarDataPropAssHash;
					QHash<CDataLiteralVariableExpression*, CDataPropertyAssertionExpression*> mDataVarDataPropAssHash;
					QSet<CDataPropertyAssertionExpression*> mHandledDataPropAssSet;


					QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*> mDataVarIndiVarHash;
					QHash<CIndividualVariableExpression*, QHash<CDataLiteralVariableExpression*, CDataPropertyTermExpression*>> mIndiVarDataVarHash;

					bool mInitializedConceptDataVariableExtensionItems;
					QSet<CIndividualVariableExpression*> mRemainingDataVarExpSet;



					QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*> mVarLastItemHash;
					QHash<CExpressionVariable*, CExpressionVariable*> mVarLastItemAssociatedVariableHash;


					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*> mAbsorptionBasedHandlingExtensionItemList;

					bool mSatisfiable;
					bool mBindingsReducible;

					QSet<CExpressionVariable*> mVariableSingleCardinalitySet;

					QList<COptimizedComplexVariableCompositionItem*> mUsedComplexVariableCompositionItemList;




					CComplexQueryMaterializationData* mTemporaryMaterializationData;

					CComplexQueryFinishingBuildingVariableCompositionsItemData* mBuildingFinishingData;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGVARIABLECOMPOSITIONSITEM_H
