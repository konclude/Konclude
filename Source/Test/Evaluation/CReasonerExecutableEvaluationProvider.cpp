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

#include "CReasonerExecutableEvaluationProvider.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerExecutableEvaluationProvider::CReasonerExecutableEvaluationProvider() {
				mProcess = nullptr;
				mKillTimeout = 1000;
				mReasonerPort = 8080;
			}


			CReasonerExecutableEvaluationProvider::~CReasonerExecutableEvaluationProvider() {
			}


			bool CReasonerExecutableEvaluationProvider::createReasoner(CConfiguration *config) {

				mReasonerName = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Name");

				mReasonerBinaryFile = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.Binary.File");
				mReasonerBinaryArguments = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.Binary.Arguments");
				mReasonerPort = CConfigDataReader::readConfigInteger(config,"Konclude.Evaluation.Reasoner.Address.Port");

				mKillScriptString = CConfigDataReader::readConfigString(config, "Konclude.Evaluation.TerminateAssistProgram");
				mKillScriptArgumentsString = CConfigDataReader::readConfigString(config, "Konclude.Evaluation.TerminateAssistAdditionalArgument");

				QString argString = QString("%1").arg(mReasonerBinaryArguments);
				QString appString = QString("%1 %2").arg(mReasonerBinaryFile).arg(argString);

				mProcess = new QProcess();

				connect(mProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)),Qt::DirectConnection);
				connect(mProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)),Qt::DirectConnection);

				mProcessFinished = false;
				mProcessError = false;

				mProcess->start(appString);

				return true;
			}


			QString CReasonerExecutableEvaluationProvider::getReasonerAddressString() {
				return QString("127.0.0.1:%1").arg(mReasonerPort);
			}


			CReasonerEvaluationTerminationResult* CReasonerExecutableEvaluationProvider::destroyReasoner() {
				CReasonerEvaluationTerminationResult* result = new CReasonerEvaluationTerminationResult();

				QString stdOutText;
				QString stdErrText;
				QString errorString;
				try {
					QByteArray standOutput = mProcess->readAllStandardOutput();
					QByteArray standError = mProcess->readAllStandardError();
					stdOutText = QString(standOutput);
					stdErrText = QString(standError);
					errorString = mProcess->errorString();
				} catch (std::bad_alloc& ba) {
					stdErrText = "Reasoners outputs exceed size limitations\r\n";
					LOG(INFO, "::Konclude::Test::ReasonerEvaluationProvider", logTr("Failed to read reasoner output due to size limitations."), this);
				}

				bool forcedTermination = false;
				bool finishedInTime = mProcessFinished;

				disconnect(mProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)));
				disconnect(mProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
				disconnect(mProcess);
				disconnect(this);
				if (!mProcessFinished) {
					assistTermination(mProcess->pid());
					mProcessFinished = true;
					forcedTermination = true;
					mProcess->terminate();
					mProcess->waitForFinished(mKillTimeout);
					mProcess->kill();
					mProcess->waitForFinished(mKillTimeout);
					mProcess->kill();
					mProcess->waitForFinished(mKillTimeout);
				}
				mProcess->close();
				delete mProcess;
				mProcess = nullptr;
				result->initResult(mProcessError,finishedInTime,forcedTermination,errorString,stdOutText,stdErrText);
				return result;
			}



			bool CReasonerExecutableEvaluationProvider::assistTermination(Q_PID processID) {
				if (!mKillScriptString.isEmpty()) {
					QProcess killProcess;
					QStringList argumentList;
					argumentList += QString::number((cint64)processID);
					if (!mKillScriptArgumentsString.isEmpty()) {
						argumentList += mKillScriptArgumentsString;
					}
					LOG(INFO,"::Konclude::Test::ReasonerEvaluationProvider",logTr("Executing '%1' to assist termination of process with id '%2'.").arg(mKillScriptString).arg((cint64)processID),this);
					killProcess.start(mKillScriptString,argumentList);
					if (!killProcess.waitForStarted()) {
						return false;
					}
					if (!killProcess.waitForFinished()) {
						return false;
					}
					if (killProcess.exitCode() != 0) {
						return true;
					}
				}
				return false;
			}

			void CReasonerExecutableEvaluationProvider::processError(QProcess::ProcessError error) {
				if (!mProcessFinished) {
					mProcessError = true;
				}

			}

			void CReasonerExecutableEvaluationProvider::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
				if (!mProcessFinished) {
					mProcessFinished = true;
					if (exitStatus != QProcess::NormalExit) {
						mProcessError = true;
					}
				}
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
