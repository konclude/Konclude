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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTSPARQLTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTSPARQLTHREAD_H

// Libraries includes
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationRequestClientBaseThread.h"
#include "CReasonerEvaluationRequestResult.h"
#include "CReasonerEvaluationRequestResponseSPARQL.h"

// Other includes
#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Test/Evaluation/Events/CReasonerEvaluationStartEvent.h"
#include "Test/Evaluation/Events/CReasonerEvaluationNextEvent.h"

#include "Concurrent/CIntervalThread.h"

#include "Network/HTTP/CQtHttpTransactionManager.h"

#include "Parser/CSPARQLKnowledgeBaseSplittingOperationParser.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Parser;
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
			class CReasonerEvaluationRequestClientSPARQLThread : public CReasonerEvaluationRequestClientBaseThread {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationRequestClientSPARQLThread();

					//! Destructor
					virtual ~CReasonerEvaluationRequestClientSPARQLThread();

					virtual bool canHandleRequestFile(const QString& testFileString) override;

				// protected methods
				protected:
					virtual bool loadReasonerRequests(const QString& initFileString, const QString& testFileString) override;
					virtual bool loadReasonerRequest(const QString& requestFileString) override;

					virtual bool sendNextRequest() override;
					virtual bool parseResponse(QIODevice* device) override;

					QString getAdaptedRequestNode(const QString& reqeustString, CSPARQLKnowledgeBaseSplittingOperationData* operationData);

					QString getIRI(const QString& iriString);

					virtual CReasonerEvaluationRequestClientBaseThread* readConfig(CConfiguration* config);


				// protected variables
				protected:
					QHash<QString, QString> mPrefixHash;
					QString mBaseIRI;

					QHash<QString, QString> mSPARQLOperationKeywordNameSet;

					bool mConfSendPrepareBeforeQuerying;
					bool mConfSupportsPrepareCommand;

					cint64 mConfResponseSizeLimit;


				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONREQUESTCLIENTSPARQLTHREAD_H
