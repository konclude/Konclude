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

#include "CQtHttpListner.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {

			CQtHttpListner::CQtHttpListner(CQtHttpConnectionHandlerFactory* handlerFactory, cint64 port) {
				mPort = port;
				mHandlerFactory = handlerFactory;

				listen(QHostAddress::Any,port);
				if (isListening()) {
					LOG(INFO,"::Konclude::Network::HTTP::HttpListner",logTr("Listening on port '%1'").arg(port),this);
				} else {
					LOG(WARNING,"::Konclude::Network::HTTP::HttpListner",logTr("Cannot bind to port '%1', because: '%2'.").arg(port).arg(errorString()),this);
				}
			}

			CQtHttpListner::~CQtHttpListner() {
			}

			void CQtHttpListner::incomingConnection(qintptr  socketDescriptor) {
				CQtHttpConnectionHandler* handler = mHandlerFactory->getConnectionHandler();
				if (handler) {
					handler->handleIncomingConnection(socketDescriptor);
				} else {
					QTcpSocket* socket=new QTcpSocket(this);
					if (socket->setSocketDescriptor(socketDescriptor)) {
						connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
						socket->write("HTTP/1.1 503 too many connections\r\nConnection: close\r\n\r\nToo many connections\r\n");
						LOG(WARNING,"::Konclude::Network::HTTP::HttpListner",logTr("Too many connections on port '%1'.").arg(mPort),this);
						socket->disconnectFromHost();
					} else {
						connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
						LOG(WARNING,"::Konclude::Network::HTTP::HttpListner",logTr("Set socket description error."),this);
					}
				}
			}


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude

