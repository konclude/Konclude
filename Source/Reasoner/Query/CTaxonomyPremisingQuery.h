/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_QUERY_CTAXONOMYPREMISINGQUERY_H
#define KONCLUDE_REASONER_QUERY_CTAXONOMYPREMISINGQUERY_H

// Libraries includes


// Namespace includes
#include "CQuery.h"
#include "CTaxonomyPremisingQuerySupport.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CTaxonomyPremisingQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTaxonomyPremisingQuery : public CQuery {
				// public methods
				public:
					//! Constructor
					CTaxonomyPremisingQuery(CConcreteOntology *premisingTaxonomyOntology, CConfigurationBase *configuration);

					//! Destructor
					virtual ~CTaxonomyPremisingQuery();

					virtual CConcreteOntology *getOntology();
					virtual CQueryResult *constructResult(CTaxonomy *taxonomy) = 0;
					virtual CConfigurationBase *getConfiguration();

					virtual CQuerySupport *getQuerySupport();
					virtual CTaxonomyPremisingQuerySupport *getTaxonomyQuerySupport();

					CTaxonomyPremisingQuery* setQueryStatistics(CQueryStatistics* stats);
					virtual CQueryStatistics* getQueryStatistics();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology *ontology;
					CConfigurationBase *config;
					CQueryStatistics* mQueryStats;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CTAXONOMYPREMISINGQUERY_H
