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

#include "CDataLiteralDoubleValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralDoubleValue::CDataLiteralDoubleValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralDoubleValue::getDataValueType() {
				return CDataLiteralValue::DLVT_DOUBLE;
			}



			CDataLiteralCompareValue* CDataLiteralDoubleValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					mDouble = doubleValue->mDouble;
				}
				return this;
			}

			QString CDataLiteralDoubleValue::getValueString() {
				return QString::number(mDouble);
			}



			CDataLiteralDoubleValue* CDataLiteralDoubleValue::initValueFromDouble(double doubleValue) {
				clearValue();
				mDouble = doubleValue;
				return this;
			}


			CDataLiteralDoubleValue* CDataLiteralDoubleValue::initValueFromInfinite(bool negative) {
				clearValue();
				if (negative) {
					mDouble = -std::numeric_limits<double>::infinity();
				} else {
					mDouble = std::numeric_limits<double>::infinity();
				}
				return this;
			}

			CDataLiteralDoubleValue* CDataLiteralDoubleValue::initValue(CDataLiteralDoubleValue* value) {
				if (value) {
					mDouble = value->mDouble;
				}
				return this;
			}


			CDataLiteralDoubleValue* CDataLiteralDoubleValue::clearValue() {
				mDouble = 0.;
				return this;
			}


			double CDataLiteralDoubleValue::getDouble() {
				return mDouble;
			}


			bool CDataLiteralDoubleValue::isInfinite() {
				if (mDouble >= std::numeric_limits<double>::infinity()) {
					return true;
				}
				if (mDouble <= -std::numeric_limits<double>::infinity()) {
					return true;
				}
				return false;
			}

			bool CDataLiteralDoubleValue::isNegative() {
				return mDouble < 0;
			}

			bool CDataLiteralDoubleValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					return isEqualTo(doubleValue);
				}
				return false;
			}



			bool CDataLiteralDoubleValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					return isEqualTo(doubleValue) || isLessThan(doubleValue);
				}
				return false;
			}

			bool CDataLiteralDoubleValue::isLessEqualThan(CDataLiteralDoubleValue* doubleValue) {
				return isEqualTo(doubleValue) || isLessThan(doubleValue);
			}


			bool CDataLiteralDoubleValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					return isEqualTo(doubleValue);
				}
				return false;
			}

			bool CDataLiteralDoubleValue::isEqualTo(CDataLiteralDoubleValue* doubleValue) {
				return mDouble == doubleValue->mDouble;
			}

			bool CDataLiteralDoubleValue::isGreaterEqualThan(CDataLiteralDoubleValue* doubleValue) {
				return isEqualTo(doubleValue) || isGreaterThan(doubleValue);
			}

			bool CDataLiteralDoubleValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					return isEqualTo(doubleValue) || isGreaterThan(doubleValue);
				}
				return false;
			}

			bool CDataLiteralDoubleValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralDoubleValue::isGreaterThan(CDataLiteralDoubleValue* doubleValue) {
				return doubleValue->isLessThan(this);
			}


			bool CDataLiteralDoubleValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralDoubleValue* doubleValue = dynamic_cast<CDataLiteralDoubleValue*>(value);
				if (doubleValue) {
					return isLessThan(doubleValue);
				}
				return false;
			}


			bool CDataLiteralDoubleValue::isLessThan(CDataLiteralDoubleValue* doubleValue) {
				if (mDouble < doubleValue->mDouble) {
					return true;
				} else {
					return false;
				}
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
