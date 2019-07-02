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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPOSSIBLEASSERTIONCOLLECTINGANALYSER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPOSSIBLEASSERTIONCOLLECTINGANALYSER_H

// Libraries includes


// Namespace includes
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Context/CContextBase.h"

#include "Reasoner/Realizer/CRealizationMessageObserver.h"
#include "Reasoner/Realizer/CRealizationMarkerCandidatesMessageData.h"

#include "Reasoner/Consistiser/CSaturationConceptDataItem.h"

#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"
#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

#include "Reasoner/Kernel/Process/Dependency/CDependencyTrackPoint.h"
#include "Reasoner/Kernel/Process/Dependency/CDependencyNode.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Consistiser;
		using namespace Realizer;
		using namespace Ontology;

		namespace Kernel {

			using namespace Task;
			using namespace Process::Dependency;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CSatisfiableTaskPossibleAssertionCollectingAnalyser
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSatisfiableTaskPossibleAssertionCollectingAnalyser {
					// public methods
					public:
						//! Constructor
						CSatisfiableTaskPossibleAssertionCollectingAnalyser();

						bool readCalculationConfig(CCalculationConfigurationExtension *config);
						bool analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext);

						CIndividualProcessNode* getCorrectedIndividualNode(cint64 baseIndiID, CIndividualProcessNodeVector* indiNodeVec);



						CSatisfiableCalculationTask* getCachedDeterministicCompletionGraphTask(CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:

					// protected variables
					protected:



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CSATISFIABLETASKPOSSIBLEASSERTIONCOLLECTINGANALYSER_H
