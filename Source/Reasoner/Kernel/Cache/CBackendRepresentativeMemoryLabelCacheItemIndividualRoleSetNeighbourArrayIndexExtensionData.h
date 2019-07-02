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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALROLESETNEIGHBOURARRAYINDEXEXTENSIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALROLESETNEIGHBOURARRAYINDEXEXTENSIONDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"
#include "CBackendRepresentativeMemoryLabelCacheItemExtensionData.h"


// Other includes
#include "Reasoner/Ontology/CIndividualBackendCachingData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArrayIndexData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData : public CBackendRepresentativeMemoryLabelCacheItemExtensionData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryCacheContext* context);

						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* initNeighbourArrayIndexData(CBackendRepresentativeMemoryLabelCacheItem* combinedNeighbourRoleSetLabel);
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* initNeighbourArrayIndexData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* neighArrayIndexData);

						CBackendRepresentativeMemoryLabelCacheItem* getCombinedNeighbourRoleSetLabel();
						cint64 getArraySize();

						CBackendRepresentativeMemoryLabelCacheItem* getNeighbourRoleSetLabel(cint64 index);
						cint64 getIndex(CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel);

					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryLabelCacheItem* mCombinedNeighbourRoleSetLabel;
						cint64 mArraySize;

						CBackendRepresentativeMemoryLabelCacheItem** mIndexNeighbourRoleSetLabelArray;
						CCACHINGHASH<CBackendRepresentativeMemoryLabelCacheItem*, cint64>* mNeighbourRoleSetLabelIndexHash;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEMINDIVIDUALROLESETNEIGHBOURARRAYINDEXEXTENSIONDATA_H
