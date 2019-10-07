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

#ifndef KONCLUDE_REASONER_QUERY_CDETERMINISTICNONDETERMINISTICINDIVIDUALSQUERY_H
#define KONCLUDE_REASONER_QUERY_CDETERMINISTICNONDETERMINISTICINDIVIDUALSQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "CQuery.h"
#include "CIndividualsResult.h"
#include "CRealizationPremisingQuery.h"

#include "Config/CConfigDataReader.h"

#include "Reasoner/Kernel/Process/CProcessingDataBox.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"

#include "Reasoner/Realization/CIndividualConceptAssertionSetCollector.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Kernel::Process;
		using namespace Realization;

		namespace Query {

			/*! 
			 *
			 *		\class		CDeterministicNondeterministicIndividualsQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CDeterministicNondeterministicIndividualsQuery : public CRealizationPremisingQuery {
				// public methods
				public:
					//! Constructor
					CDeterministicNondeterministicIndividualsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, const QSet<CIndividual*>& individualSet, const QString& queryName = QString("UnnamedIndividualsQuery"));

					//! Destructor
					virtual ~CDeterministicNondeterministicIndividualsQuery();

					virtual CQueryResult* constructResult(CRealization* realization);
					virtual CIndividualsResult* constructIndividualsResult(CRealization* realization);

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual CQueryResult* getQueryResult();

					virtual bool hasError();

				// protected methods
				protected:
					virtual bool addIndividualResultData(CIndividual* indi, CIndividualProcessNodeVector* indiNodeVec, QSet< QPair<CIndividual*,CConcept*> >* possAssSet);

				// protected variables
				protected:
					QString mQueryName;
					QString mQueryString;

					CIndividualsResult* mResult;

					bool mUseAbbreviatedIRIs;

					bool mQueryConstructError;
					bool mConsistencyCalcError;

					CConfigurationBase* mCalcConfig;

					QSet<CIndividual*> mIndividualSet;
					bool mDeterministicIndividuals;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CDETERMINISTICNONDETERMINISTICINDIVIDUALSQUERY_H
