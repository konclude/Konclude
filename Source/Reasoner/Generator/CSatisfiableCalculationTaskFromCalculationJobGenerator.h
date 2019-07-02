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

#ifndef KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONTASKFROMCALCULATIONJOBGENERATOR_H
#define KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONTASKFROMCALCULATIONJOBGENERATOR_H

// Libraries includes


// Namespace includes
#include "CGeneratorTaskHandleContextBase.h"

// Other includes
#include "Reasoner/Query/CCalculationJob.h"
#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CSatisfiableCalculationConstruct.h"
#include "Reasoner/Query/CApproximatedSaturationCalculationJob.h"
#include "Reasoner/Query/CSatisfiableCalculationConceptConstruct.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConceptProcessData.h"
#include "Reasoner/Ontology/CConceptSaturationReferenceLinkingData.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"
#include "Reasoner/Kernel/Task/CSaturationTaskData.h"

#include "Reasoner/Kernel/Process/CProcessContext.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeDescriptor.h"
#include "Reasoner/Kernel/Process/CProcessingDataBox.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Query;
		using namespace Ontology;
		using namespace Kernel::Task;
		using namespace Kernel::Process;

		namespace Generator {

			/*! 
			 *
			 *		\class		CSatisfiableCalculationTaskFromCalculationJobGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSatisfiableCalculationTaskFromCalculationJobGenerator {
				// public methods
				public:
					//! Constructor
					CSatisfiableCalculationTaskFromCalculationJobGenerator(CGeneratorTaskHandleContextBase* genTaskHandleContext);

					CSatisfiableCalculationTask* createSatisfiableCalculationTask(CCalculationJob* calculationJob, CCallbackData* additionalCalculatedCallback = nullptr);
					CSatisfiableCalculationTask* createSatisfiableCalculationTask(CConcreteOntology *ontology, CCalculationJob* calculationJob, CCallbackData* additionalCalculatedCallback = nullptr);


					CSatisfiableCalculationTask* createSatisfiableCalculationTaskExtension(CCalculationJob* calculationJob, CSatisfiableCalculationTask* baseTask, bool lastConDesReapplication, bool clearIndiProcessingQueue, CCallbackData* additionalCalculatedCallback = nullptr);
					CSatisfiableCalculationTask* createSatisfiableCalculationTaskExtension(CConcreteOntology *ontology, CCalculationJob* calculationJob, CSatisfiableCalculationTask* baseTask, bool lastConDesReapplication, bool clearIndiProcessingQueue, CCallbackData* additionalCalculatedCallback = nullptr);

					CSatisfiableCalculationTask* createApproximatedSaturationCalculationTask(CConcreteOntology *ontology, CApproximatedSaturationCalculationJob* approxSaturCalcJob, CCallbackData* additionalCalculatedCallback);
				// protected methods
				protected:

				// protected variables
				protected:					
					CGeneratorTaskHandleContextBase* mGenTaskHandleContext;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_GENERATOR_CSATISFIABLECALCULATIONTASKFROMCALCULATIONJOBGENERATOR_H
