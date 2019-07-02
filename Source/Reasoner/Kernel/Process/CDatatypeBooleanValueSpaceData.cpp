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

#include "CDatatypeBooleanValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeBooleanValueSpaceData::CDatatypeBooleanValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mBooleanValueSpaceMap = nullptr;
				}


				CDatatypeBooleanValueSpaceData* CDatatypeBooleanValueSpaceData::initBooleanValueSpaceData(CDatatypeValueSpaceBooleanType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mBooleanValueSpaceMap) {
						mBooleanValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeBooleanValueSpaceData* CDatatypeBooleanValueSpaceData::copyBooleanValueSpaceData(CDatatypeBooleanValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mBooleanValueSpaceMap && !mBooleanValueSpaceMap) {
						getBooleanValueSpaceMap(true);
						mBooleanValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mBooleanValueSpaceMap);
					} else if (mBooleanValueSpaceMap) {
						if (spaceData->mBooleanValueSpaceMap) {
							mBooleanValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mBooleanValueSpaceMap);
						} else {
							mBooleanValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeBooleanValueSpaceMap* CDatatypeBooleanValueSpaceData::getBooleanValueSpaceMap(bool create) {
					if (create && !mBooleanValueSpaceMap) {
						mBooleanValueSpaceMap = CObjectParameterizingAllocator< CDatatypeBooleanValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBooleanValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mBooleanValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeBooleanValueSpaceData::createValueSpaceMap() {
					return getBooleanValueSpaceMap(true);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
