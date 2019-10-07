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

#ifndef KONCLUDE_REASONER_QUERY_CCLASSIFICATIONPREMISINGQUERY_H
#define KONCLUDE_REASONER_QUERY_CCLASSIFICATIONPREMISINGQUERY_H

// Libraries includes


// Namespace includes
#include "CQuery.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Classification/CClassification.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Classification;

		namespace Query {

			/*! 
			 *
			 *		\class		CClassificationPremisingQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationPremisingQuery : public CQuery {
				// public methods
				public:
					//! Constructor
					CClassificationPremisingQuery(CConcreteOntology* ontology, CConfigurationBase* configuration);

					//! Destructor
					virtual ~CClassificationPremisingQuery();

					virtual CConcreteOntology* getOntology();
					virtual CQueryResult* constructResult(CClassification* classificationData) = 0;
					virtual CConfigurationBase* getConfiguration();

					CClassificationPremisingQuery* setQueryStatistics(CQueryStatistics* stats);
					virtual CQueryStatistics* getQueryStatistics();

					bool isConceptClassificationRequired();
					bool isObjectRoleClassificationRequired();
					bool isDataRoleClassificationRequired();

				// protected methods
				protected:

				// protected variables
				protected:
					CConcreteOntology* mOntology;
					CConfigurationBase* mConfig;
					CQueryStatistics* mQueryStats;

					bool mRequiresConceptClassification;
					bool mRequiresObjectRoleClassification;
					bool mRequiresDataRoleClassification;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CCLASSIFICATIONPREMISINGQUERY_H
