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

#include "CReasonerMemoryUsageCheckingThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerMemoryUsageCheckingThread::CReasonerMemoryUsageCheckingThread(CConfiguration *config, QString processId) : CIntervalThread("ReasonerMemoryCheckingThread"), CLogIdentifier("::Konclude::Test::Evaluation::ReasonerEvaluationManagerThread",this) {
				mProcessId = processId;
				mConfig = config;
				mChecking = true;
				mContinue = true;

				mMaxMemoryUsage = 0;

				mMemoryCheckingIntervall = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.MemoryUsageCheckingInterval");

				mMemoryCheckingProgramm = CConfigDataReader::readConfigString(config, "Konclude.Evaluation.MemoryUsageCheckingProgram");

				startThread();
			}


			CReasonerMemoryUsageCheckingThread::~CReasonerMemoryUsageCheckingThread() {
				stopThread();
			}


			cint64 CReasonerMemoryUsageCheckingThread::getMemoryUsage(bool stopChecking) {
				if (stopChecking) {
					mContinue = false;
				}
				CBlockingCallbackData blockCallbackData;
				postEvent(new CReasonerMemoryUsageFinishedEvent(&blockCallbackData));
				blockCallbackData.waitForCallback();
				return mMaxMemoryUsage;
			}


			void CReasonerMemoryUsageCheckingThread::threadStarted() {
				CIntervalThread::threadStarted();
				startTimerWithInterval(MEMORY_CHECKING_TIMER, mMemoryCheckingIntervall);
			}

			void CReasonerMemoryUsageCheckingThread::threadStopped() {
				CIntervalThread::threadStopped();
			}


			bool CReasonerMemoryUsageCheckingThread::processTimer(qint64 timerID) {
				if (timerID == MEMORY_CHECKING_TIMER) {
					checkMemory();
					return true;
				}
				return false;
			}

			void CReasonerMemoryUsageCheckingThread::checkMemory() {
				if (!mMemoryCheckingProgramm.isEmpty()) {
					QProcess* process = new QProcess();
					QString argString = QString("%1").arg(mProcessId);
					QString appString = QString("%1 %2").arg(mMemoryCheckingProgramm).arg(argString);
					process->start(appString);
					process->waitForFinished();
					QString output = process->readAllStandardOutput().trimmed();
					bool parsed = false;
					cint64 memoryUsage = output.toLongLong(&parsed);
					if (!parsed) {
						memoryUsage = output.toDouble(&parsed);
					}
					mMaxMemoryUsage = qMax(mMaxMemoryUsage, memoryUsage);
					delete process;
				}
			}

			bool CReasonerMemoryUsageCheckingThread::processCustomsEvents(QEvent::Type type, CCustomEvent* event) {
				if (CThread::processCustomsEvents(type,event)) {
					return true;
				} else {
					if (type == CReasonerMemoryUsageFinishedEvent::EVENTTYPE) {
						CReasonerMemoryUsageFinishedEvent* rmusfe = (CReasonerMemoryUsageFinishedEvent*)event;
						if (mChecking && !mContinue) {
							stopTimer(MEMORY_CHECKING_TIMER);
							mChecking = false;
						}
						if (mChecking) {
							checkMemory();
						}
						CCallbackData* callback = rmusfe->getCallback();
						if (callback) {
							callback->doCallback();
						}
						return true;
					}
				}
				return false;
			}




		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
