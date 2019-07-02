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

#ifndef KONCLUDE_REASONER_QUERY_CSUPERCLASSESQUERY_H
#define KONCLUDE_REASONER_QUERY_CSUPERCLASSESQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CClassSynsetsResult.h"
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
			 *		\class		CSuperClassesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSuperClassesQuery : public CTaxonomyPremisingQuery {
				// public methods
				public:
					//! Constructor
					CSuperClassesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, bool direct, const QString& className, const QString& queryName = QString("UnnamedSuperClassesQuery"));

					//! Destructor
					virtual ~CSuperClassesQuery();

					virtual CQueryResult* constructResult(CTaxonomy *taxonomy);
					virtual CClassSynsetsResult* constructClassSynsetsResult(CTaxonomy *taxonomy);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

					bool isDirect();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mClassName;
					CConcept* mConcept;
					QString mQueryName;
					QString mQueryString;
					bool mDirect;

					CClassSynsetsResult* mResult;

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

#endif // KONCLUDE_REASONER_QUERY_CSUPERCLASSESQUERY_H
