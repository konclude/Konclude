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

#include "CReasonerTestfileTest.h"


namespace Konclude {

	namespace Test {



		CReasonerTestfileTest::CReasonerTestfileTest() {
			testExeCount = 1;
			timeout = 0;
			testResult = new CReasonerTestfileTestResult();
		}


		CReasonerTestfileTest::~CReasonerTestfileTest() {
			delete testResult;
		}


		QString CReasonerTestfileTest::getTestRequestFile() {
			return testRequestFile;
		}

		CReasonerTestfileTest *CReasonerTestfileTest::setTestRequestFile(const QString &requestFile) {
			testRequestFile = requestFile;
			return this;
		}

		bool CReasonerTestfileTest::hasTestRequestFile() {
			return !testRequestFile.isEmpty();
		}


		QString CReasonerTestfileTest::getTestExpectedResponseFile() {
			return expectedResponseFile;
		}

		CReasonerTestfileTest *CReasonerTestfileTest::setTestExpectedResponseFile(const QString &responseFile) {
			expectedResponseFile = responseFile;
			return this;
		}

		bool CReasonerTestfileTest::hasTestExpectedResponseFile() {
			return !expectedResponseFile.isEmpty();
		}


		qint64 CReasonerTestfileTest::getTestLoopCount() {
			return testExeCount;
		}

		CReasonerTestfileTest *CReasonerTestfileTest::setTestLoopCount(qint64 loopCount) {
			testExeCount = loopCount;
			return this;
		}

		CReasonerTestfileTestResult *CReasonerTestfileTest::getTestResults() {
			return testResult;
		}

		QString CReasonerTestfileTest::getWorkingDirectory() {
			return workDir;
		}

		CReasonerTestfileTest *CReasonerTestfileTest::setWorkingDirectory(const QString &workingDir) {
			workDir = workingDir;
			return this;
		}

		bool CReasonerTestfileTest::hasWorkingDirectory() {
			return !workDir.isEmpty();
		}

		qint64 CReasonerTestfileTest::getTimeout() {
			return timeout;
		}

		CReasonerTestfileTest *CReasonerTestfileTest::setTimeout(qint64 testTimeout) {
			timeout = testTimeout;
			return this;
		}


	}; // end namespace Test

}; // end namespace Konclude
