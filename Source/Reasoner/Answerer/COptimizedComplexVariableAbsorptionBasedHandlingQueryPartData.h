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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGQUERYPARTDATA_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGQUERYPARTDATA_H

// Libraries includes
#include <QPair>

// Namespace includes
#include "AnswererSettings.h"


// Other includes
#include "Parser/Expressions/CExpressionVariable.h"
#include "Parser/Expressions/CIndividualVariableExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CObjectPropertyAssertionExpression.h"


#include "Reasoner/Ontology/CConcept.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData(const QSet<CExpressionVariable*>& anonymousVariableSet, const QSet<CExpressionVariable*>& restrictedAbsorptionVariableSet);



					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* addVariableClassExpressions(CExpressionVariable* variable, QList<CBuildExpression*>& classExpressions);
					QList<CBuildExpression*> getVariableClassExpressions(CExpressionVariable* variable);

					QHash<CExpressionVariable*, CBuildExpression*>* getVariableBuiltExpressionsHash();


					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* addVariableNeighbouringPropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* addVariableSelfPropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion);


					bool isVariableHandled(CExpressionVariable* variable);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setVariableHandled(CExpressionVariable* variable);

					QSet<CExpressionVariable*>* getRestrictedAbsorptionVariableSet();

					QSet<CExpressionVariable*>* getAbsorptionVariableSet();
					QSet<CExpressionVariable*>* getIndividualVariableSet();
					QSet<CExpressionVariable*>* getAnonymousVariableSet();

					// preparation variables differ from individual variables in the form that they have not been used in the absorption for generating a binder concept
					QSet<CExpressionVariable*>* getPrepareVariableSet();
					QSet<CExpressionVariable*>* getBindingsVariableSet();
					QSet<CExpressionVariable*>* getIndividualBindingsVariableSet();


					QList<CObjectPropertyAssertionExpression*> getNeighbouringPropertyAssertions(CExpressionVariable* variable, bool removeAbsorbed = true);
					QList<CObjectPropertyAssertionExpression*> getSelfPropertyAssertions(CExpressionVariable* variable);

					bool isPropertyNeighbouringRemovalDisconnecting(CExpressionVariable* variable, CObjectPropertyAssertionExpression* propAss);
					
					CExpressionVariable* getOtherVariableExpression(CExpressionVariable* varEpx, CObjectPropertyAssertionExpression* propAss);


					QHash<CExpressionVariable*, CConcept*>* getVariableLastTriggerConceptHash();
					CConcept* getVariableLastTriggerConcept(CExpressionVariable* varEpx);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setVariableLastTriggerConcept(CExpressionVariable* varEpx, CConcept* lastTriggerConcept);


					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* removePropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion);




					QHash<CExpressionVariable*, CConcept*>* getVariableBindingConceptHash();
					CConcept* getVariableBindingConcept(CExpressionVariable* varEpx);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setVariableBindingConcept(CExpressionVariable* varEpx, CConcept* bindingConcept);


					QHash<CExpressionVariable*, CVariable*>* getVariableExpressionVariableHash();
					QHash<CVariable*, CExpressionVariable*>* getExpressionVariableVariableHash();


					CConcept* getInitializerConcept();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setInitializerConcept(CConcept* concept);
					
					CConcept* getLastTriggerConcept();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setLastTriggerConcept(CConcept* concept);

					CExpressionVariable* getLastTriggerVariable();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setLastTriggerVariable(CExpressionVariable* variable);


					CConcept* getPropagationFinalizationConcept();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setPropagationFinalizationConcept(CConcept* concept);


					CExpressionVariable* getInitializerVariableExpression();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setInitializerVariableExpression(CExpressionVariable* variable);

					CExpressionVariable* getBindingPropagationStartVariableExpression();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setBindingPropagationStartVariableExpression(CExpressionVariable* variable);


					QHash<CConcept*, QSet<CExpressionVariable*> >* getConceptPropagatedVariableSetHash();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* addConceptPropagatedVariable(CConcept* concept, CExpressionVariable* propagatedVariable);
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* propagateConceptPropagatedVariables(CConcept* concept, CConcept* propagationConcept);


					bool hasPreparationVariables();

					bool hasIndividualVariables();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setHasIndividualVariables(bool indiVariables);

					bool isTopObjectPropertyUsed();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setTopObjectPropertyUsage(bool topObjectPropertyUsed);


					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* resetAbsorption();




					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setInitialObjectPropertyPropagatedTriggerConcept(CObjectPropertyTermExpression* objectPropertyExp, bool inversion, CConcept* propagatedTriggerConcept);
					CConcept* getInitialObjectPropertyPropagatedTriggerConcept(CObjectPropertyTermExpression* objectPropertyExp, bool inversion);


					QSet<CObjectPropertyAssertionExpression*>* getPropertyAssertionSet();


					bool hasNonTrivialAbsorptionPropagation();
					COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* setNonTrivialAbsorptionPropagation(bool prop);

				// protected methods
				protected:

				// protected variables
				protected:
					QSet<CObjectPropertyAssertionExpression*> mAxiomAssertionSet;

					QHash<CExpressionVariable*, CBuildExpression*> mVariableBuiltExpressionsHash;
					QHash<CExpressionVariable*, CObjectPropertyAssertionExpression*> mVarPropNeighbouringAssHash;
					QHash<CExpressionVariable*, CObjectPropertyAssertionExpression*> mVarPropSelfAssHash;

					QSet< QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*> > mVarPropAssSet;

					QSet<CExpressionVariable*> mHandledVariableSet;
					QSet<CExpressionVariable*> mAnonymousVariableSet;
					QSet<CExpressionVariable*> mPrepareIndiVariableSet;
					QSet<CExpressionVariable*> mIndividualVariableSet;
					QSet<CExpressionVariable*> mBindingsVariableSet;
					QSet<CExpressionVariable*> mIndividualBindingsVariableSet;

					QSet<CExpressionVariable*> mRestrictedAbsorptionVariableSet;


					QSet<CExpressionVariable*> mVariableAbsorptionSet;


					QSet<CObjectPropertyAssertionExpression*> mRemovedPropAssSet;


					QHash<CExpressionVariable*, CConcept*> mVariableLastTriggerConceptHash;
					QHash<CExpressionVariable*, CConcept*> mVariableBindingConceptHash;


					QHash<CConcept*, QSet<CExpressionVariable*> > mConceptPropagatedVariableSetHash;



					QHash<CExpressionVariable*, CVariable*> mVariableExpressionVariableHash;
					QHash<CVariable*, CExpressionVariable*> mExpressionVariableVariableHash;

					CExpressionVariable* mInitializerVariableExpression;
					CExpressionVariable* mBindingPropagationStartVariableExpression;
					CConcept* mInitializerConcept;
					CConcept* mLastTriggerConcept;
					CConcept* mPropagationFinalizationConcept;
					CExpressionVariable* mLastTriggerVariable;


					bool mIndiVariables;

					bool mTopObjectPropertyUsed;

					bool mNonTrivialAbsorptionPropagation;

					QHash<QPair<CObjectPropertyTermExpression*, bool>, CConcept*> mInitialObjectPropertyPropagatedTriggerConceptHash;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEABSORPTIONBASEDHANDLINGQUERYPARTDATA_H
