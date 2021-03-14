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

#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray::CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray(CBackendRepresentativeMemoryCacheContext* context) {
					mContext = context;
					mIndexData = nullptr;
				}



				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray::initNeighbourArray(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighArray) {
					mIndexData = neighArray->getIndexData();
					mDataArray = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData >::allocateAndConstructArray(mContext->getMemoryAllocationManager(), mIndexData->getArraySize());
					for (cint64 i = 0; i < mIndexData->getArraySize(); ++i) {
						mDataArray[i] = neighArray->at(i);
					}
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray::initNeighbourArray(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData) {
					mIndexData = indexData;
					mDataArray = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData >::allocateAndConstructArray(mContext->getMemoryAllocationManager(), mIndexData->getArraySize());
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray::at(cint64 index) {
					if (index < 0 || index >= mIndexData->getArraySize()) {
						bool debug = true;
					}
					return mDataArray[index];
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray::getIndexData() {
					return mIndexData;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
