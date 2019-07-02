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

#ifndef KONCLUDE_SCHEDULER_CTASKRESERVEARRAYQUEUE_H
#define KONCLUDE_SCHEDULER_CTASKRESERVEARRAYQUEUE_H

// Libraries includes
#include <QVector>
#include <QMutex>

// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskReserveQueue.h"
#include "CTaskReserveArrayQueueItem.h"
#include "CTaskReserveArrayQueueConsumer.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskReserveArrayQueue
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskReserveArrayQueue : public CTaskReserveQueue {
			// public methods
			public:
				//! Constructor
				CTaskReserveArrayQueue(cint64 vectorSize);


				bool tryAddTasks(CTask*& tasks);
				bool tryGetTask(CTask*& task);
				cint64 getTaskCount();


				CTaskReserveQueueConsumer* createTaskReserveQueueConsumer();


			// protected methods
			protected:
				inline cint64 getIndex(cint64 i) {
					return i % mVectorSize;
				}

			// protected variables
			protected:
				cint64 mNextItem;
				QVector<CTaskReserveArrayQueueItem> mTaskVector;
				cint64 mVectorSize;
				cint64 mLastItem;

				CTaskReserveArrayQueueConsumer* mConsumerLinker;
				QMutex mAddConsumerMutex;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKRESERVEARRAYQUEUE_H
