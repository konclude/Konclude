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

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCallbackClassifiedOntologyEvent_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCallbackClassifiedOntologyEvent_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		CCallbackClassifiedOntologyEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CCallbackClassifiedOntologyEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CCallbackClassifiedOntologyEvent(CConcreteOntology *ontology, CCallbackData *callbackPossibleCallbackData, CCallbackData *classifiedCallbackData);

						//! Destructor
						virtual ~CCallbackClassifiedOntologyEvent();

						CConcreteOntology *getOntology();
						CCallbackData *getClassifiedCallbackData();
						CCallbackData *getCallbackPossibleCallbackData();

						static const QEvent::Type EVENTTYPE = EVENTCALLBACKCLASSIFIEDONTOLOGY;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology *onto;
						CCallbackData *classifiedCallback;
						CCallbackData *possibleCallback;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCallbackClassifiedOntologyEvent_H
