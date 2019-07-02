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

#include "CDatatypeDateTimeValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeDateTimeValueSpaceData::CDatatypeDateTimeValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mDateTimeValueSpaceMap = nullptr;
				}


				CDatatypeDateTimeValueSpaceData* CDatatypeDateTimeValueSpaceData::initDateTimeValueSpaceData(CDatatypeValueSpaceDateTimeType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mDateTimeValueSpaceMap) {
						mDateTimeValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeDateTimeValueSpaceData* CDatatypeDateTimeValueSpaceData::copyDateTimeValueSpaceData(CDatatypeDateTimeValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mDateTimeValueSpaceMap && !mDateTimeValueSpaceMap) {
						getDateTimeValueSpaceMap(true);
						mDateTimeValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mDateTimeValueSpaceMap);
					} else if (mDateTimeValueSpaceMap) {
						if (spaceData->mDateTimeValueSpaceMap) {
							mDateTimeValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mDateTimeValueSpaceMap);
						} else {
							mDateTimeValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeDateTimeValueSpaceMap* CDatatypeDateTimeValueSpaceData::getDateTimeValueSpaceMap(bool create) {
					if (create && !mDateTimeValueSpaceMap) {
						mDateTimeValueSpaceMap = CObjectParameterizingAllocator< CDatatypeDateTimeValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mDateTimeValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mDateTimeValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeDateTimeValueSpaceData::createValueSpaceMap() {
					return getDateTimeValueSpaceMap(true);
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
