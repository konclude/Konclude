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

#include "CDatatypeRealValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeRealValueSpaceData::CDatatypeRealValueSpaceData(CProcessContext* processContext) : CDatatypeValueSpaceData(processContext),mProcessContext(processContext) {
					mValueSpaceMap = nullptr;
				}


				CDatatypeRealValueSpaceData* CDatatypeRealValueSpaceData::initRealValueSpaceData(CDatatypeValueSpaceRealType* valueSpaceType) {
					CDatatypeValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mValueSpaceMap) {
						mValueSpaceMap->initDatatypeRealValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeRealValueSpaceData* CDatatypeRealValueSpaceData::copyRealValueSpaceData(CDatatypeRealValueSpaceData* spaceData) {
					CDatatypeValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mValueSpaceMap && !mValueSpaceMap) {
						getValueSpaceMap(true);
						mValueSpaceMap->initDatatypeRealValueSpaceMap(spaceData->mValueSpaceMap);
					} else if (mValueSpaceMap) {
						if (spaceData->mValueSpaceMap) {
							mValueSpaceMap->initDatatypeRealValueSpaceMap(spaceData->mValueSpaceMap);
						} else {
							mValueSpaceMap->initDatatypeRealValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				bool CDatatypeRealValueSpaceData::hasValueSpaceMap() {
					return mValueSpaceMap;
				}

				CDatatypeRealValueSpaceMap* CDatatypeRealValueSpaceData::getValueSpaceMap(bool create) {
					if (create && !mValueSpaceMap) {
						mValueSpaceMap = CObjectParameterizingAllocator< CDatatypeRealValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mValueSpaceMap->initDatatypeRealValueSpaceMap(mValueSpaceType);
					}
					return mValueSpaceMap;
				}




			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
