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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODECOMPAREVALUESPACEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODECOMPAREVALUESPACEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"
#include "CCalculationClashProcessingException.h"
#include "CDatatypeDependencyCollection.h"
#include "CDependencyFactory.h"
#include "CIndividualProcessNodeValueSpaceHandler.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"
#include "Reasoner/Kernel/Process/CDatatypeCompareValueSpaceData.h"

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
				 *		\class		CIndividualProcessNodeCompareValueSpaceHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualProcessNodeCompareValueSpaceHandler : public CIndividualProcessNodeValueSpaceHandler {
					// public methods
					public:
						//! Constructor
						CIndividualProcessNodeCompareValueSpaceHandler(CDatatypeValueSpaceType* valueSpaceType);

						virtual bool addDataLiteral(CIndividualProcessNode* indiProcNode, CDataLiteral* dataLiteral, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						virtual bool addDataRestriction(CIndividualProcessNode* indiProcNode, CConcept* dataRestricionConcept, bool negated, CDependencyTrackPoint* depTrackPoint, CCalculationAlgorithmContext* calcAlgContext);

						virtual bool testValueSpaceClashed(CIndividualProcessNode* indiProcNode, CCalculationAlgorithmContext* calcAlgContext);

						virtual CConceptDescriptor* triggerValueSpaceConcepts(CIndividualProcessNode* indiProcNode, CConceptDescriptor* appendConceptLinker, cint64& remainingRequiredValuesCount, CDatatypeDependencyCollection* depCollection, CCalculationAlgorithmContext* calcAlgContext);

						virtual bool addValueSpaceDependencies(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceDependencyCollector* depCollection, CCalculationAlgorithmContext* calcAlgContext);
					
						virtual bool countPossibleValueSpaceValues(CIndividualProcessNode* indiProcNode, CDatatypeValueSpaceValuesCounter* valueCounter, CCalculationAlgorithmContext* calcAlgContext);


						virtual bool getNextPossibleDataValue(CIndividualProcessNode* indiProcNode, CDataLiteralValue*& nextValue, CDataLiteralValue* lastValue, CDatatypeValueTestingCollection* valueTestingCollection, CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:

					// protected variables
					protected:
						CDatatypeValueSpaceCompareType* mCompareValueSpaceType;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CINDIVIDUALPROCESSNODECOMPAREVALUESPACEHANDLER_H
