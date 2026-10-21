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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEREPORTER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEREPORTER_H

// Libraries includes
#include <QtGlobal>
#include <QString>
#include <QStringList>
#include <QHash>


namespace Konclude {

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceReporter
			 *		\brief		Formats conformance test results as readable, human-sentence-style
			 *					report lines (e.g. outcome counts with their abbreviations, and
			 *					per-test-type / per-interface breakdowns). Produces the strings only;
			 *					the caller logs them through the Konclude logger. Reusable for any
			 *					collection of CConformanceTestResult outcomes.
			 */
			class CConformanceReporter {

				// public methods
				public:

				//! Builds the overall header line, e.g.
				//! "The run discovered 6 test description files containing 26 tests:".
				static QString formatSummaryHeader(int descriptionFileCount, int testCount);

				//! Builds the list of per-outcome lines, each on its own string, e.g.
				//! ["22 passed (PASS)", "4 with known issues (KNOWN)", "0 failed (FAIL)", ...].
				//! When includeZeroOutcomes is true every outcome is listed (including zeros);
				//! otherwise only outcomes with a non-zero count are produced.
				static QStringList formatOutcomeLines(const QHash<QString, int>& outcomeCounts, bool includeZeroOutcomes);

				//! Builds a per-category header line, e.g.
				//! "test type classification (2 tests):" or "interface CommandLine (24 tests):".
				static QString formatCategoryHeader(const QString& label, const QString& name, const QHash<QString, int>& outcomeCounts);

				//! Builds the overall timing line, e.g.
				//! "Total run time taken for 26 tests: 0.920 s (920 ms), maximum per-test time: 0.410 s (410 ms).".
				static QString formatTotalTimeLine(qint64 totalMilliseconds, int testCount, qint64 maximumPerTestMilliseconds);

				//! Builds the line reporting how many test workers are used, e.g.
				//! "Using 1 test worker to run 26 test cases.".
				static QString formatWorkersLine(int workerCount, int testCount);

				//! Builds a periodic progress/ETA line, e.g.
				//! "Progress: 5 of 26 tests completed (19%), elapsed 1.2 s, estimated remaining time 4.1 s".
				static QString formatProgressLine(int completedCount, int totalCount, qint64 elapsedMilliseconds, qint64 estimatedRemainingMilliseconds);

				//! Builds the clear "run finished" line, e.g.
				//! "Conformance run finished: all 26 tests passed successfully." or
				//! "Conformance run finished: 1 failing test out of 26 tests.".
				static QString formatFinishedLine(int failingCount, int totalCount);

				//! Builds one line for a failing test for the final failure summary, e.g.
				//! "[FAIL] kt:galen-classification  -- expected: match, got: mismatch (subsumption hierarchies differ)".
				static QString formatFailureLine(const QString& testId, const QString& outcome, const QString& expected, const QString& actual);

			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCEREPORTER_H