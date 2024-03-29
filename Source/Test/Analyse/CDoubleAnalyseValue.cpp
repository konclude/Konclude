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

#include "CDoubleAnalyseValue.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CDoubleAnalyseValue::CDoubleAnalyseValue() {
			}

			CDoubleAnalyseValue::CDoubleAnalyseValue(double doubleValue) {
				doubleVal = doubleValue;
			}

			CDoubleAnalyseValue::~CDoubleAnalyseValue() {
			}


			QString CDoubleAnalyseValue::getString() {
				return QString::number(doubleVal);
			}

			bool CDoubleAnalyseValue::parseFromString(const QString &string) {
				bool ok = true;
				double number = string.toDouble(&ok);
				if (ok) {
					doubleVal = number;
				}
				return ok;
			}


			qint64 CDoubleAnalyseValue::getInteger() {
				return (qint64)doubleVal;
			}

			bool CDoubleAnalyseValue::parseFromInteger(qint64 integer) {
				doubleVal = integer;
				return true;
			}

			double CDoubleAnalyseValue::getDouble() {
				return doubleVal;
			}

			bool CDoubleAnalyseValue::parseFromDouble(double doubleValue) {
				doubleVal = doubleValue;
				return true;
			}

			CAnalyseValue *CDoubleAnalyseValue::createCopy() {
				CDoubleAnalyseValue *value = new CDoubleAnalyseValue(doubleVal);
				return value;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
