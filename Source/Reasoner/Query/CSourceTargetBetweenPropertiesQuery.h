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

#ifndef KONCLUDE_REASONER_QUERY_CSOURCETARGETBETWEENPROPERTIESQUERY_H
#define KONCLUDE_REASONER_QUERY_CSOURCETARGETBETWEENPROPERTIESQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CPropertySynsetsResult.h"
#include "CRealizationPremisingQuery.h"
#include "CPropertySynsetsResultVisitorGenerator.h"

#include "Config/CConfigDataReader.h"

// Other includes
#include "Reasoner/Ontology/COntologyProcessingRoleRealizationRequirement.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CSourceTargetBetweenPropertiesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSourceTargetBetweenPropertiesQuery : public CRealizationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CSourceTargetBetweenPropertiesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& sourceIndi, const QString& sourceIndiName, const CIndividualReference& targetIndi, const QString& targetIndiName, const QString& queryName = QString());

					//! Destructor
					virtual ~CSourceTargetBetweenPropertiesQuery();

					virtual CQueryResult* constructResult(CRealization* realization);
					virtual CPropertySynsetsResult* constructPropertySynsetsResult(CRoleRealization *roleRealization);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

					virtual COntologyProcessingDynamicRealizationRequirement* getDynamicRealizationRequirement();

				// protected methods
				protected:

				// protected variables
				protected:
					QString mSourceIndiName;
					QString mTargetIndiName;
					CIndividualReference mSourceIndi;
					CIndividualReference mTargetIndi;

					QString mQueryName;
					QString mQueryString;

					CPropertySynsetsResult* mResult;

					bool mUseAbbreviatedIRIs;

					bool mQueryConstructError;
					bool mRealizationCalcError;

					CConfigurationBase* mCalcConfig;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSOURCETARGETBETWEENPROPERTIESQUERY_H
