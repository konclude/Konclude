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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEBINDINGABSORPTIONBASEDQUERYPARTHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEBINDINGABSORPTIONBASEDQUERYPARTHANDLER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingNeighbouringData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexConceptOntologyAnsweringItem.h"
#include "COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler.h"


// Other includes
#include "Parser/Expressions/CExpressionVariable.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableAbsorptionBasedQueryPartHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler : public COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader);

					COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* absorbQueryPart();

					COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* readConfig(CCalculationConfigurationExtension* config);

				// protected methods
				protected:





					CConcept* createVariableBindingPropagationConcept(CConcept* followingTriggerConcept, CRole* role);
					CConcept* createVariableBindingPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept, bool propagateBindings);
					CConcept* createVariableBindingPropagationTriggerConcept();
					CConcept* createVariableBindingPropagationJoiningConcept(CConcept* joinConcept1, CConcept* joinConcept2, CConcept* impliedConcept);
					CConcept* createVariableBindingCreationConcept(CConcept* followingConcept, CVariable* variable);
					CConcept* createOrConcept();


					CConcept* createVariableBindingPrepareConcept(CConcept* followingConcept);


					CConcept* createClassExpressionAbsorption(CConcept* baseConcept, CExpressionVariable* currentVariable, bool propagateBindings);

					CConcept* createVariableBindingFinalizerConcept();


					virtual CExpressionVariable* propagateAbsorptionOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty);


					CConcept* createTriggerConcept();
					CConcept* createPropagationConcept(CConcept* followingTriggerConcept, CRole* role);
					CConcept* createRepeatedTopRolePropagationConcept(CConcept* followingTriggerConcept);

					virtual COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* handleCurrentVariableSwitch(CExpressionVariable* newCurrentVariable);


					virtual CExpressionVariable* absorbStraightObjectPropertyAssertionPath(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty);
					COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* absorbIndividualVariablePathInversed(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*>& absorbPropAssList);
					bool isIndividualVariablePathInversedAbsorbable(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*>& absorbPropAssList);

					CExpressionVariable* propagateOrdinaryTriggerOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty, CConcept* lastTrigger);
					CConcept* createOrdinaryPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept);
					CConcept* createOrdinaryPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept);
					CConcept* createOrdinaryPropagationTriggerConcept();
					CConcept* createOrdinaryPropagationConcept(CConcept* followingTriggerConcept, CRole* role);
					CExpressionVariable* propagateOrdinaryTriggerToIndividualVariableOverSinglePath(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*> absorbPropAssList);



					virtual CExpressionVariable* findNextAbsorptionVariable(CExpressionVariable* currentVariable, QList<CExpressionVariable*>& absorbedPropagatedVariables, QList<CObjectPropertyAssertionExpression*>& objectPropertyPath);


				// protected variables
				protected:

					bool mConfCreateAlwaysBindingConcepts;
					bool mConfInverseIndividualVariableSinglePathAbsorption;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEBINDINGABSORPTIONBASEDQUERYPARTHANDLER_H
