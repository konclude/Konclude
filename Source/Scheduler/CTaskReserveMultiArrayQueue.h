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

#ifndef KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUE_H
#define KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUE_H

// Libraries includes
#include <QVector>
#include <QMutex>

// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskReserveQueue.h"
#include "CTaskReserveArrayQueue.h"
#include "CTaskReserveMultiArrayQueueConsumer.h"

// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskReserveMultiArrayQueue
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskReserveMultiArrayQueue : public CTaskReserveQueue {
			// public methods
			public:
				//! Constructor
				CTaskReserveMultiArrayQueue(CTaskReserveArrayQueue* mainQueue);


				bool tryAddTasks(CTask*& tasks);
				bool tryGetTask(CTask*& task);
				cint64 getTaskCount();


				CTaskReserveQueueConsumer* createTaskReserveQueueConsumer();

				CTaskReserveMultiArrayQueue* addTaskReserveArrayQueue(CTaskReserveArrayQueue* queue);
				CXLinker<CTaskReserveArrayQueue*>* getTaskReserveArrayQueueLinker();
				cint64 getTaskReserveArrayQueueCount();

			// protected methods
			protected:

			// protected variables
			protected:
				CTaskReserveArrayQueue* mMainQueue;
				CXLinker<CTaskReserveArrayQueue*>* mQueueLinker;
				QMutex mAddQueueMutex;
				cint64 mQueuesCount;

				CTaskReserveMultiArrayQueueConsumer* mConsumerLinker;
				QMutex mAddConsumerMutex;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUE_H
