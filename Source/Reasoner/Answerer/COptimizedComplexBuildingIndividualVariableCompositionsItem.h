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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALVARIABLECOMPOSITIONSITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALVARIABLECOMPOSITIONSITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexConceptItem.h"
#include "COptimizedComplexVariableCompositionItemVariableExpressionMapping.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "COptimizedComplexBuildingVariableCompositionsItem.h"
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
			 *		\class		COptimizedComplexBuildingIndividualVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingIndividualVariableCompositionsItem : public COptimizedComplexBuildingVariableCompositionsItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingIndividualVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData);
					virtual ~COptimizedComplexBuildingIndividualVariableCompositionsItem();



					COptimizedComplexBuildingIndividualVariableCompositionsItem* createExtendingBuildingVariableCompositionsItem();


					
					CComplexQueryMaterializationData* getMaterializationData();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setMaterializationData(CComplexQueryMaterializationData* tmpOnto);


					CComplexQueryFinishingBuildingVariableCompositionsItemData* getBuildingFinishingData();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setBuildingFinishingData(CComplexQueryFinishingBuildingVariableCompositionsItemData* tmpOnto);




					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariableComplexConceptItem(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariablePropertyAssertion(CIndividualVariableExpression* varExp, CObjectPropertyAssertionExpression* propAssExp);

					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariablePropertyAssertions(const QList<CObjectPropertyAssertionExpression*>& propAssExpList);


					QSet<CIndividualVariableExpression*>* getRemainingVariableExpressionSet();

					QList<CObjectPropertyAssertionExpression*> getUnhanledPropertyAssertionsExpressions(CIndividualVariableExpression* varExp);






					bool isInitializedBaseConceptVariableItems();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setInitializedBaseConceptVariableItems(bool initialized);




					bool isRemovingPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss);
					bool isRemovingDataPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CDataPropertyAssertionExpression* dataPropAss);

					COptimizedComplexBuildingIndividualVariableCompositionsItem* setPropertyAssertionHandled(CObjectPropertyAssertionExpression* propAss);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setVariableExpressionHandled(CIndividualVariableExpression* varEpx);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setVariableExpressionUnhandled(CIndividualVariableExpression* varEpx);


					CIndividualVariableExpression* getOtherVariableExpression(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss);








					bool isWaitingRoleTargetsRealization();
					cint64 getWaitingRoleTargetsRealizationCount();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* incWaitingRoleTargetsRealizationCount(cint64 count = 1);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* decWaitingRoleTargetsRealizationCount(cint64 count = 1);



					bool isWaitingVariableBindingsPropagation();
					cint64 getWaitingVariableBindingsPropagationCount();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* incVariableBindingsPropagationCount(cint64 count = 1);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* decVariableBindingsPropagationCount(cint64 count = 1);


					bool isWaitingVariableBindingsConfirmation();
					cint64 getWaitingVariableBindingsConfirmationCount();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* incVariableBindingsConfirmationCount(cint64 count = 1);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* decVariableBindingsConfirmationCount(cint64 count = 1);



					virtual bool isWaitingComputation();






					QSet<CIndividualVariableExpression*>* getReuseVariableExpressionComputationsCheckSet();


					QSet<CExpressionVariable*>* getAllVariableSet();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setAllVariableSet(const QSet<CExpressionVariable*>& varSet);


					QSet<CExpressionVariable*>* getDistinguishedVariableSet();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setDistinguishedVariableSet(const QSet<CExpressionVariable*>& varSet);


					QSet<CExpressionVariable*>* getReductionDeniedVariableSet();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setReductionDeniedVariableSet(const QSet<CExpressionVariable*>& varSet);



					COptimizedComplexBuildingIndividualVariableCompositionsItem* addBasicDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList);
					QList<CDataPropertyAssertionExpression*>* getBasicDataPropertyAssertionList();
					QSet<CDataLiteralVariableExpression*>* getBasicDataVariableJoiningSet();
					bool isInitializedBasicDataVariableItems();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setInitializedBasicDataVariableItems(bool initialized);

					bool isJoinedBasicDataVariableItems();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setJoinedBasicDataVariableItems(bool initialized);


					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariableDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariableDataPropertyAssertion(CIndividualVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addVariableDataPropertyAssertion(CDataLiteralVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp);

					QList<CDataPropertyAssertionExpression*> getUnhanledDataPropertyAssertionsExpressions(CIndividualVariableExpression* varExp);
					QList<CDataPropertyAssertionExpression*> getUnhanledDataPropertyAssertionsExpressions(CDataLiteralVariableExpression* varExp);
					QList<CIndividualVariableExpression*> getUnhandledDataExtensionIndividualTermExpressions(CDataPropertyAssertionExpression* propAssExp, CIndividualVariableExpression* varExp);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setDataPropertyAssertionHandled(CDataPropertyAssertionExpression* propAss);




					QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* getDataLiteralVariableIndidualVariableMappingHash();
					QHash<CIndividualVariableExpression*, QHash<CDataLiteralVariableExpression*, CDataPropertyTermExpression*>>* getIndidualVariableDataLiteralVariableRoleMappingHash();

					bool isInitializedConceptDataVariableExtensionItems();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setInitializedConceptDataVariableExtensionItems(bool initialized);

					COptimizedComplexBuildingIndividualVariableCompositionsItem* addIndidualVariableDataLiteralVariableRoleRequiredExtension(CIndividualVariableExpression* indiVar, CDataLiteralVariableExpression* dataLitVar, CDataPropertyTermExpression* dataRole);

					QSet<CIndividualVariableExpression*>* getRemainingExtendingDataVariableSet();



					COptimizedComplexBuildingIndividualVariableCompositionsItem* getAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* subItem);

					bool hasAbsorptionBasedQueryPartsOrdinaryEvaluated();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setAbsorptionBasedQueryPartsOrdinaryEvaluated(bool ordnaryEvaluated);

					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbingQueryPartsList();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbingQueryParts(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbingQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbedQueryPartItemExtensionHandlingList();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbedBasedQueryPartsItemExtensionHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbedBasedQueryPartItemExtensionHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* getAbsorbedQueryPartEntailmentCheckingHandlingList();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbedBasedQueryPartsEntailmentCheckingHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list);
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorbedBasedQueryPartEntailmentCheckingHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data);







					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* getAbsorptionBasedHandlingExtensionItemPropagationList();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorptionBasedHandlingExtensionPropagationItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem);



					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* getAbsorptionBasedHandlingExtensionItemJoiningList();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* addAbsorptionBasedHandlingExtensionJoiningItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem);


					QSet<CExpressionVariable*>* getVariableSingleCardinalitySet();
					bool hasVariableSingleCardinality(CExpressionVariable* varExp);



					bool isBindingsReducible();
					COptimizedComplexBuildingIndividualVariableCompositionsItem* setBindingsReducible(bool reducible);




					QList<CExpressionVariable*>* getExtendibleQueryTransferringExtractionVariableList();


				// protected methods
				protected:

				// protected variables
				protected:

					QHash<CIndividualVariableExpression*, CObjectPropertyAssertionExpression*> mVarPropAssHash;


					QSet<CIndividualVariableExpression*> mRemainingVarExpSet;
					QSet<CIndividualVariableExpression*> mReuseVarExpComputationsCheckSet;
					QSet<CIndividualVariableExpression*> mHandledVarExpSet;
					QSet<CObjectPropertyAssertionExpression*> mHandledPropAssSet;


					bool mAbsorptionBasedQueryPartsOrdinaryEvaluated;
					COptimizedComplexBuildingIndividualVariableCompositionsItem* mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem;
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorbingQueryPartsList;


					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorptionBasedQueryPartItemExtensionHandlingList;
					QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*> mAbsorptionBasedQueryPartEntailmentCheckingHandlingList;

					bool mInitializedBaseConceptVariableItems;


					cint64 mWaitingVariableBindingsPropagationCount;
					cint64 mWaitingVariableBindingsConfirmationCount;


					cint64 mWaitingRoleTargetsRealizationCount;



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





					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*> mAbsorptionBasedHandlingExtensionItemPropagationList;
					QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*> mAbsorptionBasedHandlingExtensionItemJoiningList;

					bool mBindingsReducible;

					QSet<CExpressionVariable*> mVariableSingleCardinalitySet;





					CComplexQueryMaterializationData* mTemporaryMaterializationData;

					CComplexQueryFinishingBuildingVariableCompositionsItemData* mBuildingFinishingData;

					QList<CExpressionVariable*> mExtendibleQueryTransferringExtractionVariableList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALVARIABLECOMPOSITIONSITEM_H
