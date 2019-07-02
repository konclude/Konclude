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

#include "CIndividualProcessNodeStringValueSpaceHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndividualProcessNodeStringValueSpaceHandler::CIndividualProcessNodeStringValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType) : CIndividualProcessNodeValueSpaceHandler(valueSpaceType) {
					mStringValueSpaceType = (CDatatypeValueSpaceStringType*)valueSpaceType;
				}





				bool CIndividualProcessNodeStringValueSpaceHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
					CDataLiteralStringValue* dataLitStringValue = dynamic_cast<CDataLiteralStringValue*>(dataLitValue);

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,true);

					bool newValuesPotentiallyExcluded = false;

					if (!stringValueSpaceData->isValueSpaceClashed() && dataLitStringValue) {
						CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(true);
						if (!negated) {
							newValuesPotentiallyExcluded |= stringValueSpaceMap->restrictToValue(dataLitStringValue,depTrackPoint);
						} else {
							newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeValue(dataLitStringValue,depTrackPoint);
						}
					}
					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(stringValueSpaceData);
						stringValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}




				bool CIndividualProcessNodeStringValueSpaceHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatype* datatype = dataRestricionConcept->getDatatype();
					cint64 restrictionCode = dataRestricionConcept->getParameter();
					CDataLiteral* dataLiteral = dataRestricionConcept->getDataLiteral();

					CDataLiteralValue* restrictionDataLitValue = nullptr;
					if (dataLiteral) {
						restrictionDataLitValue = dataLiteral->getDataLiteralValue();
					}
					CDataLiteralStringValue* restrictionDataLitStringValue = nullptr;
					if (restrictionDataLitValue) {
						restrictionDataLitStringValue = dynamic_cast<CDataLiteralStringValue*>(restrictionDataLitValue);
					}

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,true);

					if (stringValueSpaceData->isValueSpaceClashed()) {
						return false;
					}

					CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(true);

					bool newValuesPotentiallyExcluded = false;
					if (!negated) {
						if (restrictionCode == CDFC_STRING) {
							newValuesPotentiallyExcluded |= stringValueSpaceMap->restrictToWithoutLanguageTagsValues(depTrackPoint);
						} else if (restrictionDataLitStringValue) {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMinimum(restrictionDataLitStringValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMinimum(restrictionDataLitStringValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMaximum(restrictionDataLitStringValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMaximum(restrictionDataLitStringValue,true,depTrackPoint);
							}
						}
					} else {
						if (restrictionCode == CDFC_STRING) {
							newValuesPotentiallyExcluded |= stringValueSpaceMap->restrictToWithLanguageTagsValues(depTrackPoint);
						} else if (restrictionDataLitStringValue) {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMaximum(restrictionDataLitStringValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMaximum(restrictionDataLitStringValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMinimum(restrictionDataLitStringValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= stringValueSpaceMap->excludeMinimum(restrictionDataLitStringValue,false,depTrackPoint);
							}
						}
					}

					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(stringValueSpaceData);
						stringValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}









				bool CIndividualProcessNodeStringValueSpaceHandler::testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,false);
					if (stringValueSpaceData) {
						if (stringValueSpaceData->isValueSpaceClashed()) {
							return true;
						}
						CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(false);
						if (stringValueSpaceMap) {
							bool clashed = stringValueSpaceMap->testValueSpaceReturnClashed();
							if (clashed) {
								stringValueSpaceMap->addValueSpaceDependencies(stringValueSpaceData->getClashDependencyTrackPointCollection());
								stringValueSpaceData->setValueSpaceClashed(true);
								return true;
							}
						}
					}
					return false;
				}



				bool CIndividualProcessNodeStringValueSpaceHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	

					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,false);
					if (stringValueSpaceData) {
						if (!stringValueSpaceData->isValueSpaceClashed()) {
							CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(false);
							if (stringValueSpaceMap) {
								return stringValueSpaceMap->addValueSpaceDependencies(depCollection);
							}
						} else {
							return stringValueSpaceData->getClashDependencyTrackPointCollection()->addCollectionDependencies(depCollection);
						}
					}
					return false;
				}


				bool CIndividualProcessNodeStringValueSpaceHandler::countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext) {
					bool counted = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	
					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,false);
					if (stringValueSpaceData) {
						if (!stringValueSpaceData->isValueSpaceClashed()) {
							if (!stringValueSpaceData->isValueSpaceCounted() || stringValueSpaceData->isValueSpaceCountingRequired()) {
								stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,true);
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = stringValueSpaceData->getValuesCounter();
								valueSpaceValueCounter->resetValueCounter();
								CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(false);
								if (stringValueSpaceMap) {
									stringValueSpaceMap->countAllValues(valueSpaceValueCounter);
								} else {
									valueSpaceValueCounter->incInfinite();
								}
								stringValueSpaceData->setValueSpaceCounted(true);
								stringValueSpaceData->setValueSpaceCountingRequired(false);
								if (valueCounter) {
									counted = valueCounter->combineWithValueCounter(valueSpaceValueCounter);
								}
							} else {
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = stringValueSpaceData->getValuesCounter();
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


				CConceptDescriptor* CIndividualProcessNodeStringValueSpaceHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					bool substractValueSpaceValuesCount = false;

					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,false);
					if (!stringValueSpaceData || (!stringValueSpaceData->isValueSpaceClashed() && !stringValueSpaceData->isValueSpaceTriggeringCompleted())) {
						stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,true);
						bool valueSpaceTriggeringFinished = false;
						if (!stringValueSpaceData->isValueSpaceTriggeringStarted()) {
							stringValueSpaceData->setValueSpaceTriggeringStarted(true);
						}
						CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(false);

						CDatatypeValueSpacesTriggers* valueSpacesTriggers = ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
						if (valueSpacesTriggers) {

							CDatatypeValueSpaceStringTriggers* stringValueSpaceTriggers = valueSpacesTriggers->getStringValueSpaceTriggers();
							CDatatypeValueSpaceTriggeringMap* stringValueSpaceTriggerMap = stringValueSpaceTriggers->getValueSpaceTriggeringMap();

							depCollection->addDependency(stringValueSpaceData->getValueSpaceInitialisationDependencyTrackPoint());

							CDatatypeValueSpaceConceptTriggeringData* valueSpaceConceptTriggeringData = stringValueSpaceTriggers->getStringConceptTriggeringData();
							if (valueSpaceConceptTriggeringData->hasPartialConceptTriggers()) {
								conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,valueSpaceConceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
							}


							if (stringValueSpaceMap) {


								CDataLiteralStringValue* minValue = nullptr;
								bool minValueInclusive = false;
								CDependencyTrackPoint* minValueDepTrackPoint = nullptr;




								if (stringValueSpaceMap->getAbsoluteMinimumValue(minValue,minValueInclusive,&minValueDepTrackPoint)) {

									CDatatypeDependencyCollection minValueDepCollection(depCollection,calcAlgContext);

									bool minDatatypeDepAdded = false;

									CDatatypeValueSpaceTriggeringIterator leftTriggerIt = stringValueSpaceTriggerMap->getLeftTriggeringIterator(minValue,!minValueInclusive);
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

									CDatatypeValueSpaceTriggeringData* minValueTriggerData = stringValueSpaceTriggerMap->getDatatypeValueTriggeringData(minValue,false);
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




								CDataLiteralStringValue* maxValue = nullptr;
								bool maxValueInclusive = false;
								CDependencyTrackPoint* maxValueDepTrackPoint = nullptr;
								if (stringValueSpaceMap->getAbsoluteMaximumValue(maxValue,maxValueInclusive,&maxValueDepTrackPoint)) {

									CDatatypeDependencyCollection maxValueDepCollection(depCollection,calcAlgContext);

									bool maxDatatypeDepAdded = false;

									CDatatypeValueSpaceTriggeringIterator rightTriggerIt = stringValueSpaceTriggerMap->getRightTriggeringIterator(maxValue,!maxValueInclusive);
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


									CDatatypeValueSpaceTriggeringData* maxValueTriggerData = stringValueSpaceTriggerMap->getDatatypeValueTriggeringData(maxValue,false);
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

									CDatatypeValueSpaceStringValuesCounter valueCounter;


									CDataLiteralCompareValue* freeLeftValue = nullptr;
									CDataLiteralCompareValue* freeRightValue = nullptr;
									bool freeLeftValueInclusive = false;
									bool freeRightValueInclusive = false;

									if (stringValueSpaceTriggerMap->getIntervalMinMaxTriggerFreeInterval(minValue,minValueInclusive,maxValue,maxValueInclusive,freeLeftValue,freeLeftValueInclusive,freeRightValue,freeRightValueInclusive)) {
										CDataLiteralStringValue* freeLeftStringValue = dynamic_cast<CDataLiteralStringValue*>(freeLeftValue);
										CDataLiteralStringValue* freeRightStringValue = dynamic_cast<CDataLiteralStringValue*>(freeRightValue);
										if (freeLeftStringValue && freeRightStringValue) {
											CDatatypeValueSpaceStringValuesCounter tmpValueCounter;
											if (stringValueSpaceMap->countIntervalValues(freeLeftStringValue,freeLeftValueInclusive,freeRightStringValue,freeRightValueInclusive,&tmpValueCounter)) {
												cint64 triggerValueCount = 0;
												if (stringValueSpaceTriggerMap->countIntervalValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,triggerValueCount)) {
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
										stringValueSpaceTriggerMap->countIntervalMinMaxValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,minTriggerCount,maxTriggerCount,valueTriggerCount);

										if (minTriggerCount < maxTriggerCount) {
											minToMaxTriggerDirection = false;
										}

										depCollection->addDependency(minValueDepTrackPoint);
										depCollection->addDependency(maxValueDepTrackPoint);

										CDatatypeValueSpaceTriggeringIterator triggerIt = stringValueSpaceTriggerMap->getIntervalTriggeringIterator(minValue,minValueInclusive,maxValue,maxValueInclusive,minToMaxTriggerDirection);

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
											CDataLiteralStringValue* lastStringValue = nullptr;
											if (minToMaxTriggerDirection) {
												lastStringValue = minValue;
											} else {
												lastStringValue = maxValue;
											}


											CDatatypeValueSpaceTriggeringIterator stepMinMaxDirectionTriggerIt = triggerIt;
											while (!valueCounter.hasValueAchieved(remainingRequiredValuesCount) && stepMinMaxDirectionTriggerIt.hasNext()) {

												CDatatypeValueSpaceTriggeringData* triggeringData = stepMinMaxDirectionTriggerIt.next();
												CDataLiteralCompareValue* currentValue = triggeringData->getValue();
												CDataLiteralStringValue* currentStringValue = dynamic_cast<CDataLiteralStringValue*>(currentValue);


												if (lastStringValue && !lastStringValue->isEqualTo(currentStringValue)) {
													if (minToMaxTriggerDirection) {
														stringValueSpaceMap->countIntervalValues(lastStringValue,false,currentStringValue,false,&valueCounter);
														stringValueSpaceMap->addIntervalExclusionDependencies(lastStringValue,false,currentStringValue,false,depCollection);
													} else {
														stringValueSpaceMap->countIntervalValues(currentStringValue,false,lastStringValue,false,&valueCounter);
														stringValueSpaceMap->addIntervalExclusionDependencies(currentStringValue,false,lastStringValue,false,depCollection);
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
													stringValueSpaceMap->countValueValues(currentStringValue,&valueCounter);
													stringValueSpaceMap->addValueExclusionDependencies(currentStringValue,depCollection);
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
											CDataLiteralStringValue* currentStringValue = dynamic_cast<CDataLiteralStringValue*>(currentValue);

											if (triggeringData->getDirectValueTriggeringData()->hasPartialConceptTriggers()) {
												stringValueSpaceMap->addValueExclusionDependencies(currentStringValue,depCollection);
												if (!stringValueSpaceMap->isValueExcluded(currentStringValue,nullptr)) {
													stringValueSpaceMap->countValueValues(currentStringValue,&valueCounter);

													CDatatypeValueSpaceConceptTriggeringData* directValueTriggeringConceptData = triggeringData->getDirectValueTriggeringData();
													if (directValueTriggeringConceptData->hasPartialConceptTriggers()) {
														conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,directValueTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
													}
												}
											}
										}



									}





									if (!valueCounter.hasValueAchieved(CDatatypeValueSpaceStringValuesCounter::SVT_WITH_LANGUAG_TAG_COUNT,remainingRequiredValuesCount)) {
										// trigger non-language stings
										CDatatypeValueSpaceConceptTriggeringData* conceptTriggeringData = stringValueSpaceTriggers->getNonLanguageTagConceptTriggeringData();
										if (conceptTriggeringData->hasPartialConceptTriggers()) {
											if (depCollection && !depCollection->hasDependencies()) {
												stringValueSpaceMap->addValueSpaceDependencies(depCollection);
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
							if (!stringValueSpaceData->isValueSpaceTriggeringCompleted()) {
								stringValueSpaceData->setValueSpaceTriggeringCompleted(true);
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




				bool CIndividualProcessNodeStringValueSpaceHandler::getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CDataLiteralStringValue* lastStringValue = nullptr;
					if (lastValue) {
						lastStringValue = dynamic_cast<CDataLiteralStringValue*>(lastValue);
					}


					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();

					CDatatypeStringValueSpaceData* stringValueSpaceData = datatypesSpaceValue->getStringValueSpace(mStringValueSpaceType,true);
					if (stringValueSpaceData && !stringValueSpaceData->isValueSpaceClashed()) {
						CDataLiteralValue* tmpNextValue = valueTestingCollection->createDataLiteralValue(mStringValueSpaceType);
						CDataLiteralStringValue* nextStringValue = dynamic_cast<CDataLiteralStringValue*>(tmpNextValue);

						CDataLiteralValue* tmpLastValue = nullptr;
						CDataLiteralStringValue* tmpLastStringValue = nullptr;
						if (lastStringValue) {
							tmpLastValue = valueTestingCollection->createDataLiteralValue(mStringValueSpaceType);
							tmpLastStringValue = dynamic_cast<CDataLiteralStringValue*>(tmpLastValue);	
							tmpLastStringValue->initValue(lastStringValue);
						}

						CDatatypeStringValueSpaceMap* stringValueSpaceMap = stringValueSpaceData->getValueSpaceMap(true);
						bool searchNextValue = true;
						while (searchNextValue) {
							if (stringValueSpaceMap->getNextPossibleDataValue(nextStringValue,tmpLastStringValue)) {
								if (!valueTestingCollection->isDataLiteralValueUsed(tmpNextValue,mStringValueSpaceType)) {
									valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mStringValueSpaceType);
									nextValue = tmpNextValue;
									return true;
								}
							} else {
								searchNextValue = false;
							}
							if (!tmpLastStringValue) {
								tmpLastValue = valueTestingCollection->createDataLiteralValue(mStringValueSpaceType);
								tmpLastStringValue = dynamic_cast<CDataLiteralStringValue*>(tmpLastValue);	
								tmpLastStringValue->initValue(nextStringValue);
							} else {
								tmpLastStringValue->initValue(nextStringValue);
							}
						}
						if (!searchNextValue) {
							valueTestingCollection->releaseDataLiteralValue(tmpNextValue,mStringValueSpaceType);
							valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mStringValueSpaceType);
						}
					}
					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
