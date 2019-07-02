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

#include "CIndividualProcessNodeUnknownValueSpaceHandler.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CIndividualProcessNodeUnknownValueSpaceHandler::CIndividualProcessNodeUnknownValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType) : CIndividualProcessNodeValueSpaceHandler(valueSpaceType) {
					mUnknownValueSpaceType = (CDatatypeValueSpaceUnknownType*)valueSpaceType;
				}





				bool CIndividualProcessNodeUnknownValueSpaceHandler::addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					return false;
				}




				bool CIndividualProcessNodeUnknownValueSpaceHandler::addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext) {
					return false;
				}









				bool CIndividualProcessNodeUnknownValueSpaceHandler::testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		

					CDatatypeUnknownValueSpaceData* unknownValueSpaceData = datatypesSpaceValue->getUnknownValueSpace(mUnknownValueSpaceType,false);
					if (unknownValueSpaceData) {
						if (unknownValueSpaceData->isValueSpaceClashed()) {
							return true;
						}
					}
					return false;
				}



				bool CIndividualProcessNodeUnknownValueSpaceHandler::addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CDatatypeUnknownValueSpaceData* unknownValueSpaceData = datatypesSpaceValue->getUnknownValueSpace(mUnknownValueSpaceType,false);
					if (unknownValueSpaceData) {
						if (unknownValueSpaceData->isValueSpaceClashed()) {
							return unknownValueSpaceData->getClashDependencyTrackPointCollection()->addCollectionDependencies(depCollection);
						}
					}
					return false;
				}


				bool CIndividualProcessNodeUnknownValueSpaceHandler::countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext) {
					bool counted = false;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(false);	
					if (datatypesSpaceValue) {
						CDatatypeUnknownValueSpaceData* unknownValueSpaceData = datatypesSpaceValue->getUnknownValueSpace(mUnknownValueSpaceType,false);
						if (unknownValueSpaceData && unknownValueSpaceData->isValueSpaceClashed()) {
							return false;
						}
					}
					if (valueCounter) {
						counted = valueCounter->incInfinite();
					}
					return counted;
				}


				CConceptDescriptor* CIndividualProcessNodeUnknownValueSpaceHandler::triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext) {
					CConceptDescriptor* conceptTriggerLinker = appendConceptLinker;
					CDatatypesValueSpaceData* datatypesSpaceValue = indiProcNode->getDatatypesValueSpaceData(true);		
					CConcreteOntology* ontology = calcAlgContext->getUsedProcessingDataBox()->getOntology();

					CDatatypeUnknownValueSpaceData* unknownValueSpaceData = datatypesSpaceValue->getUnknownValueSpace(mUnknownValueSpaceType,false);
					if (!unknownValueSpaceData || !unknownValueSpaceData->isValueSpaceTriggeringCompleted()) {
						remainingRequiredValuesCount = 0;
					}
					return conceptTriggerLinker;
				}




				bool CIndividualProcessNodeUnknownValueSpaceHandler::getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext) {

					return false;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
