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
#include "CIntervalThread.h"


namespace Konclude {

	namespace Concurrent {


		CIntervalThread::CIntervalThread(QString threadIdentifierName, CWatchDog *watchDog) : CThread(threadIdentifierName,watchDog),mSyncTimerMutex(QMutex::Recursive) {
		}

		CIntervalThread::~CIntervalThread() {
		}



		void CIntervalThread::setTimerInterval(qint64 timerID, qint64 timerIntervalMilliSecs) {
			postEvent(new CTimerIntervalEvent(timerID,timerIntervalMilliSecs));
		}

		void CIntervalThread::startTimerWithInterval(qint64 timerID, qint64 timerIntervalMilliSecs) {
			postEvent(new CTimerIntervalEvent(timerID,timerIntervalMilliSecs,true));
		}


		void CIntervalThread::startTimerWithIntervalLimited(qint64 timerID, qint64 timerIntervalMilliSecs, qint64 remainingCallCount) {
			postEvent(new CTimerIntervalEvent(timerID,timerIntervalMilliSecs,true,false,remainingCallCount));
		}

		void CIntervalThread::startTimer(qint64 timerID) {
			postEvent(new CTimerIntervalEvent(timerID,-1,true));
		}



		void CIntervalThread::stopTimer(qint64 timerID) {

			mSyncTimerMutex.lock();

			if (timers.contains(timerID)) {
				CIntervalThreadData *timerData = timers.value(timerID);
				timerData->incDeactivateCount();
			} 

			mSyncTimerMutex.unlock();

			postEvent(new CTimerIntervalEvent(timerID,-1,false,true));
		}


		void CIntervalThread::threadStarted() {
		}

		

		void CIntervalThread::threadStopped() {
			mSyncTimerMutex.lock();
			QList<CIntervalThreadData *> timerList = timers.values();
			foreach (CIntervalThreadData *timerData, timerList) {
				bool isActive = timerData->isTimerActive();
				if (isActive) {
					int timerThreadID = timerData->getTimerThreadID();
					QObject::killTimer(timerThreadID);
				}
				timerData->setTimerActive(false);
			}
			mSyncTimerMutex.unlock();
		}


		bool CIntervalThread::processEvents(QEvent *event) {
			QEvent *ev = event;
			if (CThread::processEvents(event)) {
				return true;
			} else if (event->type() == QEvent::Timer) {
				SETTASKDESCRIPTION("Process Timer Timeout");
				QTimerEvent *te = (QTimerEvent *)event;
				int timerThreadID = te->timerId();
				mSyncTimerMutex.lock();
				if (timerMapping.contains(timerThreadID)) {
					qint64 timerID = timerMapping.value(timerThreadID);
					if (timers.contains(timerID)) {
						CIntervalThreadData *timerData = timers.value(timerID);
						if (timerData->isTimerActive() && timerData->getDeactivateCount() <= 0) {
							if (timerData->hasRemainingTimerInvocations()) {
								timerData->decRemainingTimerInvocations(1);
								processTimer(timerID);
							} else {
								int timerThreadID = timerData->getTimerThreadID();
								QObject::killTimer(timerThreadID);
								timerData->setTimerActive(false);
							}
						}
					}
				}
				mSyncTimerMutex.unlock();
				return true;
			}
			return false;
		}


		bool CIntervalThread::processControlEvents(QEvent::Type type, CControlEvent *event) {
			if (CThread::processControlEvents(type,event)) {
				return true;
			} else if (type == EVENTTIMERINTERVAL) {
				SETTASKDESCRIPTION("Configure Timer Settings");
				CTimerIntervalEvent *tie = (CTimerIntervalEvent *)event;

				qint64 timerID = tie->getTimerID();
				qint64 timerInterval = tie->getTimerInterval();

				bool activateTimer = tie->getTimerActive();
				bool deactivateTimer = tie->getTimerDeactive();

				qint64 remTimerInvocs = tie->getRemainingTimerInvocations();


				CIntervalThreadData *timerData = 0;
				mSyncTimerMutex.lock();

				bool isNew = false;

				if (timers.contains(timerID)) {
					timerData = timers.value(timerID);
				} else {
					timerData = new CIntervalThreadData();
					timers.insert(timerID,timerData);
					isNew = true;
				}

				if (deactivateTimer) {
					timerData->decDeactivateCount();
				}

				bool reactivateTimer = false;

				if (timerInterval >= 0) {
					timerData->setInterval(timerInterval);
					bool isActive = timerData->isTimerActive();
					if (isActive) {
						reactivateTimer = true;
					}
				}
				timerData->setRemainingTimerInvocations(remTimerInvocs);

				if (deactivateTimer) {
					reactivateTimer = false;
					
					int timerThreadID = timerData->getTimerThreadID();
					bool isActive = timerData->isTimerActive();
					if (isActive) {
						QObject::killTimer(timerThreadID);
					}
					timerData->setTimerActive(false);
				} 
				
				if (activateTimer || reactivateTimer) {
					qint64 deactivateCount = timerData->getDeactivateCount();
					if (deactivateCount <= 0) {
						bool isActive = timerData->isTimerActive();
						int timerThreadID = timerData->getTimerThreadID();
						if (isActive) {
							QObject::killTimer(timerThreadID);
						}
						timerThreadID = QObject::startTimer(timerData->getInterval());
						timerData->setTimerThreadID(timerThreadID);
						timerData->setTimerActive(true);
						timerMapping.insert(timerThreadID,timerID);
					}
				}

				mSyncTimerMutex.unlock();
				return true;
			}

			return false;
		}



	}; // end namespace Concurrent

}; // end namespace Konclude
