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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEVALUESPACEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEVALUESPACEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CCalculationClashProcessingException.h"
#include "CDatatypeDependencyCollection.h"
#include "CDatatypeValueTestingCollection.h"
#include "CDependencyFactory.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CDATATYPETRIGGERDependencyNode.h"
#include "Reasoner/Kernel/Process/Dependency/CDATATYPECONNECTIONDependencyNode.h"


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
				 *		\class		CIndividualProcessNodeValueSpaceHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualProcessNodeValueSpaceHandler {
					// public methods
					public:
						//! Constructor
						CIndividualProcessNodeValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType);


						virtual bool triggerDataLiteralConcept(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);
						virtual bool triggerDatatypeConcept(CIndividualProcessNode* indiProcNode, CDatatype* datatype, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);
						virtual bool triggerDataRestrictionConcept(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CConcept* triggerConcept, bool otherValueSpacesClosed, bool* directlyTriggeredFlag, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool closeValueSpace(CIndividualProcessNode* indiProcNode, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						virtual bool testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);


						virtual CConceptDescriptor* triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext);
						
						virtual bool countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext);
						

					// protected methods
					protected:

						CConceptDescriptor* addConceptLinkerCollectionDependency(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinker, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);

						CClashedDependencyDescriptor* createClashedDependencyDescriptor(CClashedDependencyDescriptor* appendDepDesLinker, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext, cint64 testAlreadyContainedCount = 3);
						CClashedDependencyDescriptor* copyClashedDependencyDescriptors(CClashedDependencyDescriptor* appendDepDesLinker, CClashedDependencyDescriptor* copyDepDesLinker, CCalculationAlgorithmContext* calcAlgContext);


						CConceptDescriptor* addConceptLinker(CConceptDescriptor* appendConceptLinker, CConcept* concept, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						CConceptDescriptor* addConceptLinker(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, CDatatypeValueSpaceConceptTriggerLinker* triggeringConceptLinker, CDependencyTrackPoint* depTrackPoint, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);



						bool hasConceptTriggered(CIndividualProcessNode* indiProcNode, CConcept* concept, CCalculationAlgorithmContext* calcAlgContext);


					// protected variables
					protected:
						CDatatypeValueSpaceType* mValueSpaceType;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODEVALUESPACEHANDLER_H
