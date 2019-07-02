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

#include "CTaskProcessorCompletorThread.h"


namespace Konclude {

	namespace Scheduler {



		CTaskProcessorCompletorThread::CTaskProcessorCompletorThread(CTaskHandleAlgorithm* taskHandleAlgo, CConsiderateMemoryPoolProvider* memoryPoolProvider) : CTaskProcessorThreadBase(taskHandleAlgo,"TaskProcesserCompletorThread"),CConcreteMemoryPoolDistributor(nullptr) {
			mMemoryPoolReleaser = new CDirectDistributionMemoryPoolReleaser(this);
			mMemoryPoolProvider = memoryPoolProvider;
			if (!mMemoryPoolProvider) {
				//mMemoryPoolProvider = CConcreteMemoryPoolDistributor::createDistributionMemoryPoolProvider(mMemoryPoolReleaser);
				mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
			}
			mMemoryAllocator = new CTaskHandleLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider,300,30);
			mTaskProcessorContext = new CTaskProcessorContextBase(this,mMemoryAllocator);

			mSchedulerCommunicator = nullptr;
			mDispenseNotificationTag = 0;
		}

		CTaskProcessorCompletorThread::~CTaskProcessorCompletorThread() {
		}


		CTaskEventHandlerBasedProcessor* CTaskProcessorCompletorThread::installScheduler(CTaskEventHandlerBasedScheduler* scheduler) {
			mTaskReserveQueueConsumer = scheduler->createTaskReserveQueueConsumer(this);
			CTaskSchedulerCommunicator* schedulerCommunicator = scheduler->createSchedulerProcessorCommunication(this);
			installSchedulerCommunicator(schedulerCommunicator);
			return this;
		}

		CTaskProcessorCompletorThread* CTaskProcessorCompletorThread::installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator) {
			mSchedulerCommunicator = schedulerCommunicator;
			mSchedulerCommunicator->communicateRequestProcessTask(mMemoryAllocator);
			return this;
		}

		CDistributionMemoryPoolProvider* CTaskProcessorCompletorThread::createDistributionMemoryPoolProvider(CDistributionMemoryPoolReleaser* distributionMemoryReleaser) {
			return CConcreteMemoryPoolDistributor::createDistributionMemoryPoolProvider(distributionMemoryReleaser);
		}

		CConcreteMemoryPoolDistributor* CTaskProcessorCompletorThread::distributeMemoryPools(CMemoryPool* memoryPools) {
			return CConcreteMemoryPoolDistributor::distributeMemoryPools(memoryPools);
		}


		bool CTaskProcessorCompletorThread::processEvent(CEvent *event, CContext* handlerContext) {
			mLastProcessedEvent = event;
			cint64 eventID = event->getEventTypeID();
			if (eventID == CTaskStatusUpdatedEvent::EVENTTYPEID) {
				CTaskStatusUpdatedEvent* tsue = (CTaskStatusUpdatedEvent*)event;
				CTask* parentTask = tsue->getParentTask();
				CTask* updatedTask = tsue->getUpdatedTask();
				bool updateSuccess = tsue->getUpdateSuccess();
				processTaskStatusUpdated(parentTask,updatedTask,updateSuccess);
				return true;
			} else if (eventID == CAlignedUpdateTaskStatusEvent::EVENTTYPEID) {
				CAlignedUpdateTaskStatusEvent* autse = (CAlignedUpdateTaskStatusEvent*)event;
				CTask* updateTask = autse->getUpdateTask();
				processUpdateTaskStatus(updateTask);
				return true;
			} else if (CTaskProcessorThreadBase::processEvent(event,handlerContext)) {
				return true;
			} else {
				// not supported event
			}
			return false;
		}



		bool CTaskProcessorCompletorThread::processDispenseNotification() {
			if (canDispenseProcessingTasks() && mSchedulerCommunicator->requiresTaskDispenseNotification(mDispenseNotificationTag)) {
				mSchedulerCommunicator->communicateTaskDispenseNotification(mMemoryAllocator);
			}
			return true;
		}


		bool CTaskProcessorCompletorThread::processRequestProcessTask() {
			mSchedulerCommunicator->communicateRequestProcessTask(mMemoryAllocator);
			return true;
		}

		bool CTaskProcessorCompletorThread::processCriticalScheduleTask() {
			mSchedulerCommunicator->communicateCriticalScheduleTask(mMemoryAllocator);
			return true;
		}


		bool CTaskProcessorCompletorThread::processCompleteTask(CTask* task) {
			cint64 completedCount = 0;
			mTaskCompletionQueue = task->getLastListLink()->setNext(mTaskCompletionQueue);
			while (mTaskCompletionQueue) {
				CTask* completionTask = mTaskCompletionQueue;
				mTaskCompletionQueue = mTaskCompletionQueue->getNext();

				if (completionTask) {
					if (completionTask->hasDependedStatusUpdates()) {
						completionTask->setCompletionRequested(true);
					} else if (!completionTask->hasActiveReferencedTask()) {

						CTask* parentTask = completionTask->getParentTask();
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
									INCTASKPROCESSINGSTAT(mStats.incStatisticCallbacksExecutedCount(1));
								} else {
									callback->doCallback();
								}
							}
						}
#ifndef KONCLUDE_SCHEDULER_DEBUG_TASK_MEMORY_RELEASE
						if (memoryReleaseable) {
							mMemoryAllocator->releaseMemoryPoolContainer(completionTask);
						}
#endif
						completedCount++;
					}
				}

			}
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCompletedCount(completedCount));
			return true;
		}


		bool CTaskProcessorCompletorThread::processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool) {
			task->appendMemoryPool(memoryPool);
			return true;
		}

		bool CTaskProcessorCompletorThread::processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask) {
			mSchedulerCommunicator->communicateResponseScheduleTask(taskScheduleItem,task,schedulingTask,mMemoryAllocator);
			return true;
		}

		bool CTaskProcessorCompletorThread::processTaskStatusUpdated(CTask* parentTask, CTask* updatedTask, bool updateSuccess) {
			bool needsMayTaskCompletion = false;
			if (updateSuccess) {
				parentTask->decDependedStatusUpdatesCount();
				needsMayTaskCompletion = true;
			} else {
				// task is already completed or is now owned by completer
				// check whether task is completed
				bool taskCompleted = false;
				CXNegLinker<CTask*>* refTaskLinkerIt = parentTask->getReferencedTaskLinker();
				while (refTaskLinkerIt) {
					CTask* refTask = refTaskLinkerIt->getData();
					if (refTask == updatedTask) {
						if (!refTaskLinkerIt->isNegated()) {
							taskCompleted = true;
						}
						break;
					}
					refTaskLinkerIt = refTaskLinkerIt->getNext();
				}
				if (taskCompleted) {
					parentTask->decDependedStatusUpdatesCount();
					needsMayTaskCompletion = true;
				} else {
					if (parentTask->hasTaskOwner(this) || !parentTask->hasTaskOwner()) {
						processUpdateTaskStatus(parentTask,updatedTask);
						needsMayTaskCompletion = true;
					} else {
						mSchedulerCommunicator->communicateScheduleUpdateTaskStatus(parentTask,updatedTask,mMemoryAllocator);
					}
				}
			}
			if (needsMayTaskCompletion) {
				if (parentTask->isCompletionRequested()) {
					if (!parentTask->hasDependedStatusUpdates()) {
						processCompleteTask(parentTask);
					}
				}
			}
			return true;
		}

		bool CTaskProcessorCompletorThread::processUpdateTaskStatus(CTask* parentTask, CTask* updateTask) {
			if (!parentTask || parentTask->hasTaskOwner(this) || !parentTask->hasTaskOwner()) {
				// local task status update
				processUpdateTaskStatus(updateTask);
				processTaskStatusUpdated(parentTask,updateTask,true);
			} else {
				processTaskStatusUpdated(parentTask,updateTask,false);
			}
			return true;
		}


		bool CTaskProcessorCompletorThread::processUpdateTaskStatus(CTask* task) {
			CTask* updateTask = task;
			bool downProp = false;
			bool upProp = false;
			CTaskOwner* taskOwner = updateTask->getTaskOwner();
			if (mTaskStatusPropagator && mTaskStatusPropagator->updateTaskStatus(updateTask,downProp,upProp)) {
				if (downProp) {
					if (taskOwner == nullptr || taskOwner == this) {
						// update task status direct
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
					} else {
						// signalize task status update per event to other processing units
						CXNegLinker<CTask*>* refTaskIt = updateTask->getReferencedTaskLinker();
						while (refTaskIt) {
							if (refTaskIt->isNegated()) {
								// task still exists, update task status with event to corresponding processor unit
								CTask* refTask = refTaskIt->getData();
								updateTask->incDependedStatusUpdatesCount();
								mSchedulerCommunicator->communicateScheduleUpdateTaskStatus(updateTask,refTask,mMemoryAllocator);
							}
							refTaskIt = refTaskIt->getNext();
						}
					}
				}
				if (upProp) {
					// all parent task are owned by the completer
					CTask* parentTask = updateTask->getParentTask();
					if (parentTask) {
						processUpdateTaskStatus(parentTask);
					}
				}
			}
			return true;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
