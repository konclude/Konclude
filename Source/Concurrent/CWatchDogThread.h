/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef KONCLUDE_CONCURRENT_CWATCHDOGTHREAD_H
#define KONCLUDE_CONCURRENT_CWATCHDOGTHREAD_H


#include <QSemaphore>
#include <QReadWriteLock>
#include <QHash>
#include <QDateTime>
#include <QLinkedList>

#include "CThread.h"
#include "CIntervalThread.h"
#include "CWatchDogThreadData.h"
#include "CWatchDog.h"
#include "ConcurrentSettings.h"


#include "Events/CCustomEvent.h"
#include "Events/CControlEvent.h"



namespace Konclude {

	namespace Concurrent {

		using namespace Events;


		/*! 
		 *	\class		CWatchDogThread
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CWatchDogThread : public CIntervalThread, public CWatchDog {

			// public methods
			public:
				//! Constructor
				CWatchDogThread();

				//! Destructor
				virtual ~CWatchDogThread();


				static CWatchDogThread *getInstance();

				bool registerThread(CThread *thread);
				void feedbackThread(CThread *thread, qint64 updateNumber);
				void checkoutThread(CThread *thread);
				void crashedThread(CThread *thread);

				QLinkedList<CWatchDogThreadData> getActiveThreadList();


			// protected methods
			protected:

				virtual void threadStarted();

				virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

				virtual bool processTimer(qint64 timerID);

			// private methods
			private:

				void sendRequestFeedbackEvent(CThread *thread, qint64 uNr);
				void updateThreadData(CThread *thread, CWatchDogThreadData *threadData);


			// private variables
			private:
				static const qint64 REQUESTFEEDBACKTIMERID = 1;
				static const qint64 UPDATETHREADINFOTIMERID = 2;

				static CWatchDogThread *watchDogInstance;
				static QSemaphore *syncSemaphore;

				QReadWriteLock regSync;
				QHash<qint64,CWatchDogThreadData *> threadWatch;

				qint64 currentUpdateNumber;

		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CWATCHDOGTHREAD_H
