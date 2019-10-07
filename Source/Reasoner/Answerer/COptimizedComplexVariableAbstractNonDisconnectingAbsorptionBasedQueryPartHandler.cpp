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

#include "COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* queryPart, COptimizedComplexConceptOntologyAnsweringItem* ontoAnsweringItem, COccurrenceStatisticsCacheReader* occStatsCacheReader) : COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler(queryPart, ontoAnsweringItem) {
				mOntology = mOntoAnsweringItem->getOntology();
				mOccStatsCacheReader = occStatsCacheReader;

				mConfOccurrenceStatisticsCollection = true;
				mConfOccurrenceStatisticsBasedAbsorptionOrder = true;
				mConfPrioritizePrepareVariables = false;
			}



			COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::readConfig(CCalculationConfigurationExtension* config) {
				COptimizedComplexVariableAbstractAbsorptionBasedQueryPartHandler::readConfig(config);
				mConfOccurrenceStatisticsBasedAbsorptionOrder = CConfigDataReader::readConfigInteger(config, "Konclude.Answering.QueryAbsorption.OccurrenceStatisticsBasedAbsorptionOrder", true);
				mConfOccurrenceStatisticsCollection = CConfigDataReader::readConfigBoolean(config, "Konclude.Calculation.Optimization.OccurrenceStatisticsCollecting", true);
				return this;
			}



			CVariable* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::getPropagationVariableForVariableExpression(CExpressionVariable* expVar) {
				cint64 nextVariableId = mOntoAnsweringItem->getNextVariableId();
				QHash<CExpressionVariable*, CVariable*>* variableExpressionVariableHash = mQueryPart->getVariableExpressionVariableHash();
				QHash<CVariable*, CExpressionVariable*>* expressionVariableVariableHash = mQueryPart->getExpressionVariableVariableHash();
				CVariable* variable = variableExpressionVariableHash->value(expVar);
				if (!variable) {
					variable = new CVariable();
					variable->initVariable(nullptr, nextVariableId++);
					variableExpressionVariableHash->insert(expVar, variable);
					expressionVariableVariableHash->insert(variable, expVar);
				}
				return variable;
			}




			COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::absorbQueryPart() {
				QSet<CExpressionVariable*>* absorptionVariableSet = mQueryPart->getAbsorptionVariableSet();
				QSet<CExpressionVariable*>* anonymousVariableSet = mQueryPart->getAnonymousVariableSet();

				CExpressionVariable* currentVariable = mQueryPart->getLastTriggerVariable(); 
				if (!currentVariable) {
					QList<CExpressionVariable*> remainingVarList(getAbsorptionVariableList());

					bool hasIndividualVariables = false;
					for (CExpressionVariable* variable : remainingVarList) {
						if (!anonymousVariableSet->contains(variable)) {
							hasIndividualVariables = true;
						}
					}
					mQueryPart->setHasIndividualVariables(hasIndividualVariables);

					if (!mOntoAnsweringItem->getTestingOntology()->getABox()->hasUniversalConnectionIndividual()) {
						bool topObjProProp = checkHasTopObjectPropertyPropagation();
						if (topObjProProp) {
							mQueryPart->setTopObjectPropertyUsage(topObjProProp);
						}
					}

					currentVariable = remainingVarList.takeFirst();
				}

				while (currentVariable) {

					handleCurrentVariableSwitch(currentVariable);

					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(currentVariable);
					while (!selfPropAssList.isEmpty()) {
						CObjectPropertyAssertionExpression* propAss = selfPropAssList.takeFirst();
						absorbObjectPropertyAssertion(currentVariable, propAss);
					}

					QList<CExpressionVariable*> propagationVarList;
					QList<CObjectPropertyAssertionExpression*> propAssList = mQueryPart->getNeighbouringPropertyAssertions(currentVariable);
					while (!propAssList.isEmpty()) {
						propAssList = getOrderedObjectPropertyAbsorptionList(currentVariable, propAssList);
						CObjectPropertyAssertionExpression* propAss = propAssList.takeFirst();
						if (propAssList.isEmpty() || !mQueryPart->isPropertyNeighbouringRemovalDisconnecting(currentVariable, propAss)) {
							CExpressionVariable* otherVar = absorbStraightObjectPropertyAssertionPath(currentVariable, propAss);
							if (otherVar != currentVariable) {
								propagationVarList.append(otherVar);
							}
							propAssList = mQueryPart->getNeighbouringPropertyAssertions(currentVariable);
						}
					}

					QList<CObjectPropertyAssertionExpression*> objectPropertyPath;
					CExpressionVariable* nextVariable = findNextAbsorptionVariable(currentVariable, propagationVarList, objectPropertyPath);

					propagateAbsorptionOverObjectPropertyAssertionPath(currentVariable, objectPropertyPath);

					currentVariable = nextVariable;
				}



				return this;
			}





			QList<CExpressionVariable*> COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::getAbsorptionVariableList() {
				QSet<CExpressionVariable*>* absorbtionVariableSet = mQueryPart->getAbsorptionVariableSet();
				return getFilteredOrderedVariableAbsorptionList(absorbtionVariableSet->toList());
			}


			QList<CExpressionVariable*> COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::getFilteredOrderedVariableAbsorptionList(QList<CExpressionVariable*> varExpList) {
				QSet<CExpressionVariable*>* anonymousVariableSet = mQueryPart->getAnonymousVariableSet();
				QList<CExpressionVariable*> absorbableDistVarList;
				QList<CExpressionVariable*> absorbableDistVarMultiList;
				QList<CExpressionVariable*> absorbableNonDistVarList;
				QList<CExpressionVariable*> absorbableNonDistVarMultiList;
				QList<CExpressionVariable*> remainingVarList;
				for (CExpressionVariable* var : varExpList) {
					QList<CObjectPropertyAssertionExpression*> neighPropAssList = mQueryPart->getNeighbouringPropertyAssertions(var);
					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(var);
					bool hasAbsorbableProperty = false;
					if (selfPropAssList.isEmpty()) {
						for (CObjectPropertyAssertionExpression* propAss : neighPropAssList) {
							if (neighPropAssList.size() == 1 || !mQueryPart->isPropertyNeighbouringRemovalDisconnecting(var, propAss)) {
								hasAbsorbableProperty = true;
								break;
							}
						}
					}
					bool prioritizedVariable = !anonymousVariableSet->contains(var);
					if (mConfPrioritizePrepareVariables && !prioritizedVariable) {
						if (mQueryPart->getPrepareVariableSet()->contains(var)) {
							prioritizedVariable = true;
						}
					}
					if (prioritizedVariable) {
						if (hasAbsorbableProperty) {
							if (neighPropAssList.size() > 2) {
								absorbableDistVarMultiList.append(var);
							} else {
								absorbableDistVarList.append(var);
							}
						} else {
							remainingVarList.append(var);
						}
					} else {
						if (hasAbsorbableProperty) {
							if (neighPropAssList.size() > 2 || !selfPropAssList.isEmpty()) {
								absorbableNonDistVarMultiList.append(var);
							} else {
								absorbableNonDistVarList.append(var);
							}
						} else {
							remainingVarList.append(var);
						}
					}
				}
				QList<CExpressionVariable*> absorbVarList;
				absorbVarList.append(getOrderedVariableAbsorptionList(absorbableDistVarMultiList));
				absorbVarList.append(getOrderedVariableAbsorptionList(absorbableDistVarList));
				absorbVarList.append(getOrderedVariableAbsorptionList(absorbableNonDistVarMultiList));
				absorbVarList.append(getOrderedVariableAbsorptionList(absorbableNonDistVarList));
				absorbVarList.append(getOrderedVariableAbsorptionList(remainingVarList));
				return absorbVarList;

			}




			CExpressionVariable* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::findNextAbsorptionVariable(CExpressionVariable* currentVariable, QList<CExpressionVariable*>& absorbedPropagatedVariables, QList<CObjectPropertyAssertionExpression*>& objectPropertyPath) {

				QList<CExpressionVariable*> testingVariableList;
				QSet<CExpressionVariable*> testingVariableSet;
				QHash<CExpressionVariable*, CObjectPropertyAssertionExpression*> variablePredecessorObjectPropertyAssertionHash;
				testingVariableSet.insert(currentVariable);
				testingVariableList.append(currentVariable);
				for (CExpressionVariable* absorbedPropagatedVariable : absorbedPropagatedVariables) {
					if (!testingVariableSet.contains(absorbedPropagatedVariable)) {
						testingVariableSet.insert(absorbedPropagatedVariable);
						testingVariableList.append(absorbedPropagatedVariable);
					}
				}
				testingVariableList = getFilteredOrderedVariableAbsorptionList(testingVariableList);
				while (!testingVariableList.isEmpty()) {
					CExpressionVariable* nextVariable = testingVariableList.takeFirst();
					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(nextVariable);
					bool absorbable = false;
					if (!selfPropAssList.isEmpty()) {
						absorbable = true;
					}
					QList<CObjectPropertyAssertionExpression*> propAssList = mQueryPart->getNeighbouringPropertyAssertions(nextVariable);
					for (CObjectPropertyAssertionExpression* propAss : propAssList) {
						if (propAssList.size() == 1 || !mQueryPart->isPropertyNeighbouringRemovalDisconnecting(nextVariable, propAss)) {
							absorbable = true;
							break;
						} else {
							CExpressionVariable* otherVariable = mQueryPart->getOtherVariableExpression(nextVariable, propAss);
							if (!testingVariableSet.contains(otherVariable)) {
								testingVariableSet.insert(otherVariable);
								variablePredecessorObjectPropertyAssertionHash.insert(otherVariable, propAss);
								testingVariableList.append(otherVariable);
							}
						}
					}
					if (absorbable) {
						objectPropertyPath.clear();
						CExpressionVariable* predecessorVariable = nextVariable;
						while (predecessorVariable) {
							CObjectPropertyAssertionExpression* predecessorObjectPropertyAss = variablePredecessorObjectPropertyAssertionHash.value(predecessorVariable);
							if (predecessorObjectPropertyAss) {
								objectPropertyPath.append(predecessorObjectPropertyAss);
								predecessorVariable = mQueryPart->getOtherVariableExpression(nextVariable, predecessorObjectPropertyAss);
							}
							else {
								predecessorVariable = nullptr;
							}
						}
						return nextVariable;
					}

				}
				return nullptr;

			}

			



			bool COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::checkVariableCompletelyHandled(CExpressionVariable* currentVariable) {
				if (mQueryPart->isVariableHandled(currentVariable)) {
					return true;
				}
				QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(currentVariable);
				QList<CObjectPropertyAssertionExpression*> propAssList = mQueryPart->getNeighbouringPropertyAssertions(currentVariable);
				bool completelyHandled = propAssList.size() <= 0 && selfPropAssList.isEmpty();
				if (completelyHandled) {
					mQueryPart->setVariableHandled(currentVariable);
				}
				return completelyHandled;
			}




			CExpressionVariable* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::propagateAbsorptionOverObjectPropertyAssertionPath(CExpressionVariable* currentVariable, const QList<CObjectPropertyAssertionExpression*>& objectPropertyPath) {
				CExpressionVariable* nextVariable = currentVariable;
				for (CObjectPropertyAssertionExpression* objectProperty : objectPropertyPath) {
					nextVariable = propagateAbsorptionOverObjectPropertyAssertion(nextVariable, objectProperty);

				}
				return nextVariable;
			}



			CExpressionVariable* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::absorbStraightObjectPropertyAssertionPath(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty) {

				CExpressionVariable* nextVariable = absorbObjectPropertyAssertion(currentVariable, startingProperty);

				QSet<CExpressionVariable*>* anonymousVarSet = mQueryPart->getAnonymousVariableSet();

				while (nextVariable) {
					QList<CObjectPropertyAssertionExpression*> neighbouringPropAssList = mQueryPart->getNeighbouringPropertyAssertions(nextVariable);
					QList<CObjectPropertyAssertionExpression*> selfPropAssList = mQueryPart->getSelfPropertyAssertions(nextVariable);
					if (neighbouringPropAssList.size() == 1 && selfPropAssList.isEmpty()) {
						CObjectPropertyAssertionExpression* nextObjectPropertyAssertion = neighbouringPropAssList.first();
						nextVariable = absorbObjectPropertyAssertion(nextVariable, nextObjectPropertyAssertion);
					} else {
						break;
					}
				}

				return nextVariable;
			}


			CExpressionVariable* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::absorbObjectPropertyAssertion(CExpressionVariable* currentVariable, CObjectPropertyAssertionExpression* startingProperty) {
				CExpressionVariable* variable = propagateAbsorptionOverObjectPropertyAssertion(currentVariable, startingProperty);
				mQueryPart->removePropertyAssertion(startingProperty);
				return variable;
			}

			static bool itemSortLessObjectPropertyTermOccurrenceScoreThan(const TObjectPropertyOccurrenceScoreSortingItem& item1, const TObjectPropertyOccurrenceScoreSortingItem& item2) {
				return item1.second < item2.second;
			}

			static bool itemSortLessVariableTermsOccurrenceScoreThan(const TVariableTermsOccurrenceScoreSortingItem& item1, const TVariableTermsOccurrenceScoreSortingItem& item2) {
				return item1.second < item2.second;
			}


			QList<CExpressionVariable*> COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::getOrderedVariableAbsorptionList(QList<CExpressionVariable*> varExpList) {
				if (!mConfOccurrenceStatisticsBasedAbsorptionOrder || !mConfOccurrenceStatisticsCollection || !mOccStatsCacheReader) {
					return varExpList;
				}

				QList<CExpressionVariable*> orderedVarExpList;
				QList<TVariableTermsOccurrenceScoreSortingItem> varTermsOccScoreList;
				cint64 ontologyId = mOntology->getOntologyID();

				for (CExpressionVariable* varExp : varExpList) {
					double varOccScore = 0.;

					double varClassAssOccScoreMin = 0;
					double varClassAssOccScore = 0.;
					cint64 varClassAssOccCount = 0;
					for (CBuildExpression* classBuildExp : mQueryPart->getVariableClassExpressions(varExp)) {
						CClassTermExpression* classExp = dynamic_cast<CClassTermExpression*>(classBuildExp);
						if (dynamic_cast<CClassExpression*>(classExp)) {
							CConcept* concept = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermConceptMappingHash()->value((CClassTermExpression*)classExp);
							if (concept->getOperatorCode() == CCEQ) {
								concept = mOntology->getDataBoxes()->getTBox()->getEquivalentConceptCandidateHash(false)->value(concept);
							}
							COccurrenceStatisticsConceptData conceptOccStatsData = mOccStatsCacheReader->getAccummulatedConceptDataOccurrenceStatistics(ontologyId, concept->getConceptTag());

							double conceptOccFactor = conceptOccStatsData.getDeterministicInstanceOccurrencesCount() + conceptOccStatsData.getNonDeterministicInstanceOccurrencesCount() * 2 + conceptOccStatsData.getExistentialInstanceOccurrencesCount() * 1.5 + conceptOccStatsData.getIndividualInstanceOccurrencesCount();
							if (varClassAssOccCount != 0) {
								varClassAssOccScoreMin = qMin(varClassAssOccScoreMin, conceptOccFactor);
							} else {
								varClassAssOccScoreMin = conceptOccFactor;
							}
							varClassAssOccCount++;
						}
					}
					varClassAssOccScore = varClassAssOccScoreMin * (1. / (varClassAssOccCount + 1) + 0.5);
					varOccScore += varClassAssOccScore;


					QList<CObjectPropertyAssertionExpression*> propAssList = mQueryPart->getNeighbouringPropertyAssertions(varExp);
					propAssList.append(mQueryPart->getSelfPropertyAssertions(varExp));
					if (!propAssList.isEmpty()) {
						double varPropAssOccScore = 0.;
						getOrderedObjectPropertyAbsorptionList(varExp, propAssList, &varPropAssOccScore);
						varOccScore += varPropAssOccScore / 3.;
					}

					varTermsOccScoreList.append(TVariableTermsOccurrenceScoreSortingItem(varExp, varOccScore));
				}

				qSort(varTermsOccScoreList.begin(), varTermsOccScoreList.end(), itemSortLessVariableTermsOccurrenceScoreThan);
				for (TVariableTermsOccurrenceScoreSortingItem varScoreItem : varTermsOccScoreList) {
					CExpressionVariable* varExp = varScoreItem.first;
					orderedVarExpList.append(varExp);
				}

				return orderedVarExpList;
			}

			QList<CObjectPropertyAssertionExpression*> COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::getOrderedObjectPropertyAbsorptionList(CExpressionVariable* currentVariable, QList<CObjectPropertyAssertionExpression*> objectPropertyList, double* totalOccScore) {
				if (!mConfOccurrenceStatisticsBasedAbsorptionOrder || !mConfOccurrenceStatisticsCollection || !mOccStatsCacheReader) {
					return objectPropertyList;
				}

				QList<CObjectPropertyAssertionExpression*> orderedList;
				QList<TObjectPropertyOccurrenceScoreSortingItem> propOccScoreList;
				cint64 ontologyId = mOntology->getOntologyID();

				//bool hasLikeRole = false;

				double totalScore = 0;
				for (CObjectPropertyAssertionExpression* propAssExp : objectPropertyList) {

					CRole* role = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propAssExp->getObjectPropertyTermExpression());
					if (role) {
						//if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) == "http://semantics.crl.ibm.com/univ-bench-dl.owl#like") {
						//	hasLikeRole = true;
						//}


						double complexFactor = 1.;
						if (role->isComplexRole()) {
							complexFactor = 2.5;
						}

						COccurrenceStatisticsRoleData roleOccStatsData = mOccStatsCacheReader->getAccummulatedRoleDataOccurrenceStatistics(ontologyId, role->getRoleTag());
						double roleOccFactor = complexFactor * (roleOccStatsData.getDeterministicInstanceOccurrencesCount() + roleOccStatsData.getNonDeterministicInstanceOccurrencesCount() * 2 + roleOccStatsData.getExistentialInstanceOccurrencesCount() * 1.5 + roleOccStatsData.getIndividualInstanceOccurrencesCount());
						totalScore += roleOccFactor;
						propOccScoreList.append(TObjectPropertyOccurrenceScoreSortingItem(propAssExp, roleOccFactor));
					}

				}

				qSort(propOccScoreList.begin(), propOccScoreList.end(), itemSortLessObjectPropertyTermOccurrenceScoreThan);
				//if (!propOccScoreList.isEmpty() && hasLikeRole) {
				//	CRole* role = mOntology->getDataBoxes()->getExpressionDataBoxMapping()->getObjectPropertyTermRoleMappingHash()->value((CObjectPropertyTermExpression*)propOccScoreList.last().first->getObjectPropertyTermExpression());
				//	if (CIRIName::getRecentIRIName(role->getPropertyNameLinker()) != "http://semantics.crl.ibm.com/univ-bench-dl.owl#like") {
				//		bool debug = true;
				//		if (debug) {
				//			getOrderedObjectPropertyAbsorptionList(currentVariable, objectPropertyList, totalOccScore);
				//		}
				//	}
				//}
				for (TObjectPropertyOccurrenceScoreSortingItem propScoreItem : propOccScoreList) {
					CObjectPropertyAssertionExpression* propAssExp = propScoreItem.first;
					orderedList.append(propAssExp);
				}

				if (totalOccScore) {
					*totalOccScore = totalScore;
				}

				return orderedList;
			}


			CConcept* COptimizedComplexVariableAbstractNonDisconnectingAbsorptionBasedQueryPartHandler::createNewConcept() {
				CConcept* concept = CObjectAllocator< CConcept >::allocateAndConstruct(mMemMan);
				concept->initConcept();
				cint64 nextConTag = mTBox->getNextConceptID();
				concept->initTag(nextConTag);
				mConceptVec->setLocalData(nextConTag, concept);
				return concept;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
