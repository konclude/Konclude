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

#include "CSPARQLBooleanTaskHandler.h"

#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CSPARQLBooleanTaskHandler::CSPARQLBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile)
					: CConformanceTaskHandler(testCase, binaryFile, configFile) {
			}


			CSPARQLBooleanTaskHandler::~CSPARQLBooleanTaskHandler() {
			}


			CConformanceTask::TYPE CSPARQLBooleanTaskHandler::getTaskType() const {
				return CConformanceTask::TYPE::SPARQL;
			}


			QStringList CSPARQLBooleanTaskHandler::buildArguments(const QString& responseFilePath) {
				QStringList arguments;
				arguments << "sparqlfile";
				if (!mTestCase.mDatasetFile.isEmpty()) {
					arguments << "-i" << mTestCase.mDatasetFile;
				}
				arguments << "-s" << mTestCase.mOntologyFile << "-o" << responseFilePath;
				appendConfigArgument(arguments);
				return arguments;
			}


			QStringList CSPARQLBooleanTaskHandler::buildServerArguments(quint16 port) {
				QStringList arguments;
				arguments << "sparqlserver" << "-p" << QString::number(port);
				if (!mTestCase.mDatasetFile.isEmpty()) {
					arguments << "-i" << mTestCase.mDatasetFile;
				}
				appendConfigArgument(arguments);
				return arguments;
			}


			CConformanceTaskResult CSPARQLBooleanTaskHandler::interpretResponse(const QString& responseFilePath, const QString& logOutput) {
				Q_UNUSED(logOutput);

				// With kt:expectedFile the complete result document is compared, which
				// also covers SELECT queries with variable bindings.
				if (!mTestCase.mExpectedFile.isEmpty()) {
					return compareWithExpectedFile(responseFilePath);
				}

				CConformanceTaskResult result;
				QString responseText;
				if (!readTextFile(responseFilePath, responseText)) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("no SPARQL response written to '%1'").arg(QFileInfo(responseFilePath).fileName());
					return result;
				}
				return interpretAnswer(responseText);
			}


			CConformanceTaskResult CSPARQLBooleanTaskHandler::interpretAnswer(const QString& responseText) const {
				CConformanceTaskResult result;
				CConformanceTask::VERDICT verdict = CConformanceTask::VERDICT::UNSET;
				QString errorMessage;
				if (!parseBooleanAnswer(responseText, verdict, errorMessage)) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = errorMessage;
					return result;
				}

				result.mStatus = CConformanceTaskResult::STATUS::COMPLETED;
				result.mVerdict = verdict;
				result.mMatched = (verdict == mTestCase.mExpectedVerdict);
				return result;
			}


			bool CSPARQLBooleanTaskHandler::parseBooleanAnswer(const QString& responseText, CConformanceTask::VERDICT& verdict, QString& errorMessage) {
				QDomDocument document;
				QString parseError;
				int errorLine = 0;
				int errorColumn = 0;
				if (!document.setContent(responseText, &parseError, &errorLine, &errorColumn)) {
					errorMessage = QString("cannot parse SPARQL result (%1 at line %2)").arg(parseError).arg(errorLine);
					return false;
				}

				QDomElement booleanElement;
				QDomNodeList booleanElements = document.elementsByTagName("boolean");
				if (!booleanElements.isEmpty()) {
					booleanElement = booleanElements.at(0).toElement();
				} else {
					// Namespace-prefixed variant, e.g. <sr:boolean>.
					QDomElement rootElement = document.documentElement();
					for (QDomElement childElement = rootElement.firstChildElement(); !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
						if (childElement.tagName().endsWith(":boolean")) {
							booleanElement = childElement;
							break;
						}
					}
				}
				if (booleanElement.isNull()) {
					errorMessage = QString("no boolean element in the SPARQL result");
					return false;
				}

				QString resultText = booleanElement.text().trimmed();
				if (QString::compare(resultText, "true", Qt::CaseInsensitive) == 0) {
					verdict = CConformanceTask::VERDICT::POSITIVE;
					return true;
				}
				if (QString::compare(resultText, "false", Qt::CaseInsensitive) == 0) {
					verdict = CConformanceTask::VERDICT::NEGATIVE;
					return true;
				}

				errorMessage = QString("SPARQL result contains no valid boolean value");
				return false;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
