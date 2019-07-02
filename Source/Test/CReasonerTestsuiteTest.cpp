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

#include "CReasonerTestsuiteTest.h"


namespace Konclude {

	namespace Test {



		CReasonerTestsuiteTest::CReasonerTestsuiteTest() {
			testExeCount = 1;
			timeout = 0;
		}


		CReasonerTestsuiteTest::~CReasonerTestsuiteTest() {
			qDeleteAll(testList);
		}


		CReasonerTestsuiteTestResult *CReasonerTestsuiteTest::getTestsuiteResult() {
			return &testsuiteResult;
		}


		qint64 CReasonerTestsuiteTest::getDefaultTestLoopCount() {
			return testExeCount;
		}

		CReasonerTestsuiteTest *CReasonerTestsuiteTest::setDefaultTestLoopCount(qint64 loopCount) {
			testExeCount = loopCount;
			return this;
		}

		QString CReasonerTestsuiteTest::getDefaultWorkingDirectory() {
			return workDir;
		}

		CReasonerTestsuiteTest *CReasonerTestsuiteTest::setDefaultWorkingDirectory(const QString &workingDir) {
			workDir = workingDir;
			return this;
		}

		bool CReasonerTestsuiteTest::hasDefaultWorkingDirectory() {
			return !workDir.isEmpty();
		}

		CReasonerTestfileTest *CReasonerTestsuiteTest::createTestfileTest() {
			CReasonerTestfileTest *test = new CReasonerTestfileTest();
			testList.append(test);
			if (hasDefaultWorkingDirectory()) {
				test->setWorkingDirectory(getDefaultWorkingDirectory());
			}
			test->setTestLoopCount(getDefaultTestLoopCount());
			test->setTimeout(timeout);
			return test;
		}

		QLinkedList<CReasonerTestfileTest *> CReasonerTestsuiteTest::getTestfileTestList() {
			return testList;
		}


		qint64 CReasonerTestsuiteTest::getDefaultTimeout() {
			return timeout;
		}

		CReasonerTestsuiteTest *CReasonerTestsuiteTest::setDefaultTimeout(qint64 defTimeout) {
			timeout = defTimeout;
			return this;
		}



	}; // end namespace Test

}; // end namespace Konclude
