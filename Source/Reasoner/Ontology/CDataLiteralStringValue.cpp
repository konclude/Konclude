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

#include "CDataLiteralStringValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralStringValue::CDataLiteralStringValue(CContext* context) {
				mContext = context;
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

					mStringByteDataArraySize = stringValue->mStringByteDataArraySize;
					mStringByteDataArray = createStringByteDataArrayCopy(stringValue->mStringByteDataArray, mStringByteDataArraySize);

					mLanguageByteDataArraySize = stringValue->mLanguageByteDataArraySize;
					mLanguageByteDataArray = createStringByteDataArrayCopy(stringValue->mLanguageByteDataArray, mLanguageByteDataArraySize);
				}
				return this;
			}


			QChar* CDataLiteralStringValue::createStringByteDataArrayCopy(QChar* stringByteDataArray, cint64 arrayLength) {
				QChar* stringByteDataArrayCopy = CObjectAllocator<QChar>::allocateArray(CContext::getMemoryAllocationManager(mContext), arrayLength);
				for (cint64 i = 0; i < arrayLength; ++i) {
					stringByteDataArrayCopy[i] = stringByteDataArray[i];
				}
				return stringByteDataArrayCopy;
			}


			QChar* CDataLiteralStringValue::createStringByteDataArray(const QString& string) {
				QChar* stringByteDataArrayCopy = CObjectAllocator<QChar>::allocateArray(CContext::getMemoryAllocationManager(mContext), string.length());
				const QChar* stringData = string.constData();
				for (cint64 i = 0; i < string.length(); ++i) {
					stringByteDataArrayCopy[i] = stringData[i];
				}
				return stringByteDataArrayCopy;
			}



			QString CDataLiteralStringValue::getValueString() {
				if (mLanguageByteDataArraySize <= 0) {
					return QString(mStringByteDataArray, mStringByteDataArraySize);
				} else {
					return QString("%1@%2").arg(QString::fromRawData(mStringByteDataArray, mStringByteDataArraySize)).arg(QString::fromRawData(mLanguageByteDataArray, mLanguageByteDataArraySize));
				}
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
				mStringByteDataArray = createStringByteDataArray(string);
				mStringByteDataArraySize = string.length();
				mLanguageByteDataArray = createStringByteDataArray(langString);
				mLanguageByteDataArraySize = langString.length();

				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::initValue(CDataLiteralStringValue* value) {
				if (value) {
					mInfiniteLength = value->mInfiniteLength;
					mLength = value->mLength;

					mStringByteDataArraySize = value->mStringByteDataArraySize;
					mStringByteDataArray = createStringByteDataArrayCopy(value->mStringByteDataArray, mStringByteDataArraySize);

					mLanguageByteDataArraySize = value->mLanguageByteDataArraySize;
					mLanguageByteDataArray = createStringByteDataArrayCopy(value->mLanguageByteDataArray, mLanguageByteDataArraySize);

				}
				return this;
			}


			CDataLiteralStringValue* CDataLiteralStringValue::clearValue() {
				mInfiniteLength = false;
				mLength = 0;

				mStringByteDataArray = nullptr;
				mStringByteDataArraySize = 0;

				mLanguageByteDataArray = nullptr;
				mLanguageByteDataArraySize = 0;

				return this;
			}

			bool CDataLiteralStringValue::isInfiniteLength() {
				return mInfiniteLength;
			}

			bool CDataLiteralStringValue::hasLanguageTag() {
				return mLanguageByteDataArraySize > 0;
			}


			cuint64 CDataLiteralStringValue::getLength() {
				if (mLength > 0) {
					return mLength;
				} else {
					return mStringByteDataArraySize;
				}
			}

			QString CDataLiteralStringValue::getStringValue() {
				return QString(mStringByteDataArray, mStringByteDataArraySize);
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
				if (QString::fromRawData(mLanguageByteDataArray, mLanguageByteDataArraySize) != QString::fromRawData(stringValue->mLanguageByteDataArray, stringValue->mLanguageByteDataArraySize)) {
					return false;
				}
				if (QString::fromRawData(mStringByteDataArray, mStringByteDataArraySize) != QString::fromRawData(stringValue->mStringByteDataArray, stringValue->mStringByteDataArraySize)) {
					return false;
				}
				return true;
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
					int stringComp = QString::fromRawData(mStringByteDataArray, mStringByteDataArraySize).compare(QString::fromRawData(stringValue->mStringByteDataArray, stringValue->mStringByteDataArraySize));
					if (stringComp < 0) {
						return true;
					} else if (stringComp > 0) {
						return false;
					}
					int lanComp = QString::fromRawData(mLanguageByteDataArray, mLanguageByteDataArraySize).compare(QString::fromRawData(stringValue->mLanguageByteDataArray, stringValue->mLanguageByteDataArraySize));
					return lanComp < 0;
				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
