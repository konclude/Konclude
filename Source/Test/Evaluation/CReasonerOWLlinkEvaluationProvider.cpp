/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CReasonerOWLlinkEvaluationProvider.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerOWLlinkEvaluationProvider::CReasonerOWLlinkEvaluationProvider() {
				mAdapterProcess = nullptr;
				mSupportProcess = nullptr;
				mKillTimeout = 2000;
				mReasonerPort = 8080;
				mMemoryLimit = (cint64)1024*1024*1024*10;
			}


			CReasonerOWLlinkEvaluationProvider::~CReasonerOWLlinkEvaluationProvider() {
			}


			bool CReasonerOWLlinkEvaluationProvider::createReasoner(CConfiguration *config) {

				mReasonerName = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Name");

				mReasonerLibDirectory = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.ClassPathLibraries");
				mReasonerLibSeparator = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.ClassPathLibrariesSeparator");
				mReasonerClassCallName = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.MainClass");
				mReasonerPort = CConfigDataReader::readConfigInteger(config,"Konclude.Evaluation.Reasoner.Address.Port");
				mMemoryLimit = CConfigDataReader::readConfigInteger(config,"Konclude.Evaluation.MemoryLimit");
				mReasonerAdditionalParameters = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.AdditionalParameters");

				mSupportProcessString = CConfigDataReader::readConfigString(config,"Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.AdditionalProcess");
				if (!mSupportProcessString.isEmpty()) {
					mSupportProcess = new QProcess();
					mSupportProcess->start(mSupportProcessString);
				}

				QStringList jarFilesStringList;
				addLibrariesRecursive(mReasonerLibDirectory,jarFilesStringList);
				QString argString = QString("-Xmx%1 -Djava.library.path=%2 -cp .%3\"%4\" %5 -port %6 %7").arg(mMemoryLimit).arg(mReasonerLibDirectory).arg(mReasonerLibSeparator).arg(jarFilesStringList.join(mReasonerLibSeparator)).arg(mReasonerClassCallName).arg(mReasonerPort).arg(mReasonerAdditionalParameters);
				QString appString = QString("java %1").arg(argString);


				mAdapterProcess = new QProcess();

				connect(mAdapterProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)),Qt::DirectConnection);
				connect(mAdapterProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)),Qt::DirectConnection);

				mProcessFinished = false;
				mProcessError = false;

				mAdapterProcess->start(appString);

				return true;
			}


			QString CReasonerOWLlinkEvaluationProvider::getReasonerAddressString() {
				return QString("127.0.0.1:%1").arg(mReasonerPort);
			}


			CReasonerEvaluationTerminationResult* CReasonerOWLlinkEvaluationProvider::destroyReasoner() {
				CReasonerEvaluationTerminationResult* result = new CReasonerEvaluationTerminationResult();

				QByteArray standOutput = mAdapterProcess->readAllStandardOutput();
				QByteArray standError = mAdapterProcess->readAllStandardError();
				QString stdOutText(standOutput);
				QString stdErrText(standError);
				QString errorString = mAdapterProcess->errorString();

				bool forcedTermination = false;
				bool finishedInTime = mProcessFinished;

				disconnect(mAdapterProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)));
				disconnect(mAdapterProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
				disconnect(mAdapterProcess);
				disconnect(this);
				if (!mProcessFinished) {
					mProcessFinished = true;
					forcedTermination = true;
					mAdapterProcess->terminate();
					mAdapterProcess->waitForFinished(mKillTimeout);
					mAdapterProcess->kill();
					mAdapterProcess->waitForFinished(mKillTimeout);
					mAdapterProcess->kill();
					mAdapterProcess->waitForFinished(mKillTimeout);
				}
				if (mSupportProcess) {
					mSupportProcess->terminate();
					mSupportProcess->waitForFinished(mKillTimeout);
					mSupportProcess->kill();
					mSupportProcess->waitForFinished(mKillTimeout);
					mSupportProcess->kill();
					mSupportProcess->waitForFinished(mKillTimeout);
				}
				mAdapterProcess->close();
				delete mAdapterProcess;
				mAdapterProcess = nullptr;
				result->initResult(mProcessError,finishedInTime,forcedTermination,errorString,stdOutText,stdErrText);
				return result;
			}

			void CReasonerOWLlinkEvaluationProvider::processError(QProcess::ProcessError error) {
				if (!mProcessFinished) {
					mProcessError = true;
				}

			}

			void CReasonerOWLlinkEvaluationProvider::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
				if (!mProcessFinished) {
					mProcessFinished = true;
					if (exitStatus != QProcess::NormalExit) {
						mProcessError = true;
					}
				}
			}


			void CReasonerOWLlinkEvaluationProvider::addLibrariesRecursive(const QString& dirString, QStringList& jarStringList) {
				QDir dir(dirString);
				QStringList filterList;
				filterList<<QString("*.jar");
				QStringList entrylist(dir.entryList(filterList,QDir::Files));
				foreach (QString fileString, entrylist) {
					jarStringList.append(dirString+fileString);
				}
				foreach (QString subDir, dir.entryList(QDir::Dirs)) {
					if (!subDir.endsWith(".") && !subDir.endsWith("..")) {
						addLibrariesRecursive(dirString+subDir+"/",jarStringList);
					}
				}
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
