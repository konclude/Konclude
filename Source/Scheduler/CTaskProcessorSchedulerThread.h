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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSORSCHEDULERTHREAD_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSORSCHEDULERTHREAD_H

// Libraries includes
#include <QMutex>

// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskProcessorThreadBase.h"
#include "CTaskEventHandlerBasedScheduler.h"
#include "CTaskHandleContext.h"
#include "CTaskHandleMemoryAllocationManager.h"
#include "CTaskHandleMemoryPoolAllocationManager.h"
#include "CTaskHandleLimitedReserveMemoryPoolAllocationManager.h"
#include "CTaskHandleAlgorithm.h"
#include "CTaskProcessorContextBase.h"
#include "CTaskDistributionCompletorCommunicator.h"
#include "CConcreteTaskScheduleItem.h"
#include "CEventDistributionMemoryPoolReleaser.h"
#include "CTaskReserveArrayQueue.h"
#include "CTaskReserveMultiArrayQueueAdderController.h"

// Other includes
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CMemoryPoolProvider.h"

#include "Scheduler/Events/CTaskDispenseNotificationEvent.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Scheduler {

		using namespace Events;


		/*! 
		 *
		 *		\class		CTaskProcessorSchedulerThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessorSchedulerThread : public CTaskProcessorThreadBase, public CTaskEventHandlerBasedScheduler {
			// public methods
			public:
				//! Constructor
				CTaskProcessorSchedulerThread(CTaskHandleAlgorithm* taskHandleAlgo, CTaskEventHandlerBasedDistributionCompletor* distributorCompletor, CConsiderateMemoryPoolProvider* memoryPoolProvider = nullptr);

				//! Destructor
				virtual ~CTaskProcessorSchedulerThread();

				virtual CTaskReserveQueueConsumer* createTaskReserveQueueConsumer(CTaskEventHandlerBasedProcessor* taskProcessor);

				virtual CTaskSchedulerCommunicator* createSchedulerProcessorCommunication(CTaskEventHandlerBasedProcessor* taskProcessor);
				virtual CTaskEventHandlerBasedProcessor* installScheduler(CTaskEventHandlerBasedScheduler* scheduler);

				virtual bool requiresTaskDispenseNotification(cint64 &updateDispenseNotificationTag);

				CTaskReserveQueue* getTaskReserveQueue();
				CTaskReserveQueueAdder* createTaskReserveQueueAdder();

			// protected methods
			protected:
				virtual CTaskProcessorSchedulerThread* installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator);

				virtual bool processEvent(CEvent *event, CContext* handlerContext);

				virtual bool processDispenseNotification();
				virtual bool processRequestProcessTask();
				virtual bool processCriticalScheduleTask();
				virtual bool processCompleteTask(CTask* task);
				virtual bool processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool);
				virtual bool processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask);
				virtual bool processUpdateTaskStatus(CTask* task);
				virtual bool processUpdateTaskStatus(CTask* parentTask, CTask* updateTask);


				virtual bool processSchedulerRequestProcessTask(CTaskSchedulerCommunicator* communicator);
				virtual bool processSchedulerCriticalScheduleTask(CTaskSchedulerCommunicator* communicator);
				virtual bool processSchedulerResponseScheduleTask(CTask* task, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, bool schedulingTask);
				virtual bool processSchedulerScheduleTask(CTask* task, bool schedulingTask);
				virtual bool processTaskScheduling();
				virtual bool processSchedulingScheduleTasks();
				virtual bool processSchedulerDispenseNotification(CTaskSchedulerCommunicator* communicator);

				virtual bool requiresSchedulingTasks();

			// protected variables
			protected:
				CConsiderateMemoryPoolProvider* mMemoryPoolProvider;
				CDistributionMemoryPoolReleaser* mMemoryPoolReleaser;
				CTaskDistributionCompletorCommunicator* mDistributorCompletorCommunicator;

				// scheduler variables

				CConcreteTaskScheduleItem* mFreeSchedulerItemLinker;
				CConcreteTaskScheduleItem* mWaitingSchedulerItemLinker;
				CXLinker<CConcreteTaskSchedulerCommunicator*>* mSchedulerCommunicatorLinker;

				cint64 mRequestingTaskCount;
				cint64 mDeliveringWaitingCount;
				bool mRequiresTaskDispenseNotification;
				cint64 mSchedulerDispenseNotificationTag;
				bool mProcessorWithCriticalScheduleTask;

				QMutex mCreateCommunicatorSyncMutex;

				CTaskSchedulerCommunicator* mSchedulerCommunicator;
				CTaskReserveArrayQueue* mTaskReserveQueue;
				CTaskReserveMultiArrayQueueAdderController* mTaskReserveQueueController;

				// processor variables
				cint64 mDispenseNotificationTag;

				// for debugging only
				CTask* mLastScheduledTask;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSORSCHEDULERTHREAD_H
