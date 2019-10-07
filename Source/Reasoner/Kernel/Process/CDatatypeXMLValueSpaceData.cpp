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

#include "CDatatypeXMLValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeXMLValueSpaceData::CDatatypeXMLValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mXMLValueSpaceMap = nullptr;
				}


				CDatatypeXMLValueSpaceData* CDatatypeXMLValueSpaceData::initXMLValueSpaceData(CDatatypeValueSpaceXMLType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mXMLValueSpaceMap) {
						mXMLValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeXMLValueSpaceData* CDatatypeXMLValueSpaceData::copyXMLValueSpaceData(CDatatypeXMLValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mXMLValueSpaceMap && !mXMLValueSpaceMap) {
						getXMLValueSpaceMap(true);
						mXMLValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mXMLValueSpaceMap);
					} else if (mXMLValueSpaceMap) {
						if (spaceData->mXMLValueSpaceMap) {
							mXMLValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mXMLValueSpaceMap);
						} else {
							mXMLValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeXMLValueSpaceMap* CDatatypeXMLValueSpaceData::getXMLValueSpaceMap(bool create) {
					if (create && !mXMLValueSpaceMap) {
						mXMLValueSpaceMap = CObjectParameterizingAllocator< CDatatypeXMLValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mXMLValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mXMLValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeXMLValueSpaceData::createValueSpaceMap() {
					return getXMLValueSpaceMap(true);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
