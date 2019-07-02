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
