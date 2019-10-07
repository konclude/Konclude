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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSORTHREADBASE_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSORTHREADBASE_H

// Libraries includes
#include <QTime>

// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskEventHandlerBasedProcessor.h"
#include "CTaskHandleContext.h"
#include "CTaskHandleMemoryAllocationManager.h"
#include "CTaskHandleAlgorithm.h"
#include "CTaskProcessorContextBase.h"
#include "CTaskStatusPropagator.h"
#include "CTaskCallbackExecuter.h"
#include "CTaskProcessingStatistics.h"
#include "CTaskReserveQueueConsumer.h"
#include "CTask.h"

// Other includes
#include "Scheduler/Events/CSendTaskProcessEvent.h"
#include "Scheduler/Events/CSendTaskCompleteEvent.h"
#include "Scheduler/Events/CRequestScheduleTaskEvent.h"
#include "Scheduler/Events/CResponseScheduleTaskEvent.h"
#include "Scheduler/Events/CTaskAdditionalAllocationEvent.h"
#include "Scheduler/Events/CUpdateTaskStatusEvent.h"

#include "Concurrent/Events/CHandleEventsEvent.h"
#include "Concurrent/CQueuedLinkedEventHandler.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;
	using namespace Utilities::Memory;

	namespace Scheduler {

		using namespace Events;

//#define KONCLUDE_SCHEDULER_DEBUG_TASK_MEMORY_RELEASE




		/*! 
		 *
		 *		\class		CTaskProcessorThreadBase
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessorThreadBase : public CTaskEventHandlerBasedProcessor, public CThread, public CThreadActivator, public CTaskProcessorCommunicator, public CEventProcessor {
			// public methods
			public:
				//! Constructor
				CTaskProcessorThreadBase(CTaskHandleAlgorithm* taskHandleAlgo, const QString &threadStringName = "TaskProcesserThread");

				//! Destructor
				virtual ~CTaskProcessorThreadBase();

				CTaskProcessingStatistics* getTaskProcessingStatistics();

				virtual CThreadActivator* signalizeEvent();
				virtual CEventHandler* getEventHandler();

				virtual CTaskProcessorThreadBase* startProcessing();
				virtual CTaskProcessorThreadBase* stopProcessing();

				cint64 getStatisticTaskProcessed();
				cint64 getStatisticTaskCompleted();
				cint64 getStatisticEventProcessed();
				cint64 getStatisticComputionTime();
				cint64 getStatisticBlockingTime();

				cint64 getTaskProcessingCount();

				virtual bool canDispenseProcessingTasks();
				virtual bool requiresProcessingTasks();
				virtual bool requiresSchedulingTasks();
				virtual cint64 countDispensableProcessingTasks();
				virtual cint64 countScheduleableProcessingTasks();
				virtual cint64 getRecievedTasks();

				CTaskProcessorThreadBase* installCallbackExecuter(CTaskCallbackExecuter* callbackExecuter);
				CTaskProcessorThreadBase* installStatusPropagator(CTaskStatusPropagator* statusPropagator);

				virtual CTaskHandleAlgorithm* getTaskHandleAlgorithm();

				bool isProcessing();
				bool isBlocked();


			// protected methods
			protected:
				virtual bool processControlEvents(QEvent::Type type, CControlEvent *event);

				virtual bool processEvent(CEvent *event, CContext* handlerContext);

				virtual bool processingLoop();

				void organizeDispenseTasks(bool forceUpdate);

				virtual CTaskProcessorCommunicator* communicateTaskComplete(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskError(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskCreation(CTask* newTask);
				virtual CTaskProcessorCommunicator* communicateTaskAdditionalAllocation(CTask* task, CMemoryPool* additionalAllocatedMemoryPool);
				virtual CTaskProcessorCommunicator* communicateTaskStatusUpdate(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskRelevant(CTask* task);

				virtual bool verifyContinueTaskProcessing(CTask* task);

				bool processTask(CTask* task);
				bool addProcessingTask(CTask* task);
				bool handleEvents();

				virtual bool processDispenseNotification() = 0;
				virtual bool processRequestProcessTask() = 0;
				virtual bool processCriticalScheduleTask() = 0;
				virtual bool processCompleteTask(CTask* task) = 0;
				virtual bool processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool) = 0;
				virtual bool processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask) = 0;
				virtual bool processUpdateTaskStatus(CTask* task) = 0;
				virtual bool processUpdateTaskStatus(CTask* parentTask, CTask* updateTask) = 0;



			// protected variables
			protected:
				CTask* mTaskSchedulingQueue;
				CTask* mTaskProcessingQueue;
				CTask* mTaskCompletionQueue;
				CTaskProcessorContext *mTaskProcessorContext;
				CTaskHandleMemoryAllocationManager *mMemoryAllocator;
				CTaskHandleAlgorithm* mTaskHandleAlgo;
				CTaskStatusPropagator* mTaskStatusPropagator;
				CTaskCallbackExecuter* mCallbackExecuter;
				CQueuedLinkedEventHandler* mEventHandler;


				QSemaphore mProcessingWakeUpSemaphore;
				CXLinker<CEventHandler*>* mEventHandlerLinker;

				CTaskReserveQueueConsumer* mTaskReserveQueueConsumer;


				bool mProcessingStopped;

				bool mThreadBlocked;

				bool mEventSignalized;
				bool mProcessingBlocked;
				cint64 mLastProcessingStartRequestTag;
				cint64 mLastProcessingStartedTag;
				cint64 mTaskProcessingCount;
				cint64 mTaskParentRelevantCount;
				cint64 mTaskSchedulingCount;
				cint64 mTaskReserveQueueCount;
				cint64 mCriticalSchedulingCount;
				cint64 mCriticalSchedulingThresholdCount;
				cint64 mRecievedTasks;
				bool mCriticalSchedulingNotified;

				bool mProcessTaskRequesting;

				bool mMinimalPriorityTaskInvalidated;
				double mMinTaskPriority;
				CTask* mMinPriorityTask;

				bool mConfScheduleTasksDistribution;
				bool mConfProcessingTasksDistribution;
				bool mConfUseReserveTasksQueue;

				bool mConfProcessReserveTasks;
				bool mConfProcessSchedulingTasks;

				CTaskProcessingStatistics mStats;
				cint64 mStatComputionTime;
				cint64 mStatBlockingTime;

#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
				QTime mComputionTimer;
				QTime mBlockingTimer;
#endif

				// for debugging only
				CTask* mLastProcessedTask;
				CTask* mLastDispendedTask;
				CEvent* mLastProcessedEvent;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSORTHREADBASE_H
