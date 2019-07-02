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

#include "CTaskProcessorThread.h"


namespace Konclude {

	namespace Scheduler {



		CTaskProcessorThread::CTaskProcessorThread(CTaskHandleAlgorithm* taskHandleAlgo, CTaskEventHandlerBasedDistributionCompletor* distributorCompletor, CConsiderateMemoryPoolProvider* memoryPoolProvider) : CTaskProcessorThreadBase(taskHandleAlgo,"TaskProcesserSchedulerThread") {
			mDistributorCompletorCommunicator = new CTaskDistributionCompletorCommunicator(distributorCompletor,this);
			mMemoryPoolReleaser = new CEventDistributionMemoryPoolReleaser(mDistributorCompletorCommunicator);
			mMemoryPoolProvider = memoryPoolProvider;
			if (!mMemoryPoolProvider) {
				//mMemoryPoolProvider = distributorCompletor->createDistributionMemoryPoolProvider(mMemoryPoolReleaser);
				mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
			}
			mMemoryAllocator = new CTaskHandleLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider,80000,30);
			mTaskProcessorContext = new CTaskProcessorContextBase(this,mMemoryAllocator);

			mDispenseNotificationTag = 0;

			mSchedulerCommunicator = nullptr;
		}

		CTaskProcessorThread::~CTaskProcessorThread() {
		}




		CTaskEventHandlerBasedProcessor* CTaskProcessorThread::installScheduler(CTaskEventHandlerBasedScheduler* scheduler) {
			mTaskReserveQueueConsumer = scheduler->createTaskReserveQueueConsumer(this);
			CTaskSchedulerCommunicator* schedulerCommunicator = scheduler->createSchedulerProcessorCommunication(this);
			installSchedulerCommunicator(schedulerCommunicator);
			return this;
		}

		CTaskProcessorThread* CTaskProcessorThread::installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator) {
			mSchedulerCommunicator = schedulerCommunicator;
			mSchedulerCommunicator->communicateRequestProcessTask(mMemoryAllocator);
			return this;
		}



		bool CTaskProcessorThread::processEvent(CEvent *event, CContext* handlerContext) {
			cint64 eventID = event->getEventTypeID();
			if (CTaskProcessorThreadBase::processEvent(event,handlerContext)) {
				return true;
			} else {
				// not supported
			}
			return false;
		}




		bool CTaskProcessorThread::processDispenseNotification() {
			bool schedulerReqDis = mSchedulerCommunicator->requiresTaskDispenseNotification(mDispenseNotificationTag);
			organizeDispenseTasks(schedulerReqDis);
			if (canDispenseProcessingTasks() && schedulerReqDis) {
				mSchedulerCommunicator->communicateTaskDispenseNotification(mMemoryAllocator);
			}
			return true;
		}


		bool CTaskProcessorThread::processRequestProcessTask() {
			mSchedulerCommunicator->communicateRequestProcessTask(mMemoryAllocator);
			return true;
		}

		bool CTaskProcessorThread::processCriticalScheduleTask() {
			mSchedulerCommunicator->communicateCriticalScheduleTask(mMemoryAllocator);
			return true;
		}


		bool CTaskProcessorThread::processCompleteTask(CTask* task) {
			// try locale to complete task
			cint64 completedCount = 0;
			mTaskCompletionQueue = task->getLastListLink()->setNext(mTaskCompletionQueue);
			while (mTaskCompletionQueue) {
				CTask* completionTask = mTaskCompletionQueue;
				mTaskCompletionQueue = mTaskCompletionQueue->getNext();

				if (completionTask && !completionTask->hasActiveReferencedTask()) {
					CTask* parentTask = completionTask->getParentTask();
					if (!parentTask || parentTask->hasNoTaskOwnerAndNoParentTask() || parentTask->hasTaskOwner(this)) {
						// locale task completion
						bool upPropagation = false;
						if (mTaskStatusPropagator && mTaskStatusPropagator->completeTaskStatus(completionTask,upPropagation)) {
							if (parentTask && upPropagation) {
								processUpdateTaskStatus(parentTask);
							}
						}
						completionTask->completeTask();
						completionTask->getTaskStatus()->setTaskCOMPLETEDState();
						bool memoryReleaseable = completionTask->getTaskStatus()->isMemoryReleaseable();
						if (parentTask) {
							parentTask->decActiveReferenceCount();
							if (!parentTask->hasActiveReferencedTask()) {
								mTaskCompletionQueue = parentTask->setNext(mTaskCompletionQueue);
							}
						}
						if (completionTask) {
							// callbacks
							CCallbackData* callbackLinkerIt = completionTask->getCallbackLinker();
							while (callbackLinkerIt) {
								CCallbackData* callback = callbackLinkerIt;
								callbackLinkerIt = callbackLinkerIt->getNext();
								if (mCallbackExecuter) {
									mCallbackExecuter->executeCallback(completionTask,callback);
								} else {
									callback->doCallback();
								}
							}
						}
#ifndef KONCLUDE_SCHEDULER_DEBUG_TASK_MEMORY_RELEASE
						if (memoryReleaseable) {
							for (CDeletionLinker* deletionLinkerIt = completionTask->takeDeletionLinker(); deletionLinkerIt; deletionLinkerIt = deletionLinkerIt->getNext()) {
								deletionLinkerIt->deleteObject();
							}
							mMemoryAllocator->releaseMemoryPoolContainer(completionTask);
						}
#endif
						completedCount++;
					} else {
						// task completion by completer
						mDistributorCompletorCommunicator->communicateTaskComplete(completionTask,mMemoryAllocator);
					}
				}				
			}
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCompletedCount(completedCount));
			return true;
		}


		bool CTaskProcessorThread::processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool) {
			CTask* parentTask = task->getParentTask();
			if (!parentTask || parentTask->hasNoTaskOwnerAndNoParentTask() || parentTask->hasTaskOwner(this)) {
				task->appendMemoryPool(memoryPool);
			} else {
				mDistributorCompletorCommunicator->communicateAdditionalAllocationTask(task,memoryPool,mMemoryAllocator);
			}
			return true;
		}

		bool CTaskProcessorThread::processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask) {
			mSchedulerCommunicator->communicateResponseScheduleTask(taskScheduleItem,task,schedulingTask,mMemoryAllocator);
			return true;
		}



		bool CTaskProcessorThread::processUpdateTaskStatus(CTask* updateTask) {
			CTask* parentTask = updateTask->getParentTask();
			if (!parentTask || parentTask->hasNoTaskOwnerAndNoParentTask() || parentTask->hasTaskOwner(this)) {
				// update status local
				bool downProp = false;
				bool upProp = false;
				if (mTaskStatusPropagator && mTaskStatusPropagator->updateTaskStatus(updateTask,downProp,upProp)) {
					if (downProp) {
						// update task status local
						CXNegLinker<CTask*>* refTaskIt = updateTask->getReferencedTaskLinker();
						while (refTaskIt) {
							if (refTaskIt->isNegated()) {
								// task still exists, update task status
								// TODO: check better not recursive?
								CTask* refTask = refTaskIt->getData();
								processUpdateTaskStatus(refTask);
							}
							refTaskIt = refTaskIt->getNext();
						}
					}
					if (upProp && parentTask) {
						processUpdateTaskStatus(parentTask);
					}
				}
			} else {
				mDistributorCompletorCommunicator->communicateAlignedUpdateTaskStatus(updateTask,mMemoryAllocator);
			}
			return true;
		}


		bool CTaskProcessorThread::processUpdateTaskStatus(CTask* parentTask, CTask* updateTask) {
			if (parentTask->hasNoTaskOwnerAndNoParentTask() || parentTask->hasTaskOwner(this)) {
				// local task status update
				// check whether task is completed
				bool taskCompleted = false;
				CXNegLinker<CTask*>* refTaskLinkerIt = parentTask->getReferencedTaskLinker();
				while (refTaskLinkerIt) {
					CTask* refTask = refTaskLinkerIt->getData();
					if (refTask == updateTask) {
						if (!refTaskLinkerIt->isNegated()) {
							taskCompleted = true;
						}
						break;
					}
					refTaskLinkerIt = refTaskLinkerIt->getNext();
				}
				if (!taskCompleted) {
					processUpdateTaskStatus(updateTask);
				}
				mDistributorCompletorCommunicator->communicateTaskStatusUpdated(parentTask,updateTask,true,mMemoryAllocator);
			} else {
				mDistributorCompletorCommunicator->communicateTaskStatusUpdated(parentTask,updateTask,false,mMemoryAllocator);
			}
			return true;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
