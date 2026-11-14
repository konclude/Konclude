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

#ifndef KONCLUDE_REASONER_QUERY_CDATAPROPERTYFACTSQUERY_H
#define KONCLUDE_REASONER_QUERY_CDATAPROPERTYFACTSQUERY_H

// Libraries includes
#include <QSet>
#include <QStringList>

// Namespace includes
#include "CRealizationPremisingQuery.h"
#include "CPropertySynsetsResult.h"
#include "CPropertySynsetResult.h"
#include "CIndividualSynsetsResult.h"
#include "CIndividualSynsetResult.h"
#include "CIndividualsResult.h"

// Other includes
#include "Reasoner/Ontology/CIndividualReference.h"
#include "Reasoner/Ontology/CDataLiteral.h"
#include "Reasoner/Ontology/CRole.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CDataPropertyFactsQuery
			 *
			 *		\brief		OWLlink data property fact requests that are answered from the realization.
			 *
			 *		The requests that share the argument kinds are handled by one query class with a
			 *		request kind:
			 *		- GetDataPropertiesOfSource(individual): the data property synsets of an individual,
			 *		- GetDataPropertiesBetween(individual, literal): the data property synsets between
			 *		  an individual and a literal,
			 *		- GetDataPropertiesOfLiteral(literal): the data property synsets of a literal,
			 *		- GetDataPropertySources(data property, literal): the individual synsets that have
			 *		  the value for the data property (with the negative flag as complement),
			 *		- GetFlattenedDataPropertySources(data property, literal): the same sources without
			 *		  the individual synsets.
			 *
			 *		owl:topDataProperty is part of every data property synset answer, because every
			 *		individual is related to every named literal by it (OWLlink specification).
			 *
			 */
			class CDataPropertyFactsQuery : public CRealizationPremisingQuery {
				// public methods
				public:
					enum DATAPROPERTYFACTSQUERYKIND {
						DPFQK_DATAPROPERTIESOFSOURCE,
						DPFQK_DATAPROPERTIESBETWEEN,
						DPFQK_DATAPROPERTIESOFLITERAL,
						DPFQK_DATAPROPERTYSOURCES,
						DPFQK_FLATTENEDDATAPROPERTYSOURCES
					};

					CDataPropertyFactsQuery(CConcreteOntology* ontology, CConfigurationBase* configuration, DATAPROPERTYFACTSQUERYKIND queryKind, const CIndividualReference& individualReference, const QString& individualName, CRole* dataRole, CDataLiteral* dataLiteral, bool negative, const QString& queryName = QString());

					virtual ~CDataPropertyFactsQuery();

					virtual CQueryResult* getQueryResult();

					virtual CQueryResult* constructResult(CRealization* realization);

					virtual QString getQueryName();
					virtual QString getQueryString();

					virtual bool hasAnswer();
					virtual QString getAnswerString();

					virtual bool hasError();

					virtual COntologyProcessingDynamicRealizationRequirement* getDynamicRealizationRequirement();

				// protected methods
				protected:
					CQueryResult* constructPropertySynsetsResult(CRealization* realization);
					CQueryResult* constructIndividualSynsetsResult(CRealization* realization, bool flattened);

					QString getDataRoleName(CRole* dataRole);
					QString getIndividualName(const CIndividualReference& indiRef);

					CPropertySynsetResult* getPropertySynset(CRealization* realization, CRole* dataRole);
					bool collectPropertySynsets(CRealization* realization, const QSet<CRole*>& dataRoleSet, CPropertySynsetsResult* result);
					CIndividualSynsetResult* getIndividualSynset(CRealization* realization, const CIndividualReference& indiRef);

					bool isDataLiteralInstance(CRealization* realization, CRole* dataRole, const CIndividualReference& indiRef);
					bool isTopDataRole(CRole* dataRole);

				// protected variables
				protected:
					DATAPROPERTYFACTSQUERYKIND mQueryKind;

					CIndividualReference mIndividualReference;
					QString mIndividualName;
					CRole* mDataRole;
					QString mDataRoleName;
					CDataLiteral* mDataLiteral;
					bool mNegative;

					QString queryName;
					QString queryString;

					CQueryResult* mResult;

					CConfigurationBase* mCalcConfig;

					bool mUseAbbreviatedIRIs;
					bool mAnonymousIndividuals;

					bool mRealizationCalcError;
					bool mQueryConstructError;

					QSet<QString> mAddedPropertySynsetKeySet;
					QSet<QString> mAddedIndividualSynsetKeySet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CDATAPROPERTYFACTSQUERY_H
