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

#ifndef KONCLUDE_NETWORK_HTTP_EVENTS_CEXTRACTRESPONSETEXTEVENT_H
#define KONCLUDE_NETWORK_HTTP_EVENTS_CEXTRACTRESPONSETEXTEVENT_H



// Libraries includes


// Namespace includes
#include "../HttpSettings.h"
#include "../CQtHttpResponse.h"


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
				 *	\class		CExtractResponseTextEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CExtractResponseTextEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CExtractResponseTextEvent(CQtHttpResponse* response, QString* text, CCallbackData* callbacks);

						//! Destructor
						virtual ~CExtractResponseTextEvent();

						CQtHttpResponse* getResponse();
						QString* getText();
						CCallbackData* getCallbacks();

						static const QEvent::Type EVENTTYPE = EVENTEXTRACTRESPONSETEXT;

					// protected methods
					protected:

					// protected variables
					protected:
						CQtHttpResponse* mResponse;
						QString* mText;
						CCallbackData* mCallbacks;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

#endif // KONCLUDE_NETWORK_HTTP_EVENTS_CEXTRACTRESPONSETEXTEVENT_H
