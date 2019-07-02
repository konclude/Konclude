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

#ifndef KONCLUDE_SCHEDULER_EVENTS_CDISTRIBUTEMEMORYPOOLEVENT_H
#define KONCLUDE_SCHEDULER_EVENTS_CDISTRIBUTEMEMORYPOOLEVENT_H



// Libraries includes


// Namespace includes
#include "../SchedulerSettings.h"


// Other includes
#include "Concurrent/CEvent.h"

#include "Utilities/Memory/CMemoryPool.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;
	using namespace Utilities::Memory;

	namespace Scheduler {

		namespace Events {



			/*! 
			 *	\class		CDistributeMemoryPoolEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CDistributeMemoryPoolEvent : public CEvent {
				// public methods
				public:
					//! Constructor
					CDistributeMemoryPoolEvent(CMemoryPool* memoryPool = nullptr);

					//! Destructor
					virtual ~CDistributeMemoryPoolEvent();


					static const cint64 EVENTTYPEID = CDISTRIBUTEMEMORYPOOLEVENTTYPEID;

					CMemoryPool* getMemoryPools();
					CDistributeMemoryPoolEvent* setMemoryPools(CMemoryPool* memoryPools);


				// protected methods
				protected:

				// protected variables
				protected:
					CMemoryPool* mMemoryPools;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Events

	}; // end namespace Scheduler

}; // end namespace Konclude

#endif // KONCLUDE_SCHEDULER_EVENTS_CDISTRIBUTEMEMORYPOOLEVENT_H
