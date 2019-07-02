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

#include "CDatatypeDateTimeValueSpaceMap.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeDateTimeValueSpaceMap::CDatatypeDateTimeValueSpaceMap(CProcessContext* processContext) : CDatatypeCompareValueSpaceMap(processContext) {
				}


				CDatatypeDateTimeValueSpaceMap* CDatatypeDateTimeValueSpaceMap::initDatatypeDateTimeValueSpaceMap(CDatatypeValueSpaceDateTimeType* valueSpaceType) {
					CPROCESSMAP<CDatatypeCompareValueSpaceMapArranger,CDatatypeCompareValueSpaceMapData>::clear();
					mAbsoluteMinimumExclusionValue = nullptr;
					mAbsoluteMaximumExclusionValue = nullptr;

					excludeMinimum(valueSpaceType->getMinimumDataLiteralDateTimeValue(),true,nullptr);
					excludeMaximum(valueSpaceType->getMaximumDataLiteralDateTimeValue(),true,nullptr);
					return this;
				}


				bool CDatatypeDateTimeValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralDateTimeValue* dateTimeLeftValue = dynamic_cast<CDataLiteralDateTimeValue*>(leftValueExcluded);
					CDataLiteralDateTimeValue* dateTimeRightValue = dynamic_cast<CDataLiteralDateTimeValue*>(rightValueExcluded);
					if (dateTimeLeftValue->isFirstDateTime()) {
						return true;
					}
					if (dateTimeRightValue->isLastDateTime()) {
						return true;
					}
					return false;
				}

				bool CDatatypeDateTimeValueSpaceMap::representsInfinitelyManyValues(CDataLiteralCompareValue* value) {
					return false;
				}

				bool CDatatypeDateTimeValueSpaceMap::getValueCopy(CDataLiteralCompareValue* valueCopyInto, CDataLiteralCompareValue* valueCopyFrom) {
					CDataLiteralDateTimeValue* dateTimeValueCopyInto = dynamic_cast<CDataLiteralDateTimeValue*>(valueCopyInto); 
					CDataLiteralDateTimeValue* dateTimeValueCopyFrom = dynamic_cast<CDataLiteralDateTimeValue*>(valueCopyFrom);
					dateTimeValueCopyInto->initValue(dateTimeValueCopyFrom);
					return true;
				}

				bool CDatatypeDateTimeValueSpaceMap::getValueNext(CDataLiteralCompareValue* valueNext, CDataLiteralCompareValue* valueLast) {
					CDataLiteralDateTimeValue* dateTimeValueNext = dynamic_cast<CDataLiteralDateTimeValue*>(valueNext);
					CDataLiteralDateTimeValue* dateTimeValueLast = dynamic_cast<CDataLiteralDateTimeValue*>(valueLast);

					dateTimeValueNext->initValue(dateTimeValueLast);
					dateTimeValueNext->incrementDateTimeValue();
					return true;
				}


				cuint64 CDatatypeDateTimeValueSpaceMap::getIntervalValueCount(CDataLiteralCompareValue* leftValueExcluded, CDataLiteralCompareValue* rightValueExcluded) {
					CDataLiteralDateTimeValue* dateTimeLeftValue = dynamic_cast<CDataLiteralDateTimeValue*>(leftValueExcluded);
					CDataLiteralDateTimeValue* dateTimeRightValue = dynamic_cast<CDataLiteralDateTimeValue*>(rightValueExcluded);
					cuint64 valueCount = 0;

					QDateTime leftValueDateTime = dateTimeLeftValue->getDateTimeValue();
					QDateTime rightValueDateTime = dateTimeRightValue->getDateTimeValue();
					cuint64 secondsBetween = leftValueDateTime.secsTo(rightValueDateTime)-1;
					if (secondsBetween < 0) {
						secondsBetween = 0;
						valueCount += dateTimeRightValue->getTimeZoneOffset()-dateTimeLeftValue->getTimeZoneOffset()-1;
					} else if (secondsBetween >= 0) {
						if (dateTimeLeftValue->getTimeZoneOffset() < 840) {
							valueCount += 840-dateTimeLeftValue->getTimeZoneOffset()-1;
						}
						if (dateTimeRightValue->getTimeZoneOffset() > -840) {
							valueCount += dateTimeRightValue->getTimeZoneOffset()+840-1;
						}
						if (secondsBetween > 0) {
							secondsBetween--;
						}
					}
					valueCount = secondsBetween*(840*2-1);
					return valueCount;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
