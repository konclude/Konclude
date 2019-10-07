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

#include "CDatatypeStringValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeStringValueSpaceData::CDatatypeStringValueSpaceData(CProcessContext* processContext) : CDatatypeValueSpaceData(processContext),mProcessContext(processContext) {
					mValueSpaceMap = nullptr;
				}


				CDatatypeStringValueSpaceData* CDatatypeStringValueSpaceData::initStringValueSpaceData(CDatatypeValueSpaceStringType* valueSpaceType) {
					CDatatypeValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mValueSpaceMap) {
						mValueSpaceMap->initDatatypeStringValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeStringValueSpaceData* CDatatypeStringValueSpaceData::copyStringValueSpaceData(CDatatypeStringValueSpaceData* spaceData) {
					CDatatypeValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mValueSpaceMap && !mValueSpaceMap) {
						getValueSpaceMap(true);
						mValueSpaceMap->initDatatypeStringValueSpaceMap(spaceData->mValueSpaceMap);
					} else if (mValueSpaceMap) {
						if (spaceData->mValueSpaceMap) {
							mValueSpaceMap->initDatatypeStringValueSpaceMap(spaceData->mValueSpaceMap);
						} else {
							mValueSpaceMap->initDatatypeStringValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}

				CDatatypeStringValueSpaceMap* CDatatypeStringValueSpaceData::getValueSpaceMap(bool create) {
					if (create && !mValueSpaceMap) {
						mValueSpaceMap = CObjectParameterizingAllocator< CDatatypeStringValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mValueSpaceMap->initDatatypeStringValueSpaceMap(mValueSpaceType);
					}
					return mValueSpaceMap;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
