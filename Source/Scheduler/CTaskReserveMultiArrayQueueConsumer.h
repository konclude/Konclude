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

#ifndef KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUECONSUMER_H
#define KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUECONSUMER_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskReserveQueueConsumer.h"

// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CTaskReserveMultiArrayQueueConsumer
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CTaskReserveMultiArrayQueueConsumer : public CTaskReserveQueueConsumer, public CLinkerBase<CTaskReserveMultiArrayQueueConsumer*,CTaskReserveMultiArrayQueueConsumer> {
			// public methods
			public:
				//! Constructor
				CTaskReserveMultiArrayQueueConsumer(CTaskReserveMultiArrayQueue* arrayQueue);

				CTask* getTask();

				cint64 getTakenTask();
				cint64 getRequestedTask();


			// protected methods
			protected:

			// protected variables
			protected:
				CTaskReserveMultiArrayQueue* mArrayQueue;
				CXLinker<CTaskReserveArrayQueue*>* mLastQueueLinker;
				CXLinker<CTaskReserveArrayQueueConsumer*>* mQueueConsumerLinker;
				cint64 mTakenTask;
				cint64 mRequestedTask;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CTASKRESERVEMULTIARRAYQUEUECONSUMER_H
