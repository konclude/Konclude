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

#include "CRealizationPremisingQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CRealizationPremisingQuery::CRealizationPremisingQuery(CConcreteOntology* ontology, CConfigurationBase* configuration) {
				mOntology = ontology;
				mConfig = configuration;
				mQueryStats = nullptr;
				mRequiresSameIndividualRealisation = false;
				mRequiresConceptRealisation = false;
				mRequiresRoleRealisation = false;
				mDynamicRealisation = false;
			}

			CRealizationPremisingQuery::~CRealizationPremisingQuery() {
				delete mQueryStats;
			}

			CConcreteOntology* CRealizationPremisingQuery::getOntology() {
				return mOntology;
			}

			CConfigurationBase* CRealizationPremisingQuery::getConfiguration() {
				return mConfig;
			}

			CRealizationPremisingQuery* CRealizationPremisingQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CRealizationPremisingQuery::getQueryStatistics() {
				return mQueryStats;
			}

			bool CRealizationPremisingQuery::isConceptRealisationRequired() {
				return mRequiresConceptRealisation;
			}

			bool CRealizationPremisingQuery::isRoleRealisationRequired() {
				return mRequiresRoleRealisation;
			}

			bool CRealizationPremisingQuery::isSameIndividualRealisationRequired() {
				return mRequiresSameIndividualRealisation;
			}

			bool CRealizationPremisingQuery::isDynamicRealisationRequired() {
				return mDynamicRealisation;
			}

			COntologyProcessingDynamicRealizationRequirement* CRealizationPremisingQuery::getDynamicRealizationRequirement() {
				return nullptr;
			}



		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
