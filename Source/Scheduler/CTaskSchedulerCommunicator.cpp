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

#include "CTaskSchedulerCommunicator.h"


namespace Konclude {

	namespace Scheduler {


		CTaskSchedulerCommunicator::CTaskSchedulerCommunicator(CTaskEventHandlerBasedScheduler* taskScheduler, CTaskEventHandlerBasedProcessor* taskProcessor) : mSchedulerTaskEventCommunicator(taskScheduler->getEventHandler()), mProcessorTaskEventCommunicator(taskProcessor->getEventHandler()) {
			mTaskScheduler = taskScheduler;
			mTaskProcessor = taskProcessor;
		}

		CTaskSchedulerCommunicator::~CTaskSchedulerCommunicator() {
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateRequestScheduleTask(CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mProcessorTaskEventCommunicator.postRequestScheduleTaskEvent(this,taskScheduleItem,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateResponseScheduleTask(CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulingTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mSchedulerTaskEventCommunicator.postResponseScheduleTaskEvent(this,taskScheduleItem,task,schedulingTask,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateCriticalScheduleTask(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mSchedulerTaskEventCommunicator.postCriticalScheduleTaskEvent(this,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateRequestProcessTask(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mSchedulerTaskEventCommunicator.postRequestProcessTaskEvent(this,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateTaskDispenseNotification(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mSchedulerTaskEventCommunicator.postTaskDispenseNotificationEvent(this,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateProcessTask(CTask* task, bool schedulingTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mProcessorTaskEventCommunicator.postSendTaskProcessEvent(task,schedulingTask,memoryTmpAllocatorManager);
			return this;
		}

		bool CTaskSchedulerCommunicator::requiresTaskDispenseNotification(cint64 &dispenseNotificationTag) {
			return mTaskScheduler->requiresTaskDispenseNotification(dispenseNotificationTag);
		}


		bool CTaskSchedulerCommunicator::canDispenseProcessingTasks() {
			return mTaskProcessor->canDispenseProcessingTasks();
		}

		bool CTaskSchedulerCommunicator::requiresProcessingTasks() {
			return mTaskProcessor->requiresProcessingTasks();
		}

		bool CTaskSchedulerCommunicator::requiresSchedulingTasks() {
			return mTaskProcessor->requiresSchedulingTasks();
		}

		cint64 CTaskSchedulerCommunicator::countDispensableProcessingTasks() {
			return mTaskProcessor->countDispensableProcessingTasks();
		}


		cint64 CTaskSchedulerCommunicator::countScheduleableProcessingTasks() {
			return mTaskProcessor->countScheduleableProcessingTasks();
		}


		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateScheduleUpdateTaskStatus(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mSchedulerTaskEventCommunicator.postUpdateTaskStatusEvent(parentTask,updateTask,memoryTmpAllocatorManager);
			return this;
		}

		CTaskSchedulerCommunicator* CTaskSchedulerCommunicator::communicateUpdateTaskStatus(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			mProcessorTaskEventCommunicator.postUpdateTaskStatusEvent(parentTask,updateTask,memoryTmpAllocatorManager);
			return this;
		}

		bool CTaskSchedulerCommunicator::hasTaskOwner(CTaskOwner* taskOwner) {
			return taskOwner == mTaskProcessor;
		}

		cint64 CTaskSchedulerCommunicator::getRecievedTasks() {
			return mTaskProcessor->getRecievedTasks();
		}

	}; // end namespace Scheduler

}; // end namespace Konclude

