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

#include "CConformanceCliBooleanTaskHandler.h"

#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>

#include "Parser/COWLlinkQtXMLResultParser.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CConformanceCliBooleanTaskHandler::CConformanceCliBooleanTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile)
					: CConformanceTaskHandler(testCase, binaryFile, configFile) {
			}


			CConformanceCliBooleanTaskHandler::~CConformanceCliBooleanTaskHandler() {
			}


			CConformanceTask::TYPE CConformanceCliBooleanTaskHandler::getTaskType() const {
				return mTestCase.mTask;
			}


			QStringList CConformanceCliBooleanTaskHandler::buildArguments(const QString& responseFilePath) {
				// The canonical task name is also the command name of the binary.
				QStringList arguments;
				arguments << CConformanceTask::typeToString(mTestCase.mTask) << "-i" << mTestCase.mOntologyFile;
				if (mTestCase.mTask == CConformanceTask::TYPE::SATISFIABILITY) {
					arguments << "-x" << mTestCase.mTestClass;
				}
				arguments << "-o" << responseFilePath;
				appendConfigArgument(arguments);
				return arguments;
			}


			CConformanceTaskResult CConformanceCliBooleanTaskHandler::interpretResponse(const QString& responseFilePath, const QString& logOutput) {
				CConformanceTaskResult result;
				CConformanceTask::VERDICT verdict = CConformanceTask::VERDICT::UNSET;

				QString responseText;
				bool haveVerdict = readTextFile(responseFilePath, responseText) && parseBooleanResponse(responseText, verdict);
				if (haveVerdict) {
					result.mStatus = CConformanceTaskResult::STATUS::COMPLETED;
					result.mVerdict = verdict;
					result.mMatched = (verdict == mTestCase.mExpectedVerdict);
					return result;
				}

				// The response file is the primary answer source. The console output is
				// consulted only as an additional diagnostic when that file is unusable.
				QString responseFileName = QFileInfo(responseFilePath).fileName();
				if (verdictFromLogOutput(logOutput, verdict)) {
					result.mStatus = CConformanceTaskResult::STATUS::COMPLETED;
					result.mVerdict = verdict;
					result.mMatched = (verdict == mTestCase.mExpectedVerdict);
					result.mExplanation = QString("response file '%1' unusable, verdict taken from log output").arg(responseFileName);
					return result;
				}

				result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
				result.mExplanation = QString("no answer in response file '%1' or log output").arg(responseFileName);
				return result;
			}


			bool CConformanceCliBooleanTaskHandler::parseBooleanResponse(const QString& responseText, CConformanceTask::VERDICT& verdict) {
				// The CLI loaders write a plain token followed by a newline; trimming
				// removes that newline (the file, not the console log, is the source).
				QString token = responseText.trimmed();
				if (token.isEmpty()) {
					return false;
				}

				if (token.startsWith('<')) {
					// Structured variant: a BooleanResponse document.
					QDomDocument document;
					if (!document.setContent(token)) {
						return false;
					}
					QDomNodeList responses = document.elementsByTagName("BooleanResponse");
					if (responses.isEmpty()) {
						responses = document.elementsByTagNameNS(QString("*"), QString("BooleanResponse"));
					}
					if (responses.isEmpty()) {
						return false;
					}
					QDomElement responseElement = responses.at(0).toElement();
					Parser::COWLlinkQtXMLResultParser resultParser;
					CBooleanQueryResult* booleanResult = resultParser.parseBooleanQueryResult(&responseElement);
					if (!booleanResult) {
						return false;
					}
					verdict = booleanResult->getResult() ? CConformanceTask::VERDICT::POSITIVE : CConformanceTask::VERDICT::NEGATIVE;
					delete booleanResult;
					return true;
				}

				if (QString::compare(token, "true", Qt::CaseInsensitive) == 0) {
					verdict = CConformanceTask::VERDICT::POSITIVE;
					return true;
				}
				if (QString::compare(token, "false", Qt::CaseInsensitive) == 0) {
					verdict = CConformanceTask::VERDICT::NEGATIVE;
					return true;
				}
				return false;
			}


			bool CConformanceCliBooleanTaskHandler::verdictFromLogOutput(const QString& logOutput, CConformanceTask::VERDICT& verdict) const {
				// Check the negated wording first: it contains the positive one as a
				// substring ("is not satisfiable" contains "satisfiable").
				if (mTestCase.mTask == CConformanceTask::TYPE::SATISFIABILITY) {
					if (logOutput.contains("is not satisfiable")) {
						verdict = CConformanceTask::VERDICT::NEGATIVE;
						return true;
					}
					if (logOutput.contains("is satisfiable")) {
						verdict = CConformanceTask::VERDICT::POSITIVE;
						return true;
					}
					return false;
				}

				if (logOutput.contains("is inconsistent")) {
					verdict = CConformanceTask::VERDICT::NEGATIVE;
					return true;
				}
				if (logOutput.contains("is consistent")) {
					verdict = CConformanceTask::VERDICT::POSITIVE;
					return true;
				}
				return false;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
