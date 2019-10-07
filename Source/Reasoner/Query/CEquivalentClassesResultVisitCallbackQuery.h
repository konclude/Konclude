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

#ifndef KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESRESULTVISITCALLBACKQUERY_H
#define KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESRESULTVISITCALLBACKQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CSucceedQueryResult.h"
#include "CEntityExpressionSetResultVisitingCallback.h"
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
			 *		\class		CSubSuperEquivalentClassesResultVisitCallbackQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CEquivalentClassesResultVisitCallbackQuery : public CTaxonomyPremisingQuery {
				// public methods
				public:
					//! Constructor
					CEquivalentClassesResultVisitCallbackQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CConcept* concept, const QString& className, const QString& queryName = QString("UnnamedQuery"));

					//! Destructor
					virtual ~CEquivalentClassesResultVisitCallbackQuery();

					virtual CQueryResult* constructResult(CTaxonomy *taxonomy);
					virtual bool callbackVisitingClasses(CEntityExpressionSetResultVisitingCallback* visitingCallback);

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

					CSucceedQueryResult* mResult;

					bool mQueryConstructError;
					bool mTaxonomyCalcError;

					CHierarchyNode* mNode;

					CConfigurationBase* mCalcConfig;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CEQUIVALENTCLASSESRESULTVISITCALLBACKQUERY_H
