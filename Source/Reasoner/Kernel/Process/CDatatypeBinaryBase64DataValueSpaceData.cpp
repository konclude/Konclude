/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CDatatypeBinaryBase64DataValueSpaceData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeBinaryBase64DataValueSpaceData::CDatatypeBinaryBase64DataValueSpaceData(CProcessContext* processContext) : CDatatypeCompareValueSpaceData(processContext),mBinaryBase64DataValueSpaceMap(processContext) {
					mValueSpaceMap = &mBinaryBase64DataValueSpaceMap;
				}


				CDatatypeBinaryBase64DataValueSpaceData* CDatatypeBinaryBase64DataValueSpaceData::initBinaryBase64DataValueSpaceData(CDatatypeValueSpaceBinaryBase64DataType* valueSpaceType) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(nullptr);
					mBinaryBase64DataValueSpaceMap.initDatatypeCompareValueSpaceMap(valueSpaceType);
					return this;
				}


				CDatatypeBinaryBase64DataValueSpaceData* CDatatypeBinaryBase64DataValueSpaceData::copyBinaryBase64DataValueSpaceData(CDatatypeBinaryBase64DataValueSpaceData* spaceData) {
					CDatatypeCompareValueSpaceData::initValueSpaceData(spaceData);
					mBinaryBase64DataValueSpaceMap.initDatatypeCompareValueSpaceMap(spaceData->getBinaryBase64DataValueSpaceMap());
					return this;
				}


				CDatatypeBinaryDataValueSpaceMap* CDatatypeBinaryBase64DataValueSpaceData::getBinaryBase64DataValueSpaceMap() {
					return &mBinaryBase64DataValueSpaceMap;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
