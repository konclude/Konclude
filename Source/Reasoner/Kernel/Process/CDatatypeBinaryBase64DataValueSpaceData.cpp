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

#include "CDatatypeBinaryBase64DataValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeBinaryBase64DataValueSpaceData::CDatatypeBinaryBase64DataValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext) {
					mBinaryBase64DataValueSpaceMap = nullptr;
				}


				CDatatypeBinaryBase64DataValueSpaceData* CDatatypeBinaryBase64DataValueSpaceData::initBinaryBase64DataValueSpaceData(CDatatypeValueSpaceBinaryBase64DataType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mValueSpaceType = valueSpaceType;
					if (mBinaryBase64DataValueSpaceMap) {
						mBinaryBase64DataValueSpaceMap->initDatatypeCompareValueSpaceMap(valueSpaceType);
					}
					return this;
				}


				CDatatypeBinaryBase64DataValueSpaceData* CDatatypeBinaryBase64DataValueSpaceData::copyBinaryBase64DataValueSpaceData(CDatatypeBinaryBase64DataValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mValueSpaceType = spaceData->mValueSpaceType;
					if (spaceData->mBinaryBase64DataValueSpaceMap && !mBinaryBase64DataValueSpaceMap) {
						getBinaryBase64DataValueSpaceMap(true);
						mBinaryBase64DataValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mBinaryBase64DataValueSpaceMap);
					} else if (mBinaryBase64DataValueSpaceMap) {
						if (spaceData->mBinaryBase64DataValueSpaceMap) {
							mBinaryBase64DataValueSpaceMap->initDatatypeCompareValueSpaceMap(spaceData->mBinaryBase64DataValueSpaceMap);
						} else {
							mBinaryBase64DataValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
						}
					}
					return this;
				}


				CDatatypeBinaryDataValueSpaceMap* CDatatypeBinaryBase64DataValueSpaceData::getBinaryBase64DataValueSpaceMap(bool create) {
					if (create && !mBinaryBase64DataValueSpaceMap) {
						mBinaryBase64DataValueSpaceMap = CObjectParameterizingAllocator< CDatatypeBinaryDataValueSpaceMap,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						mBinaryBase64DataValueSpaceMap->initDatatypeCompareValueSpaceMap(mValueSpaceType);
					}
					return mBinaryBase64DataValueSpaceMap;
				}

				CDatatypeCompareValueSpaceMap* CDatatypeBinaryBase64DataValueSpaceData::createValueSpaceMap() {
					return getBinaryBase64DataValueSpaceMap(true);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
