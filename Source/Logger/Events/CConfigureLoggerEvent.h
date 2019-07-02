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
#ifndef LOGGER_EVENTS_CCONFIGURELOGGEREVENT_H
#define LOGGER_EVENTS_CCONFIGURELOGGEREVENT_H

#include <QEvent>
#include <QLinkedList>
#include <QStringList>

#include "Concurrent/Events/CCustomEvent.h"

#include "../LoggerSettings.h"
#include "../CLogMessage.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Events;

	namespace Logger {

		namespace Events {


			/*! 
			 *	\class		CConfigureLoggerEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CConfigureLoggerEvent : public CCustomEvent {
				// public methods
				public:
					//! Constructor
					CConfigureLoggerEvent(qint64 maxLogMessages, double minLogLevel);

					//! Destructor
					virtual ~CConfigureLoggerEvent();

					qint64 getMaxLogMessageCount();
					double getMinLoggingLevel();


				// protected methods
				protected:


				// private members
				private:
					qint64 mMaxLogMessageCount;
					double mMinLogLevel;


			};

		}; // end namespace Events

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_EVENTS_CCONFIGURELOGGEREVENT_H
