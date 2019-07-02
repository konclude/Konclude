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

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CSATURATIONCALCULATEDCALLBACKEVENT_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CSATURATIONCALCULATEDCALLBACKEVENT_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"
#include "../CClassificationWorkItem.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Query/CSaturationCalculationJob.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;
		using namespace Kernel::Task;

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		CSaturationCalculatedCallbackEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CSaturationCalculatedCallbackEvent : public CCustomEvent, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CSaturationCalculatedCallbackEvent(CThread *receiverThread, CSaturationCalculationJob *satCalcJob);

						//! Destructor
						virtual ~CSaturationCalculatedCallbackEvent();

						static const QEvent::Type EVENTTYPE = EVENTSATURATIONCALCULATEDCALLBACK;

						virtual void doCallback();

						CSaturationCalculationJob *getSatisfiableCalculationJob();


					// protected methods
					protected:

					// protected variables
					protected:
						CThread *recThread;
						CSaturationCalculationJob* mSatCalcJob;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CSATURATIONCALCULATEDCALLBACKEVENT_H
