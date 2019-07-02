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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEINDIVIDUALPROCESSNODEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEINDIVIDUALPROCESSNODEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CCalculationClashProcessingException.h"
#include "CIndividualProcessNodeValueSpaceHandler.h"
#include "CIndividualProcessNodeRealValueSpaceHandler.h"
#include "CIndividualProcessNodeUnknownValueSpaceHandler.h"
#include "CIndividualProcessNodeStringValueSpaceHandler.h"
#include "CIndividualProcessNodeBooleanValueSpaceHandler.h"
#include "CIndividualProcessNodeDoubleValueSpaceHandler.h"
#include "CIndividualProcessNodeFloatValueSpaceHandler.h"
#include "CIndividualProcessNodeIRIValueSpaceHandler.h"
#include "CIndividualProcessNodeXMLValueSpaceHandler.h"
#include "CIndividualProcessNodeBinaryBase64DataValueSpaceHandler.h"
#include "CIndividualProcessNodeBinaryHexDataValueSpaceHandler.h"
#include "CIndividualProcessNodeDateTimeValueSpaceHandler.h"
#include "CDatatypeDependencyCollection.h"
#include "CDependencyFactory.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CClashedDatatypeValueSpaceExclusionDescriptor.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"

#include "Context/CContextBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CDatatypeIndividualProcessNodeHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CDatatypeIndividualProcessNodeHandler {
					// public methods
					public:
						//! Constructor
						CDatatypeIndividualProcessNodeHandler();

						//! Destructor
						virtual ~CDatatypeIndividualProcessNodeHandler();

						bool addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						bool addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						bool addDatatype(CIndividualProcessNode* indiProcNode, CConcept* datatypeConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						bool triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor*& triggeredConcepts, CCalculationAlgorithmContext* calcAlgContext);


						bool involveDistinctNodeForSatisfiabilityChecking(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);
						bool requiresSatisfiabilityChecking(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);

						bool checkSatisfiability(CIndividualProcessNode* indiProcNode, CPROCESSINGSET<CIndividualProcessNode*>* distinctIndividualNodeSet, CCalculationAlgorithmContext* calcAlgContext);

						bool notifyDistinctChanges(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:

						void throwDatatypeValueSpaceExclusionClash(CClashedDependencyDescriptor* depDesLinker, CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);


						CIndividualProcessNodeValueSpaceHandler* getValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType, CCalculationAlgorithmContext* calcAlgContext);

						CClashedDependencyDescriptor* createClashedDependencyDescriptor(CClashedDependencyDescriptor* appendDepDesLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						CClashedDependencyDescriptor* copyClashedDependencyDescriptors(CClashedDependencyDescriptor* appendDepDesLinker, CClashedDependencyDescriptor* copyDepDesLinker, CCalculationAlgorithmContext* calcAlgContext);

						bool processValueSpaceModified(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);

						cint64 getDisjointCount(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);


						bool closeOtherValueSpaces(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceType* exceptionValueSpaceType, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);
						bool closeValueSpace(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceType* closeValueSpaceType, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						bool testAllValueSpacesClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);

						CConceptDescriptor* triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeValueSpaceData* valueSpaceData, CDatatypeValueSpaceType* valueSpaceType, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);

						bool addValueSpacesDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext);

						bool addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceData* valueSpaceData, CDatatypeValueSpaceType* valueSpaceType, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext);


						bool countPossibleValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valuesCounter, CCalculationAlgorithmContext* calcAlgContext);


						bool seachNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDatatypeValueSpaceType*& nextValueSpaceType, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext);

						bool addDistinctDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						bool mConfBuildDependencies;

						CIndividualProcessNodeValueSpaceHandler* mValueSpaceHandlerArray[CDatatypeValueSpaceType::VALUESPACETYPECOUNT];


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CDATATYPEINDIVIDUALPROCESSNODEHANDLER_H
