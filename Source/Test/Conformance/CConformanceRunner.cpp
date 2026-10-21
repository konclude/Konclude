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

#include "CConformanceRunner.h"

#include <QHash>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CConformanceRunner::CConformanceRunner()
					: CLogIdentifier("::Konclude::Test::ConformanceRunner", this),
					  mJobCount(1), mRepeatCount(1), mTimeoutSeconds(60), mProgressIntervalSeconds(3) {
			}


			CConformanceRunner::~CConformanceRunner() {
			}


			void CConformanceRunner::setInputDirectory(const QString& dir) { mInputDirectory = dir; }
			void CConformanceRunner::setOutputFile(const QString& file) { mOutputFile = file; }
			void CConformanceRunner::setBinaryFile(const QString& file) { mBinaryFile = file; }
			void CConformanceRunner::setKnownIssuesFile(const QString& file) { mKnownIssuesFile = file; }
			void CConformanceRunner::setJobCount(qint64 jobs) { mJobCount = jobs > 0 ? jobs : 1; }
			void CConformanceRunner::setRepeatCount(qint64 repeat) { mRepeatCount = repeat > 0 ? repeat : 1; }
			void CConformanceRunner::setTimeoutSeconds(qint64 seconds) { mTimeoutSeconds = seconds > 0 ? seconds : 60; }
			void CConformanceRunner::setProgressIntervalSeconds(qint64 seconds) { mProgressIntervalSeconds = seconds >= 0 ? seconds : 0; }


			QString CConformanceRunner::resolveBinaryFile() {
				if (!mBinaryFile.isEmpty()) {
					return mBinaryFile;
				}
				// Default: the currently running binary itself.
				QString appFile = QCoreApplication::applicationFilePath();
				if (!appFile.isEmpty()) {
					return appFile;
				}
				return QString("Konclude");
			}


			QStringList CConformanceRunner::discoverControlOntologies() {
				QStringList controlFiles;
				QDir baseDir(mInputDirectory);
				if (!baseDir.exists()) {
					LOG(ERROR, getLogDomain(), logTr("Conformance input directory '%1' does not exist.").arg(mInputDirectory), this);
					return controlFiles;
				}
				QStringList pendingDirs;
				pendingDirs.append(baseDir.absolutePath());
				while (!pendingDirs.isEmpty()) {
					QString dirPath = pendingDirs.takeFirst();
					QDir dir(dirPath);
					// Files: all *.kt.owl.xml
					QStringList nameFilters;
					nameFilters << "*.kt.owl.xml";
					QFileInfoList fileInfos = dir.entryInfoList(nameFilters, QDir::Files, QDir::Name);
					for (const QFileInfo& fi : fileInfos) {
						controlFiles.append(fi.absoluteFilePath());
					}
					// Subdirectories recursively.
					QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
					for (const QFileInfo& di : subDirs) {
						pendingDirs.append(di.absoluteFilePath());
					}
				}
				controlFiles.sort();
				return controlFiles;
			}


			void CConformanceRunner::parseControlOntology(const QString& filePath) {
				QFile file(filePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					LOG(ERROR, getLogDomain(), logTr("Cannot open control ontology '%1'.").arg(filePath), this);
					return;
				}
				QDomDocument doc;
				QString errMsg;
				int errLine = 0, errCol = 0;
				if (!doc.setContent(&file, &errMsg, &errLine, &errCol)) {
					file.close();
					LOG(ERROR, getLogDomain(), logTr("Cannot parse control ontology '%1' (%2 at line %3).").arg(filePath).arg(errMsg).arg(errLine), this);
					return;
				}
				file.close();

				QFileInfo controlInfo(filePath);
				QDir caseDir = controlInfo.absoluteDir();

				// Collect the data for each test individual.
				QHash<QString, CConformanceTestCase> caseById;

				QDomElement root = doc.documentElement();

				// 1) ClassAssertion(kt:ConsistencyTest, individual) -> test kind.
				QDomNodeList classAssertions = root.elementsByTagName("ClassAssertion");
				for (int i = 0; i < classAssertions.count(); ++i) {
					QDomElement ca = classAssertions.at(i).toElement();
					QDomElement cls = ca.firstChildElement("Class");
					QDomElement ind = ca.firstChildElement("NamedIndividual");
					if (cls.isNull() || ind.isNull()) {
						continue;
					}
					QString className = cls.attribute("abbreviatedIRI");
					if (className.isEmpty()) {
						className = cls.attribute("IRI");
					}
					QString indName = ind.attribute("abbreviatedIRI");
					if (indName.isEmpty()) {
						indName = ind.attribute("IRI");
					}
					if (className.endsWith("ConsistencyTest")) {
						CConformanceTestCase& tc = caseById[indName];
						tc.mId = indName;
						tc.mControlFile = filePath;
						if (tc.mTask.isEmpty()) {
							tc.mTask = "consistency";
						}
					} else if (className.endsWith("SatisfiabilityTest")) {
						CConformanceTestCase& tc = caseById[indName];
						tc.mId = indName;
						tc.mControlFile = filePath;
						if (tc.mTask.isEmpty()) {
							tc.mTask = "satisfiability";
						}
					} else if (className.endsWith("ClassificationTest")) {
						CConformanceTestCase& tc = caseById[indName];
						tc.mId = indName;
						tc.mControlFile = filePath;
						if (tc.mTask.isEmpty()) {
							tc.mTask = "classification";
						}
					}
				}

				// 2) DataPropertyAssertion(kt:xxx, individual, literal) -> fields.
				QDomNodeList dataAssertions = root.elementsByTagName("DataPropertyAssertion");
				for (int i = 0; i < dataAssertions.count(); ++i) {
					QDomElement da = dataAssertions.at(i).toElement();
					QDomElement prop = da.firstChildElement("DataProperty");
					QDomElement ind = da.firstChildElement("NamedIndividual");
					QDomElement lit = da.firstChildElement("Literal");
					if (prop.isNull() || ind.isNull() || lit.isNull()) {
						continue;
					}
					QString propName = prop.attribute("abbreviatedIRI");
					if (propName.isEmpty()) {
						propName = prop.attribute("IRI");
					}
					QString indName = ind.attribute("abbreviatedIRI");
					if (indName.isEmpty()) {
						indName = ind.attribute("IRI");
					}
					QString value = lit.text().trimmed();

					if (!caseById.contains(indName)) {
						// Data for an individual whose test kind was not seen yet -> create it.
						CConformanceTestCase tc;
						tc.mId = indName;
						tc.mControlFile = filePath;
						caseById.insert(indName, tc);
					}
					CConformanceTestCase& tc = caseById[indName];

					if (propName.endsWith("task")) {
						tc.mTask = value;
					} else if (propName.endsWith("usesOntology")) {
						// Relative filename -> resolve to absolute against the case directory.
						QString resolved = QDir::isAbsolutePath(value) ? value : caseDir.absoluteFilePath(value);
						tc.mOntologyFile = QDir::cleanPath(resolved);
					} else if (propName.endsWith("expectedFile")) {
						// Relative filename -> resolve to absolute against the case directory.
						QString resolved = QDir::isAbsolutePath(value) ? value : caseDir.absoluteFilePath(value);
						tc.mExpectedFile = QDir::cleanPath(resolved);
					} else if (propName.endsWith("expectedResult")) {
						tc.mExpectedResult = value;
					} else if (propName.endsWith("testClass")) {
						tc.mTestClass = value;
					} else if (propName.endsWith("timeoutSeconds")) {
						tc.mTimeoutSeconds = value.toLongLong();
					} else if (propName.endsWith("flaky")) {
						if (value.compare("true", Qt::CaseInsensitive) == 0) {
							tc.mKnownFailure = true;
						}
					}
				}

				// 3) ObjectPropertyAssertion(kt:knownFailure, individual, issue) -> KNOWN.
				QDomNodeList objAssertions = root.elementsByTagName("ObjectPropertyAssertion");
				for (int i = 0; i < objAssertions.count(); ++i) {
					QDomElement oa = objAssertions.at(i).toElement();
					QDomElement prop = oa.firstChildElement("ObjectProperty");
					QDomElement ind = oa.firstChildElement("NamedIndividual");
					if (prop.isNull() || ind.isNull()) {
						continue;
					}
					QString propName = prop.attribute("abbreviatedIRI");
					if (propName.isEmpty()) {
						propName = prop.attribute("IRI");
					}
					QString indName = ind.attribute("abbreviatedIRI");
					if (indName.isEmpty()) {
						indName = ind.attribute("IRI");
					}
					if (propName.endsWith("knownFailure") && caseById.contains(indName)) {
						caseById[indName].mKnownFailure = true;
					} else if (propName.endsWith("onChannel") && caseById.contains(indName)) {
						// Interface the test runs on, e.g. kt:CommandLine, kt:OWLlink.
						QDomElement target = ind.nextSiblingElement("NamedIndividual");
						if (!target.isNull()) {
							QString channel = target.attribute("abbreviatedIRI");
							if (channel.isEmpty()) {
								channel = target.attribute("IRI");
							}
							if (!channel.isEmpty()) {
								int colon = channel.indexOf(':');
								if (colon >= 0) {
									channel = channel.mid(colon + 1);
								}
								caseById[indName].mInterface = channel;
							}
						}
					}
				}

				// Keep complete test cases (consistency and satisfiability tests).
				for (CConformanceTestCase& tc : caseById) {
					bool ok = false;
					if (tc.mTask == "classification" && !tc.mOntologyFile.isEmpty() && !tc.mExpectedFile.isEmpty()) {
						// Classification compares against an expected taxonomy file ("match").
						tc.mExpectedResult = "match";
						ok = true;
					} else if (tc.mTask == "consistency" && !tc.mOntologyFile.isEmpty() && !tc.mExpectedResult.isEmpty()) {
						ok = true;
					} else if (tc.mTask == "satisfiability" && !tc.mOntologyFile.isEmpty()
							&& !tc.mExpectedResult.isEmpty() && !tc.mTestClass.isEmpty()) {
						ok = true;
					} else if (tc.mTask == "owllink-boolean" && !tc.mOntologyFile.isEmpty()
							&& !tc.mExpectedResult.isEmpty()) {
						ok = true;
					}
					if (ok) {
						if (tc.mInterface.isEmpty()) {
							// Default interface derived from the task/command used to run it.
							tc.mInterface = (tc.mTask == "owllink-boolean") ? QString("OWLlink") : QString("CommandLine");
						}
						mTestCases.append(tc);
					} else {
						LOG(INFO, getLogDomain(), logTr("Skipping incomplete/unsupported test '%1' in '%2' (supported: consistency, satisfiability, classification, owllink-boolean).").arg(tc.mId).arg(filePath), this);
					}
				}
			}


			QString CConformanceRunner::runTask(const CConformanceTestCase& testCase, qint64 timeoutMs) {
				QProcess process;
				QStringList args;
				QString responseFile;
				if (testCase.mTask == "satisfiability") {
					args << "satisfiability" << "-i" << testCase.mOntologyFile << "-x" << testCase.mTestClass;
				} else if (testCase.mTask == "owllink-boolean") {
					// OWLlink file channel: run the request, write the response to a temp file.
					responseFile = testCase.mOntologyFile + ".response.xml";
					args << "owllinkfile" << "-i" << testCase.mOntologyFile << "-o" << responseFile;
				} else if (testCase.mTask == "classification") {
					// Run classification and write the taxonomy to a temp file that is then
					// compared semantically (order-independent) against the expected result.
					responseFile = testCase.mOntologyFile + ".classification-result.owl.xml";
					args << "classification" << "-i" << testCase.mOntologyFile << "-o" << responseFile;
				} else {
					args << "consistency" << "-i" << testCase.mOntologyFile;
				}
				process.start(resolveBinaryFile(), args);
				if (!process.waitForStarted(10000)) {
					return QString("error");
				}
				if (!process.waitForFinished(static_cast<int>(timeoutMs))) {
					process.kill();
					process.waitForFinished(2000);
					return QString("timeout");
				}
				QString out = QString::fromUtf8(process.readAllStandardOutput());
				out += QString::fromUtf8(process.readAllStandardError());

				if (testCase.mTask == "classification") {
					// Compare the produced taxonomy against the expected result file.
					if (QFile::exists(responseFile) && QFile::exists(testCase.mExpectedFile)) {
						QString detail;
						bool equivalent = CClassificationResultComparator::compareClassificationFiles(responseFile, testCase.mExpectedFile, detail);
						QFile::remove(responseFile);
						if (equivalent) {
							return QString("match");
						}
						return QString("mismatch") + (detail.isEmpty() ? QString() : QString(" (") + detail + QString(")"));
					}
					QFile::remove(responseFile);
					return QString("error");
				}

				if (testCase.mTask == "owllink-boolean") {
					// Read the first BooleanResponse from the response file.
					QFile rf(responseFile);
					QString resp;
					if (rf.open(QIODevice::ReadOnly | QIODevice::Text)) {
						resp = QString::fromUtf8(rf.readAll());
						rf.close();
					}
					QFile::remove(responseFile);
					int idx = resp.indexOf("<BooleanResponse");
					if (idx < 0) {
						return QString("error");
					}
					QString frag = resp.mid(idx, 60);
					if (frag.contains("result=\"true\"")) {
						return QString("true");
					}
					if (frag.contains("result=\"false\"")) {
						return QString("false");
					}
					return QString("error");
				}

				if (testCase.mTask == "satisfiability") {
					// Order matters: "not satisfiable" contains "satisfiable".
					if (out.contains("is not satisfiable")) {
						return QString("unsatisfiable");
					}
					if (out.contains("is satisfiable")) {
						return QString("satisfiable");
					}
					return QString("error");
				}

				// consistency: "inconsistent" contains "consistent" as a substring.
				if (out.contains("is inconsistent")) {
					return QString("inconsistent");
				}
				if (out.contains("is consistent")) {
					return QString("consistent");
				}
				return QString("error");
			}


			CConformanceTestResult CConformanceRunner::executeTestCase(const CConformanceTestCase& testCase) {
				CConformanceTestResult result;
				result.mId = testCase.mId;
				result.mTask = testCase.mTask;
				result.mInterface = testCase.mInterface;
				result.mExpected = testCase.mExpectedResult;

				qint64 timeoutSec = testCase.mTimeoutSeconds > 0 ? testCase.mTimeoutSeconds : mTimeoutSeconds;
				qint64 timeoutMs = timeoutSec * 1000;

				QElapsedTimer timer;
				timer.start();

				QString firstOutcome;
				bool sawTimeout = false;
				bool allEqual = true;
				QString actual;
				for (qint64 r = 0; r < mRepeatCount; ++r) {
					actual = runTask(testCase, timeoutMs);
					if (actual == "timeout") {
						sawTimeout = true;
					}
					if (r == 0) {
						firstOutcome = actual;
					} else if (actual != firstOutcome) {
						allEqual = false;
					}
				}
				result.mDurationMs = timer.elapsed();
				result.mActual = actual;

				// Flaky: repeated runs with diverging outcome.
				if (!allEqual) {
					if (testCase.mKnownFailure) {
						result.mOutcome = "KNOWN";
						result.mFailing = false;
					} else {
						result.mOutcome = "FLAKY";
						result.mFailing = true;
					}
					return result;
				}

				if (sawTimeout) {
					if (testCase.mKnownFailure) {
						result.mOutcome = "KNOWN";
						result.mFailing = false;
					} else {
						result.mOutcome = "TIMEOUT";
						result.mFailing = true;
					}
					return result;
				}

				bool correct = (actual == testCase.mExpectedResult);
				if (correct && !testCase.mKnownFailure) {
					result.mOutcome = "PASS";
					result.mFailing = false;
				} else if (!correct && !testCase.mKnownFailure) {
					result.mOutcome = "FAIL";
					result.mFailing = true;
				} else if (!correct && testCase.mKnownFailure) {
					result.mOutcome = "KNOWN";
					result.mFailing = false;
				} else { // correct && knownFailure -> fixed, needs cleanup
					result.mOutcome = "FIXED";
					result.mFailing = true;
				}
				return result;
			}


			void CConformanceRunner::writeResultOntology(const QList<CConformanceTestResult>& results) {
				if (mOutputFile.isEmpty()) {
					return;
				}
				QFile file(mOutputFile);
				if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
					LOG(ERROR, getLogDomain(), logTr("Cannot write result ontology '%1'.").arg(mOutputFile), this);
					return;
				}
				QTextStream ts(&file);
				ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
				ts << "<Ontology xmlns=\"http://www.w3.org/2002/07/owl#\"\n";
				ts << "          ontologyIRI=\"http://konclude.test/results\">\n";
				ts << "  <Prefix name=\"kt\" IRI=\"http://konclude.test/vocabulary#\"/>\n";
				int n = 0;
				for (const CConformanceTestResult& res : results) {
					QString resId = QString("res:r-%1").arg(n++, 4, 10, QChar('0'));
					ts << "  <ClassAssertion><Class abbreviatedIRI=\"kt:TestResult\"/>"
					   << "<NamedIndividual abbreviatedIRI=\"" << resId << "\"/></ClassAssertion>\n";
					ts << "  <ObjectPropertyAssertion><ObjectProperty abbreviatedIRI=\"kt:resultOf\"/>"
					   << "<NamedIndividual abbreviatedIRI=\"" << resId << "\"/>"
					   << "<NamedIndividual abbreviatedIRI=\"" << res.mId << "\"/></ObjectPropertyAssertion>\n";
					ts << "  <DataPropertyAssertion><DataProperty abbreviatedIRI=\"kt:outcome\"/>"
					   << "<NamedIndividual abbreviatedIRI=\"" << resId << "\"/>"
					   << "<Literal>" << res.mOutcome << "</Literal></DataPropertyAssertion>\n";
					ts << "  <DataPropertyAssertion><DataProperty abbreviatedIRI=\"kt:durationMs\"/>"
					   << "<NamedIndividual abbreviatedIRI=\"" << resId << "\"/>"
					   << "<Literal>" << res.mDurationMs << "</Literal></DataPropertyAssertion>\n";
				}
				ts << "</Ontology>\n";
				file.close();
			}

			int CConformanceRunner::run() {

				LOG(INFO, getLogDomain(), logTr("Starting conformance run in '%1'.").arg(mInputDirectory), this);

				QStringList controlFiles = discoverControlOntologies();
				LOG(INFO, getLogDomain(), logTr("Discovered %1 control ontologies (*.kt.owl.xml).").arg(controlFiles.count()), this);
				for (const QString& cf : controlFiles) {
					parseControlOntology(cf);
				}

				LOG(INFO, getLogDomain(), logTr("Collected %1 executable test case(s).").arg(mTestCases.count()), this);

				// Report how many test workers will run the suite.
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatWorkersLine((int)mJobCount, mTestCases.count())), this);

				QList<CConformanceTestResult> results;
				QList<CConformanceTestResult> failingResults;
				QHash<QString, int> totalOutcomeHash;
				QHash<QString, QHash<QString, int> > typeOutcomeHash;
				QHash<QString, QHash<QString, int> > interfaceOutcomeHash;

				qint64 runStartMSecs = QDateTime::currentMSecsSinceEpoch();
				qint64 lastProgressMSecs = runStartMSecs;
				qint64 maxDurationMSecs = 0;
				for (const CConformanceTestCase& tc : mTestCases) {
					CConformanceTestResult res = executeTestCase(tc);
					results.append(res);
					totalOutcomeHash[res.mOutcome]++;
					typeOutcomeHash[res.mTask][res.mOutcome]++;
					interfaceOutcomeHash[res.mInterface][res.mOutcome]++;
					if (res.mDurationMs > maxDurationMSecs) {
						maxDurationMSecs = res.mDurationMs;
					}

					// Periodic progress/estimated-remaining-time report (configurable via the
					// progress interval; a value of 0 disables it).
					if (mProgressIntervalSeconds > 0) {
						qint64 nowMSecs = QDateTime::currentMSecsSinceEpoch();
						if (nowMSecs - lastProgressMSecs >= mProgressIntervalSeconds * 1000) {
							lastProgressMSecs = nowMSecs;
							qint64 elapsedMSecs = nowMSecs - runStartMSecs;
							int completedCount = results.count();
							qint64 avgMSecsPerTest = completedCount > 0 ? elapsedMSecs / completedCount : 0;
							qint64 remainingMSecs = avgMSecsPerTest * (mTestCases.count() - completedCount);
							LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatProgressLine(completedCount, mTestCases.count(), elapsedMSecs, remainingMSecs)), this);
						}
					}

					QString line = QString("[%1] %2  (%3ms)")
						.arg(res.mOutcome, -7).arg(res.mId).arg(res.mDurationMs);
					if (res.mFailing) {
						line += QString("  -- expected: %1, got: %2").arg(res.mExpected).arg(res.mActual);
						failingResults.append(res);
						// Failing tests are reported as log errors so they stand out.
						LOG(ERROR, getLogDomain(), logTr("%1").arg(line), this);
					} else {
						LOG(INFO, getLogDomain(), logTr("%1").arg(line), this);
					}
				}

				// Timing directly after the last test and before the evaluation breakdown.
				qint64 totalMSecs = QDateTime::currentMSecsSinceEpoch() - runStartMSecs;
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatTotalTimeLine(totalMSecs, mTestCases.count(), maxDurationMSecs)), this);

				// Overall summary: header line, then each outcome count on its own line.
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatSummaryHeader(controlFiles.count(), mTestCases.count())), this);
				for (const QString& outcomeLine : CConformanceReporter::formatOutcomeLines(totalOutcomeHash, true)) {
					LOG(INFO, getLogDomain(), logTr("%1").arg(QString("  ") + outcomeLine), this);
				}

				// Per-test-type breakdown: one header + outcome lines per test type.
				QStringList taskNames = typeOutcomeHash.keys();
				taskNames.sort();
				for (const QString& taskName : taskNames) {
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatCategoryHeader("test type", taskName, typeOutcomeHash.value(taskName))), this);
					for (const QString& outcomeLine : CConformanceReporter::formatOutcomeLines(typeOutcomeHash.value(taskName), false)) {
						LOG(INFO, getLogDomain(), logTr("%1").arg(QString("  ") + outcomeLine), this);
					}
				}

				// Per-interface breakdown: one header + outcome lines per interface.
				QStringList interfaceNames = interfaceOutcomeHash.keys();
				interfaceNames.sort();
				for (const QString& interfaceName : interfaceNames) {
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatCategoryHeader("interface", interfaceName, interfaceOutcomeHash.value(interfaceName))), this);
					for (const QString& outcomeLine : CConformanceReporter::formatOutcomeLines(interfaceOutcomeHash.value(interfaceName), false)) {
						LOG(INFO, getLogDomain(), logTr("%1").arg(QString("  ") + outcomeLine), this);
					}
				}

				// Clear completion message (also covers the case with no failures).
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatFinishedLine(failingResults.count(), mTestCases.count())), this);

				// List all failing tests again at the end, as errors.
				if (!failingResults.isEmpty()) {
					LOG(ERROR, getLogDomain(), logTr("The following tests failed:"), this);
					for (const CConformanceTestResult& failedRes : failingResults) {
						LOG(ERROR, getLogDomain(), logTr("%1").arg(QString("  ") + CConformanceReporter::formatFailureLine(failedRes.mId, failedRes.mOutcome, failedRes.mExpected, failedRes.mActual)), this);
					}
				}

				writeResultOntology(results);

				// An individual test counts as a red run if it FAILed, FIXED, TIMEOUT, or FLAKY.
				int failures = totalOutcomeHash.value("FAIL", 0) + totalOutcomeHash.value("FIXED", 0)
								+ totalOutcomeHash.value("TIMEOUT", 0) + totalOutcomeHash.value("FLAKY", 0);
				return failures;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
