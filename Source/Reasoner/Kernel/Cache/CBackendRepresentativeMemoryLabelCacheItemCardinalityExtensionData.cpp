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

#include "CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData(CBackendRepresentativeMemoryCacheContext* context) : CBackendRepresentativeMemoryLabelCacheItemExtensionData(context) {
					mCacheItemExtensionType = CARDINALITY_HASH;
				}

				CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::initCardinalityExtensionData() {
					mRoleCardinalityDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(mContext->getMemoryAllocationManager(), mContext);
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItemCardinalityData* CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::getRoleCardinalityData(cint64 roleTag) {
					return mRoleCardinalityDataHash->value(roleTag);
				}

				CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::setRoleCardinalityData(cint64 roleTag, CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardinalityData) {
					mRoleCardinalityDataHash->insert(roleTag, cardinalityData);
					return this;
				}

				CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::getRoleCardinalityDataHash() {
					return mRoleCardinalityDataHash;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
