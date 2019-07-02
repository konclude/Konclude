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

#include "CTaskDistributionCompletorCommunicator.h"


namespace Konclude {

	namespace Scheduler {


		CTaskDistributionCompletorCommunicator::CTaskDistributionCompletorCommunicator(CTaskEventHandlerBasedDistributionCompletor* taskDistributionCompletor, CTaskEventHandlerBasedProcessor* taskProcessor) : mDistributionCompletorTaskEventCommunicator(taskDistributionCompletor->getEventHandler()), mProcessorTaskEventCommunicator(taskProcessor->getEventHandler()) {
			mTaskDistributionCompletor = taskDistributionCompletor;
			mTaskProcessor = taskProcessor;
		}

		CTaskDistributionCompletorCommunicator::~CTaskDistributionCompletorCommunicator() {
		}


		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateTaskComplete(CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mDistributionCompletorTaskEventCommunicator.postSendTaskCompleteEvent(task,memoryTmpAllocatorManager);
			return this;
		}

		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateAdditionalAllocationTask(CTask* task, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mDistributionCompletorTaskEventCommunicator.postAdditionalAllocationTaskEvent(task,memoryPools,memoryTmpAllocatorManager);
			return this;
		}

		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateDistributeMemoryPools(CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mDistributionCompletorTaskEventCommunicator.postDistributeMemoryPoolsEvent(memoryPools,memoryTmpAllocatorManager);
			return this;
		}

		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateDistributeMemoryPools(CMemoryPool* memoryPools) {
			mDistributionCompletorTaskEventCommunicator.postDistributeMemoryPoolsEvent(memoryPools);
			return this;
		}


		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateAlignedUpdateTaskStatus(CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mDistributionCompletorTaskEventCommunicator.postAlignedUpdateTaskStatusEvent(updateTask,memoryTmpAllocatorManager);
			return this;
		}

		CTaskDistributionCompletorCommunicator* CTaskDistributionCompletorCommunicator::communicateTaskStatusUpdated(CTask* parentTask, CTask* updatedTask, bool success, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mDistributionCompletorTaskEventCommunicator.postTaskStatusUpdatedEvent(parentTask,updatedTask,success,memoryTmpAllocatorManager);
			return this;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude

