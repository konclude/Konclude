/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CReasonerTestfileTester.h"


namespace Konclude {

	namespace Test {



		CReasonerTestfileTester::CReasonerTestfileTester(CConfiguration *config, CWatchDog *watchDog) 
					: CIntervalThread("Testfile-Tester-Thread",watchDog), CLogIdentifier("::Konclude::Test::ReasonerTestfileTester",this) {
			wCount = 1;
			testProcess = 0;
			processCrashed = false;
			processTimedout = false;
			testCommand = 0;
			testfileTest = 0;
			evaluator = 0;
			commandRecordRouter = 0;
			reasonerString = QString("ExperimentalReasonerLoader");

			closeTimeout = 1000;
			killTimeout = 1000;
			runTimeout = 60000;

			CConfigData *confData = config->getRelatedConfigChange("Konclude.Calculation.WorkerCount");
			if (confData) {
				QString workerCountString = confData->getString();
				bool ok = true;
				wCount = qMax(workerCountString.toInt(&ok),0);
				if (!ok) {
					wCount = CThread::idealThreadCount();
				} 
			}

			confData = config->getRelatedConfigChange("Konclude.Execution.CalculationManager");
			if (confData && confData->supportsStringConvertion()) {
				reasonerConfigString += QString(" +=%1=%2 ").arg(confData->getConfigDescription()->getConfigName()).arg(confData->getString());
			}
			confData = config->getRelatedConfigChange("Konclude.Calculation.WorkerCount");
			if (confData && confData->supportsStringConvertion()) {
				reasonerConfigString += QString(" +=%1=%2 ").arg(confData->getConfigDescription()->getConfigName()).arg(confData->getString());
			}
			confData = config->getRelatedConfigChange("Konclude.Calculation.ProcessorCount");
			if (confData && confData->supportsStringConvertion()) {
				reasonerConfigString += QString(" +=%1=%2 ").arg(confData->getConfigDescription()->getConfigName()).arg(confData->getString());
			}
			confData = config->getRelatedConfigChange("Konclude.OWLlink.Test.RunTimeout");
			if (confData) {
				runTimeout = confData->getInteger();
			}
			confData = config->getRelatedConfigChange("Konclude.OWLlink.Test.CloseTimeout");
			if (confData) {
				closeTimeout = confData->getInteger();
			}
			confData = config->getRelatedConfigChange("Konclude.OWLlink.Test.KillTimeout");
			if (confData) {
				killTimeout = confData->getInteger();
			}
			confData = config->getRelatedConfigChange("Konclude.Execution.DefaultReasonerManager");
			if (confData) {
				reasonerString = confData->getString();
				if (reasonerString == "ASExperimentalReasonerManager") {
					reasonerString = QString("ExperimentalReasonerLoader");
				} else if (reasonerString == "ASAnalyseReasonerManager") {
					reasonerString = QString("AnalyseReasonerLoader");
				} else {
					reasonerString = QString("DefaultReasonerLoader");
				}
			}

			CConfigurationGroup* configGroup = config->getConfigurationGroup();
			QList<CConfigDescription*> configDesList(configGroup->getConfigDescriptonList());
			FOREACHIT (CConfigDescription* configDes, configDesList) {
				QString configName = configDes->getConfigName();
				bool propagateConfig = false;
				if (configName.startsWith("Konclude.Calculation.")) {
					propagateConfig = true;
				} else if (configName.startsWith("Konclude.Execution.")) {
					if (!configName.startsWith("Konclude.Execution.CommandLine")) {
						propagateConfig = true;
					}
				}

				if (propagateConfig) {
					confData = config->getRelatedConfigChange(configName);
					if (confData && confData->supportsStringConvertion()) {
						reasonerConfigString += QString(" +=%1=%2 ").arg(configName).arg(confData->getString());
					}
				}
			}


			//runTimeout = 120000;

			startThread();

		}


		CReasonerTestfileTester::~CReasonerTestfileTester() {
		}


		CReasonerTestfileTester *CReasonerTestfileTester::realizeTestCommand(CTestTestfileCommand *command, CReasonerTestfileEvaluator *testEvaluator) {
			if (commandRecordRouter) {
				delete commandRecordRouter;
				commandRecordRouter = 0;
			}
			commandRecordRouter = new CCommandRecordRouter(command,this);
			CStartProcessCommandRecord::makeRecord(commandRecordRouter);
			testCommand = command;
			testfileTest = testCommand->getReasonerTestfileTest();
			testfile = testfileTest->getTestRequestFile();
			evaluator = testEvaluator;
			sendStartTestfileTestEvent();
			return this;
		}


		bool CReasonerTestfileTester::processTimer(qint64 timerID) {
			if (timerID == TIMERCLOSEPROCESSID) {
				CUnspecifiedMessageInformationRecord::makeRecord(QString("Test %1 for testfile '%2' timed out.").arg(testfileTest->getTestResults()->getTotalTestCount()).arg(testfile),commandRecordRouter);
				LOG(WARNING,getLogDomain(),logTr("Test %1 for '%2' timed out.").arg(testfileTest->getTestResults()->getTotalTestCount()+1).arg(testfile),this);
				processTimedout = true;
				sendEvaluateTestfileTestEvent();
				return true;
			}
			return false;
		}

		bool CReasonerTestfileTester::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
			if (CIntervalThread::processCustomsEvents(type,event)) {
				return true;
			} else if (type == EVENTTESTFILE) {
				CTestTestfileEvent *tte = (CTestTestfileEvent *)event;
				startTestfileTest();
			} else if (type == EVENTEVALUATETESTFILE) {
				CEvaluateTestfileTestEvent *ette = (CEvaluateTestfileTestEvent *)event;
				evaluateTestfileTest();
			}
			return false;
		}

		void CReasonerTestfileTester::sendEvaluateTestfileTestEvent() {
            QCoreApplication::postEvent(this,new CEvaluateTestfileTestEvent());
		}

		void CReasonerTestfileTester::sendStartTestfileTestEvent() {
            QCoreApplication::postEvent(this,new CTestTestfileEvent());
		}

		void CReasonerTestfileTester::startTestfileTest() {
			if (!testProcess) {
				qint64 testTimeout = testfileTest->getTimeout();
				if (testTimeout <= 0) {
					testTimeout = runTimeout;
				}
				startTimerWithInterval(TIMERCLOSEPROCESSID,testTimeout);
				processCrashed = false;
				processTimedout = false;
				testProcess = new QProcess();
				QString workDir = QDir::currentPath();
				if (testfileTest->hasWorkingDirectory()) {
					workDir = testfileTest->getWorkingDirectory();
				}
				testProcess->setWorkingDirectory(workDir);
                QString appString = QCoreApplication::applicationFilePath();
				QStringList argumentList;
				argumentList += QString("-ConfigurableCoutLogObserverLoader");
				argumentList += QString("-%1 %2").arg(reasonerString).arg(reasonerConfigString);
				argumentList += QString("-OWLlinkBatchFileLoader +=Konclude.OWLlink.RequestFile=%1").arg(testfile);
				argumentList += QString("+=Konclude.OWLlink.WriteResponseToStandardError=true +=Konclude.OWLlink.CloseAfterProcessedRequest=true");

				connect(testProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)),Qt::DirectConnection);
				connect(testProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)),Qt::DirectConnection);

				LOG(INFO,getLogDomain(),logTr("Starting %1. test for '%2'.").arg(testfileTest->getTestResults()->getTotalTestCount()+1).arg(testfile),this);

				testProcess->start(appString,argumentList);
				if (!testProcess->waitForStarted(10000)) {
					testfileTest->getTestResults()->addFailedResult();
					sendEvaluateTestfileTestEvent();
				}
				//testProcess->start("echo",QStringList()<<"hallo");

			}
		}


		void CReasonerTestfileTester::evaluateTestfileTest() {
			if (testProcess) {
				stopTimer(TIMERCLOSEPROCESSID);
				QString errorString = testProcess->errorString();
				QByteArray standOutput = testProcess->readAllStandardOutput();
				QString outputString(standOutput);
				QByteArray standError = testProcess->readAllStandardError();
				testProcess->close();
				if (!testProcess->waitForFinished(closeTimeout)) {
					testProcess->kill();
					testProcess->waitForFinished(killTimeout);
				}
				if (processCrashed) {
					testfileTest->getTestResults()->addTestResult(true,processTimedout,false,false,0.);
					if (!processTimedout) {
						LOG(WARNING,getLogDomain(),logTr("Test %1 for '%2' has CRASHED.").arg(testfileTest->getTestResults()->getTotalTestCount()).arg(testfile),this);
					}
				} else if (processTimedout) {
					testfileTest->getTestResults()->addTestResult(false,true,false,false,0.);
				} else {
					evaluator->evaluateTestfile(testfileTest,standError);
				}
				disconnect(testProcess);
				delete testProcess;
				testProcess = 0;
				CUnspecifiedMessageInformationRecord::makeRecord(QString("Test %1 for testfile '%2' concluded.").arg(testfileTest->getTestResults()->getTotalTestCount()).arg(testfile),commandRecordRouter);
				//LOG(INFO,getLogDomain(),logTr("Test %1 for testfile '%2' concluded.").arg(testfileTest->getTestResults()->getTotalTestCount()).arg(testfile),this);

				if (testfileTest->getTestResults()->getTotalTestCount() < testfileTest->getTestLoopCount()) {
					sendStartTestfileTestEvent();
				} else {
					CStopProcessCommandRecord::makeRecord(commandRecordRouter);
					CFinishProcessCommandRecord::makeRecord(commandRecordRouter);
				}
			}
		}



		void CReasonerTestfileTester::processError(QProcess::ProcessError error) {
			processCrashed = true;
			sendEvaluateTestfileTestEvent();
		}

		void CReasonerTestfileTester::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
			if (exitStatus != QProcess::NormalExit) {
				processCrashed = true;
			}
			sendEvaluateTestfileTestEvent();
		}


	}; // end namespace Test

}; // end namespace Konclude
