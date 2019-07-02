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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHESLOTITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHESLOTITEM_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelSignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData.h"

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


						CBackendRepresentativeMemoryCacheSlotItem* setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash);
						CBackendRepresentativeMemoryCacheSlotItem* setSignatureCardinalityItemHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* sigItemHash);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* getSignatureLabelItemHash(cint64 labelType);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* getSignatureCardinalityItemHash();
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* getNominaIIndividualdIndirectConnectionDataHash();

						bool hasCacheReaders();

						bool hasSameIndividualsMergings();
						CBackendRepresentativeMemoryCacheSlotItem* setSameIndividualsMergings(bool sameIndisMergings);


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* getIndividualIdAssociationDataHash();
						CBackendRepresentativeMemoryCacheSlotItem* setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash);
						CBackendRepresentativeMemoryCacheSlotItem* setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash);

						cint64 getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** getIndividualIdAssoiationDataVector();
						CBackendRepresentativeMemoryCacheSlotItem* setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector);

						// protected methods
					protected:

						// protected variables
					protected:
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* mSigLabelItemHash[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* mSigCardLabelItemHash;

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* mIndiIdAssoDataHash;

						cint64 mIndiIdAssoDataVectorSize;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** mIndiIdAssoDataVector;


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* mNominaIIndividualdIndirectConnectionDataHash;


						QAtomicInt mReaderSharingCount;
						bool mReaderUsing;
						bool mSameIndividalsMergings;


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
