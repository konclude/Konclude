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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYCACHEITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYCACHEITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CBackendRepresentativeMemoryCacheContext.h"
#include "CBackendRepresentativeMemoryCardinalityValueLinker.h"


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
				 *		\class		CBackendRepresentativeMemoryCardinalityCacheItem
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCardinalityCacheItem : public CLinkerBase<cint64,CBackendRepresentativeMemoryCardinalityCacheItem> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCardinalityCacheItem(CBackendRepresentativeMemoryCacheContext* context);


						CBackendRepresentativeMemoryCardinalityCacheItem* initCacheEntry(cint64 signature, cint64 entryID);

						cint64 getCacheEntryID();
						CBackendRepresentativeMemoryCardinalityCacheItem* setCacheEntryID(cint64 entryID);

						cint64 getSignature();
						CBackendRepresentativeMemoryCardinalityCacheItem* setSignature(cint64 signature);


						CBackendRepresentativeMemoryCardinalityCacheItem* addCardinalityCacheValueLinker(CBackendRepresentativeMemoryCardinalityValueLinker* linker);
						CBackendRepresentativeMemoryCardinalityValueLinker* getCardinalityCacheValueLinker();


						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* getTagCardinalityCacheValueHash(bool create = false);
						CBackendRepresentativeMemoryCardinalityCacheItem* setTagCardinalityCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* hash);
						cint64 getCardinalityCacheValueCount();



					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheContext* mContext;
						cint64 mSignature;
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalityValueLinker*>* mTagCardValueHash;
						CBackendRepresentativeMemoryCardinalityValueLinker* mCardinalityValueLinker;
						cint64 mCardinalityValueCount;




					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCARDINALITYCACHEITEM_H
