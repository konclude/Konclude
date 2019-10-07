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

#ifndef KONCLUDE_REASONER_QUERY_COBJECTDATAPROPERTYCLASSIFICATIONPREMISINGQUERY_H
#define KONCLUDE_REASONER_QUERY_COBJECTDATAPROPERTYCLASSIFICATIONPREMISINGQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CClassificationPremisingQuery.h"


// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CObjectDataPropertyClassificationPremisingQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CObjectDataPropertyClassificationPremisingQuery : public CClassificationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CObjectDataPropertyClassificationPremisingQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, bool dataPropertyQuery);

					//! Destructor
					virtual ~CObjectDataPropertyClassificationPremisingQuery();

					virtual bool isDataPropertyQuery();
					virtual bool isObjectPropertyQuery();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mDataPropertiesRolesQuery;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_COBJECTDATAPROPERTYCLASSIFICATIONPREMISINGQUERY_H
