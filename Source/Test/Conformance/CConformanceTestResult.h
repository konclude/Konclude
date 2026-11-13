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

// Libraries includes
#include <QString>
#include <QStringList>

// Namespace includes
#include "CConformanceTask.h"
#include "CConformanceOutcome.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceTestResult
			 *		\brief		Result of one execution of a test case: the typed outcome,
			 *					the expected and observed verdicts, the duration, the
			 *					explanation of a failure, the per-test output log file and
			 *					the suite / run configuration / system configuration the
			 *					execution belonged to.
			 *
			 *					Whether the result turns a run red is derived from the
			 *					outcome; it is not stored separately.
			 */
			class CConformanceTestResult {
				// public methods
				public:
					CConformanceTestResult();

					//! True when the outcome turns a test run red.
					bool isFailing() const;

				// public variables
				public:
					QString mId;                            // test individual
					CConformanceTask::TYPE mTask;           // task kind the test asked for
					QString mInterface;                     // interface the test ran on ("CommandLine" | "OWLlink" | ...)
					CConformanceOutcome::TYPE mOutcome;     // PASS/KNOWN/FAIL/FIXED/TIMEOUT/FLAKY
					CConformanceTask::VERDICT mExpectedVerdict; // expected answer
					CConformanceTask::VERDICT mActualVerdict;   // observed answer, UNSET when none was obtained
					QString mExplanation;                   // why the run failed or mismatched
					QString mOutputLogFile;                 // absolute path to the logged output of the executed run
					QString mResponseFile;                  // absolute path to the answer written by the run
					int mWorkerIndex;                       // 1-based test worker that executed the test
					QString mThreadId;                      // id of the thread the worker ran the test in
					qint64 mDurationMs;                     // run time
					QString mSuiteId;                       // suite of the test
					QString mSuiteName;                     // suite name used in reports
					QString mRunConfigId;                   // run configuration of this execution
					QString mRunConfigName;                 // run configuration name used in reports
					QString mSystemConfigPath;              // '-c' file of this execution, may be empty
					QString mSystemConfigName;              // system configuration name used in reports
					QStringList mKnownIssueIds;             // issues linked to the test
					QStringList mKnownIssueTypes;           // issue types, parallel to mKnownIssueIds
					QStringList mKnownIssueDescriptions;    // issue descriptions, parallel to mKnownIssueIds
			};


			inline CConformanceTestResult::CConformanceTestResult()
					: mTask(CConformanceTask::TYPE::CONSISTENCY), mOutcome(CConformanceOutcome::TYPE::FAIL),
					  mExpectedVerdict(CConformanceTask::VERDICT::UNSET),
					  mActualVerdict(CConformanceTask::VERDICT::UNSET), mWorkerIndex(1), mDurationMs(0) {
			}


			inline bool CConformanceTestResult::isFailing() const {
				return CConformanceOutcome::typeIsFailing(mOutcome);
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCETESTRESULT_H
