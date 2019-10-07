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

#ifndef KONCLUDE_SCHEDULER_CSINGLETHREADTASKPROCESSORUNIT_H
#define KONCLUDE_SCHEDULER_CSINGLETHREADTASKPROCESSORUNIT_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskEventHandlerBasedProcessor.h"
#include "CTaskEventHandlerBasedCompletor.h"
#include "CTaskEventHandlerBasedScheduler.h"
#include "CTaskHandleContext.h"
#include "CTaskHandleMemoryAllocationManager.h"
#include "CTaskHandleMemoryPoolAllocationManager.h"
#include "CTaskHandleLimitedReserveMemoryPoolAllocationManager.h"
#include "CTaskHandleAlgorithm.h"
#include "CTaskProcessorContextBase.h"
#include "CTaskStatusPropagator.h"
#include "CTaskCallbackExecuter.h"
#include "CTaskProcessingStatistics.h"
#include "CTask.h"

// Other includes
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"

#include "Scheduler/Events/CSendTaskProcessEvent.h"
#include "Scheduler/Events/CSendTaskCompleteEvent.h"
#include "Scheduler/Events/CSendTaskScheduleEvent.h"
#include "Scheduler/Events/CRequestScheduleTaskEvent.h"
#include "Scheduler/Events/CResponseScheduleTaskEvent.h"
#include "Scheduler/Events/CTaskAdditionalAllocationEvent.h"
#include "Scheduler/Events/CRequestProcessTaskEvent.h"

#include "Concurrent/Events/CHandleEventsEvent.h"
#include "Concurrent/CQueuedLinkedEventHandler.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;
	using namespace Utilities::Memory;

	namespace Scheduler {

		using namespace Events;


		/*! 
		 *
		 *		\class		CSingleThreadTaskProcessorUnit
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSingleThreadTaskProcessorUnit : virtual public CTaskEventHandlerBasedProcessor, virtual public CTaskEventHandlerBasedCompletor, virtual public CTaskEventHandlerBasedScheduler, public CThread, public CThreadActivator, public CTaskProcessorCommunicator, public CEventProcessor {
			// public methods
			public:
				//! Constructor
				CSingleThreadTaskProcessorUnit(CTaskHandleAlgorithm* taskHandleAlgo, CConsiderateMemoryPoolProvider* memoryPoolProvider = nullptr);

				//! Destructor
				virtual ~CSingleThreadTaskProcessorUnit();

				virtual CTaskEventHandlerBasedProcessor* installScheduler(CTaskEventHandlerBasedScheduler* scheduler);
				virtual CTaskSchedulerCommunicator* createSchedulerProcessorCommunication(CTaskEventHandlerBasedProcessor* taskProcessor);
				virtual CTaskReserveQueueConsumer* createTaskReserveQueueConsumer(CTaskEventHandlerBasedProcessor* taskProcessor);

				virtual CThreadActivator* signalizeEvent();
				virtual CEventHandler* getEventHandler();

				virtual CSingleThreadTaskProcessorUnit* startProcessing();
				virtual CSingleThreadTaskProcessorUnit* stopProcessing();

				virtual bool processEvent(CEvent *event, CContext* handlerContext);

				CTaskProcessingStatistics* getTaskProcessingStatistics();
				cint64 getStatisticComputionTime();
				cint64 getStatisticBlockingTime();

				cint64 getTaskProcessingCount();
				virtual cint64 getRecievedTasks();

				virtual bool canDispenseProcessingTasks();
				virtual bool requiresProcessingTasks();
				virtual bool requiresSchedulingTasks();
				virtual cint64 countDispensableProcessingTasks();
				virtual bool requiresTaskDispenseNotification(cint64 &updateDispenseNotificationTag);
				virtual cint64 countScheduleableProcessingTasks();

				CSingleThreadTaskProcessorUnit* installCallbackExecuter(CTaskCallbackExecuter* callbackExecuter);
				CSingleThreadTaskProcessorUnit* installStatusPropagator(CTaskStatusPropagator* statusPropagator);

				virtual CTaskHandleAlgorithm* getTaskHandleAlgorithm();


			// protected methods
			protected:
				virtual bool processControlEvents(QEvent::Type type, CControlEvent *event);

				bool addProcessingTask(CTask* task);


				virtual bool processingLoop();

				virtual CTaskProcessorCommunicator* communicateTaskComplete(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskError(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskCreation(CTask* newTask);
				virtual CTaskProcessorCommunicator* communicateTaskAdditionalAllocation(CTask* task, CMemoryPool* additionalAllocatedMemoryPool);
				virtual CTaskProcessorCommunicator* communicateTaskStatusUpdate(CTask* task);
				virtual CTaskProcessorCommunicator* communicateTaskRelevant(CTask* task);

				virtual bool verifyContinueTaskProcessing(CTask* task);


				virtual cint64 completeTask(CTask* task);
				virtual bool updateTaskStatus(CTask* task);
				virtual bool updateTaskStatusDown(CTask* task, cint64 depth);
				virtual bool processTask(CTask* task);

				virtual bool handleEvents();

				cint64 countProcessingTasksMemoryPools();
				cint64 countProcessedOpenTasksMemoryPools();
				cint64 closeOpenTasksMemoryPools();


			// protected variables
			protected:
				CTask* mTaskSchedulingQueue;
				CTask* mTaskProcessingQueue;
				CTask* mTaskCompletionQueue;
				CTaskProcessorContext *mTaskProcessorContext;
				CTaskHandleMemoryAllocationManager *mMemoryAllocator;
				CTaskHandleAlgorithm* mTaskHandleAlgo;
				CQueuedLinkedEventHandler* mEventHandler;
				CTaskStatusPropagator* mTaskStatusPropagator;
				CTaskCallbackExecuter* mCallbackExecuter;
				CConsiderateMemoryPoolProvider* mMemoryPoolProvider;


				QSemaphore mProcessingWakeUpSemaphore;
				CXLinker<CEventHandler*>* mEventHandlerLinker;

				bool mProcessingStopped;

				bool mThreadBlocked;


				bool mEventSignalized;
				bool mProcessingBlocked;
				cint64 mLastProcessingStartRequestTag;
				cint64 mLastProcessingStartedTag;
				cint64 mTaskProcessingCount;
				cint64 mTaskSchedulingCount;

				CTaskProcessingStatistics mStats;
				cint64 mStatComputionTime;
				cint64 mStatBlockingTime;
				cint64 mStatRecievedScheduleTasks;
				cint64 mStatRemovedTasks;

#ifdef KONCLUDE_SCHEDULER_TASK_THREADS_TIME_STATISTICS
				QTime mComputionTimer;
				QTime mBlockingTimer;
#endif


				CTask* mDebugLastCompletedTask;
				CTask* mDebugLastProcessedTask;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CSINGLETHREADTASKPROCESSORUNIT_H
