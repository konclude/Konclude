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

#include "COWLLinkBooleanTaskHandler.h"

#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>

#include "Parser/COWLlinkQtXMLResultParser.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			COWLLinkBooleanTaskHandler::COWLLinkBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile)
					: CConformanceTaskHandler(testCase, binaryFile, configFile) {
			}


			COWLLinkBooleanTaskHandler::~COWLLinkBooleanTaskHandler() {
			}


			CConformanceTask::TYPE COWLLinkBooleanTaskHandler::getTaskType() const {
				return CConformanceTask::TYPE::OWLLINK_BOOLEAN;
			}


			QStringList COWLLinkBooleanTaskHandler::buildArguments(const QString& responseFilePath) {
				QStringList arguments;
				arguments << "owllinkfile" << "-i" << mTestCase.mOntologyFile << "-o" << responseFilePath;
				appendConfigArgument(arguments);
				return arguments;
			}


			CConformanceTaskResult COWLLinkBooleanTaskHandler::interpretResponse(const QString& responseFilePath, const QString& logOutput) {
				Q_UNUSED(logOutput);

				CConformanceTaskResult result;
				QList<CConformanceTask::VERDICT> actualVerdicts;
				QString errorMessage;
				if (!parseBooleanResponseFile(responseFilePath, actualVerdicts, errorMessage)) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = errorMessage;
					return result;
				}

				const QList<CConformanceTask::VERDICT>& expectedVerdicts = mTestCase.mExpectedVerdicts;
				if (expectedVerdicts.isEmpty()) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("no expected result declared for this OWLlink test");
					return result;
				}
				if (expectedVerdicts.count() > 1 && expectedVerdicts.count() != actualVerdicts.count()) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("expected %1 answers but the OWLlink response contains %2")
							.arg(expectedVerdicts.count()).arg(actualVerdicts.count());
					return result;
				}

				// A single expected result applies to every response; otherwise the
				// expectations are matched one-to-one in document order.
				bool matched = true;
				CConformanceTask::VERDICT mismatchVerdict = CConformanceTask::VERDICT::UNSET;
				QString mismatchText;
				for (int i = 0; i < actualVerdicts.count(); ++i) {
					CConformanceTask::VERDICT expectedVerdict = (expectedVerdicts.count() == 1) ? expectedVerdicts.first() : expectedVerdicts.at(i);
					if (actualVerdicts.at(i) != expectedVerdict) {
						matched = false;
						mismatchVerdict = actualVerdicts.at(i);
						mismatchText = QString("answer %1 of %2: %3, expected %4")
								.arg(i + 1).arg(actualVerdicts.count())
								.arg(CConformanceTask::verdictToString(CConformanceTask::TYPE::OWLLINK_BOOLEAN, actualVerdicts.at(i)))
								.arg(CConformanceTask::verdictToString(CConformanceTask::TYPE::OWLLINK_BOOLEAN, expectedVerdict));
						break;
					}
				}

				result.mStatus = CConformanceTaskResult::STATUS::COMPLETED;
				result.mVerdict = matched ? CConformanceTask::VERDICT::POSITIVE : mismatchVerdict;
				result.mMatched = matched;
				result.mExplanation = mismatchText;
				return result;
			}


			bool COWLLinkBooleanTaskHandler::parseBooleanResponseFile(const QString& responseFilePath, QList<CConformanceTask::VERDICT>& verdicts, QString& errorMessage) {
				verdicts.clear();

				QString responseFileName = QFileInfo(responseFilePath).fileName();
				QFile file(responseFilePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					errorMessage = QString("no OWLlink response written to '%1'").arg(responseFileName);
					return false;
				}

				QDomDocument document;
				QString parseError;
				int errorLine = 0;
				int errorColumn = 0;
				bool parsed = document.setContent(&file, &parseError, &errorLine, &errorColumn);
				file.close();
				if (!parsed) {
					errorMessage = QString("cannot parse OWLlink response '%1' (%2 at line %3)").arg(responseFileName).arg(parseError).arg(errorLine);
					return false;
				}

				// An OWLlink ResponseMessage contains one child per request, in request
				// order (KB, OK, BooleanResponse, ...). Parse every BooleanResponse with
				// the parser that the evaluation code uses for OWLlink results.
				Parser::COWLlinkQtXMLResultParser resultParser;
				QDomElement responseElement = document.documentElement().firstChildElement();
				while (!responseElement.isNull()) {
					QString tagName = responseElement.tagName();
					if (tagName == "BooleanResponse" || tagName.endsWith(":BooleanResponse")) {
						CBooleanQueryResult* booleanResult = resultParser.parseBooleanQueryResult(&responseElement);
						if (booleanResult) {
							verdicts.append(booleanResult->getResult() ? CConformanceTask::VERDICT::POSITIVE : CConformanceTask::VERDICT::NEGATIVE);
							delete booleanResult;
						}
					}
					responseElement = responseElement.nextSiblingElement();
				}

				if (verdicts.isEmpty()) {
					errorMessage = QString("no BooleanResponse in OWLlink response '%1'").arg(responseFileName);
					return false;
				}
				return true;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
