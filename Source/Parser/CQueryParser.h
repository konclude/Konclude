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

#ifndef KONCLUDE_PARSER_CQUERYPARSER_H
#define KONCLUDE_PARSER_CQUERYPARSER_H


// Libraries includes
#include <QString>


// Namespace includes
#include "CQueryBuilder.h"


// Other includes
#include "Expressions/CQueryIsClassSatisfiableExpression.h"
#include "Expressions/CQueryAreClassesEquivalentExpression.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {




		/*! 
		 *
		 *		\class		CQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CQueryParser {
			// public methods
			public:
				//! Constructor
				CQueryParser();

				//! Destructor
				virtual ~CQueryParser();

				virtual bool parseQueryFile(QString filename) = 0;

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

#endif // KONCLUDE_PARSER_CQueryParser_H
