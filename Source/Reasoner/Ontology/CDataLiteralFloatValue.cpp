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

#include "CDataLiteralFloatValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralFloatValue::CDataLiteralFloatValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralFloatValue::getDataValueType() {
				return CDataLiteralValue::DLVT_FLOAT;
			}



			QString CDataLiteralFloatValue::getValueString() {
				return QString::number(mFloat);
			}


			CDataLiteralCompareValue* CDataLiteralFloatValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					mFloat = floatValue->mFloat;
				}
				return this;
			}


			CDataLiteralFloatValue* CDataLiteralFloatValue::initValueFromFloat(float floatValue) {
				clearValue();
				mFloat = floatValue;
				return this;
			}

			CDataLiteralFloatValue* CDataLiteralFloatValue::initValueFromInfinite(bool negative) {
				clearValue();
				if (negative) {
					mFloat = -std::numeric_limits<float>::infinity();
				} else {
					mFloat = std::numeric_limits<float>::infinity();
				}
				return this;
			}


			CDataLiteralFloatValue* CDataLiteralFloatValue::initValue(CDataLiteralFloatValue* value) {
				if (value) {
					mFloat = value->mFloat;
				}
				return this;
			}


			CDataLiteralFloatValue* CDataLiteralFloatValue::clearValue() {
				mFloat = 0.;
				return this;
			}


			float CDataLiteralFloatValue::getFloat() {
				return mFloat;
			}



			bool CDataLiteralFloatValue::isInfinite() {
				if (mFloat >= std::numeric_limits<float>::infinity()) {
					return true;
				}
				if (mFloat <= -std::numeric_limits<float>::infinity()) {
					return true;
				}
				return false;
			}

			bool CDataLiteralFloatValue::isNegative() {
				return mFloat < 0;
			}


			bool CDataLiteralFloatValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					return isEqualTo(floatValue);
				}
				return false;
			}



			bool CDataLiteralFloatValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					return isEqualTo(floatValue) || isLessThan(floatValue);
				}
				return false;
			}

			bool CDataLiteralFloatValue::isLessEqualThan(CDataLiteralFloatValue* floatValue) {
				return isEqualTo(floatValue) || isLessThan(floatValue);
			}


			bool CDataLiteralFloatValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					return isEqualTo(floatValue);
				}
				return false;
			}

			bool CDataLiteralFloatValue::isEqualTo(CDataLiteralFloatValue* floatValue) {
				return mFloat == floatValue->mFloat;
			}

			bool CDataLiteralFloatValue::isGreaterEqualThan(CDataLiteralFloatValue* floatValue) {
				return isEqualTo(floatValue) || isGreaterThan(floatValue);
			}

			bool CDataLiteralFloatValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					return isEqualTo(floatValue) || isGreaterThan(floatValue);
				}
				return false;
			}

			bool CDataLiteralFloatValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralFloatValue::isGreaterThan(CDataLiteralFloatValue* floatValue) {
				return floatValue->isLessThan(this);
			}


			bool CDataLiteralFloatValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralFloatValue* floatValue = dynamic_cast<CDataLiteralFloatValue*>(value);
				if (floatValue) {
					return isLessThan(floatValue);
				}
				return false;
			}


			bool CDataLiteralFloatValue::isLessThan(CDataLiteralFloatValue* floatValue) {
				if (mFloat < floatValue->mFloat) {
					return true;
				} else {
					return false;
				}
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
