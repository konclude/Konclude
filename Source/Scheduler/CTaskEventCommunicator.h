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

#ifndef KONCLUDE_SCHEDULER_CTASKEVENTCOMMUNICATOR_H
#define KONCLUDE_SCHEDULER_CTASKEVENTCOMMUNICATOR_H

// Library includes


// Namespace includes
#include "SchedulerSettings.h"


// Other includes
#include "Concurrent/CEventCommunicator.h"
#include "Concurrent/CEventAllocator.h"

#include "Scheduler/Events/CSendTaskProcessEvent.h"
#include "Scheduler/Events/CSendTaskCompleteEvent.h"
#include "Scheduler/Events/CSendTaskScheduleEvent.h"
#include "Scheduler/Events/CRequestScheduleTaskEvent.h"
#include "Scheduler/Events/CResponseScheduleTaskEvent.h"
#include "Scheduler/Events/CRequestProcessTaskEvent.h"
#include "Scheduler/Events/CTaskAdditionalAllocationEvent.h"
#include "Scheduler/Events/CDistributeMemoryPoolEvent.h"
#include "Scheduler/Events/CTaskDispenseNotificationEvent.h"
#include "Scheduler/Events/CUpdateTaskStatusEvent.h"
#include "Scheduler/Events/CTaskStatusUpdatedEvent.h"
#include "Scheduler/Events/CAlignedUpdateTaskStatusEvent.h"
#include "Scheduler/Events/CCriticalScheduleTaskEvent.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent;

	namespace Scheduler {

		using namespace Scheduler::Events;


		/*! 
		 *
		 *		\class		CTaskEventCommunicator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskEventCommunicator : public CEventCommunicator {
			// public methods
			public:
				//! Constructor
				CTaskEventCommunicator(CEventHandler* eventHandler);

				//! Destructor
				virtual ~CTaskEventCommunicator();

				CTaskEventCommunicator* postSendTaskProcessEvent(CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postSendTaskCompleteEvent(CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postSendTaskScheduleEvent(CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postRequestScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postResponseScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postRequestProcessTaskEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postCriticalScheduleTaskEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postAdditionalAllocationTaskEvent(CTask* task, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postDistributeMemoryPoolsEvent(CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postDistributeMemoryPoolsEvent(CMemoryPool* memoryPools);
				CTaskEventCommunicator* postTaskDispenseNotificationEvent(CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postUpdateTaskStatusEvent(CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postAlignedUpdateTaskStatusEvent(CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				CTaskEventCommunicator* postTaskStatusUpdatedEvent(CTask* parentTask, CTask* updatedTask, bool updateSuccess, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);

				static bool postSendTaskProcessEvent(CEventHandler* eventHandler, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postSendTaskCompleteEvent(CEventHandler* eventHandler, CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postSendTaskScheduleEvent(CEventHandler* eventHandler, CTask* task, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postRequestScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postResponseScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CTaskScheduleItem* taskScheduleItem, CTask* task, bool schedulerTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postRequestProcessTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postCriticalScheduleTaskEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postAdditionalAllocationTaskEvent(CEventHandler* eventHandler, CTask* task, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postDistributeMemoryPoolsEvent(CEventHandler* eventHandler, CMemoryPool* memoryPools, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postTaskDispenseNotificationEvent(CEventHandler* eventHandler, CTaskSchedulerCommunicator* communicator, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postUpdateTaskStatusEvent(CEventHandler* eventHandler, CTask* parentTask, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postAlignedUpdateTaskStatusEvent(CEventHandler* eventHandler, CTask* updateTask, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);
				static bool postTaskStatusUpdatedEvent(CEventHandler* eventHandler, CTask* parentTask, CTask* updatedTask, bool updateSuccess, CMemoryTemporaryAllocationManager* memoryTmpAllocatorManager);

			// protected methods
			protected:

			// protected variables
			protected:
				CMemoryPool* mTmpStoredMemoryPool;
				cint64 mPostedEventCount;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKEVENTCOMMUNICATOR_H
