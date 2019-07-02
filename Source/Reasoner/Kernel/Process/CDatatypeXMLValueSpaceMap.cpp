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

#include "CDatatypeXMLValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeXMLValueSpaceMap::CDatatypeXMLValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeXMLValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return true;
				}

				bool CDatatypeXMLValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					CDataLiteralXMLValue* xmlValue = dynamic_cast<CDataLiteralXMLValue*>(value);
					if (xmlValue) {
						return xmlValue->isInfiniteLength();
					}
					return false;
				}

				bool CDatatypeXMLValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralXMLValue* xmlValueCopyInto = dynamic_cast<CDataLiteralXMLValue*>(valueCopyInto); 
					CDataLiteralXMLValue* xmlValueCopyFrom = dynamic_cast<CDataLiteralXMLValue*>(valueCopyFrom);
					xmlValueCopyInto->initValue(xmlValueCopyFrom);
					return false;
				}

				bool CDatatypeXMLValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralXMLValue* xmlValueNext = dynamic_cast<CDataLiteralXMLValue*>(valueNext);
					CDataLiteralXMLValue* xmlValueLast = dynamic_cast<CDataLiteralXMLValue*>(valueLast);
					return false;
				}


				cuint64 CDatatypeXMLValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralXMLValue* xmlLeftValue = dynamic_cast<CDataLiteralXMLValue*>(leftValueExcluded);
					CDataLiteralXMLValue* xmlRightValue = dynamic_cast<CDataLiteralXMLValue*>(rightValueExcluded);
					cuint64 valueCount = 0;
					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
