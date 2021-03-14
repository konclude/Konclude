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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGCLASSVARIABLECOMPOSITIONSITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGCLASSVARIABLECOMPOSITIONSITEM_H

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
			 *		\class		COptimizedComplexBuildingClassVariableCompositionsItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexBuildingClassVariableCompositionsItem : public COptimizedComplexBuildingVariableCompositionsItem {
				// public methods
				public:
					//! Constructor
					COptimizedComplexBuildingClassVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData);
					virtual ~COptimizedComplexBuildingClassVariableCompositionsItem();



					virtual bool isWaitingComputation();

					COptimizedComplexBuildingClassVariableCompositionsItem* addVariableComplexSuperConceptItem(CClassVariableExpression* varExp, COptimizedComplexConceptItem* conItem);
					COptimizedComplexBuildingClassVariableCompositionsItem* addVariableComplexSubConceptItem(CClassVariableExpression* varExp, COptimizedComplexConceptItem* conItem);

					QSet<CClassVariableExpression*>* getRemainingVariableExpressionSet();
					QSet<CClassVariableExpression*>* getAllVariableSet();

					QList<COptimizedComplexConceptItem*> getVariableSuperConceptItems(CClassVariableExpression* varExp);
					QList<COptimizedComplexConceptItem*> getVariableSubConceptItems(CClassVariableExpression* varExp);

					bool isInitializedBaseConceptVariableItems();
					COptimizedComplexBuildingClassVariableCompositionsItem* setInitializedBaseConceptVariableItems(bool initialized);



					QSet<QPair<CExpressionVariable*, CExpressionVariable*>>* getVariableSubSuperPropagationSet();
					COptimizedComplexBuildingClassVariableCompositionsItem* addVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);
					COptimizedComplexBuildingClassVariableCompositionsItem* removeVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);
					bool hasVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar);

					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> getVariableRemainingSubSuperPropagationSet(CExpressionVariable* varExp);
					bool isRemovingPropagationDisconnecting(CExpressionVariable* varExp, CExpressionVariable* subVar, CExpressionVariable* superVar);

					COptimizedComplexBuildingClassVariableCompositionsItem* setVariableExpressionUnhandled(CClassVariableExpression* varExp);
					COptimizedComplexBuildingClassVariableCompositionsItem* setVariableExpressionHandled(CClassVariableExpression* varExp);


				// protected methods
				protected:

				// protected variables
				protected:
					QHash<CClassVariableExpression*, COptimizedComplexConceptItem*> mVarSuperConItemHash;
					QHash<CClassVariableExpression*, COptimizedComplexConceptItem*> mVarSubConItemHash;


					bool mInitializedBaseConceptVariableItems;

					QSet<CClassVariableExpression*> mRemainingVarExpSet;
					QSet<CClassVariableExpression*> mAllVarExpSet;


					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> mVariableSubSuperPropagationSet;
					QHash<CExpressionVariable*, CExpressionVariable*> mVariableSubPropagationHash;
					QHash<CExpressionVariable*, CExpressionVariable*> mVariableSuperPropagationHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXBUILDINGCLASSVARIABLECOMPOSITIONSITEM_H
