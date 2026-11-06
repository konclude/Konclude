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

#include "CConformanceOWLXMLResultWriter.h"

#include <QFile>
#include <QMap>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CConformanceOWLXMLResultWriter::CConformanceOWLXMLResultWriter() {
			}


			CConformanceOWLXMLResultWriter::~CConformanceOWLXMLResultWriter() {
			}


			bool CConformanceOWLXMLResultWriter::writeResults(const QString& filePath, const QList<CConformanceTestResult>& results) {
				QFile file(filePath);
				if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
					LOG(ERROR, getLogDomain(), logTr("Cannot write result ontology '%1'.").arg(filePath), this);
					return false;
				}

				QTextStream stream(&file);
				stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
				stream << "<Ontology xmlns=\"http://www.w3.org/2002/07/owl#\"\n";
				stream << "          ontologyIRI=\"http://konclude.test/results\">\n";
				stream << "  <Prefix name=\"kt\" IRI=\"http://konclude.test/vocabulary#\"/>\n";
				stream << "  <Prefix name=\"res\" IRI=\"http://konclude.test/results/vocabulary#\"/>\n";

				// Declare every suite once and keep its readable name on the suite
				// individual; each result then points at that individual.
				QMap<QString, QString> suiteNames;
				for (const CConformanceTestResult& result : results) {
					if (!result.mSuiteId.isEmpty()) {
						suiteNames.insert(result.mSuiteId, result.mSuiteName);
					}
				}
				for (auto suiteIt = suiteNames.constBegin(); suiteIt != suiteNames.constEnd(); ++suiteIt) {
					stream << "  <ClassAssertion><Class abbreviatedIRI=\"kt:TestSuite\"/>";
					writeIndividual(stream, suiteIt.key());
					stream << "</ClassAssertion>\n";
					if (!suiteIt.value().isEmpty()) {
						writeLiteralAssertion(stream, "kt:suiteName", suiteIt.key(), suiteIt.value());
					}
				}

				int resultIndex = 0;
				for (const CConformanceTestResult& result : results) {
					writeResult(stream, result, resultIndex++);
				}

				stream << "</Ontology>\n";
				file.close();
				return true;
			}


			void CConformanceOWLXMLResultWriter::writeResult(QTextStream& stream, const CConformanceTestResult& result, int resultIndex) {
				QString resultIRI = QString("res:r-%1").arg(resultIndex, 4, 10, QChar('0'));

				stream << "  <ClassAssertion><Class abbreviatedIRI=\"kt:TestResult\"/>";
				writeIndividual(stream, resultIRI);
				stream << "</ClassAssertion>\n";

				writeObjectPropertyAssertion(stream, "kt:resultOf", resultIRI, result.mId);
				if (!result.mSuiteId.isEmpty()) {
					writeObjectPropertyAssertion(stream, "kt:suite", resultIRI, result.mSuiteId);
				}

				writeLiteralAssertion(stream, "kt:task", resultIRI, CConformanceTask::typeToString(result.mTask));
				writeLiteralAssertion(stream, "kt:interface", resultIRI, result.mInterface);
				if (!result.mRunConfigName.isEmpty()) {
					writeLiteralAssertion(stream, "kt:runConfig", resultIRI, result.mRunConfigName);
				}
				writeLiteralAssertion(stream, "kt:systemConfig", resultIRI, result.mSystemConfigName);
				writeLiteralAssertion(stream, "kt:outcome", resultIRI, CConformanceOutcome::typeToString(result.mOutcome));
				writeLiteralAssertion(stream, "kt:durationMs", resultIRI, QString::number(result.mDurationMs));

				QString expectedText = CConformanceTask::verdictToString(result.mTask, result.mExpectedVerdict);
				if (!expectedText.isEmpty()) {
					writeLiteralAssertion(stream, "kt:expected", resultIRI, expectedText);
				}
				QString actualText = CConformanceTask::verdictToString(result.mTask, result.mActualVerdict);
				if (!actualText.isEmpty()) {
					writeLiteralAssertion(stream, "kt:actual", resultIRI, actualText);
				}
				if (!result.mExplanation.isEmpty()) {
					writeLiteralAssertion(stream, "kt:explanation", resultIRI, result.mExplanation);
				}
				for (int i = 0; i < result.mKnownIssueIds.count(); ++i) {
					writeObjectPropertyAssertion(stream, "kt:knownIssue", resultIRI, result.mKnownIssueIds.at(i));
					writeLiteralAssertion(stream, "kt:knownIssueType", resultIRI, result.mKnownIssueTypes.value(i));
					if (!result.mKnownIssueDescriptions.value(i).isEmpty()) {
						writeLiteralAssertion(stream, "kt:knownIssueDescription", resultIRI, result.mKnownIssueDescriptions.at(i));
					}
				}
				if (!result.mOutputLogFile.isEmpty()) {
					// Path of the file containing the raw output of the executed Konclude
					// run for this test (a 'test-outputs' folder next to the output file).
					writeLiteralAssertion(stream, "kt:outputFile", resultIRI, result.mOutputLogFile);
				}
				if (!result.mResponseFile.isEmpty()) {
					// Path of the answer file: the '-o' response of a file channel or the
					// body of the HTTP response of a live channel.
					writeLiteralAssertion(stream, "kt:responseFile", resultIRI, result.mResponseFile);
				}
			}


			void CConformanceOWLXMLResultWriter::writeIndividual(QTextStream& stream, const QString& individualIRI) {
				bool abbreviated = individualIRI.contains(':') && !individualIRI.contains("://");
				if (abbreviated) {
					stream << "<NamedIndividual abbreviatedIRI=\"" << individualIRI.toHtmlEscaped() << "\"/>";
				} else {
					stream << "<NamedIndividual IRI=\"" << individualIRI.toHtmlEscaped() << "\"/>";
				}
			}


			void CConformanceOWLXMLResultWriter::writeObjectPropertyAssertion(QTextStream& stream, const QString& objectPropertyIRI, const QString& subjectIRI, const QString& objectIRI) {
				stream << "  <ObjectPropertyAssertion><ObjectProperty abbreviatedIRI=\"" << objectPropertyIRI << "\"/>";
				writeIndividual(stream, subjectIRI);
				writeIndividual(stream, objectIRI);
				stream << "</ObjectPropertyAssertion>\n";
			}


			void CConformanceOWLXMLResultWriter::writeLiteralAssertion(QTextStream& stream, const QString& dataPropertyIRI, const QString& subjectIRI, const QString& value) {
				stream << "  <DataPropertyAssertion><DataProperty abbreviatedIRI=\"" << dataPropertyIRI << "\"/>";
				writeIndividual(stream, subjectIRI);
				stream << "<Literal>" << value.toHtmlEscaped() << "</Literal></DataPropertyAssertion>\n";
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
