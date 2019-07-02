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

#include "CTaskProcessorSchedulerThread.h"


namespace Konclude {

	namespace Scheduler {



		CTaskProcessorSchedulerThread::CTaskProcessorSchedulerThread(CTaskHandleAlgorithm* taskHandleAlgo, CTaskEventHandlerBasedDistributionCompletor* distributorCompletor, CConsiderateMemoryPoolProvider* memoryPoolProvider) : CTaskProcessorThreadBase(taskHandleAlgo,"TaskProcesserSchedulerThread") {
			mDistributorCompletorCommunicator = new CTaskDistributionCompletorCommunicator(distributorCompletor,this);
			mMemoryPoolReleaser = new CEventDistributionMemoryPoolReleaser(mDistributorCompletorCommunicator);
			mMemoryPoolProvider = memoryPoolProvider;
			if (!mMemoryPoolProvider) {
				//mMemoryPoolProvider = distributorCompletor->createDistributionMemoryPoolProvider(mMemoryPoolReleaser);
				mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
			}
			mMemoryAllocator = new CTaskHandleLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider,300,30);
			mTaskProcessorContext = new CTaskProcessorContextBase(this,mMemoryAllocator);


			mFreeSchedulerItemLinker = nullptr;
			mWaitingSchedulerItemLinker = nullptr;
			mSchedulerCommunicatorLinker = nullptr;

			mRequestingTaskCount = 0;
			mDeliveringWaitingCount = 0;
			mRequiresTaskDispenseNotification = false;
			mSchedulerDispenseNotificationTag = 0;
			mDispenseNotificationTag = 0;
			mProcessorWithCriticalScheduleTask = false;

			mSchedulerCommunicator = nullptr;
			mLastScheduledTask = nullptr;

			mTaskReserveQueue = new CTaskReserveArrayQueue(10000);
			mTaskReserveQueueController = nullptr;
			//mTaskReserveQueue = new CTaskReserveMultiArrayQueue(10000);
			//mTaskReserveQueueController = new CTaskReserveMultiArrayQueueAdderController(mTaskReserveQueue);
		}

		CTaskProcessorSchedulerThread::~CTaskProcessorSchedulerThread() {
		}

		bool CTaskProcessorSchedulerThread::requiresSchedulingTasks() {
			return false;
		}

		CTaskReserveQueue* CTaskProcessorSchedulerThread::getTaskReserveQueue() {
			return mTaskReserveQueue;
		}

		CTaskReserveQueueAdder* CTaskProcessorSchedulerThread::createTaskReserveQueueAdder() {
			if (mTaskReserveQueueController) {
				return mTaskReserveQueueController->createTaskReserveQueueAdder();
			} else {
				return nullptr;
			}
		}

		bool CTaskProcessorSchedulerThread::requiresTaskDispenseNotification(cint64 &updateDispenseNotificationTag) {
			bool requiresNotification = updateDispenseNotificationTag != mSchedulerDispenseNotificationTag;
			updateDispenseNotificationTag = mSchedulerDispenseNotificationTag;
			return requiresNotification;
		}


		CTaskSchedulerCommunicator* CTaskProcessorSchedulerThread::createSchedulerProcessorCommunication(CTaskEventHandlerBasedProcessor* taskProcessor) {
			CConcreteTaskSchedulerCommunicator* taskSchedulerCommunicator = new CConcreteTaskSchedulerCommunicator(this,taskProcessor);
			mCreateCommunicatorSyncMutex.lock();
			mSchedulerCommunicatorLinker = new CXLinker<CConcreteTaskSchedulerCommunicator*>(taskSchedulerCommunicator,mSchedulerCommunicatorLinker);
			mCreateCommunicatorSyncMutex.unlock();
			return taskSchedulerCommunicator;
		}


		CTaskReserveQueueConsumer* CTaskProcessorSchedulerThread::createTaskReserveQueueConsumer(CTaskEventHandlerBasedProcessor* taskProcessor) {
			CTaskReserveQueueConsumer* consumer = nullptr;
			if (mConfUseReserveTasksQueue && mTaskReserveQueue) {
				consumer = mTaskReserveQueue->createTaskReserveQueueConsumer();
			} 
			return consumer;
		}


		CTaskEventHandlerBasedProcessor* CTaskProcessorSchedulerThread::installScheduler(CTaskEventHandlerBasedScheduler* scheduler) {
			mTaskReserveQueueConsumer = scheduler->createTaskReserveQueueConsumer(this);
			CTaskSchedulerCommunicator* schedulerCommunicator = scheduler->createSchedulerProcessorCommunication(this);
			installSchedulerCommunicator(schedulerCommunicator);
			return this;
		}

		CTaskProcessorSchedulerThread* CTaskProcessorSchedulerThread::installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator) {
			mSchedulerCommunicator = schedulerCommunicator;
			processSchedulerRequestProcessTask(mSchedulerCommunicator);
			return this;
		}



		bool CTaskProcessorSchedulerThread::processEvent(CEvent *event, CContext* handlerContext) {
			mLastProcessedEvent = event;
			cint64 eventID = event->getEventTypeID();
			if (eventID == CSendTaskScheduleEvent::EVENTTYPEID) {
				CTask* taskIt = ((CSendTaskScheduleEvent*)event)->getTask();
				while (taskIt) {
					CTask* task = taskIt;
					taskIt = taskIt->getNext();
					task->clearNext();
					processSchedulerScheduleTask(task,true);
				}
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CRequestProcessTaskEvent::EVENTTYPEID) {
				CRequestProcessTaskEvent* reqEvent = (CRequestProcessTaskEvent*)event;
				processSchedulerRequestProcessTask(reqEvent->getCommunicator());
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CTaskDispenseNotificationEvent::EVENTTYPEID) {
				CTaskDispenseNotificationEvent* disNotEvent = (CTaskDispenseNotificationEvent*)event;
				processSchedulerDispenseNotification(disNotEvent->getCommunicator());
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CCriticalScheduleTaskEvent::EVENTTYPEID) {
				CCriticalScheduleTaskEvent* criticalScheduleTasksEvent = (CCriticalScheduleTaskEvent*)event;
				processSchedulingScheduleTasks();
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CResponseScheduleTaskEvent::EVENTTYPEID) {
				CResponseScheduleTaskEvent* resEvent = (CResponseScheduleTaskEvent*)event;
				processSchedulerResponseScheduleTask(resEvent->getTask(),resEvent->getCommunicator(),resEvent->getTaskScheduleItem(),resEvent->isSchedulingTask());
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (CTaskProcessorThreadBase::processEvent(event,handlerContext)) {
				return true;
			} else {
				// not supported
			}
			return false;
		}


		bool CTaskProcessorSchedulerThread::processUpdateTaskStatus(CTask* updateTask) {
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


		bool CTaskProcessorSchedulerThread::processUpdateTaskStatus(CTask* parentTask, CTask* updateTask) {
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
				// delegate task update to processor unit
				CTaskOwner* taskOwner = parentTask->getTaskOwner();
				bool scheduled = false;
				CXLinker<CConcreteTaskSchedulerCommunicator*>* commLinkIt = mSchedulerCommunicatorLinker;
				while (commLinkIt) {
					CConcreteTaskSchedulerCommunicator* communicator = commLinkIt->getData();
					if (communicator->hasTaskOwner(taskOwner)) {
						communicator->communicateUpdateTaskStatus(parentTask,updateTask,mMemoryAllocator);
						scheduled = true;
						break;
					}
					commLinkIt = commLinkIt->getNext();
				}
				if (!scheduled) {
					mDistributorCompletorCommunicator->communicateTaskStatusUpdated(parentTask,updateTask,false,mMemoryAllocator);
				}
			}
			return true;
		}


		bool CTaskProcessorSchedulerThread::processSchedulerRequestProcessTask(CTaskSchedulerCommunicator* communicator) {
			if (!mFreeSchedulerItemLinker) {
				mFreeSchedulerItemLinker = new CConcreteTaskScheduleItem();
			}
			CConcreteTaskScheduleItem* tmpTaskScheduleItem = mFreeSchedulerItemLinker;
			mFreeSchedulerItemLinker = mFreeSchedulerItemLinker->getNext();

			tmpTaskScheduleItem->initTaskScheduleItem((CConcreteTaskSchedulerCommunicator*)communicator,mWaitingSchedulerItemLinker);
			mWaitingSchedulerItemLinker = tmpTaskScheduleItem;
			mDeliveringWaitingCount++;

			processTaskScheduling();
			return true;
		}


		bool CTaskProcessorSchedulerThread::processSchedulerCriticalScheduleTask(CTaskSchedulerCommunicator* communicator) {
			mProcessorWithCriticalScheduleTask = true;
			processTaskScheduling();
			return true;
		}


		bool CTaskProcessorSchedulerThread::processSchedulerResponseScheduleTask(CTask* task, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, bool schedulingTask) {
			CConcreteTaskSchedulerCommunicator* concreteCommunicator = (CConcreteTaskSchedulerCommunicator*)communicator;
			concreteCommunicator->decRequestProcessingTask();
			--mRequestingTaskCount;
			processSchedulerScheduleTask(task,schedulingTask);
			return true;
		}

		bool CTaskProcessorSchedulerThread::processTaskScheduling() {
			bool requestingTasks = false;
			bool canContinueRequestProcessTask = true;
			bool canContinueRequestScheduleTask = true;
			if (mConfUseReserveTasksQueue && mTaskReserveQueueConsumer && mWaitingSchedulerItemLinker) {
				CTask* task = nullptr;
				do {
					task = mTaskReserveQueueConsumer->getTask();
					processSchedulerScheduleTask(task,true);
				} while (task && mWaitingSchedulerItemLinker);
			}
			while (canContinueRequestScheduleTask && mDeliveringWaitingCount > mRequestingTaskCount) {
				canContinueRequestScheduleTask = false;
				cint64 maxSchedTasks = 0;
				CConcreteTaskSchedulerCommunicator* maxSchedComm = nullptr;
				CXLinker<CConcreteTaskSchedulerCommunicator*>* schedulerCommunicatorLinkerIt = mSchedulerCommunicatorLinker;
				while (schedulerCommunicatorLinkerIt && mDeliveringWaitingCount > mRequestingTaskCount) {
					CConcreteTaskSchedulerCommunicator* communicator = schedulerCommunicatorLinkerIt->getData();
					cint64 schedTasks = communicator->getRequestUpdatedDispensableSchedulingTasksCount();
					if (schedTasks > 0) {
						if (!maxSchedComm || schedTasks > maxSchedTasks) {
							maxSchedComm = communicator;
							maxSchedTasks = schedTasks;
						}
					}
					schedulerCommunicatorLinkerIt = schedulerCommunicatorLinkerIt->getNext();
				}
				if (maxSchedComm) {
					maxSchedComm->communicateRequestScheduleTask(nullptr,mMemoryAllocator);
					maxSchedComm->incRequestProcessingTask();
					canContinueRequestScheduleTask = true;
					++mRequestingTaskCount;
					requestingTasks = true;
				}
			}
			while (canContinueRequestProcessTask && mDeliveringWaitingCount > mRequestingTaskCount) {
				canContinueRequestProcessTask = false;
				CXLinker<CConcreteTaskSchedulerCommunicator*>* schedulerCommunicatorLinkerIt = mSchedulerCommunicatorLinker;
				while (schedulerCommunicatorLinkerIt && mDeliveringWaitingCount > mRequestingTaskCount) {
					CConcreteTaskSchedulerCommunicator* communicator = schedulerCommunicatorLinkerIt->getData();
					if (mConfProcessingTasksDistribution && communicator->getRequestUpdatedDispensableProcessingAndSchedulingTasksCount() > 1) {
						communicator->communicateRequestScheduleTask(nullptr,mMemoryAllocator);
						communicator->incRequestProcessingTask();
						canContinueRequestProcessTask = true;
						++mRequestingTaskCount;
						requestingTasks = true;
					}
					schedulerCommunicatorLinkerIt = schedulerCommunicatorLinkerIt->getNext();
				}
			}
			if (mDeliveringWaitingCount > mRequestingTaskCount) {
				++mSchedulerDispenseNotificationTag;
				mRequiresTaskDispenseNotification = true;
			} else {
				mRequiresTaskDispenseNotification = false;
			}
			return requestingTasks;
		}



		bool CTaskProcessorSchedulerThread::processSchedulingScheduleTasks() {
			bool requiringSchedulingTask = true;
			if (mConfScheduleTasksDistribution) {
				while (requiringSchedulingTask && mTaskSchedulingCount > mCriticalSchedulingThresholdCount && mTaskSchedulingQueue) {
					requiringSchedulingTask = false;
					cint64 minSchedTasks = 1;
					CConcreteTaskSchedulerCommunicator* minSchedulerComm = nullptr;
					for (CXLinker<CConcreteTaskSchedulerCommunicator*>* schedulerCommunicatorLinkerIt = mSchedulerCommunicatorLinker; schedulerCommunicatorLinkerIt && minSchedTasks > 0; schedulerCommunicatorLinkerIt = schedulerCommunicatorLinkerIt->getNext()) {
						CConcreteTaskSchedulerCommunicator* schedulerComm = schedulerCommunicatorLinkerIt->getData();
						if (schedulerComm->requiresSchedulingTasks()) {
							requiringSchedulingTask = true;
							cint64 schedTasks = schedulerComm->getRequestUpdatedRemainingSchedulingTasksCount();
							if (schedTasks <= mCriticalSchedulingThresholdCount) {
								if (!minSchedulerComm || schedTasks < minSchedTasks) {
									minSchedTasks = schedTasks;
									minSchedulerComm = schedulerComm;
								}
							}
						}
					}
					if (minSchedulerComm) {
						requiringSchedulingTask = true;
						CTask* task = mTaskSchedulingQueue;
						mTaskSchedulingQueue = mTaskSchedulingQueue->getNext();
						--mTaskSchedulingCount;
						task->clearNext();
						minSchedulerComm->communicateProcessTask(task,true,mMemoryAllocator);
						minSchedulerComm->incSendedTasks();
					}
				}
			}
			return true;
		}


		bool CTaskProcessorSchedulerThread::processSchedulerScheduleTask(CTask* task, bool schedulingTask) {
			if (task) {
				if (mWaitingSchedulerItemLinker) {
					CConcreteTaskScheduleItem* concreteTaskScheduleItem = mWaitingSchedulerItemLinker;
					mWaitingSchedulerItemLinker = mWaitingSchedulerItemLinker->getNext();
					CConcreteTaskSchedulerCommunicator* communicator = concreteTaskScheduleItem->getCommunicator();
					communicator->communicateProcessTask(task,schedulingTask,mMemoryAllocator);
					communicator->incSendedTasks();
					mLastScheduledTask = task;
					--mDeliveringWaitingCount;
					mFreeSchedulerItemLinker = concreteTaskScheduleItem->setNext(mFreeSchedulerItemLinker);
				} else {
					if (!schedulingTask) {
						addProcessingTask(task);
					} else {
						if (mConfUseReserveTasksQueue) {
							mTaskReserveQueue->tryAddTasks(task);
							++mTaskReserveQueueCount;
						}
						if (task) {
							if (!mTaskProcessingQueue && !mTaskSchedulingQueue) {
								addProcessingTask(task);
							} else {
								mTaskSchedulingCount += task->getCount();
								if (mTaskSchedulingQueue) {
									mTaskSchedulingQueue = task->append(mTaskSchedulingQueue);
								} else {
									mTaskSchedulingQueue = task;
								}
								if (mTaskSchedulingCount >= mCriticalSchedulingThresholdCount) {
									processSchedulingScheduleTasks();
								}
							}
						}
					}
				}				
			}
			return true;
		}

		bool CTaskProcessorSchedulerThread::processSchedulerDispenseNotification(CTaskSchedulerCommunicator* communicator) {
			processTaskScheduling();
			return true;
		}


		bool CTaskProcessorSchedulerThread::processDispenseNotification() {
			if (canDispenseProcessingTasks() && requiresTaskDispenseNotification(mDispenseNotificationTag)) {
				processSchedulerDispenseNotification(mSchedulerCommunicator);
			}
			return true;
		}


		bool CTaskProcessorSchedulerThread::processRequestProcessTask() {
			processSchedulerRequestProcessTask(mSchedulerCommunicator);
			return true;
		}

		bool CTaskProcessorSchedulerThread::processCriticalScheduleTask() {
			return true;
		}


		bool CTaskProcessorSchedulerThread::processCompleteTask(CTask* task) {
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
					} else {
						// task completion by completer
						mDistributorCompletorCommunicator->communicateTaskComplete(completionTask,mMemoryAllocator);
					}
				}				
			}
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCompletedCount(completedCount));
			return true;
		}


		bool CTaskProcessorSchedulerThread::processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool) {
			CTask* parentTask = task->getParentTask();
			if (!parentTask || parentTask->hasNoTaskOwnerAndNoParentTask() || parentTask->hasTaskOwner(this)) {
				task->appendMemoryPool(memoryPool);
			} else {
				mDistributorCompletorCommunicator->communicateAdditionalAllocationTask(task,memoryPool,mMemoryAllocator);
			}
			return true;
		}

		bool CTaskProcessorSchedulerThread::processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask) {
			processSchedulerResponseScheduleTask(task,mSchedulerCommunicator,taskScheduleItem,schedulingTask);
			return true;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
