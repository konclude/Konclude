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

#ifndef KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZEONTOLOGYEVENT_H
#define KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZEONTOLOGYEVENT_H



// Libraries includes


// Namespace includes
#include "../RealizerSettings.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {

			namespace Events {



				/*! 
				 *	\class		CRealizeOntologyEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CRealizeOntologyEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CRealizeOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callbackData = 0);
						CRealizeOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, CCallbackData* callbackData = 0);

						CConcreteOntology* getOntology();
						CCallbackData* getCallbackData();
						CConfigurationBase* getConfiguration();
						QList<COntologyProcessingRequirement*>* getRealizingRequirementList();

						static const QEvent::Type EVENTTYPE = EVENTPREALIZEONTOLOGY;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology* onto;
						CConfigurationBase* config;
						CCallbackData* callback;
						QList<COntologyProcessingRequirement*> mRequirementList;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_EVENTS_CREALIZEONTOLOGYEVENT_H
