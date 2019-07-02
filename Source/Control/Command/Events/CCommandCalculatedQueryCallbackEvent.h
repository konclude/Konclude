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

#ifndef KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDCALCULATEDQUERYCALLBACKEVENT_H
#define KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDCALCULATEDQUERYCALLBACKEVENT_H



// Libraries includes


// Namespace includes
#include "../CommandSettings.h"
#include "../CCommand.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Control {

		namespace Command {

			namespace Events {



				/*! 
				 *	\class		CCommandCalculatedQueryCallbackEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CCommandCalculatedQueryCallbackEvent : public CCustomEvent, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CCommandCalculatedQueryCallbackEvent(CThread *eventReceiver, CCommand *command);

						//! Destructor
						virtual ~CCommandCalculatedQueryCallbackEvent();

						virtual CCommand *getCommand();

						virtual void doCallback();

					// protected methods
					protected:

					// protected variables
					protected:
						CCommand *comm;
						CThread *receiver;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_COMMAND_EVENTS_CCOMMANDCALCULATEDQUERYCALLBACKEVENT_H
