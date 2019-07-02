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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CBackendRepresentativeMemoryCacheContext.h"
#include "CBackendRepresentativeMemoryLabelValueLinker.h"
#include "CBackendRepresentativeMemoryCachingFlags.h"
#include "CBackendRepresentativeMemoryLabelCacheItemExtensionData.h"


// Other includes
#include "Utilities/Memory/CObjectParameterizingAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryLabelCacheItem
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryLabelCacheItem : public CLinkerBase<cint64,CBackendRepresentativeMemoryLabelCacheItem>, public CBackendRepresentativeMemoryCachingFlags {
					// public methods
					public:

						enum LABEL_CACHE_ITEM_TYPE {
							DETERMINISTIC_CONCEPT_SET_LABEL = 0,
							NONDETERMINISTIC_CONCEPT_SET_LABEL = 1,

							FULL_CONCEPT_SET_LABEL = 2,

							DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL = 3,
							NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL = 4,
							DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL = 5,
							NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL = 6,


							DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL = 7,
							NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL = 8,


							DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL = 9,
							NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL = 10,

							DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL = 11,
							NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL = 12,


							INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL = 13,


							NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL = 14,
							NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL = 15,

						};

						static const cint64 LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT = 15;
						static const cint64 LABEL_CACHE_ITEM_TYPE_COUNT = 16;


						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItem(CBackendRepresentativeMemoryCacheContext* context);


						CBackendRepresentativeMemoryLabelCacheItem* initCacheEntry(cint64 signature, cint64 entryID, LABEL_CACHE_ITEM_TYPE type);
						LABEL_CACHE_ITEM_TYPE getLabelType();

						cint64 getCacheEntryID();
						CBackendRepresentativeMemoryLabelCacheItem* setCacheEntryID(cint64 entryID);

						cint64 getSignature();
						CBackendRepresentativeMemoryLabelCacheItem* setSignature(cint64 signature);


						CBackendRepresentativeMemoryLabelCacheItem* addCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker);
						CBackendRepresentativeMemoryLabelValueLinker* getCacheValueLinker();


						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* getTagCacheValueHash(bool create = false);
						CBackendRepresentativeMemoryLabelCacheItem* setTagCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* hash);
						cint64 getCacheValueCount();


						bool hasCachedTagValue(cint64 tag);


						CBackendRepresentativeMemoryLabelCacheItemExtensionData* getExtensionData(cint64 extensionType);
						CBackendRepresentativeMemoryLabelCacheItem* setExtensionData(cint64 extensionType, CBackendRepresentativeMemoryLabelCacheItemExtensionData* extensionData);

						CBackendRepresentativeMemoryLabelCacheItem* incIndividualAssociationCount(cint64 count = 1);
						CBackendRepresentativeMemoryLabelCacheItem* decIndividualAssociationCount(cint64 count = 1);
						cint64 getIndividualAssociationCount();



					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheContext* mContext;

						LABEL_CACHE_ITEM_TYPE mCacheItemType;

						cint64 mSignature;
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* mTagValueHash;
						CBackendRepresentativeMemoryLabelValueLinker* mValueLinker;
						cint64 mValueCount;

						cint64 mIndiAssociationCount;

						CBackendRepresentativeMemoryLabelCacheItemExtensionData* mExtensionData[CBackendRepresentativeMemoryLabelCacheItemExtensionData::LABEL_CACHE_ITEM_EXTENSION_TYPE_COUNT];



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEM_H
