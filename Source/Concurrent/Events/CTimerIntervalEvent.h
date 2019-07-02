/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */
#ifndef KONCLUDE_CONCURRENT_EVENTS_CTIMERINTERVALEVENT_H
#define KONCLUDE_CONCURRENT_EVENTS_CTIMERINTERVALEVENT_H

#include <QEvent>
#include <QTimerEvent>

#include "CControlEvent.h"

#include "../ConcurrentSettings.h"

namespace Konclude {

	namespace Concurrent {

		namespace Events {


			/*! 
			 *	\class		CTimerIntervalEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CTimerIntervalEvent : public CControlEvent {
				// public methods
				public:
					//! Constructor
					CTimerIntervalEvent(qint64 timerID = 0, qint64 interval = -1, bool active = false, bool deactive = false, qint64 remInvocs = -1);

					//! Destructor
					virtual ~CTimerIntervalEvent();

					qint64 getTimerID();
					void setTimerID(qint64 timerID);

					void setTimerInterval(qint64 interval);
					qint64 getTimerInterval();

					void setTimerActive(bool active);
					bool getTimerActive();

					void setTimerDeactive(bool deactive);
					bool getTimerDeactive();

					void setRemainingTimerInvocations(qint64 remInvoc);
					qint64 getRemainingTimerInvocations();

				// protected methods
				protected:


				// private members
				private:
					qint64 id;
					qint64 tInt;
					qint64 remCall;
					bool tActive;
					bool tDeactive;

			};

		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_EVENTS_CTIMERINTERVALEVENT_H
