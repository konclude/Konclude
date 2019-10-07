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

#include "CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryCacheContext* context) : CBackendRepresentativeMemoryLabelCacheItemExtensionData(context) {
					mCacheItemExtensionType = INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::initNeighbourArrayIndexData(CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel) {
					mCombinedNeighbourRoleSetLabel = combinedNeighbourRoleSetLabel;
					mArraySize = mCombinedNeighbourRoleSetLabel->getCacheValueCount();
					mIndexNeighbourRoleSetLabelArray = CObjectAllocator<CBackendRepresentativeMemoryLabelCacheItem*>::allocateAndConstructArray(mContext->getMemoryAllocationManager(), mArraySize);
					mNeighbourRoleSetLabelIndexHash = CObjectParameterizingAllocator< CCACHINGHASH<CBackendRepresentativeMemoryLabelCacheItem*, cint64>, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
					CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinker = mCombinedNeighbourRoleSetLabel->getCacheValueLinker();
					for (cint64 i = 0; i < mArraySize; ++i) {
						if (cacheValueLinker) {
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel = (CBackendRepresentativeMemoryLabelCacheItem*)cacheValueLinker->getCacheValue().getIdentification();
							mIndexNeighbourRoleSetLabelArray[i] = neighbourRoleSetLabel;
							mNeighbourRoleSetLabelIndexHash->insert(neighbourRoleSetLabel, i);
							cacheValueLinker = cacheValueLinker->getNext();
						} else {
							mIndexNeighbourRoleSetLabelArray[i] = nullptr;
						}
					}
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::initNeighbourArrayIndexData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* neighArray) {
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::getCombinedNeighbourRoleSetLabel() {
					return mCombinedNeighbourRoleSetLabel;
				}

				cint64 CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::getArraySize() {
					return mArraySize;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::getNeighbourRoleSetLabel(cint64 index) {
					if (index >= 0 && index < mArraySize) {
						return mIndexNeighbourRoleSetLabelArray[index];
					}
					return nullptr;
				}

				cint64 CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::getIndex(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel) {
					return mNeighbourRoleSetLabelIndexHash->value(neighbourRoleSetLabel, -1);
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
