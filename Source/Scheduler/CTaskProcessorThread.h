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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSORTHREAD_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSORTHREAD_H

// Libraries includes
#include <QMutex>

// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskProcessorThreadBase.h"
#include "CTaskHandleContext.h"
#include "CTaskHandleMemoryAllocationManager.h"
#include "CTaskHandleMemoryPoolAllocationManager.h"
#include "CTaskHandleLimitedReserveMemoryPoolAllocationManager.h"
#include "CTaskHandleAlgorithm.h"
#include "CTaskProcessorContextBase.h"
#include "CTaskDistributionCompletorCommunicator.h"
#include "CConcreteTaskScheduleItem.h"
#include "CEventDistributionMemoryPoolReleaser.h"
#include "CTaskReserveQueueConsumer.h"

// Other includes
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"

#include "Scheduler/Events/CTaskDispenseNotificationEvent.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Scheduler {

		using namespace Events;


		/*! 
		 *
		 *		\class		CTaskProcessorThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessorThread : public CTaskProcessorThreadBase {
			// public methods
			public:
				//! Constructor
				CTaskProcessorThread(CTaskHandleAlgorithm* taskHandleAlgo, CTaskEventHandlerBasedDistributionCompletor* distributorCompletor, CConsiderateMemoryPoolProvider* memoryPoolProvider = nullptr);

				//! Destructor
				virtual ~CTaskProcessorThread();


				virtual CTaskEventHandlerBasedProcessor* installScheduler(CTaskEventHandlerBasedScheduler* scheduler);

			// protected methods
			protected:
				virtual CTaskProcessorThread* installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator);

				virtual bool processEvent(CEvent *event, CContext* handlerContext);

				virtual bool processDispenseNotification();
				virtual bool processRequestProcessTask();
				virtual bool processCriticalScheduleTask();
				virtual bool processCompleteTask(CTask* task);
				virtual bool processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool);
				virtual bool processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask);
				virtual bool processUpdateTaskStatus(CTask* task);
				virtual bool processUpdateTaskStatus(CTask* parentTask, CTask* updateTask);



			// protected variables
			protected:
				CConsiderateMemoryPoolProvider* mMemoryPoolProvider;
				CDistributionMemoryPoolReleaser* mMemoryPoolReleaser;


				// processor variables
				cint64 mDispenseNotificationTag;
				CTaskSchedulerCommunicator* mSchedulerCommunicator;

				CTaskDistributionCompletorCommunicator* mDistributorCompletorCommunicator;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSORTHREAD_H
