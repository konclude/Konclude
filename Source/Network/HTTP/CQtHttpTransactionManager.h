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

#ifndef KONCLUDE_NETWORK_HTTP_CQTHTTPTRANSACTIONMANAGER_H
#define KONCLUDE_NETWORK_HTTP_CQTHTTPTRANSACTIONMANAGER_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpTransactionManager.h"
#include "CHttpResponseExtractor.h"
#include "CHttpRequestFactory.h"
#include "CQtHttpRequest.h"
#include "CQtHttpResponse.h"

// Library includes
#include <QNetworkAccessManager>
#include <QHash>

// Other includes
#include "Network/HTTP/Events/CHttpRequstEvent.h"
#include "Network/HTTP/Events/CHttpReplyFinishedEvent.h"
#include "Network/HTTP/Events/CExtractResponseTextEvent.h"
#include "Network/HTTP/Events/CExtractResponseTextCallbackEvent.h"
#include "Network/HTTP/Events/CReleaseResponseEvent.h"
#include "Network/HTTP/Events/CInstallRequestFinishedCallbackEvent.h"

#include "Concurrent/CIntervalThread.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent;

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
					CQtHttpTransactionManager(cint64 timeoutInterval = 5*60*1000);

					//! Destructor
					virtual ~CQtHttpTransactionManager();

					virtual CHttpRequest* createRequest(const QString& url);
					virtual CHttpRequest* createRequest(const QString& url, const QByteArray& byteArray);
					virtual bool releaseResponse(CHttpResponse* response, bool releaseAlsoRequest = true);

					virtual CHttpResponse* getResponse(CHttpRequest* request);
					virtual CQtHttpResponse* getResponse(CQtHttpRequest* request);

					virtual QString* getExtractedText(CHttpResponse* response);
					virtual bool hasFinishedSucecssful(CHttpResponse* response);

					virtual bool callbackFinishedRequest(CHttpResponse* response, CCallbackData* callbackData);


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
					QNetworkAccessManager* mQNAM;
					QHash<QNetworkReply*,CQtHttpResponse*> mReplyResponseHash;
					QSet<CQtHttpResponse*> mCriticalTimeoutSet;
					QSet<CQtHttpResponse*> mNextTimeoutSet;
					QSet<CQtHttpResponse*> mTimeoutSet;

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
