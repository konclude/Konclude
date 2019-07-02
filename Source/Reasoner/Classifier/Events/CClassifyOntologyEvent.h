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

#ifndef KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCLASSIFYONTOLOGYEVENT_H
#define KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCLASSIFYONTOLOGYEVENT_H



// Libraries includes


// Namespace includes
#include "../ClassifierSettings.h"
#include "../CClassificationCalculationSupport.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"
#include "Concurrent/Callback/CCallbackData.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;
	using namespace Concurrent::Callback;
	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {

			namespace Events {



				/*! 
				 *	\class		CClassifyOntologyEvent
				 *	\author		Andreas Steigmiller
				 *	\version	0.1
				 *	\brief		
				 */
				class CClassifyOntologyEvent : public CCustomEvent {
					// public methods
					public:
						//! Constructor
						CClassifyOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callbackData = nullptr);

						//! Destructor
						virtual ~CClassifyOntologyEvent();

						virtual CConcreteOntology *getOntology();
						virtual QList<COntologyProcessingRequirement*>* getClassificationRequirementList();
						virtual CCallbackData *getCallbackData();
						virtual CConfigurationBase *getConfiguration();

						static const QEvent::Type EVENTTYPE = EVENTCLASSIFYONTOLOGY;

					// protected methods
					protected:

					// protected variables
					protected:
						CConcreteOntology *onto;
						CTaxonomy *tax;
						QList<COntologyProcessingRequirement*> mReqList;
						CConfigurationBase *config;
						CCallbackData *callback;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Events

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_EVENTS_CCLASSIFYONTOLOGYEVENT_H
