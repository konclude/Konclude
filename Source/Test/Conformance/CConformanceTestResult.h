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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTRESULT_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTRESULT_H

#include <QString>

namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTestResult
			 *		\brief		Result of an executed test case.
			 *					outcome: PASS | FAIL | KNOWN | FIXED | TIMEOUT | FLAKY
			 */
			class CConformanceTestResult {
				public:
					CConformanceTestResult() : mDurationMs(0), mFailing(false) {}

					QString mId;             // test individual
					QString mTask;           // test task type ("consistency" | "satisfiability" | ...)
					QString mInterface;      // interface the test ran on ("CommandLine" | "OWLlink" | ...)
					QString mOutcome;        // PASS/FAIL/KNOWN/FIXED/TIMEOUT/FLAKY
					QString mExpected;       // expected result
					QString mActual;         // actual result
					qint64 mDurationMs;      // run time
					bool mFailing;           // counts as a red run?
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTRESULT_H
