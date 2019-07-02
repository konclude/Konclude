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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYASSOCIATIONWRITEDATALINKER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYASSOCIATIONWRITEDATALINKER_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker*,CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker> {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker();

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* initAccociationWriteData(CIndividual* individual);

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredTemporaryLabelData(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData);
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem* referredLabelData);

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredTemporaryCardinalityData(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* referredTmpCardData);
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredCardinalityData(CBackendRepresentativeMemoryCardinalityCacheItem* referredCardData);


						CIndividual* getIndividual();
						cint64 getIndividualID();

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* getReferredTemporaryLabelData();
						CBackendRepresentativeMemoryLabelCacheItem* getReferredLabelData();

						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* getReferredTemporaryCardinalityData();
						CBackendRepresentativeMemoryCardinalityCacheItem* getReferredCardinalityData();

						bool isCompletelyHandled();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setCompletelyHandled(bool completelyHandled);

						bool isCompletelySaturated();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setCompletelySaturated(bool completelySaturated);


					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mIndividualID;
						CIndividual* mIndividual;

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* mReferredTmpLabelData;
						CBackendRepresentativeMemoryLabelCacheItem* mReferredLabelData;

						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* mReferredTmpCardData;
						CBackendRepresentativeMemoryCardinalityCacheItem* mReferredCardData;


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

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHETEMPORARYASSOCIATIONWRITEDATALINKER_H
