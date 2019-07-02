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

#include "CTaskEventCommunicator.h"


namespace Konclude {

	namespace Scheduler {


		CTaskEventCommunicator::CTaskEventCommunicator(CEventHandler* eventHandler) : CEventCommunicator(eventHandler) {
			mPostedEventCount = 0;
			mTmpStoredMemoryPool = nullptr;
		}

		CTaskEventCommunicator::~CTaskEventCommunicator() {
		}


		CTaskEventCommunicator* CTaskEventCommunicator::postSendTaskProcessEvent(CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskProcessEvent* sendEvent = CEventAllocator<CSendTaskProcessEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CSendTaskProcessEvent* newSendEvent = CEventAllocator<CSendTaskProcessEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setSchedulingTask(schedulerTask);
			sendEvent->setTask(task);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postSendTaskCompleteEvent(CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskCompleteEvent* sendEvent = CEventAllocator<CSendTaskCompleteEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CSendTaskCompleteEvent* newSendEvent = CEventAllocator<CSendTaskCompleteEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postSendTaskScheduleEvent(CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskScheduleEvent* sendEvent = CEventAllocator<CSendTaskScheduleEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CSendTaskScheduleEvent* newSendEvent = CEventAllocator<CSendTaskScheduleEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postRequestScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CRequestScheduleTaskEvent* sendEvent = CEventAllocator<CRequestScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CRequestScheduleTaskEvent* newSendEvent = CEventAllocator<CRequestScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTaskScheduleItem(taskScheduleItem);
			sendEvent->setCommunicator(communicator);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postResponseScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CResponseScheduleTaskEvent* sendEvent = CEventAllocator<CResponseScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CResponseScheduleTaskEvent* newSendEvent = CEventAllocator<CResponseScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			sendEvent->setTaskScheduleItem(taskScheduleItem);
			sendEvent->setCommunicator(communicator);
			sendEvent->setSchedulingTask(schedulerTask);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postRequestProcessTaskEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CRequestProcessTaskEvent* sendEvent = CEventAllocator<CRequestProcessTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CRequestProcessTaskEvent* newSendEvent = CEventAllocator<CRequestProcessTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postCriticalScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CCriticalScheduleTaskEvent* sendEvent = CEventAllocator<CCriticalScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CCriticalScheduleTaskEvent* newSendEvent = CEventAllocator<CCriticalScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postAdditionalAllocationTaskEvent(CTask* task, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskAdditionalAllocationEvent* sendEvent = CEventAllocator<CTaskAdditionalAllocationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CTaskAdditionalAllocationEvent* newSendEvent = CEventAllocator<CTaskAdditionalAllocationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			sendEvent->setAdditionalAllocatedMemoryPools(memoryPools);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postDistributeMemoryPoolsEvent(CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CDistributeMemoryPoolEvent* sendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CDistributeMemoryPoolEvent* newSendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setMemoryPools(memoryPools);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postDistributeMemoryPoolsEvent(CMemoryPool* memoryPools) {
			if (memoryPools) {
				CMemoryPool* memoryPool = memoryPools;
				memoryPools = memoryPools->getNextMemoryPool();
				memoryPool->resetMemoryBlockPointer();
				bool eventPostable = true;
				CDistributeMemoryPoolEvent* sendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(memoryPool);
				if (!CEventCommunicator::checkPostEvent(sendEvent)) {
					if (memoryPools) {
						CMemoryPool* newMemoryPool = memoryPools;
						newMemoryPool->resetMemoryBlockPointer();
						memoryPools = memoryPools->getNextMemoryPool();
						CDistributeMemoryPoolEvent* newSendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(newMemoryPool);	
						memoryPools = memoryPool->setNext(memoryPools);
						sendEvent = newSendEvent;
					} else {
						mTmpStoredMemoryPool = memoryPool;
						eventPostable = false;
					}
				}
				if (eventPostable) {
					if (mTmpStoredMemoryPool) {
						memoryPools = mTmpStoredMemoryPool->setNextMemoryPool(memoryPools);
					}
					sendEvent->setMemoryPools(memoryPools);
					++mPostedEventCount;
					CEventCommunicator::postEvent(sendEvent);
				}
			}
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postTaskDispenseNotificationEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskDispenseNotificationEvent* sendEvent = CEventAllocator<CTaskDispenseNotificationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CTaskDispenseNotificationEvent* newSendEvent = CEventAllocator<CTaskDispenseNotificationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postUpdateTaskStatusEvent(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CUpdateTaskStatusEvent* sendEvent = CEventAllocator<CUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CUpdateTaskStatusEvent* newSendEvent = CEventAllocator<CUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setParentTask(parentTask);
			sendEvent->setUpdateTask(updateTask);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postAlignedUpdateTaskStatusEvent(CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CAlignedUpdateTaskStatusEvent* sendEvent = CEventAllocator<CAlignedUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CAlignedUpdateTaskStatusEvent* newSendEvent = CEventAllocator<CAlignedUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setUpdateTask(updateTask);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}

		CTaskEventCommunicator* CTaskEventCommunicator::postTaskStatusUpdatedEvent(CTask* parentTask, CTask* updatedTask, bool updateSuccess, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskStatusUpdatedEvent* sendEvent = CEventAllocator<CTaskStatusUpdatedEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(sendEvent)) {
				CTaskStatusUpdatedEvent* newSendEvent = CEventAllocator<CTaskStatusUpdatedEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setParentTask(parentTask);
			sendEvent->setUpdatedTask(updatedTask);
			sendEvent->setUpdateSuccess(updateSuccess);
			++mPostedEventCount;
			CEventCommunicator::postEvent(sendEvent);
			return this;
		}









		bool CTaskEventCommunicator::postSendTaskProcessEvent(CEventHandler* eventHandler, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskProcessEvent* sendEvent = CEventAllocator<CSendTaskProcessEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CSendTaskProcessEvent* newSendEvent = CEventAllocator<CSendTaskProcessEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			sendEvent->setSchedulingTask(schedulerTask);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postSendTaskCompleteEvent(CEventHandler* eventHandler, CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskCompleteEvent* sendEvent = CEventAllocator<CSendTaskCompleteEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CSendTaskCompleteEvent* newSendEvent = CEventAllocator<CSendTaskCompleteEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}		
		
		bool CTaskEventCommunicator::postSendTaskScheduleEvent(CEventHandler* eventHandler, CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CSendTaskScheduleEvent* sendEvent = CEventAllocator<CSendTaskScheduleEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CSendTaskScheduleEvent* newSendEvent = CEventAllocator<CSendTaskScheduleEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postRequestScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CRequestScheduleTaskEvent* sendEvent = CEventAllocator<CRequestScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CRequestScheduleTaskEvent* newSendEvent = CEventAllocator<CRequestScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTaskScheduleItem(taskScheduleItem);
			sendEvent->setCommunicator(communicator);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postResponseScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CResponseScheduleTaskEvent* sendEvent = CEventAllocator<CResponseScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CResponseScheduleTaskEvent* newSendEvent = CEventAllocator<CResponseScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			sendEvent->setTaskScheduleItem(taskScheduleItem);
			sendEvent->setCommunicator(communicator);
			sendEvent->setSchedulingTask(schedulerTask);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postRequestProcessTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CRequestProcessTaskEvent* sendEvent = CEventAllocator<CRequestProcessTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CRequestProcessTaskEvent* newSendEvent = CEventAllocator<CRequestProcessTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postCriticalScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CCriticalScheduleTaskEvent* sendEvent = CEventAllocator<CCriticalScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CCriticalScheduleTaskEvent* newSendEvent = CEventAllocator<CCriticalScheduleTaskEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postAdditionalAllocationTaskEvent(CEventHandler* eventHandler, CTask* task, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskAdditionalAllocationEvent* sendEvent = CEventAllocator<CTaskAdditionalAllocationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CTaskAdditionalAllocationEvent* newSendEvent = CEventAllocator<CTaskAdditionalAllocationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setTask(task);
			sendEvent->setAdditionalAllocatedMemoryPools(memoryPools);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postDistributeMemoryPoolsEvent(CEventHandler* eventHandler, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CDistributeMemoryPoolEvent* sendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CDistributeMemoryPoolEvent* newSendEvent = CEventAllocator<CDistributeMemoryPoolEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setMemoryPools(memoryPools);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}


		bool CTaskEventCommunicator::postTaskDispenseNotificationEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskDispenseNotificationEvent* sendEvent = CEventAllocator<CTaskDispenseNotificationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CTaskDispenseNotificationEvent* newSendEvent = CEventAllocator<CTaskDispenseNotificationEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setCommunicator(communicator);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}


		bool CTaskEventCommunicator::postUpdateTaskStatusEvent(CEventHandler* eventHandler, CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CUpdateTaskStatusEvent* sendEvent = CEventAllocator<CUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CUpdateTaskStatusEvent* newSendEvent = CEventAllocator<CUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setParentTask(parentTask);
			sendEvent->setUpdateTask(updateTask);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postAlignedUpdateTaskStatusEvent(CEventHandler* eventHandler, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CAlignedUpdateTaskStatusEvent* sendEvent = CEventAllocator<CAlignedUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CAlignedUpdateTaskStatusEvent* newSendEvent = CEventAllocator<CAlignedUpdateTaskStatusEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setUpdateTask(updateTask);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

		bool CTaskEventCommunicator::postTaskStatusUpdatedEvent(CEventHandler* eventHandler, CTask* parentTask, CTask* updatedTask, bool updateSuccess, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager) {
			CTaskStatusUpdatedEvent* sendEvent = CEventAllocator<CTaskStatusUpdatedEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);
			if (!CEventCommunicator::checkPostEvent(eventHandler,sendEvent)) {
				CTaskStatusUpdatedEvent* newSendEvent = CEventAllocator<CTaskStatusUpdatedEvent>::allocateAndConstructEvent(memoryTmpAllocatorManager);		
				memoryTmpAllocatorManager->releaseMemoryPoolContainer(sendEvent);
				sendEvent = newSendEvent;
			}
			sendEvent->setParentTask(parentTask);
			sendEvent->setUpdatedTask(updatedTask);
			sendEvent->setUpdateSuccess(updateSuccess);
			CEventCommunicator::postEvent(eventHandler,sendEvent);
			return true;
		}

	}; // end namespace Scheduler

}; // end namespace Konclude

