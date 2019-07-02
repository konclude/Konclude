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
#include "CConfigureLoggerEvent.h"

namespace Konclude {

	namespace Logger {

		namespace Events {

			CConfigureLoggerEvent::CConfigureLoggerEvent(qint64 maxLogMessages, double minLogLevel) 
					: CCustomEvent(EVENTCONFIGURELOGGER) {

				mMaxLogMessageCount = maxLogMessages;
				mMinLogLevel = minLogLevel;
			}

			CConfigureLoggerEvent::~CConfigureLoggerEvent() {
			}

			qint64 CConfigureLoggerEvent::getMaxLogMessageCount() {
				return mMaxLogMessageCount;
			}

			double CConfigureLoggerEvent::getMinLoggingLevel() {
				return mMinLogLevel;
			}


		}; // end namespace Events

	}; // end namespace Logger

}; // end namespace Konclude