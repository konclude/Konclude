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

#ifndef KONCLUDE_SCHEDULER_CTASKSTATUS_H
#define KONCLUDE_SCHEDULER_CTASKSTATUS_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskStatus
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskStatus {
			// public methods
			public:
				//! Constructor
				CTaskStatus();

				//! Destructor
				virtual ~CTaskStatus();

				enum TASKSTATE { TSUNINITIALIZED, TSQUEUED, TSPROCESSING, TSSCHEDULING, TSFINISHED, TSCOMPLETED };

				TASKSTATE getTaskState();
				CTaskStatus* setTaskState(TASKSTATE taskState);

				CTaskStatus* setCanceled(bool canceled);
				CTaskStatus* setFinished(bool finished);
				CTaskStatus* setError(bool error, cint64 errorCode);
				CTaskStatus* setMemoryReleaseable(bool releaseable);

				bool isFinished();
				bool isCanceled();
				bool isProcessable();
				bool isError();
				bool isTaskState(TASKSTATE taskState);
				bool hasOneTaskState(TASKSTATE taskStateCompination);

				cint64 getErrorCode();

				CTaskStatus* setTaskQUEUEDState();
				CTaskStatus* setTaskPROCESSINGState();
				CTaskStatus* setTaskSCHEDULINGState();
				CTaskStatus* setTaskFINISHEDState();
				CTaskStatus* setTaskCOMPLETEDState();

				bool isTaskStateQUEUED();
				bool isTaskStatePROCESSING();
				bool isTaskStateFINISHED();
				bool isTaskStateSCHEDULING();
				bool isTaskStateCOMPLETED();
				bool isTaskStateUNINITIALIZED();

				bool isMemoryReleaseable();

			// protected methods
			protected:

			// protected variables
			protected:
				TASKSTATE mTaskState;
				bool mCanceledFlag;
				bool mFinishedFlag;
				bool mReleaseable;
				bool mErrorFlag;

				cint64 mErrorCode;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKSTATUS_H
