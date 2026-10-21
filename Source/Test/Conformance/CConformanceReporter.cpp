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

#include "CConformanceReporter.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			QString CConformanceReporter::formatSummaryHeader(int descriptionFileCount, int testCount) {
				QString fileWord = (descriptionFileCount == 1) ? QString("test description file") : QString("test description files");
				QString testWord = (testCount == 1) ? QString("test") : QString("tests");
				return QString("The run discovered %1 %2 containing %3 %4:")
					.arg(descriptionFileCount).arg(fileWord).arg(testCount).arg(testWord);
			}


			QStringList CConformanceReporter::formatOutcomeLines(const QHash<QString, int>& outcomeCounts, bool includeZeroOutcomes) {
				QStringList lines;
				auto addOutcome = [&lines, &outcomeCounts, &includeZeroOutcomes](const QString& outcome, const QString& word) {
					int count = outcomeCounts.value(outcome, 0);
					if (includeZeroOutcomes || count > 0) {
						lines << QString("%1 %2").arg(count).arg(word);
					}
				};
				addOutcome("PASS", "passed (PASS)");
				addOutcome("KNOWN", "with known issues (KNOWN)");
				addOutcome("FAIL", "failed (FAIL)");
				addOutcome("FIXED", "known issues now fixed (FIXED)");
				addOutcome("TIMEOUT", "timed out (TIMEOUT)");
				addOutcome("FLAKY", "flaky (FLAKY)");
				return lines;
			}


			QString CConformanceReporter::formatCategoryHeader(const QString& label, const QString& name, const QHash<QString, int>& outcomeCounts) {
				int total = 0;
				for (const int& count : outcomeCounts) {
					total += count;
				}
				QString testWord = (total == 1) ? QString("test") : QString("tests");
				return QString("%1 %2 (%3 %4):").arg(label).arg(name).arg(total).arg(testWord);
			}


			QString CConformanceReporter::formatTotalTimeLine(qint64 totalMilliseconds, int testCount, qint64 maximumPerTestMilliseconds) {
				double totalSeconds = totalMilliseconds / 1000.0;
				double maxSeconds = maximumPerTestMilliseconds / 1000.0;
				QString testWord = (testCount == 1) ? QString("test") : QString("tests");
				return QString("Total run time taken for %1 %2: %3 s (%4 ms), maximum per-test time: %5 s (%6 ms)")
					.arg(testCount).arg(testWord).arg(totalSeconds, 0, 'f', 3).arg(totalMilliseconds).arg(maxSeconds, 0, 'f', 3).arg(maximumPerTestMilliseconds);
			}


			QString CConformanceReporter::formatWorkersLine(int workerCount, int testCount) {
				QString workerWord = (workerCount == 1) ? QString("worker") : QString("workers");
				QString testWord = (testCount == 1) ? QString("test case") : QString("test cases");
				return QString("Using %1 test %2 to run %3 %4.").arg(workerCount).arg(workerWord).arg(testCount).arg(testWord);
			}


			QString CConformanceReporter::formatProgressLine(int completedCount, int totalCount, qint64 elapsedMilliseconds, qint64 estimatedRemainingMilliseconds) {
				double percentage = (totalCount > 0) ? (100.0 * completedCount) / totalCount : 0.0;
				double elapsedSeconds = elapsedMilliseconds / 1000.0;
				double remainingSeconds = estimatedRemainingMilliseconds / 1000.0;
				QString testWord = (totalCount == 1) ? QString("test") : QString("tests");
				return QString("Progress: %1 of %2 %3 completed (%4%), elapsed %5 s, estimated remaining time %6 s")
					.arg(completedCount).arg(totalCount).arg(testWord).arg(percentage, 0, 'f', 1).arg(elapsedSeconds, 0, 'f', 1).arg(remainingSeconds, 0, 'f', 1);
			}


			QString CConformanceReporter::formatFinishedLine(int failingCount, int totalCount) {
				QString testWord = (totalCount == 1) ? QString("test") : QString("tests");
				if (failingCount == 0) {
					return QString("Conformance run finished: all %1 %2 passed successfully.").arg(totalCount).arg(testWord);
				}
				QString failWord = (failingCount == 1) ? QString("failing test") : QString("failing tests");
				return QString("Conformance run finished: %1 %2 out of %3 %4.").arg(failingCount).arg(failWord).arg(totalCount).arg(testWord);
			}


			QString CConformanceReporter::formatFailureLine(const QString& testId, const QString& outcome, const QString& expected, const QString& actual) {
				return QString("[%1] %2  -- expected: %3, got: %4").arg(outcome).arg(testId).arg(expected).arg(actual);
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude