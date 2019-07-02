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
