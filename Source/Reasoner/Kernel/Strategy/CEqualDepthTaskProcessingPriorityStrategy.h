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

#ifndef KONCLUDE_REASONER_KERNEL_STRATEGY_CEQUALDEPTHTASKPROCESSINGPRIORITYSTRATEGY_H
#define KONCLUDE_REASONER_KERNEL_STRATEGY_CEQUALDEPTHTASKPROCESSINGPRIORITYSTRATEGY_H

// Libraries includes


// Namespace includes
#include "CTaskProcessingPriorityStrategy.h"

// Other includes


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
				 *		\class		CEqualDepthTaskProcessingPriorityStrategy
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CEqualDepthTaskProcessingPriorityStrategy : public CTaskProcessingPriorityStrategy {
					// public methods
					public:
						//! Constructor
						CEqualDepthTaskProcessingPriorityStrategy();

						//! Destructor
						virtual ~CEqualDepthTaskProcessingPriorityStrategy();

						virtual double getPriorityForTaskBranching(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, CIndividualProcessNode* indiProcessNode, CConceptDescriptor* branchingConcept, CSortedNegLinker<CConcept*>* branchedConcept, cint64 branchNumber);
						virtual double getPriorityForTaskQualifing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool qualifingNegated);
						virtual double getPriorityForTaskMerging(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask);
						virtual double getPriorityForTaskReusing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool reusingAlternative);


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

#endif // KONCLUDE_REASONER_KERNEL_STRATEGY_CEQUALDEPTHTASKPROCESSINGPRIORITYSTRATEGY_H
