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

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSEHANDLER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSEHANDLER_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpResponse.h"
#include "CQtHttpRequest.h"
#include "CQtHttpResponse.h"
#include "CHttpTransactionManager.h"

// Library includes
#include <QNetworkReply>

// Other includes
#include "Network/CNetworkResponse.h"

#include "Concurrent/Callback/CCallbackData.h"
#include "Concurrent/Callback/CCallbackRememberLinkedExecuter.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Network {

		namespace HTTP {


			/*! 
			*
			*		\class		CQtHttpResponseHandler
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpResponseHandler {
				// public methods
				public:
					//! Constructor
					CQtHttpResponseHandler(CQtHttpRequest* request, CQtHttpResponse* response, cint64 downloadSizeLimit, CHttpTransactionManager* manager);


					CQtHttpRequest* getRequest();
					CQtHttpResponse* getResponse();

					void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

				// protected methods
				protected:

				// protected variables
				protected:
					CQtHttpRequest* mRequest;
					CQtHttpResponse* mResponse;

					cint64 mDownloadSizeLimit;
					CHttpTransactionManager* mManager;
					bool mAborted;
					cint64 mTotalBytesReceived;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSEHANDLER_H
