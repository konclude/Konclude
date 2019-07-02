/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CDataLiteralXMLValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralXMLValue::CDataLiteralXMLValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralXMLValue::getDataValueType() {
				return CDataLiteralValue::DLVT_XML;
			}


			CDataLiteralCompareValue* CDataLiteralXMLValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value); 
				if (xmlValue) {
					mInfiniteLength = xmlValue->mInfiniteLength;
					mLength = xmlValue->mLength;
					mXMLString = xmlValue->mXMLString;
				}
				return this;
			}


			CDataLiteralXMLValue* CDataLiteralXMLValue::initValueFromInfiniteLength() {
				clearValue();
				mInfiniteLength = true;
				return this;
			}

			CDataLiteralXMLValue* CDataLiteralXMLValue::initValueFromLength(cuint64 length) {
				clearValue();
				mLength = length;
				return this;
			}


			CDataLiteralXMLValue* CDataLiteralXMLValue::initValueFromXMLString(const QString& xmlString) {
				clearValue();
				mXMLString = xmlString;
				return this;
			}


			CDataLiteralXMLValue* CDataLiteralXMLValue::initValue(CDataLiteralXMLValue* value) {
				if (value) {
					mInfiniteLength = value->mInfiniteLength;
					mLength = value->mLength;
					mXMLString = value->mXMLString;
				}
				return this;
			}


			CDataLiteralXMLValue* CDataLiteralXMLValue::clearValue() {
				mInfiniteLength = false;
				mLength = 0;
				mXMLString.clear();
				return this;
			}


			bool CDataLiteralXMLValue::isInfiniteLength() {
				return mInfiniteLength;
			}

			cuint64 CDataLiteralXMLValue::getLength() {
				if (mLength > 0) {
					return mLength;
				} else {
					return mXMLString.length();
				}
			}

			QString CDataLiteralXMLValue::getXMLValue() {
				return mXMLString;
			}


			bool CDataLiteralXMLValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
				if (xmlValue) {
					return isEqualTo(xmlValue);
				}
				return false;
			}



			bool CDataLiteralXMLValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
				if (xmlValue) {
					return isEqualTo(xmlValue) || isLessThan(xmlValue);
				}
				return false;
			}

			bool CDataLiteralXMLValue::isLessEqualThan(CDataLiteralXMLValue* xmlValue) {
				return isEqualTo(xmlValue) || isLessThan(xmlValue);
			}


			bool CDataLiteralXMLValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
				if (xmlValue) {
					return isEqualTo(xmlValue);
				}
				return false;
			}

			bool CDataLiteralXMLValue::isEqualTo(CDataLiteralXMLValue* xmlValue) {
				if (mInfiniteLength && xmlValue->mInfiniteLength) {
					return true;
				}
				if (mInfiniteLength || xmlValue->mInfiniteLength) {
					return false;
				}
				if (mLength > 0 && mLength == xmlValue->mLength) {
					return true;
				}
				if (mXMLString == xmlValue->mXMLString) {
					return true;
				}
				return false;
			}

			bool CDataLiteralXMLValue::isGreaterEqualThan(CDataLiteralXMLValue* xmlValue) {
				return isEqualTo(xmlValue) || isGreaterThan(xmlValue);
			}

			bool CDataLiteralXMLValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
				if (xmlValue) {
					return isEqualTo(xmlValue) || isGreaterThan(xmlValue);
				}
				return false;
			}

			bool CDataLiteralXMLValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralXMLValue::isGreaterThan(CDataLiteralXMLValue* xmlValue) {
				return xmlValue->isLessThan(this);
			}


			bool CDataLiteralXMLValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
				if (xmlValue) {
					return isLessThan(xmlValue);
				}
				return false;
			}





			bool CDataLiteralXMLValue::isLessThan(CDataLiteralXMLValue* xmlValue) {
				if (!mInfiniteLength && xmlValue->mInfiniteLength) {
					return true;
				}
				if (mInfiniteLength) {
					return false;
				}
				if (getLength() < xmlValue->getLength()) {
					return true;
				} else if (getLength() > xmlValue->getLength()) {
					return false;
				} else {
					int stringComp = mXMLString.compare(xmlValue->mXMLString);
					if (stringComp < 0) {
						return true;
					} else if (stringComp > 0) {
						return false;
					}
				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
