/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CDataLiteralStringValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralStringValue::CDataLiteralStringValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralStringValue::getDataValueType() {
				return CDataLiteralValue::DLVT_STRING;
			}


			CDataLiteralCompareValue* CDataLiteralStringValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value); 
				if (stringValue) {
					mInfiniteLength = stringValue->mInfiniteLength;
					mLength = stringValue->mLength;
					mString = stringValue->mString;
					mLanguage = stringValue->mLanguage;
				}
				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::initValueFromInfiniteLength() {
				clearValue();
				mInfiniteLength = true;
				return this;
			}

			CDataLiteralStringValue* CDataLiteralStringValue::initValueFromLength(cuint64 length) {
				clearValue();
				mLength = length;
				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::initValueFromString(const QString& string, const QString& langString) {
				clearValue();
				mString = string;
				mLanguage = langString;
				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::initValue(CDataLiteralStringValue* value) {
				if (value) {
					mInfiniteLength = value->mInfiniteLength;
					mLength = value->mLength;
					mString = value->mString;
					mLanguage = value->mLanguage;
				}
				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::clearValue() {
				mInfiniteLength = false;
				mLength = 0;
				mString.clear();
				mLanguage.clear();
				return this;
			}


			bool CDataLiteralStringValue::isInfiniteLength() {
				return mInfiniteLength;
			}

			bool CDataLiteralStringValue::hasLanguageTag() {
				return !mLanguage.isEmpty();
			}


			cuint64 CDataLiteralStringValue::getLength() {
				if (mLength > 0) {
					return mLength;
				} else {
					return mString.length();
				}
			}

			QString CDataLiteralStringValue::getStringValue() {
				return mString;
			}


			bool CDataLiteralStringValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value);
				if (stringValue) {
					return isEqualTo(stringValue);
				}
				return false;
			}



			bool CDataLiteralStringValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value);
				if (stringValue) {
					return isEqualTo(stringValue) || isLessThan(stringValue);
				}
				return false;
			}

			bool CDataLiteralStringValue::isLessEqualThan(CDataLiteralStringValue* stringValue) {
				return isEqualTo(stringValue) || isLessThan(stringValue);
			}


			bool CDataLiteralStringValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value);
				if (stringValue) {
					return isEqualTo(stringValue);
				}
				return false;
			}

			bool CDataLiteralStringValue::isEqualTo(CDataLiteralStringValue* stringValue) {
				if (mInfiniteLength && stringValue->mInfiniteLength) {
					return true;
				}
				if (mInfiniteLength || stringValue->mInfiniteLength) {
					return false;
				}
				if (mLength > 0 && mLength == stringValue->mLength) {
					return true;
				}
				if (mLanguage == stringValue->mLanguage && mString == stringValue->mString) {
					return true;
				}
				return false;
			}

			bool CDataLiteralStringValue::isGreaterEqualThan(CDataLiteralStringValue* stringValue) {
				return isEqualTo(stringValue) || isGreaterThan(stringValue);
			}

			bool CDataLiteralStringValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value);
				if (stringValue) {
					return isEqualTo(stringValue) || isGreaterThan(stringValue);
				}
				return false;
			}

			bool CDataLiteralStringValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralStringValue::isGreaterThan(CDataLiteralStringValue* stringValue) {
				return stringValue->isLessThan(this);
			}


			bool CDataLiteralStringValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralStringValue* stringValue = dynamic_cast<CDataLiteralStringValue*>(value);
				if (stringValue) {
					return isLessThan(stringValue);
				}
				return false;
			}





			bool CDataLiteralStringValue::isLessThan(CDataLiteralStringValue* stringValue) {
				if (mInfiniteLength) {
					return false;
				}
				if (!mInfiniteLength && stringValue->mInfiniteLength) {
					return true;
				}
				if (getLength() < stringValue->getLength()) {
					return true;
				} else if (getLength() > stringValue->getLength()) {
					return false;
				} else {
					int stringComp = mString.compare(stringValue->mString);
					if (stringComp < 0) {
						return true;
					} else if (stringComp > 0) {
						return false;
					}
					int lanComp = mLanguage.compare(stringValue->mLanguage);
					return lanComp < 0;
				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
