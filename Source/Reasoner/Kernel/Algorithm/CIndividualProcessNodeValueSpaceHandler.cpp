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

#include "CIndividualProcessNodeValueSpaceHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndividualProcessNodeValueSpaceHandler::CIndividualProcessNodeValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType) {
					mValueSpaceType = valueSpaceType;
				}





				bool CIndividualProcessNodeValueSpaceHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool potentiallyModified = false;
					return potentiallyModified;
				}




				bool CIndividualProcessNodeValueSpaceHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool potentiallyModified = false;
					return potentiallyModified;
				}





				bool CIndividualProcessNodeValueSpaceHandler::closeValueSpace(CIndividualProcessNode* indiProcNode, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					bool valueSpaceClosed = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeValueSpaceData* valueSpaceData = datatypesSpaceValue->getValueSpace(mValueSpaceType,false);
					if (!valueSpaceData || !valueSpaceData->isValueSpaceClashed()) {
						valueSpaceData = datatypesSpaceValue->getValueSpace(mValueSpaceType,true);
						valueSpaceData->getClashDependencyTrackPointCollection()->addDependency(depTrackPoint);
						valueSpaceData->setValueSpaceClashed(true);
						valueSpaceClosed = true;

						datatypesSpaceValue->setValueSapceModified(valueSpaceData);
					}
					return valueSpaceClosed;
				}






				bool CIndividualProcessNodeValueSpaceHandler::testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();
					CDatatypeValueSpaceUnknownType* unknownValueSpaceType = ontology->getDataBoxes()->getDatatypeValueSpaceTypes()->getValueSpaceUnkownType();

					CDatatypeUnknownValueSpaceData* unknownValueSpaceData = datatypesSpaceValue->getUnknownValueSpace(unknownValueSpaceType,false);
					if (unknownValueSpaceData) {
						if (unknownValueSpaceData->isValueSpaceClashed()) {
							return true;
						}
					}
					return false;
				}


				CClashedDependencyDescriptor* CIndividualProcessNodeValueSpaceHandler::createClashedDependencyDescriptor(CClashedDependencyDescriptor* appendDepDesLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext, cint64 testAlreadyContainedCount) {
					if (depTrackPoint) {
						CClashedDependencyDescriptor* testDepDesLinker = appendDepDesLinker;
						for (cint64 i = testAlreadyContainedCount; i > 0 && testDepDesLinker; --i) {
							if (testDepDesLinker->getDependencyTrackPoint() == depTrackPoint) {
								return appendDepDesLinker;
							}
							testDepDesLinker = testDepDesLinker->getNext();
						}
						CClashedDependencyDescriptor* clashDes = CObjectAllocator<CClashedDependencyDescriptor>::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						clashDes->initClashedDependencyDescriptor(depTrackPoint);
						clashDes->append(appendDepDesLinker);
						return clashDes;
					} else {
						return appendDepDesLinker;
					}
				}


				CClashedDependencyDescriptor* CIndividualProcessNodeValueSpaceHandler::copyClashedDependencyDescriptors(CClashedDependencyDescriptor* appendDepDesLinker, CClashedDependencyDescriptor* copyDepDesLinker, CCalculationAlgorithmContext* calcAlgContext) {
					CClashedDependencyDescriptor* depDesLinker = appendDepDesLinker;
					for (CClashedDependencyDescriptor* copyDepDesLinkerIt = copyDepDesLinker; copyDepDesLinkerIt; copyDepDesLinkerIt = copyDepDesLinkerIt->getNext()) {
						depDesLinker = createClashedDependencyDescriptor(depDesLinker,copyDepDesLinkerIt->getDependencyTrackPoint(),calcAlgContext);
					}
					return depDesLinker;					
				}






				CConceptDescriptor* CIndividualProcessNodeValueSpaceHandler::addConceptLinker(CConceptDescriptor* appendConceptLinker, CConcept* concept, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					if (concept) {
						CConceptDescriptor* newConceptLinker = CObjectAllocator< CConceptDescriptor >::allocateAndConstruct(calcAlgContext->getUsedProcessTaskMemoryAllocationManager());
						newConceptLinker->initConceptDescriptor(concept,false,depTrackPoint);
						newConceptLinker->append(appendConceptLinker);
						return newConceptLinker;
					} else {
						return appendConceptLinker;
					}
				}



				CConceptDescriptor* CIndividualProcessNodeValueSpaceHandler::addConceptLinker(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinker, CDependencyTrackPoint* depTrackPoint, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					for (CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinkerIt = triggeringConceptLinker; triggeringConceptLinkerIt; triggeringConceptLinkerIt = triggeringConceptLinkerIt->getNext()) {
						CConcept* triggerConcept = triggeringConceptLinkerIt->getTriggerConcept();
						if (!hasConceptTriggered(indiProcNode,triggerConcept,calcAlgContext)) {
							conceptTriggerLinker = addConceptLinker(conceptTriggerLinker,triggerConcept,depTrackPoint,calcAlgContext);
						}
					}
					return conceptTriggerLinker;
				}



				CConceptDescriptor* CIndividualProcessNodeValueSpaceHandler::addConceptLinkerCollectionDependency(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinker, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					for (CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinkerIt = triggeringConceptLinker; triggeringConceptLinkerIt; triggeringConceptLinkerIt = triggeringConceptLinkerIt->getNext()) {
						CConcept* triggerConcept = triggeringConceptLinkerIt->getTriggerConcept();
						if (!hasConceptTriggered(indiProcNode,triggerConcept,calcAlgContext)) {
							CDependencyTrackPoint* datatypeTriggerDepTrackPoint = nullptr;
							if (depCollection) {
								datatypeTriggerDepTrackPoint = depCollection->getDatatypeTriggerDependencyTrackPoint();
							}
							conceptTriggerLinker = addConceptLinker(conceptTriggerLinker,triggerConcept,datatypeTriggerDepTrackPoint,calcAlgContext);
						}
					}
					return conceptTriggerLinker;
				}



				bool CIndividualProcessNodeValueSpaceHandler::hasConceptTriggered(CIndividualProcessNode* indiProcNode, CConcept* concept, CCalculationAlgorithmContext* calcAlgContext) {
					CReapplyConceptLabelSet* conSet = indiProcNode->getReapplyConceptLabelSet(false);
					if (conSet) {
						if (conSet->hasConcept(concept,false)) {
							return true;
						}
					}
					return false;
				}



				CConceptDescriptor* CIndividualProcessNodeValueSpaceHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					return appendConceptLinker;
				}


				bool CIndividualProcessNodeValueSpaceHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					return false;
				}


				bool CIndividualProcessNodeValueSpaceHandler::countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext) {
					return false;
				}



				bool CIndividualProcessNodeValueSpaceHandler::getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {
					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
