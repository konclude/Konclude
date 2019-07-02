/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_PARSER_COWLLINKQTXMLRESULTPARSER_H
#define KONCLUDE_PARSER_COWLLINKQTXMLRESULTPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "CStringPrimitiveConverter.h"


// Other includes
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CSubClassHierarchyQuery.h"
#include "Reasoner/Query/CClassesResult.h"
#include "Reasoner/Query/CQueryResult.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Reasoner::Query;

	namespace Parser {




		/*! 
		 *
		 *		\class		COWLlinkQtXMLResultParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class COWLlinkQtXMLResultParser {
			// public methods
			public:
				//! Constructor
				COWLlinkQtXMLResultParser();

				//! Destructor
				virtual ~COWLlinkQtXMLResultParser();

				CQueryResult *parseQueryResult(QDomElement *node);
				CBooleanQueryResult *parseBooleanQueryResult(QDomElement *node);
				CClassesResult *parseClassesResult(QDomElement *node);
				CClassHierarchyResult *parseClassHierarchyResult(QDomElement *node);
				CClassSynsetResult *parseClassSynsetResult(QDomElement *node);
				QStringList parseClassNames(QDomElement *node);


			// protected methods
			protected:
				bool hasTopClass(CClassSynsetResult *classSynset);

			// protected variables
			protected:


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLLINKQTXMLRESULTPARSER_H
