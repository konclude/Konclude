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

#include "CConformanceTaskHandler.h"

#include <QFile>


namespace Konclude {

	namespace Test {

		namespace Conformance {


			CConformanceTaskHandler::CConformanceTaskHandler(const CConformanceTestCase& testCase, const QString& binaryFile, const QString& configFile)
					: CLogIdentifier("::Konclude::Test::ConformanceTaskHandler", this),
					  mTestCase(testCase), mBinaryFile(binaryFile), mConfigFile(configFile) {
			}


			CConformanceTaskHandler::~CConformanceTaskHandler() {
			}


			CConformanceTaskResult CConformanceTaskHandler::run(qint64 timeoutMs, const QString& outputLogFilePath, const QString& responseFilePath) {
				QProcess process;
				if (!outputLogFilePath.isEmpty()) {
					// Capture the subprocess' output directly into the log file: QProcess
					// merges stdout and stderr, so no in-memory buffering is required.
					process.setProcessChannelMode(QProcess::MergedChannels);
					process.setStandardOutputFile(outputLogFilePath);
				}

				process.start(mBinaryFile, buildArguments(responseFilePath));
				if (!process.waitForStarted(10000)) {
					CConformanceTaskResult result;
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("cannot start reasoner binary '%1'").arg(mBinaryFile);
					return result;
				}

				// The Qt timeout is an int; values beyond that wait effectively forever,
				// which keeps the behaviour for very large configured timeouts intact.
				int waitTimeoutMs = static_cast<int>(qMin<qint64>(timeoutMs, 2147483647));
				if (!process.waitForFinished(waitTimeoutMs)) {
					process.kill();
					process.waitForFinished(2000);
					CConformanceTaskResult result;
					result.mStatus = CConformanceTaskResult::STATUS::TIMEOUT;
					result.mExplanation = QString("timed out after %1 s").arg(timeoutMs / 1000.0, 0, 'g', 6);
					return result;
				}

				return interpretResponse(responseFilePath, readLogOutput(outputLogFilePath, process));
			}


			void CConformanceTaskHandler::appendConfigArgument(QStringList& arguments) const {
				// mConfigFile is already the configuration of this execution: the plan
				// resolved test-level and suite-level kt:usesTestSystemConfig assignments (or the
				// suite-wide Konclude.Conformance.ConfigFile default) beforehand.
				if (!mConfigFile.isEmpty()) {
					arguments << "-c" << mConfigFile;
				}
			}


			bool CConformanceTaskHandler::readTextFile(const QString& filePath, QString& text) {
				text.clear();
				if (filePath.isEmpty()) {
					return false;
				}
				QFile file(filePath);
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					return false;
				}
				text = QString::fromUtf8(file.readAll());
				file.close();
				return true;
			}


			QString CConformanceTaskHandler::readLogOutput(const QString& outputLogFilePath, QProcess& process) {
				QString logOutput;
				if (!outputLogFilePath.isEmpty()) {
					readTextFile(outputLogFilePath, logOutput);
				} else {
					logOutput = QString::fromUtf8(process.readAllStandardOutput());
					logOutput += QString::fromUtf8(process.readAllStandardError());
				}
				return logOutput;
			}


		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
