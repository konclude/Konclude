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

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPTRANSACTIONMANAGER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPTRANSACTIONMANAGER_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpTransactionManager.h"
#include "CHttpResponseExtractor.h"
#include "CHttpRequestFactory.h"
#include "CQtHttpRequest.h"
#include "CQtHttpResponse.h"
#include "CQtHttpResponseHandler.h"

// Library includes
#include <QNetworkAccessManager>
#include <QHash>

// Other includes
#include "Network/HTTP/Events/CHttpRequstEvent.h"
#include "Network/HTTP/Events/CHttpReplyFinishedEvent.h"
#include "Network/HTTP/Events/CExtractResponseDataEvent.h"
#include "Network/HTTP/Events/CExtractResponseDataCallbackEvent.h"
#include "Network/HTTP/Events/CReleaseResponseEvent.h"
#include "Network/HTTP/Events/CAddResponseFinishedCallbackEvent.h"
#include "Network/HTTP/Events/CInstallRequestFinishedCallbackEvent.h"
#include "Network/HTTP/Events/CAbortRequestEvent.h"

#include "Concurrent/CIntervalThread.h"
#include "Concurrent/Callback/CBlockingCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Callback;

	namespace Network {

		namespace HTTP {

			using namespace Events;


			/*! 
			*
			*		\class		CQtHttpTransactionManager
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CQtHttpTransactionManager : public CIntervalThread, public CHttpTransactionManager, public CHttpRequestFactory, public CHttpResponseExtractor {
				Q_OBJECT

				// public methods
				public:
					//! Constructor
					CQtHttpTransactionManager(cint64 timeoutInterval = 5*60*1000, cint64 downloadSizeLimit = -1, bool downloadLimitCancel = false);

					//! Destructor
					virtual ~CQtHttpTransactionManager();

					virtual CHttpRequest* createRequest(const QString& url);
					CHttpRequest* createRequest(const QString& url, const QByteArray& byteArray);
					//virtual CHttpRequest* createFileDownloadRequest(const QString& url, QIODevice* openWriteFile);

					virtual bool releaseResponse(CHttpResponse* response, bool releaseAlsoRequest = true);

					virtual CHttpResponse* getResponse(CHttpRequest* request);
					virtual CQtHttpResponse* getResponse(CQtHttpRequest* request);


					virtual CQtHttpTransactionManager* setRequestHeader(CHttpRequest* request, const QString& key, const QString& value);


					virtual bool waitFinished(CHttpResponse* response);

					virtual bool callbackFinished(CHttpResponse* response, CCallbackData* callback);
					virtual bool callbackFinished(CQtHttpResponse* response, CCallbackData* callback);
					virtual bool callbackFinishedRequest(CHttpResponse* response, CCallbackData* callbackData);

					virtual QIODevice* getResponseDataReadDevice(CHttpResponse* response);
					virtual QString* getExtractedText(CHttpResponse* response);
					virtual bool callbackResponseData(CHttpResponse* response, QByteArray* dataArray, CCallbackData* callback);
					virtual QByteArray* getResponseData(CHttpResponse* response);
					virtual bool hasFinishedSucecssfully(CHttpResponse* response);
					virtual bool hasFinished(CHttpResponse* response);
					virtual bool hasBeenAborted(CHttpResponse* response, ABORT_REASON* abortReason = nullptr);

					virtual bool abort(CHttpRequest* request, CHttpResponse* response, ABORT_REASON abortReason = ABORT_NO_REASON);

					virtual QString* getErrorString(CHttpResponse* response);

				// protected slots
				protected slots:
					void finishedNetworkReply(QNetworkReply* reply);

					virtual void threadStarted();
					virtual void threadStopped();


					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);
					virtual bool processTimer(qint64 timerID);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mTimeoutInterval;
					cint64 mDownloadSizeLimit;
					bool mDownloadLimitCancel;
					QNetworkAccessManager* mQNAM;
					QHash<QNetworkReply*,CQtHttpResponse*> mReplyResponseHash;
					QSet<CQtHttpResponse*> mCriticalTimeoutSet;
					QSet<CQtHttpResponse*> mNextTimeoutSet;
					QSet<CQtHttpResponse*> mTimeoutSet;
					QHash<CQtHttpResponse*, CQtHttpResponseHandler*> mHandlerHash;

					const static qint64 TIMEOUTCHECKTIMER = 1;

				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CQTHTTPTRANSACTIONMANAGER_H
