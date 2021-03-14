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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALASSOCIATIONDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEINDIVIDUALASSOCIATIONDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"
#include "CBackendRepresentativeMemoryCachingFlags.h"
#include "CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash.h"
#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationContext.h"


// Other includes
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
				class CBackendRepresentativeMemoryCacheIndividualAssociationData : public CIndividualBackendCachingData, public CBackendRepresentativeMemoryCachingFlags {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheIndividualAssociationData();

						CBackendRepresentativeMemoryCacheIndividualAssociationData* initAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData, bool increaseUpdateId = true);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* initAssociationData(cint64 indiId);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIndividualId(cint64 indiId);

						CBackendRepresentativeMemoryLabelCacheItem* getDeterministicConceptSetLabelCacheEntry();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setDeterministicConceptSetLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem* cacheEntry);

						CBackendRepresentativeMemoryLabelCacheItem* getLabelCacheEntry(cint64 labelType);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setLabelCacheEntry(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* cacheEntry);


						CBackendRepresentativeMemoryLabelCacheItem* getDeterministicMergedSameConsideredLabelCacheEntry();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setDeterministicMergedSameConsideredLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem* cacheEntry);


						CBackendRepresentativeMemoryCardinalityCacheItem* getBackendCardinalityCacheEntry();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setBackendCardinalityCacheEntry(CBackendRepresentativeMemoryCardinalityCacheItem* cacheEntry);

						bool isIncompletelyMarked();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIncompletelyMarked(bool marked);



						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* getNeighbourRoleSetHash();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setNeighbourRoleSetHash(CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetHash);
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* getRoleSetNeighbourArray();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setRoleSetNeighbourArray(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray);

						cint64 getAssociationDataUpdateId();
						cint64 getCacheUpdateId();
						cint64 getCacheTouchId();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setCacheUpdateId(cint64 updateId);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setCacheTouchId(cint64 updateId);



						cint64 getLastIntegratedIndirectlyConnectedIndividualsChangeId();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setLastIntegratedIndirectlyConnectedIndividualsChangeId(cint64 lastIntegratedChangeId);


						bool isIndirectlyConnectedNominalIndividual();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIndirectlyConnectedNominalIndividual(bool indirectlyConnected);

						bool hasIndirectlyConnectedIndividualIntegration();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIndirectlyConnectedIndividualIntegration(bool indirectlyConnectedIndividualIntegration);

						cint64 getRepresentativeSameIndividualId();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setRepresentativeSameIndividualId(cint64 indiId);
						cint64 hasRepresentativeSameIndividualMerging();


						cint64 getDeterministicSameIndividualId();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setDeterministicSameIndividualId(cint64 indiId);
						cint64 hasDeterministicSameIndividualMerging();


						cint64 getAssociatedIndividualId();



						cint64 hasProblematicLevel();
						cint64 getProblematicLevel();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setProblematicLevel(cint64 level);
						CBackendRepresentativeMemoryCacheIndividualAssociationData* incProblematicLevel(cint64 count = 1);



						cint64 hasProblematicLeveledNeigbour();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setProblematicLeveledNeigbour(bool neighbourPropLeveled);


						CBackendRepresentativeMemoryCacheIndividualAssociationData* getPreviousData();
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						cint64 mDebugUpdateRecomputationId = -1;
						bool mDebugNeighbourCompletionRequested = false;
						bool mDebugNeighbourCompletionPerformed = false;
						bool mDebugNeighbourCompletionExtended = false;
						bool mDebugSameIndividualsCompletion = false;
						QString mDebugUpdateFunction;
						CIndividual* mDebugIndi = nullptr;
#endif


						CBackendRepresentativeMemoryCacheIndividualAssociationContext* getIndividualAssociationMemoryContext();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setIndividualAssociationMemoryContext(CBackendRepresentativeMemoryCacheIndividualAssociationContext* memCon);


						CBackendRepresentativeMemoryCacheIndividualAssociationData* setLastPropagationCuttingUpdateId(cint64 id);
						cint64 getLastPropagationCuttingUpdateId();
						bool hasLastPropagationCuttingUpdateId();


						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* getPropagationCutRemovedNeighbourIndividualLinker();
						CBackendRepresentativeMemoryCacheIndividualAssociationData* setPropagationCutRemovedNeighbourIndividualLinker(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* linker);


					// protected methods
					protected:

					// protected variables
					protected:
						CBackendRepresentativeMemoryLabelCacheItem* mLabelCacheEntries[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT];
						CBackendRepresentativeMemoryCardinalityCacheItem* mCardinalityCacheEntry;

						CBackendRepresentativeMemoryLabelCacheItem* mDetMergedSameConsideredLabelCacheEntry;

						cint64 mIndiID;

						bool mIncompletelyMarked;
						bool mIndirectlyConnectedNominalIndividual;
						bool mIndirectlyConnectedIndividualIntegration;


						cint64 mProblematicLevel;
						bool mProblematicLeveledNeighbour;


						cint64 mAssociationDataUpdateId;
						cint64 mCacheUpdateId;
						cint64 mCacheTouchId;
						cint64 mLastIntegratedIndirectlyConnectedIndividualsChangeId;

						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* mNeighbourRoleSetHash;
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* mRoleSetNeighbourArray;

						cint64 mRepresentativeSameIndiId;
						cint64 mDeterministicSameIndiId;


						cint64 mLastPropagationCuttingUpdateId;
						CBackendRepresentativeMemoryCacheIndividualAssociationContext* mMemContext;

						CBackendRepresentativeMemoryCacheIndividualAssociationData* mPrevData = nullptr;


						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* mPropCutRemovedNeighbourIndiLinker;

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
