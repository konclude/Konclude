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

#ifndef KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNNER_H
#define KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNNER_H

// Libraries includes
#include <QString>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QDateTime>
#include <QCoreApplication>

// Namespace includes
#include "CConformanceTestCase.h"
#include "CConformanceTestResult.h"
#include "CConformanceExecution.h"
#include "CConformanceRunConfig.h"
#include "CConformanceTestSuite.h"
#include "CConformanceReporter.h"
#include "CConformanceKnownIssue.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Logger;

	namespace Test {

		namespace Conformance {

			/*!
			 *		\class		CConformanceRunner
			 *		\brief		Discovers all *.kt.owl.xml control ontologies under the
			 *					input directory, executes the described tasks with the
			 *					Konclude binary as a subprocess and writes a console report
			 *					as well as a result ontology.
			 *
			 *					The runner owns discovery, parsing, repetition handling and
			 *					reporting. The task specific work is delegated to a
			 *					CConformanceTaskHandler created by the handler factory, and
			 *					serialisation is delegated to a CConformanceResultWriter
			 *					created by the writer factory.
			 */
			class CConformanceRunner : public CLogIdentifier {
				// public methods
				public:
					CConformanceRunner();
					virtual ~CConformanceRunner();

					void setInputDirectory(const QString& dir);
					void setOutputFile(const QString& file);
					void setBinaryFile(const QString& file);
					void setKnownIssuesFile(const QString& file);
					//! Sets the default reasoner configuration file that is passed as
					//! '-c <file>' to every executed test case for which the control
					//! ontology does not specify its own kt:usesTestSystemConfig file.
					void setConfigFile(const QString& file);
					void setJobCount(qint64 jobs);
					void setRepeatCount(qint64 repeat);
					void setTimeoutSeconds(qint64 seconds);

					//! Sets how often progress/estimated-remaining-time is reported (0 disables it).
					void setProgressIntervalSeconds(qint64 seconds);
					//! Runs the whole suite. Returns the number of real failures
					//! (FAIL/FIXED/TIMEOUT/FLAKY), 0 when everything is green.
					int run();

				// protected methods
				protected:
					//! Discovery: all *.kt.owl.xml recursively under mInputDirectory.
					QStringList discoverControlOntologies();

					//! Reads a control ontology and appends its test cases to mTestCases.
					void parseControlOntology(const QString& filePath);

					//! Reads the kt:TestSuite declaration of a control ontology and
					//! registers the suite for the directory, deriving it when absent.
					void parseSuiteDeclaration(const QDomElement& root, const QString& filePath, const QString& relativeDirectory, QString& suiteId);

					//! Reads the kt:RunConfig declarations (timeout, repetitions) of a
					//! control ontology into mRunConfigs.
					void parseRunConfigDeclarations(const QDomElement& root);

					//! Reads the task marker ClassAssertions of a control ontology.
					void parseTaskClassAssertions(const QDomElement& root, const QString& filePath, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById);

					//! Reads the DataPropertyAssertions of a control ontology.
					void parseDataPropertyAssertions(const QDomElement& root, const QString& filePath, const QDir& caseDir, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById);

					//! Reads the ObjectPropertyAssertions of a control ontology: suite
					//! membership (kt:memberOfSuite) and the kt:usesTestRunConfig /
					//! kt:usesTestSystemConfig assignments (test or suite subject).
					void parseObjectPropertyAssertions(const QDomElement& root, const QString& filePath, const QDir& caseDir, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById);

					//! Interprets the expected result literal, applies the default
					//! interface and rejects incomplete test cases.
					bool finalizeTestCase(CConformanceTestCase& testCase);

					//! Expands every test case into one execution per applicable run
					//! configuration and system configuration combination.
					void buildExecutionPlan();

					//! Returns the suite with the given id, or a suite derived from the id.
					CConformanceTestSuite suiteFor(const QString& suiteId) const;

					//! Loads known-issues.owl.xml and collects the tests it declares
					//! affected by an open issue. An issue can affect many tests.
					void loadKnownIssues();

					//! Resolves a run configuration id, filling in the runner defaults.
					CConformanceRunConfig resolveRunConfig(const QString& runConfigId);

					//! Run configuration ids assigned to the test and to its suite.
					QStringList effectiveRunConfigIds(const CConformanceTestCase& testCase) const;

					//! System configuration files assigned to the test and to its suite,
					//! falling back to the suite-wide default of the runner.
					QStringList effectiveSystemConfigPaths(const CConformanceTestCase& testCase) const;

					//! Runs one execution (with repetitions) and classifies the result
					//! against the expectation and the known-issue marker. The worker
					//! index and thread id are only recorded for the report.
					CConformanceTestResult executeTestRun(const CConformanceExecution& execution, int workerIndex, const QString& threadId);

					//! Determines the path to this binary (for subprocess calls).
					QString resolveBinaryFile();

					//! Replaces characters that are unsafe in file names with underscores.
					QString sanitizeFileName(const QString& name) const;

					//! Returns the directory for per-test output files, creating it if
					//! needed: a 'test-outputs' folder next to the result ontology, or a
					//! temporary directory when no result ontology is configured.
					QString resolveOutputDirectory();

					//! Base name of the per-execution artifacts: sanitised test id plus the
					//! run configuration and system configuration, so that different
					//! configurations of the same test do not overwrite each other.
					QString resolveExecutionFileBase(const CConformanceExecution& execution, qint64 runIndex) const;

					//! Resolves the log file path for one run of an execution. Returns an
					//! empty string when output logging is disabled (no result ontology).
					QString resolveOutputLogFilePath(const CConformanceExecution& execution, qint64 runIndex);

					//! Resolves the response file path that the executed binary writes and
					//! from which the answer is read back.
					QString resolveResponseFilePath(const CConformanceExecution& execution, qint64 runIndex);

					//! Writes the result ontology through the writer matching mOutputFile.
					void writeResults(const QList<CConformanceTestResult>& results);

					//! Logs a periodic progress/estimated-remaining-time line when the
					//! configured interval has elapsed since the last report.
					void reportProgress(int completedCount, qint64 runStartMSecs, qint64& lastProgressMSecs);

					//! Logs one indented line per outcome of the given counts.
					void logOutcomeLines(const QMap<CConformanceOutcome::TYPE, int>& outcomeCounts, bool includeZeroOutcomes);

					//! Logs only the non-PASS outcomes with a non-zero count.
					void logNonPassingOutcomeLines(const QMap<CConformanceOutcome::TYPE, int>& outcomeCounts);

					//! Logs the KNOWN results grouped by issue type, with issue counts and
					//! descriptions.
					void logKnownIssueSummary(const QList<CConformanceTestResult>& results);

					//! Resolves the IRI or abbreviatedIRI of an element.
					static QString elementIRI(const QDomElement& element);

					//! Resolves a path from a control ontology against the directory that
					//! contains the control ontology.
					static QString resolveCaseFilePath(const QDir& caseDir, const QString& path);

					//! Returns the test case for id, creating and initialising it if needed.
					static CConformanceTestCase& testCaseFor(QHash<QString, CConformanceTestCase>& caseById, const QString& id, const QString& controlFile, const QString& suiteId);

					//! Appends value to values when it is not contained yet.
					static void appendUnique(QStringList& values, const QString& value);

				// protected variables
				protected:
					QString mInputDirectory;
					QString mOutputFile;
					QString mBinaryFile;
					QString mKnownIssuesFile;
					QString mConfigFile;             // default reasoner config file ('-c'), may be empty
					qint64 mJobCount;
					qint64 mRepeatCount;             // default repetitions of the built-in run configuration
					qint64 mTimeoutSeconds;          // default timeout of the built-in run configuration
					qint64 mProgressIntervalSeconds;   // > 0: report progress/ETA every N seconds; 0 disables

					QList<CConformanceTestCase> mTestCases;
					QList<CConformanceExecution> mExecutions;
					QHash<QString, CConformanceTestSuite> mSuites;
					QHash<QString, CConformanceRunConfig> mRunConfigs;
					QHash<QString, QString> mSuiteIdByDirectory;
					QHash<QString, QStringList> mRunConfigAssignments;		// subject (test/suite) -> run config ids
					QHash<QString, QStringList> mSystemConfigAssignments;	// subject (test/suite) -> '-c' file paths
					QHash<QString, CConformanceKnownIssue> mKnownIssues;	// issue id -> metadata
					QHash<QString, QStringList> mKnownIssuesByTest;			// test id -> issue ids
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNNER_H
