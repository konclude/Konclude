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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATA_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATA_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CBackendRepresentativeMemoryCacheBaseContext.h"
#include "CBackendRepresentativeMemoryCacheOntologyContext.h"
#include "CBackendRepresentativeMemoryLabelCacheItem.h"
#include "CBackendRepresentativeMemoryCardinalityCacheItem.h"
#include "CBackendRepresentativeMemoryCachingFlags.h"
#include "CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash.h"
#include "CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray.h"
#include "CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem.h"
#include "CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationContext.h"
#include "CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker.h"


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
				 *		\class		CBackendRepresentativeMemoryCacheOntologyData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCacheOntologyData {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCacheOntologyData(CBackendRepresentativeMemoryCacheBaseContext* baseContext);

						CBackendRepresentativeMemoryCacheOntologyData* initOntologyData(cint64 ontologyIdentifer, bool directIndexing);
						CBackendRepresentativeMemoryCacheOntologyData* copyOntologyData(CBackendRepresentativeMemoryCacheOntologyData* data);

						cint64 getOntologyIdentifer();
						cint64 getUsageCount();
						CBackendRepresentativeMemoryCacheOntologyData* incUsageCount(cint64 count = 1);
						CBackendRepresentativeMemoryCacheOntologyData* decUsageCount(cint64 count = 1);

						CBackendRepresentativeMemoryCacheOntologyData* setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* getSignatureLabelItemHash(cint64 labelType);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* getNominaIIndividualdIndirectConnectionDataHash();

						bool hasSameIndividualsMergings();
						CBackendRepresentativeMemoryCacheOntologyData* setSameIndividualsMergings(bool sameIndisMergings);


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* getIndividualIdAssociationDataHash();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash);
						CBackendRepresentativeMemoryCacheOntologyData* setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash);

						cint64 getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** getIndividualIdAssoiationDataVector();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector);
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualIdAssoiationDataVectorSize(cint64 size);


						cint64 getNextEntryID(bool moveNext = true);
						CBackendRepresentativeMemoryCacheOntologyData* setNextEntryID(cint64 nextEntryID);

						cint64 getMaxStoredIndvidualiId();
						CBackendRepresentativeMemoryCacheOntologyData* updateMaxStoredIndvidualiId(cint64 id);


						cint64 getLastMinIncompletelyHandledIndvidualiId();
						CBackendRepresentativeMemoryCacheOntologyData* setLastMinIncompletelyHandledIndvidualiId(cint64 id);
						CBackendRepresentativeMemoryCacheOntologyData* incLastMinIncompletelyHandledIndvidualiId(cint64 count = 1);

						cint64 getIncompletelyHandledIndividualIdCount();
						CBackendRepresentativeMemoryCacheOntologyData* setIncompletelyHandledIndividualIdCount(cint64 count);
						CBackendRepresentativeMemoryCacheOntologyData* incIncompletelyHandledIndividualIdCount(cint64 count = 1);
						CBackendRepresentativeMemoryCacheOntologyData* decIncompletelyHandledIndividualIdCount(cint64 count = 1);


						cint64 getIndividualAssociationsCount();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualAssociationsCount(cint64 count);
						CBackendRepresentativeMemoryCacheOntologyData* incIndividualAssociationsCount(cint64 count = 1);


						bool isAssociationCompleted();
						CBackendRepresentativeMemoryCacheOntologyData* setAssociationCompleted(bool completed);


						bool isFirstIncompletelyHandledIndividualsRetrieved();
						CBackendRepresentativeMemoryCacheOntologyData* setFirstIncompletelyHandledIndividualsRetrieved(bool retrieved);


						cint64 getMaxIndividualAssociationDataUpdateCount();
						CBackendRepresentativeMemoryCacheOntologyData* setMaxIndividualAssociationDataUpdateCount(cint64 count);

						CCACHINGSET<cint64>* getProblematicIncompletelyHandledIndividualSet();
						CBackendRepresentativeMemoryCacheOntologyData* setProblematicIncompletelyHandledIndividualSet(CCACHINGSET<cint64>* indiSet);


						CBackendRepresentativeMemoryCacheOntologyContext* getTemporaryContext();
						CBackendRepresentativeMemoryCacheOntologyContext* getOntologyContext();
						CBackendRepresentativeMemoryCacheOntologyData* setOntologyContext(CBackendRepresentativeMemoryCacheOntologyContext* ontContext);


						CBackendRepresentativeMemoryLabelCacheItem* getPrioritizedPropagationMarkedNeighbourLabelItem();
						CBackendRepresentativeMemoryCacheOntologyData* setPrioritizedPropagationMarkedNeighbourLabelItem(CBackendRepresentativeMemoryLabelCacheItem* labelItem);


						bool isIndividualLabelAssociationIndexed();
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualLabelAssociationIndexed(bool indexed);
						cint64 updateIndividualLabelAssociationIndexed(bool releaseWaiting, CMemoryPool* memoryPools);
						CBackendRepresentativeMemoryCacheOntologyData* setIndividualLabelAssociationIndexingCount(cint64 count);

						CBackendRepresentativeMemoryCacheOntologyData* waitIndividualLabelAssociationIndexed();


						cint64 getNextSlotUpdateWaitingCount();
						CBackendRepresentativeMemoryCacheOntologyData* setNextSlotUpdateWaitingCount(cint64 updateCount);
						bool isSlotUpdateIntegrated();
						CBackendRepresentativeMemoryCacheOntologyData* setSlotUpdateIntegrated(bool integrated);

						bool isBasicPrecomputationMode();
						CBackendRepresentativeMemoryCacheOntologyData* setBasicPrecomputationMode(bool basicPrecomputation);
						CBackendRepresentativeMemoryCacheIndividualAssociationData** getBasicPrecomputationIndividualIdAssoiationDataVector();
						CBackendRepresentativeMemoryCacheOntologyData* setBasicPrecomputationIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector);
						cint64 getBasicPrecomputationIndividualIdAssoiationDataVectorSize();


						cint64 getBasicPrecompuationRetrievalIndividualIdPosition();
						CBackendRepresentativeMemoryCacheOntologyData* setBasicPrecompuationRetrievalIndividualIdPosition(cint64 indiPos);


						bool hasBasicPrecomputationModeActivation();
						CBackendRepresentativeMemoryCacheOntologyData* setBasicPrecomputationModeActivation(bool basicPrecomputationActivation);



						cint64 getIndividualAssociationDataUpdateCount();
						CBackendRepresentativeMemoryCacheOntologyData* incIndividualAssociationDataUpdateCount(cint64 count = 1);

						cint64 getIndividualAssociationDataDirectUpdateCount();
						CBackendRepresentativeMemoryCacheOntologyData* incIndividualAssociationDataDirectUpdateCount(cint64 count = 1);


						cint64 getIndividualAssociationMergingCount();
						CBackendRepresentativeMemoryCacheOntologyData* incIndividualAssociationMergingCount(cint64 count = 1);

						cint64 getInvolvedIndividualCount();
						bool hasInvolvedIndividuals();
						CBackendRepresentativeMemoryCacheOntologyData* incInvolvedIndividualCount(cint64 count = 1);



						cint64 getIncompletelyHandledIndividualsRetrievalCount();
						CBackendRepresentativeMemoryCacheOntologyData* incIncompletelyHandledIndividualsRetrievalCount(cint64 count = 1);

						cint64 getCacheDataUpdateWritingCount();
						CBackendRepresentativeMemoryCacheOntologyData* incCacheDataUpdateWritingCount(cint64 count = 1);



						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* getLastActiveRecomputationReferenceLinker();

						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* getRecomputationReferenceLinker();
						CBackendRepresentativeMemoryCacheOntologyData* setRecomputationReferenceLinker(CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* linker);


						cint64 getOntologyDataUpdateId();
						cint64 getMinimumValidRecomputationId();
						cint64 getNextUpdateMinimumValidRecomputationId();

						CBackendRepresentativeMemoryCacheOntologyData* setMinimumValidRecomputationId(cint64 recomputationId);
						CBackendRepresentativeMemoryCacheOntologyData* setNextUpdateMinimumValidRecomputationId(cint64 recomputationId);

						CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* getRecomputationIdReleasingIndividualAssociationContextMap();
						CBackendRepresentativeMemoryCacheOntologyData* setRecomputationIdReleasingIndividualAssociationContextMap(CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* map);
						CBackendRepresentativeMemoryCacheIndividualAssociationContext* getReleaseQueuedIndividualAssociationContextLinker();
						CBackendRepresentativeMemoryCacheOntologyData* setReleaseQueuedIndividualAssociationContextLinker(CBackendRepresentativeMemoryCacheIndividualAssociationContext* linker);
						CBackendRepresentativeMemoryCacheOntologyData* addReleaseQueuedIndividualAssociationContextLinker(CBackendRepresentativeMemoryCacheIndividualAssociationContext* linker);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mOntologyIdentifer;
						cint64 mUsageCount;


						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* mSigLabelItemHash[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						cint64 mNextEntryID;

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* mIndiIdAssoDataHash;
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* mNominalIndiIdIndirectConnectionDataHash;


						cint64 mMaxStoredIndvidualiId;

						cint64 mIndiIdAssoDataVectorSize;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** mIndiIdAssoDataVector;

						cint64 mLastMinIncompletelyHandledIndiId;
						cint64 mIncompletelyHandledIndiIdCount;
						cint64 mIndividualAssociationsCount;
						bool mAssociationCompleted;
						bool mFirstIncompletelyHandledIndividualsRetrieved;

						cint64 mMaxIndiAssocDataUpdateCount;

						bool mSameMergedIndisInCache;


						CBackendRepresentativeMemoryCacheOntologyContext mTemporaryContext;
						CBackendRepresentativeMemoryCacheOntologyContext* mOntologyContext;

						CCACHINGSET<cint64>* mProblematicIncompletelyHandledIndiSet;

						CBackendRepresentativeMemoryLabelCacheItem* mPrioritizedPropagationMarkedNeighbourLabelItem;


						bool mIndividualLabelAssociationIndexed;
						QMutex mIndividualLabelAssociationIndexedWaitingMutex;
						cint64 mIndividualLabelAssociationIndexedWaitingCount;
						QAtomicInt mIndividualLabelAssociationIndexingCount;
						QSemaphore mIndividualLabelAssociationIndexedWaitingSemaphore;

						cint64 mNextSlotUpdateWaitingCount;
						bool mSlotUpdateIntegrated;

						bool mBasicPrecomputationMode;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** mBasicPrecompuationIndiIdAssoDataVector;
						cint64 mBasicPrecompuationIndiIdAssoDataVectorSize;
						cint64 mBasicPrecompuationRetrievalIndiIdPos;
						bool mBasicPrecomputationModeActivation;


						cint64 mInvolvedIndividualCount;

						cint64 mIndividualAssociationDataUpdateCount;
						cint64 mIndividualAssociationDataDirectUpdateCount;
						cint64 mIndividualAssociationMergingCount;

						cint64 mIncompletelyHandledIndividualsRetrievalCount;
						cint64 mCacheDataUpdateWritingCount;


						cint64 mMinimumValidRecomputationId;
						cint64 mNextUpdateMinimumValidRecomputationId;

						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* mRecomputationReferenceLinker;
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* mLastActiveRecomputationReferenceLinker;
						cint64 mOntologyDataUpdateId;
						CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* mRecomputationIdReleasingIndividualAssociationMap;
						CBackendRepresentativeMemoryCacheIndividualAssociationContext* mReleaseQueuedIndividualAssociationContextLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHEONTOLOGYDATA_H
