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

#include "CDataLiteralBooleanValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralBooleanValue::CDataLiteralBooleanValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralBooleanValue::getDataValueType() {
				return CDataLiteralValue::DLVT_BOOLEAN;
			}



			CDataLiteralCompareValue* CDataLiteralBooleanValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					mBoolean = booleanValue->mBoolean;
				}
				return this;
			}


			QString CDataLiteralBooleanValue::getValueString() {
				if (mBoolean) {
					return "true";
				} else {
					return "false";
				}
			}


			CDataLiteralBooleanValue* CDataLiteralBooleanValue::initValueFromBoolean(bool boolean) {
				clearValue();
				mBoolean = boolean;
				return this;
			}


			CDataLiteralBooleanValue* CDataLiteralBooleanValue::initValue(CDataLiteralBooleanValue* value) {
				if (value) {
					mBoolean = value->mBoolean;
				}
				return this;
			}


			CDataLiteralBooleanValue* CDataLiteralBooleanValue::clearValue() {
				mBoolean = false;
				return this;
			}


			bool CDataLiteralBooleanValue::getBoolean() {
				return mBoolean;
			}



			bool CDataLiteralBooleanValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					return isEqualTo(booleanValue);
				}
				return false;
			}



			bool CDataLiteralBooleanValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					return isEqualTo(booleanValue) || isLessThan(booleanValue);
				}
				return false;
			}

			bool CDataLiteralBooleanValue::isLessEqualThan(CDataLiteralBooleanValue* booleanValue) {
				return isEqualTo(booleanValue) || isLessThan(booleanValue);
			}


			bool CDataLiteralBooleanValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					return isEqualTo(booleanValue);
				}
				return false;
			}

			bool CDataLiteralBooleanValue::isEqualTo(CDataLiteralBooleanValue* booleanValue) {
				return mBoolean == booleanValue->mBoolean;
			}

			bool CDataLiteralBooleanValue::isGreaterEqualThan(CDataLiteralBooleanValue* booleanValue) {
				return isEqualTo(booleanValue) || isGreaterThan(booleanValue);
			}

			bool CDataLiteralBooleanValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					return isEqualTo(booleanValue) || isGreaterThan(booleanValue);
				}
				return false;
			}

			bool CDataLiteralBooleanValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralBooleanValue::isGreaterThan(CDataLiteralBooleanValue* booleanValue) {
				return booleanValue->isLessThan(this);
			}


			bool CDataLiteralBooleanValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralBooleanValue* booleanValue = dynamic_cast<CDataLiteralBooleanValue*>(value);
				if (booleanValue) {
					return isLessThan(booleanValue);
				}
				return false;
			}


			bool CDataLiteralBooleanValue::isLessThan(CDataLiteralBooleanValue* booleanValue) {
				if (!mBoolean && booleanValue->mBoolean) {
					return true;
				} else {
					return false;
				}
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
