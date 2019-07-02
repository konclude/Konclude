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

#ifndef KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONSTARTEVENT_H
#define KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONSTARTEVENT_H



// Libraries includes
#include <QString>

// Namespace includes
#include "../EvaluationSettings.h"
#include "../CReasonerEvaluationTestsuite.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Test {

		namespace Evaluation {

			namespace Events {



				/*! 
				 *	\class		CReasonerEvaluationStartEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CReasonerEvaluationStartEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CReasonerEvaluationStartEvent(const QString& initFile, const QString& testFile, const QString& addressString, CConfiguration* configuration, CCallbackData* callbackData);

						//! Destructor
						virtual ~CReasonerEvaluationStartEvent();

						static const QEvent::Type EVENTTYPE = EVENTREASONEREVALUATIONSTART;

						QString getInitFileString();
						QString getTestFileString();
						QString getAddressString();
						CConfiguration* getConfiguration();
						CCallbackData* getCallback();

					// protected methods
					protected:

					// protected variables
					protected:
						QString mInitFileString;
						QString mTestFileString;
						QString mAddressString;
						CConfiguration* mConfiguration;
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

#endif // KONCLUDE_TEST_EVALUATION_EVENTS_CREASONEREVALUATIONSTARTEVENT_H
