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
#ifndef KONCLUDE_CONCURRENT_EVENTS_CREQUESTFEEDBACKEVENT_H
#define KONCLUDE_CONCURRENT_EVENTS_CREQUESTFEEDBACKEVENT_H

#include <QEvent>
#include <QThread>


#include "../ConcurrentSettings.h"
#include "../CWatchDog.h"



#include "CControlEvent.h"


namespace Konclude {

	namespace Concurrent {

		namespace Events {



			/*! 
			 *	\class		CRequestFeedbackEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CRequestFeedbackEvent : public CControlEvent {
				// public methods
				public:
					//! Constructor
					CRequestFeedbackEvent();
					CRequestFeedbackEvent(CWatchDog *watchDogThread, qint64 number);

					//! Destructor
					virtual ~CRequestFeedbackEvent();

					CWatchDog *getWatchDogThread();
					void setWatchDogThread(CWatchDog *watchDogThread);

					qint64 getUpdateNumber();
					void setUpdateNumber(qint64 number);


				// protected methods
				protected:


				// private members
				private:
					CWatchDog *watchDog;
					qint64 updateNumber;

			};

		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_EVENTS_CREQUESTFEEDBACKEVENT_H
