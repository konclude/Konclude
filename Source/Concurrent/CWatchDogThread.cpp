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
#include "CWatchDogThread.h"


#include "Events/CWatchDogFeedbackEvent.h"
#include "Events/CRequestFeedbackEvent.h"


namespace Konclude {

	namespace Concurrent {

		CWatchDogThread * CWatchDogThread::watchDogInstance = 0;
		QSemaphore * CWatchDogThread::syncSemaphore = new QSemaphore(1);


		CWatchDogThread::CWatchDogThread() : CIntervalThread("WatchDog") {
			startThread();
			currentUpdateNumber = 0;
		}

		CWatchDogThread::~CWatchDogThread() {
			stopThread();
		}


		CWatchDogThread *CWatchDogThread::getInstance() {
			if (watchDogInstance == 0) {
				syncSemaphore->acquire();
				if (watchDogInstance == 0) {
					watchDogInstance = new CWatchDogThread();
				}
				syncSemaphore->release();
			}
			return watchDogInstance;
		}


		bool CWatchDogThread::registerThread(CThread *thread) {
			regSync.lockForWrite();

			qint64 threadID = thread->getThreadID();

			CWatchDogThreadData *data = 0;
			if (threadWatch.contains(threadID)) {
				data = threadWatch.value(threadID);
			} else {
				data = new CWatchDogThreadData();
				threadWatch.insert(threadID,data);

				data->setRegisterThread(true);
				data->setThreadID(threadID);
				data->setThreadName(thread->getThreadName());
				data->setThread(thread);
			}
			data->setThreadActive(true);

			data->setRunTime(thread->getRunTimeSecs());
			data->setWaitTime(thread->getWaitTimeSecs());
			data->setTaskDescription(thread->getTaskDescription());


			regSync.unlock();
			return true;
		}

		void CWatchDogThread::feedbackThread(CThread *thread, qint64 updateNumber) {
			CWatchDogFeedbackEvent *wdFEvent = new CWatchDogFeedbackEvent(thread->getThreadID(),updateNumber);
			postEvent(wdFEvent);
		}

		void CWatchDogThread::checkoutThread(CThread *thread) {
			regSync.lockForWrite();

			qint64 threadID = thread->getThreadID();

			CWatchDogThreadData *data = 0;
			if (threadWatch.contains(threadID)) {
				data = threadWatch.value(threadID);
			} else {
				data = new CWatchDogThreadData();
				threadWatch.insert(threadID,data);

				data->setRegisterThread(true);
				data->setThreadID(threadID);
				data->setThreadName(thread->getThreadName());
				data->setThread(thread);
			}
			data->setThreadActive(false);
			data->setThreadError(false);

			data->setRunTime(thread->getRunTimeSecs());
			data->setWaitTime(thread->getWaitTimeSecs());
			data->setTaskDescription(thread->getTaskDescription());

			regSync.unlock();
		}


		void CWatchDogThread::crashedThread(CThread *thread) {
			regSync.lockForWrite();

			qint64 threadID = thread->getThreadID();

			CWatchDogThreadData *data = 0;
			if (threadWatch.contains(threadID)) {
				data = threadWatch.value(threadID);
			} else {
				data = new CWatchDogThreadData();
				threadWatch.insert(threadID,data);

				data->setRegisterThread(true);
				data->setThreadID(threadID);
				data->setThreadName(thread->getThreadName());
				data->setThread(thread);
			}
			data->setThreadActive(false);
			data->setThreadError(true);

			data->setRunTime(thread->getRunTimeSecs());
			data->setWaitTime(thread->getWaitTimeSecs());
			data->setTaskDescription(thread->getTaskDescription());

			regSync.unlock();
		}





		QLinkedList<CWatchDogThreadData> CWatchDogThread::getActiveThreadList() {
			QLinkedList<CWatchDogThreadData> retDataList;
			regSync.lockForRead();
			QList<CWatchDogThreadData *> threadList = threadWatch.values();
			foreach (CWatchDogThreadData *threadData, threadList) {
				if (threadData->isThreadActive()) {
					retDataList.append(*threadData);
				}
			}
			regSync.unlock();
			return retDataList;
		}




		// Thread functions


		void CWatchDogThread::threadStarted() {
			CIntervalThread::threadStarted();
			startTimerWithInterval(REQUESTFEEDBACKTIMERID,1000);
			startTimerWithInterval(UPDATETHREADINFOTIMERID,500);
		}

		bool CWatchDogThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
			if (CIntervalThread::processCustomsEvents(type,event)) {
				return true;
			} if (type == EVENTFEEDBACKTHREADWATCHDOG) {
				SETTASKDESCRIPTION("Process Thread Feedback");
				CWatchDogFeedbackEvent *wdfe = (CWatchDogFeedbackEvent *)event;
				regSync.lockForWrite();

				qint64 threadID = wdfe->getThreadID();
				qint64 uNr = wdfe->getUpdateNumber();

				CWatchDogThreadData *data = 0;
				if (threadWatch.contains(threadID)) {
					data = threadWatch.value(threadID);
				}
				if (data) {
					data->setUpdateNumber(uNr,QDateTime::currentDateTime());
				}
				regSync.unlock();
				return true;
			}
			return false;
		}


		bool CWatchDogThread::processTimer(qint64 timerID) {
			if (timerID == REQUESTFEEDBACKTIMERID) {
				SETTASKDESCRIPTION("Request Thread Feedbacks");
				regSync.lockForRead();
				QList<CWatchDogThreadData *> threadList = threadWatch.values();
				foreach (CWatchDogThreadData *threadData, threadList) {
					if (threadData->isThreadActive()) {
						CThread *thread = threadData->getThread();
						sendRequestFeedbackEvent(thread,currentUpdateNumber);
					}
				}
				regSync.unlock();
				currentUpdateNumber++;
				return true;
			} else if (timerID == UPDATETHREADINFOTIMERID) {
				SETTASKDESCRIPTION("Update Thread Informations");
				regSync.lockForWrite();
				QList<CWatchDogThreadData *> threadList = threadWatch.values();
				foreach (CWatchDogThreadData *threadData, threadList) {
					if (threadData->isThreadActive()) {
						CThread *thread = threadData->getThread();
						updateThreadData(thread,threadData);
					}
				}
				regSync.unlock();
				return true;
			}
			return false;
		}



		void CWatchDogThread::sendRequestFeedbackEvent(CThread *thread, qint64 uNr) {
			thread->postEvent(new CRequestFeedbackEvent(this,uNr));
		}

		void CWatchDogThread::updateThreadData(CThread *thread, CWatchDogThreadData *threadData) {
			threadData->setTaskDescription(thread->getTaskDescription());
			threadData->setRunTime(thread->getRunTimeSecs());
			threadData->setWaitTime(thread->getWaitTimeSecs());
		}


	}; // end namespace Concurrent

}; // end namespace Konclude
