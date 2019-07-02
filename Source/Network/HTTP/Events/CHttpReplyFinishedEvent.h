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

#ifndef KONCLUDE_NETWORK_HTTP_EVENTS_CHTTPREPLYFINISHEDEVENT_H
#define KONCLUDE_NETWORK_HTTP_EVENTS_CHTTPREPLYFINISHEDEVENT_H



// Libraries includes
#include <QNetworkReply>

// Namespace includes
#include "../HttpSettings.h"


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
				 *	\class		CHttpReplyFinishedEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CHttpReplyFinishedEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CHttpReplyFinishedEvent(QNetworkReply* reply);

						//! Destructor
						virtual ~CHttpReplyFinishedEvent();

						QNetworkReply* getReply();

						static const QEvent::Type EVENTTYPE = EVENTREPLYFINISHED;

					// protected methods
					protected:

					// protected variables
					protected:
						QNetworkReply* mReply;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

#endif // KONCLUDE_NETWORK_HTTP_EVENTS_CHTTPREPLYFINISHEDEVENT_H
