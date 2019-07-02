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

#include "CPreprocessOntologyEvent.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {

			namespace Events {


				CPreprocessOntologyEvent::CPreprocessOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callbackData) 
						: CCustomEvent(EVENTTYPE),mRequirementList(requirementList) {
					onto = ontology;
					callback = callbackData;
					config = configuration;
				}

				CPreprocessOntologyEvent::CPreprocessOntologyEvent(CConcreteOntology* ontology, CConfigurationBase* configuration, CCallbackData* callbackData) 
						: CCustomEvent(EVENTTYPE) {
					onto = ontology;
					callback = callbackData;
					config = configuration;
				}


				CPreprocessOntologyEvent::~CPreprocessOntologyEvent() {
				}


				CConcreteOntology* CPreprocessOntologyEvent::getOntology() {
					return onto;
				}

				CCallbackData* CPreprocessOntologyEvent::getCallbackData() {
					return callback;
				}

				CConfigurationBase* CPreprocessOntologyEvent::getConfiguration() {
					return config;
				}

				QList<COntologyProcessingRequirement*>* CPreprocessOntologyEvent::getPreprocessingRequirementList() {
					return &mRequirementList;
				}


			}; // end namespace Events

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
