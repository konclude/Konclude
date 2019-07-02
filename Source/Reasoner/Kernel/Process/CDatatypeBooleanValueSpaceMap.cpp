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

#include "CDatatypeBooleanValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeBooleanValueSpaceMap::CDatatypeBooleanValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}

				bool CDatatypeBooleanValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return false;
				}

				bool CDatatypeBooleanValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					return false;
				}

				bool CDatatypeBooleanValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralBooleanValue* booleanValueCopyInto = dynamic_cast<CDataLiteralBooleanValue*>(valueCopyInto); 
					CDataLiteralBooleanValue* booleanValueCopyFrom = dynamic_cast<CDataLiteralBooleanValue*>(valueCopyFrom);
					booleanValueCopyInto->initValue(booleanValueCopyFrom);
					return true;
				}

				bool CDatatypeBooleanValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralBooleanValue* booleanValueNext = dynamic_cast<CDataLiteralBooleanValue*>(valueNext);
					CDataLiteralBooleanValue* booleanValueLast = dynamic_cast<CDataLiteralBooleanValue*>(valueLast);
					if (booleanValueLast->getBoolean() == false) {
						booleanValueNext->initValueFromBoolean(true);
						return true;
					}
					return false;
				}


				cuint64 CDatatypeBooleanValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					return 0;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
