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

#ifndef KONCLUDE_CONCURRENT_CEVENTQUEUE_H
#define KONCLUDE_CONCURRENT_CEVENTQUEUE_H

// Library includes


// Namespace includes
#include "ConcurrentSettings.h"
#include "CEventChannel.h"
#include "CSyncedEventChannel.h"
#include "CSyncedFreeEventChannel.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *
		 *		\class		CEventQueue
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CEventQueue {
			// public methods
			public:
				//! Constructor
				CEventQueue();

				//! Destructor
				virtual ~CEventQueue();

				virtual CSyncedEventChannel* getDefaultSyncedEventChannel(double channelPriority = 0.) = 0;
				virtual CSyncedFreeEventChannel* createSyncedFreeEventChannel(double channelPriority = 0.) = 0;
				
				virtual bool hasEvents(double minEventPriority = 0.) = 0;

			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:
		};


	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CEVENTQUEUE_H
