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

#include "CSingleThreadTaskProcessorUnit.h"


namespace Konclude {

	namespace Scheduler {



		CSingleThreadTaskProcessorUnit::CSingleThreadTaskProcessorUnit(CTaskHandleAlgorithm* taskHandleAlgo, CConsiderateMemoryPoolProvider* memoryPoolProvider) : CThread("TaskProcessingUnitThread") {
			mTaskProcessingQueue = nullptr;
			mTaskCompletionQueue = nullptr;
			mTaskStatusPropagator = nullptr;
			mCallbackExecuter = nullptr;
			mMemoryPoolProvider = memoryPoolProvider;
			if (!mMemoryPoolProvider) {
				mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
			}
			mMemoryAllocator = new CTaskHandleLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider,30000,30);
			mTaskProcessorContext = new CTaskProcessorContextBase(this,mMemoryAllocator);
			mTaskHandleAlgo = taskHandleAlgo;
			mProcessingBlocked = true;
			mEventHandler = new CQueuedLinkedEventHandler(this,this);

			CXLinker<CEventHandler*>* eventHandlerLinker = new CXLinker<CEventHandler*>(mEventHandler,nullptr);
			mEventHandlerLinker = eventHandlerLinker;

			mProcessingStopped = false;
			
			mStatComputionTime = 0;
			mStatBlockingTime = 0;

			mLastProcessingStartedTag = 0;
			mLastProcessingStartRequestTag = 0;
			mTaskProcessingCount = 0;
			mTaskSchedulingCount = 0;
			mStatRecievedScheduleTasks = 0;
			mStatRemovedTasks = 0;
			mEventSignalized = false;
			mThreadBlocked = false;
			mDebugLastProcessedTask = nullptr;
			mDebugLastCompletedTask = nullptr;
			mTaskSchedulingQueue = nullptr;
		}

		CSingleThreadTaskProcessorUnit::~CSingleThreadTaskProcessorUnit() {
		}


		cint64 CSingleThreadTaskProcessorUnit::countProcessingTasksMemoryPools() {
			cint64 memoryPoolCount = 0;
			CTask* taskProcessingQueueIt = mTaskProcessingQueue;
			while (taskProcessingQueueIt) {
				CMemoryPool* memoryPoolIt = taskProcessingQueueIt->getMemoryPools();
				memoryPoolCount += memoryPoolIt->getCount();
				taskProcessingQueueIt = taskProcessingQueueIt->getNext();
			}
			return memoryPoolCount;
		}


		cint64 CSingleThreadTaskProcessorUnit::countProcessedOpenTasksMemoryPools() {
			cint64 memoryPoolCount = 0;
			QSet<CTask*> taskSet;
			QList<CTask*> taskList;
			CTask* taskProcessingQueueIt = mTaskProcessingQueue;
			while (taskProcessingQueueIt) {
				if (taskProcessingQueueIt->getParentTask()) {
					if (!taskSet.contains(taskProcessingQueueIt->getParentTask())) {
						taskSet.insert(taskProcessingQueueIt->getParentTask());
						taskList.append(taskProcessingQueueIt->getParentTask());
					}
				}
				taskProcessingQueueIt = taskProcessingQueueIt->getNext();
			}

			while (!taskList.isEmpty()) {
				CTask* task = taskList.takeFirst();
				if (task->getParentTask()) {
					if (!taskSet.contains(task->getParentTask())) {
						taskSet.insert(task->getParentTask());
						taskList.append(task->getParentTask());
					}
				}
				CMemoryPool* memoryPoolIt = task->getMemoryPools();
				memoryPoolCount += memoryPoolIt->getCount();
			}

			return memoryPoolCount;
		}





		cint64 CSingleThreadTaskProcessorUnit::closeOpenTasksMemoryPools() {
			cint64 closedMemoryPoolCount = 0;
			QSet<CTask*> taskSet;
			QList<CTask*> taskList;
			CTask* taskProcessingQueueIt = mTaskProcessingQueue;
			while (taskProcessingQueueIt) {
				if (!taskSet.contains(taskProcessingQueueIt)) {
					taskSet.insert(taskProcessingQueueIt);
					taskList.append(taskProcessingQueueIt);
				}
				taskProcessingQueueIt = taskProcessingQueueIt->getNext();
			}

			while (!taskList.isEmpty()) {

				CTask* task = taskList.takeFirst();
				CBooleanTaskResult* satResult = (CBooleanTaskResult*)task->getTaskResult();
				if (satResult) {
					satResult->installResult(false);
				}
				CCallbackData* callbackLinkerIt = task->getCallbackLinker();
				while (callbackLinkerIt) {
					CCallbackData* callback = callbackLinkerIt;
					callbackLinkerIt = callbackLinkerIt->getNext();
					if (mCallbackExecuter) {
						mCallbackExecuter->executeCallback(task,callback);
						INCTASKPROCESSINGSTAT(mStats.incStatisticCallbacksExecutedCount(1));
					} else {
						callback->doCallback();
					}
				}
				if (task->getParentTask()) {
					if (!taskSet.contains(task->getParentTask())) {
						taskSet.insert(task->getParentTask());
						taskList.append(task->getParentTask());
					}
				}
				CMemoryPool* memoryPoolIt = task->getMemoryPools();
				closedMemoryPoolCount += memoryPoolIt->getCount();
				mMemoryAllocator->releaseMemoryPoolContainer(task);
			}

			mTaskProcessingQueue = 0;
			mTaskCompletionQueue = 0;

			return closedMemoryPoolCount;
		}


		CTaskHandleAlgorithm* CSingleThreadTaskProcessorUnit::getTaskHandleAlgorithm() {
			return mTaskHandleAlgo;
		}

		CSingleThreadTaskProcessorUnit* CSingleThreadTaskProcessorUnit::installCallbackExecuter(CTaskCallbackExecuter* callbackExecuter) {
			mCallbackExecuter = callbackExecuter;
			return this;
		}

		CSingleThreadTaskProcessorUnit* CSingleThreadTaskProcessorUnit::installStatusPropagator(CTaskStatusPropagator* statusPropagator) {
			mTaskStatusPropagator = statusPropagator;
			return this;
		}

		cint64 CSingleThreadTaskProcessorUnit::getTaskProcessingCount() {
			return mTaskProcessingCount + mTaskSchedulingCount;
		}

		bool CSingleThreadTaskProcessorUnit::canDispenseProcessingTasks() {
			return false;
		}

		bool CSingleThreadTaskProcessorUnit::requiresProcessingTasks() {
			return false;
		}

		bool CSingleThreadTaskProcessorUnit::requiresSchedulingTasks() {
			return false;
		}

		cint64 CSingleThreadTaskProcessorUnit::countDispensableProcessingTasks() {
			return 0;
		}

		cint64 CSingleThreadTaskProcessorUnit::getRecievedTasks() {
			return 0;
		}

		cint64 CSingleThreadTaskProcessorUnit::countScheduleableProcessingTasks() {
			return 0;
		}

		bool CSingleThreadTaskProcessorUnit::requiresTaskDispenseNotification(cint64 &updateDispenseNotificationTag) {
			return false;
		}


		CTaskProcessingStatistics* CSingleThreadTaskProcessorUnit::getTaskProcessingStatistics() {
			return &mStats;
		}

		cint64 CSingleThreadTaskProcessorUnit::getStatisticBlockingTime() {
			cint64 blockTime = mStatBlockingTime;
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
			if (mThreadBlocked) {
				blockTime += mBlockingTimer.elapsed();
			}
#endif
			return blockTime;
		}

		cint64 CSingleThreadTaskProcessorUnit::getStatisticComputionTime() {
			cint64 compTime = mStatComputionTime;
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
			if (!mThreadBlocked) {
				compTime += mComputionTimer.elapsed();
			}
#endif
			return compTime;
		}

		CEventHandler* CSingleThreadTaskProcessorUnit::getEventHandler() {
			return mEventHandler;
		}

		CTaskEventHandlerBasedProcessor* CSingleThreadTaskProcessorUnit::installScheduler(CTaskEventHandlerBasedScheduler* scheduler) {
			return this;
		}
				
		CTaskSchedulerCommunicator* CSingleThreadTaskProcessorUnit::createSchedulerProcessorCommunication(CTaskEventHandlerBasedProcessor* taskProcessor) {
			return nullptr;
		}

		CTaskReserveQueueConsumer* CSingleThreadTaskProcessorUnit::createTaskReserveQueueConsumer(CTaskEventHandlerBasedProcessor* taskProcessor) {
			return nullptr;
		}


		CSingleThreadTaskProcessorUnit* CSingleThreadTaskProcessorUnit::startProcessing() {
			if (!isRunning()) {
				startThread();
				postEvent(new Concurrent::Events::CHandleEventsEvent());
			}
			return this;
		}

		CSingleThreadTaskProcessorUnit* CSingleThreadTaskProcessorUnit::stopProcessing() {
			mProcessingStopped = true;
			return this;
		}

		CThreadActivator* CSingleThreadTaskProcessorUnit::signalizeEvent() {
			mEventSignalized = true;
			if (mProcessingBlocked) {
				// reactivate processing
				if (mLastProcessingStartedTag == mLastProcessingStartRequestTag) {
					++mLastProcessingStartRequestTag;
					mProcessingWakeUpSemaphore.release();
				}
			}
			return this;
		}


		bool CSingleThreadTaskProcessorUnit::processControlEvents(QEvent::Type type, CControlEvent *event) {
			if (CThread::processControlEvents(type,event)) {
				return true;
			} else {
				if (type == Concurrent::Events::CHandleEventsEvent::EVENTTYPE) {
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
					mComputionTimer.start();
#endif
					processingLoop();
					return true;
				}
			}
			return false;
		}

		bool CSingleThreadTaskProcessorUnit::processingLoop() {
			bool eventSafeguardProcessed = false;
			while (!mProcessingStopped) {
				if (!mTaskProcessingQueue && mProcessingBlocked) {
					// block until new events or task are available
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
					mStatComputionTime += mComputionTimer.elapsed();
					mBlockingTimer.start();
#endif
					INCTASKPROCESSINGSTAT(mStats.incStatisticThreadsBlockedCount(1));
					mThreadBlocked = true;
					mProcessingWakeUpSemaphore.acquire(1);
					mThreadBlocked = false;
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
					mStatBlockingTime += mBlockingTimer.elapsed();
					mComputionTimer.start();
#endif
					mLastProcessingStartedTag = mLastProcessingStartRequestTag;
				}
				mProcessingBlocked = false;
				eventSafeguardProcessed = false;
				while (mEventSignalized) {
					bool eventsProcessed = handleEvents();
				}
				if (mTaskProcessingQueue) {
					CTask* processingTask = mTaskProcessingQueue;
					mTaskProcessingQueue = mTaskProcessingQueue->getNext();
					cint64 taskDepth = processingTask->getTaskDepth();

					//cint64 memoryPoolCount1 = countProcessingTasksMemoryPools();
					//cint64 memoryPoolCount2 = countProcessedOpenTasksMemoryPools();

					bool continueProcessing = processTask(processingTask);
					if (continueProcessing) {
						processingTask->getTaskStatus()->setTaskQUEUEDState();
						if (mTaskProcessingQueue) {
							mTaskProcessingQueue = mTaskProcessingQueue->insertNextSorted(processingTask);
						} else {
							mTaskProcessingQueue = processingTask;
						}
					} else {
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksProcessedDepthCount(taskDepth,1));
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksProcessedCount(1));
						--mTaskProcessingCount;
						++mStatRemovedTasks;
					}

					//if (mStatRemovedTasks > 100000) {
					//	closeOpenTasksMemoryPools();
					//}
				}
				if (!mTaskProcessingQueue && mTaskSchedulingQueue) {
					CTask* nextTask = mTaskSchedulingQueue;
					mTaskSchedulingQueue = mTaskSchedulingQueue->getNext();
					nextTask->clearNext();
					--mTaskSchedulingCount;
					addProcessingTask(nextTask);
				}

				if (!mTaskProcessingQueue) {
					while (mEventSignalized || !eventSafeguardProcessed) {
						bool eventsProcessed = handleEvents();
						if (eventsProcessed) {
							eventSafeguardProcessed = false;
							mProcessingBlocked = false;
						} else if (!mProcessingBlocked) {
							mProcessingBlocked = true;
						} else if (!eventSafeguardProcessed) {
							eventSafeguardProcessed = true;
						}
					}
				}
			}
			return true;
		}

		bool CSingleThreadTaskProcessorUnit::processEvent(CEvent *event, CContext* handlerContext) {
			cint64 eventID = event->getEventTypeID();
			if (eventID == CSendTaskProcessEvent::EVENTTYPEID) {				
				CTask* task = ((CSendTaskProcessEvent*)event)->getTask();
				addProcessingTask(task);
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CSendTaskScheduleEvent::EVENTTYPEID) {
				++mStatRecievedScheduleTasks;
				CTask* task = ((CSendTaskScheduleEvent*)event)->getTask();
				if (!mTaskProcessingQueue) {
					addProcessingTask(task);
				} else {
					++mTaskSchedulingCount;
					if (mTaskSchedulingQueue) {
						mTaskSchedulingQueue = mTaskSchedulingQueue->append(task);
					} else {
						mTaskSchedulingQueue = task;
					}
				}
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CRequestProcessTaskEvent::EVENTTYPEID) {
				// not supported
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CResponseScheduleTaskEvent::EVENTTYPEID) {
				// not supported
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CSendTaskCompleteEvent::EVENTTYPEID) {
				CTask* task = ((CSendTaskCompleteEvent*)event)->getTask();
				completeTask(task);
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CTaskAdditionalAllocationEvent::EVENTTYPEID) {
				CTaskAdditionalAllocationEvent* taskAddAllocEvent = (CTaskAdditionalAllocationEvent*)event;
				CTask* task = taskAddAllocEvent->getTask();
				CMemoryPool* addMemoryPools = taskAddAllocEvent->getAdditionalAllocatedMemoryPools();
				task->appendMemoryPool(addMemoryPools);
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CRequestScheduleTaskEvent::EVENTTYPEID) {
				// this unit is also the scheduler
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else {
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			}
			return false;
		}


		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskComplete(CTask* task) {
			task->getTaskStatus()->setTaskFINISHEDState();
			completeTask(task);
			return this;
		}

		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskError(CTask* task) {
			task->getTaskStatus()->setTaskFINISHEDState();
			completeTask(task);
			return this;
		}


		bool CSingleThreadTaskProcessorUnit::addProcessingTask(CTask* task) {
			CTask* newTask = task;
			CTask* newTaskIt = newTask;
			cint64 taskCount = 0;
			while (newTaskIt) {
				newTaskIt->getTaskStatus()->setTaskQUEUEDState();
				newTaskIt = newTaskIt->getNext();
				INCTASKPROCESSINGSTAT(mStats.incStatisticTasksAddedCount(1));
				++taskCount;
			}
			mTaskProcessingCount += taskCount;
			if (!mTaskProcessingQueue) {
				mTaskProcessingQueue = newTask;
				newTask = newTask->getNext();
				mTaskProcessingQueue->clearNext();
			}
			if (newTask) {
				mTaskProcessingQueue = mTaskProcessingQueue->insertNextSorted(newTask);
			}
			return task != nullptr;
		}


		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskCreation(CTask* newTask) {
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCreatedCount(newTask->getCount()));
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCreatedDepthCount(newTask->getTaskDepth(),newTask->getCount()));

			addProcessingTask(newTask);
			return this;
		}

		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskAdditionalAllocation(CTask* task, CMemoryPool* additionalAllocatedMemoryPool) {
			task->appendMemoryPool(additionalAllocatedMemoryPool);
			return this;
		}


		bool CSingleThreadTaskProcessorUnit::verifyContinueTaskProcessing(CTask* task) {
			if (mEventSignalized) {
				return false;
			}
			if (!task->getTaskStatus()->isProcessable()) {
				return false;
			}
			if (mTaskProcessingQueue && mTaskProcessingQueue->getTaskPriority() > task->getTaskPriority()) {
				return false;
			}
			return true;
		}


		bool CSingleThreadTaskProcessorUnit::handleEvents() {
			bool handleNextRound = true;
			bool roundEventProcessed = false;
			bool eventProcessed = false;
			while (handleNextRound && !mProcessingStopped) {
				roundEventProcessed = false;
				CXLinker<CEventHandler*>* eventHandlerLinkerIt = mEventHandlerLinker;
				if (eventHandlerLinkerIt) {

					CEventHandler* eventHandler = eventHandlerLinkerIt->getData();
					if (eventHandler->needEventProcessing()) {
						cint64 handledEventCount = eventHandler->handleEvents(mTaskProcessorContext);
						INCTASKPROCESSINGSTAT(mStats.incStatisticEventsProcessedCount(handledEventCount));
						roundEventProcessed = handledEventCount > 0;
					}
					// round robin
					eventHandlerLinkerIt = eventHandlerLinkerIt->getNext();
					if (!eventHandlerLinkerIt) {
						if (mEventSignalized || roundEventProcessed) {
							handleNextRound = true;
							mEventSignalized = false;
						} else {
							handleNextRound = false;
						}
						eventHandlerLinkerIt = mEventHandlerLinker;
					}
					eventProcessed |= roundEventProcessed;
				} else {
					handleNextRound = false;
				}
			}
			return eventProcessed;
		}



		cint64 CSingleThreadTaskProcessorUnit::completeTask(CTask* task) {
			cint64 completedCount = 0;
			mTaskCompletionQueue = task->getLastListLink()->setNext(mTaskCompletionQueue);
			while (mTaskCompletionQueue) {
				CTask* completionTask = mTaskCompletionQueue;
				mTaskCompletionQueue = mTaskCompletionQueue->getNext();

				if (completionTask && !completionTask->hasActiveReferencedTask()) {
					mDebugLastCompletedTask = completionTask;

					CTask* parentTask = completionTask->getParentTask();
					bool upPropagation = false;
					if (mTaskStatusPropagator && mTaskStatusPropagator->completeTaskStatus(completionTask,upPropagation)) {
						if (parentTask && upPropagation) {
							updateTaskStatus(parentTask);
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
					if (memoryReleaseable) {
						for (CDeletionLinker* deletionLinkerIt = completionTask->takeDeletionLinker(); deletionLinkerIt; deletionLinkerIt = deletionLinkerIt->getNext()) {
							deletionLinkerIt->deleteObject();
						}
						mMemoryAllocator->releaseMemoryPoolContainer(completionTask);
					}
					completedCount++;
				}				
			}
			INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCompletedCount(completedCount));
			return completedCount;
		}


		bool CSingleThreadTaskProcessorUnit::processTask(CTask* task) {
			mDebugLastProcessedTask = task;
			task->clearNext();
			task->getTaskStatus()->setTaskPROCESSINGState();
			return mTaskHandleAlgo->handleTask(mTaskProcessorContext,task);
		}


		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskRelevant(CTask* task) {
			// nothing to do for single processor unit
			return this;
		}


		CTaskProcessorCommunicator* CSingleThreadTaskProcessorUnit::communicateTaskStatusUpdate(CTask* task) {
			updateTaskStatus(task);
			return this;
		}


		bool CSingleThreadTaskProcessorUnit::updateTaskStatusDown(CTask* task, cint64 depth) {
			if (depth < 1000) {
				bool downProp = false;
				bool upProp = false;
				CTask* updateTask = task;
				updateTask->setTaskID(-1);
				if (mTaskStatusPropagator && mTaskStatusPropagator->updateTaskStatus(updateTask, downProp, upProp)) {
					INCTASKPROCESSINGSTAT(mStats.incStatisticTasksUpdatedCount(1));

					if (downProp) {
						CXNegLinker<CTask*>* refTaskIt = updateTask->getReferencedTaskLinker();
						while (refTaskIt) {
							if (refTaskIt->isNegated()) {
								// task still exists, update task status
								// TODO: check better not recursive?
								CTask* refTask = refTaskIt->getData();
								updateTaskStatusDown(refTask, depth + 1);
							}
							refTaskIt = refTaskIt->getNext();
						}
					}
				}

			} else {
				QList<CTask*> updateTaskList;
				updateTaskList.append(task);
				while (!updateTaskList.isEmpty()) {
					bool downProp = false;
					bool upProp = false;
					CTask* updateTask = updateTaskList.takeFirst();
					if (mTaskStatusPropagator && mTaskStatusPropagator->updateTaskStatus(updateTask, downProp, upProp)) {
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksUpdatedCount(1));

						if (downProp) {
							CXNegLinker<CTask*>* refTaskIt = updateTask->getReferencedTaskLinker();
							while (refTaskIt) {
								if (refTaskIt->isNegated()) {
									// task still exists, update task status
									// TODO: check better not recursive?
									CTask* refTask = refTaskIt->getData();
									updateTaskList.append(refTask);
								}
								refTaskIt = refTaskIt->getNext();
							}
						}
					}

				}
			}
			return true;
		}


		bool CSingleThreadTaskProcessorUnit::updateTaskStatus(CTask* task) {
			CTask* updateTask = task;
			bool downProp = false;
			bool upProp = false;
			while (updateTask) {
				if (mTaskStatusPropagator && mTaskStatusPropagator->updateTaskStatus(updateTask,downProp,upProp)) {
					INCTASKPROCESSINGSTAT(mStats.incStatisticTasksUpdatedCount(1));
					if (downProp) {
						CXNegLinker<CTask*>* refTaskIt = updateTask->getReferencedTaskLinker();
						while (refTaskIt) {
							if (refTaskIt->isNegated()) {
								// task still exists, update task status
								// TODO: check better not recursive?
								CTask* refTask = refTaskIt->getData();
								updateTaskStatusDown(refTask, 0);
							}
							refTaskIt = refTaskIt->getNext();
						}

					}
					if (upProp) {
						// all parent task are owned by the completer
						//CTask* parentTask = updateTask->getParentTask();
						//if (parentTask) {
						//	updateTaskStatus(parentTask);
						//}
						updateTask = updateTask->getParentTask();
					} else {
						updateTask = nullptr;
					}
				} else {
					updateTask = nullptr;
				}
			}
			return true;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
