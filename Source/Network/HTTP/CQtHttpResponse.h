/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSE_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSE_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpResponse.h"
#include "CQtHttpRequest.h"

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
			*		\class		CQtHttpResponse
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpResponse : public CHttpResponse {
				// public methods
				public:
					//! Constructor
					CQtHttpResponse(CQtHttpRequest* request, QNetworkReply* qNetworkReply = nullptr);

					//! Destructor
					virtual ~CQtHttpResponse();

					QNetworkReply* getQNetworkReply();
					CQtHttpRequest* getHttpRequest();
					CQtHttpResponse* setQNetworkReply(QNetworkReply* qNetworkReply);

					CCallbackRememberLinkedExecuter* getFinishedCallbackList();

					cint64 getDownloadSizeLimit();
					CQtHttpResponse* setDownloadSizeLimit(cint64 limit);

					bool isAborted();
					cint64 getAbortReason();
					CQtHttpResponse* setAborted(cint64 abortReason = 0);

				// protected methods
				protected:

				// protected variables
				protected:
					QNetworkReply* mQNetworkReply;
					CCallbackRememberLinkedExecuter finishCallbackList;
					CQtHttpRequest* mRequest;

					cint64 mDownloadSizeLimit;
					cint64 mAbortReason;
					bool mAborted;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPRESPONSE_H
