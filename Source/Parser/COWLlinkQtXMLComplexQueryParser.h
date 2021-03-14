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

#ifndef KONCLUDE_PARSER_COWLLINKQTXMLCOMPLEXQUERYPARSER_H
#define KONCLUDE_PARSER_COWLLINKQTXMLCOMPLEXQUERYPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "CQueryParser.h"
#include "COntologyQtXMLDocumentParser.h"
#include "COWLlinkQueryParser.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {




		/*! 
		 *
		 *		\class		COWLlinkQtXMLComplexQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLlinkQtXMLComplexQueryParser : public COWLlinkQueryParser {
			// public methods
			public:
				//! Constructor
				COWLlinkQtXMLComplexQueryParser(CQueryBuilder *queryBuilder, COntologyQtXMLDocumentParser* ontoParser);

				//! Destructor
				virtual ~COWLlinkQtXMLComplexQueryParser();


			// protected methods
			protected:

				virtual QList<CBuildExpression *> parseOntologyChildNodes(QDomElement *node);
				virtual QList<CBuildExpression*> parseOntologyChildNodesInverseHandling(QDomElement *node, bool& inversed);

			// private methods
			private:
				COntologyQtXMLDocumentParser* mOntoParser;


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLLINKQTXMLCOMPLEXQUERYPARSER_H
