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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALMIXEDVARIABLECOMPOSITIONSITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALMIXEDVARIABLECOMPOSITIONSITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexBuildingVariableCompositionsItem.h"
#include "COptimizedComplexBuildingIndividualVariableCompositionsItem.h"
#include "COptimizedComplexBuildingClassVariableCompositionsItem.h"
#include "COptimizedComplexBuildingPropertyVariableCompositionsItem.h"


// Other includes
#include "Reasoner/Ontology/CRole.h"
#include "Reasoner/Ontology/OntologySettings.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Query/CComplexAssertionsIndividualVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsClassVariablesAnsweringQuery.h"
#include "Reasoner/Query/CComplexAxiomsObjectPropertyVariablesAnsweringQuery.h"

#include "Parser/Expressions/CPropertyVariableExpression.h"
#include "Parser/Expressions/CIndividualVariableExpression.h"
#include "Parser/Expressions/CClassVariableExpression.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Query;

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexBuildingIndividualMixedVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingIndividualMixedVariableCompositionsItem : public COptimizedComplexBuildingVariableCompositionsItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData);
					virtual ~COptimizedComplexBuildingIndividualMixedVariableCompositionsItem();


					virtual bool isWaitingComputation();

					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addIndividualVariableTypeVariableExtension(CIndividualVariableExpression* indiVarExp, CClassVariableExpression* classVarExp);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addIndividualVariablePropertyVariableExtension(CIndividualVariableExpression* indiVarExp, CPropertyVariableExpression* propVarExp);

					QSet<CIndividualVariableExpression*>* getRemainingVariableExpressionSet();
					QSet<CIndividualVariableExpression*>* getAllVariableSet();


					bool isInitializedVariableExtensions();
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setInitializedVariableExtensions(bool initialized);


					bool isIndividualVariableTypeExtensionsProcessed();
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setIndividualVariableTypeExtensionsProcessed(bool processed);


					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addIndividualVariableSubQueryItem(CComplexAssertionsIndividualVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingIndividualVariableCompositionsItem* subQueryItem);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addClassVariableSubQueryItem(CComplexAxiomsClassVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingClassVariableCompositionsItem* subQueryItem);


					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addPropertyVariableSubQueryItem(CComplexAxiomsObjectPropertyVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingPropertyVariableCompositionsItem* subQueryItem);




					QList<CClassVariableExpression*> getIndividualVariableTypeExtensionVariables(CIndividualVariableExpression* extVar);
					COptimizedComplexBuildingClassVariableCompositionsItem* getExtensionClassVariableSubQueryBuildingItem(CClassVariableExpression* extVar);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setExtensionClassVariableSubQueryBuildingItem(CClassVariableExpression* extVar, COptimizedComplexBuildingClassVariableCompositionsItem* item);

					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addIndividualVariableTypeExtension(CIndividualVariableExpression* indiVar, CClassVariableExpression* classVar);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* addIndividualVariablePropertyExtension(CIndividualVariableExpression* indiVar, CPropertyVariableExpression* propVar);
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setExtensionPropertyVariableSubQueryBuildingItem(CPropertyVariableExpression* extVar, COptimizedComplexBuildingPropertyVariableCompositionsItem* item);
					COptimizedComplexBuildingPropertyVariableCompositionsItem* getExtensionPropertyVariableSubQueryBuildingItem(CPropertyVariableExpression* extVar);



					QSet<QPair<CIndividualVariableExpression*, CClassVariableExpression*>>* getIndividualVariableTypeExtensionVariableHandledPairSet();



					QList<CPropertyVariableExpression*> getIndividualVariablePropertyExtensionVariables(CIndividualVariableExpression* extVar);
					QSet<QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>>* getIndividualVariablePropertyExtensionVariableHandledPairSet();


					COptimizedComplexVariableCompositionItem* getLastIndividualVariableCompositionItem();
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setLastIndividualVariableCompositionItem(COptimizedComplexVariableCompositionItem* lastItem);


					COptimizedComplexVariableCompositionItemVariableIndexMapping* getLastIndividualVariableCompositionItemIndexMapping();
					COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* setLastIndividualVariableCompositionItemIndexMapping(COptimizedComplexVariableCompositionItemVariableIndexMapping* indexMapping);



					COptimizedComplexBuildingIndividualVariableCompositionsItem* getIndividualVariableSubQueryBuildingItem();


					QList<COptimizedComplexBuildingClassVariableCompositionsItem*>* getSubQueryClassVariableBuildingItemList();


					QSet<COptimizedComplexVariableCompositionItem*>* getJoiningVariableCompositionItemSet();

				// protected methods
				protected:

				// protected variables
				protected:

					COptimizedComplexBuildingIndividualVariableCompositionsItem* mIndiSubItem;

					QList<COptimizedComplexBuildingVariableCompositionsItem*> mSubItemList;
					QList<COptimizedComplexBuildingClassVariableCompositionsItem*> mSubClassItemList;
					QList<COptimizedComplexBuildingPropertyVariableCompositionsItem*> mSubPropertyItemList;
					QList<CComplexVariablesAnsweringQuery*> mSubQueryList;


					bool mInitializedVariableExtensions;
					bool mIndividualVariableTypeExtensionsProcessed;

					QSet<CIndividualVariableExpression*> mRemainingVarExpSet;
					QSet<CIndividualVariableExpression*> mAllVarExpSet;

					QHash<CIndividualVariableExpression*, CClassVariableExpression*> mIndividualVariableTypeExtensionHash;
					QHash<CIndividualVariableExpression*, CPropertyVariableExpression*> mIndividualVariablePropertyExtensionHash;

					QHash<CExpressionVariable*, COptimizedComplexBuildingVariableCompositionsItem*> mVariableItemHash;
					QHash<CComplexVariablesAnsweringQuery*, COptimizedComplexBuildingVariableCompositionsItem*> mSubQueryBuildingItemHash;

					QSet<QPair<CIndividualVariableExpression*, CClassVariableExpression*>> mHandledIndividualVariableTypeExtensionVariablePairSet;
					QSet<QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>> mHandledIndividualVariablePropertyExtensionVariablePairSet;

					COptimizedComplexVariableCompositionItem* mLastIndiVarCompItem;
					COptimizedComplexVariableCompositionItemVariableIndexMapping* mLastIndiVarCompItemIndexMapping;



					QSet<COptimizedComplexVariableCompositionItem*> mJoiningVarCompItemSet;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGINDIVIDUALMIXEDVARIABLECOMPOSITIONSITEM_H
