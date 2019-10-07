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

#ifndef KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZATIONMESSAGEEVENT_H
#define KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZATIONMESSAGEEVENT_H



// Libraries includes


// Namespace includes
#include "../RealizerSettings.h"
#include "../CRealizationMessageData.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Utilities/Memory/CMemoryPool.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {

			namespace Events {



				/*! 
				 *	\class		CRealizationMessageEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CRealizationMessageEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CRealizationMessageEvent(CConcreteOntology *ontology, CRealizationMessageData* messageData, CMemoryPool* memoryPool);

						CConcreteOntology* getOntology();
						CRealizationMessageData* getMessageData();
						CMemoryPool* getMemoryPool();

						static const QEvent::Type EVENTTYPE = EVENTREALIZATIONMESSAGE;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology* mOntology;
						CRealizationMessageData* mMessageData;
						CMemoryPool* mMemoryPool;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZATIONMESSAGEEVENT_H
