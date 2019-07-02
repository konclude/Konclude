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
#include "CTimerIntervalEvent.h"


namespace Konclude {

	namespace Concurrent {

		namespace Events {


			CTimerIntervalEvent::CTimerIntervalEvent(qint64 timerID, qint64 interval, bool active, bool deactive, qint64 remInvocs) 
					: CControlEvent(EVENTTIMERINTERVAL) {
				id = timerID;
				tInt = interval;
				tActive = active;
				tDeactive = deactive;
				remCall = remInvocs;
			}

			CTimerIntervalEvent::~CTimerIntervalEvent() {
			}

			qint64 CTimerIntervalEvent::getTimerID() {
				return id;
			}

			void CTimerIntervalEvent::setTimerID(qint64 timerID) {
				id = timerID;
			}

			void CTimerIntervalEvent::setTimerInterval(qint64 interval) {
				tInt = interval;
			}

			qint64 CTimerIntervalEvent::getTimerInterval() {
				return tInt;
			}

			void CTimerIntervalEvent::setTimerActive(bool active) {
				tActive = active;
			}

			bool CTimerIntervalEvent::getTimerActive() {
				return tActive;
			}

			void CTimerIntervalEvent::setTimerDeactive(bool deactive) {
				tDeactive = deactive;
			}

			bool CTimerIntervalEvent::getTimerDeactive() {
				return tDeactive;
			}

			void CTimerIntervalEvent::setRemainingTimerInvocations(qint64 remInvoc) {
				remCall = remInvoc;
			}

			qint64 CTimerIntervalEvent::getRemainingTimerInvocations() {
				return remCall;
			}


		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude
