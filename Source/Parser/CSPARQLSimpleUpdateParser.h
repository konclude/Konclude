/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_PARSER_CSPARQLSIMPLEUPDATEPARSER_H
#define KONCLUDE_PARSER_CSPARQLSIMPLEUPDATEPARSER_H

// Libraries includes


// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"
#include "COntologyBuilder.h"
#include "CSPARQLSimpleBuildingParser.h"


// Other includes
#include "Parser/Expressions/CQuerySPARQLBasicGraphPatternExpression.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		using namespace Expression;




		/*! 
		 *
		 *		\class		CSPARQLSimpleUpdateParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLSimpleUpdateParser : public CSPARQLSimpleBuildingParser {
			// public methods
			public:
				//! Constructor
				CSPARQLSimpleUpdateParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology);

				//! Destructor
				virtual ~CSPARQLSimpleUpdateParser();

				virtual bool parseUpdateFile(const QString& filename);
				virtual bool parseUpdateText(const QString& filetext);
				virtual bool parseUpdateTextList(const QStringList& queryTextList);

			// protected methods
			protected:
				bool processParts(QStringList& parsedParts);


			// protected variables
			protected:


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLSIMPLEUPDATEPARSER_H
