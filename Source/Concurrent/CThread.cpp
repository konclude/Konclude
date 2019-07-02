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
#include "CThread.h"



#include "Events/CRequestFeedbackEvent.h"

#include "Logger/CLogger.h"


namespace Konclude {

	namespace Concurrent {


		qint64 CThread::nextThreadID = 1;
		QSemaphore *CThread::syncThreadID = new QSemaphore(1);

		CThread::CThread(QString threadIdentifierName, CWatchDog *watchDogThread) {
			threadName = threadIdentifierName;
			waitTimeSecs = 0;
			blockTimeSecs = 0;
			runTimeSecs = 0;
			threadID = 0;
			mWatchDog = watchDogThread;
			threadRuns = false;
			mActiveEventProcessing = false;
		}

		CThread::~CThread() {
			stopThread(true);
		}


		void CThread::setTaskDescription(QString description) {
			lastTaskDescription = taskDescription;
			taskDescription = description;
		}


		QString CThread::getLastTaskDescription() {
			return lastTaskDescription;
		}

		QString CThread::getTaskDescription() {
			return taskDescription;
		}

		QString CThread::getThreadName() {
			return threadName;
		}

		qint64 CThread::getWaitTimeSecs() {
			return waitTimeSecs;
		}

		qint64 CThread::getRunTimeSecs() {
			return runTimeSecs;
		}

		qint64 CThread::getBlockTimeSecs() {
			return blockTimeSecs;
		}

		qint64 CThread::getThreadID() {
			return threadID;
		}

		void CThread::postEvent(QEvent *event, int priority) {
			QCoreApplication::postEvent(this,event,priority);
		}


		void CThread::waitSynchronization() {
			QSemaphore syncSemaphore(0);
			postEvent(new CWaitSynchronizationEvent(&syncSemaphore));
			syncSemaphore.acquire();
		}


		bool CThread::isThreadRunning() {
			return threadRuns;
		}

		
		void CThread::restartThread() {
			terminate();
			wait();
			startThread();
		}



		void CThread::run() {
			bool registerd = false;

			try {
				// get thread id
				syncThreadID->acquire();
				threadID = nextThreadID++;
				syncThreadID->release();


				// register thread to watchdog
				if (mWatchDog) {
					registerd = mWatchDog->registerThread(this);
				}


				// workload measurement
				TIMEMEASUREMENTBEGINWAITING();

				threadStarted();
				threadRuns = true;
				exec();
				threadRuns = false;
				threadStopped();

				// workload measurement
				TIMEMEASUREMENTENDWAITING();

				if (mWatchDog) {
					mWatchDog->checkoutThread(this);
				}

			} catch (...) {
				if (mWatchDog) {
					mWatchDog->crashedThread(this);
				}
			}
		}


		void CThread::prepareExecution() {
			runTimer.start();
		}
		void CThread::concludeExecution() {
			runTimeSecs += runTimer.elapsed();
		}

		void CThread::prepareWaiting() {
			waitTimer.start();
		}

		void CThread::concludeWaiting() {
			waitTimeSecs += waitTimer.elapsed();
		}


		bool CThread::event(QEvent *event) {

			try {
				// workload measurement
				TIMEMEASUREMENTENDWAITING();
				TIMEMEASUREMENTBEGINEXECUTION();

				SETTASKDESCRIPTION("Start Processing Events");

				mActiveEventProcessing = true;
				bool processed = processEvents(event);

				if (!processed) {
					// event processing
					QEvent::Type type = event->type();
					if (type >= 1200 && type < 2000) {
						processControlEvents(type,(CControlEvent *)event);
						processed = true;
					} else if (type >= 2000) {
						processCustomsEvents(type,(CCustomEvent *)event);
						processed = true;
					}
				}
				mActiveEventProcessing = false;


				// workload measurement
				TIMEMEASUREMENTENDEXECUTION();
				TIMEMEASUREMENTBEGINWAITING();

				SETTASKDESCRIPTION("Wait for Events");


				return processed;
			} catch (...) {

				LOG(ERROR,"::Konclude::Thread",logTr("Unhandled exception, thread %1 stopped.").arg(threadName),this);

				return false;
			}

		}

		bool CThread::isThreadProcessingEvents() {
			return mActiveEventProcessing;
		}


		void CThread::prepareBlocking() {
			runTimeSecs += runTimer.elapsed();
			blockTimer.start();
		}

		void CThread::concludeBlocking() {
			blockTimeSecs += blockTimer.elapsed();
			runTimer.start();
		}



		void CThread::threadStarted() {
		}

		void CThread::threadStopped() {
		}

		void CThread::startThread(Priority priority) {
			start(priority);
			moveToThread(this);
		}

		void CThread::stopThread(bool waitStopped) {
			quit();
			if (waitStopped) {
				wait();
			}
		}

		bool CThread::processEvents(QEvent *event) {
			return false;
		}


		bool CThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
			return false;
		}

		bool CThread::processControlEvents(QEvent::Type type, CControlEvent *event) {
			if (type == EVENTREQUESTFEEDBACKWATCHDOG) {
				SETTASKDESCRIPTION("Send WatchDog Feedback");
				CRequestFeedbackEvent *rfe = (CRequestFeedbackEvent *)event;
				CWatchDog *watchDog = (CWatchDog *)rfe->getWatchDogThread();

				qint64 updateNumber = rfe->getUpdateNumber();

				watchDog->feedbackThread(this,updateNumber);
				return true;
			} else if (type == EVENTWAITSYNCHRONIZATION) {
				SETTASKDESCRIPTION("Synchronize");
				CWaitSynchronizationEvent *wse = (CWaitSynchronizationEvent *)event;
				wse->synchronize();
				return true;
			}
			return false;
		}

	}; // end namespace Concurrent

}; // end namespace Konclude
