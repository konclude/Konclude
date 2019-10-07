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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCQUERYEVENT_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCQUERYEVENT_H



// Libraries includes


// Namespace includes
#include "../ManagerSettings.h"


// Other includes
#include "Reasoner/Query/CQuery.h"

#include "Concurrent/Events/CCustomEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Reasoner {

		using namespace Query;

		namespace Kernel {

			namespace Manager {

				namespace Events {



					/*! 
					 *	\class		CCalcQueryEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CCalcQueryEvent : public CCustomEvent {
						// public methods
						public:
							//! Constructor
							CCalcQueryEvent(CQuery *query, CCallbackData *callback);

							//! Destructor
							virtual ~CCalcQueryEvent();

							CQuery *getQuery();
							CCallbackData *getCallbackData();

							static const QEvent::Type EVENTTYPE = EVENTCALCQUERY;

						// protected methods
						protected:

						// protected variables
						protected:
							CQuery *query;
							CCallbackData *callbackData;


						// private methods
						private:

						// private variables
						private:

					};

				}; // end namespace Events

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCQUERYEVENT_H
