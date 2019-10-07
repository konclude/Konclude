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

#ifndef KONCLUDE_EXPRESSION_FUNCTIONALJAVACC_FUNCTIONALJAVACCSETTINGS
#define KONCLUDE_EXPRESSION_FUNCTIONALJAVACC_FUNCTIONALJAVACCSETTINGS

// Libraries includes


// Namespace includes
#include "JavaCC.h"

// Other includes
#include "Parser/ParserSettings.h"
#include "Parser/COntologyBuilder.h"

#include "Parser/Expressions/CBuildExpression.h"
#include "Parser/Expressions/CDatatypeExpression.h"
#include "Parser/Expressions/CDataLexicalValueExpression.h"


// Logger includes


namespace Konclude {

	namespace Parser {

		using namespace Expression;

		namespace FunctionalJAVACC {





#ifndef JAVACC_TO_QSTRING_DEFINED
	#define JAVACC_TO_QSTRING_DEFINED
	#ifndef EXPLICIT_JAVACC_QSTRING_CONVERTION
		#define JAVACC_TO_QSTRING(s) QString::fromStdWString(s)
	#else

		class JAVACCToQStringConverter {
		public:
			static const QString convertJAVACCToQString(JAVACC_STRING_TYPE string) {
				QString qstring;
				qstring.reserve(string.size());
				for (JAVACC_STRING_TYPE::const_iterator it = string.cbegin(), itEnd = string.cend(); it != itEnd; ++it) {
					JAVACC_CHAR_TYPE character = *it;
					uint unicode = (uint)character;
					qstring.append(QChar(unicode));
				}
				return qstring;
			}
		};

		#define JAVACC_TO_QSTRING(s) JAVACCToQStringConverter::convertJAVACCToQString(s)
	#endif
#endif


			/*! 
			 *
			 *		\file		FunctionalJAVACCSettings
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */


			// forward declarations

			typedef CBuildExpression* CBuildExpressionPtr;
			typedef CDatatypeExpression* CDatatypeExpressionPtr;
			typedef CDataLexicalValueExpression* CDataLexicalValueExpressionPtr;


		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude


#endif // end KONCLUDE_EXPRESSION_FUNCTIONALJAVACC_FUNCTIONALJAVACCSETTINGS