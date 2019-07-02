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
