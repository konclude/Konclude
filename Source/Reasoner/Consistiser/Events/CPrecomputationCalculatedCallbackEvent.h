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

#ifndef KONCLUDE_REASONER_CONSISTISER_EVENTS_CPRECOMPUTATIONCALCULATEDCALLBACKEVENT_H
#define KONCLUDE_REASONER_CONSISTISER_EVENTS_CPRECOMPUTATIONCALCULATEDCALLBACKEVENT_H



// Libraries includes


// Namespace includes
#include "../ConsistiserSettings.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Query/CSatisfiableCalculationJob.h"
#include "Reasoner/Query/CJobSatisfiableCallbackContextData.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Query;
		using namespace Ontology;
		using namespace Kernel::Task;

		namespace Consistiser {

			namespace Events {



				/*! 
				 *	\class		CPrecomputationCalculatedCallbackEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CPrecomputationCalculatedCallbackEvent : public CCustomEvent, public CJobSatisfiableCallbackContextData, public CCallbackData {
					// public methods
					public:
						//! Constructor
						CPrecomputationCalculatedCallbackEvent(CThread* receiverThread, CSatisfiableCalculationJob* satCalcJob, CPrecomputationTestingItem* testingItem);

						//! Destructor
						virtual ~CPrecomputationCalculatedCallbackEvent();

						static const QEvent::Type EVENTTYPE = EVENTPRECOMPUTATIONCALCULATEDCALLBACK;

						virtual void doCallback();

						virtual bool getTestResultSatisfiable();
						virtual CSatisfiableCalculationJob* getSatisfiableCalculationJob();
						virtual CPrecomputationTestingItem* getTestingItem();

					// protected methods
					protected:

					// protected variables
					protected:
						CThread* mRecThread;
						CSatisfiableCalculationJob* mSatCalcJob;
						CPrecomputationTestingItem* mTestingItem;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_EVENTS_CPRECOMPUTATIONCALCULATEDCALLBACKEVENT_H
