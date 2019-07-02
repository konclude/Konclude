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
#ifndef KONCLUDE_CONCURRENT_CWATCHDOGTHREADDATA_H
#define KONCLUDE_CONCURRENT_CWATCHDOGTHREADDATA_H


#include <QString>


#include "CThread.h"


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *	\class		CWatchDogThreadData
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CWatchDogThreadData {
			// public methods
			public:
				//! Constructor
				CWatchDogThreadData();

				//! Destructor
				virtual ~CWatchDogThreadData();

				bool isRegisterThread();
				void setRegisterThread(bool isRegister);

				void setThreadName(QString threadName);
				QString getThreadName();

				void setThreadID(qint64 threadID);
				qint64 getThreadID();

				void setWaitTime(qint64 threadWaitTime);
				qint64 getWaitTime();

				void setRunTime(qint64 threadRunTime);
				qint64 getRunTime();

				CThread *getThread();
				void setThread(CThread *threadObject);

				void setThreadActive(bool threadIsActive);
				bool isThreadActive();

				void setUpdateNumber(qint64 number, QDateTime updateTime);
				qint64 getUpdateNumber();

				void setTaskDescription(QString description);
				QString getTaskDescription();

				void setThreadError(bool threadHasError);
				bool isThreadError();



			// protected methods
			protected:



			// private methods
			private:


			// private variables
			private:
				bool reg;
				QString name;
				qint64 id;
				qint64 waitTime;
				qint64 runTime;

				CThread *thread;

				bool active;
				bool error;
				qint64 updateNumber;

				QString taskDescription;

				QDateTime upTime;


		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CWATCHDOGTHREADDATA_H
