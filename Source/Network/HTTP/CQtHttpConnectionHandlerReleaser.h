/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPCONNECTIONHANDLERRELEASER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPCONNECTIONHANDLERRELEASER_H

// Namespace includes
#include "HttpSettings.h"
#include "CQtHttpConnectionHandler.h"


// Library includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {


			/*! 
			*
			*		\class		CQtHttpConnectionHandlerReleaser
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpConnectionHandlerReleaser {

				// public methods
				public:
					//! Constructor
					CQtHttpConnectionHandlerReleaser();

					virtual CQtHttpConnectionHandlerReleaser* releaseConnectionHandler(CQtHttpConnectionHandler* connectionHandler) = 0;

				// protected methods
				protected:

				// protected variables
				protected:


				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPCONNECTIONHANDLERRELEASER_H
