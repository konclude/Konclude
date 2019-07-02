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

#include "CTaskReserveMultiArrayQueueConsumer.h"
#include "CTaskReserveMultiArrayQueue.h"


namespace Konclude {

	namespace Scheduler {



		CTaskReserveMultiArrayQueueConsumer::CTaskReserveMultiArrayQueueConsumer(CTaskReserveMultiArrayQueue* arrayQueue) : CLinkerBase<CTaskReserveMultiArrayQueueConsumer*,CTaskReserveMultiArrayQueueConsumer>(this), mArrayQueue(arrayQueue) {
			mTakenTask = 0;
			mRequestedTask = 0;
			mLastQueueLinker = nullptr;
			mQueueConsumerLinker = nullptr;
		}

		CTask* CTaskReserveMultiArrayQueueConsumer::getTask() {

			CXLinker<CTaskReserveArrayQueue*>* currentQueueLinker = mArrayQueue->getTaskReserveArrayQueueLinker();
			while (currentQueueLinker != mLastQueueLinker) {
				CXLinker<CTaskReserveArrayQueueConsumer*>* newConsumerLinker = new CXLinker<CTaskReserveArrayQueueConsumer*>();
				CTaskReserveArrayQueue* queue = currentQueueLinker->getData();
				CTaskReserveArrayQueueConsumer* queueConsumer = dynamic_cast<CTaskReserveArrayQueueConsumer*>(queue->createTaskReserveQueueConsumer());
				newConsumerLinker->initLinker(queueConsumer);

				mQueueConsumerLinker = newConsumerLinker->append(mQueueConsumerLinker);
			}
			CTask* task = nullptr;

			for (CXLinker<CTaskReserveArrayQueueConsumer*>* queueConsumerLinkerIt = mQueueConsumerLinker; !task && queueConsumerLinkerIt; queueConsumerLinkerIt = queueConsumerLinkerIt->getNext()) {
				CTaskReserveArrayQueueConsumer* queueConsumer = queueConsumerLinkerIt->getData();
				task = queueConsumer->getTask();
			}
			return task;
		}


		cint64 CTaskReserveMultiArrayQueueConsumer::getTakenTask() {
			return mTakenTask;
		}

		cint64 CTaskReserveMultiArrayQueueConsumer::getRequestedTask() {
			return mRequestedTask;
		}



	}; // end namespace Scheduler

}; // end namespace Konclude
