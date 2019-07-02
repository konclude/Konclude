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
#ifndef KONCLUDE_CONCURRENT_CTHREAD_H
#define KONCLUDE_CONCURRENT_CTHREAD_H


#include <QThread>
#include <QSemaphore>
#include <QTime>
#include <QCoreApplication>


#include "CWatchDog.h"
#include "ConcurrentSettings.h"

#include "Events/CCustomEvent.h"
#include "Events/CControlEvent.h"


#include "Events/CWaitSynchronizationEvent.h"


namespace Konclude {

	namespace Concurrent {

		using namespace Events;

//#define THREADTIMEMEASURE 


#ifdef DEBUG_THREADS
		#define SETTASKDESCRIPTION(a) setTaskDescription(a)
#else
		#define SETTASKDESCRIPTION(a) 
#endif

#ifdef THREADTIMEMEASURE
		#define TIMEMEASUREMENTBEGINWAITING() prepareWaiting()
		#define TIMEMEASUREMENTENDWAITING() concludeWaiting()
		#define TIMEMEASUREMENTBEGINEXECUTION() prepareExecution()
		#define TIMEMEASUREMENTENDEXECUTION() concludeExecution()
		#define TIMEMEASUREMENTBEGINBLOCKING() prepareBlocking()
		#define TIMEMEASUREMENTENDBLOCKING() concludeBlocking()
#else
		#define TIMEMEASUREMENTBEGINWAITING()
		#define TIMEMEASUREMENTENDWAITING()
		#define TIMEMEASUREMENTBEGINEXECUTION()
		#define TIMEMEASUREMENTENDEXECUTION()
		#define TIMEMEASUREMENTBEGINBLOCKING()
		#define TIMEMEASUREMENTENDBLOCKING()
#endif

		class CWatchDog;


		/*! 
		 *	\class		CThread
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CThread : public QThread {

			// public methods
			public:
				//! Constructor
				CThread(QString threadIdentifierName, CWatchDog *watchDogThread = 0);

				//! Destructor
				virtual ~CThread();


				void startThread(Priority priority = InheritPriority);
				void stopThread(bool waitStopped = true);

				void waitSynchronization();


				QString getThreadName();
				QString getLastTaskDescription();
				QString getTaskDescription();
				qint64 getWaitTimeSecs();
				qint64 getRunTimeSecs();
				qint64 getBlockTimeSecs();
				qint64 getThreadID();


				//! Terminates and restarts the thread. NOTE: because of semaphores, mutex, ... this function is very critical
				void restartThread();

				void postEvent(QEvent *event, int priority = Qt::NormalEventPriority);


				bool isThreadRunning();


			// protected methods
			protected:

				virtual void run();
				virtual bool event(QEvent *event);


				virtual void prepareBlocking();
				virtual void concludeBlocking();

				virtual void prepareExecution();
				virtual void concludeExecution();

				virtual void prepareWaiting();
				virtual void concludeWaiting();

				virtual void threadStarted();
				virtual void threadStopped();

				virtual bool processEvents(QEvent *event);

				virtual bool processControlEvents(QEvent::Type type, CControlEvent *event);
				virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


				void setTaskDescription(QString description);

				bool isThreadProcessingEvents();

			// protected methods
			protected:
				bool mActiveEventProcessing;

			// private methods
			private:


			// private variables
			private:

				QString threadName;
				QString taskDescription;
				QString lastTaskDescription;
				qint64 threadID;

				bool threadRuns;

				// workload measurement
				qint64 waitTimeSecs;
				qint64 runTimeSecs;
				qint64 blockTimeSecs;
				QTime runTimer;
				QTime waitTimer;
				QTime blockTimer;

				static qint64 nextThreadID;
				static QSemaphore *syncThreadID;
				CWatchDog *mWatchDog;
		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CTHREAD_H
