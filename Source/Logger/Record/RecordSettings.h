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

#ifndef UUPR_LOGGER_SETTINGS_H
#define UUPR_LOGGER_SETTINGS_H

#include <QEvent>


namespace UUPR {

	namespace Logger {

		#define LOGGER_DEBUG


		/*! \file
		 *	\version	0.1
		 *	\author		Andreas Steigmiller
		 *	\brief		Settings for Logger
		 */
		const QEvent::Type EVENTLOG = (QEvent::Type)1200;
		const QEvent::Type EVENTLOGMESSAGESREQUEST = (QEvent::Type)1201;
		const QEvent::Type EVENTSAVELOG = (QEvent::Type)1202;


		enum LogMessageTyp { LMINFORMATION = 0, LMWARNING = 1, LMERROR = 2 };


		const QEvent::Type EVENTLOGMESSAGE				= (QEvent::Type)3000;
		const QEvent::Type EVENTREQUESTLOGMESSAGES		= (QEvent::Type)3001;
		const QEvent::Type EVENTRELEASELOGMESSAGES		= (QEvent::Type)3002;


	}; // end namespace Logger

}; // end namespace UUPR

#endif // UUPR_LOGGER_SETTINGS_H