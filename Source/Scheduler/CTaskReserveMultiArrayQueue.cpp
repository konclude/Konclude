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

#include "CTaskReserveMultiArrayQueue.h"


namespace Konclude {

	namespace Scheduler {



		CTaskReserveMultiArrayQueue::CTaskReserveMultiArrayQueue(CTaskReserveArrayQueue* mainQueue) {
			mMainQueue = nullptr;
			mQueueLinker = nullptr;
			mConsumerLinker = nullptr;
			mQueuesCount = 0;
			if (!mainQueue) {
				mainQueue = new CTaskReserveArrayQueue(10000);
			}
			addTaskReserveArrayQueue(mainQueue);
		}


		CTaskReserveMultiArrayQueue* CTaskReserveMultiArrayQueue::addTaskReserveArrayQueue(CTaskReserveArrayQueue* queue) {
			CXLinker<CTaskReserveArrayQueue*>* queueLinker = new CXLinker<CTaskReserveArrayQueue*>();
			queueLinker->initLinker(queue);
			mAddConsumerMutex.lock();
			mQueueLinker = queueLinker->append(mQueueLinker);
			++mQueuesCount;
			mAddConsumerMutex.unlock();
			return this;
		}


		bool CTaskReserveMultiArrayQueue::tryAddTasks(CTask*& tasks) {
			bool taskAdded = mMainQueue->tryAddTasks(tasks);
			return taskAdded;
		}


		cint64 CTaskReserveMultiArrayQueue::getTaskCount() {
			cint64 taskCount = 0;
			for (CXLinker<CTaskReserveArrayQueue*>* queueLinkerIt = mQueueLinker; queueLinkerIt; queueLinkerIt = queueLinkerIt->getNext()) {
				CTaskReserveArrayQueue* queue = queueLinkerIt->getData();
				taskCount += queue->getTaskCount();
			}
			return taskCount;
		}


		bool CTaskReserveMultiArrayQueue::tryGetTask(CTask*& task) {
			for (CXLinker<CTaskReserveArrayQueue*>* queueLinkerIt = mQueueLinker; queueLinkerIt; queueLinkerIt = queueLinkerIt->getNext()) {
				CTaskReserveArrayQueue* queue = queueLinkerIt->getData();
				if (queue->tryGetTask(task)) {
					return true;
				}
			}
			return false;
		}


		CTaskReserveQueueConsumer* CTaskReserveMultiArrayQueue::createTaskReserveQueueConsumer() {
			CTaskReserveMultiArrayQueueConsumer* consumer = new CTaskReserveMultiArrayQueueConsumer(this);
			mAddConsumerMutex.lock();
			mConsumerLinker = consumer->append(mConsumerLinker);
			mAddConsumerMutex.unlock();
			return consumer;
		}

		CXLinker<CTaskReserveArrayQueue*>* CTaskReserveMultiArrayQueue::getTaskReserveArrayQueueLinker() {
			return mQueueLinker;
		}


		cint64 CTaskReserveMultiArrayQueue::getTaskReserveArrayQueueCount() {
			return mQueuesCount;
		}


	}; // end namespace Scheduler

}; // end namespace Konclude
