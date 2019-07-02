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
#ifndef KONCLUDE_CONCURRENT_CINTERVALTHREAD_H
#define KONCLUDE_CONCURRENT_CINTERVALTHREAD_H


#include <QThread>
#include <QSemaphore>
#include <QTime>
#include <QTimer>
#include <QHash>
#include <QMutex>

#include "CIntervalThreadData.h"
#include "CThread.h"

#include "Events/CTimerIntervalEvent.h"

namespace Konclude {

	namespace Concurrent {


		/*! 
		 *	\class		CIntervalThread
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CIntervalThread : public CThread {
			// public methods
			public:
				//! Constructor
				CIntervalThread(QString threadIdentifierName, CWatchDog *watchDog = 0);

				//! Destructor
				virtual ~CIntervalThread();


				void setTimerInterval(qint64 timerID, qint64 timerIntervalMilliSecs);
				void startTimerWithInterval(qint64 timerID, qint64 timerIntervalMilliSecs);
				void startTimerWithIntervalLimited(qint64 timerID, qint64 timerIntervalMilliSecs, qint64 remainingCallCount);
				void startTimer(qint64 timerID);
				void stopTimer(qint64 timerID);



			// protected methods
			protected:

				virtual bool processTimer(qint64 timerID) = 0;

				bool processEvents(QEvent *event);
				virtual bool processControlEvents(QEvent::Type type, CControlEvent *event);

				virtual void threadStarted();
				virtual void threadStopped();


			// private methods
			private:


			// private variables
			private:
				QMutex mSyncTimerMutex;
				QHash<qint64,CIntervalThreadData *> timers;
				QHash<qint64,qint64> timerMapping;

		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CINTERVALTHREAD_H
