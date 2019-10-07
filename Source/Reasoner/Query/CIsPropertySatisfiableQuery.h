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

#ifndef KONCLUDE_REASONER_QUERY_CISPROPERTYSATISFIABLEQUERY_H
#define KONCLUDE_REASONER_QUERY_CISPROPERTYSATISFIABLEQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CBooleanQueryResult.h"
#include "CObjectDataPropertyClassificationPremisingQuery.h"

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
			 *		\class		CIsPropertySatisfiableQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIsPropertySatisfiableQuery : public CObjectDataPropertyClassificationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CIsPropertySatisfiableQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, CRole* role, const QString& propertyName, bool dataProperty, const QString& queryName = QString());

					//! Destructor
					virtual ~CIsPropertySatisfiableQuery();

					virtual CQueryResult* constructResult(CClassification* classification);
					virtual CBooleanQueryResult* constructPropertySynsetsResult(CRolePropertiesHierarchy *hierarchy);

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
					QString mPropertyName;
					CRole* mRole;
					QString mQueryName;
					QString mQueryString;

					CBooleanQueryResult* mResult;

					bool mUseAbbreviatedIRIs;

					bool mQueryConstructError;
					bool mClassificationCalcError;

					CConfigurationBase* mCalcConfig;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CISPROPERTYSATISFIABLEQUERY_H
