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

#ifndef KONCLUDE_NETWORK_HTTP_HTTPSETTINGS
#define KONCLUDE_NETWORK_HTTP_HTTPSETTINGS

// Library includes
#include <QEvent>

// Namespace includes


// Other includes
#include "Network/NetworkSettings.h"

// Logger includes


namespace Konclude {

	namespace Network {

		namespace HTTP {


			/*! 
			 *
			 *		\file		HTTPSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */

			// Custom Events >= 2000
			const QEvent::Type EVENTHTTPREQUEST							= (QEvent::Type)3000;
			const QEvent::Type EVENTREPLYFINISHED						= (QEvent::Type)3001;
			const QEvent::Type EVENTERACTRESPONSEDATA					= (QEvent::Type)3002;
			const QEvent::Type EVENTEXTRACTDATACALLBACK					= (QEvent::Type)3003;
			const QEvent::Type EVENTRELEASERESPONSE						= (QEvent::Type)3004;
			const QEvent::Type EVENTINSTALLREQUESTFINISHEDCALLBACK		= (QEvent::Type)3005;
			const QEvent::Type EVENTHANDLEINCOMINGHTTPCONNECTION		= (QEvent::Type)3006;
			const QEvent::Type EVENTCONNECTIONREADYREAD					= (QEvent::Type)3007;
			const QEvent::Type EVENTCONNECTIONDISCONNECTED				= (QEvent::Type)3008;
			const QEvent::Type EVENTADDRESPONSEFINISHEDTEXT				= (QEvent::Type)3009;
			const QEvent::Type EVENTRESPONSEFINISHEDCALLBACK			= (QEvent::Type)3010;

			// forward declarations

			// Konclude code configs


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // end KONCLUDE_NETWORK_HTTP_HTTPSETTINGS