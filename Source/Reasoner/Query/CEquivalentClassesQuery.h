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

#ifndef KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESQUERY_H
#define KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CClassSetResult.h"
#include "CTaxonomyPremisingQuery.h"

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
			 *		\class		CEquivalentClassesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CEquivalentClassesQuery : public CTaxonomyPremisingQuery {
				// public methods
				public:
					//! Constructor
					CEquivalentClassesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, const QString& className, const QString& queryName = QString("UnnamedEquivalentClassesQuery"));

					//! Destructor
					virtual ~CEquivalentClassesQuery();

					virtual CQueryResult* constructResult(CTaxonomy *taxonomy);
					virtual CClassSetResult* constructClassesResult(CTaxonomy *taxonomy);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mClassName;
					CConcept* mConcept;
					QString mQueryName;
					QString mQueryString;

					CClassSetResult* mResult;

					bool mUseAbbreviatedIRIs;

					bool mQueryConstructError;
					bool mTaxonomyCalcError;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESQUERY_H
