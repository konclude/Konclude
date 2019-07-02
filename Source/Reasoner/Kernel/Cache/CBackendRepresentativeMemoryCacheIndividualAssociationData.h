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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALASSOCIATIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALASSOCIATIONDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"
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
				 *		\class		CBackendRepresentativeMemoryCacheIndividualAssociationData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheIndividualAssociationData : public CIndividualBackendCachingData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheIndividualAssociationData();

						CBackendRepresentativeMemoryCacheIndividualAssociationData* initAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* initAssociationData(CIndividual* individual);

						CBackendRepresentativeMemoryLabelCacheItem* getBackendLabelCacheEntry();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setBackendLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem* cacheEntry);

						CBackendRepresentativeMemoryCardinalityCacheItem* getBackendCardinalityCacheEntry();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setBackendCardinalityCacheEntry(CBackendRepresentativeMemoryCardinalityCacheItem* cacheEntry);

						bool isCompletelyHandled();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setCompletelyHandled(bool completelyHandled);

						bool isCompletelySaturated();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setCompletelySaturated(bool completelySaturated);

						bool isIncompletelyMarked();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIncompletelyMarked(bool marked);

					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryLabelCacheItem* mLabelCacheEntry;
						CBackendRepresentativeMemoryCardinalityCacheItem* mCardinalityCacheEntry;

						CIndividual* mIndividual;
						bool mCompletelyHandled;
						bool mCompletelySaturated;

						bool mIncompletelyMarked;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALASSOCIATIONDATA_H
