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

#ifndef KONCLUDE_REASONER_QUERY_CTYPESQUERY_H
#define KONCLUDE_REASONER_QUERY_CTYPESQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CClassSynsetsResult.h"
#include "CRealizationPremisingQuery.h"
#include "CClassSynsetsResultVisitorGenerator.h"

#include "Config/CConfigDataReader.h"

// Other includes
#include "Reasoner/Ontology/COntologyProcessingConceptRealizationRequirement.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			/*! 
			 *
			 *		\class		CTypesQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTypesQuery : public CRealizationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CTypesQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const CIndividualReference& indi, bool direct, const QString& individualName, const QString& flatternedTypeQueryName = QString("UnnamedTypesQuery"));

					//! Destructor
					virtual ~CTypesQuery();

					virtual CQueryResult* constructResult(CRealization* realization);
					virtual CClassSynsetsResult* constructClassSynsetsResult(CRealization* realization);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

					bool isDirect();

					virtual COntologyProcessingDynamicRealizationRequirement* getDynamicRealizationRequirement();


				// protected methods
				protected:

				// protected variables
				protected:
					QString mIndividualName;
					CIndividualReference mIndividual;
					QString queryName;
					QString queryString;
					bool mDirect;

					CClassSynsetsResult* mResult;

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

#endif // KONCLUDE_REASONER_QUERY_CTYPESQUERY_H
