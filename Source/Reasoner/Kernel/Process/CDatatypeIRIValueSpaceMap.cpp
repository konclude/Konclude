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

#include "CDatatypeIRIValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeIRIValueSpaceMap::CDatatypeIRIValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeIRIValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return true;
				}

				bool CDatatypeIRIValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					CDataLiteralIRIValue* iriValue = dynamic_cast<CDataLiteralIRIValue*>(value);
					if (iriValue) {
						return iriValue->isInfiniteLength();
					}
					return false;
				}

				bool CDatatypeIRIValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralIRIValue* iriValueCopyInto = dynamic_cast<CDataLiteralIRIValue*>(valueCopyInto); 
					CDataLiteralIRIValue* iriValueCopyFrom = dynamic_cast<CDataLiteralIRIValue*>(valueCopyFrom);
					iriValueCopyInto->initValue(iriValueCopyFrom);
					return false;
				}

				bool CDatatypeIRIValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralIRIValue* iriValueNext = dynamic_cast<CDataLiteralIRIValue*>(valueNext);
					CDataLiteralIRIValue* iriValueLast = dynamic_cast<CDataLiteralIRIValue*>(valueLast);
					return false;
				}


				cuint64 CDatatypeIRIValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralIRIValue* iriLeftValue = dynamic_cast<CDataLiteralIRIValue*>(leftValueExcluded);
					CDataLiteralIRIValue* iriRightValue = dynamic_cast<CDataLiteralIRIValue*>(rightValueExcluded);
					cuint64 valueCount = 0;
					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
