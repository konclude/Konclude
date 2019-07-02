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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CRETRIEVEINCOMPLETELYASSOCIATIONCACHEDEVENT_H
#define KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CRETRIEVEINCOMPLETELYASSOCIATIONCACHEDEVENT_H

// Libraries includes
#include <QEvent>
#include <QList>

// Namespace includes


// Other includes
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CIndividualReference.h"

#include "Concurrent/Events/CCustomEvent.h"

#include "Concurrent/Callback/CCallbackData.h"

#include "../CacheSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				namespace Events {


					/*! 
					 *
					 *		\class		CRetrieveIncompletelyAssociationCachedEvent
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CRetrieveIncompletelyAssociationCachedEvent : public Concurrent::Events::CCustomEvent {
						// public methods
						public:
							//! Constructor
							CRetrieveIncompletelyAssociationCachedEvent(CCallbackData* callbackData, cint64 ontologyIdentifier, QSet<CIndividualReference>* individualSet, cint64 limit);

							static const QEvent::Type EVENTTYPE = EVENTRETRIEVEINCOMPLETELYASSOCIATIONCACHED;


							CCallbackData* getCallback();

							QSet<CIndividualReference>* getIndividualSet();
							cint64 getRetrievalLimit();

							cint64 getOntologyIdentifier();

						// protected methods
						protected:

						// protected variables
						protected:
							cint64 mOntologyIdentifier;
							CCallbackData* mCallback;
							QSet<CIndividualReference>* mIndividualSet;
							cint64 mLimit;

						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Events

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_EVENTS_CRETRIEVEINCOMPLETELYASSOCIATIONCACHEDEVENT_H
