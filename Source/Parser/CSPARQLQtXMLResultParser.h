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

#ifndef KONCLUDE_PARSER_CSPARQLQtXMLResultParser_H
#define KONCLUDE_PARSER_CSPARQLQtXMLResultParser_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>
#include <QVector>

// Namespace includes
#include "CStringPrimitiveConverter.h"


// Other includes
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersListResult.h"
#include "Reasoner/Query/CVariableBindingResult.h"
#include "Reasoner/Query/CVariableBindingStringResult.h"
#include "Reasoner/Query/CVariableBindingsListAnswerResult.h"
#include "Reasoner/Query/CVariableBindingsAnswersSetResult.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Reasoner::Query;

	namespace Parser {




		/*! 
		 *
		 *		\class		CSPARQLQtXMLResultParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CSPARQLQtXMLResultParser {
			// public methods
			public:
				//! Constructor
				CSPARQLQtXMLResultParser();

				//! Destructor
				virtual ~CSPARQLQtXMLResultParser();

				CQueryResult *parseQueryResult(QDomElement *node);

				CVariableBindingsAnswersSetResult* parseVariableBindings(QDomElement *node, QHash<QString, cint64>* variableNameIndexHash = nullptr, bool locallyRenameBlankNamesBindings = false);

			// protected methods
			protected:

			// protected variables
			protected:


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLQtXMLResultParser_H
