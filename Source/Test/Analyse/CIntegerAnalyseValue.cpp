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

#include "CIntegerAnalyseValue.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CIntegerAnalyseValue::CIntegerAnalyseValue() : CConvertIntegerAnalyseValue() {
			}

			CIntegerAnalyseValue::CIntegerAnalyseValue(qint64 integer) : CConvertIntegerAnalyseValue() {
				integerValue = integer;
			}

			CIntegerAnalyseValue::~CIntegerAnalyseValue() {
			}


			QString CIntegerAnalyseValue::getString() {
				return QString::number(integerValue);
			}

			bool CIntegerAnalyseValue::parseFromString(const QString &string) {
				bool ok = true;
				qint64 number = string.toLongLong(&ok);
				if (ok) {
					integerValue = number;
				}
				return ok;
			}


			qint64 CIntegerAnalyseValue::getInteger() {
				return integerValue;
			}

			bool CIntegerAnalyseValue::parseFromInteger(qint64 integer) {
				integerValue = integer;
				return true;
			}

			double CIntegerAnalyseValue::getDouble() {
				return (double)integerValue;
			}

			bool CIntegerAnalyseValue::parseFromDouble(double doubleValue) {
				integerValue = doubleValue;
				return true;
			}

			CAnalyseValue *CIntegerAnalyseValue::createCopy() {
				CIntegerAnalyseValue *value = new CIntegerAnalyseValue(integerValue);
				return value;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
