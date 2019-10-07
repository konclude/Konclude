/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
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
#include "CBackendRepresentativeMemoryCachingFlags.h"
#include "CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker.h"
#include "CBackendRepresentativeMemoryCacheTemporaryLabelReference.h"


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
				class CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker : public CLinkerBase<CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker*,CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker>, public CBackendRepresentativeMemoryCachingFlags {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker();

						enum UPDATE_TYPE {
							ADDITION,
							REPLACEMENT,
							REMOVAL
						};

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* initAccociationWriteData(CIndividual* individual, UPDATE_TYPE labelUpdateType, UPDATE_TYPE linksUpdateType);
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* initAccociationWriteData(cint64 individualID, UPDATE_TYPE labelUpdateType, UPDATE_TYPE linksUpdateType);

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredTemporaryLabelData(cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData);
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredLabelData(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* referredLabelData);

						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredLabel(cint64 labelType, CBackendRepresentativeMemoryCacheTemporaryLabelReference& labelRef);


						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredTemporaryCardinalityData(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* referredTmpCardData);
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setReferredCardinalityData(CBackendRepresentativeMemoryCardinalityCacheItem* referredCardData);


						CIndividual* getIndividual();
						cint64 getIndividualID();

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* getReferredTemporaryLabelData(cint64 labelType);
						CBackendRepresentativeMemoryLabelCacheItem* getReferredLabelData(cint64 labelType);

						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* getReferredTemporaryCardinalityData();
						CBackendRepresentativeMemoryCardinalityCacheItem* getReferredCardinalityData();

						UPDATE_TYPE getLabelUpdateType();
						UPDATE_TYPE getLinksUpdateType();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setLinksUpdateType(UPDATE_TYPE updateType);


						CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* getRoleSetNeighbourUpdateDataLinker();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setRoleSetNeighbourUpdateDataLinker(CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinker);

						cint64 getUsedAssociationUpdateId();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setUsedAssociationUpdateId(cint64 id);


						cint64 getIntegratedIndirectlyConnectedIndividualsChangeId();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setIntegratedIndirectlyConnectedIndividualsChangeId(cint64 integratedChangeId);

						bool isIndirectlyConnectedNominalIndividual();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setIndirectlyConnectedNominalIndividual(bool indirectlyConnected);

						bool hasIndirectlyConnectedIndividualIntegration();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setIndirectlyConnectedIndividualIntegration(bool indirectlyConnectedIndividualIntegration);


						cint64 getRepresentativeSameIndividualId();
						CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* setRepresentativeSameIndividualId(cint64 indiId);


					// protected methods
					protected:

					// protected variables
					protected:
						UPDATE_TYPE mLabelUpdateType;
						UPDATE_TYPE mLinksUpdateType;

						cint64 mAssociationUpdateId;
						cint64 mIntegratedIndirectlyConnectedIndividualsChangeId;

						bool mIndirectlyConnectedIndividualIntegration;
						bool mIndirectlyConnectedNominalIndividual;

						cint64 mIndividualID;
						CIndividual* mIndividual;

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* mReferredTmpLabelData[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT];
						CBackendRepresentativeMemoryLabelCacheItem* mReferredLabelData[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT];

						CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* mReferredTmpCardData;
						CBackendRepresentativeMemoryCardinalityCacheItem* mReferredCardData;

						CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* mRoleSetNeighbourUpdateDataLinker;

						cint64 mRepresentativeSameIndiId;


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
