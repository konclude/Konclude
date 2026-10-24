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
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QProcess>
#include <QDomDocument>
#include <QTextStream>
#include <QElapsedTimer>
#include <QDateTime>
#include <QCoreApplication>

// Namespace includes
#include "CConformanceTestCase.h"
#include "CConformanceTestResult.h"
#include "CClassificationResultComparator.h"
#include "CConformanceReporter.h"

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
			 *					input directory, runs the described tests (currently
			 *					consistency and satisfiability tests via the Konclude
			 *					binary as a subprocess) and writes a console report as
			 *					well as an OWL/XML result ontology. First stage of variant B.
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
					//! ontology does not specify its own kt:usesConfig file.
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

					//! Runs a single test case and returns the result (with repetition).
					CConformanceTestResult executeTestCase(const CConformanceTestCase& testCase);

					//! Starts the Konclude binary as a subprocess for one test.
					//! Returns consistent/inconsistent or satisfiable/unsatisfiable,
					//! or "match"/"mismatch" for classification, otherwise "timeout"/"error".
					//! When logFilePath is non-empty, the subprocess' standard output
					//! and error (merged) are captured directly into that file by
					//! QProcess; otherwise they stay in the in-memory channel buffers.
					QString runTask(const CConformanceTestCase& testCase, qint64 timeoutMs, const QString& logFilePath);

					//! Parses an OWL/XML classification result file into a semantic,
					//! order-independent class hierarchy (equivalence classes become synsets).
					//! Returns null if the file cannot be read or contains no taxonomy.
					CClassHierarchyResult* loadClassificationHierarchy(const QString& filePath);

					//! Compares two OWL/XML classification result files semantically,
					//! independent of the order of subsumption assertions. On mismatch,
					//! detailMessage receives a short human-readable hint.
					bool isClassificationHierarchyEquivalent(const QString& actualFile, const QString& expectedFile, QString& detailMessage);

					//! Resolves the IRI of a <Class> element (expanding abbreviatedIRI
					//! against the declared prefixes, falling back to built-in prefixes).
					QString resolveClassIRI(const QDomElement& classElement, const QHash<QString, QString>& prefixIRIHash);

					//! Determines the path to this binary (for subprocess calls).
					QString resolveBinaryFile();

					//! Replaces characters that are unsafe in file names with underscores.
					QString sanitizeFileName(const QString& name) const;

					//! Resolves the log file path for one run of a test case under the
					//! output log directory (creating the directory if needed). Returns
					//! an empty string when output logging is disabled.
					QString resolveOutputLogFilePath(const CConformanceTestCase& testCase, qint64 runIndex);

					//! Writes the OWL/XML result ontology if an output path is set.
					void writeResultOntology(const QList<CConformanceTestResult>& results);

				// protected variables
				protected:
					QString mInputDirectory;
					QString mOutputFile;
					QString mBinaryFile;
					QString mKnownIssuesFile;
					QString mConfigFile;             // default reasoner config file ('-c'), may be empty
					qint64 mJobCount;
					qint64 mRepeatCount;
					qint64 mTimeoutSeconds;
qint64 mProgressIntervalSeconds;   // > 0: report progress/ETA every N seconds; 0 disables

					QList<CConformanceTestCase> mTestCases;
			};

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CONFORMANCE_CCONFORMANCERUNNER_H
