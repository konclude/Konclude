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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H

// Libraries includes
#include <QAtomicPointer>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheSlotItem.h"
#include "CBackendRepresentativeMemoryLabelSignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"


// Other includes
#include "Utilities/Container/CLinker.h"

#include "Reasoner/Kernel/Process/CConceptSaturationDescriptor.h"
#include "Reasoner/Kernel/Process/CConceptDescriptor.h"

#include "Reasoner/Ontology/CIndividualProcessData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Process;

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheReader : public CLinkerBase<CBackendRepresentativeMemoryCacheReader*,CBackendRepresentativeMemoryCacheReader> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheReader();


						CBackendRepresentativeMemoryCacheReader* updateSlot(CBackendRepresentativeMemoryCacheSlotItem* updatedSlot);

						bool hasCacheEntry(cint64 signature);
						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem* getLabelCacheEntry(cint64 signature);
						CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem* getCardinalityCacheEntry(cint64 signature);

						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntry(cint64 signature, cint64 conCount, CConceptSaturationDescriptor* conDesLinker);
						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntry(cint64 signature, cint64 conCount, CConceptDescriptor* conDesLinker, CConcept* exceptionConcept);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* getIndividualAssociation(CIndividual* individual);

						CBackendRepresentativeMemoryCardinalityCacheItem* getCardinalityCacheEntry(cint64 signature, CPROCESSHASH<CRole*,cint64>* roleFreeCardHash);

						const CCacheValue getCacheValue(CConcept* concept, bool negation);
						const CCacheValue getCacheValue(CRole* role);

					// protected methods
					protected:
						bool hasUpdatedSlotItem();
						bool switchToUpdatedSlotItem();

					// protected variables
					protected:
						CBackendRepresentativeMemoryCacheSlotItem* mCurrentSlot;
						QAtomicPointer<CBackendRepresentativeMemoryCacheSlotItem> mUpdatedSlot;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEREADER_H
