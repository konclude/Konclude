/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_CONSISTISER_EVENTS_CCALLBACKPRECOMPUTEDONTOLOGYEVENT_H
#define KONCLUDE_REASONER_CONSISTISER_EVENTS_CCALLBACKPRECOMPUTEDONTOLOGYEVENT_H



// Libraries includes


// Namespace includes
#include "../ConsistiserSettings.h"


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

		namespace Consistiser {

			namespace Events {



				/*! 
				 *	\class		CCallbackPrecomputedOntologyEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CCallbackPrecomputedOntologyEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CCallbackPrecomputedOntologyEvent(CConcreteOntology* ontology, CCallbackData* callbackPossibleCallbackData, CCallbackData* precomputedCallbackData);

						//! Destructor
						virtual ~CCallbackPrecomputedOntologyEvent();

						CConcreteOntology* getOntology();
						CCallbackData* getPrecomputedCallbackData();
						CCallbackData* getCallbackPossibleCallbackData();

						static const QEvent::Type EVENTTYPE = EVENTCALLBACKPRECOMPUTEDONTOLOGY;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology* onto;
						CCallbackData* precomputedCallback;
						CCallbackData* possibleCallback;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_EVENTS_CCALLBACKPRECOMPUTEDONTOLOGYEVENT_H
