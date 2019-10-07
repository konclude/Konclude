/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_CONCURRENT_CEVENT_H
#define KONCLUDE_CONCURRENT_CEVENT_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventLinker.h"


// Other includes

#include "Utilities/Memory/CMemoryPool.h"
#include "Utilities/Memory/CMemoryPoolContainer.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Concurrent {

		/*! 
		 *
		 *		\class		CEvent
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEvent : public CEventLinker, public CMemoryPoolContainer {
			// public methods
			public:
				//! Constructor
				CEvent(cint64 eventTypeID = 0, CMemoryPool* memoryPool = 0);

				//! Destructor
				~CEvent();

				double getEventChannelPriority();
				CEvent* setEventChannelPriority(double eventPriority);

				cint64 getEventTypeID();
				CEvent* setEventTypeID(cint64 eventTypeID);


			// protected methods
			protected:

			// protected variables
			protected:
				double mEventPriority;
				cint64 mEventTypeID;

			// private methods
			private:

			// private variables
			private:
		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENT_H
