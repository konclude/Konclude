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

#ifndef KONCLUDE_REASONER_QUERY_CISCONSISTENTQUERY_H
#define KONCLUDE_REASONER_QUERY_CISCONSISTENTQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CBooleanQueryResult.h"
#include "CConsistencePremisingQuery.h"
#include "CQueryInconsitentOntologyError.h"

#include "Config/CConfigDataReader.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CIsConsistentQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIsConsistentQuery : public CConsistencePremisingQuery {
				// public methods
				public:
					//! Constructor
					CIsConsistentQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QString& consistentQueryName = QString("UnnamedConsistencyQuery"));

					//! Destructor
					virtual ~CIsConsistentQuery();

					virtual CQueryResult* constructResult(CConsistence* consistence);
					virtual CBooleanQueryResult* constructConsistenceResult(CConsistence* consistence);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual CQuery* addQueryError(CQueryError* queryError);

				// protected methods
				protected:

				// protected variables
				protected:
					QString queryName;
					QString queryString;

					CBooleanQueryResult* mResult;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CISCONSISTENTQUERY_H
