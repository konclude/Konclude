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

#ifndef KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONNEXTEVENT_H
#define KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONNEXTEVENT_H



// Libraries includes
#include <QString>

// Namespace includes
#include "../EvaluationSettings.h"
#include "../CReasonerEvaluationTestsuite.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Concurrent/Callback/CCallbackData.h"
#include "Concurrent/CThread.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;
	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Test {

		namespace Evaluation {

			namespace Events {



				/*! 
				 *	\class		CReasonerEvaluationNextEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CReasonerEvaluationNextEvent : public CCustomEvent, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CReasonerEvaluationNextEvent(CThread* recieverThread, cint64 operationNumber);

						//! Destructor
						virtual ~CReasonerEvaluationNextEvent();

						static const QEvent::Type EVENTTYPE = EVENTREASONEREVALUATIONNEXT;

						virtual void doCallback();
						cint64 getOperationNumber();

					// protected methods
					protected:

					// protected variables
					protected:
						CThread* mRecieverThread;
						cint64 mOperationNumber;

					// private methods
					private:

					// private variables
					private:

				};


			}; // end namespace Events

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONNEXTEVENT_H
