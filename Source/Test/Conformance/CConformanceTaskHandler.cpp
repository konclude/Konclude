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
#include <QFileInfo>
#include <QElapsedTimer>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include "Network/HTTP/CQtHttpTransactionManager.h"


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
				if (CConformanceChannel::typeIsServer(mTestCase.mChannel)) {
					return runServerChannel(timeoutMs, outputLogFilePath);
				}
				return runFileChannel(timeoutMs, outputLogFilePath, responseFilePath);
			}


			CConformanceTaskResult CConformanceTaskHandler::runFileChannel(qint64 timeoutMs, const QString& outputLogFilePath, const QString& responseFilePath) {
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


			CConformanceTaskResult CConformanceTaskHandler::runServerChannel(qint64 timeoutMs, const QString& outputLogFilePath) {
				CConformanceTaskResult result;

				quint16 port = findFreeTcpPort();
				if (port == 0) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("cannot allocate a free TCP port for the test server");
					return result;
				}

				QStringList arguments = buildServerArguments(port);
				if (arguments.isEmpty()) {
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("channel '%1' is not supported for task '%2'").arg(
							CConformanceChannel::typeToString(mTestCase.mChannel), CConformanceTask::typeToString(mTestCase.mTask));
					return result;
				}

				QProcess server;
				if (!outputLogFilePath.isEmpty()) {
					server.setProcessChannelMode(QProcess::MergedChannels);
					server.setStandardOutputFile(outputLogFilePath);
				}
				server.start(mBinaryFile, arguments);
				if (!server.waitForStarted(10000)) {
					server.kill();
					result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					result.mExplanation = QString("cannot start test server '%1'").arg(mBinaryFile);
					return result;
				}

				// The server needs a moment to load its data and bind the port; poll the
				// port instead of sleeping for a fixed time.
				qint64 startupTimeoutMs = qMin<qint64>(timeoutMs, 30000);
				CConformanceTaskResult taskResult;
				if (!waitForTcpPort(port, startupTimeoutMs)) {
					taskResult.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
					taskResult.mExplanation = QString("test server did not accept connections on port %1 within %2 s")
							.arg(port).arg(startupTimeoutMs / 1000.0, 0, 'g', 6);
				} else {
					QString requestText;
					if (!readTextFile(mTestCase.mOntologyFile, requestText)) {
						taskResult.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
						taskResult.mExplanation = QString("cannot read request file '%1'").arg(QFileInfo(mTestCase.mOntologyFile).fileName());
					} else {
						QString contentType = (mTestCase.mChannel == CConformanceChannel::TYPE::SPARQL_SERVER)
								? QString("application/sparql-query") : QString("application/xml");
						QString url = QString("http://127.0.0.1:%1/").arg(port);
						QString responseText;
						QString errorText;
						if (postHttpRequest(url, requestText.toUtf8(), contentType, timeoutMs, responseText, errorText)) {
							taskResult = interpretServerResponse(responseText, readLogOutput(outputLogFilePath, server));
						} else {
							taskResult.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
							taskResult.mExplanation = QString("HTTP request to '%1' failed: %2").arg(url).arg(errorText);
						}
					}
				}

				server.kill();
				server.waitForFinished(5000);
				return taskResult;
			}


			QStringList CConformanceTaskHandler::buildServerArguments(quint16 port) {
				Q_UNUSED(port);
				return QStringList();
			}


			CConformanceTaskResult CConformanceTaskHandler::interpretServerResponse(const QString& responseText, const QString& logOutput) {
				Q_UNUSED(responseText);
				Q_UNUSED(logOutput);

				CConformanceTaskResult result;
				result.mStatus = CConformanceTaskResult::STATUS::EXECUTION_ERROR;
				result.mExplanation = QString("channel '%1' is not supported for task '%2'").arg(
						CConformanceChannel::typeToString(mTestCase.mChannel), CConformanceTask::typeToString(mTestCase.mTask));
				return result;
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


			quint16 CConformanceTaskHandler::findFreeTcpPort() {
				QTcpServer probe;
				if (!probe.listen(QHostAddress::LocalHost, 0)) {
					return 0;
				}
				quint16 port = probe.serverPort();
				probe.close();
				return port;
			}


			bool CConformanceTaskHandler::waitForTcpPort(quint16 port, qint64 timeoutMs) {
				QElapsedTimer timer;
				timer.start();
				while (timer.elapsed() < timeoutMs) {
					QTcpSocket socket;
					socket.connectToHost(QHostAddress::LocalHost, port);
					bool connected = socket.waitForConnected(200);
					socket.abort();
					if (connected) {
						return true;
					}
					QThread::msleep(100);
				}
				return false;
			}


			bool CConformanceTaskHandler::postHttpRequest(const QString& url, const QByteArray& requestBody, const QString& contentType, qint64 timeoutMs, QString& responseText, QString& errorText) {
				Network::HTTP::CQtHttpTransactionManager transactionManager(timeoutMs);

				Network::HTTP::CHttpRequest* request = transactionManager.createRequest(url, requestBody);
				if (!request) {
					errorText = QString("cannot create the HTTP request");
					return false;
				}
				transactionManager.setRequestHeader(request, QString("Content-Type"), contentType);

				Network::HTTP::CHttpResponse* response = transactionManager.getResponse(request);
				if (!response) {
					errorText = QString("cannot send the HTTP request");
					return false;
				}

				QString* text = transactionManager.getExtractedText(response);
				bool success = false;
				if (text && !text->isEmpty()) {
					responseText = *text;
					success = true;
				} else {
					QString* transactionError = transactionManager.getErrorString(response);
					if (transactionError) {
						errorText = *transactionError;
					}
				}
				delete text;
				transactionManager.releaseResponse(response);

				if (!success && errorText.isEmpty()) {
					errorText = QString("empty HTTP response");
				}
				return success;
			}

		}; // end namespace Conformance

	}; // end namespace Test

}; // end namespace Konclude
