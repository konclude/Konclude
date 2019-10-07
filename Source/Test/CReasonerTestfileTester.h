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

#ifndef KONCLUDE_TEST_CREASONERTESTFILETESTER_H
#define KONCLUDE_TEST_CREASONERTESTFILETESTER_H

// Libraries includes
#include <QProcess>
#include <QDir>

// Namespace includes
#include "CReasonerTestfileEvaluator.h"

// Other includes
#include "Test/Events/CTestTestfileEvent.h"
#include "Test/Events/CEvaluateTestfileTestEvent.h"

#include "Concurrent/CIntervalThread.h"

#include "Config/CConfiguration.h"

#include "Control/Command/Instructions/CTestTestfileCommand.h"

#include "Control/Command/CCommandRecordRouter.h"

#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"
#include "Control/Command/Records/CUnspecifiedMessageInformationRecord.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Control::Command::Instructions;

	namespace Test {

		using namespace Test::Events;


		/*! 
		 *
		 *		\class		CReasonerTestfileTester
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CReasonerTestfileTester : public CIntervalThread, public CLogIdentifier {
			Q_OBJECT

			// public methods
			public:
				//! Constructor
				CReasonerTestfileTester(CConfiguration *config, CWatchDog *watchDog = 0);

				//! Destructor
				virtual ~CReasonerTestfileTester();

				virtual CReasonerTestfileTester *realizeTestCommand(CTestTestfileCommand *command, CReasonerTestfileEvaluator *testEvaluator);

			// protected methods
			protected:

				virtual bool processTimer(qint64 timerID);

				virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

			// protected variables
			protected:
				QString testfile;
				QProcess *testProcess;
				CReasonerTestfileEvaluator *evaluator;
				CReasonerTestfileTest *testfileTest;
				CTestTestfileCommand *testCommand;
				CCommandRecordRouter *commandRecordRouter;

				QString reasonerString;
				QString reasonerConfigString;
				qint64 wCount;

				bool processCrashed;
				bool processTimedout;

				qint64 closeTimeout;
				qint64 killTimeout;
				qint64 runTimeout;

				const static qint64 TIMERCLOSEPROCESSID = 1;


			// private methods
			private:
				void startTestfileTest();
				void evaluateTestfileTest();

				virtual void sendEvaluateTestfileTestEvent();
				virtual void sendStartTestfileTestEvent();

			// private methods
			private slots:
				void processError(QProcess::ProcessError error);
				void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CREASONERTESTFILETESTER_H
