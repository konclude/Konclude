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

#include "CDatatypeBinaryDataValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeBinaryDataValueSpaceMap::CDatatypeBinaryDataValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeBinaryDataValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralBinaryDataValue* binaryDataLeftValue = dynamic_cast<CDataLiteralBinaryDataValue*>(leftValueExcluded);
					CDataLiteralBinaryDataValue* binaryDataRightValue = dynamic_cast<CDataLiteralBinaryDataValue*>(rightValueExcluded);
					if (binaryDataRightValue->getLength() >= 7) {
						return true;
					}
					return false;
				}

				bool CDatatypeBinaryDataValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					CDataLiteralBinaryDataValue* binaryDataValue = dynamic_cast<CDataLiteralBinaryDataValue*>(value);
					if (binaryDataValue) {
						return binaryDataValue->isInfiniteLength();
					}
					return false;
				}

				bool CDatatypeBinaryDataValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralBinaryDataValue* binaryDataValueCopyInto = dynamic_cast<CDataLiteralBinaryDataValue*>(valueCopyInto); 
					CDataLiteralBinaryDataValue* binaryDataValueCopyFrom = dynamic_cast<CDataLiteralBinaryDataValue*>(valueCopyFrom);
					binaryDataValueCopyInto->initValue(binaryDataValueCopyFrom);
					return true;
				}

				bool CDatatypeBinaryDataValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralBinaryDataValue* binaryDataValueNext = dynamic_cast<CDataLiteralBinaryDataValue*>(valueNext);
					CDataLiteralBinaryDataValue* binaryDataValueLast = dynamic_cast<CDataLiteralBinaryDataValue*>(valueLast);

					binaryDataValueNext->initValue(binaryDataValueLast);
					binaryDataValueNext->incrementBinaryData();
					return true;
				}


				cuint64 CDatatypeBinaryDataValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralBinaryDataValue* binaryDataLeftValue = dynamic_cast<CDataLiteralBinaryDataValue*>(leftValueExcluded);
					CDataLiteralBinaryDataValue* binaryDataRightValue = dynamic_cast<CDataLiteralBinaryDataValue*>(rightValueExcluded);
					cuint64 valueCount = 0;

					cuint64 leftValueID = binaryDataLeftValue->getBinaryDataID();
					cuint64 rightValueID = binaryDataRightValue->getBinaryDataID();
					valueCount = rightValueID - leftValueID;
					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
