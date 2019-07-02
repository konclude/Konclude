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

#ifndef KONCLUDE_SCHEDULER_CCONCRETETASKSCHEDULEITEM_H
#define KONCLUDE_SCHEDULER_CCONCRETETASKSCHEDULEITEM_H

// Libraries includes


// Namespace includes
#include "SchedulerSettings.h"
#include "CTaskScheduleItem.h"
#include "CConcreteTaskSchedulerCommunicator.h"

// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Scheduler {


		/*! 
		 *
		 *		\class		CConcreteTaskScheduleItem
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CConcreteTaskScheduleItem : public CTaskScheduleItem, public CLinkerBase<CConcreteTaskScheduleItem*,CConcreteTaskScheduleItem> {
			// public methods
			public:
				//! Constructor
				CConcreteTaskScheduleItem();

				//! Destructor
				virtual ~CConcreteTaskScheduleItem();

				CConcreteTaskScheduleItem* initTaskScheduleItem(CConcreteTaskSchedulerCommunicator* communicator, CConcreteTaskScheduleItem* nextSchedulerItem = nullptr);


				CConcreteTaskSchedulerCommunicator* getCommunicator();
				CConcreteTaskScheduleItem* setCommunicator(CConcreteTaskSchedulerCommunicator* communicator);

			// protected methods
			protected:

			// protected variables
			protected:
				CConcreteTaskSchedulerCommunicator* mCommunicator;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_CCONCRETETASKSCHEDULEITEM_H
