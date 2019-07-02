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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHESLOTITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHESLOTITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelSignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem.h"

// Other includes
#include "Utilities/Memory/CMemoryPoolContainer.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheSlotItem
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheSlotItem : public CMemoryPoolContainer, public CLinkerBase<CBackendRepresentativeMemoryCacheSlotItem*,CBackendRepresentativeMemoryCacheSlotItem> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheSlotItem(CMemoryPool* memoryPool = nullptr);

						bool incReader();
						bool incReader(cint64 incCount);
						bool decReader();

						CBackendRepresentativeMemoryCacheSlotItem* setSignatureLabelItemHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* sigItemHash);
						CBackendRepresentativeMemoryCacheSlotItem* setSignatureCardinalityItemHash(CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* sigItemHash);
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* getSignatureLabelItemHash();
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* getSignatureCardinalityItemHash();

						bool hasCacheReaders();


					// protected methods
					protected:

					// protected variables
					protected:
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* mSigLabelItemHash;
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* mSigCardLabelItemHash;

						QAtomicInt mReaderSharingCount;
						bool mReaderUsing;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHESLOTITEM_H
