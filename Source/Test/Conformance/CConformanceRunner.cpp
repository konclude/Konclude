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

#include <QScopedPointer>
#include <QThread>

#include "CConformanceTaskHandler.h"
#include "CConformanceTaskHandlerFactory.h"
#include "CConformanceResultWriter.h"
#include "CConformanceResultWriterFactory.h"


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
			void CConformanceRunner::setConfigFile(const QString& file) { mConfigFile = file; }
			void CConformanceRunner::setJobCount(qint64 jobs) { mJobCount = jobs > 0 ? jobs : 1; }
			void CConformanceRunner::setRepeatCount(qint64 repeat) { mRepeatCount = repeat > 0 ? repeat : 1; }
			void CConformanceRunner::setTimeoutSeconds(qint64 seconds) { mTimeoutSeconds = seconds > 0 ? seconds : 60; }
			void CConformanceRunner::setProgressIntervalSeconds(qint64 seconds) { mProgressIntervalSeconds = seconds >= 0 ? seconds : 0; }


			QString CConformanceRunner::resolveBinaryFile() {
				if (!mBinaryFile.isEmpty()) {
					return mBinaryFile;
				}
				// Default: the currently running binary itself.
				QString applicationFile = QCoreApplication::applicationFilePath();
				if (!applicationFile.isEmpty()) {
					return applicationFile;
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

				QStringList pendingDirectories;
				pendingDirectories.append(baseDir.absolutePath());
				while (!pendingDirectories.isEmpty()) {
					QDir directory(pendingDirectories.takeFirst());

					QStringList nameFilters;
					nameFilters << "*.kt.owl.xml";
					QFileInfoList fileInfos = directory.entryInfoList(nameFilters, QDir::Files, QDir::Name);
					for (const QFileInfo& fileInfo : fileInfos) {
						controlFiles.append(fileInfo.absoluteFilePath());
					}

					QFileInfoList subDirectories = directory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
					for (const QFileInfo& subDirectory : subDirectories) {
						pendingDirectories.append(subDirectory.absoluteFilePath());
					}
				}

				controlFiles.sort();
				return controlFiles;
			}


			QString CConformanceRunner::elementIRI(const QDomElement& element) {
				if (element.isNull()) {
					return QString();
				}
				QString abbreviatedIRI = element.attribute("abbreviatedIRI");
				if (!abbreviatedIRI.isEmpty()) {
					return abbreviatedIRI;
				}
				return element.attribute("IRI");
			}


			CConformanceTestCase& CConformanceRunner::testCaseFor(QHash<QString, CConformanceTestCase>& caseById, const QString& id, const QString& controlFile, const QString& suiteId) {
				if (!caseById.contains(id)) {
					CConformanceTestCase testCase;
					testCase.mId = id;
					testCase.mControlFile = controlFile;
					testCase.mSuiteId = suiteId;
					caseById.insert(id, testCase);
				}
				return caseById[id];
			}


			void CConformanceRunner::appendUnique(QStringList& values, const QString& value) {
				if (!values.contains(value)) {
					values.append(value);
				}
			}

			void CConformanceRunner::parseControlOntology(const QString& filePath) {
				QFile file(filePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					LOG(ERROR, getLogDomain(), logTr("Cannot open control ontology '%1'.").arg(filePath), this);
					return;
				}

				QDomDocument document;
				QString errorMessage;
				int errorLine = 0;
				int errorColumn = 0;
				if (!document.setContent(&file, &errorMessage, &errorLine, &errorColumn)) {
					file.close();
					LOG(ERROR, getLogDomain(), logTr("Cannot parse control ontology '%1' (%2 at line %3).").arg(filePath).arg(errorMessage).arg(errorLine), this);
					return;
				}
				file.close();

				// The directory of the control ontology relative to the input root is
				// the suite "folder"; the ontology may name the suite explicitly.
				QDir inputDirectory(QFileInfo(mInputDirectory).absoluteFilePath());
				QString relativeDirectory = QDir::cleanPath(inputDirectory.relativeFilePath(QFileInfo(filePath).absolutePath()));
				if (relativeDirectory == ".") {
					relativeDirectory = QString();
				}

				QDir caseDirectory = QFileInfo(filePath).absoluteDir();
				QDomElement root = document.documentElement();

				QString suiteId;
				parseSuiteDeclaration(root, filePath, relativeDirectory, suiteId);
				parseRunConfigDeclarations(root);

				QHash<QString, CConformanceTestCase> caseById;
				parseTaskClassAssertions(root, filePath, suiteId, caseById);
				parseDataPropertyAssertions(root, filePath, caseDirectory, suiteId, caseById);
				parseObjectPropertyAssertions(root, filePath, caseDirectory, suiteId, caseById);

				for (CConformanceTestCase& testCase : caseById) {
					if (finalizeTestCase(testCase)) {
						mTestCases.append(testCase);
					} else {
						LOG(INFO, getLogDomain(), logTr("Skipping incomplete or unsupported test '%1' in '%2'.").arg(testCase.mId).arg(filePath), this);
					}
				}
			}


			void CConformanceRunner::parseSuiteDeclaration(const QDomElement& root, const QString& filePath, const QString& relativeDirectory, QString& suiteId) {
				QString directoryPart = relativeDirectory.isEmpty() ? QString("root") : relativeDirectory;
				QString derivedSuiteId = QString("kt:suite-%1").arg(sanitizeFileName(directoryPart));

				// ClassAssertion(kt:TestSuite, kt:suite-...) declares the suite of this file.
				QString declaredSuiteId;
				QDomNodeList classAssertions = root.elementsByTagName("ClassAssertion");
				for (int i = 0; i < classAssertions.count(); ++i) {
					QDomElement assertion = classAssertions.at(i).toElement();
					QString className = elementIRI(assertion.firstChildElement("Class"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					if (individualName.isEmpty() || QString::compare(CConformanceTask::localName(className), "TestSuite", Qt::CaseInsensitive) != 0) {
						continue;
					}
					if (!declaredSuiteId.isEmpty() && declaredSuiteId != individualName) {
						LOG(WARN, getLogDomain(), logTr("Control ontology '%1' declares more than one suite; using '%2'.").arg(filePath).arg(declaredSuiteId), this);
						continue;
					}
					declaredSuiteId = individualName;
				}
				suiteId = declaredSuiteId.isEmpty() ? derivedSuiteId : declaredSuiteId;

				// Suite metadata: kt:suiteName and kt:suiteDirectory.
				QString suiteName;
				QString declaredDirectory;
				QDomNodeList dataAssertions = root.elementsByTagName("DataPropertyAssertion");
				for (int i = 0; i < dataAssertions.count(); ++i) {
					QDomElement assertion = dataAssertions.at(i).toElement();
					QString propertyName = elementIRI(assertion.firstChildElement("DataProperty"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					QDomElement literal = assertion.firstChildElement("Literal");
					if (individualName != suiteId || literal.isNull()) {
						continue;
					}
					QString value = literal.text().trimmed();
					if (propertyName.endsWith("suiteName")) {
						suiteName = value;
					} else if (propertyName.endsWith("suiteDirectory")) {
						declaredDirectory = QDir::cleanPath(value);
					}
				}

				if (!declaredDirectory.isEmpty() && declaredDirectory != QDir::cleanPath(directoryPart)) {
					LOG(WARN, getLogDomain(), logTr("Suite '%1' declares directory '%2' but its control ontology is in '%3'; using the file location.").arg(suiteId).arg(declaredDirectory).arg(directoryPart), this);
				}

				CConformanceTestSuite suite = mSuites.value(suiteId);
				suite.mId = suiteId;
				suite.mDirectory = directoryPart;
				if (!suiteName.isEmpty()) {
					suite.mName = suiteName;
				}
				if (suite.mName.isEmpty()) {
					suite.mName = directoryPart;
				}
				mSuites.insert(suiteId, suite);
				mSuiteIdByDirectory.insert(directoryPart, suiteId);
			}

			void CConformanceRunner::parseRunConfigDeclarations(const QDomElement& root) {
				// ClassAssertion(kt:RunConfig, kt:run-...) declares an execution configuration.
				QDomNodeList classAssertions = root.elementsByTagName("ClassAssertion");
				for (int i = 0; i < classAssertions.count(); ++i) {
					QDomElement assertion = classAssertions.at(i).toElement();
					QString className = elementIRI(assertion.firstChildElement("Class"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					if (individualName.isEmpty() || QString::compare(CConformanceTask::localName(className), "RunConfig", Qt::CaseInsensitive) != 0) {
						continue;
					}
					CConformanceRunConfig runConfig = mRunConfigs.value(individualName);
					runConfig.mId = individualName;
					mRunConfigs.insert(individualName, runConfig);
				}

				// kt:timeoutSeconds / kt:repeatCount / kt:runConfigName on those individuals.
				QDomNodeList dataAssertions = root.elementsByTagName("DataPropertyAssertion");
				for (int i = 0; i < dataAssertions.count(); ++i) {
					QDomElement assertion = dataAssertions.at(i).toElement();
					QString propertyName = elementIRI(assertion.firstChildElement("DataProperty"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					QDomElement literal = assertion.firstChildElement("Literal");
					if (individualName.isEmpty() || literal.isNull() || !mRunConfigs.contains(individualName)) {
						continue;
					}

					CConformanceRunConfig runConfig = mRunConfigs.value(individualName);
					QString value = literal.text().trimmed();
					if (propertyName.endsWith("timeoutSeconds")) {
						runConfig.mTimeoutSeconds = value.toLongLong();
					} else if (propertyName.endsWith("repeatCount")) {
						runConfig.mRepeatCount = value.toLongLong();
					} else if (propertyName.endsWith("runConfigName")) {
						runConfig.mName = value;
					}
					mRunConfigs.insert(individualName, runConfig);
				}
			}

			void CConformanceRunner::parseTaskClassAssertions(const QDomElement& root, const QString& filePath, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById) {
				// ClassAssertion(kt:ConsistencyTest, individual) and the sibling markers
				// declare the task kind of a test individual.
				QDomNodeList classAssertions = root.elementsByTagName("ClassAssertion");
				for (int i = 0; i < classAssertions.count(); ++i) {
					QDomElement assertion = classAssertions.at(i).toElement();
					QString className = elementIRI(assertion.firstChildElement("Class"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					if (className.isEmpty() || individualName.isEmpty()) {
						continue;
					}

					CConformanceTask::TYPE taskType;
					if (!CConformanceTask::typeFromTestClassName(className, taskType)) {
						continue;
					}

					CConformanceTestCase& testCase = testCaseFor(caseById, individualName, filePath, suiteId);
					testCase.mTask = taskType;
					testCase.mHasTask = true;
				}
			}


			void CConformanceRunner::parseDataPropertyAssertions(const QDomElement& root, const QString& filePath, const QDir& caseDir, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById) {
				// DataPropertyAssertion(kt:<name>, individual, literal) carries the test data.
				QDomNodeList dataAssertions = root.elementsByTagName("DataPropertyAssertion");
				for (int i = 0; i < dataAssertions.count(); ++i) {
					QDomElement assertion = dataAssertions.at(i).toElement();
					QString propertyName = elementIRI(assertion.firstChildElement("DataProperty"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					QDomElement literal = assertion.firstChildElement("Literal");
					if (propertyName.isEmpty() || individualName.isEmpty() || literal.isNull()) {
						continue;
					}
					// Suite and run configuration individuals have their own properties.
					if (mSuites.contains(individualName) || mRunConfigs.contains(individualName)) {
						continue;
					}

					QString value = literal.text().trimmed();
					CConformanceTestCase& testCase = testCaseFor(caseById, individualName, filePath, suiteId);

					if (propertyName.endsWith("task")) {
						CConformanceTask::TYPE taskType;
						if (CConformanceTask::typeFromString(value, taskType)) {
							testCase.mTask = taskType;
							testCase.mHasTask = true;
						} else {
							LOG(WARN, getLogDomain(), logTr("Unknown task '%1' in '%2'; test '%3' is skipped.").arg(value).arg(filePath).arg(individualName), this);
						}
					} else if (propertyName.endsWith("usesOntology")) {
						testCase.mOntologyFile = resolveCaseFilePath(caseDir, value);
					} else if (propertyName.endsWith("usesDataset")) {
						// Ontology a SPARQL test loads before the query is answered.
						testCase.mDatasetFile = resolveCaseFilePath(caseDir, value);
					} else if (propertyName.endsWith("expectedFile")) {
						testCase.mExpectedFile = resolveCaseFilePath(caseDir, value);
					} else if (propertyName.endsWith("expectedResult")) {
						// Repeated assertions describe one expected answer per response;
						// OWLlink requests may produce several responses.
						testCase.mExpectedResultTexts.append(value);
					} else if (propertyName.endsWith("testClass")) {
						testCase.mTestClass = value;
					} else if (propertyName.endsWith("timeoutSeconds")) {
						LOG(WARN, getLogDomain(), logTr("kt:timeoutSeconds on test '%1' in '%2' is ignored; declare a kt:RunConfig and assign it with kt:usesTestRunConfig.").arg(individualName).arg(filePath), this);
					} else if (propertyName.endsWith("flaky")) {
						testCase.mKnownFailure = (QString::compare(value, "true", Qt::CaseInsensitive) == 0);
					}
				}
			}


			void CConformanceRunner::parseObjectPropertyAssertions(const QDomElement& root, const QString& filePath, const QDir& caseDir, const QString& suiteId, QHash<QString, CConformanceTestCase>& caseById) {
				// ObjectPropertyAssertion(kt:<name>, individual, <target individual>)
				QDomNodeList objectAssertions = root.elementsByTagName("ObjectPropertyAssertion");
				for (int i = 0; i < objectAssertions.count(); ++i) {
					QDomElement assertion = objectAssertions.at(i).toElement();
					QString propertyName = elementIRI(assertion.firstChildElement("ObjectProperty"));
					QString individualName = elementIRI(assertion.firstChildElement("NamedIndividual"));
					if (propertyName.isEmpty() || individualName.isEmpty()) {
						continue;
					}

					QDomElement target = assertion.firstChildElement("NamedIndividual").nextSiblingElement("NamedIndividual");
					if (propertyName.endsWith("knownFailure")) {
						if (caseById.contains(individualName)) {
							caseById[individualName].mKnownFailure = true;
						}
					} else if (propertyName.endsWith("memberOfSuite") && !target.isNull()) {
						// Suites are first-class individuals with their own description and
						// configuration links; a test points at its suite with an object
						// property instead of being bound to the folder implicitly.
						CConformanceTestCase& testCase = testCaseFor(caseById, individualName, filePath, suiteId);
						testCase.mSuiteId = elementIRI(target);
					} else if (propertyName.endsWith("usesTestRunConfig") && !target.isNull()) {
						// Execution configuration for a test or for a whole suite.
						appendUnique(mRunConfigAssignments[individualName], elementIRI(target));
					} else if (propertyName.endsWith("usesTestSystemConfig") && !target.isNull()) {
						// Reasoner configuration ('-c') for a test or for a whole suite.
						appendUnique(mSystemConfigAssignments[individualName], resolveCaseFilePath(caseDir, elementIRI(target)));
					} else if (propertyName.endsWith("onChannel") && !target.isNull()) {
						// Transport the test runs on: kt:CommandLine, kt:OWLlink,
						// kt:OWLlinkServer, kt:SPARQL or kt:SPARQLServer.
						CConformanceChannel::TYPE channel;
						if (CConformanceChannel::typeFromOntologyName(elementIRI(target), channel)) {
							CConformanceTestCase& testCase = testCaseFor(caseById, individualName, filePath, suiteId);
							testCase.mChannel = channel;
							testCase.mHasChannel = true;
						} else {
							LOG(WARN, getLogDomain(), logTr("Unknown channel '%1' for test '%2' in '%3'.").arg(elementIRI(target)).arg(individualName).arg(filePath), this);
						}
					}
				}
			}


			QString CConformanceRunner::resolveCaseFilePath(const QDir& caseDir, const QString& path) {
				if (path.isEmpty() || QDir::isAbsolutePath(path)) {
					return path;
				}
				return QDir::cleanPath(caseDir.absoluteFilePath(path));
			}


			bool CConformanceRunner::finalizeTestCase(CConformanceTestCase& testCase) {
				if (!testCase.mHasTask || testCase.mOntologyFile.isEmpty()) {
					return false;
				}

				testCase.mExpectedVerdicts.clear();
				// Classification always compares result files; OWLlink and SPARQL do so
				// when the description declares kt:expectedFile. Otherwise those tasks
				// compare the boolean answer(s) declared with kt:expectedResult.
				bool fileComparison = (testCase.mTask == CConformanceTask::TYPE::CLASSIFICATION)
						|| ((testCase.mTask == CConformanceTask::TYPE::OWLLINK || testCase.mTask == CConformanceTask::TYPE::SPARQL)
							&& !testCase.mExpectedFile.isEmpty());
				if (fileComparison) {
					if (testCase.mExpectedFile.isEmpty()) {
						return false;
					}
					testCase.mExpectedVerdicts.append(CConformanceTask::VERDICT::POSITIVE);
				} else {
					for (const QString& expectedText : testCase.mExpectedResultTexts) {
						CConformanceTask::VERDICT expectedVerdict;
						if (!CConformanceTask::verdictFromString(testCase.mTask, expectedText, expectedVerdict)) {
							return false;
						}
						testCase.mExpectedVerdicts.append(expectedVerdict);
					}
					if (testCase.mExpectedVerdicts.isEmpty()) {
						return false;
					}
					if (testCase.mTask != CConformanceTask::TYPE::OWLLINK && testCase.mExpectedVerdicts.count() > 1) {
						LOG(WARN, getLogDomain(), logTr("Test '%1' in '%2' declares %3 expected results; task '%4' uses only the first.")
								.arg(testCase.mId).arg(testCase.mControlFile).arg(testCase.mExpectedVerdicts.count())
								.arg(CConformanceTask::typeToString(testCase.mTask)), this);
						testCase.mExpectedVerdicts = testCase.mExpectedVerdicts.mid(0, 1);
					}
				}
				testCase.mExpectedVerdict = testCase.mExpectedVerdicts.first();

				if (testCase.mTask == CConformanceTask::TYPE::SATISFIABILITY && testCase.mTestClass.isEmpty()) {
					return false;
				}

				if (!testCase.mHasChannel) {
					testCase.mChannel = CConformanceChannel::typeDefaultForTask(testCase.mTask);
				}
				if (!CConformanceChannel::typeIsSupportedForTask(testCase.mChannel, testCase.mTask)) {
					LOG(WARN, getLogDomain(), logTr("Channel '%1' cannot execute task '%2' (test '%3' in '%4').")
							.arg(CConformanceChannel::typeToString(testCase.mChannel))
							.arg(CConformanceTask::typeToString(testCase.mTask))
							.arg(testCase.mId).arg(testCase.mControlFile), this);
					return false;
				}
				if (testCase.mChannel == CConformanceChannel::TYPE::SPARQL_SERVER && testCase.mDatasetFile.isEmpty()) {
					LOG(WARN, getLogDomain(), logTr("SPARQL server test '%1' in '%2' requires kt:usesDataset.").arg(testCase.mId).arg(testCase.mControlFile), this);
					return false;
				}
				testCase.mInterface = CConformanceChannel::typeToString(testCase.mChannel);
				return true;
			}


			CConformanceTestSuite CConformanceRunner::suiteFor(const QString& suiteId) const {
				if (mSuites.contains(suiteId)) {
					return mSuites.value(suiteId);
				}
				CConformanceTestSuite suite;
				suite.mId = suiteId;
				suite.mName = CConformanceTask::localName(suiteId);
				return suite;
			}


			CConformanceRunConfig CConformanceRunner::resolveRunConfig(const QString& runConfigId) {
				CConformanceRunConfig runConfig;
				if (mRunConfigs.contains(runConfigId)) {
					runConfig = mRunConfigs.value(runConfigId);
				} else if (runConfigId != QString("default")) {
					LOG(WARN, getLogDomain(), logTr("Unknown run configuration '%1'; using the built-in default.").arg(runConfigId), this);
				}

				if (runConfig.mId.isEmpty()) {
					runConfig.mId = QString("default");
					runConfig.mName = QString("default");
				}
				if (runConfig.mTimeoutSeconds <= 0) {
					runConfig.mTimeoutSeconds = mTimeoutSeconds;
				}
				if (runConfig.mRepeatCount <= 0) {
					runConfig.mRepeatCount = mRepeatCount;
				}
				return runConfig;
			}


			QStringList CConformanceRunner::effectiveRunConfigIds(const CConformanceTestCase& testCase) const {
				// Test-level assignments first, then the suite's, without duplicates.
				QStringList runConfigIds;
				for (const QString& runConfigId : mRunConfigAssignments.value(testCase.mId)) {
					appendUnique(runConfigIds, runConfigId);
				}
				for (const QString& runConfigId : mRunConfigAssignments.value(testCase.mSuiteId)) {
					appendUnique(runConfigIds, runConfigId);
				}
				if (runConfigIds.isEmpty()) {
					runConfigIds.append(QString("default"));
				}
				return runConfigIds;
			}


			QStringList CConformanceRunner::effectiveSystemConfigPaths(const CConformanceTestCase& testCase) const {
				// Test-level assignments first, then the suite's, without duplicates.
				QStringList systemConfigPaths;
				for (const QString& configPath : mSystemConfigAssignments.value(testCase.mId)) {
					appendUnique(systemConfigPaths, configPath);
				}
				for (const QString& configPath : mSystemConfigAssignments.value(testCase.mSuiteId)) {
					appendUnique(systemConfigPaths, configPath);
				}
				if (systemConfigPaths.isEmpty()) {
					// Suite-wide default of the runner, or "no -c" when none is configured.
					systemConfigPaths.append(mConfigFile);
				}
				return systemConfigPaths;
			}


			void CConformanceRunner::buildExecutionPlan() {
				mExecutions.clear();
				for (const CConformanceTestCase& testCase : mTestCases) {
					CConformanceTestSuite suite = suiteFor(testCase.mSuiteId);
					QStringList runConfigIds = effectiveRunConfigIds(testCase);
					QStringList systemConfigPaths = effectiveSystemConfigPaths(testCase);

					for (const QString& runConfigId : runConfigIds) {
						CConformanceRunConfig runConfig = resolveRunConfig(runConfigId);
						for (const QString& systemConfigPath : systemConfigPaths) {
							CConformanceExecution execution;
							execution.mTestCase = testCase;
							execution.mRunConfig = runConfig;
							execution.mSystemConfigPath = systemConfigPath;
							execution.mSuiteId = suite.mId;
							execution.mSuiteName = suite.getDisplayName();
							mExecutions.append(execution);
						}
					}
				}
			}

			QString CConformanceRunner::sanitizeFileName(const QString& name) const {
				QString cleanedName = name;
				for (int i = 0; i < cleanedName.size(); ++i) {
					const QChar c = cleanedName.at(i);
					if (!c.isLetterOrNumber() && c != '.' && c != '_' && c != '-') {
						cleanedName[i] = QChar('_');
					}
				}
				return cleanedName;
			}


			QString CConformanceRunner::resolveOutputDirectory() {
				// The per-test output files are companions of the result ontology and
				// live in a 'test-outputs' folder next to it. Without a result ontology
				// they go to a temporary directory instead of polluting the input tree.
				QString directoryPath;
				if (!mOutputFile.isEmpty()) {
					directoryPath = QDir::cleanPath(QFileInfo(mOutputFile).absolutePath() + "/test-outputs");
				} else {
					directoryPath = QDir::cleanPath(QDir::tempPath() + "/konclude-conformance");
				}

				QDir directory(directoryPath);
				if (!directory.exists() && !directory.mkpath(directoryPath)) {
					LOG(ERROR, getLogDomain(), logTr("Cannot create output directory '%1'.").arg(directoryPath), this);
					return QString();
				}
				return directoryPath;
			}


			QString CConformanceRunner::resolveExecutionFileBase(const CConformanceExecution& execution, qint64 runIndex) const {
				// The configuration combination is part of the name, so different
				// configurations of the same test do not overwrite each other's logs.
				QString fileName = sanitizeFileName(execution.mTestCase.mId);
				fileName += QString("-%1").arg(sanitizeFileName(execution.mRunConfig.getDisplayName()));
				fileName += QString("-%1").arg(sanitizeFileName(execution.getSystemConfigName()));
				if (execution.mRunConfig.mRepeatCount > 1) {
					fileName += QString("-r%1").arg(runIndex);
				}
				return fileName;
			}


			QString CConformanceRunner::resolveOutputLogFilePath(const CConformanceExecution& execution, qint64 runIndex) {
				if (mOutputFile.isEmpty()) {
					return QString();
				}
				QString directoryPath = resolveOutputDirectory();
				if (directoryPath.isEmpty()) {
					return QString();
				}
				return QDir(directoryPath).absoluteFilePath(resolveExecutionFileBase(execution, runIndex) + ".log");
			}


			QString CConformanceRunner::resolveResponseFilePath(const CConformanceExecution& execution, qint64 runIndex) {
				QString directoryPath = resolveOutputDirectory();
				if (directoryPath.isEmpty()) {
					return QString();
				}

				QString fileName = resolveExecutionFileBase(execution, runIndex);
				switch (execution.mTestCase.mTask) {
					case CConformanceTask::TYPE::CLASSIFICATION:
						fileName += "-classification.owl.xml";
						break;
					case CConformanceTask::TYPE::OWLLINK:
						fileName += "-response.xml";
						break;
					default:
						fileName += "-response.txt";
						break;
				}
				return QDir(directoryPath).absoluteFilePath(fileName);
			}

			CConformanceTestResult CConformanceRunner::executeTestRun(const CConformanceExecution& execution, int workerIndex, const QString& threadId) {
				CConformanceTestResult result;
				result.mWorkerIndex = workerIndex;
				result.mThreadId = threadId;
				result.mId = execution.mTestCase.mId;
				result.mTask = execution.mTestCase.mTask;
				result.mInterface = execution.mTestCase.mInterface;
				result.mExpectedVerdict = execution.mTestCase.mExpectedVerdict;
				result.mSuiteId = execution.mSuiteId;
				result.mSuiteName = execution.mSuiteName;
				result.mRunConfigId = execution.mRunConfig.mId;
				result.mRunConfigName = execution.mRunConfig.getDisplayName();
				result.mSystemConfigPath = execution.mSystemConfigPath;
				result.mSystemConfigName = execution.getSystemConfigName();

				qint64 timeoutMs = execution.mRunConfig.mTimeoutSeconds * 1000;

				QScopedPointer<CConformanceTaskHandler> handler(
						CConformanceTaskHandlerFactory::createHandler(execution.mTestCase.mTask, execution.mTestCase.mChannel, execution.mTestCase, resolveBinaryFile(), execution.mSystemConfigPath));
				if (handler.isNull()) {
					result.mOutcome = CConformanceOutcome::TYPE::FAIL;
					result.mExplanation = QString("no handler for task '%1'").arg(CConformanceTask::typeToString(execution.mTestCase.mTask));
					return result;
				}

				QElapsedTimer timer;
				timer.start();

				CConformanceTaskResult lastTaskResult;
				bool isFirstRun = true;
				bool sameObservation = true;
				bool sawTimeout = false;
				for (qint64 runIndex = 0; runIndex < execution.mRunConfig.mRepeatCount; ++runIndex) {
					QString logFilePath = resolveOutputLogFilePath(execution, runIndex);
					QString responseFilePath = resolveResponseFilePath(execution, runIndex);

					CConformanceTaskResult taskResult = handler->run(timeoutMs, logFilePath, responseFilePath);

					if (!logFilePath.isEmpty()) {
						result.mOutputLogFile = logFilePath;
					}
					if (!responseFilePath.isEmpty()) {
						result.mResponseFile = responseFilePath;
					}
					if (taskResult.mStatus == CConformanceTaskResult::STATUS::TIMEOUT) {
						sawTimeout = true;
					}
					if (!isFirstRun && !taskResult.isSameObservationAs(lastTaskResult)) {
						sameObservation = false;
					}
					lastTaskResult = taskResult;
					isFirstRun = false;
				}

				result.mDurationMs = timer.elapsed();
				result.mActualVerdict = lastTaskResult.mVerdict;
				result.mExplanation = lastTaskResult.mExplanation;

				if (!sameObservation) {
					result.mOutcome = execution.mTestCase.mKnownFailure ? CConformanceOutcome::TYPE::KNOWN : CConformanceOutcome::TYPE::FLAKY;
					return result;
				}
				if (sawTimeout) {
					result.mOutcome = execution.mTestCase.mKnownFailure ? CConformanceOutcome::TYPE::KNOWN : CConformanceOutcome::TYPE::TIMEOUT;
					return result;
				}
				if (lastTaskResult.mStatus == CConformanceTaskResult::STATUS::EXECUTION_ERROR) {
					result.mOutcome = execution.mTestCase.mKnownFailure ? CConformanceOutcome::TYPE::KNOWN : CConformanceOutcome::TYPE::FAIL;
					return result;
				}

				if (lastTaskResult.isMatched()) {
					result.mOutcome = execution.mTestCase.mKnownFailure ? CConformanceOutcome::TYPE::FIXED : CConformanceOutcome::TYPE::PASS;
				} else {
					result.mOutcome = execution.mTestCase.mKnownFailure ? CConformanceOutcome::TYPE::KNOWN : CConformanceOutcome::TYPE::FAIL;
				}
				return result;
			}


			void CConformanceRunner::writeResults(const QList<CConformanceTestResult>& results) {
				if (mOutputFile.isEmpty()) {
					return;
				}

				QScopedPointer<CConformanceResultWriter> writer(CConformanceResultWriterFactory::createWriter(mOutputFile));
				if (writer.isNull()) {
					LOG(ERROR, getLogDomain(), logTr("Unsupported result ontology format for '%1'.").arg(mOutputFile), this);
					return;
				}
				writer->writeResults(mOutputFile, results);
			}

			void CConformanceRunner::reportProgress(int completedCount, qint64 runStartMSecs, qint64& lastProgressMSecs) {
				if (mProgressIntervalSeconds <= 0) {
					return;
				}
				qint64 nowMSecs = QDateTime::currentMSecsSinceEpoch();
				if (nowMSecs - lastProgressMSecs < mProgressIntervalSeconds * 1000) {
					return;
				}
				lastProgressMSecs = nowMSecs;

				qint64 elapsedMSecs = nowMSecs - runStartMSecs;
				qint64 averageMSecsPerTest = completedCount > 0 ? elapsedMSecs / completedCount : 0;
				qint64 remainingMSecs = averageMSecsPerTest * (mExecutions.count() - completedCount);
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatProgressLine(completedCount, mExecutions.count(), elapsedMSecs, remainingMSecs)), this);
			}


			void CConformanceRunner::logOutcomeLines(const QMap<CConformanceOutcome::TYPE, int>& outcomeCounts, bool includeZeroOutcomes) {
				for (const QString& outcomeLine : CConformanceReporter::formatOutcomeLines(outcomeCounts, includeZeroOutcomes)) {
					LOG(INFO, getLogDomain(), logTr("%1").arg(QString("  ") + outcomeLine), this);
				}
			}

			int CConformanceRunner::run() {

				LOG(INFO, getLogDomain(), logTr("Starting conformance run in '%1'.").arg(mInputDirectory), this);

				QStringList controlFiles = discoverControlOntologies();
				LOG(INFO, getLogDomain(), logTr("Discovered %1 control ontologies (*.kt.owl.xml).").arg(controlFiles.count()), this);
				for (const QString& controlFile : controlFiles) {
					parseControlOntology(controlFile);
				}

				buildExecutionPlan();
				LOG(INFO, getLogDomain(), logTr("Collected %1 test case(s) in %2 suite(s), scheduled %3 test execution(s).")
						.arg(mTestCases.count()).arg(mSuites.count()).arg(mExecutions.count()), this);
				// Executions currently run one after another, so exactly one worker is
				// active even when more jobs were requested.
				const int effectiveWorkerCount = 1;
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatWorkersLine(effectiveWorkerCount, mExecutions.count())), this);

				// Which suite is executed with which configuration combinations.
				QMap<QString, QString> suiteNames;
				QMap<QString, QStringList> suiteRunConfigNames;
				QMap<QString, QStringList> suiteSystemConfigNames;
				for (const CConformanceExecution& execution : mExecutions) {
					suiteNames[execution.mSuiteId] = execution.mSuiteName;
					appendUnique(suiteRunConfigNames[execution.mSuiteId], execution.mRunConfig.getDisplayName());
					appendUnique(suiteSystemConfigNames[execution.mSuiteId], execution.getSystemConfigName());
				}
				for (const QString& suiteId : suiteNames.keys()) {
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatSuiteConfigurationLine(
							suiteNames.value(suiteId), suiteRunConfigNames.value(suiteId), suiteSystemConfigNames.value(suiteId))), this);
				}

				QList<CConformanceTestResult> results;
				QList<CConformanceTestResult> failingResults;
				QMap<CConformanceOutcome::TYPE, int> totalOutcomeCounts;
				QMap<CConformanceTask::TYPE, QMap<CConformanceOutcome::TYPE, int> > typeOutcomeCounts;
				QMap<QString, QMap<CConformanceOutcome::TYPE, int> > interfaceOutcomeCounts;
				QMap<QString, QMap<CConformanceOutcome::TYPE, int> > suiteOutcomeCounts;

				qint64 runStartMSecs = QDateTime::currentMSecsSinceEpoch();
				qint64 lastProgressMSecs = runStartMSecs;
				qint64 maxDurationMSecs = 0;

				QString threadId = QString("0x%1").arg((quintptr)QThread::currentThreadId(), 0, 16);
				int executionIndex = 0;
				for (const CConformanceExecution& execution : mExecutions) {
					++executionIndex;
					// Logged before the run so a hanging test is identifiable, including
					// the interface it uses and the worker/thread that runs it.
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatStartLine(
							execution, effectiveWorkerCount, threadId, executionIndex, mExecutions.count())), this);

					CConformanceTestResult result = executeTestRun(execution, effectiveWorkerCount, threadId);
					results.append(result);
					totalOutcomeCounts[result.mOutcome]++;
					typeOutcomeCounts[result.mTask][result.mOutcome]++;
					interfaceOutcomeCounts[result.mInterface][result.mOutcome]++;
					suiteOutcomeCounts[result.mSuiteId][result.mOutcome]++;
					if (result.mDurationMs > maxDurationMSecs) {
						maxDurationMSecs = result.mDurationMs;
					}

					reportProgress(results.count(), runStartMSecs, lastProgressMSecs);

					QString line = CConformanceReporter::formatResultLine(result);
					if (result.isFailing()) {
						failingResults.append(result);
						// Failing tests are reported as log errors so they stand out.
						LOG(ERROR, getLogDomain(), logTr("%1").arg(line), this);
					} else {
						LOG(INFO, getLogDomain(), logTr("%1").arg(line), this);
					}
				}

				// Timing directly after the last execution and before the breakdown.
				qint64 totalMSecs = QDateTime::currentMSecsSinceEpoch() - runStartMSecs;
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatTotalTimeLine(totalMSecs, mExecutions.count(), maxDurationMSecs)), this);

				// Overall summary: header line, then each outcome count on its own line.
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatSummaryHeader(controlFiles.count(), mTestCases.count(), mSuites.count(), mExecutions.count())), this);
				logOutcomeLines(totalOutcomeCounts, true);

				// Per-suite, per-task and per-interface breakdown.
				for (const QString& suiteId : suiteOutcomeCounts.keys()) {
					QMap<CConformanceOutcome::TYPE, int> singleSuiteOutcomeCounts = suiteOutcomeCounts.value(suiteId);
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatCategoryHeader("suite", suiteNames.value(suiteId), singleSuiteOutcomeCounts)), this);
					logOutcomeLines(singleSuiteOutcomeCounts, false);
				}
				for (CConformanceTask::TYPE taskType : typeOutcomeCounts.keys()) {
					QMap<CConformanceOutcome::TYPE, int> taskOutcomeCounts = typeOutcomeCounts.value(taskType);
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatCategoryHeader("test type", CConformanceTask::typeToString(taskType), taskOutcomeCounts)), this);
					logOutcomeLines(taskOutcomeCounts, false);
				}
				for (const QString& interfaceName : interfaceOutcomeCounts.keys()) {
					QMap<CConformanceOutcome::TYPE, int> interfaceTaskOutcomeCounts = interfaceOutcomeCounts.value(interfaceName);
					LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatCategoryHeader("interface", interfaceName, interfaceTaskOutcomeCounts)), this);
					logOutcomeLines(interfaceTaskOutcomeCounts, false);
				}

				// Clear completion message (also covers the case with no failures).
				LOG(INFO, getLogDomain(), logTr("%1").arg(CConformanceReporter::formatFinishedLine(failingResults.count(), mExecutions.count())), this);

				// List all failing tests again at the end, as errors.
				if (!failingResults.isEmpty()) {
					LOG(ERROR, getLogDomain(), logTr("The following tests failed:"), this);
					for (const CConformanceTestResult& failedResult : failingResults) {
						LOG(ERROR, getLogDomain(), logTr("%1").arg(QString("  ") + CConformanceReporter::formatResultLine(failedResult)), this);
					}
				}

				writeResults(results);

				int failureCount = 0;
				for (CConformanceOutcome::TYPE outcome : CConformanceOutcome::allTypes()) {
					if (CConformanceOutcome::typeIsFailing(outcome)) {
						failureCount += totalOutcomeCounts.value(outcome, 0);
					}
				}
				return failureCount;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
