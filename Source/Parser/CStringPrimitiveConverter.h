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

#ifndef KONCLUDE_PARSER_CSTRINGPRIMITIVECONVERTER_H
#define KONCLUDE_PARSER_CSTRINGPRIMITIVECONVERTER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {




		/*! 
		 *
		 *		\class		CStringPrimitiveConverter
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for OWL/XML Files
		 *
		 */
		class CStringPrimitiveConverter {
			// public methods
			public:
				//! Constructor
				CStringPrimitiveConverter();

				//! Destructor
				virtual ~CStringPrimitiveConverter();

				static bool convertStringToBoolean(const QString &booleanString, bool defaultValue = 0, bool *parseErrorFlag = 0);
				static qint64 convertStringToInteger(const QString &integerString, qint64 defaultValue = false, bool *parseErrorFlag = 0);

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

#endif // KONCLUDE_PARSER_CSTRINGPRIMITIVECONVERTER_H
