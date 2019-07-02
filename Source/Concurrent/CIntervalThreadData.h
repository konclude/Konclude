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
#ifndef KONCLUDE_CONCURRENT_CINTERVALTHREADDATA_H
#define KONCLUDE_CONCURRENT_CINTERVALTHREADDATA_H


#include <QString>


namespace Konclude {

	namespace Concurrent {


		/*! 
		 *	\class		CIntervalThreadData
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CIntervalThreadData {
			// public methods
			public:
				//! Constructor
				CIntervalThreadData();

				//! Destructor
				virtual ~CIntervalThreadData();


				void setTimerID(qint64 timerID);
				qint64 getTimerID();

				void setInterval(qint64 threadInterval);
				qint64 getInterval();

				void setTimerActive(bool threadIsActive);
				bool isTimerActive();

				void setTimerThreadID(qint64 timerThreadID);
				qint64 getTimerThreadID();

				void incDeactivateCount();
				void decDeactivateCount();
				qint64 getDeactivateCount();

				void setRemainingTimerInvocations(qint64 remInvoc);
				qint64 getRemainingTimerInvocations();
				bool hasRemainingTimerInvocations();
				void decRemainingTimerInvocations(qint64 decCount = 1);

			// protected methods
			protected:



			// private methods
			private:


			// private variables
			private:
				qint64 interval;
				bool active;
				qint64 id;
				qint64 timerID;
				qint64 deactivateCount;
				qint64 remInvocations;



		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CINTERVALTHREADDATA_H
