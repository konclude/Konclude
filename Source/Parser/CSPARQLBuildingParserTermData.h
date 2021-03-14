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

#ifndef KONCLUDE_PARSER_CSPARQLBUILDINGPARSERTERMDATA_H
#define KONCLUDE_PARSER_CSPARQLBUILDINGPARSERTERMDATA_H

// Libraries includes


// Namespace includes
#include "ParserSettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {


	namespace Parser {

		/*! 
		 *
		 *		\class		CSPARQLBuildingParserTermData
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CSPARQLBuildingParserTermData {
			// public methods
			public:
				//! Constructor
				CSPARQLBuildingParserTermData(const QString& term);

				enum TermType {
					UNKNON_TYPE,

					VARIABLE_TYPE,
					CLASS_VARIABLE_TYPE,
					PROPERTY_VARIABLE_TYPE,
					OBJECT_PROPERTY_VARIABLE_TYPE,
					DATA_PROPERTY_VARIABLE_TYPE,
					INDIVIDUAL_VARIABLE_TYPE,
					NAMED_INDIVIDUAL_VARIABLE_TYPE,
					DATA_LITERAL_VARIABLE_TYPE,

					BLANK_NODE_TYPE,
					ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE,

					RESOURCE_TYPE,
					CLASS_TYPE,
					PROPERTY_TYPE,
					OBJECT_PROPERTY_TYPE,
					DATA_PROPERTY_TYPE,
					NAMED_INDIVIDUAL_TYPE,
					ANONYMOUS_INDIVIDUAL_TYPE,

					DATA_LITERAL_TYPE

				};

				QString getTermString();
				TermType getTermType();

				CSPARQLBuildingParserTermData* setTermType(TermType type);

				bool isCompletelyTyped();


				bool isClassTerm();
				bool isPropertyTerm();
				bool isDataPropertyTerm();
				bool isObjectPropertyTerm();
				bool isIndividualTerm();
				bool isLiteralTerm();

				bool isVariableTerm();
				bool isNonVariableTerm();

				QList<CSPARQLBuildingParserTripleData*>* getOccuringTripleList();
				CSPARQLBuildingParserTermData* addOccuringTriple(CSPARQLBuildingParserTripleData* tripleData);

			// protected methods
			protected:

			// protected variables
			protected:
				QString mTermString;
				TermType mTermType;

				QList<CSPARQLBuildingParserTripleData*> mOccuringTripleList;

			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CSPARQLBUILDINGPARSERTERMDATA_H
