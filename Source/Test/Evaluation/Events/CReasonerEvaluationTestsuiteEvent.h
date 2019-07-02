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

#ifndef KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONTESTSUITEEVENT_H
#define KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONTESTSUITEEVENT_H



// Libraries includes


// Namespace includes
#include "../EvaluationSettings.h"
#include "../CReasonerEvaluationTestsuite.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Test {

		namespace Evaluation {

			namespace Events {



				/*! 
				 *	\class		CReasonerEvaluationTestsuiteEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CReasonerEvaluationTestsuiteEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CReasonerEvaluationTestsuiteEvent(CReasonerEvaluationTestsuite* testsuite, CCallbackData* callbackData);

						//! Destructor
						virtual ~CReasonerEvaluationTestsuiteEvent();

						static const QEvent::Type EVENTTYPE = EVENTREASONEREVALUATIONTESTSUITE;

						CReasonerEvaluationTestsuite* getEvaluationTestsuite();
						CCallbackData* getCallbackData();

					// protected methods
					protected:

					// protected variables
					protected:
						CReasonerEvaluationTestsuite* mEvalTestsuite;
						CCallbackData* mCallbackData;

					// private methods
					private:

					// private variables
					private:

				};
			
			}; // end namespace Events

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONTESTSUITEEVENT_H
