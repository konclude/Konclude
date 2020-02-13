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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_QUERY_CREDLANDRASQALBGPSCOMPOSITIONQUERY_H
#define KONCLUDE_REASONER_QUERY_CREDLANDRASQALBGPSCOMPOSITIONQUERY_H

// Libraries includes
#include <Redland.h>

// Namespace includes
#include "CComplexAnsweringCompositionQuery.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfigurationBase.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Ontology;

		namespace Query {

			typedef QPair<CConcreteOntology*, CConcreteOntology*> TDependentOntologyPair;

			/*! 
			 *
			 *		\class		CRedlandRasqalBGPsCompositionQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandRasqalBGPsCompositionQuery : public CComplexAnsweringCompositionQuery {
				// public methods
				public:
					//! Constructor
					CRedlandRasqalBGPsCompositionQuery(CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology, const QSet<QString>& dependentKBNames, const QString& queryText, librdf_world* redlandWorld, librdf_query* redlandQuery, rasqal_query* rasqalQuery, CConfigurationBase* configuration, const QString& queryName = "Unknown-Rasqal-Query");

					//! Destructor
					virtual ~CRedlandRasqalBGPsCompositionQuery();


					QSet<QString>* getDependentKnowledgeBaseNames();

					librdf_world* getRedlandWorld();
					librdf_query* getRedlandQuery();
					rasqal_query* getRasqalQuery();

					QString getQueryText();


					QHash<QString, TDependentOntologyPair>* getDependentKnowledgeBaseRevisionHash();
					CRedlandRasqalBGPsCompositionQuery* setDependentKnowledgeBaseRevision(const QString& kbName, TDependentOntologyPair depOntPair);
					CRedlandRasqalBGPsCompositionQuery* setDependentKnowledgeBaseRevision(const QString& kbName, CConcreteOntology* baseOntology, CConcreteOntology* expressionOntology);


					CConcreteOntology* getDependentBaseKnowledgeBaseRevision(const QString& kbName);
					CConcreteOntology* getDependentExpressionKnowledgeBaseRevisionBase(const QString& kbName);


				// protected methods
				protected:

				// protected variables
				protected:
					QHash<QString, TDependentOntologyPair> mDependentKnowledgeBaseRevisionHash;
					QSet<QString> mDependentKBNames;

					librdf_world* mRedlandWorld;
					librdf_query* mRedlandQuery;
					rasqal_query* mRasqalQuery;


					QString mQueryText;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CREDLANDRASQALBGPSCOMPOSITIONQUERY_H

#endif // KONCLUDE_REDLAND_INTEGRATION
