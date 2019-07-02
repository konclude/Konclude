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
#include "CIntervalThreadData.h"


namespace Konclude {

	namespace Concurrent {


		CIntervalThreadData::CIntervalThreadData() {
			id = 0;
			timerID = 0;
			interval = 0;
			active = false;
			deactivateCount = 0;
			remInvocations = 0;
		}

		CIntervalThreadData::~CIntervalThreadData() {
		}


		void CIntervalThreadData::setTimerID(qint64 timerID) {
			id = timerID;
		}

		qint64 CIntervalThreadData::getTimerID() {
			return id;
		}

		void CIntervalThreadData::setInterval(qint64 timerInterval) {
			interval = timerInterval;
		}

		qint64 CIntervalThreadData::getInterval() {
			return interval;
		}

		void CIntervalThreadData::setTimerActive(bool timerIsActive) {
			active = timerIsActive;
		}

		bool CIntervalThreadData::isTimerActive() {
			return active;
		}

		void CIntervalThreadData::setTimerThreadID(qint64 timerThreadID) {
			timerID = timerThreadID;
		}

		qint64 CIntervalThreadData::getTimerThreadID() {
			return timerID;
		}

		void CIntervalThreadData::incDeactivateCount() {
			deactivateCount++;
		}

		void CIntervalThreadData::decDeactivateCount() {
			deactivateCount--;
			if (deactivateCount < 0) {
				deactivateCount = 0;
			}
		}

		qint64 CIntervalThreadData::getDeactivateCount() {
			return deactivateCount;
		}


		void CIntervalThreadData::setRemainingTimerInvocations(qint64 remInvoc) {
			remInvocations = remInvoc;
		}

		qint64 CIntervalThreadData::getRemainingTimerInvocations() {
			return remInvocations;
		}

		bool CIntervalThreadData::hasRemainingTimerInvocations() {
			return remInvocations > 0 || remInvocations == -1;
		}

		void CIntervalThreadData::decRemainingTimerInvocations(qint64 decCount) {
			if (remInvocations > 0) {
				remInvocations -= decCount;
				if (remInvocations < 0) {
					remInvocations = 0;
				}
			}
		}


	}; // end namespace Concurrent

}; // end namespace Konclude
