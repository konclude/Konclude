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

#include "CEqualDepthTaskProcessingPriorityStrategy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Strategy {

				CEqualDepthTaskProcessingPriorityStrategy::CEqualDepthTaskProcessingPriorityStrategy() {
				}

				CEqualDepthTaskProcessingPriorityStrategy::~CEqualDepthTaskProcessingPriorityStrategy() {
				}

				double CEqualDepthTaskProcessingPriorityStrategy::getPriorityForTaskBranching(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, CIndividualProcessNode* indiProcessNode, CConceptDescriptor* branchingConcept, CSortedNegLinker<CConcept*>* branchedConcept, cint64 branchNumber, CDisjunctBranchingStatistics* branchStats) {
					double parentDepth = parentTask->getTaskDepth();
					double priority = 0.;
					double maxBranchCount = branchingConcept->getConcept()->getOperandCount();
					double parentPriority = parentTask->getTaskPriority();
					//priority = parentDepth + 1. + (0.1 - branchNumber / ((1+parentDepth) * 10 * maxBranchCount));
					priority = parentDepth + 1. + (0.1 - branchNumber / (10 * maxBranchCount));
					return priority;
				}


				double CEqualDepthTaskProcessingPriorityStrategy::getPriorityForTaskQualifing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool qualifingNegated) {
					double parentDepth = parentTask->getTaskDepth();
					double priority = 0.;
					double parentPriority = parentTask->getTaskPriority();
					priority = parentDepth + 1.;
					if (qualifingNegated) {
						priority += 0.1;
					}
					return priority;
				}

				double CEqualDepthTaskProcessingPriorityStrategy::getPriorityForTaskMerging(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask) {
					double parentDepth = parentTask->getTaskDepth();
					double priority = 0.;
					double parentPriority = parentTask->getTaskPriority();
					priority = parentDepth + 1.;
					return priority;
				}

				double CEqualDepthTaskProcessingPriorityStrategy::getPriorityForTaskReusing(CSatisfiableCalculationTask* branchingTask, CSatisfiableCalculationTask* parentTask, bool reusingAlternative) {
					double parentDepth = parentTask->getTaskDepth();
					double priority = 0.;
					double parentPriority = parentTask->getTaskPriority();
					priority = parentDepth + 1.;
					if (reusingAlternative) {
						priority += 0.1;
					}
					return priority;
				}

			}; // end namespace Strategy

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
