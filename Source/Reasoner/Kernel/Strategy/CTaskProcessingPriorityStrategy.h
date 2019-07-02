/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_STRATEGY_CTASKPROCESSINGPRIORITYSTRATEGY_H
#define KONCLUDE_REASONER_KERNEL_STRATEGY_CTASKPROCESSINGPRIORITYSTRATEGY_H

// Libraries includes


// Namespace includes


// Other includes
#include "Reasoner/Kernel/Process/CConceptDescriptor.h"
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Task;

			namespace Strategy {

				/*! 
				 *
				 *		\class		CTaskProcessingPriorityStrategy
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CTaskProcessingPriorityStrategy {
					// public methods
					public:
						//! Constructor
						CTaskProcessingPriorityStrategy();

						//! Destructor
						virtual ~CTaskProcessingPriorityStrategy();

						virtual double getPriorityForTaskBranching(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, CConceptDescriptor* branchingConcept, CSortedNegLinker<CConcept*>* branchedConcept, cint64 branchNumber) = 0;
						virtual double getPriorityForTaskQualifing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool qualifingNegated) = 0;
						virtual double getPriorityForTaskMerging(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask) = 0;
						virtual double getPriorityForTaskReusing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool reusingAlternative) = 0;


					// protected methods
					protected:

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_STRATEGY_CTASKPROCESSINGPRIORITYSTRATEGY_H
