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


			QString CConformanceReporter::formatSummaryHeader(int descriptionFileCount, int testCount, int suiteCount, int executionCount) {
				QString fileWord = (descriptionFileCount == 1) ? QString("test description file") : QString("test description files");
				QString testWord = (testCount == 1) ? QString("test") : QString("tests");
				QString suiteWord = (suiteCount == 1) ? QString("suite") : QString("suites");
				QString executionWord = (executionCount == 1) ? QString("test execution") : QString("test executions");
				return QString("The run discovered %1 %2 containing %3 %4 in %5 %6; %7 %8:")
					.arg(descriptionFileCount).arg(fileWord).arg(testCount).arg(testWord)
					.arg(suiteCount).arg(suiteWord).arg(executionCount).arg(executionWord);
			}


			QString CConformanceReporter::formatSuiteConfigurationLine(const QString& suiteName, const QStringList& runConfigNames, const QStringList& systemConfigNames) {
				QString runConfigText = runConfigNames.isEmpty() ? QString("default") : runConfigNames.join(", ");
				QString systemConfigText = systemConfigNames.isEmpty() ? QString("default") : systemConfigNames.join(", ");
				return QString("Suite %1: run configurations: %2; system configurations: %3")
					.arg(suiteName).arg(runConfigText).arg(systemConfigText);
			}


			QString CConformanceReporter::formatSuiteLabel(const QString& suiteName) {
				return QString("(%1 suite)").arg(suiteName);
			}


			QString CConformanceReporter::formatConfigLabel(const QString& runConfigName, const QString& systemConfigName) {
				return QString("[configs: %1 + %2]").arg(runConfigName).arg(systemConfigName);
			}


			QStringList CConformanceReporter::formatOutcomeLines(const QMap<CConformanceOutcome::TYPE, int>& outcomeCounts, bool includeZeroOutcomes) {
				QStringList lines;
				for (CConformanceOutcome::TYPE outcome : CConformanceOutcome::allTypes()) {
					int count = outcomeCounts.value(outcome, 0);
					if (includeZeroOutcomes || count > 0) {
						lines << QString("%1 %2").arg(count).arg(CConformanceOutcome::typeToDescription(outcome));
					}
				}
				return lines;
			}


			QString CConformanceReporter::formatCategoryHeader(const QString& label, const QString& name, const QMap<CConformanceOutcome::TYPE, int>& outcomeCounts) {
				int total = 0;
				for (const int& count : outcomeCounts) {
					total += count;
				}
				int passed = outcomeCounts.value(CConformanceOutcome::TYPE::PASS, 0);
				int nonPassing = total - passed;
				QString executionWord = (total == 1) ? QString("test execution") : QString("test executions");
				if (nonPassing > 0) {
					return QString("%1 %2 (%3 %4, %5 non-passing):").arg(label).arg(name).arg(total).arg(executionWord).arg(nonPassing);
				}
				return QString("%1 %2 (%3 %4):").arg(label).arg(name).arg(total).arg(executionWord);
			}


			QString CConformanceReporter::formatTotalTimeLine(qint64 totalMilliseconds, int testCount, qint64 maximumPerTestMilliseconds) {
				double totalSeconds = totalMilliseconds / 1000.0;
				double maxSeconds = maximumPerTestMilliseconds / 1000.0;
				QString testWord = (testCount == 1) ? QString("test execution") : QString("test executions");
				return QString("Total run time taken for %1 %2: %3 s (%4 ms), maximum per-execution time: %5 s (%6 ms)")
					.arg(testCount).arg(testWord).arg(totalSeconds, 0, 'f', 3).arg(totalMilliseconds).arg(maxSeconds, 0, 'f', 3).arg(maximumPerTestMilliseconds);
			}


			QString CConformanceReporter::formatWorkersLine(int workerCount, int testCount) {
				QString workerWord = (workerCount == 1) ? QString("worker") : QString("workers");
				QString testWord = (testCount == 1) ? QString("test execution") : QString("test executions");
				return QString("Using %1 test %2 to run %3 %4.").arg(workerCount).arg(workerWord).arg(testCount).arg(testWord);
			}


			QString CConformanceReporter::formatProgressLine(int completedCount, int totalCount, qint64 elapsedMilliseconds, qint64 estimatedRemainingMilliseconds) {
				double percentage = (totalCount > 0) ? (100.0 * completedCount) / totalCount : 0.0;
				double elapsedSeconds = elapsedMilliseconds / 1000.0;
				double remainingSeconds = estimatedRemainingMilliseconds / 1000.0;
				QString testWord = (totalCount == 1) ? QString("test execution") : QString("test executions");
				return QString("Progress: %1 of %2 %3 completed (%4%), elapsed %5 s, estimated remaining time %6 s")
					.arg(completedCount).arg(totalCount).arg(testWord).arg(percentage, 0, 'f', 1).arg(elapsedSeconds, 0, 'f', 1).arg(remainingSeconds, 0, 'f', 1);
			}


			QString CConformanceReporter::formatFinishedLine(int failingCount, int totalCount) {
				QString testWord = (totalCount == 1) ? QString("test execution") : QString("test executions");
				if (failingCount == 0) {
					return QString("Conformance run finished: all %1 %2 passed successfully.").arg(totalCount).arg(testWord);
				}
				QString failWord = (failingCount == 1) ? QString("failing test execution") : QString("failing test executions");
				return QString("Conformance run finished: %1 %2 out of %3 %4.").arg(failingCount).arg(failWord).arg(totalCount).arg(testWord);
			}


			QString CConformanceReporter::formatResultLine(const CConformanceTestResult& result) {
				QString suiteName = result.mSuiteName.isEmpty() ? result.mSuiteId : result.mSuiteName;
				QString runConfigName = result.mRunConfigName.isEmpty() ? result.mRunConfigId : result.mRunConfigName;
				QString systemConfigName = result.mSystemConfigName.isEmpty() ? QString("system-default") : result.mSystemConfigName;
				QString interfaceSuffix = result.mInterface.isEmpty() ? QString() : QString("  [%1]").arg(result.mInterface);
				QString workerSuffix = result.mWorkerIndex > 0
						? QString("  --  %1").arg(formatWorkerLabel(result.mWorkerIndex, result.mThreadId)) : QString();

				QString line = QString("[%1]  %2  --  %3%4  --  %5%6  (%7ms)")
						.arg(CConformanceOutcome::typeToString(result.mOutcome), -7)
						.arg(formatSuiteLabel(suiteName))
						.arg(result.mId)
						.arg(interfaceSuffix)
						.arg(formatConfigLabel(runConfigName, systemConfigName))
						.arg(workerSuffix)
						.arg(result.mDurationMs);

				if (result.isFailing()) {
					QString expectedText = CConformanceTask::verdictToString(result.mTask, result.mExpectedVerdict);
					QString actualText = CConformanceTask::verdictToString(result.mTask, result.mActualVerdict);
					if (actualText.isEmpty()) {
						actualText = QString("no verdict");
					}
					line += QString("  --  expected: %1, got: %2").arg(expectedText).arg(actualText);
					if (!result.mExplanation.isEmpty()) {
						line += QString("  (%1)").arg(result.mExplanation);
					}
				}
				if (result.mOutcome == CConformanceOutcome::TYPE::KNOWN || result.mOutcome == CConformanceOutcome::TYPE::FIXED) {
					QString typeText = result.mKnownIssueTypes.isEmpty()
							? QString("unknown type") : result.mKnownIssueTypes.join(", ");
					QString idText = result.mKnownIssueIds.isEmpty()
							? QString() : QString(" [%1]").arg(result.mKnownIssueIds.join(", "));
					QString descriptionText = result.mKnownIssueDescriptions.isEmpty()
							? QString() : QString(": %1").arg(result.mKnownIssueDescriptions.join("; "));
					line += QString("  --  known issue: %1%2%3").arg(typeText).arg(idText).arg(descriptionText);
				}
				return line;
			}



			QString CConformanceReporter::formatWorkerLabel(int workerIndex, const QString& threadId) {
				if (threadId.isEmpty()) {
					return QString("[worker %1]").arg(workerIndex);
				}
				return QString("[worker %1, thread %2]").arg(workerIndex).arg(threadId);
			}


			QString CConformanceReporter::formatStartLine(const CConformanceExecution& execution, int workerIndex,
					const QString& threadId, int executionIndex, int executionCount) {
				QString suiteName = execution.mSuiteName.isEmpty() ? execution.mSuiteId : execution.mSuiteName;
				QString interfaceName = execution.mTestCase.mInterface.isEmpty()
						? CConformanceChannel::typeToString(execution.mTestCase.mChannel) : execution.mTestCase.mInterface;
				return QString("[START  ]  %1  --  %2  [%3]  --  %4  --  %5  (%6/%7)")
						.arg(formatSuiteLabel(suiteName))
						.arg(execution.mTestCase.mId)
						.arg(interfaceName)
						.arg(formatConfigLabel(execution.mRunConfig.getDisplayName(), execution.getSystemConfigName()))
						.arg(formatWorkerLabel(workerIndex, threadId))
						.arg(executionIndex)
						.arg(executionCount);
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
