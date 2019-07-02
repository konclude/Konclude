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

#ifndef KONCLUDE_PARSER_CPARSINGUTILITIES_H
#define KONCLUDE_PARSER_CPARSINGUTILITIES_H

// Libraries includes


// Namespace includes
#include <QString>
#include <QRegExp>

// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Utilities;

	namespace Parser {



		/*! 
		*
		*		\class		CParsingUtilities
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CParsingUtilities {
			// public methods
			public:
				//! Constructor
				CParsingUtilities();

				bool matchesLanguageTag(const QString& string);


				const QString DATATYPE_XML_STRING = QString("http://www.w3.org/2001/XMLSchema#string");
				const QString DATATYPE_RDF_PLAINLITERAL = QString("http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral");

			// protected methods
			protected:

			// protected variables
			protected:
				QRegExp mLanguageTagRegExp;



			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CPARSINGUTILITIES_H
