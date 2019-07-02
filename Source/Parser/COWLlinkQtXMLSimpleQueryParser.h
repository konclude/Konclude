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

#ifndef KONCLUDE_PARSER_COWLLINKQTXMLSIMPLEQUERYPARSER_H
#define KONCLUDE_PARSER_COWLLINKQTXMLSIMPLEQUERYPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "CQueryParser.h"
#include "COWLlinkQueryParser.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Generator/CStringRefStringHasher.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Ontology;
	using namespace Reasoner::Generator;

	namespace Parser {




		/*! 
		 *
		 *		\class		COWLlinkQtXMLSimpleQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLlinkQtXMLSimpleQueryParser : public COWLlinkQueryParser {
			// public methods
			public:
				//! Constructor
				COWLlinkQtXMLSimpleQueryParser(CQueryBuilder *queryBuilder, CConcreteOntology* conOntology);

				//! Destructor
				virtual ~COWLlinkQtXMLSimpleQueryParser();


			// protected methods
			protected:

				virtual QList<CBuildExpression *> parseOntologyChildNodes(QDomElement *node);
				virtual QList<CBuildExpression*> parseOntologyChildNodesInverseHandling(QDomElement *node, bool& inversed);
				QString getEntityStringName(QDomElement *node);
				QString resolveAbbreviationName(const QString& name);

			// private methods
			private:
				CConcreteOntology* mConOntology;


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLLINKQTXMLSIMPLEQUERYPARSER_H
