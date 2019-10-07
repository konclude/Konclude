/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "CWatchDogFeedbackEvent.h"


namespace Konclude {

	namespace Concurrent {

		namespace Events {


			CWatchDogFeedbackEvent::CWatchDogFeedbackEvent(qint64 threadID, qint64 updateNumber) 
					: CControlEvent(EVENTFEEDBACKTHREADWATCHDOG) {
				id = threadID;
				uNr = updateNumber;
			}

			CWatchDogFeedbackEvent::~CWatchDogFeedbackEvent() {
			}

			qint64 CWatchDogFeedbackEvent::getThreadID() {
				return id;
			}

			void CWatchDogFeedbackEvent::seThreadID(qint64 threadID) {
				id = threadID;
			}

			void CWatchDogFeedbackEvent::setUpdateNumber(qint64 updateNumber) {
				uNr = updateNumber;
			}

			qint64 CWatchDogFeedbackEvent::getUpdateNumber() {
				return uNr;
			}


		}; // end namespace Events

	}; // end namespace Concurrent

}; // end namespace Konclude
