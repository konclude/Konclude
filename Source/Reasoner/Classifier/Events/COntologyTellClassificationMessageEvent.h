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

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCLASSIFICATIONMESSAGEEVENT_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCLASSIFICATIONMESSAGEEVENT_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"
#include "../CClassificationMessageData.h"


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

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		COntologyTellClassificationMessageEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class COntologyTellClassificationMessageEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						COntologyTellClassificationMessageEvent(CConcreteOntology *ontology, CClassificationMessageData* messageData, CMemoryPool* memoryPool);

						CConcreteOntology* getOntology();
						CClassificationMessageData* getMessageData();
						CMemoryPool* getMemoryPool();

						static const QEvent::Type EVENTTYPE = EVENTONTOLOGYTELLCLASSIFICATIONMESSAGE;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology* mOntology;
						CClassificationMessageData* mMessageData;
						CMemoryPool* mMemoryPool;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CONTOLOGYTELLCLASSIFICATIONMESSAGEEVENT_H
