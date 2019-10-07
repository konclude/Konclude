/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CThroughputTestTaskHandleAlgorithm.h"


namespace Konclude {

	namespace Test {



		CThroughputTestTaskHandleAlgorithm::CThroughputTestTaskHandleAlgorithm() {
		}


		CThroughputTestTaskHandleAlgorithm::~CThroughputTestTaskHandleAlgorithm() {
		}


		bool CThroughputTestTaskHandleAlgorithm::handleTask(CTaskProcessorContext *processorContext, CTask* task) {
			CTaskHandleMemoryAllocationManager* processorMemoryManager = processorContext->getTaskHandleMemoryAllocationManager();
			CTaskProcessorCommunicator* processorCommunicator = processorContext->getTaskProcessorCommunicator();

			CThroughputTestTask* testTask = dynamic_cast<CThroughputTestTask*>(task);
			if (testTask) {
				bool complete = false;
				try {
					if (!testTask->hasReachedMaxBranchingDepth()) {

						CMemoryAllocationManager* taskMemMan = testTask->getTaskContext(processorContext)->getMemoryAllocationManager();
						cint64 branchingFactor = testTask->getBranchingFactor();

						cint64 allocCount = 0;
						for (int i = 0; i < allocCount; ++i) {
							cint64* testArray = CObjectAllocator<cint64>::allocateAndConstructArray(taskMemMan,branchingFactor);
							for (int j = 0; j < branchingFactor; ++j) {
								testArray[j] = 0;
							}
						}

						CTask* lastNewTestTask = nullptr;

						for (cint64 branchIdx = 0; branchIdx < branchingFactor; ++branchIdx) {
							CThroughputTestTask* newTestTask = CTaskAllocator<CThroughputTestTask>::allocateAndConstructTask(processorMemoryManager);
							newTestTask->initTask(testTask,processorContext);
							lastNewTestTask = newTestTask->setNext(lastNewTestTask);
						}

						processorCommunicator->communicateTaskCreation(lastNewTestTask);
					}
					throw 0;
				} catch (const int& x) {
					if (x == 0) {
						complete = true;
					}
				}
				processorCommunicator->communicateTaskComplete(testTask);

			}
			return false;
		}



	}; // end namespace Test

}; // end namespace Konclude
