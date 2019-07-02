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

#ifndef KONCLUDE_SCHEDULER_CTASKPROCESSORCOMPLETORTHREAD_H
#define KONCLUDE_SCHEDULER_CTASKPROCESSORCOMPLETORTHREAD_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskProcessorThreadBase.h"
#include "CTaskEventHandlerBasedDistributionCompletor.h"
#include "CTaskHandleContext.h"
#include "CTaskHandleMemoryAllocationManager.h"
#include "CTaskHandleMemoryPoolAllocationManager.h"
#include "CTaskHandleLimitedReserveMemoryPoolAllocationManager.h"
#include "CTaskHandleAlgorithm.h"
#include "CTaskProcessorContextBase.h"
#include "CConcreteMemoryPoolDistributor.h"
#include "CTaskSchedulerCommunicator.h"
#include "CDirectDistributionMemoryPoolReleaser.h"

// Other includes
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CConsiderateMemoryPoolProvider.h"

#include "Scheduler/Events/CTaskStatusUpdatedEvent.h"
#include "Scheduler/Events/CAlignedUpdateTaskStatusEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Memory;

	namespace Scheduler {

		using namespace Events;


		/*! 
		 *
		 *		\class		CTaskProcessorCompletorThread
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskProcessorCompletorThread : public CTaskProcessorThreadBase, public CConcreteMemoryPoolDistributor, virtual public CTaskEventHandlerBasedDistributionCompletor {
			// public methods
			public:
				//! Constructor
				CTaskProcessorCompletorThread(CTaskHandleAlgorithm* taskHandleAlgo, CConsiderateMemoryPoolProvider* memoryPoolProvider = nullptr);

				//! Destructor
				virtual ~CTaskProcessorCompletorThread();

				virtual CTaskEventHandlerBasedProcessor* installScheduler(CTaskEventHandlerBasedScheduler* scheduler);

				virtual CDistributionMemoryPoolProvider* createDistributionMemoryPoolProvider(CDistributionMemoryPoolReleaser* distributionMemoryReleaser);

				virtual CConcreteMemoryPoolDistributor* distributeMemoryPools(CMemoryPool* memoryPools);

			// protected methods
			protected:
				virtual CTaskProcessorCompletorThread* installSchedulerCommunicator(CTaskSchedulerCommunicator* schedulerCommunicator);

				virtual bool processEvent(CEvent *event, CContext* handlerContext);

				virtual bool processDispenseNotification();
				virtual bool processRequestProcessTask();
				virtual bool processCriticalScheduleTask();
				virtual bool processCompleteTask(CTask* task);
				virtual bool processAdditionalTaskAllocation(CTask* task, CMemoryPool* memoryPool);
				virtual bool processResponseScheduleTask(CTask* task, CTaskScheduleItem* taskScheduleItem, bool schedulingTask);
				virtual bool processUpdateTaskStatus(CTask* task);
				virtual bool processUpdateTaskStatus(CTask* parentTask, CTask* updateTask);

				virtual bool processTaskStatusUpdated(CTask* parentTask, CTask* updatedTask, bool updateSuccess);


			// protected variables
			protected:
				CConsiderateMemoryPoolProvider* mMemoryPoolProvider;
				CDistributionMemoryPoolReleaser* mMemoryPoolReleaser;

				CTaskSchedulerCommunicator* mSchedulerCommunicator;

				cint64 mDispenseNotificationTag;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKPROCESSORCOMPLETORTHREAD_H
