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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHESLOTITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHESLOTITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CSignatureSatisfiableExpanderCacheRedirectionItem.h"
#include "CSignatureSatisfiableExpanderCacheHasher.h"
#include "CSignatureSatisfiableExpanderCacheEntry.h"


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
				 *		\class		CSignatureSatisfiableExpanderCacheSlotItem
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCacheSlotItem : public CMemoryPoolContainer, public CLinkerBase<CSignatureSatisfiableExpanderCacheSlotItem*,CSignatureSatisfiableExpanderCacheSlotItem> {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCacheSlotItem(CMemoryPool* memoryPool = nullptr);

						bool incReader();
						bool incReader(cint64 incCount);
						bool decReader();


						CSignatureSatisfiableExpanderCacheSlotItem* setSignatureItemHash(CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* sigItemHash);
						CSignatureSatisfiableExpanderCacheSlotItem* setHasherItemHash(CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* hasherItemHash);

						bool hasCacheReaders();


						CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* getSignatureItemHash();
						CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* getHasherItemHash();



					// protected methods
					protected:

					// protected variables
					protected:
						CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* mSigItemHash;
						CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* mHasherItemHash;

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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHESLOTITEM_H
