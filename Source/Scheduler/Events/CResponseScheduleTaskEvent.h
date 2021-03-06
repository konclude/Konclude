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

#ifndef KONCLUDE_SCHEDULER_EVENTS_CRESPONSESCHEDULETASKEVENT_H
#define KONCLUDE_SCHEDULER_EVENTS_CRESPONSESCHEDULETASKEVENT_H



// Libraries includes


// Namespace includes
#include "../SchedulerSettings.h"
#include "../CTaskScheduleItem.h"
#include "../CTask.h"


// Other includes
#include "Concurrent/CEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;

	namespace Scheduler {

		namespace Events {



			/*! 
			 *	\class		CResponseScheduleTaskEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CResponseScheduleTaskEvent : public CEvent {
				// public methods
				public:
					//! Constructor
					CResponseScheduleTaskEvent(CMemoryPool* memoryPool = nullptr);

					//! Destructor
					virtual ~CResponseScheduleTaskEvent();


					static const cint64 EVENTTYPEID = CRESPONSESCHEDULETASKEVENTTYPEID;


					CResponseScheduleTaskEvent* setTask(CTask* task);
					CTask* getTask();

					CTaskScheduleItem* getTaskScheduleItem();
					CResponseScheduleTaskEvent* setTaskScheduleItem(CTaskScheduleItem* taskScheduleItem);

					CTaskSchedulerCommunicator* getCommunicator();
					CResponseScheduleTaskEvent* setCommunicator(CTaskSchedulerCommunicator* communicator);
					
					CResponseScheduleTaskEvent* setSchedulingTask(bool schedulable);
					bool isSchedulingTask();

				// protected methods
				protected:

				// protected variables
				protected:
					CTask* mTask;
					CTaskScheduleItem* mScheduleItem;
					CTaskSchedulerCommunicator* mCommunicator;
					bool mSchedulingTask;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Events

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_EVENTS_CRESPONSESCHEDULETASKEVENT_H
