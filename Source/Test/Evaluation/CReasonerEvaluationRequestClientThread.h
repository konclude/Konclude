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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTTHREAD_H

// Libraries includes
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationRequestClient.h"
#include "CReasonerEvaluationRequestResult.h"
#include "CReasonerEvaluationExecutionCallbackContextData.h"

// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Test/Evaluation/Events/CReasonerEvaluationStartEvent.h"
#include "Test/Evaluation/Events/CReasonerEvaluationNextEvent.h"

#include "Concurrent/CIntervalThread.h"

#include "Network/HTTP/CQtHttpTransactionManager.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Network::HTTP;

	namespace Test {

		namespace Evaluation {

			using namespace Events;


			/*! 
			 *
			 *		\class		CReasonerEvaluationRequestClientThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationRequestClientThread : public CIntervalThread, public CReasonerEvaluationRequestClient, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationRequestClientThread();

					//! Destructor
					virtual ~CReasonerEvaluationRequestClientThread();


					bool evaluateReasoner(const QString& initFileString, const QString& testFileString, const QString& addressString, CConfiguration* configuration, CCallbackData* callback);
					CReasonerEvaluationRequestResult* getReasonerEvaluationResult();

				// protected methods
				protected:
					virtual bool processTimer(qint64 timerID);

					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					virtual void threadStarted();
					virtual void threadStopped();


					bool loadReasonerRequests(const QString& initFileString, const QString& testFileString);
					bool loadReasonerRequest(const QString& requestFileString);

					QDomElement getAdabtedRequestNode(QDomElement& el);

					QDomElement getResolvedAbbreviatedIRIsNode(QDomElement& el, const QHash<QString,QString>& appreviatePrefixHash, const QString& xmlBase);
					bool startsWithScheme(const QString& uriString);

					bool sendNextRequest();
					bool hasNextRequest();
					QDomDocument createRequestDocument(const QDomElement& reqNode);

					bool closeNextRequest();
					bool closeAllRequestTimeout();

					bool finishReasonerRequests();

					QDomElement getModifiedRequestNode(const QDomElement& el);

				// protected variables
				protected:
					CQtHttpTransactionManager* mTransManager;
					CConfiguration* mConfig;

					CReasonerEvaluationRequestResult* mResponse;
					QString mInitFileString;
					QString mTestFileString;
					QString mAddressString;
					CCallbackData* mRequestFinishedCallback;

					cint64 mRequestTimeout;
					cint64 mTotalTimeout;

					QList<CReasonerEvaluationRequestResponse*> mRemainingRequestList;

					cint64 mNumberInitRequests;
					CReasonerEvaluationRequestResponse* mNextReqRes;
					CHttpResponse* mWebResponse;
					CHttpRequest* mWebRequest;
					cint64 mCurrentRequestCount;
					cint64 mTotalRequestCount;
					QTime mNextRequestTiming;
					QTime mTotalTiming;
					QTime mTestingTiming;
					cint64 mCurrentOperationNumber;

					bool mModifyRequestAbsolutePaths;
					bool mReplaceLoadOntologiesWithTells;
					bool mResolveAppreviatedIRIsForReplacedTells;


					const static cint64 TIMERIDREQUESTTIMEOUT		= 1;
					const static cint64 TIMERIDTOTALTIMEOUT			= 2;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTTHREAD_H
