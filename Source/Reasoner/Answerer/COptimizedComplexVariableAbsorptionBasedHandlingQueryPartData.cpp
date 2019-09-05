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

#include "COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData(const QSet<CExpressionVariable*>& anonymousVariableSet, const QSet<CExpressionVariable*>& restrictedAbsorptionVariableSet) {
				mAnonymousVariableSet = anonymousVariableSet;
				mRestrictedAbsorptionVariableSet = restrictedAbsorptionVariableSet;
				mPropagationFinalizationConcept = nullptr;
				mInitializerConcept = nullptr;
				mLastTriggerConcept = nullptr;
				mLastTriggerVariable = nullptr;
				mInitializerVariableExpression = nullptr;
				mBindingPropagationStartVariableExpression = nullptr;
				mIndiVariables = false;
				mTopObjectPropertyUsed = false;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::addVariableClassExpressions(CExpressionVariable* variable, QList<CBuildExpression*>& classExpressions) {
				for (CBuildExpression* classExpression : classExpressions) {
					mVariableBuiltExpressionsHash.insertMulti(variable, classExpression);
				}
				mVariableAbsorptionSet.insert(variable);
				if (!mAnonymousVariableSet.contains(variable)) {
					mIndividualVariableSet.insert(variable);
				}
				return this;
			}


			QList<CBuildExpression*> COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableClassExpressions(CExpressionVariable* variable) {
				return mVariableBuiltExpressionsHash.values(variable);
			}


			QHash<CExpressionVariable*, CBuildExpression*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableBuiltExpressionsHash() {
				return &mVariableBuiltExpressionsHash;
			}



			QSet<CObjectPropertyAssertionExpression*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getPropertyAssertionSet() {
				return &mAxiomAssertionSet;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::addVariableNeighbouringPropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion) {
				mAxiomAssertionSet.insert(propertyAssertion);
				CExpressionVariable* var1Exp = dynamic_cast<CExpressionVariable*>(propertyAssertion->getFirstIndividualTermExpression());
				CExpressionVariable* var2Exp = dynamic_cast<CExpressionVariable*>(propertyAssertion->getSecondIndividualTermExpression());
				if (!mVarPropAssSet.contains(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(var1Exp, propertyAssertion))) {
					mVarPropAssSet.insert(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(var1Exp, propertyAssertion));
					mVarPropNeighbouringAssHash.insertMulti(var1Exp, propertyAssertion);
				}
				if (!mVarPropAssSet.contains(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(var2Exp, propertyAssertion))) {
					mVarPropAssSet.insert(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(var2Exp, propertyAssertion));
					mVarPropNeighbouringAssHash.insertMulti(var2Exp, propertyAssertion);
				}
				return this;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::addVariableSelfPropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion) {
				mAxiomAssertionSet.insert(propertyAssertion);
				CExpressionVariable* varExp = dynamic_cast<CExpressionVariable*>(propertyAssertion->getFirstIndividualTermExpression());
				if (!mVarPropAssSet.contains(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(varExp, propertyAssertion))) {
					mVarPropAssSet.insert(QPair<CExpressionVariable*, CObjectPropertyAssertionExpression*>(varExp, propertyAssertion));
					mVarPropSelfAssHash.insertMulti(varExp, propertyAssertion);
				}
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::isVariableHandled(CExpressionVariable* variable) {
				return mHandledVariableSet.contains(variable);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setVariableHandled(CExpressionVariable* variable) {
				mHandledVariableSet.insert(variable);
				return this;
			}


			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getRestrictedAbsorptionVariableSet() {
				return &mRestrictedAbsorptionVariableSet;
			}


			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getAbsorptionVariableSet() {
				return &mVariableAbsorptionSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getIndividualVariableSet() {
				return &mIndividualVariableSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getAnonymousVariableSet() {
				return &mAnonymousVariableSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getPrepareVariableSet() {
				return &mPrepareIndiVariableSet;
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::hasPreparationVariables() {
				return !mPrepareIndiVariableSet.isEmpty();
			}


			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getBindingsVariableSet() {
				return &mBindingsVariableSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getIndividualBindingsVariableSet() {
				return &mIndividualBindingsVariableSet;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::isPropertyNeighbouringRemovalDisconnecting(CExpressionVariable* variable, CObjectPropertyAssertionExpression* propAss) {
				CExpressionVariable* otherVarEpx = getOtherVariableExpression(variable, propAss);
				if (variable == otherVarEpx) {
					return false;
				}

				QSet<CExpressionVariable*> reachedVarExpSet;
				QList<CExpressionVariable*> testingVarExpList;
				testingVarExpList.append(variable);
				reachedVarExpSet.insert(variable);

				while (!testingVarExpList.isEmpty()) {
					CExpressionVariable* testingVarEpx = testingVarExpList.takeFirst();
					QList<CObjectPropertyAssertionExpression*> conPropAssList = mVarPropNeighbouringAssHash.values(testingVarEpx);
					for (CObjectPropertyAssertionExpression* connPropAss : conPropAssList) {
						if (connPropAss != propAss && !mRemovedPropAssSet.contains(connPropAss)) {
							CExpressionVariable* connVarEpx = getOtherVariableExpression(testingVarEpx, connPropAss);
							if (connVarEpx == variable) {
								return false;
							}
							if (!reachedVarExpSet.contains(connVarEpx)) {
								reachedVarExpSet.insert(connVarEpx);
								testingVarExpList.append(connVarEpx);
							}
						}
					}
				}

				return false;
			}
			

			QList<CObjectPropertyAssertionExpression*> COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getNeighbouringPropertyAssertions(CExpressionVariable* variable, bool removeAbsorbed) {
				QList<CObjectPropertyAssertionExpression*> propertyAssExps = mVarPropNeighbouringAssHash.values(variable);
				for (QList<CObjectPropertyAssertionExpression*>::iterator it = propertyAssExps.begin(); removeAbsorbed && it != propertyAssExps.end(); ) {
					CObjectPropertyAssertionExpression* propertyAssertion = *it;
					if (mRemovedPropAssSet.contains(propertyAssertion)) {
						it = propertyAssExps.erase(it);
					} else {
						++it;
					}
				}
				return propertyAssExps;
			}



			QList<CObjectPropertyAssertionExpression*> COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getSelfPropertyAssertions(CExpressionVariable* variable) {
				QList<CObjectPropertyAssertionExpression*> propertyAssExps = mVarPropSelfAssHash.values(variable);
				for (QList<CObjectPropertyAssertionExpression*>::iterator it = propertyAssExps.begin(); it != propertyAssExps.end(); ) {
					CObjectPropertyAssertionExpression* propertyAssertion = *it;
					if (mRemovedPropAssSet.contains(propertyAssertion)) {
						it = propertyAssExps.erase(it);
					} else {
						++it;
					}
				}
				return propertyAssExps;
			}



			CExpressionVariable* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getOtherVariableExpression(CExpressionVariable* varEpx, CObjectPropertyAssertionExpression* propAss) {
				CExpressionVariable* otherVarExp = dynamic_cast<CIndividualVariableExpression*>(propAss->getFirstIndividualTermExpression());
				if (otherVarExp != varEpx) {
					return otherVarExp;
				}
				else {
					return dynamic_cast<CIndividualVariableExpression*>(propAss->getSecondIndividualTermExpression());
				}
			}


			QHash<CExpressionVariable*, CConcept*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableLastTriggerConceptHash() {
				return &mVariableLastTriggerConceptHash;
			}

			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableLastTriggerConcept(CExpressionVariable* varEpx) {
				return mVariableLastTriggerConceptHash.value(varEpx);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setVariableLastTriggerConcept(CExpressionVariable* varEpx, CConcept* lastTriggerConcept) {
				mVariableLastTriggerConceptHash.insert(varEpx, lastTriggerConcept);
				return this;
			}



			QHash<CExpressionVariable*, CConcept*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableBindingConceptHash() {
				return &mVariableBindingConceptHash;
			}

			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableBindingConcept(CExpressionVariable* varEpx) {
				return mVariableBindingConceptHash.value(varEpx);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setVariableBindingConcept(CExpressionVariable* varEpx, CConcept* bindingConcept) {
				mVariableBindingConceptHash.insert(varEpx, bindingConcept);
				return this;
			}



			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::removePropertyAssertion(CObjectPropertyAssertionExpression* propertyAssertion) {
				mRemovedPropAssSet.insert(propertyAssertion);
				return this;
			}


			QHash<CExpressionVariable*, CVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getVariableExpressionVariableHash() {
				return &mVariableExpressionVariableHash;
			}

			QHash<CVariable*, CExpressionVariable*>* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getExpressionVariableVariableHash() {
				return &mExpressionVariableVariableHash;
			}


			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getInitializerConcept() {
				return mInitializerConcept;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setInitializerConcept(CConcept* concept) {
				mInitializerConcept = concept;
				return this;
			}


			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getPropagationFinalizationConcept() {
				return mPropagationFinalizationConcept;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setPropagationFinalizationConcept(CConcept* concept) {
				mPropagationFinalizationConcept = concept;
				return this;
			}

			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getLastTriggerConcept() {
				return mLastTriggerConcept;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setLastTriggerConcept(CConcept* concept) {
				mLastTriggerConcept = concept;
				return this;
			}

			CExpressionVariable* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getLastTriggerVariable() {
				return mLastTriggerVariable;				
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setLastTriggerVariable(CExpressionVariable* variable) {
				mLastTriggerVariable = variable;
				return this;
			}



			CExpressionVariable* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getInitializerVariableExpression() {
				return mInitializerVariableExpression;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setInitializerVariableExpression(CExpressionVariable* variable) {
				mInitializerVariableExpression = variable;
				return this;
			}


			CExpressionVariable* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getBindingPropagationStartVariableExpression() {
				return mBindingPropagationStartVariableExpression;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setBindingPropagationStartVariableExpression(CExpressionVariable* variable) {
				mBindingPropagationStartVariableExpression = variable;
				return this;
			}




			QHash<CConcept*, QSet<CExpressionVariable*> >* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getConceptPropagatedVariableSetHash() {
				return &mConceptPropagatedVariableSetHash;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::addConceptPropagatedVariable(CConcept* concept, CExpressionVariable* propagatedVariable) {
				mConceptPropagatedVariableSetHash[concept].insert(propagatedVariable);
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::propagateConceptPropagatedVariables(CConcept* concept, CConcept* propagationConcept) {
				QSet<CExpressionVariable*>& varSet = mConceptPropagatedVariableSetHash[concept];
				for (CExpressionVariable* propagatedVar : mConceptPropagatedVariableSetHash[propagationConcept]) {
					varSet.insert(propagatedVar);
				}
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::hasIndividualVariables() {
				return mIndiVariables;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setHasIndividualVariables(bool indiVariables) {
				mIndiVariables = indiVariables;
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::isTopObjectPropertyUsed() {
				return mTopObjectPropertyUsed;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setTopObjectPropertyUsage(bool topObjectPropertyUsed) {
				mTopObjectPropertyUsed = topObjectPropertyUsed;
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::resetAbsorption() {
				mRemovedPropAssSet.clear();
				mHandledVariableSet.clear();
				return this;
			}




			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::setInitialObjectPropertyPropagatedTriggerConcept(CObjectPropertyTermExpression* objectPropertyExp, bool inversion, CConcept* propagatedTriggerConcept) {
				mInitialObjectPropertyPropagatedTriggerConceptHash.insert(QPair<CObjectPropertyTermExpression*, bool>(objectPropertyExp, inversion), propagatedTriggerConcept);
				return this;
			}

			CConcept* COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData::getInitialObjectPropertyPropagatedTriggerConcept(CObjectPropertyTermExpression* objectPropertyExp, bool inversion) {
				return mInitialObjectPropertyPropagatedTriggerConceptHash.value(QPair<CObjectPropertyTermExpression*, bool>(objectPropertyExp, inversion));
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
