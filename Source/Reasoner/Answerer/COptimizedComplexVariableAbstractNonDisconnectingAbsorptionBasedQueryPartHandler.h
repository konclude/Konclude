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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTNONDISCONNECTINGABSORPTIONBASEDQUERYPARTHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTNONDISCONNECTINGABSORPTIONBASEDQUERYPARTHANDLER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingNeighbouringData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexConceptOntologyAnsweringItem.h"
#include "COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler.h"


// Other includes
#include "Parser/Expressions/CExpressionVariable.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CClassAssertionExpression.h"

#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCacheReader.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Kernel::Cache;

		namespace Answerer {



			typedef QPair<CObjectPropertyAssertionExpression*, double> TObjectPropertyOccurrenceScoreSortingItem;
			typedef QPair<CExpressionVariable*, double> TVariableTermsOccurrenceScoreSortingItem;


			/*! 
			 *
			 *		\class		COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler : public COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader);

					COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* absorbQueryPart();

					COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* readConfig(CCalculationConfigurationExtension* config);

				// protected methods
				protected:
					virtual CExpressionVariable* findNextAbsorptionVariable(CExpressionVariable* currentVariable, QList<CExpressionVariable*>& absorbedPropagatedVariables, QList<CObjectPropertyAssertionExpression*>& objectPropertyPath);
					virtual bool checkVariableCompletelyHandled(CExpressionVariable* currentVariable);

					virtual CExpressionVariable* absorbStraightObjectPropertyAssertionPath(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty);
					virtual CExpressionVariable* absorbObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty);


					virtual CExpressionVariable* propagateAbsorptionOverObjectPropertyAssertionPath(CExpressionVariable* currentVariable, const QList<CObjectPropertyAssertionExpression*>& objectPropertyPath);
					virtual CExpressionVariable* propagateAbsorptionOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty) = 0;




					virtual QList<CExpressionVariable*> getAbsorptionVariableList();

					virtual COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* handleCurrentVariableSwitch(CExpressionVariable* newCurrentVariable) = 0;




					virtual CVariable* getPropagationVariableForVariableExpression(CExpressionVariable* expVar);


					QList<CObjectPropertyAssertionExpression*> getOrderedObjectPropertyAbsorptionList(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*> objectPropertyList, double* totalOccScore = nullptr);
					QList<CExpressionVariable*> getOrderedVariableAbsorptionList(QList<CExpressionVariable*> varExpList);
					virtual QList<CExpressionVariable*> getFilteredOrderedVariableAbsorptionList(QList<CExpressionVariable*> varExpList);

					CConcept* createNewConcept();

				// protected variables
				protected:

					CConcreteOntology* mOntology;
					COccurrenceStatisticsCacheReader* mOccStatsCacheReader;

					bool mConfOccurrenceStatisticsBasedAbsorptionOrder;
					bool mConfOccurrenceStatisticsCollection;
					bool mConfPrioritizePrepareVariables;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSTRACTNONDISCONNECTINGABSORPTIONBASEDQUERYPARTHANDLER_H
