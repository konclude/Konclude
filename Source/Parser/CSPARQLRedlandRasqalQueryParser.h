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


#ifndef KONCLUDE_PARSER_CSPARQLREDLANDRASQALQUERYPARSER_H
#define KONCLUDE_PARSER_CSPARQLREDLANDRASQALQUERYPARSER_H

// Libraries includes
#include <Redland.h>


// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"
#include "CSPARQLQueryBuilder.h"
#include "COntologyBuilder.h"
#include "CSPARQLSimpleBuildingParser.h"
#include "CSPARQLQueryParser.h"


// Other includes
#include "Reasoner/Query/CRedlandRasqalBGPsCompositionQuery.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Query;

	namespace Parser {


		/*! 
		 *
		 *		\class		CSPARQLRedlandRasqalQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLRedlandRasqalQueryParser : public CSPARQLQueryParser {
			// public methods
			public:
				//! Constructor
				CSPARQLRedlandRasqalQueryParser(CConcreteOntology* baseOntology, CConcreteOntology* ontology, CConfigurationBase* config);

				//! Destructor
				virtual ~CSPARQLRedlandRasqalQueryParser();

				virtual bool parseQueryFile(const QString& filename);
				virtual bool parseQueryText(const QString& filetext);
				virtual bool parseQueryTextList(const QStringList& queryTextList);
				virtual bool parseQueryTextList(const QStringList& queryTextList, const QString& queryName);

				CQuery* getQuery();

			// protected methods
			protected:
				bool processQueryText(const QString& queryText, const QString& queryName = "Unknown-Rasqal-Query");
				
			// protected variables
			protected:
				CConcreteOntology* mBaseOntology;
				CConcreteOntology* mOntology;
				CConfigurationBase* mConfig;

				CRedlandRasqalBGPsCompositionQuery* mQuery;

			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLREDLANDRASQALQUERYPARSER_H

#endif // KONCLUDE_REDLAND_INTEGRATION
