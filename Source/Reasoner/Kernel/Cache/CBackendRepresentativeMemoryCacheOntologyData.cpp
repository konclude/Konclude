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

#include "CBackendRepresentativeMemoryCacheOntologyData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheOntologyData::CBackendRepresentativeMemoryCacheOntologyData(CBackendRepresentativeMemoryCacheBaseContext* baseContext) : mTemporaryContext(baseContext) {
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::initOntologyData(cint64 ontologyIdentifer, bool directIndexing) {
					mOntologyIdentifer = ontologyIdentifer;

					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
						mSigLabelItemHash[i] = nullptr;
					}
					mNominalIndiIdIndirectConnectionDataHash = nullptr;
					mIndiIdAssoDataHash = nullptr;
					mIndiIdAssoDataVector = nullptr;
					mIndiIdAssoDataVectorSize = 0;
					mSameMergedIndisInCache = false;
					mAssociationCompleted = false;
					mFirstIncompletelyHandledIndividualsRetrieved = false;
					mLastMinIncompletelyHandledIndiId = CINT64_MAX;
					mMaxIndiAssocDataUpdateCount = 0;
					mIncompletelyHandledIndiIdCount = 0;
					mIndividualAssociationsCount = 0;
					mUsageCount = 0;
					mOntologyContext = nullptr;
					mNextEntryID = 1;
					mProblematicIncompletelyHandledIndiSet = nullptr;
					mPrioritizedPropagationMarkedNeighbourLabelItem = nullptr;

					mIndividualLabelAssociationIndexed = directIndexing;
					mIndividualLabelAssociationIndexedWaitingCount = 0;
					mIndividualLabelAssociationIndexingCount = 0;
					mMaxStoredIndvidualiId = 0;
					mNextSlotUpdateWaitingCount = 0;
					mSlotUpdateIntegrated = false;
					mBasicPrecomputationMode = false;
					mBasicPrecompuationIndiIdAssoDataVector = nullptr;
					mBasicPrecompuationIndiIdAssoDataVectorSize = 0;
					mBasicPrecompuationRetrievalIndiIdPos = 0;
					mBasicPrecomputationModeActivation = false;
					mIndividualAssociationDataUpdateCount = 0;
					mIndividualAssociationDataDirectUpdateCount = 0;
					mIndividualAssociationMergingCount = 0;
					mInvolvedIndividualCount = 0;
					mIncompletelyHandledIndividualsRetrievalCount = 0;
					mCacheDataUpdateWritingCount = 0;
					mRecomputationReferenceLinker = nullptr;
					mLastActiveRecomputationReferenceLinker = nullptr;

					mMinimumValidRecomputationId = 0;
					mNextUpdateMinimumValidRecomputationId = 0;
					mOntologyDataUpdateId = 1;
					mReleaseQueuedIndividualAssociationContextLinker = nullptr;
					mRecomputationIdReleasingIndividualAssociationMap = nullptr;
					return this;
				}



				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::copyOntologyData(CBackendRepresentativeMemoryCacheOntologyData* data) {
					mSameMergedIndisInCache = data->mSameMergedIndisInCache;
					mMaxIndiAssocDataUpdateCount = data->mMaxIndiAssocDataUpdateCount;
					mAssociationCompleted = data->mAssociationCompleted;
					mIncompletelyHandledIndiIdCount = data->mIncompletelyHandledIndiIdCount;
					mLastMinIncompletelyHandledIndiId = data->mLastMinIncompletelyHandledIndiId;
					mNextEntryID = data->mNextEntryID;
					mMaxStoredIndvidualiId = data->mMaxStoredIndvidualiId;
					mIndividualLabelAssociationIndexed = data->mIndividualLabelAssociationIndexed;
					mIndividualAssociationsCount = data->mIndividualAssociationsCount;
					mFirstIncompletelyHandledIndividualsRetrieved = data->mFirstIncompletelyHandledIndividualsRetrieved;
					mNextSlotUpdateWaitingCount = data->mNextSlotUpdateWaitingCount;

					mBasicPrecomputationMode = data->mBasicPrecomputationMode;
					mBasicPrecompuationIndiIdAssoDataVector = data->mBasicPrecompuationIndiIdAssoDataVector;
					mBasicPrecompuationIndiIdAssoDataVectorSize = data->mBasicPrecompuationIndiIdAssoDataVectorSize;
					mBasicPrecompuationRetrievalIndiIdPos = data->mBasicPrecompuationRetrievalIndiIdPos;
					mBasicPrecomputationModeActivation = data->mBasicPrecomputationModeActivation;
					
					mIndividualAssociationDataDirectUpdateCount = data->mIndividualAssociationDataDirectUpdateCount;
					mIndividualAssociationDataUpdateCount = data->mIndividualAssociationDataUpdateCount;
					mIndividualAssociationMergingCount = data->mIndividualAssociationMergingCount;
					mInvolvedIndividualCount = data->mInvolvedIndividualCount;

					mIncompletelyHandledIndividualsRetrievalCount = data->mIncompletelyHandledIndividualsRetrievalCount;
					mCacheDataUpdateWritingCount = data->mCacheDataUpdateWritingCount;
					mRecomputationReferenceLinker = data->mRecomputationReferenceLinker;
					mLastActiveRecomputationReferenceLinker = nullptr;

					mMinimumValidRecomputationId = data->mMinimumValidRecomputationId;
					mNextUpdateMinimumValidRecomputationId = data->mNextUpdateMinimumValidRecomputationId;
					mOntologyDataUpdateId = data->mOntologyDataUpdateId + 1;
					mReleaseQueuedIndividualAssociationContextLinker = data->mReleaseQueuedIndividualAssociationContextLinker;
					mRecomputationIdReleasingIndividualAssociationMap = data->mRecomputationIdReleasingIndividualAssociationMap;
					mPrioritizedPropagationMarkedNeighbourLabelItem = data->mPrioritizedPropagationMarkedNeighbourLabelItem;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheOntologyData::isIndividualLabelAssociationIndexed() {
					return mIndividualLabelAssociationIndexed;
				}



				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualLabelAssociationIndexingCount(cint64 count) {
					mIndividualLabelAssociationIndexingCount = count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualLabelAssociationIndexed(bool indexed) {
					mIndividualLabelAssociationIndexed = indexed;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::updateIndividualLabelAssociationIndexed(bool releaseWaiting, CMemoryPool* memoryPools) {
					cint64 remainingCount = 0;
					mIndividualLabelAssociationIndexedWaitingMutex.lock();
					mOntologyContext->getMemoryPoolContainer()->appendMemoryPool(memoryPools);
					mIndividualLabelAssociationIndexingCount.deref();
					remainingCount = mIndividualLabelAssociationIndexingCount;
					if (mIndividualLabelAssociationIndexingCount <= 0) {
						mIndividualLabelAssociationIndexed = true;
						if (releaseWaiting) {
							mIndividualLabelAssociationIndexedWaitingSemaphore.release(mIndividualLabelAssociationIndexedWaitingCount);
							mIndividualLabelAssociationIndexedWaitingCount = 0;
						}
					}
					mIndividualLabelAssociationIndexedWaitingMutex.unlock();
					return remainingCount;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::waitIndividualLabelAssociationIndexed() {
					if (!mIndividualLabelAssociationIndexed) {
						mIndividualLabelAssociationIndexedWaitingMutex.lock();
						if (!mIndividualLabelAssociationIndexed) {
							++mIndividualLabelAssociationIndexedWaitingCount;
							mIndividualLabelAssociationIndexedWaitingMutex.unlock();
							mIndividualLabelAssociationIndexedWaitingSemaphore.acquire();
						} else {
							mIndividualLabelAssociationIndexedWaitingMutex.unlock();
						}

					}
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getOntologyIdentifer() {
					return mOntologyIdentifer;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getUsageCount() {
					return mUsageCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incUsageCount(cint64 count) {
					mUsageCount += count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::decUsageCount(cint64 count) {
					mUsageCount -= count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setSignatureLabelItemHash(cint64 labelType, CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigItemHash) {
					mSigLabelItemHash[labelType] = sigItemHash;
					return this;
				}



				bool CBackendRepresentativeMemoryCacheOntologyData::hasSameIndividualsMergings() {
					return mSameMergedIndisInCache;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setSameIndividualsMergings(bool sameIndisMergings) {
					mSameMergedIndisInCache = sameIndisMergings;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* CBackendRepresentativeMemoryCacheOntologyData::getSignatureLabelItemHash(cint64 labelType) {
					return mSigLabelItemHash[labelType];
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssociationDataHash() {
					return mIndiIdAssoDataHash;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssociationDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* indiIdAssoDataHash) {
					mIndiIdAssoDataHash = indiIdAssoDataHash;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssoiationDataVectorSize() {
					return mIndiIdAssoDataVectorSize;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssoiationDataVectorSize(cint64 size) {
					mIndiIdAssoDataVectorSize = size;
					return this;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData** CBackendRepresentativeMemoryCacheOntologyData::getIndividualIdAssoiationDataVector() {
					return mIndiIdAssoDataVector;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector) {
					mIndiIdAssoDataVector = indiIdAssoDataVector;
					mIndiIdAssoDataVectorSize = indiIdAssoDataVectorSize;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNominaIIndividualdIndirectConnectionDataHash(CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* hash) {
					mNominalIndiIdIndirectConnectionDataHash = hash;
					return this;
				}


				CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* CBackendRepresentativeMemoryCacheOntologyData::getNominaIIndividualdIndirectConnectionDataHash() {
					return mNominalIndiIdIndirectConnectionDataHash;
				}




				cint64 CBackendRepresentativeMemoryCacheOntologyData::getMaxStoredIndvidualiId() {
					return mMaxStoredIndvidualiId;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::updateMaxStoredIndvidualiId(cint64 id) {
					mMaxStoredIndvidualiId = qMax(id, mMaxStoredIndvidualiId);
					return this;
				}




				cint64 CBackendRepresentativeMemoryCacheOntologyData::getNextEntryID(bool moveNext) {
					cint64 nextEntryId = mNextEntryID;
					if (moveNext) {
						mNextEntryID++;
					}
					return nextEntryId;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNextEntryID(cint64 nextEntryID) {
					mNextEntryID = nextEntryID;
					return this;
				}



				cint64 CBackendRepresentativeMemoryCacheOntologyData::getLastMinIncompletelyHandledIndvidualiId() {
					return mLastMinIncompletelyHandledIndiId;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setLastMinIncompletelyHandledIndvidualiId(cint64 id) {
					mLastMinIncompletelyHandledIndiId = id;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incLastMinIncompletelyHandledIndvidualiId(cint64 count) {
					mLastMinIncompletelyHandledIndiId += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualAssociationsCount() {
					return mIndividualAssociationsCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIndividualAssociationsCount(cint64 count) {
					mIndividualAssociationsCount = count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIndividualAssociationsCount(cint64 count) {
					mIndividualAssociationsCount += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIncompletelyHandledIndividualIdCount() {
					return mIncompletelyHandledIndiIdCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount = count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount += count;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::decIncompletelyHandledIndividualIdCount(cint64 count) {
					mIncompletelyHandledIndiIdCount -= count;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheOntologyData::isAssociationCompleted() {
					return mAssociationCompleted;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setAssociationCompleted(bool completed) {
					mAssociationCompleted = completed;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheOntologyData::isFirstIncompletelyHandledIndividualsRetrieved() {
					return mFirstIncompletelyHandledIndividualsRetrieved;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setFirstIncompletelyHandledIndividualsRetrieved(bool retrieved) {
					mFirstIncompletelyHandledIndividualsRetrieved = retrieved;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getMaxIndividualAssociationDataUpdateCount() {
					return mMaxIndiAssocDataUpdateCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setMaxIndividualAssociationDataUpdateCount(cint64 count) {
					mMaxIndiAssocDataUpdateCount = count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyContext* CBackendRepresentativeMemoryCacheOntologyData::getTemporaryContext() {
					return &mTemporaryContext;
				}

				CBackendRepresentativeMemoryCacheOntologyContext* CBackendRepresentativeMemoryCacheOntologyData::getOntologyContext() {
					return mOntologyContext;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setOntologyContext(CBackendRepresentativeMemoryCacheOntologyContext* ontContext) {
					mOntologyContext = ontContext;
					return this;
				}

				CCACHINGSET<cint64>* CBackendRepresentativeMemoryCacheOntologyData::getProblematicIncompletelyHandledIndividualSet() {
					return mProblematicIncompletelyHandledIndiSet;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setProblematicIncompletelyHandledIndividualSet(CCACHINGSET<cint64>* indiSet) {
					mProblematicIncompletelyHandledIndiSet = indiSet;
					return this;
				}


				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheOntologyData::getPrioritizedPropagationMarkedNeighbourLabelItem() {
					return mPrioritizedPropagationMarkedNeighbourLabelItem;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setPrioritizedPropagationMarkedNeighbourLabelItem(CBackendRepresentativeMemoryLabelCacheItem* labelItem) {
					mPrioritizedPropagationMarkedNeighbourLabelItem = labelItem;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getNextSlotUpdateWaitingCount() {
					return mNextSlotUpdateWaitingCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNextSlotUpdateWaitingCount(cint64 updateCount) {
					mNextSlotUpdateWaitingCount = updateCount;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheOntologyData::isSlotUpdateIntegrated() {
					return mSlotUpdateIntegrated;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setSlotUpdateIntegrated(bool integrated) {
					mSlotUpdateIntegrated = integrated;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheOntologyData::isBasicPrecomputationMode() {
					return mBasicPrecomputationMode;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setBasicPrecomputationMode(bool basicPrecomputation) {
					mBasicPrecomputationMode = basicPrecomputation;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getBasicPrecomputationIndividualIdAssoiationDataVectorSize() {
					return mBasicPrecompuationIndiIdAssoDataVectorSize;
				}


				CBackendRepresentativeMemoryCacheIndividualAssociationData** CBackendRepresentativeMemoryCacheOntologyData::getBasicPrecomputationIndividualIdAssoiationDataVector() {
					return mBasicPrecompuationIndiIdAssoDataVector;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setBasicPrecomputationIndividualIdAssoiationDataVector(cint64 indiIdAssoDataVectorSize, CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector) {
					mBasicPrecompuationIndiIdAssoDataVector = indiIdAssoDataVector;
					mBasicPrecompuationIndiIdAssoDataVectorSize = indiIdAssoDataVectorSize;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getBasicPrecompuationRetrievalIndividualIdPosition() {
					return mBasicPrecompuationRetrievalIndiIdPos;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setBasicPrecompuationRetrievalIndividualIdPosition(cint64 indiPos) {
					mBasicPrecompuationRetrievalIndiIdPos = indiPos;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheOntologyData::hasBasicPrecomputationModeActivation() {
					return mBasicPrecomputationModeActivation;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setBasicPrecomputationModeActivation(bool basicPrecomputationActivation) {
					mBasicPrecomputationModeActivation = basicPrecomputationActivation;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualAssociationDataUpdateCount() {
					return mIndividualAssociationDataUpdateCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIndividualAssociationDataUpdateCount(cint64 count) {
					mIndividualAssociationDataUpdateCount += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualAssociationDataDirectUpdateCount() {
					return mIndividualAssociationDataDirectUpdateCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIndividualAssociationDataDirectUpdateCount(cint64 count) {
					mIndividualAssociationDataDirectUpdateCount += count;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIndividualAssociationMergingCount() {
					return mIndividualAssociationMergingCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIndividualAssociationMergingCount(cint64 count) {
					mIndividualAssociationMergingCount += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getInvolvedIndividualCount() {
					return mInvolvedIndividualCount;
				}

				bool CBackendRepresentativeMemoryCacheOntologyData::hasInvolvedIndividuals() {
					return mInvolvedIndividualCount > 0;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incInvolvedIndividualCount(cint64 count) {
					mInvolvedIndividualCount += count;
					return this;
				}


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getIncompletelyHandledIndividualsRetrievalCount() {
					return mIncompletelyHandledIndividualsRetrievalCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incIncompletelyHandledIndividualsRetrievalCount(cint64 count) {
					mIncompletelyHandledIndividualsRetrievalCount += count;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getCacheDataUpdateWritingCount() {
					return mCacheDataUpdateWritingCount;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::incCacheDataUpdateWritingCount(cint64 count) {
					mCacheDataUpdateWritingCount += count;
					return this;
				}


				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheOntologyData::getLastActiveRecomputationReferenceLinker() {
					if (!mLastActiveRecomputationReferenceLinker) {
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* linkerIt = mRecomputationReferenceLinker;
						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* lastLinker = linkerIt;
						while (linkerIt) {
							if (linkerIt->isOntologyDataActive()) {
								mLastActiveRecomputationReferenceLinker = linkerIt;
							}
							if (linkerIt->isNextOntologyDataAllInactive()) {
								break;
							}
							lastLinker = linkerIt;
							linkerIt = linkerIt->getNext();
						}
						if (!mLastActiveRecomputationReferenceLinker) {
							mLastActiveRecomputationReferenceLinker = lastLinker;
						}

						linkerIt = mLastActiveRecomputationReferenceLinker->getNext();
						while (linkerIt && !linkerIt->isNextOntologyDataAllInactive()) {
							linkerIt->setNextOntologyDataAllInactive();
							linkerIt = linkerIt->getNext();
						}
					}
					return mLastActiveRecomputationReferenceLinker;
				}


				CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* CBackendRepresentativeMemoryCacheOntologyData::getRecomputationReferenceLinker() {
					return mRecomputationReferenceLinker;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setRecomputationReferenceLinker(CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* linker) {
					mRecomputationReferenceLinker = linker->append(mRecomputationReferenceLinker);
					return this;
				}
				


				cint64 CBackendRepresentativeMemoryCacheOntologyData::getMinimumValidRecomputationId() {
					return mMinimumValidRecomputationId;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getNextUpdateMinimumValidRecomputationId() {
					return mNextUpdateMinimumValidRecomputationId;
				}


				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setMinimumValidRecomputationId(cint64 recomputationId) {
					mMinimumValidRecomputationId = recomputationId;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setNextUpdateMinimumValidRecomputationId(cint64 recomputationId) {
					mNextUpdateMinimumValidRecomputationId = recomputationId;
					return this;
				}

				cint64 CBackendRepresentativeMemoryCacheOntologyData::getOntologyDataUpdateId() {
					return mOntologyDataUpdateId;
				}


				CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* CBackendRepresentativeMemoryCacheOntologyData::getRecomputationIdReleasingIndividualAssociationContextMap() {
					return mRecomputationIdReleasingIndividualAssociationMap;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setRecomputationIdReleasingIndividualAssociationContextMap(CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* map) {
					mRecomputationIdReleasingIndividualAssociationMap = map;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationContext* CBackendRepresentativeMemoryCacheOntologyData::getReleaseQueuedIndividualAssociationContextLinker() {
					return mReleaseQueuedIndividualAssociationContextLinker;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::setReleaseQueuedIndividualAssociationContextLinker(CBackendRepresentativeMemoryCacheIndividualAssociationContext* linker) {
					mReleaseQueuedIndividualAssociationContextLinker = linker;
					return this;
				}

				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCacheOntologyData::addReleaseQueuedIndividualAssociationContextLinker(CBackendRepresentativeMemoryCacheIndividualAssociationContext* linker) {
					mReleaseQueuedIndividualAssociationContextLinker = linker->append(mReleaseQueuedIndividualAssociationContextLinker);
					return this;
				}



			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
