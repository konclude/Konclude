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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCBOXSATISFIABLEEVENT_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCBOXSATISFIABLEEVENT_H



// Libraries includes


// Namespace includes
#include "../ManagerSettings.h"


// Other includes
#include "Reasoner/Query/CSatisfiableCalculationJob.h"

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
					 *	\class		CReasoningSatisfiableCalculationJobEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CReasoningSatisfiableCalculationJobEvent : public CCustomEvent {
						// public methods
						public:
							//! Constructor
							CReasoningSatisfiableCalculationJobEvent(CSatisfiableCalculationJob *satCalcJob, CCallbackData *callback);

							//! Destructor
							virtual ~CReasoningSatisfiableCalculationJobEvent();

							CSatisfiableCalculationJob *getSatisfiableCalculationJob();
							CCallbackData *getCallbackData();

							static const QEvent::Type EVENTTYPE = EVENTREASONINGSATISFIABLECALCULATIONJOB;

						// protected methods
						protected:

						// protected variables
						protected:
							CSatisfiableCalculationJob *mSatCalcJob;
							CCallbackData *mCallbackData;


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

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_EVENTS_CCALCBOXSATISFIABLEEVENT_H
