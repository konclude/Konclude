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

#ifndef KONCLUDE_TEST_EVENTS_CTESTTESTFILEEVENT_H
#define KONCLUDE_TEST_EVENTS_CTESTTESTFILEEVENT_H



// Libraries includes


// Namespace includes
#include "../TestSettings.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Test {

		namespace Events {



			/*! 
			 *	\class		CTestTestfileEvent
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		
			 */
			class CTestTestfileEvent : public CCustomEvent {
				// public methods
				public:
					//! Constructor
					CTestTestfileEvent();

					//! Destructor
					virtual ~CTestTestfileEvent();


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Events

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVENTS_CTESTTESTFILEEVENT_H
