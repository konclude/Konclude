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

#include "CDatatypeFloatValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeFloatValueSpaceData::CDatatypeFloatValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mFloatValueSpaceMap = nullptr;
				}


				CDatatypeFloatValueSpaceData* CDatatypeFloatValueSpaceData::initFloatValueSpaceData(CDatatypeValueSpaceFloatType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mFloatValueSpaceMap) {
						mFloatValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeFloatValueSpaceData* CDatatypeFloatValueSpaceData::copyFloatValueSpaceData(CDatatypeFloatValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mFloatValueSpaceMap && !mFloatValueSpaceMap) {
						getFloatValueSpaceMap(true);
						mFloatValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mFloatValueSpaceMap);
					} else if (mFloatValueSpaceMap) {
						if (spaceData->mFloatValueSpaceMap) {
							mFloatValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mFloatValueSpaceMap);
						} else {
							mFloatValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeFloatValueSpaceMap* CDatatypeFloatValueSpaceData::getFloatValueSpaceMap(bool create) {
					if (create && !mFloatValueSpaceMap) {
						mFloatValueSpaceMap = CObjectParameterizingAllocator< CDatatypeFloatValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mFloatValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mFloatValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeFloatValueSpaceData::createValueSpaceMap() {
					return getFloatValueSpaceMap(true);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
