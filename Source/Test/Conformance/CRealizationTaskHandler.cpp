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

#include "CRealizationTaskHandler.h"

#include <QFile>
#include <QFileInfo>

#include "CRealizationResultComparator.h"


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CRealizationTaskHandler::CRealizationTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile)
					: CConformanceTaskHandler(testCase, binaryFile, configFile) {
			}


			CRealizationTaskHandler::~CRealizationTaskHandler() {
			}


			CConformanceTask::TYPE CRealizationTaskHandler::getTaskType() const {
				return CConformanceTask::TYPE::CLASS_REALIZATION;
			}


			QStringList CRealizationTaskHandler::buildArguments(const QString& responseFilePath) {
				QStringList arguments;
				arguments << "realization" << "-i" << mTestCase.mOntologyFile << "-o" << responseFilePath;
				if (!mTestCase.mTestIndividual.isEmpty()) {
					arguments << "-x" << mTestCase.mTestIndividual;
				}
				appendConfigArgument(arguments);
				return arguments;
			}


			CConformanceTaskResult CRealizationTaskHandler::interpretResponse(const QString& responseFilePath, const QString& logOutput) {
				Q_UNUSED(logOutput);

				CConformanceTaskResult result;
				if (!QFile::exists(responseFilePath)) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("no realization result written to '%1'").arg(QFileInfo(responseFilePath).fileName());
					return result;
				}

				QString detailMessage;
				bool equivalent = CRealizationResultComparator::compareRealizationFiles(responseFilePath, mTestCase.mExpectedFile, detailMessage);
				result.mStatus = CConformanceTaskResult::STATUS::COMPLETED;
				result.mVerdict = equivalent ? CConformanceTask::VERDICT::POSITIVE : CConformanceTask::VERDICT::NEGATIVE;
				result.mMatched = equivalent;
				result.mExplanation = detailMessage;
				return result;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
