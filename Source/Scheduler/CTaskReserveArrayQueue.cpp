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

#include "CTaskReserveArrayQueue.h"


namespace Konclude {

	namespace Scheduler {



		CTaskReserveArrayQueue::CTaskReserveArrayQueue(cint64 vectorSize) : mTaskVector(vectorSize) {
			mVectorSize = vectorSize;

			for (cint64 i = 0; i < mVectorSize; ++i) {
				cint64 ind1 = getIndex(i);
				cint64 ind2 = getIndex(i+1);

				CTaskReserveArrayQueueItem* item1 = &mTaskVector[ind1];
				CTaskReserveArrayQueueItem* item2 = &mTaskVector[ind2];

				item1->setNext(item2);
			}

			mNextItem = 0;
			mLastItem = 0;

			mConsumerLinker = nullptr;
		}


		bool CTaskReserveArrayQueue::tryAddTasks(CTask*& tasks) {
			bool taskAdded = false;
			bool canAddMoreTasks = true;
			while (tasks && canAddMoreTasks) {
				if (mLastItem - mNextItem < mVectorSize) {
					CTask* tmpTask = tasks;
					tasks = tasks->getNext();
					tmpTask->clearNext();

					CTaskReserveArrayQueueItem* item = &mTaskVector[getIndex(mLastItem)];
					if (!item->trySetTask(tmpTask)) {
						tasks = tmpTask->append(tasks);
					} else {
						taskAdded = true;
					}
					++mLastItem;
				} else {
					canAddMoreTasks = false;
				}
			}
			return taskAdded;
		}


		cint64 CTaskReserveArrayQueue::getTaskCount() {
			return mLastItem - mNextItem;
		}


		bool CTaskReserveArrayQueue::tryGetTask(CTask*& task) {
			task = nullptr;
			cint64 nextItem = mNextItem;
			if (nextItem == mLastItem) {
				return false;
			}
			cint64 index = getIndex(nextItem++);
			CTaskReserveArrayQueueItem* item = &mTaskVector[index];
			task = item->tryGetTask();
			mNextItem = nextItem;
			return task != nullptr;
		}


		CTaskReserveQueueConsumer* CTaskReserveArrayQueue::createTaskReserveQueueConsumer() {
			CTaskReserveArrayQueueConsumer* consumer = new CTaskReserveArrayQueueConsumer(this);
			mAddConsumerMutex.lock();
			mConsumerLinker = consumer->append(mConsumerLinker);
			mAddConsumerMutex.unlock();
			return consumer;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
