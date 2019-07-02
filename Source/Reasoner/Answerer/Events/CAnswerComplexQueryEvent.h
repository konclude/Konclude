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

#ifndef KONCLUDE_REASONER_ANSWERER_EVENTS_CANSWERCOMPLEXQUERYEVENT_H
#define KONCLUDE_REASONER_ANSWERER_EVENTS_CANSWERCOMPLEXQUERYEVENT_H



// Libraries includes


// Namespace includes
#include "../AnswererSettings.h"
#include "../CAnsweringHandler.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Utilities/Memory/CMemoryPool.h"

#include "Reasoner/Query/CComplexAnsweringQuery.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Query;

		namespace Answerer {

			namespace Events {



				/*! 
				 *	\class		CAnswerComplexQueryEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CAnswerComplexQueryEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CAnswerComplexQueryEvent(CComplexAnsweringQuery* complexQuery, CCallbackData* callbackData);

						CComplexAnsweringQuery* getComplexQuery();
						CCallbackData* getCallbackData();

						static const QEvent::Type EVENTTYPE = EVENTANSWERCOMPLEXQUERY;

					// protected methods
					protected:

					// protected variables
					protected:
						CComplexAnsweringQuery* mComplexQuery;
						CCallbackData* mCallbackData;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_EVENTS_CANSWERCOMPLEXQUERYEVENT_H
