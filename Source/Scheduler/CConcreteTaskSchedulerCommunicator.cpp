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

#include "CConcreteTaskSchedulerCommunicator.h"


namespace Konclude {

	namespace Scheduler {


		CConcreteTaskSchedulerCommunicator::CConcreteTaskSchedulerCommunicator(CTaskEventHandlerBasedScheduler* taskScheduler, CTaskEventHandlerBasedProcessor* taskProcessor) : CTaskSchedulerCommunicator(taskScheduler,taskProcessor) {
			mRequestCount = 0;
			mSendedSchedulingTaskCount = 0;
		}

		CConcreteTaskSchedulerCommunicator::~CConcreteTaskSchedulerCommunicator() {
		}


		CConcreteTaskSchedulerCommunicator* CConcreteTaskSchedulerCommunicator::incRequestProcessingTask(cint64 incCount) {
			mRequestCount += incCount;
			return this;
		}

		CConcreteTaskSchedulerCommunicator* CConcreteTaskSchedulerCommunicator::decRequestProcessingTask(cint64 decCount) {
			mRequestCount -= decCount;
			return this;
		}

		cint64 CConcreteTaskSchedulerCommunicator::getRequestUpdatedDispensableProcessingAndSchedulingTasksCount() {
			return countDispensableProcessingTasks()-mRequestCount;
		}

		cint64 CConcreteTaskSchedulerCommunicator::getRequestUpdatedDispensableSchedulingTasksCount() {
			return countScheduleableProcessingTasks()-mRequestCount;
		}

		cint64 CConcreteTaskSchedulerCommunicator::getRequestUpdatedRemainingSchedulingTasksCount() {
			cint64 transitTasksCount = mSendedSchedulingTaskCount - getRecievedTasks();
			return countScheduleableProcessingTasks()-mRequestCount+transitTasksCount;
		}


		CConcreteTaskSchedulerCommunicator* CConcreteTaskSchedulerCommunicator::incSendedTasks(cint64 incCount) {
			mSendedSchedulingTaskCount += incCount;
			return this;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude

