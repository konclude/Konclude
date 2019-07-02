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

#include "CDatatypeDoubleValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeDoubleValueSpaceData::CDatatypeDoubleValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mDoubleValueSpaceMap = nullptr;
				}


				CDatatypeDoubleValueSpaceData* CDatatypeDoubleValueSpaceData::initDoubleValueSpaceData(CDatatypeValueSpaceDoubleType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mDoubleValueSpaceMap) {
						mDoubleValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeDoubleValueSpaceData* CDatatypeDoubleValueSpaceData::copyDoubleValueSpaceData(CDatatypeDoubleValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mDoubleValueSpaceMap && !mDoubleValueSpaceMap) {
						getDoubleValueSpaceMap(true);
						mDoubleValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mDoubleValueSpaceMap);
					} else if (mDoubleValueSpaceMap) {
						if (spaceData->mDoubleValueSpaceMap) {
							mDoubleValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mDoubleValueSpaceMap);
						} else {
							mDoubleValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeDoubleValueSpaceMap* CDatatypeDoubleValueSpaceData::getDoubleValueSpaceMap(bool create) {
					if (create && !mDoubleValueSpaceMap) {
						mDoubleValueSpaceMap = CObjectParameterizingAllocator< CDatatypeDoubleValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mDoubleValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mDoubleValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeDoubleValueSpaceData::createValueSpaceMap() {
					return getDoubleValueSpaceMap(true);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
