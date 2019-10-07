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

#include "CDatatypeIRIValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeIRIValueSpaceData::CDatatypeIRIValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mIRIValueSpaceMap = nullptr;
				}


				CDatatypeIRIValueSpaceData* CDatatypeIRIValueSpaceData::initIRIValueSpaceData(CDatatypeValueSpaceIRIType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mIRIValueSpaceMap) {
						mIRIValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeIRIValueSpaceData* CDatatypeIRIValueSpaceData::copyIRIValueSpaceData(CDatatypeIRIValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mIRIValueSpaceMap && !mIRIValueSpaceMap) {
						getIRIValueSpaceMap(true);
						mIRIValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mIRIValueSpaceMap);
					} else if (mIRIValueSpaceMap) {
						if (spaceData->mIRIValueSpaceMap) {
							mIRIValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mIRIValueSpaceMap);
						} else {
							mIRIValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeIRIValueSpaceMap* CDatatypeIRIValueSpaceData::getIRIValueSpaceMap(bool create) {
					if (create && !mIRIValueSpaceMap) {
						mIRIValueSpaceMap = CObjectParameterizingAllocator< CDatatypeIRIValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mIRIValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mIRIValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeIRIValueSpaceData::createValueSpaceMap() {
					return getIRIValueSpaceMap(true);
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
