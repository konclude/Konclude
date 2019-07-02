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
#ifndef LOGGER_SETTINGS_H
#define LOGGER_SETTINGS_H

#include <QEvent>

namespace Konclude {

	namespace Concurrent {

		/*! 
		 *	\file
		 *	\version	0.1
		 *	\author		Andreas Steigmiller
		 *	\brief		Event Types for CThreads
		 */

		// forward declarations
		class CEvent;

#ifdef _DEBUG
		#define DEBUG_THREADS
#endif


		// Control Events >= 1200 && < 2000
		const QEvent::Type EVENTREQUESTFEEDBACKWATCHDOG		= (QEvent::Type)1231;
		const QEvent::Type EVENTWAITSYNCHRONIZATION			= (QEvent::Type)1232;

		const QEvent::Type EVENTTIMERINTERVAL				= (QEvent::Type)1233;
		const QEvent::Type EVENTFEEDBACKTHREADWATCHDOG		= (QEvent::Type)1234;

		const QEvent::Type EVENTHANDLEEVENTS				= (QEvent::Type)1235;


		// Custom Events >= 2000



	}; // end namespace Concurrent

}; // end namespace Konclude


#endif // LOGGER_SETTINGS_H