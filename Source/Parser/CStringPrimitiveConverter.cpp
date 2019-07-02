/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CStringPrimitiveConverter.h"


namespace Konclude {

	namespace Parser {



		CStringPrimitiveConverter::CStringPrimitiveConverter() {
		}


		CStringPrimitiveConverter::~CStringPrimitiveConverter() {
		}



		qint64 CStringPrimitiveConverter::convertStringToInteger(const QString &integerString, qint64 defaultValue, bool *parseErrorFlag) {			
			bool integerParseSuccFlag = false;
			qint64 integer = integerString.toLongLong(&integerParseSuccFlag);
			if (!integerParseSuccFlag) {
				if (parseErrorFlag) {
					*parseErrorFlag = true;
				}
				integer = defaultValue;
			}
			return integer;
		}


		bool CStringPrimitiveConverter::convertStringToBoolean(const QString &booleanString, bool defaultValue, bool *parseErrorFlag) {
			if (parseErrorFlag) {
				*parseErrorFlag = false;
			}
			bool integerConvertErrorFlag = false;
			qint64 integer = convertStringToInteger(booleanString,0,&integerConvertErrorFlag);
			if (!integerConvertErrorFlag) {
				return integer != 0;
			} else {
				if (booleanString.toUpper() == "TRUE") {
					return true;
				} else if (booleanString.toUpper() == "FALSE") {
					return false;
				} else {
					if (parseErrorFlag) {
						*parseErrorFlag = true;
					}
					return defaultValue;
				}
			}			
		}


	}; // end namespace Parser

}; // end namespace Konclude
