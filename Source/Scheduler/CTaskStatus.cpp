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

#include "CTaskStatus.h"


namespace Konclude {

	namespace Scheduler {



		CTaskStatus::CTaskStatus() {
			mTaskState = TSUNINITIALIZED;
			mCanceledFlag = false;
			mFinishedFlag = false;
			mReleaseable = true;
			mErrorFlag = false;
			mErrorCode = 0;
		}

		CTaskStatus::~CTaskStatus() {
		}


		CTaskStatus::TASKSTATE CTaskStatus::getTaskState() {
			return mTaskState;
		}

		CTaskStatus* CTaskStatus::setTaskState(TASKSTATE taskState) {
			mTaskState = taskState;
			return this;
		}

		CTaskStatus* CTaskStatus::setCanceled(bool canceled) {
			mCanceledFlag = canceled;
			return this;
		}

		CTaskStatus* CTaskStatus::setFinished(bool finished) {
			mFinishedFlag = finished;
			return this;
		}

		CTaskStatus* CTaskStatus::setError(bool error, cint64 errorCode) {
			mErrorFlag = error;
			mErrorCode = errorCode;
			return this;
		}

		CTaskStatus* CTaskStatus::setMemoryReleaseable(bool releaseable) {
			mReleaseable = releaseable;
			return this;
		}

		bool CTaskStatus::isFinished() {
			return mFinishedFlag;
		}

		bool CTaskStatus::isCanceled() {
			return mCanceledFlag;
		}

		bool CTaskStatus::isError() {
			return mErrorFlag;
		}

		bool CTaskStatus::isProcessable() {
			return !mCanceledFlag && !mFinishedFlag && !mErrorFlag;
		}


		cint64 CTaskStatus::getErrorCode() {
			return mErrorCode;
		}



		bool CTaskStatus::isTaskState(TASKSTATE taskState) {
			return mTaskState = taskState;
		}

		bool CTaskStatus::hasOneTaskState(TASKSTATE taskStateCompination) {
			return (mTaskState & taskStateCompination) != 0;
		}


		CTaskStatus* CTaskStatus::setTaskQUEUEDState() {
			mTaskState = TSQUEUED;
			return this;
		}

		CTaskStatus* CTaskStatus::setTaskPROCESSINGState() {
			mTaskState = TSPROCESSING;
			return this;
		}

		CTaskStatus* CTaskStatus::setTaskSCHEDULINGState() {
			mTaskState = TSSCHEDULING;
			return this;
		}

		CTaskStatus* CTaskStatus::setTaskCOMPLETEDState() {
			mTaskState = TSCOMPLETED;
			return this;
		}

		CTaskStatus* CTaskStatus::setTaskFINISHEDState() {
			mTaskState = TSFINISHED;
			return this;
		}

		bool CTaskStatus::isTaskStateQUEUED() {
			return mTaskState == TSQUEUED;
		}

		bool CTaskStatus::isTaskStatePROCESSING() {
			return mTaskState == TSPROCESSING;
		}

		bool CTaskStatus::isTaskStateSCHEDULING() {
			return mTaskState == TSSCHEDULING;
		}

		bool CTaskStatus::isTaskStateFINISHED() {
			return mTaskState == TSFINISHED;
		}

		bool CTaskStatus::isTaskStateCOMPLETED() {
			return mTaskState == TSCOMPLETED;
		}

		bool CTaskStatus::isTaskStateUNINITIALIZED() {
			return mTaskState == TSUNINITIALIZED;
		}


		bool CTaskStatus::isMemoryReleaseable() {
			return mReleaseable;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
