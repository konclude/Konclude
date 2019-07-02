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

#include "CTaxonomyPremisingQuery.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			CTaxonomyPremisingQuery::CTaxonomyPremisingQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration) {
				ontology = premisingTaxonomyOntology;
				config = configuration;
				mQueryStats = nullptr;
			}

			CTaxonomyPremisingQuery::~CTaxonomyPremisingQuery() {
				delete mQueryStats;
			}

			CConcreteOntology *CTaxonomyPremisingQuery::getOntology() {
				return ontology;
			}

			CConfigurationBase *CTaxonomyPremisingQuery::getConfiguration() {
				return config;
			}

			CQuerySupport *CTaxonomyPremisingQuery::getQuerySupport() {
				return getTaxonomyQuerySupport();
			}

			CTaxonomyPremisingQuerySupport *CTaxonomyPremisingQuery::getTaxonomyQuerySupport() {
				return 0;
			}

			CTaxonomyPremisingQuery* CTaxonomyPremisingQuery::setQueryStatistics(CQueryStatistics* stats) {
				mQueryStats = stats;
				return this;
			}

			CQueryStatistics* CTaxonomyPremisingQuery::getQueryStatistics() {
				return mQueryStats;
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude
