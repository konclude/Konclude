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

#ifndef KONCLUDE_NETWORK_HTTP_EVENTS_CCONNECTIONDISCONNECTEDEVENT_H
#define KONCLUDE_NETWORK_HTTP_EVENTS_CCONNECTIONDISCONNECTEDEVENT_H



// Libraries includes


// Namespace includes
#include "../HttpSettings.h"
#include "../CHttpResponse.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Network {

		namespace HTTP {

			namespace Events {



				/*! 
				 *	\class		CConnectionDisconnectedEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CConnectionDisconnectedEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CConnectionDisconnectedEvent();

						//! Destructor
						virtual ~CConnectionDisconnectedEvent();

						static const QEvent::Type EVENTTYPE = EVENTCONNECTIONDISCONNECTED;

					// protected methods
					protected:

					// protected variables
					protected:


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

#endif // KONCLUDE_NETWORK_HTTP_EVENTS_CCONNECTIONDISCONNECTEDEVENT_H
