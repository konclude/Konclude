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

#include "CTaskReserveArrayQueueConsumer.h"
#include "CTaskReserveArrayQueue.h"


namespace Konclude {

	namespace Scheduler {



		CTaskReserveArrayQueueConsumer::CTaskReserveArrayQueueConsumer(CTaskReserveArrayQueue* arrayQueue) : CLinkerBase<CTaskReserveArrayQueueConsumer*,CTaskReserveArrayQueueConsumer>(this), mArrayQueue(arrayQueue) {
			mTakenTask = 0;
			mRequestedTask = 0;
		}

		CTask* CTaskReserveArrayQueueConsumer::getTask() {
			CTask* task = nullptr;
			bool moreTasksAvailable = true;
			task = nullptr;
			while (moreTasksAvailable && !task) {
				++mRequestedTask;
				moreTasksAvailable = mArrayQueue->tryGetTask(task);
			}
			if (task != nullptr) {
				++mTakenTask;
			}
			return task;
		}


		cint64 CTaskReserveArrayQueueConsumer::getTakenTask() {
			return mTakenTask;
		}

		cint64 CTaskReserveArrayQueueConsumer::getRequestedTask() {
			return mRequestedTask;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
