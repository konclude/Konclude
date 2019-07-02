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

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPLISTNER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPLISTNER_H

// Namespace includes
#include "HttpSettings.h"
#include "CQtHttpConnectionHandlerFactory.h"


// Library includes
#include <QTcpServer>
#include <QTcpSocket>

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			using namespace Events;


			/*! 
			*
			*		\class		CQtHttpListner
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpListner : public QTcpServer {

				// public methods
				public:
					//! Constructor
					CQtHttpListner(CQtHttpConnectionHandlerFactory* handlerFactory, cint64 port);

					//! Destructor
					virtual ~CQtHttpListner();


				// protected methods
				protected:
					virtual void incomingConnection(int socketDescriptor);

				// protected variables
				protected:
					CQtHttpConnectionHandlerFactory* mHandlerFactory;
					cint64 mPort;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPLISTNER_H
