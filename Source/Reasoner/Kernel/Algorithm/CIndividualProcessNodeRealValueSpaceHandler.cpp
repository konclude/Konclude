/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CIndividualProcessNodeRealValueSpaceHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndividualProcessNodeRealValueSpaceHandler::CIndividualProcessNodeRealValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType) : CIndividualProcessNodeValueSpaceHandler(valueSpaceType) {
					mRealValueSpaceType = (CDatatypeValueSpaceRealType*)valueSpaceType;
				}





				bool CIndividualProcessNodeRealValueSpaceHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
					CDataLiteralRealValue* dataLitRealValue = dynamic_cast<CDataLiteralRealValue*>(dataLitValue);

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,true);

					bool newValuesPotentiallyExcluded = false;

					if (!realValueSpaceData->isValueSpaceClashed() && dataLitRealValue) {
						CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(true);
						if (!negated) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToValue(dataLitRealValue,depTrackPoint);
						} else {
							newValuesPotentiallyExcluded |= realValueSpaceMap->excludeValue(dataLitRealValue,depTrackPoint);
						}
					}
					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(realValueSpaceData);
						realValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}




				bool CIndividualProcessNodeRealValueSpaceHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatype* datatype = dataRestricionConcept->getDatatype();
					cint64 restrictionCode = dataRestricionConcept->getParameter();
					CDataLiteral* dataLiteral = dataRestricionConcept->getDataLiteral();

					CDataLiteralValue* restrictionDataLitValue = nullptr;
					if (dataLiteral) {
						restrictionDataLitValue = dataLiteral->getDataLiteralValue();
					}
					CDataLiteralRealValue* restrictionDataLitRealValue = nullptr;
					if (restrictionDataLitValue) {
						restrictionDataLitRealValue = dynamic_cast<CDataLiteralRealValue*>(restrictionDataLitValue);
					}

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,true);

					if (realValueSpaceData->isValueSpaceClashed()) {
						return false;
					}

					CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(true);

					bool newValuesPotentiallyExcluded = false;
					if (!negated) {
						if (restrictionCode == CDFC_RATIONAL) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToRationalValues(depTrackPoint);
						} else if (restrictionCode == CDFC_DECIMAL) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToDecimalValues(depTrackPoint);
						} else if (restrictionCode == CDFC_INTEGER) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToIntegerValues(depTrackPoint);
						} else if (restrictionDataLitRealValue) {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMinimum(restrictionDataLitRealValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMinimum(restrictionDataLitRealValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMaximum(restrictionDataLitRealValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMaximum(restrictionDataLitRealValue,true,depTrackPoint);
							}
						}
					} else {
						if (restrictionCode == CDFC_RATIONAL) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToNonRationalValues(depTrackPoint);
						} else if (restrictionCode == CDFC_DECIMAL) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToNonDecimalValues(depTrackPoint);
						} else if (restrictionCode == CDFC_INTEGER) {
							newValuesPotentiallyExcluded |= realValueSpaceMap->restrictToNonIntegerValues(depTrackPoint);
						} else if (restrictionDataLitRealValue) {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMaximum(restrictionDataLitRealValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMaximum(restrictionDataLitRealValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMinimum(restrictionDataLitRealValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= realValueSpaceMap->excludeMinimum(restrictionDataLitRealValue,false,depTrackPoint);
							}
						}
					}

					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(realValueSpaceData);
						realValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}









				bool CIndividualProcessNodeRealValueSpaceHandler::testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,false);
					if (realValueSpaceData) {
						if (realValueSpaceData->isValueSpaceClashed()) {
							return true;
						}
						CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(false);
						if (realValueSpaceMap) {
							bool clashed = realValueSpaceMap->testValueSpaceReturnClashed();
							if (clashed) {
								realValueSpaceMap->addValueSpaceDependencies(realValueSpaceData->getClashDependencyTrackPointCollection());
								realValueSpaceData->setValueSpaceClashed(true);
								return true;
							}
						}
					}
					return false;
				}



				bool CIndividualProcessNodeRealValueSpaceHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	

					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,false);
					if (realValueSpaceData) {
						if (!realValueSpaceData->isValueSpaceClashed()) {
							CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(false);
							if (realValueSpaceMap) {
								return realValueSpaceMap->addValueSpaceDependencies(depCollection);
							}
						} else {
							return realValueSpaceData->getClashDependencyTrackPointCollection()->addCollectionDependencies(depCollection);
						}
					}
					return false;
				}


				bool CIndividualProcessNodeRealValueSpaceHandler::countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext) {
					bool counted = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	
					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,false);
					if (realValueSpaceData) {
						if (!realValueSpaceData->isValueSpaceClashed()) {
							if (!realValueSpaceData->isValueSpaceCounted() || realValueSpaceData->isValueSpaceCountingRequired()) {
								realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,true);
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = realValueSpaceData->getValuesCounter();
								valueSpaceValueCounter->resetValueCounter();
								CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(false);
								if (realValueSpaceMap) {
									realValueSpaceMap->countAllValues(valueSpaceValueCounter);
								} else {
									valueSpaceValueCounter->incInfinite();
								}
								realValueSpaceData->setValueSpaceCounted(true);
								realValueSpaceData->setValueSpaceCountingRequired(false);
								if (valueCounter) {
									counted = valueCounter->combineWithValueCounter(valueSpaceValueCounter);
								}
							} else {
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = realValueSpaceData->getValuesCounter();
								if (valueCounter) {
									counted = valueCounter->combineWithValueCounter(valueSpaceValueCounter);
								}
							}
						}
					} else {
						if (valueCounter) {
							counted = valueCounter->incInfinite();
						}
					}
					return counted;
				}


				CConceptDescriptor* CIndividualProcessNodeRealValueSpaceHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					bool substractValueSpaceValuesCount = false;

					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,false);
					if (!realValueSpaceData || (!realValueSpaceData->isValueSpaceClashed() && !realValueSpaceData->isValueSpaceTriggeringCompleted())) {
						realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,true);
						bool valueSpaceTriggeringFinished = false;
						if (!realValueSpaceData->isValueSpaceTriggeringStarted()) {
							realValueSpaceData->setValueSpaceTriggeringStarted(true);
						}
						CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(false);

						CDatatypeValueSpacesTriggers* valueSpacesTriggers = ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
						if (valueSpacesTriggers) {

							CDatatypeValueSpaceRealTriggers* realValueSpaceTriggers = valueSpacesTriggers->getRealValueSpaceTriggers();
							CDatatypeValueSpaceTriggeringMap* realValueSpaceTriggerMap = realValueSpaceTriggers->getValueSpaceTriggeringMap();

							depCollection->addDependency(realValueSpaceData->getValueSpaceInitialisationDependencyTrackPoint());

							CDatatypeValueSpaceConceptTriggeringData* valueSpaceConceptTriggeringData = realValueSpaceTriggers->getRealConceptTriggeringData();
							if (valueSpaceConceptTriggeringData->hasPartialConceptTriggers()) {
								conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,valueSpaceConceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
							}



							if (realValueSpaceMap) {

								CDataLiteralRealValue* minValue = nullptr;
								bool minValueInclusive = false;
								CDependencyTrackPoint* minValueDepTrackPoint = nullptr;




								if (realValueSpaceMap->getAbsoluteMinimumValue(minValue,minValueInclusive,&minValueDepTrackPoint)) {

									CDatatypeDependencyCollection minValueDepCollection(depCollection,calcAlgContext);

									bool minDatatypeDepAdded = false;

									CDatatypeValueSpaceTriggeringIterator leftTriggerIt = realValueSpaceTriggerMap->getLeftTriggeringIterator(minValue,!minValueInclusive);
									while (leftTriggerIt.hasNext()) {
										CDatatypeValueSpaceTriggeringData* triggeringData = leftTriggerIt.next();
										CDatatypeValueSpaceConceptTriggeringData* minExcTriggeringConceptData = triggeringData->getMinExclusiveTriggeringData();
										CDatatypeValueSpaceConceptTriggeringData* minIncTriggeringConceptData = triggeringData->getMinInclusiveTriggeringData();
										if (minExcTriggeringConceptData->hasPartialConceptTriggers() || minIncTriggeringConceptData->hasPartialConceptTriggers()) {
											if (!minDatatypeDepAdded && minValueDepTrackPoint) {
												minDatatypeDepAdded = true;
												minValueDepCollection.addDependency(minValueDepTrackPoint);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minExcTriggeringConceptData->getPartialConceptTriggerLinker(),&minValueDepCollection,calcAlgContext);
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minIncTriggeringConceptData->getPartialConceptTriggerLinker(),&minValueDepCollection,calcAlgContext);
										}
									}

									CDatatypeValueSpaceTriggeringData* minValueTriggerData = realValueSpaceTriggerMap->getDatatypeValueTriggeringData(minValue,false);
									if (minValueTriggerData) {
										CDatatypeValueSpaceConceptTriggeringData* minTriggeringConceptData = nullptr;
										if (!minValueInclusive) {
											minTriggeringConceptData = minValueTriggerData->getMinExclusiveTriggeringData();
										} else {
											minTriggeringConceptData = minValueTriggerData->getMinInclusiveTriggeringData();
										}
										if (minTriggeringConceptData->hasPartialConceptTriggers()) {
											if (!minDatatypeDepAdded && minValueDepTrackPoint) {
												minDatatypeDepAdded = true;
												minValueDepCollection.addDependency(minValueDepTrackPoint);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minTriggeringConceptData->getPartialConceptTriggerLinker(),&minValueDepCollection,calcAlgContext);
										}
									}
								}




								CDataLiteralRealValue* maxValue = nullptr;
								bool maxValueInclusive = false;
								CDependencyTrackPoint* maxValueDepTrackPoint = nullptr;
								if (realValueSpaceMap->getAbsoluteMaximumValue(maxValue,maxValueInclusive,&maxValueDepTrackPoint)) {

									CDatatypeDependencyCollection maxValueDepCollection(depCollection,calcAlgContext);

									bool maxDatatypeDepAdded = false;

									CDatatypeValueSpaceTriggeringIterator rightTriggerIt = realValueSpaceTriggerMap->getRightTriggeringIterator(maxValue,!maxValueInclusive);
									while (rightTriggerIt.hasNext()) {
										CDatatypeValueSpaceTriggeringData* triggeringData = rightTriggerIt.next();
										CDatatypeValueSpaceConceptTriggeringData* maxExcTriggeringConceptData = triggeringData->getMaxExclusiveTriggeringData();
										CDatatypeValueSpaceConceptTriggeringData* maxIncTriggeringConceptData = triggeringData->getMaxInclusiveTriggeringData();
										if (maxExcTriggeringConceptData->hasPartialConceptTriggers() || maxIncTriggeringConceptData->hasPartialConceptTriggers()) {
											if (!maxDatatypeDepAdded && maxValueDepTrackPoint) {
												maxDatatypeDepAdded = true;
												maxValueDepCollection.addDependency(minValueDepTrackPoint);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxExcTriggeringConceptData->getPartialConceptTriggerLinker(),&maxValueDepCollection,calcAlgContext);
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxIncTriggeringConceptData->getPartialConceptTriggerLinker(),&maxValueDepCollection,calcAlgContext);
										}
									}


									CDatatypeValueSpaceTriggeringData* maxValueTriggerData = realValueSpaceTriggerMap->getDatatypeValueTriggeringData(maxValue,false);
									if (maxValueTriggerData) {
										CDatatypeValueSpaceConceptTriggeringData* maxTriggeringConceptData = nullptr;
										if (!maxValueInclusive) {
											maxTriggeringConceptData = maxValueTriggerData->getMaxExclusiveTriggeringData();
										} else {
											maxTriggeringConceptData = maxValueTriggerData->getMaxInclusiveTriggeringData();
										}
										if (maxTriggeringConceptData->hasPartialConceptTriggers()) {
											if (!maxDatatypeDepAdded && maxValueDepTrackPoint) {
												maxDatatypeDepAdded = true;
												maxValueDepCollection.addDependency(maxValueDepTrackPoint);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxTriggeringConceptData->getPartialConceptTriggerLinker(),&maxValueDepCollection,calcAlgContext);
										}
									}
								}



								if (minValue && maxValue) {

									CDatatypeValueSpaceRealValuesCounter valueCounter;


									CDataLiteralCompareValue* freeLeftValue = nullptr;
									CDataLiteralCompareValue* freeRightValue = nullptr;
									bool freeLeftValueInclusive = false;
									bool freeRightValueInclusive = false;

									if (realValueSpaceTriggerMap->getIntervalMinMaxTriggerFreeInterval(minValue,minValueInclusive,maxValue,maxValueInclusive,freeLeftValue,freeLeftValueInclusive,freeRightValue,freeRightValueInclusive)) {
										CDataLiteralRealValue* freeLeftRealValue = dynamic_cast<CDataLiteralRealValue*>(freeLeftValue);
										CDataLiteralRealValue* freeRightRealValue = dynamic_cast<CDataLiteralRealValue*>(freeRightValue);
										if (freeLeftRealValue && freeRightRealValue) {
											CDatatypeValueSpaceRealValuesCounter tmpValueCounter;
											if (realValueSpaceMap->countIntervalValues(freeLeftRealValue,freeLeftValueInclusive,freeRightRealValue,freeRightValueInclusive,&tmpValueCounter)) {
												cint64 triggerValueCount = 0;
												if (realValueSpaceTriggerMap->countIntervalValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,triggerValueCount)) {
													valueCounter.combineWithValueCounter(&tmpValueCounter,triggerValueCount);
												}
											}
										}
									}


									if (!valueCounter.hasValueAchieved(remainingRequiredValuesCount)) {


										bool minToMaxTriggerDirection = true;

										cint64 minTriggerCount = 0;
										cint64 maxTriggerCount = 0;
										cint64 valueTriggerCount = 0;
										realValueSpaceTriggerMap->countIntervalMinMaxValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,minTriggerCount,maxTriggerCount,valueTriggerCount);

										if (minTriggerCount < maxTriggerCount) {
											minToMaxTriggerDirection = false;
										}

										depCollection->addDependency(minValueDepTrackPoint);
										depCollection->addDependency(maxValueDepTrackPoint);

										CDatatypeValueSpaceTriggeringIterator triggerIt = realValueSpaceTriggerMap->getIntervalTriggeringIterator(minValue,minValueInclusive,maxValue,maxValueInclusive,minToMaxTriggerDirection);

										if (minTriggerCount > 0 && maxTriggerCount > 0) {


											CDatatypeValueSpaceTriggeringIterator minMaxDirectionTriggerIt = triggerIt;
											while (minMaxDirectionTriggerIt.hasNext()) {

												CDatatypeValueSpaceTriggeringData* triggeringData = minMaxDirectionTriggerIt.next();
												if (minToMaxTriggerDirection) {
													CDatatypeValueSpaceConceptTriggeringData* maxExcTriggeringConceptData = triggeringData->getMaxExclusiveTriggeringData();
													CDatatypeValueSpaceConceptTriggeringData* maxIncTriggeringConceptData = triggeringData->getMaxInclusiveTriggeringData();
													if (maxExcTriggeringConceptData->hasPartialConceptTriggers() || maxIncTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxExcTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxIncTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}

												} else {

													CDatatypeValueSpaceConceptTriggeringData* minExcTriggeringConceptData = triggeringData->getMinExclusiveTriggeringData();
													CDatatypeValueSpaceConceptTriggeringData* minIncTriggeringConceptData = triggeringData->getMinInclusiveTriggeringData();
													if (minExcTriggeringConceptData->hasPartialConceptTriggers() || minIncTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minExcTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minIncTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}

												}
											}
										}


										if (!minValue->isEqualTo(maxValue)) {
											CDataLiteralRealValue* lastRealValue = nullptr;
											if (minToMaxTriggerDirection) {
												lastRealValue = minValue;
											} else {
												lastRealValue = maxValue;
											}


											CDatatypeValueSpaceTriggeringIterator stepMinMaxDirectionTriggerIt = triggerIt;
											while (!valueCounter.hasValueAchieved(remainingRequiredValuesCount) && stepMinMaxDirectionTriggerIt.hasNext()) {

												CDatatypeValueSpaceTriggeringData* triggeringData = stepMinMaxDirectionTriggerIt.next();
												CDataLiteralCompareValue* currentValue = triggeringData->getValue();
												CDataLiteralRealValue* currentRealValue = dynamic_cast<CDataLiteralRealValue*>(currentValue);


												if (lastRealValue && !lastRealValue->isEqualTo(currentRealValue)) {
													if (minToMaxTriggerDirection) {
														realValueSpaceMap->countIntervalValues(lastRealValue,false,currentRealValue,false,&valueCounter);
														realValueSpaceMap->addIntervalExclusionDependencies(lastRealValue,false,currentRealValue,false,depCollection);
													} else {
														realValueSpaceMap->countIntervalValues(currentRealValue,false,lastRealValue,false,&valueCounter);
														realValueSpaceMap->addIntervalExclusionDependencies(currentRealValue,false,lastRealValue,false,depCollection);
													}											
												}

												if (minToMaxTriggerDirection) {
													CDatatypeValueSpaceConceptTriggeringData* minExcTriggeringConceptData = triggeringData->getMinExclusiveTriggeringData();
													if (minExcTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minExcTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}
												} else {
													CDatatypeValueSpaceConceptTriggeringData* maxExcTriggeringConceptData = triggeringData->getMaxExclusiveTriggeringData();
													if (maxExcTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxExcTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}
												}

												if (!triggeringData->getDirectValueTriggeringData()->hasPartialConceptTriggers()) {
													realValueSpaceMap->countValueValues(currentRealValue,&valueCounter);
													realValueSpaceMap->addValueExclusionDependencies(currentRealValue,depCollection);
												}


												if (!valueCounter.hasValueAchieved(remainingRequiredValuesCount)) {

													if (minToMaxTriggerDirection) {
														CDatatypeValueSpaceConceptTriggeringData* minIncTriggeringConceptData = triggeringData->getMinInclusiveTriggeringData();
														if (minIncTriggeringConceptData->hasPartialConceptTriggers()) {
															conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,minIncTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
														}
													} else {
														CDatatypeValueSpaceConceptTriggeringData* maxIncTriggeringConceptData = triggeringData->getMaxInclusiveTriggeringData();
														if (maxIncTriggeringConceptData->hasPartialConceptTriggers()) {
															conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,maxIncTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
														}
													}
												}
											}
										}


										CDatatypeValueSpaceTriggeringIterator valueDirectionTriggerIt = triggerIt;
										while (!valueCounter.hasValueAchieved(remainingRequiredValuesCount) && valueDirectionTriggerIt.hasNext()) {

											CDatatypeValueSpaceTriggeringData* triggeringData = valueDirectionTriggerIt.next();
											CDataLiteralCompareValue* currentValue = triggeringData->getValue();
											CDataLiteralRealValue* currentRealValue = dynamic_cast<CDataLiteralRealValue*>(currentValue);

											if (triggeringData->getDirectValueTriggeringData()->hasPartialConceptTriggers()) {
												realValueSpaceMap->addValueExclusionDependencies(currentRealValue,depCollection);
												if (!realValueSpaceMap->isValueExcluded(currentRealValue,nullptr)) {
													realValueSpaceMap->countValueValues(currentRealValue,&valueCounter);

													CDatatypeValueSpaceConceptTriggeringData* directValueTriggeringConceptData = triggeringData->getDirectValueTriggeringData();
													if (directValueTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,directValueTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}
												}
											}
										}



									}





									if (!valueCounter.hasValueAchieved(CDatatypeValueSpaceRealValuesCounter::RVT_NON_RATIONAL_ONLY_COUNT,remainingRequiredValuesCount)) {
										// trigger integers
										CDatatypeValueSpaceConceptTriggeringData* conceptTriggeringData = realValueSpaceTriggers->getRationalConceptTriggeringData();
										if (conceptTriggeringData->hasPartialConceptTriggers()) {
											if (depCollection && !depCollection->hasDependencies()) {
												realValueSpaceMap->addValueSpaceDependencies(depCollection);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,conceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
										}
									}
									if (!valueCounter.hasValueAchieved(CDatatypeValueSpaceRealValuesCounter::RVT_NON_DECIMAL_ONLY_COUNT,remainingRequiredValuesCount)) {
										// trigger integers
										CDatatypeValueSpaceConceptTriggeringData* conceptTriggeringData = realValueSpaceTriggers->getDecimalConceptTriggeringData();
										if (conceptTriggeringData->hasPartialConceptTriggers()) {
											if (depCollection && !depCollection->hasDependencies()) {
												realValueSpaceMap->addValueSpaceDependencies(depCollection);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,conceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
										}
									}
									if (!valueCounter.hasValueAchieved(CDatatypeValueSpaceRealValuesCounter::RVT_NON_INTEGER_ONLY_COUNT,remainingRequiredValuesCount)) {
										// trigger integers
										CDatatypeValueSpaceConceptTriggeringData* conceptTriggeringData = realValueSpaceTriggers->getIntegerConceptTriggeringData();
										if (conceptTriggeringData->hasPartialConceptTriggers()) {
											if (depCollection && !depCollection->hasDependencies()) {
												realValueSpaceMap->addValueSpaceDependencies(depCollection);
											}
											conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,conceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
										}
									}


									remainingRequiredValuesCount -= valueCounter.getValueCount();
									if (valueCounter.isInfinite()) {
										remainingRequiredValuesCount = 0;
									}
									if (remainingRequiredValuesCount < 0) {
										remainingRequiredValuesCount = 0;
									}

									if (remainingRequiredValuesCount > 0) {
										valueSpaceTriggeringFinished = true;
									}


								}
							} else {
								remainingRequiredValuesCount = 0;
							}

						} else {
							valueSpaceTriggeringFinished = true;
							substractValueSpaceValuesCount = true;
						}

						if (valueSpaceTriggeringFinished) {
							if (!realValueSpaceData->isValueSpaceTriggeringCompleted()) {
								realValueSpaceData->setValueSpaceTriggeringCompleted(true);
							}
						}
					} else {
						substractValueSpaceValuesCount = true;
					}


					if (substractValueSpaceValuesCount) {
						CDatatypeValueSpaceValuesCounter valueCounter;
						countPossibleValueSpaceValues(indiProcNode,&valueCounter,calcAlgContext);
						if (valueCounter.isInfinite()) {
							remainingRequiredValuesCount = 0;
						} else {
							remainingRequiredValuesCount -= valueCounter.getValueCount();
							if (remainingRequiredValuesCount <= 0) {
								remainingRequiredValuesCount = 0;
							} else {
								addValueSpaceDependencies(indiProcNode,depCollection,calcAlgContext);
							}
						}
					}

					return conceptTriggerLinker;
				}




				bool CIndividualProcessNodeRealValueSpaceHandler::getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CDataLiteralRealValue* lastRealValue = nullptr;
					if (lastValue) {
						lastRealValue = dynamic_cast<CDataLiteralRealValue*>(lastValue);
					}


					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();

					CDatatypeRealValueSpaceData* realValueSpaceData = datatypesSpaceValue->getRealValueSpace(mRealValueSpaceType,true);
					if (realValueSpaceData && !realValueSpaceData->isValueSpaceClashed()) {
						CDataLiteralValue* tmpNextValue = valueTestingCollection->createDataLiteralValue(mRealValueSpaceType);
						CDataLiteralRealValue* nextRealValue = dynamic_cast<CDataLiteralRealValue*>(tmpNextValue);

						CDataLiteralValue* tmpLastValue = nullptr;
						CDataLiteralRealValue* tmpLastRealValue = nullptr;
						if (lastRealValue) {
							tmpLastValue = valueTestingCollection->createDataLiteralValue(mRealValueSpaceType);
							tmpLastRealValue = dynamic_cast<CDataLiteralRealValue*>(tmpLastValue);	
							tmpLastRealValue->initValue(lastRealValue);
						}

						CDatatypeRealValueSpaceMap* realValueSpaceMap = realValueSpaceData->getValueSpaceMap(true);
						bool searchNextValue = true;
						while (searchNextValue) {
							if (realValueSpaceMap->getNextPossibleDataValue(nextRealValue,tmpLastRealValue)) {
								if (!valueTestingCollection->isDataLiteralValueUsed(tmpNextValue,mRealValueSpaceType)) {
									valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mRealValueSpaceType);
									nextValue = tmpNextValue;
									return true;
								}
							} else {
								searchNextValue = false;
							}
							if (!tmpLastRealValue) {
								tmpLastValue = valueTestingCollection->createDataLiteralValue(mRealValueSpaceType);
								tmpLastRealValue = dynamic_cast<CDataLiteralRealValue*>(tmpLastValue);	
								tmpLastRealValue->initValue(nextRealValue);
							} else {
								tmpLastRealValue->initValue(nextRealValue);
							}
						}
						if (!searchNextValue) {
							valueTestingCollection->releaseDataLiteralValue(tmpNextValue,mRealValueSpaceType);
							valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mRealValueSpaceType);
						}
					}
					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
