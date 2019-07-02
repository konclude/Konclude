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

#include "CDataLiteralBinaryDataValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralBinaryDataValue::CDataLiteralBinaryDataValue(CContext* context) {
				mContext = context;
				mByteDataArray = nullptr;
				mByteDataArraySize = 0;
				clearValue();
			}



			CDataLiteralCompareValue* CDataLiteralBinaryDataValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value); 
				if (dataValue) {
					initValue(dataValue);
				}
				return this;
			}


			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::initValueFromInfiniteLength() {
				clearValue();
				mInfiniteLength = true;
				return this;
			}

			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::initValueFromLength(cuint64 length) {
				clearValue();
				mLength = length;
				return this;
			}


			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::initValueFromData(unsigned char* data, cint64 length) {
				clearValue();
				if (mByteDataArraySize < length) {
					mByteDataArraySize = length;
					mByteDataLength = length;
					mByteDataArray = CObjectAllocator<unsigned char>::allocateArray(CContext::getMemoryAllocationManager(mContext),mByteDataArraySize);
				}
				for (cint64 i = 0; i < mByteDataLength; ++i) {
					mByteDataArray[i] = data[i];
				}
				return this;
			}


			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::initValue(CDataLiteralBinaryDataValue* value) {
				clearValue();
				if (value) {
					mInfiniteLength = value->mInfiniteLength;
					mLength = value->mLength;
					if (value->mByteDataLength > 0) {
						if (mByteDataArraySize < value->mByteDataLength) {
							mByteDataArraySize = value->mByteDataLength;
							mByteDataLength = value->mByteDataLength;
							mByteDataArray = CObjectAllocator<unsigned char>::allocateArray(CContext::getMemoryAllocationManager(mContext),mByteDataArraySize);
						}
						for (cint64 i = 0; i < mByteDataLength; ++i) {
							mByteDataArray[i] = value->mByteDataArray[i];
						}
					}
				}
				return this;
			}


			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::clearValue() {
				mInfiniteLength = false;
				mLength = 0;
				mByteDataLength = 0;
				if (mByteDataArray) {
					for (cint64 i = 0; i < mByteDataArraySize; ++i) {
						mByteDataArray[i] = 0;
					}
				}
				return this;
			}

			cuint64 CDataLiteralBinaryDataValue::getBinaryDataID() {
				cuint64 dataID = 0;
				dataID += 1 << getLength()*8;
				for (cint64 i = 0; i < mByteDataLength; ++i) {
					dataID += mByteDataArray[i] << (mByteDataLength-i-1)*8;
				}
				return dataID;
			}

			CDataLiteralBinaryDataValue* CDataLiteralBinaryDataValue::incrementBinaryData() {
				if (mLength > 0) {
					if (mByteDataArraySize < mLength) {
						mByteDataArraySize = mLength;
						mByteDataArray = CObjectAllocator<unsigned char>::allocateArray(CContext::getMemoryAllocationManager(mContext),mByteDataArraySize);
					}
					mByteDataLength = mLength;
					for (cint64 i = 0; i < mByteDataLength; ++i) {
						mByteDataArray[i] = 0;
					}
					mLength = 0;
				}

				bool overflow = true;
				for (cint64 i = mByteDataLength-1; overflow && i >= 0; --i) {
					overflow = false;
					if (mByteDataArray[i] == 255) {
						mByteDataArray[i] = 0;
						overflow = true;
					} else {
						mByteDataArray[i] = mByteDataArray[i]+1;
					}
				}

				if (overflow) {
					if (mByteDataArraySize < mLength+1) {
						mByteDataArraySize = mLength+1;
						mByteDataArray = CObjectAllocator<unsigned char>::allocateArray(CContext::getMemoryAllocationManager(mContext),mByteDataArraySize);
					}
					mByteDataLength = mLength+1;
					for (cint64 i = 0; i < mByteDataLength; ++i) {
						mByteDataArray[i] = 0;
					}
				}

				return this;
			}



			bool CDataLiteralBinaryDataValue::isInfiniteLength() {
				return mInfiniteLength;
			}

			cuint64 CDataLiteralBinaryDataValue::getLength() {
				if (mLength > 0) {
					return mLength;
				} else {
					return mByteDataLength;
				}
			}


			unsigned char* CDataLiteralBinaryDataValue::getByteDataArray() {
				return mByteDataArray;
			}

			cint64 CDataLiteralBinaryDataValue::getByteDataLength() {
				return mByteDataLength;
			}


			bool CDataLiteralBinaryDataValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
				if (dataValue) {
					return isEqualTo(dataValue);
				}
				return false;
			}



			bool CDataLiteralBinaryDataValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
				if (dataValue) {
					return isEqualTo(dataValue) || isLessThan(dataValue);
				}
				return false;
			}

			bool CDataLiteralBinaryDataValue::isLessEqualThan(CDataLiteralBinaryDataValue* dataValue) {
				return isEqualTo(dataValue) || isLessThan(dataValue);
			}


			bool CDataLiteralBinaryDataValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
				if (dataValue) {
					return isEqualTo(dataValue);
				}
				return false;
			}


			bool CDataLiteralBinaryDataValue::isZeroData() {
				if (mLength > 0) {
					return true;
				}
				for (cint64 i = 0; i < mByteDataLength; ++i) {
					unsigned char data = mByteDataArray[i];
					if (data != 0) {
						return false;
					}
				}
				return true;
			}


			bool CDataLiteralBinaryDataValue::isEqualTo(CDataLiteralBinaryDataValue* dataValue) {
				if (getDataValueType() != dataValue->getDataValueType()) {
					return false;
				}
				if (mInfiniteLength && dataValue->mInfiniteLength) {
					return true;
				}
				if (mInfiniteLength || dataValue->mInfiniteLength) {
					return false;
				}
				if (getLength() != dataValue->getLength()) {
					return false;
				}
				if (mLength > 0 && mLength == dataValue->mLength) {
					return true;
				}
				bool zD = isZeroData();
				bool dataValueZD = dataValue->isZeroData();
				if (zD && dataValueZD) {
					return true;
				} else if (zD != dataValueZD) {
					return false;
				} else {
					for (cint64 i = 0; i < mByteDataLength; ++i) {
						if (mByteDataArray[i] != dataValue->mByteDataArray[i]) {
							return false;
						}
					}
					return true;
				}
				return false;
			}

			bool CDataLiteralBinaryDataValue::isGreaterEqualThan(CDataLiteralBinaryDataValue* dataValue) {
				if (getDataValueType() != dataValue->getDataValueType()) {
					return false;
				}
				return isEqualTo(dataValue) || isGreaterThan(dataValue);
			}

			bool CDataLiteralBinaryDataValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
				if (dataValue) {
					return isEqualTo(dataValue) || isGreaterThan(dataValue);
				}
				return false;
			}

			bool CDataLiteralBinaryDataValue::isGreaterThan(CDataLiteralCompareValue* value) {
				if (getDataValueType() != value->getDataValueType()) {
					return false;
				}
				return value->isLessThan(this);
			}

			bool CDataLiteralBinaryDataValue::isGreaterThan(CDataLiteralBinaryDataValue* dataValue) {
				if (getDataValueType() != dataValue->getDataValueType()) {
					return false;
				}
				return dataValue->isLessThan(this);
			}


			bool CDataLiteralBinaryDataValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralBinaryDataValue* dataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
				if (dataValue) {
					return isLessThan(dataValue);
				}
				return false;
			}





			bool CDataLiteralBinaryDataValue::isLessThan(CDataLiteralBinaryDataValue* dataValue) {
				if (getDataValueType() != dataValue->getDataValueType()) {
					return false;
				}
				if (mInfiniteLength) {
					return false;
				}
				if (!mInfiniteLength && dataValue->mInfiniteLength) {
					return true;
				}
				if (getLength() < dataValue->getLength()) {
					return true;
				} else if (getLength() > dataValue->getLength()) {
					return false;
				} else {

					if (dataValue->isZeroData()) {
						return false;
					} else if (isZeroData()) {
						return true;
					}

					for (cint64 i = 0; i < mByteDataLength; ++i) {
						if (mByteDataArray[i] < dataValue->mByteDataArray[i]) {
							return true;
						} else if (mByteDataArray[i] > dataValue->mByteDataArray[i]) {
							return false;
						}
					}

				}
				return false;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
