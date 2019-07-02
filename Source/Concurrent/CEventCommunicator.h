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

#ifndef KONCLUDE_CONCURRENT_CEVENTCOMMUNICATOR_H
#define KONCLUDE_CONCURRENT_CEVENTCOMMUNICATOR_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventChannel.h"
#include "CSyncedEventChannel.h"
#include "CSyncedFreeEventChannel.h"
#include "CEventHandler.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CEventCommunicator
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEventCommunicator {
			// public methods
			public:
				//! Constructor
				CEventCommunicator(CEventHandler* eventHandler);

				//! Destructor
				virtual ~CEventCommunicator();


				bool checkPostEvent(CEvent* event);
				CEventCommunicator* postEvent(CEvent* event);

				static bool postEvent(CEventHandler* eventHandler, CEvent* event);
				static bool checkPostEvent(CEventHandler* eventHandler, CEvent* event);

			// protected methods
			protected:

			// protected variables
			protected:
				CEventHandler* mEventHandler;
				CEventChannel* mEventChannel;

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENTCOMMUNICATOR_H
