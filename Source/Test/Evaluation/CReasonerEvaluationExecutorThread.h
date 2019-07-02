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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXECUTORTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXECUTORTHREAD_H

// Libraries includes


// Namespace includes
#include "CReasonerEvaluationExecutor.h"
#include "CReasonerEvaluationProvider.h"
#include "CConfigDependendReasonerEvaluationFactory.h"
#include "CReasonerEvaluationRequestClientThread.h"
#include "CReasonerEvaluationEvaluator.h"
#include "CReasonerEvaluationEvaluatedCallbackContextData.h"
#include "CReasonerEvaluationExecutionCallbackContextData.h"
#include "CReasonerEvaluationPathConverter.h"
#include "CReasonerEvaluationFiltering.h"


// Other includes
#include "Test/Evaluation/Events/CReasonerEvaluationTestsuiteEvent.h"
#include "Test/Evaluation/Events/CReasonerEvaluationNextEvent.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Concurrent/CIntervalThread.h"

#include "Control/Command/Events/CCommandProcessedCallbackEvent.h"

#include "Concurrent/Callback/CBlockingCallbackData.h"

#include "Control/Command/CCommanderManager.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Concurrent::Callback;
	using namespace Control::Command::Events;
	using namespace Control::Command;

	namespace Test {

		namespace Evaluation {

			using namespace Evaluation::Events;


			/*! 
			 *
			 *		\class		CReasonerEvaluationExecutorThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationExecutorThread : public CThread, public CReasonerEvaluationExecutor, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationExecutorThread(CConfiguration *config, CWatchDog *watchDog = nullptr);

					//! Destructor
					virtual ~CReasonerEvaluationExecutorThread();

					bool executeEvaluation(CReasonerEvaluationTestsuite* evaluationTestsuite, CCallbackData* callbackData);


				// protected methods
				protected:

					bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);
					bool loadConfigurations();
					bool initEvaluationTests();

					bool finishEvaluationTests();
					bool failedEvaluationTests();

					QString nextEvaluationTestfile();

					bool prepareNextEvaluationTest();
					bool executeNextEvaluationTest(const QString& testcaseInit, const QString& testcaseInput);
					
					bool evaluateExecutedTest(CReasonerEvaluationTerminationResult* terminationResults, CReasonerEvaluationRequestResult* requestResults, const QString& testcaseOutput);

					QString getNumberString(cint64 number);
					QString getNextOutputFileString();


					bool testExistFile(const QString& fileString);

					bool testFileFiltered(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString);


					QString getCorrectedPathString(const QString& string);


				// protected variables
				protected:
					CReasonerEvaluationTestsuite* mEvalTestsuite;
					CConfiguration* mLoaderConfig;
					CCommanderManager* mCommanderManager;
					CConfiguration* mConfiguration;
					CCallbackData* mExecutedCallback;
					bool mExecutedTest;
					bool mFirstTest;
					cint64 mErrorCommBreakCount;
					QString mPlatformName;

					CReasonerEvaluationFiltering* mFiltering;

					QString mInputDir;
					QString mOutputDir;


					QString mReasonerName;
					QString mReasonerOutputDirectory;
					QString mConfigName;


					QStringList mRemainingSubDirs;
					QStringList mRemainingFiles;

					cint64 mTestCount;
					cint64 mNumberStringSize;

					QString mNextTestInputFile;
					QString mNextTestOutputDir;
					QString mReasonerInitFileString;
					cint64 mNextTestRemainingExecutionCount;
					cint64 mNextTestExecutionNumber;

					cint64 mTestTimeout;
					cint64 mWaitingTimeBetweenTests;
					cint64 mWaitingTimeAfterReasonerCreation;

					bool mFirstTestWarmUp;
					bool mAllTestsWarmUp;


					CReasonerEvaluationProvider* mReasonerProvider;
					CReasonerEvaluationFactory* mReasonerEvalFactory;

					CReasonerEvaluationEvaluator* mReasonerEvaluator;
					CReasonerEvaluationRequestClientThread* mReasonerClient;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXECUTORTHREAD_H
