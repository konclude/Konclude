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

#include "COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader) : COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler(queryPart, ontoAnsweringItem, occStatsCacheReader) {
				mConfCreateAlwaysBindingConcepts = true;
				mConfPreabsorptionCount = 1;
				mConfPrioritizePrepareVariables = true;

			}



			COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::readConfig(CCalculationConfigurationExtension* config) {
				COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::readConfig(config);
				mConfPreabsorptionCount = CConfigDataReader::readConfigInteger(config, "Konclude.Answering.QueryAbsorption.PreabsorptionCount");
				return this;
			}



			COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::handleCurrentVariableSwitch(CExpressionVariable* currentVariable) {
				if (!mQueryPart->getInitializerConcept()) {
					mQueryPart->setInitializerConcept(createPropagationTriggerConcept());
					CConcept* lastTriggerConcept = mQueryPart->getInitializerConcept();
					if (!mQueryPart->getInitializerVariableExpression()) {
						if (mQueryPart->hasIndividualVariables()) {
							lastTriggerConcept = createPropagationTriggerConcept();
							CConcept* prepareConcept = createPrepareConcept(lastTriggerConcept, getPropagationVariableForVariableExpression(currentVariable));
							addConceptOperand(mQueryPart->getInitializerConcept(), prepareConcept, false);
						} else {
							CConcept* classAbsorbtionTriggerConcept = createPropagationTriggerConcept();
							CConcept* implicationConcept = createPreparedClassExpressionAbsorption(classAbsorbtionTriggerConcept, currentVariable);
							lastTriggerConcept = classAbsorbtionTriggerConcept;
							addConceptOperand(mQueryPart->getInitializerConcept(), implicationConcept, false);
						}
						mQueryPart->setInitializerVariableExpression(currentVariable);
					}
					mQueryPart->setLastTriggerConcept(lastTriggerConcept);
					mQueryPart->setLastTriggerVariable(currentVariable);
					mQueryPart->setVariableLastTriggerConcept(currentVariable, lastTriggerConcept);
				}
				return this;
			}



			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPrepareConcept(CConcept* followingConcept, CVariable* variable) {
				CConcept* prepConcept = createNewConcept();
				prepConcept->setOperatorCode(CCVARBINDPREPARE);
				prepConcept->setVariableLinker(CObjectAllocator< CSortedLinker<CVariable*> >::allocateAndConstruct(mMemMan)->init(variable));
				addConceptOperand(prepConcept, followingConcept, false);
				return prepConcept;
			}


			COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::absorbQueryPart() {

				CConcept* alreadyExistingFinalizerConcept = mQueryPart->getPropagationFinalizationConcept();
				CConcept* alreadyExistingInitializerConcept = mQueryPart->getInitializerConcept();
				if (alreadyExistingInitializerConcept) {
					mQueryPart->setInitializerConcept(nullptr);
				}

				cint64 absorptionCount = 0;
				while (absorptionCount < mConfPreabsorptionCount) {
					if (absorptionCount > 0) {
						createVariablesMultipleAbsoprtionObjectPropertiesCrossPropagationTriggers();
					}
					COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::absorbQueryPart();
					mQueryPart->resetAbsorption();
					++absorptionCount;
				}

				mQueryPart->setPropagationFinalizationConcept(mQueryPart->getLastTriggerConcept());

				if (alreadyExistingInitializerConcept) {
					addConceptOperand(alreadyExistingFinalizerConcept, mQueryPart->getInitializerConcept(), false);
					mQueryPart->setInitializerConcept(alreadyExistingInitializerConcept);
				} else if (!mQueryPart->hasPreparationVariables()) {
					CConcept* topPropConcept = createRepeatedTopRolePropagationConcept(mQueryPart->getInitializerConcept());
					mQueryPart->setInitializerConcept(topPropConcept);
				}

				return this;
			}





			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPreparedClassExpressionAbsorption(CConcept* impliedConcept, CExpressionVariable* currentVariable) {
				CConcept* classImpHanlingConcept = createClassExpressionAbsorption(impliedConcept, currentVariable);
				if (mQueryPart->getPrepareVariableSet()->contains(currentVariable)) {
					classImpHanlingConcept = createPrepareConcept(classImpHanlingConcept, getPropagationVariableForVariableExpression(currentVariable));
				}
				return classImpHanlingConcept;
			}


			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createClassExpressionAbsorption(CConcept* impliedConcept, CExpressionVariable* currentVariable) {
				QList<CBuildExpression*> classExpressions = mQueryPart->getVariableClassExpressions(currentVariable);

				QList<CConcept*> triggerConceptList;
				if (!mQueryPart->hasIndividualVariables() && !mQueryPart->hasPreparationVariables()) {
					triggerConceptList = collectVariableInitialObjectPropertiesAbsorptionTriggers(currentVariable);
				}
				bool allTriggerable = true;
				for (CBuildExpression* classExpression : classExpressions) {
					CClassTermExpression* classTermExp = (CClassTermExpression*)classExpression;
					CConcept* testingConcept = mOntoAnsweringItem->getTestingOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value(classTermExp);
					bool testingNegation = testingConcept->hasMappingNegation();

					if (classTermExp) {
						if (!testingNegation && (testingConcept->getOperatorCode() == CCIMPLTRIG || testingConcept->getOperatorCode() == CCATOM || testingConcept->getOperatorCode() == CCSUB)) {
							triggerConceptList.append(testingConcept);
						} else {
							allTriggerable = false;
						}
					}
				}


				if (triggerConceptList.isEmpty()) {
					return impliedConcept;
				}

				if (allTriggerable) {
					CConcept* implicationConcept = createPropagationImplicationTriggeredConcept(triggerConceptList, impliedConcept);
					CConcept* implicationTriggeringConcept = createPropagationTriggerConcept();
					addConceptOperand(implicationTriggeringConcept, implicationConcept, false);
					return implicationTriggeringConcept;
				} else {
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
				}

				return impliedConcept;
			}







			COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createVariablesMultipleAbsoprtionObjectPropertiesCrossPropagationTriggers() {
				QList<CConcept*> triggerConceptList;

				QHash<CExpressionVariable*, CConcept*> mObjectPropertyPropagatedTriggerMultipleAbsorptionConceptHash;


				for (CExpressionVariable* currentVariable : *mQueryPart->getAbsorptionVariableSet()) {
					CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(currentVariable);
					if (lastTrigger) {
						QList<CObjectPropertyAssertionExpression*> propList = mQueryPart->getNeighbouringPropertyAssertions(currentVariable, false);
						for (CObjectPropertyAssertionExpression* prop : propList) {
							bool inversed = false;
							CExpressionVariable* otherVariable = dynamic_cast<CIndividualVariableExpression*>(prop->getSecondIndividualTermExpression());
							if (otherVariable == currentVariable) {
								otherVariable = dynamic_cast<CIndividualVariableExpression*>(prop->getFirstIndividualTermExpression());
								inversed = true;
							}
							CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(prop->getObjectPropertyTermExpression());
							if (inversed) {
								role = role->getInverseRole();
							}

							CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(currentVariable);
							CConcept* triggConcept = createPropagationTriggerConcept();
							CConcept* propagationConcept = createPropagationConcept(triggConcept, role);
							addConceptOperand(lastTrigger, propagationConcept, false);
							mObjectPropertyPropagatedTriggerMultipleAbsorptionConceptHash.insertMulti(otherVariable, triggConcept);

						}
					}
				}

				for (CExpressionVariable* currentVariable : *mQueryPart->getAbsorptionVariableSet()) {

					QList<CConcept*> triggerConceptList = mObjectPropertyPropagatedTriggerMultipleAbsorptionConceptHash.values(currentVariable);
					CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(currentVariable);

					CConcept* impliedConcept = createPropagationTriggerConcept();
					CConcept* implicationConcept = createPropagationImplicationTriggeredConcept(triggerConceptList, impliedConcept);
					addConceptOperand(lastTrigger, implicationConcept, false);

					if (mQueryPart->getLastTriggerConcept() == lastTrigger) {
						mQueryPart->setLastTriggerConcept(impliedConcept);
					}
					mQueryPart->setVariableLastTriggerConcept(currentVariable, impliedConcept);

				}


				return this;
			}





			QList<CConcept*> COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::collectVariableInitialObjectPropertiesAbsorptionTriggers(CExpressionVariable* currentVariable) {
				QList<CConcept*> triggerConceptList;
				CConcept* initializationTrigger = mQueryPart->getInitializerConcept();

				CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(currentVariable);
				if (!lastTrigger) {
					QList<CObjectPropertyAssertionExpression*> propList = mQueryPart->getNeighbouringPropertyAssertions(currentVariable, false);
					for (CObjectPropertyAssertionExpression* prop : propList) {
						bool inversed = false;
						if (dynamic_cast<CIndividualVariableExpression*>(prop->getFirstIndividualTermExpression()) == currentVariable) {
							inversed = true;
						}
						CConcept* triggConcept = mQueryPart->getInitialObjectPropertyPropagatedTriggerConcept(prop->getObjectPropertyTermExpression(), inversed);
						if (!triggConcept) {
							CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(prop->getObjectPropertyTermExpression());
							if (inversed) {
								role = role->getInverseRole();
							}

							triggConcept = createPropagationTriggerConcept();
							CConcept* propagationConcept = createPropagationConcept(triggConcept, role);
							addConceptOperand(initializationTrigger, propagationConcept, false);
							mQueryPart->setInitialObjectPropertyPropagatedTriggerConcept(prop->getObjectPropertyTermExpression(), inversed, triggConcept);
						}
						triggerConceptList.append(triggConcept);
					}
				}

				return triggerConceptList;
			}





			CExpressionVariable* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::propagateAbsorptionOverObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* objectProperty) {
				CConcept* currentTriggerConcept = mQueryPart->getVariableLastTriggerConcept(currentVariable);

				CRole* role = mOntoAnsweringItem->getOntology()->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value(objectProperty->getObjectPropertyTermExpression());
				bool inversed = dynamic_cast<CIndividualVariableExpression*>(objectProperty->getSecondIndividualTermExpression()) == currentVariable;
				if (inversed) {
					role = role->getInverseRole();
				}

				CConcept* nextTriggerConcept = nullptr;
				CExpressionVariable* otherVariable = mQueryPart->getOtherVariableExpression(currentVariable, objectProperty);
				CConcept* lastTrigger = mQueryPart->getVariableLastTriggerConcept(otherVariable);

				if (lastTrigger) {
					nextTriggerConcept = createPropagationTriggerConcept();
					CConcept* joinedTriggerConcept = createPropagationTriggerConcept();

					CConcept* implicationConcept = createPropagationImplicationTriggeredConcept(lastTrigger, joinedTriggerConcept);
					addConceptOperand(nextTriggerConcept, implicationConcept, false);

					lastTrigger = joinedTriggerConcept;

				} else {
					CConcept* classAbsorbtionTriggerConcept = createPropagationTriggerConcept();
					nextTriggerConcept = createPreparedClassExpressionAbsorption(classAbsorbtionTriggerConcept, otherVariable);
					lastTrigger = classAbsorbtionTriggerConcept;
				}


				CConcept* propagationConcept = createPropagationConcept(nextTriggerConcept, role);
				addConceptOperand(currentTriggerConcept, propagationConcept, false);

				mQueryPart->setVariableLastTriggerConcept(otherVariable, lastTrigger);
				mQueryPart->setLastTriggerConcept(lastTrigger);
				mQueryPart->setLastTriggerVariable(otherVariable);
				return otherVariable;
			}


			


			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept) {
				QList<CConcept*> triggeredConceptList;
				triggeredConceptList.append(triggeredConcept);
				return createPropagationImplicationTriggeredConcept(triggeredConceptList, impliedConcept);
			}


			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPropagationImplicationTriggeredConcept(const QList<CConcept*>& triggeredConceptList, CConcept* impliedConcept) {
				CConcept* implicationConcept = createNewConcept();
				implicationConcept->setOperatorCode(CCIMPL);

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


			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPropagationTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCIMPLTRIG);
				return triggConcept;
			}

			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createTriggerConcept() {
				CConcept* triggConcept = createNewConcept();
				triggConcept->setOperatorCode(CCIMPLTRIG);
				return triggConcept;
			}






			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createPropagationConcept(CConcept* followingTriggerConcept, CRole* role) {
				CConcept* succPropConcept = createNewConcept();
				succPropConcept->setOperatorCode(CCIMPLALL);
				succPropConcept->setRole(role);
				addConceptOperand(succPropConcept, followingTriggerConcept, false);
				return succPropConcept;
			}

			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createRepeatedTopRolePropagationConcept(CConcept* followingTriggerConcept) {
				CRole* topRole = mOntoAnsweringItem->getTestingOntology()->getRBox()->getTopObjectRole();
				CConcept* triggerConcept = createTriggerConcept();
				CConcept* propagationConcept = createPropagationConcept(triggerConcept, topRole);
				addConceptOperand(triggerConcept, propagationConcept, false);
				addConceptOperand(triggerConcept, followingTriggerConcept, false);
				return triggerConcept;
			}




			CConcept* COptimizedComplexVariablePreAbsorptionBasedQueryPartHandler::createOrConcept() {
				CConcept* orConcept = createNewConcept();
				orConcept->setOperatorCode(CCOR);
				return orConcept;
			}




		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
