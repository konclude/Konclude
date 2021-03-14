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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEPREABSORPTIONBASEDQUERYPARTHANDLER_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEPREABSORPTIONBASEDQUERYPARTHANDLER_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingNeighbouringData.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"
#include "COptimizedComplexExpressionOntologyAnsweringItem.h"
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
			 *		\class		COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler : public COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader);

					COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* absorbQueryPart();

					COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* readConfig(CCalculationConfigurationExtension* config);

				// protected methods
				protected:


					CConcept* createPropagationToVariableConcept(CConcept* followingTriggerConcept, CRole* role, CVariable* variable);
					CConcept* createPropagationToPreparedVariableConcept(CConcept* followingTriggerConcept, CRole* role, CExpressionVariable* destVariable);
					CConcept* createPropagationConcept(CConcept* followingTriggerConcept, CRole* role);
					CConcept* createPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept);
					CConcept* createPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept);
					CConcept* createPropagationTriggerConcept();
					CConcept* createOrConcept();
					CConcept* createPrepareConcept(CConcept* followingConcept, CVariable* variable);


					CConcept* createPreparedClassExpressionAbsorption(CConcept* baseConcept, CExpressionVariable* currentVariable);
					CConcept* createClassExpressionAbsorption(CConcept* baseConcept, CExpressionVariable* currentVariable);

					virtual CExpressionVariable* propagateAbsorptionOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty);

					CConcept* createTriggerConcept();
					CConcept* createRepeatedTopRolePropagationConcept(CConcept* followingTriggerConcept);

					virtual COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* handleCurrentVariableSwitch(CExpressionVariable* newCurrentVariable);

					QList<CConcept*> collectVariableInitialObjectPropertiesAbsorptionTriggers(CExpressionVariable* currentVariable);
					COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* createVariablesMultipleAbsoprtionObjectPropertiesCrossPropagationTriggers();

				// protected variables
				protected:

					bool mConfCreateAlwaysBindingConcepts;
					cint64 mConfPreabsorptionCount;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEPREABSORPTIONBASEDQUERYPARTHANDLER_H
