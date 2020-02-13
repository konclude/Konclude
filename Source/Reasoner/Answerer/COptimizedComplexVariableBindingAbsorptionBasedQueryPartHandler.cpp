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

#include "COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader) : COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler(queryPart, ontoAnsweringItem, occStatsCacheReader) {
				mConfCreateAlwaysBindingConcepts = true;
				mConfInverseIndividualVariableSinglePathAbsorption = false;

			}



			COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::readConfig(CCalculationConfigurationExtension* config) {
				COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::readConfig(config);
				mConfCreateAlwaysBindingConcepts = CConfigDataReader::readConfigBoolean(config, "Konclude.Answering.QueryAbsorption.CreateAlwaysBindingConcepts");
				mConfInverseIndividualVariableSinglePathAbsorption = CConfigDataReader::readConfigBoolean(config, "Konclude.Answering.QueryAbsorption.InverseIndividualVariableSinglePathAbsorption");
				return this;
			}



			COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::handleCurrentVariableSwitch(CExpressionVariable* currentVariable) {
				CConcept* bindingConcept = mQueryPart->getVariableBindingConcept(currentVariable);
				if (!bindingConcept) {

					CVariable* variable = getPropagationVariableForVariableExpression(currentVariable);

					mQueryPart->getBindingsVariableSet()->insert(currentVariable);
					if (mQueryPart->getIndividualVariableSet()->contains(currentVariable)) {
						mQueryPart->getIndividualBindingsVariableSet()->insert(currentVariable);
					}

					CConcept* boundTriggerConcept = createVariableBindingPropagationTriggerConcept();
					CConcept* bindingConcept = createVariableBindingCreationConcept(boundTriggerConcept, variable);
					mQueryPart->setVariableBindingConcept(currentVariable, bindingConcept);
					mQueryPart->setLastTriggerConcept(boundTriggerConcept);
					mQueryPart->addConceptPropagatedVariable(boundTriggerConcept, currentVariable);

					if (!mQueryPart->getInitializerConcept()) {
						CConcept* initTriggerConcept = mQueryPart->getVariableLastTriggerConcept(currentVariable);
						if (!initTriggerConcept) {
							initTriggerConcept = createTriggerConcept();
						}
						CConcept* prepareConcept = createVariableBindingPrepareConcept(bindingConcept);
						prepareConcept = createClassExpressionAbsorption(prepareConcept, currentVariable, false);
						addConceptOperand(initTriggerConcept, prepareConcept, false);

						mQueryPart->setInitializerConcept(initTriggerConcept);
						mQueryPart->setBindingPropagationStartVariableExpression(currentVariable);
						if (!mQueryPart->getInitializerVariableExpression()) {
							mQueryPart->setInitializerVariableExpression(currentVariable);
						}
					} else {
						CConcept* lastTriggerConcept = mQueryPart->getVariableLastTriggerConcept(currentVariable);
						CConcept* prepareConcept = createVariableBindingPrepareConcept(bindingConcept);
						addConceptOperand(lastTriggerConcept, prepareConcept, false);

						CConcept* joinedTriggerConcept = createVariableBindingPropagationTriggerConcept();
						CConcept* joiningConcept = createVariableBindingPropagationJoiningConcept(lastTriggerConcept, boundTriggerConcept, joinedTriggerConcept);
						CConcept* joinPrepareConcept = createVariableBindingPrepareConcept(joiningConcept);
						addConceptOperand(lastTriggerConcept, joinPrepareConcept, false);

						mQueryPart->propagateConceptPropagatedVariables(joinedTriggerConcept, lastTriggerConcept);
						mQueryPart->propagateConceptPropagatedVariables(joinedTriggerConcept, boundTriggerConcept);

						boundTriggerConcept = joinedTriggerConcept;


					}
					mQueryPart->setVariableLastTriggerConcept(currentVariable, boundTriggerConcept);
					mQueryPart->setLastTriggerConcept(boundTriggerConcept);
					mQueryPart->setLastTriggerVariable(currentVariable);
				}
				return this;
			}





			COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::absorbQueryPart() {

				// required that all between all variables is chosen as the first binding variable
				mQueryPart->setLastTriggerVariable(nullptr);

				CConcept* alreadyExistingFinalizerConcept = mQueryPart->getPropagationFinalizationConcept();
				CConcept* alreadyExistingInitializerConcept = mQueryPart->getInitializerConcept();
				if (alreadyExistingInitializerConcept) {
					mQueryPart->setInitializerConcept(nullptr);
				}

				COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::absorbQueryPart();

				CConcept* lastTriggerConcept = mQueryPart->getLastTriggerConcept();
				CConcept* finalizerConcept = createVariableBindingFinalizerConcept();
				addConceptOperand(lastTriggerConcept, finalizerConcept, false);

				mQueryPart->setPropagationFinalizationConcept(finalizerConcept);


				if (alreadyExistingInitializerConcept) {
					mQueryPart->setInitializerConcept(alreadyExistingInitializerConcept);
				} else if (!alreadyExistingInitializerConcept && !mQueryPart->hasIndividualVariables()) {
					CConcept* topPropConcept = createRepeatedTopRolePropagationConcept(mQueryPart->getInitializerConcept());
					mQueryPart->setInitializerConcept(topPropConcept);
				}

				return this;
			}




			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createClassExpressionAbsorption(CConcept* impliedConcept, CExpressionVariable* currentVariable, bool propagateBindings) {
				QList<CBuildExpression*> classExpressions = mQueryPart->getVariableClassExpressions(currentVariable);
				if (classExpressions.size() > 0) {

					QList<CConcept*> triggerConceptList;
					bool allTriggerable = true;
					for (CBuildExpression* classExpression : classExpressions) {
						CClassTermExpression* classTermExp = (CClassTermExpression*)classExpression;
						CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
						bool testingNegation = testingConcept->hasMappingNegation();

						if (classTermExp) {
							if (!testingNegation && (testingConcept->getOperatorCode() == CCATOM || testingConcept->getOperatorCode() == CCSUB)) {
								triggerConceptList.append(testingConcept);
							} else {
								allTriggerable = false;
							}
						}
					}

					if (allTriggerable) {
						CConcept* implicationConcept = createVariableBindingPropagationImplicationTriggeredConcept(triggerConceptList, impliedConcept, propagateBindings);
						CConcept* implicationTriggeringConcept = createVariableBindingPropagationTriggerConcept();
						addConceptOperand(implicationTriggeringConcept, implicationConcept, false);
						return implicationTriggeringConcept;
					} else {
						if (!propagateBindings) {
							CConcept* orConcept = createOrConcept();
							CBOXLIST<TConceptNegPair>* conceptOfInterestList = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptOfInterestList(true);
							conceptOfInterestList->append(TConceptNegPair(orConcept, false));
							addConceptOperand(orConcept, impliedConcept, false);
							for (CBuildExpression* classExpression : classExpressions) {
								CClassTermExpression* classTermExp = (CClassTermExpression*)classExpression;
								CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
								bool testingNegation = testingConcept->hasMappingNegation();

								addConceptOperand(orConcept, testingConcept, !testingNegation);
							}
							return orConcept;
						} else {
							CConcept* implicationTriggeringConcept = createVariableBindingPropagationTriggerConcept();
							CConcept* implicationTriggerConcept = createVariableBindingPropagationTriggerConcept();
							CConcept* orConcept = createOrConcept();
							CBOXLIST<TConceptNegPair>* conceptOfInterestList = mOntoAnsweringItem->getTestingOntology()->getTBox()->getConceptOfInterestList(true);
							conceptOfInterestList->append(TConceptNegPair(orConcept, false));
							addConceptOperand(orConcept, implicationTriggerConcept, false);
							addConceptOperand(implicationTriggeringConcept, orConcept, false);

							triggerConceptList.append(implicationTriggerConcept);
							CConcept* implicationConcept = createVariableBindingPropagationImplicationTriggeredConcept(triggerConceptList, impliedConcept, true);
							addConceptOperand(implicationTriggeringConcept, implicationConcept, false);

							for (CBuildExpression* classExpression : classExpressions) {
								CClassTermExpression* classTermExp = (CClassTermExpression*)classExpression;
								CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
								bool testingNegation = testingConcept->hasMappingNegation();

								if (testingNegation || (testingConcept->getOperatorCode() != CCATOM && testingConcept->getOperatorCode() != CCSUB)) {
									addConceptOperand(orConcept, testingConcept, !testingNegation);
								}
							}
							return implicationTriggeringConcept;
						}
					}

				}
				return impliedConcept;
			}





			CExpressionVariable* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::propagateAbsorptionOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty) {
				CConcept* currentTriggerConcept = mQueryPart->getVariableLastTriggerConcept(currentVariable);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(objectProperty->getObjectPropertyTermExpression());
				bool inversed = dynamic_cast<CIndividualVariableExpression*>(objectProperty->getFirstIndividualTermExpression()) == currentVariable;
				if (inversed) {
					role = role->getInverseRole();
				}

				CConcept* nextTriggerConcept = nullptr;
				CExpressionVariable* otherVariable = mQueryPart->getOtherVariableExpression(currentVariable, objectProperty);
				CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(otherVariable);

				if (lastTrigger) {

					nextTriggerConcept = createVariableBindingPropagationTriggerConcept();
					QHash<CConcept*, QSet<CExpressionVariable*> >* conceptPropagatedVariableSetHash = mQueryPart->getConceptPropagatedVariableSetHash();
					QSet<CExpressionVariable*> varSet = conceptPropagatedVariableSetHash->value(lastTrigger);
					if (!varSet.isEmpty()) {
						mQueryPart->propagateConceptPropagatedVariables(nextTriggerConcept, currentTriggerConcept);
						CConcept* joinedTriggerConcept = createVariableBindingPropagationTriggerConcept();
						CConcept* joiningConcept = createVariableBindingPropagationJoiningConcept(lastTrigger, nextTriggerConcept, joinedTriggerConcept);
						CConcept* preparingConcept = createVariableBindingPrepareConcept(joiningConcept);
						addConceptOperand(nextTriggerConcept, preparingConcept, false);
						mQueryPart->propagateConceptPropagatedVariables(joinedTriggerConcept, lastTrigger);
						mQueryPart->propagateConceptPropagatedVariables(joinedTriggerConcept, nextTriggerConcept);
						lastTrigger = joinedTriggerConcept;
					} else {
						QList<CConcept*> triggerConceptList;
						triggerConceptList.append(lastTrigger);
						CConcept* impliedTriggerConcept = createVariableBindingPropagationTriggerConcept();
						CConcept* implicationConcept = createVariableBindingPropagationImplicationTriggeredConcept(triggerConceptList, impliedTriggerConcept, true);
						addConceptOperand(nextTriggerConcept, implicationConcept, false);
						lastTrigger = impliedTriggerConcept;
					}

				} else {
					QList<CBuildExpression*> classExpressions = mQueryPart->getVariableClassExpressions(otherVariable);
					if (classExpressions.size() > 0) {
						CConcept* classAbsorbtionTriggerConcept = createVariableBindingPropagationTriggerConcept();
						nextTriggerConcept = createClassExpressionAbsorption(classAbsorbtionTriggerConcept, otherVariable, true);
						lastTrigger = classAbsorbtionTriggerConcept;
					} else {
						nextTriggerConcept = createVariableBindingPropagationTriggerConcept();
						lastTrigger = nextTriggerConcept;
					}
				}


				CConcept* propagationConcept = createVariableBindingPropagationConcept(nextTriggerConcept, role);
				addConceptOperand(currentTriggerConcept, propagationConcept, false);

				mQueryPart->propagateConceptPropagatedVariables(lastTrigger, currentTriggerConcept);


				mQueryPart->setVariableLastTriggerConcept(otherVariable, lastTrigger);
				mQueryPart->setLastTriggerConcept(lastTrigger);
				mQueryPart->setLastTriggerVariable(otherVariable);
				return otherVariable;
			}




			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingPropagationConcept(CConcept* followingTriggerConcept, CRole* role) {
				CRole* inverseRole = getInverseRole(role);
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCVARBINDALL);
				succPropConcept->setRole(inverseRole);
				addConceptOperand(succPropConcept, followingTriggerConcept, false);
				return succPropConcept;
			}



			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingCreationConcept(CConcept* followingConcept, CVariable* variable) {
				CConcept* bindConcept = createNewConcept();
				bindConcept->setOperatorCode(CCVARBINDVARIABLE);
				bindConcept->setVariableLinker(CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan)->init(variable));

				addConceptOperand(bindConcept, followingConcept, false);

				return bindConcept;
			}


			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept, bool propagateBindings) {
				CConcept* implicationConcept = createNewConcept();
				if (propagateBindings) {
					implicationConcept->setOperatorCode(CCVARBINDIMPL);
				} else {
					implicationConcept->setOperatorCode(CCIMPL);
				}
				cint64 linkerCount = 0;
				CSortedNegLinker<CConcept*>* lastLinker = nullptr;
				for (CConcept* triggeredConcept : triggeredConceptList) {
					CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					lastLinker = newBaseOpCon1->init(triggeredConcept, true, lastLinker);
					++linkerCount;
				}
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedConcept, false, lastLinker);
				implicationConcept->setOperandList(newBaseOpCon2);
				implicationConcept->incOperandCount(linkerCount + 1);

				return implicationConcept;
			}


			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingPropagationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCVARBINDTRIG);
				return triggConcept;
			}

			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCIMPLTRIG);
				return triggConcept;
			}


			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingPrepareConcept(CConcept* followingConcept) {
				CConcept* prepConcept = createNewConcept();
				prepConcept->setOperatorCode(CCVARBINDPREPARE);
				addConceptOperand(prepConcept, followingConcept, false);
				return prepConcept;
			}



			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingPropagationJoiningConcept(CConcept* joinConcept1, CConcept* joinConcept2, CConcept* impliedConcept) {
				CConcept* joiningConcept = createNewConcept();
				joiningConcept->setOperatorCode(CCVARBINDJOIN);

				CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon1->init(joinConcept2, true);
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(joinConcept1, true, newBaseOpCon1);
				CSortedNegLinker<CConcept*>* newBaseOpCon3 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon3->init(impliedConcept, false, newBaseOpCon2);
				joiningConcept->setOperandList(newBaseOpCon3);
				joiningConcept->incOperandCount(3);

				QHash<CExpressionVariable*, CVariable*>* variableExpressionVariableHash = mQueryPart->getVariableExpressionVariableHash();

				QHash<CConcept*, QSet<CExpressionVariable*> >* conceptPropagatedVariableSetHash = mQueryPart->getConceptPropagatedVariableSetHash();
				QSet<CExpressionVariable*> varSet1 = conceptPropagatedVariableSetHash->value(joinConcept1);
				QSet<CExpressionVariable*> varSet2 = conceptPropagatedVariableSetHash->value(joinConcept2);
				CSortedLinker<CVariable*>* varLinker = nullptr;
				for (QSet<CExpressionVariable*>::const_iterator it = varSet1.constBegin(), itEnd = varSet1.constEnd(); it != itEnd; ++it) {
					if (varSet2.contains(*it)) {
						CExpressionVariable* variableExp = *it;
						CSortedLinker<CVariable*>* tmpVarLinker = CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan);
						CVariable* variable = variableExpressionVariableHash->value(variableExp);
						tmpVarLinker->init(variable);
						if (varLinker) {
							varLinker = varLinker->insertNextSorted(tmpVarLinker);
						} else {
							varLinker = tmpVarLinker;
						}
					}
				}
				joiningConcept->setVariableLinker(varLinker);

				return joiningConcept;
			}


			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createVariableBindingFinalizerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCVARBINDFINALZE);
				return triggConcept;
			}




			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createPropagationConcept(CConcept* followingTriggerConcept, CRole* role) {
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCIMPLALL);
				succPropConcept->setRole(role);
				addConceptOperand(succPropConcept, followingTriggerConcept, false);
				return succPropConcept;
			}

			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createRepeatedTopRolePropagationConcept(CConcept* followingTriggerConcept) {
				CRole* topRole = mOntoAnsweringItem->getTestingOntology()->getRBox()->getTopObjectRole();
				CConcept* triggerConcept = createTriggerConcept();
				CConcept* propagationConcept = createPropagationConcept(triggerConcept, topRole);
				addConceptOperand(triggerConcept, propagationConcept, false);
				addConceptOperand(triggerConcept, followingTriggerConcept, false);
				return triggerConcept;
			}




			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createOrConcept() {
				CConcept* orConcept = createNewConcept();
				orConcept->setOperatorCode(CCOR);
				return orConcept;
			}




			CExpressionVariable* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::absorbStraightObjectPropertyAssertionPath(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty) {

				CExpressionVariable* nextVariable = absorbObjectPropertyAssertion(currentVariable, startingProperty);

				QSet<CExpressionVariable*>* anonymousVarSet = mQueryPart->getAnonymousVariableSet();

				while (nextVariable) {
					if (mConfCreateAlwaysBindingConcepts || !anonymousVarSet->contains(nextVariable)) {
						break;
					}
					if (mQueryPart->getIndividualVariableSet()->contains(nextVariable) && !mQueryPart->getBindingsVariableSet()->contains(nextVariable)) {
						break;
					}
					QList<CObjectPropertyAssertionExpression*> neighbouringPropAssList = mQueryPart->getNeighbouringPropertyAssertions(nextVariable);
					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(nextVariable);
					if (neighbouringPropAssList.size() == 1 && selfPropAssList.isEmpty()) {
						QList<CObjectPropertyAssertionExpression*> absorbPropAssList;
						if (mConfInverseIndividualVariableSinglePathAbsorption && isIndividualVariablePathInversedAbsorbable(nextVariable, absorbPropAssList)) {
							absorbIndividualVariablePathInversed(nextVariable, absorbPropAssList);
							break;
						}

						CObjectPropertyAssertionExpression* nextObjectPropertyAssertion = neighbouringPropAssList.first();
						nextVariable = absorbObjectPropertyAssertion(nextVariable, nextObjectPropertyAssertion);
					} else {
						break;
					}
				}

				return nextVariable;
			}


			CExpressionVariable* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::findNextAbsorptionVariable(CExpressionVariable* currentVariable, QList<CExpressionVariable*>& absorbedPropagatedVariables, QList<CObjectPropertyAssertionExpression*>& objectPropertyPath) {
				CExpressionVariable* var = COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::findNextAbsorptionVariable(currentVariable, absorbedPropagatedVariables, objectPropertyPath);
				if (!var) {
					for (CExpressionVariable* testVar : absorbedPropagatedVariables) {
						if (mQueryPart->getIndividualVariableSet()->contains(testVar) && !mQueryPart->getBindingsVariableSet()->contains(testVar)) {
							return testVar;
						}
					}
				}
				return var;
			}














			CExpressionVariable* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::propagateOrdinaryTriggerToIndividualVariableOverSinglePath(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*> absorbPropAssList) {
				CConcept* initialTriggerConcept = createOrdinaryPropagationTriggerConcept();
				CConcept* prepareConcept = createVariableBindingPrepareConcept(initialTriggerConcept);

				CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(currentVariable);
				addConceptOperand(lastTrigger, prepareConcept, false);


				CExpressionVariable* nextVariable = currentVariable;
				for (CObjectPropertyAssertionExpression* obPropAss : absorbPropAssList) {
					nextVariable = propagateOrdinaryTriggerOverObjectPropertyAssertion(nextVariable, obPropAss, initialTriggerConcept);
					initialTriggerConcept = nullptr;
				}

				return nextVariable;
			}




			CExpressionVariable* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::propagateOrdinaryTriggerOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty, CConcept* currentTriggerConcept) {
				if (!currentTriggerConcept) {
					currentTriggerConcept = mQueryPart->getVariableLastTriggerConcept(currentVariable);
				}

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(objectProperty->getObjectPropertyTermExpression());
				bool inversed = dynamic_cast<CIndividualVariableExpression*>(objectProperty->getSecondIndividualTermExpression()) == currentVariable;
				if (inversed) {
					role = role->getInverseRole();
				}

				CConcept* nextTriggerConcept = nullptr;
				CExpressionVariable* otherVariable = mQueryPart->getOtherVariableExpression(currentVariable, objectProperty);
				CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(otherVariable);

				if (lastTrigger) {
					nextTriggerConcept = createOrdinaryPropagationTriggerConcept();
					CConcept* joinedTriggerConcept = createOrdinaryPropagationTriggerConcept();

					CConcept* implicationConcept = createOrdinaryPropagationImplicationTriggeredConcept(lastTrigger, joinedTriggerConcept);
					addConceptOperand(nextTriggerConcept, implicationConcept, false);

					lastTrigger = joinedTriggerConcept;
				}

				CConcept* propagationConcept = createOrdinaryPropagationConcept(nextTriggerConcept, role);
				addConceptOperand(currentTriggerConcept, propagationConcept, false);

				mQueryPart->setVariableLastTriggerConcept(otherVariable, lastTrigger);
				return otherVariable;
			}





			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createOrdinaryPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept) {
				QList<CConcept*> triggeredConceptList;
				triggeredConceptList.append(triggeredConcept);
				return createOrdinaryPropagationImplicationTriggeredConcept(triggeredConceptList, impliedConcept);
			}


			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createOrdinaryPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept) {
				CConcept* implicationConcept = createNewConcept();
				implicationConcept->setOperatorCode(CCIMPL);


				if (triggeredConceptList.isEmpty()) {
					bool debug = true;
				}

				cint64 linkerCount = 0;
				CSortedNegLinker<CConcept*>* lastLinker = nullptr;
				for (CConcept* triggeredConcept : triggeredConceptList) {
					CSortedNegLinker<CConcept*>* newBaseOpCon1 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					lastLinker = newBaseOpCon1->init(triggeredConcept, true, lastLinker);
					++linkerCount;
				}
				CSortedNegLinker<CConcept*>* newBaseOpCon2 = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				newBaseOpCon2->init(impliedConcept, false, lastLinker);
				implicationConcept->setOperandList(newBaseOpCon2);
				implicationConcept->incOperandCount(linkerCount + 1);

				return implicationConcept;
			}

			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createOrdinaryPropagationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCIMPLTRIG);
				return triggConcept;
			}



			CConcept* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::createOrdinaryPropagationConcept(CConcept* followingTriggerConcept, CRole* role) {
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCIMPLALL);
				succPropConcept->setRole(role);
				addConceptOperand(succPropConcept, followingTriggerConcept, false);
				return succPropConcept;
			}



			COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::absorbIndividualVariablePathInversed(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*>& absorbPropAssList) {
				CExpressionVariable* indiVariable = propagateOrdinaryTriggerToIndividualVariableOverSinglePath(currentVariable, absorbPropAssList);

				CConcept* alreadyExistingInitializerConcept = mQueryPart->getInitializerConcept();
				mQueryPart->setInitializerConcept(nullptr);
				CConcept* lastTriggerConcept = mQueryPart->getVariableLastTriggerConcept(indiVariable);
				handleCurrentVariableSwitch(indiVariable);
				CConcept* indiPrepInitializerConcept = mQueryPart->getInitializerConcept();
				addConceptOperand(lastTriggerConcept, indiPrepInitializerConcept, false);
				mQueryPart->setInitializerConcept(alreadyExistingInitializerConcept);

				CExpressionVariable* nextVariable = indiVariable;
				while (!absorbPropAssList.isEmpty()) {
					CObjectPropertyAssertionExpression* obPropAss = absorbPropAssList.takeLast();
					nextVariable = absorbObjectPropertyAssertion(nextVariable, obPropAss);
				}

				return this;
			}

			bool COptimizedComplexVariableBindingAbsorptionBasedQueryPartHandler::isIndividualVariablePathInversedAbsorbable(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*>& absorbPropAssList) {

				CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
				if (roleRealization) {
					CExpressionVariable* nextVariable = currentVariable;
					QSet<CObjectPropertyAssertionExpression*> absorbedPropAssSet;
					while (nextVariable) {
						QList<CObjectPropertyAssertionExpression*> neighbouringPropAssList = mQueryPart->getNeighbouringPropertyAssertions(nextVariable);
						QList<CObjectPropertyAssertionExpression*> filteredNeighbouringPropAssList;
						for (CObjectPropertyAssertionExpression* propAss : neighbouringPropAssList) {
							if (!absorbedPropAssSet.contains(propAss)) {
								filteredNeighbouringPropAssList.append(propAss);
							}
						}

						QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(nextVariable);
						if (filteredNeighbouringPropAssList.size() == 1 && selfPropAssList.isEmpty()) {
							CObjectPropertyAssertionExpression* nextObjectPropertyAssertion = filteredNeighbouringPropAssList.first();

							CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(nextObjectPropertyAssertion->getObjectPropertyTermExpression());
							bool inversed = dynamic_cast<CIndividualVariableExpression*>(nextObjectPropertyAssertion->getSecondIndividualTermExpression()) == nextVariable;
							if (inversed) {
								role = role->getInverseRole();
							}
							bool inversePreferred = false;
							if (roleRealization->getPreferredRolePropagationDirection(role, inversePreferred)) {
								if (inversePreferred) {
									absorbedPropAssSet.insert(nextObjectPropertyAssertion);
									absorbPropAssList.append(nextObjectPropertyAssertion);

									CExpressionVariable* otherVariable = dynamic_cast<CIndividualVariableExpression*>(nextObjectPropertyAssertion->getFirstIndividualTermExpression());
									if (dynamic_cast<CIndividualVariableExpression*>(nextObjectPropertyAssertion->getFirstIndividualTermExpression()) == nextVariable) {
										otherVariable = dynamic_cast<CIndividualVariableExpression*>(nextObjectPropertyAssertion->getSecondIndividualTermExpression());
									}

									nextVariable = otherVariable;
								} else {
									return false;
								}
							} else {
								return false;
							}

						} else if (filteredNeighbouringPropAssList.size() == 0 && selfPropAssList.isEmpty()) {
							if (mQueryPart->getIndividualVariableSet()->contains(nextVariable) && !mQueryPart->getBindingsVariableSet()->contains(nextVariable)) {
								break;
							}
						} else {
							return false;
						}
					}
					return true;
				} else {
					return false;
				}
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
