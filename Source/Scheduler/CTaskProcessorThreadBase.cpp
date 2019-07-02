/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CTaskProcessorThreadBase.h"


namespace Konclude {

	namespace Scheduler {



		CTaskProcessorThreadBase::CTaskProcessorThreadBase(CTaskHandleAlgorithm* taskHandleAlgo, const QString &threadStringName) : CThread(threadStringName) {
			mTaskProcessingQueue = nullptr;
			mTaskSchedulingQueue = nullptr;
			mTaskCompletionQueue = nullptr;
			mMemoryAllocator = nullptr;
			mTaskProcessorContext = nullptr;
			mTaskStatusPropagator = nullptr;
			mCallbackExecuter = nullptr;
			mTaskHandleAlgo = taskHandleAlgo;
			mProcessingBlocked = true;
			mEventHandler = new CQueuedLinkedEventHandler(this,this);

			CXLinker<CEventHandler*>* eventHandlerLinker = new CXLinker<CEventHandler*>(mEventHandler,nullptr);
			mEventHandlerLinker = eventHandlerLinker;

			mTaskReserveQueueConsumer = nullptr;

			mProcessingStopped = false;
			mThreadBlocked = false;

			mStatComputionTime = 0;
			mStatBlockingTime = 0;

			mLastProcessingStartedTag = 0;
			mLastProcessingStartRequestTag = 0;
			mTaskProcessingCount = 0;
			mEventSignalized = false;
			mProcessTaskRequesting = true;

			mTaskSchedulingCount = 0;
			mCriticalSchedulingCount = 3;
			mCriticalSchedulingThresholdCount = 5;
			mCriticalSchedulingNotified = true;
			mRecievedTasks = 0;
			mTaskReserveQueueCount = 0;
			mTaskParentRelevantCount = 0;

			mConfScheduleTasksDistribution = false;
			mConfProcessingTasksDistribution = false;
			mConfUseReserveTasksQueue = true;

			mMinimalPriorityTaskInvalidated = false;

			mMinTaskPriority = 0.;
			mMinPriorityTask = nullptr;

			mConfProcessSchedulingTasks = true;
			mConfProcessReserveTasks = true;

			// for debugging only
			mLastProcessedTask = nullptr;
			mLastDispendedTask = nullptr;
			mLastProcessedEvent = nullptr;
		}

		CTaskProcessorThreadBase::~CTaskProcessorThreadBase() {
		}


		CTaskHandleAlgorithm* CTaskProcessorThreadBase::getTaskHandleAlgorithm() {
			return mTaskHandleAlgo;
		}

		bool CTaskProcessorThreadBase::isProcessing() {
			return !mProcessingBlocked;
		}

		bool CTaskProcessorThreadBase::isBlocked() {
			return mProcessingBlocked;
		}

		CTaskProcessorThreadBase* CTaskProcessorThreadBase::installCallbackExecuter(CTaskCallbackExecuter* callbackExecuter) {
			mCallbackExecuter = callbackExecuter;
			return this;
		}

		CTaskProcessorThreadBase* CTaskProcessorThreadBase::installStatusPropagator(CTaskStatusPropagator* statusPropagator) {
			mTaskStatusPropagator = statusPropagator;
			return this;
		}


		CTaskProcessingStatistics* CTaskProcessorThreadBase::getTaskProcessingStatistics() {
			return &mStats;
		}

		cint64 CTaskProcessorThreadBase::getStatisticBlockingTime() {
			cint64 blockTime = mStatBlockingTime;
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
			if (mThreadBlocked) {
				blockTime += mBlockingTimer.elapsed();
			}
#endif
			return blockTime;
		}

		cint64 CTaskProcessorThreadBase::getStatisticComputionTime() {
			cint64 compTime = mStatComputionTime;
#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
			if (!mThreadBlocked) {
				compTime += mComputionTimer.elapsed();
			}
#endif
			return compTime;
		}

		CEventHandler* CTaskProcessorThreadBase::getEventHandler() {
			return mEventHandler;
		}


		bool CTaskProcessorThreadBase::canDispenseProcessingTasks() {
			return mTaskProcessingCount+mTaskSchedulingCount >= 2;
		}

		bool CTaskProcessorThreadBase::requiresProcessingTasks() {
			return mTaskProcessingCount+mTaskSchedulingCount <= 0;
		}


		bool CTaskProcessorThreadBase::requiresSchedulingTasks() {
			return mCriticalSchedulingNotified;
		}

		cint64 CTaskProcessorThreadBase::countDispensableProcessingTasks() {
			return mTaskProcessingCount+mTaskSchedulingCount;
		}

		cint64 CTaskProcessorThreadBase::countScheduleableProcessingTasks() {
			return mTaskSchedulingCount;
		}

		cint64 CTaskProcessorThreadBase::getRecievedTasks() {
			return mRecievedTasks;
		}


		CTaskProcessorThreadBase* CTaskProcessorThreadBase::startProcessing() {
			if (!isRunning()) {
				startThread(QThread::HighPriority);
				postEvent(new Concurrent::Events::CHandleEventsEvent());
			}
			return this;
		}

		CTaskProcessorThreadBase* CTaskProcessorThreadBase::stopProcessing() {
			mProcessingStopped = true;
			return this;
		}

		CThreadActivator* CTaskProcessorThreadBase::signalizeEvent() {
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


		bool CTaskProcessorThreadBase::processControlEvents(QEvent::Type type, CControlEvent *event) {
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

		bool CTaskProcessorThreadBase::processingLoop() {
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
					mProcessingWakeUpSemaphore.tryAcquire(1,100);
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
					--mTaskProcessingCount;
					cint64 taskDepth = processingTask->getTaskDepth();
					bool continueProcessing = processTask(processingTask);
					if (continueProcessing) {
						addProcessingTask(processingTask);
					} else {
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksProcessedDepthCount(taskDepth,1));
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksProcessedCount(1));
					}
				}
				if (!mTaskProcessingQueue && mTaskSchedulingQueue && mConfProcessSchedulingTasks) {
					CTask* nextTask = mTaskSchedulingQueue;
					mTaskSchedulingQueue = mTaskSchedulingQueue->getNext();
					nextTask->clearNext();
					--mTaskSchedulingCount;
					addProcessingTask(nextTask);
					if (!mCriticalSchedulingNotified && mTaskSchedulingCount <= mCriticalSchedulingCount) {
						mCriticalSchedulingNotified = true;
						processCriticalScheduleTask();
					}
				}
				if (!mTaskProcessingQueue && mConfProcessReserveTasks) {
					if (mConfUseReserveTasksQueue && mTaskReserveQueueConsumer) {
						CTask* task = mTaskReserveQueueConsumer->getTask();
						if (task) {
							++mTaskReserveQueueCount;
							addProcessingTask(task);
						}
					}
				}
				processDispenseNotification();
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
					if (!mTaskProcessingQueue && !mProcessTaskRequesting) {
						mProcessTaskRequesting = true;
						processRequestProcessTask();
					}
				}
			}
			return true;
		}

		bool CTaskProcessorThreadBase::addProcessingTask(CTask* task) {
			if (task) {

				CTask* taskIt = task;
				cint64 taskCount = 0;
				double minPriority = 0.;
				CTask* minPriorityTask = nullptr;
				while (taskIt) {
					taskIt->getTaskStatus()->setTaskQUEUEDState();
					if (!minPriorityTask || taskIt->getTaskPriority() < minPriority) {
						minPriority = taskIt->getTaskPriority();
						minPriorityTask = taskIt;
					}
					++taskCount;
					taskIt = taskIt->getNext();
				}


				if (minPriorityTask) {
					if (minPriority < mMinTaskPriority) {
						mMinimalPriorityTaskInvalidated = true;
					}
				}

				mTaskProcessingCount += taskCount;
				INCTASKPROCESSINGSTAT(mStats.incStatisticTasksAddedCount(1));

				if (!mTaskProcessingQueue) {
					mTaskProcessingQueue = task;
					task = task->getNext();
					mTaskProcessingQueue->clearNext();
				}
				if (task) {
					mTaskProcessingQueue = mTaskProcessingQueue->insertNextSorted(task);
				}


				mProcessTaskRequesting = false;

				return true;
			}
			return false;
		}


		bool CTaskProcessorThreadBase::processEvent(CEvent *event, CContext* handlerContext) {
			mLastProcessedEvent = event;
			cint64 eventID = event->getEventTypeID();
			if (eventID == CSendTaskProcessEvent::EVENTTYPEID) {	
				CSendTaskProcessEvent* sendTaskProcessEvent = (CSendTaskProcessEvent*)event;
				CTask* task = sendTaskProcessEvent->getTask();
				bool schedulable = sendTaskProcessEvent->isSchedulingTask();
				++mRecievedTasks;
				if (!mTaskProcessingQueue || !schedulable) {
					addProcessingTask(task);
				} else {
					++mTaskSchedulingCount;
					if (mTaskSchedulingQueue) {
						mTaskSchedulingQueue = mTaskSchedulingQueue->append(task);
					} else {
						mTaskSchedulingQueue = task;
					}
					if (mTaskSchedulingCount >= mCriticalSchedulingThresholdCount) {
						mCriticalSchedulingNotified = false;
					}
				}
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CSendTaskCompleteEvent::EVENTTYPEID) {
				CTask* task = ((CSendTaskCompleteEvent*)event)->getTask();
				processCompleteTask(task);
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CTaskAdditionalAllocationEvent::EVENTTYPEID) {
				CTaskAdditionalAllocationEvent* taskAddAllocEvent = (CTaskAdditionalAllocationEvent*)event;
				CTask* task = taskAddAllocEvent->getTask();
				CMemoryPool* addMemoryPools = taskAddAllocEvent->getAdditionalAllocatedMemoryPools();
				processAdditionalTaskAllocation(task,addMemoryPools);
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				return true;
			} else if (eventID == CRequestScheduleTaskEvent::EVENTTYPEID) {
				CTaskScheduleItem* taskScheduleItem = ((CRequestScheduleTaskEvent*)event)->getTaskScheduleItem();
				mMemoryAllocator->releaseMemoryPoolContainer(event);
				CTask* responseTask = mTaskSchedulingQueue;
				bool schedulingTask = false;
				if (responseTask) {
					--mTaskSchedulingCount;
					schedulingTask = true;
					mTaskSchedulingQueue = mTaskSchedulingQueue->getNext();
					if (!mCriticalSchedulingNotified && mTaskSchedulingCount <= mCriticalSchedulingCount) {
						mCriticalSchedulingNotified = true;
						processCriticalScheduleTask();
					}
				} else {
					if (mConfProcessingTasksDistribution) {
						CTask* lastTask = nullptr;
						CTask* taskIt = mTaskProcessingQueue;
						// response least priority task 
						while (taskIt && taskIt->hasNext()) {					
							lastTask = taskIt;
							taskIt = taskIt->getNext();
						}
						if (lastTask) {
							responseTask = lastTask->getNext();
							lastTask->clearNext();
							--mTaskProcessingCount;
						}
					} else if (mConfProcessingTasksDistribution) {
						CTask* lastBeforeRelevantTask = nullptr;

						CTask* lastTask = nullptr;
						CTask* taskIt = mTaskProcessingQueue;
						// response least priority task which is relevant
						while (taskIt && taskIt->hasNext()) {					
							lastTask = taskIt;
							CTask* parentTask = taskIt->getParentTask();
							if (!parentTask || parentTask->isTaskRelevant()) {
								lastBeforeRelevantTask = lastTask;
							}
							taskIt = taskIt->getNext();
						}
						if (lastBeforeRelevantTask) {
							responseTask = lastBeforeRelevantTask->getNext();
							lastBeforeRelevantTask->setNext(responseTask->getNext());
							responseTask->clearNext();
							--mTaskProcessingCount;
							--mTaskParentRelevantCount;
						}
					}
					if (responseTask) {
						INCTASKPROCESSINGSTAT(mStats.incStatisticTasksRequestedCount(1));
						responseTask->clearTaskOwner();
						CTask* parentTask = responseTask->getParentTask();
						while (parentTask) {
							if (parentTask->hasTaskOwner()) {
								parentTask->clearTaskOwner();
								parentTask = parentTask->getParentTask();
							} else {
								parentTask = nullptr;
							}
						}
						mLastDispendedTask = responseTask;
						responseTask->getTaskStatus()->setTaskSCHEDULINGState();
					}
				}
				if (responseTask) {
					responseTask->clearNext();
				}
				processResponseScheduleTask(responseTask,taskScheduleItem,schedulingTask);
				return true;
			} else if (eventID == CUpdateTaskStatusEvent::EVENTTYPEID) {
				CUpdateTaskStatusEvent* utse = (CUpdateTaskStatusEvent*)event;
				processUpdateTaskStatus(utse->getParentTask(),utse->getUpdateTask());
				return true;
			} else {
				// not supported
			}
			return false;
		}


		void CTaskProcessorThreadBase::organizeDispenseTasks(bool forceUpdate) {
			if (!mMinimalPriorityTaskInvalidated || forceUpdate) {
				// count dispense task and find minimal priority
				CTask* taskIt = mTaskProcessingQueue;
				double minPriority = 0.;
				CTask* minPriorityTask = nullptr;
				cint64 dispenseTaskCount = 0;
				while (taskIt) {
					CTask* task = taskIt;
					if (task->isTaskDispenseMarked()) {
						++dispenseTaskCount;
						double priorty = task->getTaskPriority();
						if (!minPriorityTask || minPriority < priorty) {
							minPriority = priorty;
							minPriorityTask = task;
						}
					}
					taskIt = taskIt->getNext();
				}

				mMinTaskPriority = minPriority;
				mMinPriorityTask = minPriorityTask;
				mMinimalPriorityTaskInvalidated = false;
			}
		}


		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskComplete(CTask* task) {
			task->getTaskStatus()->setTaskFINISHEDState();
			processCompleteTask(task);
			return this;
		}

		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskError(CTask* task) {
			task->getTaskStatus()->setTaskFINISHEDState();
			processCompleteTask(task);
			return this;
		}

		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskCreation(CTask* newTask) {
			CTask* taskIt = newTask;
			while (taskIt) {
				if (!taskIt->hasTaskOwner()) {
					taskIt->setTaskOwner(this);
				}
				INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCreatedCount(1));
				INCTASKPROCESSINGSTAT(mStats.incStatisticTasksCreatedDepthCount(taskIt->getTaskDepth(),1));
				taskIt = taskIt->getNext();
			}
			addProcessingTask(newTask);
			return this;
		}


		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskStatusUpdate(CTask* task) {
			processUpdateTaskStatus(task);
			return this;
		}



		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskRelevant(CTask* task) {
			// TODO: mark task as relevant and prepare for distribution
			if (!task->isTaskRelevant()) {
				if (task->hasNoTaskOwnerAndNoParentTask() || task->hasTaskOwner(this)) {
					task->setTaskRelevant(true);
					CXNegLinker<CTask*>* refTaskLinker = task->getReferencedTaskLinker();
					while (refTaskLinker) {
						bool refTaskCompleted = !refTaskLinker->isNegated();
						if (!refTaskCompleted) {
							CTask* refTask = refTaskLinker->getData();
							if (refTask->getTaskStatus()->isTaskStateQUEUED()) {
								++mTaskParentRelevantCount;
							}
						}
						refTaskLinker = refTaskLinker->getNext();
					}
				}
			}
			return this;
		}



		CTaskProcessorCommunicator* CTaskProcessorThreadBase::communicateTaskAdditionalAllocation(CTask* task, CMemoryPool* additionalAllocatedMemoryPool) {
			processAdditionalTaskAllocation(task,additionalAllocatedMemoryPool);
			return this;
		}


		bool CTaskProcessorThreadBase::verifyContinueTaskProcessing(CTask* task) {
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


		bool CTaskProcessorThreadBase::handleEvents() {
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



		bool CTaskProcessorThreadBase::processTask(CTask* task) {
			mLastProcessedTask = task;
			task->clearNext();
			task->getTaskStatus()->setTaskPROCESSINGState();
			bool handleComp = mTaskHandleAlgo->handleTask(mTaskProcessorContext,task);
			return handleComp;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
