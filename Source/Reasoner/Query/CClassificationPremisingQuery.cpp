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

#include "CClassificationPremisingQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CClassificationPremisingQuery::CClassificationPremisingQuery(CConcreteOntology* ontology, CConfigurationBase* configuration) {
				mOntology = ontology;
				mConfig = configuration;
				mQueryStats = nullptr;
				mRequiresConceptClassification = false;
				mRequiresObjectRoleClassification = false;
				mRequiresDataRoleClassification = false;
			}

			CClassificationPremisingQuery::~CClassificationPremisingQuery() {
				delete mQueryStats;
			}

			CConcreteOntology* CClassificationPremisingQuery::getOntology() {
				return mOntology;
			}

			CConfigurationBase* CClassificationPremisingQuery::getConfiguration() {
				return mConfig;
			}

			CClassificationPremisingQuery* CClassificationPremisingQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CClassificationPremisingQuery::getQueryStatistics() {
				return mQueryStats;
			}

			bool CClassificationPremisingQuery::isConceptClassificationRequired() {
				return mRequiresConceptClassification;
			}

			bool CClassificationPremisingQuery::isObjectRoleClassificationRequired() {
				return mRequiresObjectRoleClassification;
			}

			bool CClassificationPremisingQuery::isDataRoleClassificationRequired() {
				return mRequiresDataRoleClassification;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
