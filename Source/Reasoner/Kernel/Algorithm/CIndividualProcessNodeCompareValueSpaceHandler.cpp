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

#include "CIndividualProcessNodeCompareValueSpaceHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndividualProcessNodeCompareValueSpaceHandler::CIndividualProcessNodeCompareValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType) : CIndividualProcessNodeValueSpaceHandler(valueSpaceType) {
					mCompareValueSpaceType = (CDatatypeValueSpaceCompareType*)valueSpaceType;
				}



				bool CIndividualProcessNodeCompareValueSpaceHandler::triggerDataLiteralConcept(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
					CDataLiteralCompareValue* dataLitCompareValue = dynamic_cast<CDataLiteralCompareValue*>(dataLitValue);
					CMemoryTemporaryAllocationManager* taskMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType, true);
					if (!compareValueSpaceData->isValueSpaceClashed()) {
						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);
						if (otherValueSpacesClosed && directlyTriggeredFlag && compareValueSpaceMap->hasExcludedMinimum(dataLitCompareValue,false) && compareValueSpaceMap->hasExcludedMaximum(dataLitCompareValue,false)) {
							compareValueSpaceMap->addValueSpaceDependencies(depCollection);
							*directlyTriggeredFlag = true;
							return false;
						}

						CDatatypeValueSpacesTriggers* valueSpacesTriggers = datatypesSpaceValue->getCustomValueSpacesTriggers(ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false),true);
						CDatatypeValueSpaceCompareTriggers* compareValueSpaceTriggers = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(mCompareValueSpaceType);
						CDatatypeValueSpaceConceptTriggerLinker* conceptTriggerLinker = CObjectAllocator< CDatatypeValueSpaceConceptTriggerLinker >::allocateAndConstruct(taskMemMan);
						conceptTriggerLinker->initConceptTrigger(triggerConcept);
						compareValueSpaceTriggers->addValueConceptTrigger(dataLitCompareValue, conceptTriggerLinker);
						compareValueSpaceData->setValueSpaceTriggeringRequired(true);
						compareValueSpaceData->setValueSpaceTriggeringStarted(false);
						compareValueSpaceData->setValueSpaceTriggeringCompleted(false);
						return true;
					}
					return false;
				}


				bool CIndividualProcessNodeCompareValueSpaceHandler::triggerDatatypeConcept(CIndividualProcessNode* indiProcNode, CDatatype* datatype, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CMemoryTemporaryAllocationManager* taskMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType, true);
					if (!compareValueSpaceData->isValueSpaceClashed()) {

						if (otherValueSpacesClosed) {
							*directlyTriggeredFlag = true;
							return false;
						}

						CDatatypeValueSpacesTriggers* valueSpacesTriggers = datatypesSpaceValue->getCustomValueSpacesTriggers(ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false), true);
						CDatatypeValueSpaceCompareTriggers* compareValueSpaceTriggers = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(mCompareValueSpaceType);
						CDatatypeValueSpaceConceptTriggerLinker* conceptTriggerLinker = CObjectAllocator< CDatatypeValueSpaceConceptTriggerLinker >::allocateAndConstruct(taskMemMan);
						conceptTriggerLinker->initConceptTrigger(triggerConcept);
						compareValueSpaceTriggers->addValueSpaceConceptTrigger(conceptTriggerLinker);
						compareValueSpaceData->setValueSpaceTriggeringRequired(true);
						compareValueSpaceData->setValueSpaceTriggeringStarted(false);
						compareValueSpaceData->setValueSpaceTriggeringCompleted(false);
						return true;
					}
					return false;
				}


				bool CIndividualProcessNodeCompareValueSpaceHandler::triggerDataRestrictionConcept(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 restrictionCode = dataRestricionConcept->getParameter();
					CDatatype* datatype = dataRestricionConcept->getDatatype();
					CDataLiteral* dataLiteral = dataRestricionConcept->getDataLiteral();
					CDataLiteralValue* restrictionDataLitValue = nullptr;
					if (dataLiteral) {
						restrictionDataLitValue = dataLiteral->getDataLiteralValue();
					}
					CDataLiteralCompareValue* restrictionDataLitCompareValue = nullptr;
					if (restrictionDataLitValue) {
						restrictionDataLitCompareValue = dynamic_cast<CDataLiteralCompareValue*>(restrictionDataLitValue);
					}
					CMemoryTemporaryAllocationManager* taskMemMan = calcAlgContext->getUsedTemporaryMemoryAllocationManager();
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType, true);
					if (!compareValueSpaceData->isValueSpaceClashed()) {
						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);
						bool directTriggering = false;
						if (restrictionDataLitCompareValue) {
							if (otherValueSpacesClosed && directlyTriggeredFlag) {
								if (!negated) {
									if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMinimum(restrictionDataLitCompareValue, true)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(mCompareValueSpaceType->getMinimumDataLiteralCompareValue(), true, restrictionDataLitCompareValue, true, depCollection);
										}
									} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMinimum(restrictionDataLitCompareValue, false)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(mCompareValueSpaceType->getMinimumDataLiteralCompareValue(), true, restrictionDataLitCompareValue, false, depCollection);
										}
									} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMaximum(restrictionDataLitCompareValue, false)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(restrictionDataLitCompareValue, false, mCompareValueSpaceType->getMaximumDataLiteralCompareValue(), true, depCollection);
										}
									} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMaximum(restrictionDataLitCompareValue, true)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(restrictionDataLitCompareValue, true, mCompareValueSpaceType->getMaximumDataLiteralCompareValue(), true, depCollection);
										}
									}
								} else {
									if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMaximum(restrictionDataLitCompareValue, false)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(restrictionDataLitCompareValue, false, mCompareValueSpaceType->getMaximumDataLiteralCompareValue(), true, depCollection);
										}
									} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMaximum(restrictionDataLitCompareValue, true)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(restrictionDataLitCompareValue, true, mCompareValueSpaceType->getMaximumDataLiteralCompareValue(), true, depCollection);
										}
									} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMinimum(restrictionDataLitCompareValue, true)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(mCompareValueSpaceType->getMinimumDataLiteralCompareValue(), true, restrictionDataLitCompareValue, true, depCollection);
										}
									} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
										if (compareValueSpaceMap->hasExcludedMinimum(restrictionDataLitCompareValue, false)) {
											directTriggering = true;
											compareValueSpaceMap->addIntervalExclusionDependencies(mCompareValueSpaceType->getMinimumDataLiteralCompareValue(), true, restrictionDataLitCompareValue, false, depCollection);
										}
									}
								}
							}


							if (directTriggering) {
								*directlyTriggeredFlag = true;
								return false;
							}


							CDatatypeValueSpacesTriggers* valueSpacesTriggers = datatypesSpaceValue->getCustomValueSpacesTriggers(ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false), true);
							CDatatypeValueSpaceCompareTriggers* compareValueSpaceTriggers = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(mCompareValueSpaceType);
							CDatatypeValueSpaceConceptTriggerLinker* conceptTriggerLinker = CObjectAllocator< CDatatypeValueSpaceConceptTriggerLinker >::allocateAndConstruct(taskMemMan);
							conceptTriggerLinker->initConceptTrigger(triggerConcept);

							if (!negated) {
								if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
									compareValueSpaceTriggers->addMinValueConceptTrigger(restrictionDataLitCompareValue, true, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
									compareValueSpaceTriggers->addMinValueConceptTrigger(restrictionDataLitCompareValue, false, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
									compareValueSpaceTriggers->addMaxValueConceptTrigger(restrictionDataLitCompareValue, false, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
									compareValueSpaceTriggers->addMaxValueConceptTrigger(restrictionDataLitCompareValue, true, conceptTriggerLinker);
								}
							} else {
								if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
									compareValueSpaceTriggers->addMaxValueConceptTrigger(restrictionDataLitCompareValue, false, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
									compareValueSpaceTriggers->addMaxValueConceptTrigger(restrictionDataLitCompareValue, true, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
									compareValueSpaceTriggers->addMinValueConceptTrigger(restrictionDataLitCompareValue, true, conceptTriggerLinker);
								} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
									compareValueSpaceTriggers->addMinValueConceptTrigger(restrictionDataLitCompareValue, false, conceptTriggerLinker);
								}
							}
							compareValueSpaceData->setValueSpaceTriggeringRequired(true);
							compareValueSpaceData->setValueSpaceTriggeringStarted(false);
							compareValueSpaceData->setValueSpaceTriggeringCompleted(false);

						}

						return true;
					}
					return false;

				}



				bool CIndividualProcessNodeCompareValueSpaceHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDataLiteralValue* dataLitValue = dataLiteral->getDataLiteralValue();
					CDataLiteralCompareValue* dataLitCompareValue = dynamic_cast<CDataLiteralCompareValue*>(dataLitValue);

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,true);

					bool newValuesPotentiallyExcluded = false;

					if (!compareValueSpaceData->isValueSpaceClashed() && dataLitCompareValue) {
						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);
						if (!negated) {
							newValuesPotentiallyExcluded |= compareValueSpaceMap->restrictToValue(dataLitCompareValue,depTrackPoint);
						} else {
							newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeValue(dataLitCompareValue,depTrackPoint);
						}
					}
					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(compareValueSpaceData);
						compareValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}




				bool CIndividualProcessNodeCompareValueSpaceHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatype* datatype = dataRestricionConcept->getDatatype();
					cint64 restrictionCode = dataRestricionConcept->getParameter();
					CDataLiteral* dataLiteral = dataRestricionConcept->getDataLiteral();

					CDataLiteralValue* restrictionDataLitValue = nullptr;
					if (dataLiteral) {
						restrictionDataLitValue = dataLiteral->getDataLiteralValue();
					}
					CDataLiteralCompareValue* restrictionDataLitCompareValue = nullptr;
					if (restrictionDataLitValue) {
						restrictionDataLitCompareValue = dynamic_cast<CDataLiteralCompareValue*>(restrictionDataLitValue);
					}

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,true);

					if (compareValueSpaceData->isValueSpaceClashed()) {
						return false;
					}

					CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);

					bool newValuesPotentiallyExcluded = false;
					if (restrictionDataLitCompareValue) {
						if (!negated) {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMinimum(restrictionDataLitCompareValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMinimum(restrictionDataLitCompareValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMaximum(restrictionDataLitCompareValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMaximum(restrictionDataLitCompareValue,true,depTrackPoint);
							}
						} else {
							if (restrictionCode == CDFC_MIN_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMaximum(restrictionDataLitCompareValue,false,depTrackPoint);
							} else if (restrictionCode == CDFC_MIN_INCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMaximum(restrictionDataLitCompareValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_INCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMinimum(restrictionDataLitCompareValue,true,depTrackPoint);
							} else if (restrictionCode == CDFC_MAX_EXCLUSIVE) {
								newValuesPotentiallyExcluded |= compareValueSpaceMap->excludeMinimum(restrictionDataLitCompareValue,false,depTrackPoint);
							}
						}
					}

					if (newValuesPotentiallyExcluded) {
						datatypesSpaceValue->setValueSapceModified(compareValueSpaceData);
						compareValueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);
					}

					return newValuesPotentiallyExcluded;
				}









				bool CIndividualProcessNodeCompareValueSpaceHandler::testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,false);
					if (compareValueSpaceData) {
						if (compareValueSpaceData->isValueSpaceClashed()) {
							return true;
						}
						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(false);
						if (compareValueSpaceMap) {
							bool clashed = compareValueSpaceMap->testValueSpaceReturnClashed();
							if (clashed) {
								compareValueSpaceMap->addValueSpaceDependencies(compareValueSpaceData->getClashDependencyTrackPointCollection());
								compareValueSpaceData->setValueSpaceClashed(true);
								return true;
							}
						}
					}
					return false;
				}



				bool CIndividualProcessNodeCompareValueSpaceHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	

					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,false);
					if (compareValueSpaceData) {
						if (!compareValueSpaceData->isValueSpaceClashed()) {
							CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(false);
							if (compareValueSpaceMap) {
								return compareValueSpaceMap->addValueSpaceDependencies(depCollection);
							}
						} else {
							return compareValueSpaceData->getClashDependencyTrackPointCollection()->addCollectionDependencies(depCollection);
						}
					}
					return false;
				}


				bool CIndividualProcessNodeCompareValueSpaceHandler::countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext) {
					bool counted = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);	
					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,false);
					if (compareValueSpaceData) {
						if (!compareValueSpaceData->isValueSpaceClashed()) {
							if (!compareValueSpaceData->isValueSpaceCounted() || compareValueSpaceData->isValueSpaceCountingRequired()) {
								compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,true);
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = compareValueSpaceData->getValuesCounter();
								valueSpaceValueCounter->resetValueCounter();
								CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(false);
								if (compareValueSpaceMap) {
									compareValueSpaceMap->countAllValues(valueSpaceValueCounter);
								} else {
									if (mCompareValueSpaceType->hasInfiniteManyValues()) {
										valueSpaceValueCounter->incInfinite();
									} else {
										valueSpaceValueCounter->incValueCount(mCompareValueSpaceType->getMaximumValueCount());
									}
								}
								compareValueSpaceData->setValueSpaceCounted(true);
								compareValueSpaceData->setValueSpaceCountingRequired(false);
								if (valueCounter) {
									counted = valueCounter->combineWithValueCounter(valueSpaceValueCounter);
								}
							} else {
								CDatatypeValueSpaceValuesCounter* valueSpaceValueCounter = compareValueSpaceData->getValuesCounter();
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


				CConceptDescriptor* CIndividualProcessNodeCompareValueSpaceHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					bool substractValueSpaceValuesCount = false;

					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,false);
					if (!compareValueSpaceData || (!compareValueSpaceData->isValueSpaceClashed() && !compareValueSpaceData->isValueSpaceTriggeringCompleted())) {
						compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,true);
						bool valueSpaceTriggeringFinished = false;
						if (!compareValueSpaceData->isValueSpaceTriggeringStarted()) {
							compareValueSpaceData->setValueSpaceTriggeringStarted(true);
						}
						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);

						CDatatypeValueSpacesTriggers* valueSpacesTriggers = datatypesSpaceValue->getCustomValueSpacesTriggers(ontology->getDataBoxes()->getMBox()->getValueSpacesTriggers(false));
						if (valueSpacesTriggers) {

							CDatatypeValueSpaceCompareTriggers* compareValueSpaceTriggers = (CDatatypeValueSpaceCompareTriggers*)valueSpacesTriggers->getValueSpaceTriggers(mCompareValueSpaceType);
							CDatatypeValueSpaceTriggeringMap* compareValueSpaceTriggerMap = compareValueSpaceTriggers->getValueSpaceTriggeringMap();

							depCollection->addDependency(compareValueSpaceData->getValueSpaceInitialisationDependencyTrackPoint());

							CDatatypeValueSpaceConceptTriggeringData* valueSpaceConceptTriggeringData = compareValueSpaceTriggers->getValueSpaceConceptTriggeringData();
							if (valueSpaceConceptTriggeringData->hasPartialConceptTriggers()) {
								conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,valueSpaceConceptTriggeringData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
							}




							CDataLiteralCompareValue* minValue = nullptr;
							bool minValueInclusive = false;
							CDependencyTrackPoint* minValueDepTrackPoint = nullptr;


							if (compareValueSpaceMap->getAbsoluteMinimumValue(minValue,minValueInclusive,&minValueDepTrackPoint)) {


								CDatatypeDependencyCollection minValueDepCollection(depCollection,calcAlgContext);

								bool minDatatypeDepAdded = false;

								CDatatypeValueSpaceTriggeringIterator leftTriggerIt = compareValueSpaceTriggerMap->getLeftTriggeringIterator(minValue,!minValueInclusive);
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

								CDatatypeValueSpaceTriggeringData* minValueTriggerData = compareValueSpaceTriggerMap->getDatatypeValueTriggeringData(minValue,false);
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




							CDataLiteralCompareValue* maxValue = nullptr;
							bool maxValueInclusive = false;
							CDependencyTrackPoint* maxValueDepTrackPoint = nullptr;
							if (compareValueSpaceMap->getAbsoluteMaximumValue(maxValue,maxValueInclusive,&maxValueDepTrackPoint)) {

								CDatatypeDependencyCollection maxValueDepCollection(depCollection,calcAlgContext);

								bool maxDatatypeDepAdded = false;

								CDatatypeValueSpaceTriggeringIterator rightTriggerIt = compareValueSpaceTriggerMap->getRightTriggeringIterator(maxValue,!maxValueInclusive);
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


								CDatatypeValueSpaceTriggeringData* maxValueTriggerData = compareValueSpaceTriggerMap->getDatatypeValueTriggeringData(maxValue,false);
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

								CDatatypeValueSpaceValuesCounter valueCounter;


								CDataLiteralCompareValue* freeLeftValue = nullptr;
								CDataLiteralCompareValue* freeRightValue = nullptr;
								bool freeLeftValueInclusive = false;
								bool freeRightValueInclusive = false;

								if (compareValueSpaceTriggerMap->getIntervalMinMaxTriggerFreeInterval(minValue,minValueInclusive,maxValue,maxValueInclusive,freeLeftValue,freeLeftValueInclusive,freeRightValue,freeRightValueInclusive)) {
									if (freeLeftValue && freeRightValue) {
										CDatatypeValueSpaceValuesCounter tmpValueCounter;
										if (compareValueSpaceMap->countIntervalValues(freeLeftValue,freeLeftValueInclusive,freeRightValue,freeRightValueInclusive,&tmpValueCounter)) {
											cint64 triggerValueCount = 0;
											if (compareValueSpaceTriggerMap->countIntervalValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,triggerValueCount)) {
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
									compareValueSpaceTriggerMap->countIntervalMinMaxValueTriggers(minValue,minValueInclusive,maxValue,maxValueInclusive,minTriggerCount,maxTriggerCount,valueTriggerCount);

									if (minTriggerCount < maxTriggerCount) {
										minToMaxTriggerDirection = false;
									}

									depCollection->addDependency(minValueDepTrackPoint);
									depCollection->addDependency(maxValueDepTrackPoint);

									CDatatypeValueSpaceTriggeringIterator triggerIt = compareValueSpaceTriggerMap->getIntervalTriggeringIterator(minValue,minValueInclusive,maxValue,maxValueInclusive,minToMaxTriggerDirection);

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
										CDataLiteralCompareValue* lastValue = nullptr;
										if (minToMaxTriggerDirection) {
											lastValue = minValue;
										} else {
											lastValue = maxValue;
										}


										CDatatypeValueSpaceTriggeringIterator stepMinMaxDirectionTriggerIt = triggerIt;
										while (!valueCounter.hasValueAchieved(remainingRequiredValuesCount) && stepMinMaxDirectionTriggerIt.hasNext()) {

											CDatatypeValueSpaceTriggeringData* triggeringData = stepMinMaxDirectionTriggerIt.next();
											CDataLiteralCompareValue* currentValue = triggeringData->getValue();


											if (lastValue && !lastValue->isEqualTo(currentValue)) {
												if (minToMaxTriggerDirection) {
													compareValueSpaceMap->countIntervalValues(lastValue,false,currentValue,false,&valueCounter);
													compareValueSpaceMap->addIntervalExclusionDependencies(lastValue,false,currentValue,false,depCollection);
												} else {
													compareValueSpaceMap->countIntervalValues(currentValue,false,lastValue,false,&valueCounter);
													compareValueSpaceMap->addIntervalExclusionDependencies(currentValue,false,lastValue,false,depCollection);
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
												compareValueSpaceMap->countValueValues(currentValue,&valueCounter);
												compareValueSpaceMap->addValueExclusionDependencies(currentValue,depCollection);
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

										if (triggeringData->getDirectValueTriggeringData()->hasPartialConceptTriggers()) {
											compareValueSpaceMap->addValueExclusionDependencies(currentValue,depCollection);
											if (!compareValueSpaceMap->isValueExcluded(currentValue)) {
												compareValueSpaceMap->countValueValues(currentValue,&valueCounter);

												CDatatypeValueSpaceConceptTriggeringData* directValueTriggeringConceptData = triggeringData->getDirectValueTriggeringData();
												if (directValueTriggeringConceptData->hasPartialConceptTriggers()) {
													conceptTriggerLinker = addConceptLinkerCollectionDependency(indiProcNode,conceptTriggerLinker,directValueTriggeringConceptData->getPartialConceptTriggerLinker(),depCollection,calcAlgContext);
												}
											}
										}
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
							valueSpaceTriggeringFinished = true;
							substractValueSpaceValuesCount = true;
						}

						if (valueSpaceTriggeringFinished) {
							if (!compareValueSpaceData->isValueSpaceTriggeringCompleted()) {
								compareValueSpaceData->setValueSpaceTriggeringCompleted(true);
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




				bool CIndividualProcessNodeCompareValueSpaceHandler::getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CDataLiteralCompareValue* lastCompareValue = nullptr;
					if (lastValue) {
						lastCompareValue = dynamic_cast<CDataLiteralCompareValue*>(lastValue);
					}


					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();

					CDatatypeCompareValueSpaceData* compareValueSpaceData = (CDatatypeCompareValueSpaceData*)datatypesSpaceValue->getValueSpace(mCompareValueSpaceType,true);
					if (compareValueSpaceData && !compareValueSpaceData->isValueSpaceClashed()) {
						CDataLiteralValue* tmpNextValue = valueTestingCollection->createDataLiteralValue(mCompareValueSpaceType);
						CDataLiteralCompareValue* nextCompareValue = dynamic_cast<CDataLiteralCompareValue*>(tmpNextValue);

						CDataLiteralValue* tmpLastValue = nullptr;
						CDataLiteralCompareValue* tmpLastCompareValue = nullptr;
						if (lastCompareValue) {
							tmpLastValue = valueTestingCollection->createDataLiteralValue(mCompareValueSpaceType);
							tmpLastCompareValue = dynamic_cast<CDataLiteralCompareValue*>(tmpLastValue);	
							tmpLastCompareValue->initValue(lastCompareValue);
						}

						CDatatypeCompareValueSpaceMap* compareValueSpaceMap = compareValueSpaceData->getValueSpaceMap(true);
						bool searchNextValue = true;
						while (searchNextValue) {
							if (compareValueSpaceMap->getNextPossibleDataValue(nextCompareValue,tmpLastCompareValue)) {
								if (!valueTestingCollection->isDataLiteralValueUsed(tmpNextValue,mCompareValueSpaceType)) {
									valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mCompareValueSpaceType);
									nextValue = tmpNextValue;
									return true;
								}
							} else {
								searchNextValue = false;
							}
							if (!tmpLastCompareValue) {
								tmpLastValue = valueTestingCollection->createDataLiteralValue(mCompareValueSpaceType);
								tmpLastCompareValue = dynamic_cast<CDataLiteralCompareValue*>(tmpLastValue);	
								tmpLastCompareValue->initValue(nextCompareValue);
							} else {
								tmpLastCompareValue->initValue(nextCompareValue);
							}
						}
						if (!searchNextValue) {
							valueTestingCollection->releaseDataLiteralValue(tmpNextValue,mCompareValueSpaceType);
							valueTestingCollection->releaseDataLiteralValue(tmpLastValue,mCompareValueSpaceType);
						}
					}
					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
