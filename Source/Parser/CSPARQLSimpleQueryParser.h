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

#ifndef KONCLUDE_PARSER_CSPARQLSIMPLEQUERYPARSER_H
#define KONCLUDE_PARSER_CSPARQLSIMPLEQUERYPARSER_H

// Libraries includes


// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"
#include "CSPARQLQueryBuilder.h"
#include "COntologyBuilder.h"
#include "CSPARQLSimpleBuildingParser.h"


// Other includes
#include "Parser/Expressions/CQuerySPARQLBasicGraphPatternExpression.h"
#include "Parser/Expressions/COrderingVariableExpression.h"

#include "Parser/Expressions/CFilteringTermExpression.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		 *
		 *		\class		CSPARQLSimpleQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLSimpleQueryParser : public CSPARQLSimpleBuildingParser {
			// public methods
			public:
				//! Constructor
				CSPARQLSimpleQueryParser(CSPARQLQueryBuilder *queryBuilder, COntologyBuilder* ontoBuilder, CConcreteOntology* ontology);

				//! Destructor
				virtual ~CSPARQLSimpleQueryParser();

				virtual bool parseQueryFile(const QString& filename);
				virtual bool parseQueryText(const QString& filetext);
				virtual bool parseQueryTextList(const QStringList& queryTextList);

			// protected methods
			protected:
				bool processParts(QStringList& parsedParts);

				QList<CFilteringTermExpression*> getFilters(QList<QStringList>& filterList);
				CBuildExpression* getFilterORExpression(QStringList& filterParts);
				CBuildExpression* getExpression(QStringList& filterParts);

				CFilteringComparisonDataLiteralExpression* getDataLiteralComparisonFilter(CDataLiteralVariableExpression* dataLitVar, CDataLiteralExpression* dataLiteral, const QString& compareOperatorString, bool reverse);

			// protected variables
			protected:
				CSPARQLQueryBuilder* mQueryBuilder;

				QList<CQuerySPARQLBasicGraphPatternExpression*> mSPARQLBGPQueryList;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLSIMPLEQUERYPARSER_H
