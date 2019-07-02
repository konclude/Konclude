/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CReasonerTestsuiteTester.h"


namespace Konclude {

	namespace Test {



		CReasonerTestsuiteTester::CReasonerTestsuiteTester(CConfiguration *config) : CLogIdentifier("::Konclude::Test::ReasonerTestsuiteTester",this) {
			configuration = config;
			testFileTester = new CReasonerTestfileTester(config);
			testErrorEvaluator = new CReasonerTestfileErrorEvaluator();
		}


		CReasonerTestsuiteTester::~CReasonerTestsuiteTester() {
			delete testFileTester;
			delete testErrorEvaluator;
		}


		CReasonerTestsuiteTester *CReasonerTestsuiteTester::realizeTestCommand(CTestTestsuiteCommand *command) {
			CCommandRecordRouter commandRecordRouter(command,this);
			CStartProcessCommandRecord::makeRecord(&commandRecordRouter);


			QTime totalTestTimer;
			totalTestTimer.start();

			CReasonerTestsuiteTest *testsuiteTest = command->getTestsuiteTest();

			CLinker<CCommand *> *subCommandIt = command->getSubCommandLinker();
			while (subCommandIt) {
				CCommand *subCommand = subCommandIt->getData();
				CTestTestfileCommand *testTestfileCommand = dynamic_cast<CTestTestfileCommand *>(subCommand);
				if (testTestfileCommand) {
					QTime testTimer;
					testTimer.start();
					QString testfile = testTestfileCommand->getReasonerTestfileTest()->getTestRequestFile();
					LOG(INFO,getLogDomain(),logTr("Starting tests for testfile '%1'.").arg(testfile),this);
					testFileTester->realizeTestCommand(testTestfileCommand,testErrorEvaluator);
					CCommandExecutedBlocker::waitExecutedCommand(testTestfileCommand);
					// go sure that thread doesn't operate on command any more
					testFileTester->waitSynchronization();
					qint64 testTimeElapsed = testTimer.elapsed();
					LOG(INFO,getLogDomain(),logTr("Testing testfile '%1' takes %2 ms.").arg(testfile).arg(testTimeElapsed),this);
				}
				subCommandIt = subCommandIt->getNext();
			}

			qint64 totalTestTimeElapsed = totalTestTimer.elapsed();
			testsuiteTest->getTestsuiteResult()->setTotalTestingTime(totalTestTimeElapsed);


			CStopProcessCommandRecord::makeRecord(&commandRecordRouter);
			CFinishProcessCommandRecord::makeRecord(&commandRecordRouter);
			return this;
		}


	}; // end namespace Test

}; // end namespace Konclude
