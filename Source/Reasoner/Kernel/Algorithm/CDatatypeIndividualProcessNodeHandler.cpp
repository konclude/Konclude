/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CDatatypeIndividualProcessNodeHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CDatatypeIndividualProcessNodeHandler::CDatatypeIndividualProcessNodeHandler() {
					mConfBuildDependencies = true;

					CDatatypeValueSpaceTypes* valueSpaceTypes = CDatatypeValueSpaceTypes::getValueSpaceTypes();

					CDatatypeValueSpaceRealType* realValueSpaceType = valueSpaceTypes->getValueSpaceRealType();
					mValueSpaceHandlerArray[realValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeRealValueSpaceHandler(realValueSpaceType);
					CDatatypeValueSpaceUnknownType* unknownValueSpaceType = valueSpaceTypes->getValueSpaceUnkownType();
					mValueSpaceHandlerArray[unknownValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeUnknownValueSpaceHandler(unknownValueSpaceType);
					CDatatypeValueSpaceStringType* stringValueSpaceType = valueSpaceTypes->getValueSpaceStringType();
					mValueSpaceHandlerArray[stringValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeStringValueSpaceHandler(stringValueSpaceType);
					CDatatypeValueSpaceBooleanType* booleanValueSpaceType = valueSpaceTypes->getValueSpaceBooleanType();
					mValueSpaceHandlerArray[booleanValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeBooleanValueSpaceHandler(booleanValueSpaceType);
					CDatatypeValueSpaceDoubleType* doubleValueSpaceType = valueSpaceTypes->getValueSpaceDoubleType();
					mValueSpaceHandlerArray[doubleValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeDoubleValueSpaceHandler(doubleValueSpaceType);
					CDatatypeValueSpaceFloatType* floatValueSpaceType = valueSpaceTypes->getValueSpaceFloatType();
					mValueSpaceHandlerArray[floatValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeFloatValueSpaceHandler(floatValueSpaceType);
					CDatatypeValueSpaceIRIType* iriValueSpaceType = valueSpaceTypes->getValueSpaceIRIType();
					mValueSpaceHandlerArray[iriValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeIRIValueSpaceHandler(iriValueSpaceType);
					CDatatypeValueSpaceBinaryBase64DataType* binaryBase64DataValueSpaceType = valueSpaceTypes->getValueSpaceBinaryBase64DataType();
					mValueSpaceHandlerArray[binaryBase64DataValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeBinaryBase64DataValueSpaceHandler(binaryBase64DataValueSpaceType);
					CDatatypeValueSpaceBinaryHexDataType* binaryHexDataValueSpaceType = valueSpaceTypes->getValueSpaceBinaryHexDataType();
					mValueSpaceHandlerArray[binaryHexDataValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeBinaryHexDataValueSpaceHandler(binaryHexDataValueSpaceType);
					CDatatypeValueSpaceXMLType* xmlValueSpaceType = valueSpaceTypes->getValueSpaceXMLType();
					mValueSpaceHandlerArray[xmlValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeXMLValueSpaceHandler(xmlValueSpaceType);
					CDatatypeValueSpaceDateTimeType* dateTimeValueSpaceType = valueSpaceTypes->getValueSpaceDateTimeType();
					mValueSpaceHandlerArray[dateTimeValueSpaceType->getValueSpaceTypeIndex()] = new CIndividualProcessNodeDateTimeValueSpaceHandler(dateTimeValueSpaceType);
				}


				CDatatypeIndividualProcessNodeHandler::~CDatatypeIndividualProcessNodeHandler() {
				}




				bool CDatatypeIndividualProcessNodeHandler::addDatatype(CIndividualProcessNode* indiProcNode, CConcept* datatypeConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool potentiallyModified = false;
					CDatatype* datatype = datatypeConcept->getDatatype();

					if (datatype) {
						CDatatypeValueSpaceType* valueSpaceType = datatype->getValueSpaceType();
						if (valueSpaceType) {
							CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
							CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(valueSpaceType,true);
							valueSpaceData->setValueSpaceInitialisationDependencyTrackPointCandidate(depTrackPoint);

							if (!negated) {
								potentiallyModified |= closeOtherValueSpaces(indiProcNode,valueSpaceType,depTrackPoint,calcAlgContext);
							} else {
								potentiallyModified |= closeValueSpace(indiProcNode,valueSpaceType,depTrackPoint,calcAlgContext);
							}
						}
					}


					if (potentiallyModified) {
						processValueSpaceModified(indiProcNode,calcAlgContext);
					}
					return potentiallyModified;
				}



				bool CDatatypeIndividualProcessNodeHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool potentiallyModified = false;
					CDatatype* datatype = dataLiteral->getDatatype();

					if (datatype) {
						CDatatypeValueSpaceType* valueSpaceType = datatype->getValueSpaceType();
						if (valueSpaceType) {
							CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceType,calcAlgContext);
							potentiallyModified |= valueSpaceHandler->addDataLiteral(indiProcNode,dataLiteral,negated,depTrackPoint,calcAlgContext);
							if (!negated) {
								potentiallyModified |= closeOtherValueSpaces(indiProcNode,valueSpaceType,depTrackPoint,calcAlgContext);
							}
						}
					}
					if (potentiallyModified) {
						processValueSpaceModified(indiProcNode,calcAlgContext);
					}
					return potentiallyModified;
				}




				bool CDatatypeIndividualProcessNodeHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool potentiallyModified = false;
					CDatatype* datatype = dataRestricionConcept->getDatatype();

					cint64 restrictionCode = dataRestricionConcept->getParameter();
					CDataLiteral* dataLiteral = dataRestricionConcept->getDataLiteral();
					if (datatype) {
						CDatatypeValueSpaceType* valueSpaceType = datatype->getValueSpaceType();
						if (valueSpaceType) {
							if (dataLiteral && dataLiteral->getDatatype()) {
								if (valueSpaceType != dataLiteral->getDatatype()->getValueSpaceType()) {
									return false;
								}
							}
							CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceType,calcAlgContext);
							potentiallyModified |= valueSpaceHandler->addDataRestriction(indiProcNode,dataRestricionConcept,negated,depTrackPoint,calcAlgContext);
							if (!negated && restrictionCode > 0) {
								potentiallyModified |= closeOtherValueSpaces(indiProcNode,valueSpaceType,depTrackPoint,calcAlgContext);
							}
						}
					}
					if (potentiallyModified) {
						processValueSpaceModified(indiProcNode,calcAlgContext);
					}
					return potentiallyModified;
				}


				bool CDatatypeIndividualProcessNodeHandler::processValueSpaceModified(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					// test clashes
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);		
					if (datatypesSpaceValue->isValueSpaceClashCheckingRequired()) {
						datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
						datatypesSpaceValue->setValueSpaceClashCheckingRequired(false);
						testAllValueSpacesClashed(indiProcNode,calcAlgContext);
					}
					// queue triggering
					if (datatypesSpaceValue->isValueSpaceTriggeringRequired()) {
						if (!datatypesSpaceValue->isValueSpaceTriggeringQueued()) {
							CDatatypeValueSpacesTriggers* valueSpacesTriggers = calcAlgContext->getUsedProcessingDataBox()->getOntology()->getDataBoxes()->getMBox()->getValueSpacesTriggers(false);
							if (valueSpacesTriggers) {
								datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
								calcAlgContext->getUsedProcessingDataBox()->getValueSpaceTriggeringProcessingQueue(true)->insertProcessIndiviudal(indiProcNode);
								datatypesSpaceValue->setValueSpaceTriggeringQueued(true);
							}
						}
					}
					// queue satisfiability checking
					if (datatypesSpaceValue->isValueSpaceSatisfiabilityCheckingRequired()) {
						if (!datatypesSpaceValue->isValueSpaceSatisfiabilityCheckingQueued()) {
							datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
							CDatatypeValueSpaceValuesCounter valuesCounter;
							countPossibleValues(indiProcNode,&valuesCounter,calcAlgContext);
							cint64 disjointCount = getDisjointCount(indiProcNode,calcAlgContext);
							if (!valuesCounter.hasValueAchieved(disjointCount+1)) {
								// requires detailed satisfiability checking
								calcAlgContext->getUsedProcessingDataBox()->getDistinctValueSpaceSatisfiabilityCheckingQueue(true)->insertProcessIndiviudal(indiProcNode);
								datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingQueued(true);
							} else {
								datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(false);
							}
						}
					}
					return true;
				}





				bool CDatatypeIndividualProcessNodeHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor*& triggeredConcepts, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 distinctCount = getDisjointCount(indiProcNode,calcAlgContext);
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConceptDescriptor* conceptTriggerLinker = nullptr;
					bool triggered = false;

					if (datatypesSpaceValue->isValueSpaceTriggeringRequired() || distinctCount > datatypesSpaceValue->getLastTriggeringDisjointCount()) {
						datatypesSpaceValue->setLastTriggeringDisjointCount(distinctCount);

						cint64 remainingRequiredValueCount = distinctCount+1;

						CDatatypeDependencyCollection depCollection(calcAlgContext);
						addDistinctDependencies(indiProcNode,&depCollection,calcAlgContext);

						CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
						CDatatypeValueSpaceType* valueSpaceTypeLinker = ontology->getDataBoxes()->getDatatypeValueSpaceTypes()->getValueSpaceTypeLinker();
						for (CDatatypeValueSpaceType* valueSpaceTypeIt = valueSpaceTypeLinker; remainingRequiredValueCount > 0 && valueSpaceTypeIt; valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
							CDatatypeValueSpaceType* valueSpaceType = valueSpaceTypeIt;
							CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(valueSpaceType,false);
							if (valueSpaceData && valueSpaceData->isValueSpaceClashed()) {
								addValueSpaceDependencies(indiProcNode,valueSpaceData,valueSpaceType,&depCollection,calcAlgContext);
							} else {
								conceptTriggerLinker = triggerValueSpaceConcepts(indiProcNode,conceptTriggerLinker,remainingRequiredValueCount,valueSpaceData,valueSpaceType,&depCollection,calcAlgContext);
							}
						}

						datatypesSpaceValue->setValueSpaceTriggeringRequired(false);
						triggered = true;
					}
					datatypesSpaceValue->setValueSpaceTriggeringQueued(false);

					triggeredConcepts = conceptTriggerLinker;
					return triggered;
				}




				bool CDatatypeIndividualProcessNodeHandler::notifyDistinctChanges(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					bool queued = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);
					if (datatypesSpaceValue) {

						if (!datatypesSpaceValue->isValueSpaceTriggeringQueued()) {
							cint64 disjointCount = getDisjointCount(indiProcNode,calcAlgContext);
							if (datatypesSpaceValue->isValueSpaceTriggeringRequired() || datatypesSpaceValue->getLastTriggeringDisjointCount() < disjointCount) {
								datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
								calcAlgContext->getUsedProcessingDataBox()->getValueSpaceTriggeringProcessingQueue(true)->insertProcessIndiviudal(indiProcNode);
								datatypesSpaceValue->setValueSpaceTriggeringQueued(true);
								datatypesSpaceValue->setValueSpaceTriggeringRequired(true);
								queued = true;
							}
						}

						if (!datatypesSpaceValue->isValueSpaceSatisfiabilityCheckingQueued()) {
							CDatatypeValueSpaceValuesCounter valuesCounter;
							countPossibleValues(indiProcNode,&valuesCounter,calcAlgContext);
							cint64 disjointCount = getDisjointCount(indiProcNode,calcAlgContext);
							if (datatypesSpaceValue->isValueSpaceSatisfiabilityCheckingRequired() || !valuesCounter.hasValueAchieved(disjointCount+1)) {
								datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
								calcAlgContext->getUsedProcessingDataBox()->getDistinctValueSpaceSatisfiabilityCheckingQueue(true)->insertProcessIndiviudal(indiProcNode);
								datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingQueued(true);
								datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(true);
								queued = true;
							}
						}

					}
					return queued;
				}




				bool CDatatypeIndividualProcessNodeHandler::involveDistinctNodeForSatisfiabilityChecking(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);
					if (datatypesSpaceValue) {
						cint64 requiredValueCount = getDisjointCount(indiProcNode,calcAlgContext)+1;
						CDatatypeValueSpaceValuesCounter valuesCounter;
						countPossibleValues(indiProcNode,&valuesCounter,calcAlgContext);
						if (!valuesCounter.hasValueAchieved(requiredValueCount)) {
							return true;
						}
						datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
						datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(false);
					}
					return false;				
				}


				bool CDatatypeIndividualProcessNodeHandler::requiresSatisfiabilityChecking(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);
					if (datatypesSpaceValue) {
						if (datatypesSpaceValue->isValueSpaceSatisfiabilityCheckingRequired()) {

							cint64 requiredValueCount = getDisjointCount(indiProcNode,calcAlgContext)+1;
							CDatatypeValueSpaceValuesCounter valuesCounter;
							countPossibleValues(indiProcNode,&valuesCounter,calcAlgContext);
							if (!valuesCounter.hasValueAchieved(requiredValueCount)) {
								return true;
							}
							datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
							datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(false);
						}
					}
					return false;
				}





				bool CDatatypeIndividualProcessNodeHandler::checkSatisfiability(CIndividualProcessNode* indiProcNode, CPROCESSINGSET<CIndividualProcessNode*>* distinctIndividualNodeSet, CCalculationAlgorithmContext* calcAlgContext) {


					indiProcNode->getDatatypesValueSpaceData(true)->setValueSpaceSatisfiabilityCheckingQueued(false);

					if (distinctIndividualNodeSet->size() > 1) {

						CTaskProcessorContext* taskProcessorContext = calcAlgContext->getUsedTaskProcessorContext();
						CIndividualProcessNodeVector* indiNodeVector = calcAlgContext->getProcessingDataBox()->getIndividualProcessNodeVector();
						CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
						CDatatypeValueSpaceTypes* valueSpaceTypes = ontology->getDataBoxes()->getDatatypeValueSpaceTypes();
						CDatatypeValueSpaceType* valueSpaceLinker = valueSpaceTypes->getValueSpaceTypeLinker();


						CPROCESSINGMAP<cint64,CIndividualProcessNode*>* sortedDistinctNodeMap = CObjectParameterizingAllocator< CPROCESSINGMAP<cint64,CIndividualProcessNode*>,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);
						for (CPROCESSINGSET<CIndividualProcessNode*>::const_iterator it = distinctIndividualNodeSet->constBegin(), itEnd = distinctIndividualNodeSet->constEnd(); it != itEnd; ++it) {
							CIndividualProcessNode* distinctIndiNode(*it);
							cint64 distinctIndiID = distinctIndiNode->getIndividualID();
							sortedDistinctNodeMap->insert(distinctIndiID,distinctIndiNode);
						}


						CDatatypeValueTestingCollection valueTestingCollection(calcAlgContext);
						CPROCESSINGHASH<cint64,CDataLiteralValue*>* nodeValueAssociationHash = CObjectParameterizingAllocator< CPROCESSINGHASH<cint64,CDataLiteralValue*>,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);
						CPROCESSINGHASH<cint64,CDatatypeValueSpaceType*>* nodeValueTypeAssociationHash = CObjectParameterizingAllocator< CPROCESSINGHASH<cint64,CDatatypeValueSpaceType*>,CContext* >::allocateAndConstructAndParameterize(taskProcessorContext->getMemoryAllocationManager(),taskProcessorContext);



						CPROCESSINGMAP<cint64,CIndividualProcessNode*>::iterator firstValueit = sortedDistinctNodeMap->begin();
						cint64 firstIndiNodeID = firstValueit.key();
						CIndividualProcessNode* firstIndiNode = firstValueit.value();
						sortedDistinctNodeMap->erase(firstValueit);
						CDatatypeValueSpaceType* firstIndiNodeValueSpaceType = valueSpaceLinker;
						CDataLiteralValue* firstIndiNextValue = nullptr;
						CDataLiteralValue* firstIndiLastValue = nullptr;


						bool foundSatisfiableValues = false;
						bool moreValuesPossible = true;
						while (moreValuesPossible && !foundSatisfiableValues) {
							valueTestingCollection.releaseUsedDataLiteralValues();
							CDatatypeValueSpaceType* firstIndiNodeLastValueSpaceType = firstIndiNodeValueSpaceType;
							bool firstNodeFoundValue = seachNextPossibleDataValue(firstIndiNode,firstIndiNextValue,firstIndiNodeValueSpaceType,firstIndiLastValue,&valueTestingCollection,calcAlgContext);
							if (firstNodeFoundValue) {
								if (firstIndiLastValue) {
									valueTestingCollection.releaseDataLiteralValue(firstIndiLastValue,firstIndiNodeLastValueSpaceType);
								}
								firstIndiLastValue = firstIndiNextValue;

								for (CPROCESSINGMAP<cint64,CIndividualProcessNode*>::const_iterator it = sortedDistinctNodeMap->constBegin(), itEnd = sortedDistinctNodeMap->constEnd(); it != itEnd; ++it) {
									cint64 indiNodeID = it.key();
									CIndividualProcessNode* indiNode = it.value();
									nodeValueAssociationHash->insert(indiNodeID,nullptr);
									nodeValueTypeAssociationHash->insert(indiNodeID,valueSpaceLinker);
								}


								nodeValueAssociationHash->insert(firstIndiNodeID,firstIndiLastValue);
								nodeValueTypeAssociationHash->insert(firstIndiNodeID,firstIndiNodeValueSpaceType);



								bool allNodesFoundValue = true;

								for (CPROCESSINGMAP<cint64,CIndividualProcessNode*>::const_iterator it = sortedDistinctNodeMap->constBegin(), itEnd = sortedDistinctNodeMap->constEnd(); allNodesFoundValue && it != itEnd; ++it) {
									valueTestingCollection.releaseUsedDataLiteralValues();
									cint64 indiNodeID = it.key();
									CIndividualProcessNode* indiNode = it.value();
									CDataLiteralValue* nodeLastTestingValue = nodeValueAssociationHash->value(indiNodeID);
									CDatatypeValueSpaceType* nodeValueSpaceType = nodeValueTypeAssociationHash->value(indiNodeID);
									CDataLiteralValue* nodeNextTestingValue = nullptr;


									CDistinctHash* distinctHash = indiNode->getDistinctHash(false);
									if (distinctHash) {
										CDistinctIterator distinctIterator = distinctHash->getDistinctIterator();
										while (distinctIterator.hasNext()) {
											cint64 distinctIndiID = distinctIterator.nextDistinctIndividualID();
											if (distinctIndiID < indiNodeID && sortedDistinctNodeMap->contains(distinctIndiID)) {
												CDataLiteralValue* distinctValue = nodeValueAssociationHash->value(distinctIndiID);
												CDatatypeValueSpaceType* distinctValueSpaceType = nodeValueTypeAssociationHash->value(distinctIndiID);
												valueTestingCollection.addDataLiteralValueUse(distinctValue,distinctValueSpaceType);
											}
										}
									}

									
									CDatatypeValueSpaceType* nodeLastValueSpaceType = nodeValueSpaceType;
									bool foundValue = seachNextPossibleDataValue(indiNode,nodeNextTestingValue,nodeValueSpaceType,nodeLastTestingValue,&valueTestingCollection,calcAlgContext);
									if (!foundValue) {
										allNodesFoundValue = false;
									} else {
										if (nodeLastTestingValue) {
											valueTestingCollection.releaseDataLiteralValue(nodeLastTestingValue,nodeLastValueSpaceType);
										}
										nodeValueAssociationHash->insert(indiNodeID,nodeNextTestingValue);
										nodeValueTypeAssociationHash->insert(indiNodeID,nodeValueSpaceType);
									}
								}


								if (allNodesFoundValue) {
									foundSatisfiableValues = true;
								}


							} else {
								moreValuesPossible = false;
							}
						}


						if (foundSatisfiableValues) {
							// mark for all nodes that value space satisfiability is checked

							for (CPROCESSINGSET<CIndividualProcessNode*>::const_iterator it = distinctIndividualNodeSet->constBegin(), itEnd = distinctIndividualNodeSet->constEnd(); it != itEnd; ++it) {
								CIndividualProcessNode* distinctIndiNode(*it);
								CDatatypesValueSpaceData* datatypesSpaceValue = distinctIndiNode->getDatatypesValueSpaceData(true);		
								datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(false);
							}

							CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
							datatypesSpaceValue->setValueSpaceSatisfiabilityCheckingRequired(false);
						} else {
							// create clash
							CDatatypeDependencyTrackPointCollection clashedDepTrackPointCollection(calcAlgContext->getUsedProcessContext());
							for (CPROCESSINGSET<CIndividualProcessNode*>::const_iterator it = distinctIndividualNodeSet->constBegin(), itEnd = distinctIndividualNodeSet->constEnd(); it != itEnd; ++it) {
								CIndividualProcessNode* distinctIndiNode(*it);
								addValueSpacesDependencies(distinctIndiNode,&clashedDepTrackPointCollection,calcAlgContext);
								addDistinctDependencies(distinctIndiNode,&clashedDepTrackPointCollection,calcAlgContext);
							}

							CClashedDependencyDescriptor* depDesLinker = nullptr;
							depDesLinker = copyClashedDependencyDescriptors(depDesLinker,clashedDepTrackPointCollection.getClashedDependencyDescriptors(),calcAlgContext);
							throwDatatypeValueSpaceExclusionClash(depDesLinker,indiProcNode,calcAlgContext);

						}
					}



					return true;
				}


				bool CDatatypeIndividualProcessNodeHandler::addDistinctDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {

					bool dependenciesAdded = false;
					cint64 indiNodeID = indiProcNode->getIndividualID();
					CDistinctHash* distinctHash = indiProcNode->getDistinctHash(false);
					if (distinctHash) {
						CDistinctIterator distinctIterator = distinctHash->getDistinctIterator();
						while (distinctIterator.hasNext()) {
							CDistinctEdge* distinctEdge = distinctIterator.next();
							CDependencyTrackPoint* depTrackPoint = distinctEdge->getDependencyTrackPoint();
							dependenciesAdded |= depCollection->addDependency(depTrackPoint);
						}
					}
					return dependenciesAdded;
				}



				bool CDatatypeIndividualProcessNodeHandler::seachNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDatatypeValueSpaceType*& nextValueSpaceType, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					for (; nextValueSpaceType; nextValueSpaceType = nextValueSpaceType->getNextValueSpaceType()) {
						CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(nextValueSpaceType,false);
						if (!valueSpaceData->isValueSpaceClashed()) {
							CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(nextValueSpaceType,calcAlgContext);
							if (valueSpaceHandler->getNextPossibleDataValue(indiProcNode,nextValue,lastValue,valueTestingCollection,calcAlgContext)) {
								return true;
							}
						}
					}

					return false;
				}



				bool CDatatypeIndividualProcessNodeHandler::countPossibleValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valuesCounter, CCalculationAlgorithmContext* calcAlgContext) {

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);		

					if (!datatypesSpaceValue->isValueSpaceCounted() || datatypesSpaceValue->isValueSpaceCountingRequired()) {
						datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);
						datatypesSpaceValue->setValueSpaceCounted(true);
						datatypesSpaceValue->setValueSpaceCountingRequired(false);

						CDatatypeValueSpaceValuesCounter* valueSpacesValuesCounter = datatypesSpaceValue->getValuesCounter();
						valueSpacesValuesCounter->resetValueCounter();

						CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
						CDatatypeValueSpaceType* valueSpaceTypeLinker = ontology->getDataBoxes()->getDatatypeValueSpaceTypes()->getValueSpaceTypeLinker();
						for (CDatatypeValueSpaceType* valueSpaceTypeIt = valueSpaceTypeLinker; valueSpaceTypeIt && !valueSpacesValuesCounter->isInfinite(); valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
							CDatatypeValueSpaceType* valueSpaceType = valueSpaceTypeIt;
							CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceType,calcAlgContext);
							valueSpaceHandler->countPossibleValueSpaceValues(indiProcNode,valueSpacesValuesCounter,calcAlgContext);
						}
					}

					bool counted = false;
					if (valuesCounter) {
						CDatatypeValueSpaceValuesCounter* valueSpacesValuesCounter = datatypesSpaceValue->getValuesCounter();
						counted = valuesCounter->combineWithValueCounter(valueSpacesValuesCounter);
					}
					return counted;
				}




				bool CDatatypeIndividualProcessNodeHandler::addValueSpacesDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					bool dependenciesAdded = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					for (CDatatypeValueSpaceType* valueSpaceTypeIt = ontology->getDataBoxes()->getDatatypeValueSpaceTypes()->getValueSpaceTypeLinker(); valueSpaceTypeIt; valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
						CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(valueSpaceTypeIt,false);
						if (valueSpaceData) {
							dependenciesAdded = addValueSpaceDependencies(indiProcNode,valueSpaceData,valueSpaceTypeIt,depCollection,calcAlgContext);
						}
					}
					return dependenciesAdded;
				}



				bool CDatatypeIndividualProcessNodeHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceData* valueSpaceData, CDatatypeValueSpaceType* valueSpaceType, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					if (valueSpaceData->isValueSpaceClashed()) {
						bool depAddeds = false;
						CClashedDependencyDescriptor* clashedDepDesLinker = valueSpaceData->getClashDependencyDescriptorLinker();
						for (CClashedDependencyDescriptor* clashedDepDesIt = clashedDepDesLinker; clashedDepDesIt; clashedDepDesIt = clashedDepDesIt->getNext()) {
							CDependencyTrackPoint* depTrackPoint = clashedDepDesIt->getDependencyTrackPoint();
							depAddeds |= depCollection->addDependency(depTrackPoint);
						}
						return depAddeds;
					} else {
						CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceType,calcAlgContext);
						return valueSpaceHandler->addValueSpaceDependencies(indiProcNode,depCollection,calcAlgContext);
					}
					return false;
				}




				CIndividualProcessNodeValueSpaceHandler* CDatatypeIndividualProcessNodeHandler::getValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType, CCalculationAlgorithmContext* calcAlgContext) {
					CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = nullptr;
					valueSpaceHandler = mValueSpaceHandlerArray[valueSpaceType->getValueSpaceTypeIndex()];
					return valueSpaceHandler;
				}



				cint64 CDatatypeIndividualProcessNodeHandler::getDisjointCount(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					cint64 disjointCount = 0;
					CDistinctHash* distinctHash = indiProcNode->getDistinctHash(false);
					if (distinctHash) {
						disjointCount = distinctHash->getDistinctCount();
					}
					return disjointCount;
				}



				bool CDatatypeIndividualProcessNodeHandler::closeValueSpace(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceType* closeValueSpaceType, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool valueSpaceClosed = false;
					CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(closeValueSpaceType,calcAlgContext);
					if (valueSpaceHandler) {
						valueSpaceClosed |= valueSpaceHandler->closeValueSpace(indiProcNode,depTrackPoint,calcAlgContext);
					}
					return valueSpaceClosed;
				}




				bool CDatatypeIndividualProcessNodeHandler::closeOtherValueSpaces(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceType* exceptionValueSpaceType, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool valueSpaceClosed = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					for (CDatatypeValueSpaceType* valueSpaceTypeIt = ontology->getDataBoxes()->getDatatypeValueSpaceTypes()->getValueSpaceTypeLinker(); valueSpaceTypeIt; valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
						if (valueSpaceTypeIt != exceptionValueSpaceType) {
							valueSpaceClosed |= closeValueSpace(indiProcNode,valueSpaceTypeIt,depTrackPoint,calcAlgContext);
						}
					}
					return valueSpaceClosed;
				}







				bool CDatatypeIndividualProcessNodeHandler::testAllValueSpacesClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					bool allClashed = true;

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					CDatatypeValueSpaceTypes* valueSpaceTypes = ontology->getDataBoxes()->getDatatypeValueSpaceTypes();

					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					for (CDatatypeValueSpaceType* valueSpaceTypeIt = valueSpaceTypes->getValueSpaceTypeLinker(); valueSpaceTypeIt; valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
						CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceTypeIt,calcAlgContext);
						allClashed &= valueSpaceHandler->testValueSpaceClashed(indiProcNode,calcAlgContext);
					}

					if (allClashed) {

						CClashedDependencyDescriptor* depDesLinker = nullptr;
						for (CDatatypeValueSpaceType* valueSpaceTypeIt = valueSpaceTypes->getValueSpaceTypeLinker(); valueSpaceTypeIt; valueSpaceTypeIt = valueSpaceTypeIt->getNextValueSpaceType()) {
							CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(valueSpaceTypeIt,false);
							if (valueSpaceData) {
								depDesLinker = copyClashedDependencyDescriptors(depDesLinker,valueSpaceData->getClashDependencyDescriptorLinker(),calcAlgContext);
							}
						}
						throwDatatypeValueSpaceExclusionClash(depDesLinker,indiProcNode,calcAlgContext);
					}
					return allClashed;
				}




				void CDatatypeIndividualProcessNodeHandler::throwDatatypeValueSpaceExclusionClash(CClashedDependencyDescriptor* depDesLinker, CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedDatatypeValueSpaceExclusionDescriptor* clashDepLinker = nullptr;
					for (CClashedDependencyDescriptor* copyDepDesLinkerIt = depDesLinker; copyDepDesLinkerIt; copyDepDesLinkerIt = copyDepDesLinkerIt->getNext()) {
						CClashedDatatypeValueSpaceExclusionDescriptor* clashDes = CObjectAllocator<CClashedDatatypeValueSpaceExclusionDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						clashDes->initClashedDatatypeValueSpaceExclusionDescriptor(copyDepDesLinkerIt->getDependencyTrackPoint(),indiProcNode);
						clashDepLinker = (CClashedDatatypeValueSpaceExclusionDescriptor*)clashDes->append(clashDepLinker);
					}
					throw CCalculationClashProcessingException(clashDepLinker);
				}


				CClashedDependencyDescriptor* CDatatypeIndividualProcessNodeHandler::createClashedDependencyDescriptor(CClashedDependencyDescriptor* appendDepDesLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					if (depTrackPoint) {
						CClashedDependencyDescriptor* clashDes = CObjectAllocator<CClashedDependencyDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						clashDes->initClashedDependencyDescriptor(depTrackPoint);
						clashDes->append(appendDepDesLinker);
						return clashDes;
					} else {
						return appendDepDesLinker;
					}
				}

				CClashedDependencyDescriptor* CDatatypeIndividualProcessNodeHandler::copyClashedDependencyDescriptors(CClashedDependencyDescriptor* appendDepDesLinker, CClashedDependencyDescriptor* copyDepDesLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedDependencyDescriptor* depDesLinker = appendDepDesLinker;
					for (CClashedDependencyDescriptor* copyDepDesLinkerIt = copyDepDesLinker; copyDepDesLinkerIt; copyDepDesLinkerIt = copyDepDesLinkerIt->getNext()) {
						depDesLinker = createClashedDependencyDescriptor(depDesLinker,copyDepDesLinkerIt->getDependencyTrackPoint(),calcAlgContext);
					}
					return depDesLinker;					
				}













				CConceptDescriptor* CDatatypeIndividualProcessNodeHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeValueSpaceData* valueSpaceData, CDatatypeValueSpaceType* valueSpaceType, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* triggerConcepts = nullptr;
					CIndividualProcessNodeValueSpaceHandler* valueSpaceHandler = getValueSpaceHandler(valueSpaceType,calcAlgContext);
					triggerConcepts = valueSpaceHandler->triggerValueSpaceConcepts(indiProcNode,appendConceptLinker,remainingRequiredValuesCount,depCollection,calcAlgContext);
					return triggerConcepts;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
