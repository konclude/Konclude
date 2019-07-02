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

#include "CDataLiteralIRIValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralIRIValue::CDataLiteralIRIValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralIRIValue::getDataValueType() {
				return CDataLiteralValue::DLVT_IRI;
			}


			CDataLiteralCompareValue* CDataLiteralIRIValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value); 
				if (iriValue) {
					mInfiniteLength = iriValue->mInfiniteLength;
					mLength = iriValue->mLength;
					mIRIString = iriValue->mIRIString;
				}
				return this;
			}


			CDataLiteralIRIValue* CDataLiteralIRIValue::initValueFromInfiniteLength() {
				clearValue();
				mInfiniteLength = true;
				return this;
			}

			CDataLiteralIRIValue* CDataLiteralIRIValue::initValueFromLength(cuint64 length) {
				clearValue();
				mLength = length;
				return this;
			}


			CDataLiteralIRIValue* CDataLiteralIRIValue::initValueFromIRIString(const QString& iriString) {
				clearValue();
				mIRIString = iriString;
				return this;
			}


			CDataLiteralIRIValue* CDataLiteralIRIValue::initValue(CDataLiteralIRIValue* value) {
				if (value) {
					mInfiniteLength = value->mInfiniteLength;
					mLength = value->mLength;
					mIRIString = value->mIRIString;
				}
				return this;
			}


			CDataLiteralIRIValue* CDataLiteralIRIValue::clearValue() {
				mInfiniteLength = false;
				mLength = 0;
				mIRIString.clear();
				return this;
			}


			bool CDataLiteralIRIValue::isInfiniteLength() {
				return mInfiniteLength;
			}

			cuint64 CDataLiteralIRIValue::getLength() {
				if (mLength > 0) {
					return mLength;
				} else {
					return mIRIString.length();
				}
			}

			QString CDataLiteralIRIValue::getIRIValue() {
				return mIRIString;
			}


			bool CDataLiteralIRIValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
				if (iriValue) {
					return isEqualTo(iriValue);
				}
				return false;
			}



			bool CDataLiteralIRIValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
				if (iriValue) {
					return isEqualTo(iriValue) || isLessThan(iriValue);
				}
				return false;
			}

			bool CDataLiteralIRIValue::isLessEqualThan(CDataLiteralIRIValue* iriValue) {
				return isEqualTo(iriValue) || isLessThan(iriValue);
			}


			bool CDataLiteralIRIValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
				if (iriValue) {
					return isEqualTo(iriValue);
				}
				return false;
			}

			bool CDataLiteralIRIValue::isEqualTo(CDataLiteralIRIValue* iriValue) {
				if (mInfiniteLength && iriValue->mInfiniteLength) {
					return true;
				}
				if (mInfiniteLength || iriValue->mInfiniteLength) {
					return false;
				}
				if (mLength > 0 && mLength == iriValue->mLength) {
					return true;
				}
				if (mIRIString == iriValue->mIRIString) {
					return true;
				}
				return false;
			}

			bool CDataLiteralIRIValue::isGreaterEqualThan(CDataLiteralIRIValue* iriValue) {
				return isEqualTo(iriValue) || isGreaterThan(iriValue);
			}

			bool CDataLiteralIRIValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
				if (iriValue) {
					return isEqualTo(iriValue) || isGreaterThan(iriValue);
				}
				return false;
			}

			bool CDataLiteralIRIValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralIRIValue::isGreaterThan(CDataLiteralIRIValue* iriValue) {
				return iriValue->isLessThan(this);
			}


			bool CDataLiteralIRIValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
				if (iriValue) {
					return isLessThan(iriValue);
				}
				return false;
			}





			bool CDataLiteralIRIValue::isLessThan(CDataLiteralIRIValue* iriValue) {
				if (mInfiniteLength) {
					return false;
				}
				if (!mInfiniteLength && iriValue->mInfiniteLength) {
					return true;
				}
				if (getLength() < iriValue->getLength()) {
					return true;
				} else if (getLength() > iriValue->getLength()) {
					return false;
				} else {
					int stringComp = mIRIString.compare(iriValue->mIRIString);
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
