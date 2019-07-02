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

#ifndef KONCLUDE_SCHEDULER_CTASKSCHEDULERCOMMUNICATOR_H
#define KONCLUDE_SCHEDULER_CTASKSCHEDULERCOMMUNICATOR_H

// Library includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskEventHandlerBasedScheduler.h"
#include "CTaskEventHandlerBasedProcessor.h"
#include "CTaskEventCommunicator.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskSchedulerCommunicator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskSchedulerCommunicator {
			// public methods
			public:
				//! Constructor
				CTaskSchedulerCommunicator(CTaskEventHandlerBasedScheduler* taskScheduler, CTaskEventHandlerBasedProcessor* taskProcessor);

				//! Destructor
				virtual ~CTaskSchedulerCommunicator();


				// calls to task scheduler
				CTaskSchedulerCommunicator* communicateResponseScheduleTask(CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulingTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateRequestProcessTask(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateCriticalScheduleTask(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateTaskDispenseNotification(CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateScheduleUpdateTaskStatus(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);

				bool requiresTaskDispenseNotification(cint64 &dispenseNotificationTag);




				// calls to task processor
				CTaskSchedulerCommunicator* communicateProcessTask(CTask* task, bool schedulingTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateRequestScheduleTask(CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskSchedulerCommunicator* communicateUpdateTaskStatus(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);

				bool canDispenseProcessingTasks();
				bool requiresProcessingTasks();
				cint64 countDispensableProcessingTasks();
				cint64 countScheduleableProcessingTasks();
				bool requiresSchedulingTasks();
				bool hasTaskOwner(CTaskOwner* taskOwner);

				cint64 getRecievedTasks();


			// protected methods
			protected:

			// protected variables
			protected:
				CTaskEventCommunicator mSchedulerTaskEventCommunicator;
				CTaskEventCommunicator mProcessorTaskEventCommunicator;
				CTaskEventHandlerBasedScheduler* mTaskScheduler;
				CTaskEventHandlerBasedProcessor* mTaskProcessor;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKSCHEDULERCOMMUNICATOR_H
