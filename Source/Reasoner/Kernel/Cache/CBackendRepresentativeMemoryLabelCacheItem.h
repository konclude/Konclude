/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYLABELCACHEITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CBackendRepresentativeMemoryCacheContext.h"
#include "CBackendRepresentativeMemoryLabelValueLinker.h"


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
				class CBackendRepresentativeMemoryLabelCacheItem : public CLinkerBase<cint64,CBackendRepresentativeMemoryLabelCacheItem> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryLabelCacheItem(CBackendRepresentativeMemoryCacheContext* context);


						CBackendRepresentativeMemoryLabelCacheItem* initCacheEntry(cint64 signature, cint64 entryID);

						cint64 getCacheEntryID();
						CBackendRepresentativeMemoryLabelCacheItem* setCacheEntryID(cint64 entryID);

						cint64 getSignature();
						CBackendRepresentativeMemoryLabelCacheItem* setSignature(cint64 signature);


						CBackendRepresentativeMemoryLabelCacheItem* addDeterministicCacheValueLinker(CBackendRepresentativeMemoryLabelValueLinker* linker);
						CBackendRepresentativeMemoryLabelValueLinker* getDeterministicCacheValueLinker();


						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* getDeterministicTagCacheValueHash(bool create = false);
						CBackendRepresentativeMemoryLabelCacheItem* setDeterministicTagCacheValueHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* hash);
						cint64 getDeterministicCacheValueCount();

						bool isCompletelyHandled();
						CBackendRepresentativeMemoryLabelCacheItem* setCompletelyHandled(bool completelyHandled);

						bool isCompletelySaturated();
						CBackendRepresentativeMemoryLabelCacheItem* setCompletelySaturated(bool completelySaturated);


					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheContext* mContext;
						cint64 mSignature;
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelValueLinker*>* mDetTagValueHash;
						CBackendRepresentativeMemoryLabelValueLinker* mDetValueLinker;
						cint64 mDetValueCount;

						bool mCompletelyHandled;
						bool mCompletelySaturated;



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
