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

#include "CBackendRepresentativeMemoryCache.h"
#include "CBackendRepresentativeMemoryCacheWriter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCache::CBackendRepresentativeMemoryCache(CConfiguration* config, QString threadIdentifierName, CWatchDog *watchDogThread) : CThread(threadIdentifierName, watchDogThread) {

					mConfig = config;

					mSlotLinker = nullptr;
					mLastUpdatedSlotLinker = nullptr;
					mReaderLinker = nullptr;

					mNextIndiUpdateId = 1;
					mNextNomConnUpdateId = 1;

					mReaderSlotUpdateCount = 0;
					mOntologyDataUpdateCount = 0;
					mOntologyDataReleasedCount = 0;
					mOntologyDataReleasedWhileNewCreationCount = 0;
					mOntologyDataReleasedWhileSlotUpdateCount = 0;
					mReaderSlotReleasedCount = 0;
					mCheckingRemainingIncompletelyHandledCount = 0;
					mEmptyWriteDataCount = 0;
					mWriteDataCount = 0;
					mStartWriteCollectCount = 0;
					mNextWriteCollectCount = 0;
					mTmpPropCutIndiArrayNeighboursHandlingDataHash = nullptr;

					mConfInstallingDeterministicSameHandlingLargeDifference = 50;
					mConfInstallingDeterministicSameHandlingLargeDifferenceReached = false;

					mConfWaitIndividualLabelAssociationIndexed = CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.WaitIndividualLabelAssociationIndexed", true);
					mConfLateIndividualLabelAssociationIndexing = CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.LateIndividualLabelAssociationIndexing", true);
					mConfDebugWriteRepresentativeCache = CConfigDataReader::readConfigBoolean(config, "Konclude.Debugging.WriteDebuggingRepresentativeCacheContent", false);
					mStatCollectStatistics = CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.LogStorageStatistics", false);
					mConfDirectUpdateSynchronization = !CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.ThreadedUpdateSynchronization", true);


					cint64 limitCount = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.PendingWriteDataLimit", -1);
					if (limitCount >= 0) {
						mLimitRemainingWritePending = true;
						mRemainingWritePendingSemaphore.release(limitCount);
					} else {
						mLimitRemainingWritePending = false;
					}



					mConfIncrementUpdateIdForDeterministicSameAsCompletion = CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.IncrementUpdateIdForDeterministicSameAsCompletion", true);
					mConfMinRequiredDeterministicSameMergedHandledInstallationPossiblitiesForNeighbourCompletion = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.MinRequiredDeterministicSameMergedHandledInstallationPossiblitiesForNeighbourCompletion", 1);
					mConfUnchangedDeterministicSameMergeUpdatesForDeterministicSameNeighbourCompletion = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.UnchangedDeterministicSameMergeUpdatesForDeterministicSameNeighbourCompletion", 1);

					mConfInstallingDeterministicSameHandlingLargeDifference = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.TryingDeterministicSameMergedHandlingInstallationDifferenceCount", -1);
					if (mConfInstallingDeterministicSameHandlingLargeDifference != -1) {
						mConfInstallingDeterministicSameHandlingLargeDifferenceReached = true;
					}


					mConfBasicPrecomputationModeActivationUpdateMergesRatio = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.BasicPrecomputationModeActivationUpdateMergesPercentRatio", 5) / 100.;


					mConfMaxIncompletelyHandledIndividualsRetrievalCount = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.MaxIncompletelyHandledIndividualsRetrievalCount", -1);
					mConfMaxCacheDataUpdateWritingCount = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.MaxCacheDataUpdateWritingCount", -1);


					mConfUpdateRejectingIncompatibleIndividualAssociationsRatio = CConfigDataReader::readConfigDouble(config, "Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatibleIndividualAssociationsRatio", 1.0);
					mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourRatio = CConfigDataReader::readConfigDouble(config, "Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatiblePropagationCuttedIndividualLinkedNeighbourRatio", 1.0);
					mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourCount = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.RejectingUpdateIncompatiblePropagationCuttedIndividualLinkedNeighbourCount", -1);

					mConfInterpretUnchangedLabelsAsCompatible = CConfigDataReader::readConfigBoolean(config, "Konclude.Cache.RepresentativeBackendCache.InterpretUnchangedLabelsAsCompatibleIndividualAssociationUpdates", false);


					mSlotUpdateWaitingMaxCount = CConfigDataReader::readConfigInteger(config, "Konclude.Cache.RepresentativeBackendCache.SlotUpdateWaitingIncreaseMaximumCount", 20);

					mStatAddingNeighbourLinksAssociationUpdateCount = 0;
					mStatUpdatedOrRemovedNeighbourLinksAssociationUpdateCount = 0;
					mStatMaxSameAsMergedCount = 0;
					mStatMaxLabelValueCount = 0;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
						mStatLabelTypeCount[i] = 0;
						mStatLabelTypeMaxValueCount[i] = 0;
						mStatLabelTypeAllValueCount[i] = 0;
					}
					mStatLabelCount = 0;
					mStatMaxAssociationUpdateCount = 0;
					mStatCreatedNeighbourLinks = 0;
					mStatMaxNeighbourLinksCount = 0;
					mStatIncompatibleLabelNeighbourCompletionCount = 0;
					mStatChangedLabelNeighbourCompletionCount = 0;
					mStatNeighbourCompletionDetSameSuccededCount = 0;
					mStatNeighbourCompletionDetSameChangedCount = 0;
					mStatNeighbourCompletionDetSameIncompatibleCount = 0;
					mStatNeighbourCompletionDetSameUnchangedCount = 0;

					mStatDetSameAssociationInstallCount = 0;
					mStatDetSameAssociationFailedCount = 0;
					mStatDetSameAssociationDifferentUpdateIdFailedCount = 0;
					mStatDetSameAssociationDifferentDestIdFailedCount = 0;
					mStatDetSameAssociationIncompleteHandledDestFailedCount = 0;
					mStatDetSameAssociationRepMergedDestFailedCount = 0;
					mStatDetSameRepresentativeMergingCount = 0;

					mSlotUpdateWaitingIncreaseCount = 1;

					CMemoryAllocationManager* memMan = mContext.getMemoryAllocationManager();
					//mIndiIdAssoDataHash = nullptr;

					mOntologyIdentifierDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>, CBackendRepresentativeMemoryCacheBaseContext* >::allocateAndConstructAndParameterize(memMan, &mContext);

					startThread(QThread::HighestPriority);
				}


				CBackendRepresentativeMemoryCache::~CBackendRepresentativeMemoryCache() {
				}



				CCacheStatistics* CBackendRepresentativeMemoryCache::getCacheStatistics() {
					return &mCacheStat;
				}



				void CBackendRepresentativeMemoryCache::deleteExpiredIndividualAssociationMemoryContexts(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, CBackendRepresentativeMemoryCacheBaseContext* context) {
					CBackendRepresentativeMemoryCacheContext* ontologyContext = ontologyData->getOntologyContext();
					if (ontologyData->isBasicPrecomputationMode()) {
						return;
					}

					cint64 statIndividualAssociationSeparateMemoryManagmentDeletionCount = 0;
					cint64 statIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount = 0;
					CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* recomputationIdReleasingIndividualAssociationContextMap = ontologyData->getRecomputationIdReleasingIndividualAssociationContextMap();
					bool recomputationIdReleasingIndividualAssociationContextMapLocated = false;
					CBackendRepresentativeMemoryCacheIndividualAssociationContext* releaseQueuedIndividualAssociationContextLinker = ontologyData->getReleaseQueuedIndividualAssociationContextLinker();
					if (releaseQueuedIndividualAssociationContextLinker) {
						cint64 ontologyIdentifier = ontologyData->getOntologyIdentifer();

						CBackendRepresentativeMemoryCacheIndividualAssociationContext* prevReleaseQueuedIndividualAssociationContextLinker = nullptr;
						for (CBackendRepresentativeMemoryCacheIndividualAssociationContext* releaseQueuedIndividualAssociationContextLinkerIt = releaseQueuedIndividualAssociationContextLinker; releaseQueuedIndividualAssociationContextLinkerIt; ) {
							CBackendRepresentativeMemoryCacheIndividualAssociationContext* nextReleaseQueuedIndividualAssociationContextLinker = releaseQueuedIndividualAssociationContextLinkerIt->getNext();
							CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* firstRecomputationReferenceLinker = releaseQueuedIndividualAssociationContextLinkerIt->getFirstRecomputationReferenceLinker();
							CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* firstNextRecomputationReferenceLinker = firstRecomputationReferenceLinker->getNext();
							CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* lastRecomputationReferenceLinker = releaseQueuedIndividualAssociationContextLinkerIt->getLastRecomputationReferenceLinker();

							bool noneOntologyDataUpdateIdSlotReferred = true;
							cint64 maxReferedRecomputationId = CINT64_MIN;
							cint64 minReferedRecomputationId = CINT64_MAX;
							for (CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* recomputationReferenceLinkerIt = lastRecomputationReferenceLinker; recomputationReferenceLinkerIt && noneOntologyDataUpdateIdSlotReferred && recomputationReferenceLinkerIt != firstNextRecomputationReferenceLinker; recomputationReferenceLinkerIt = recomputationReferenceLinkerIt->getNext()) {
								cint64 ontologyDataUpdateId = recomputationReferenceLinkerIt->getOntologyDataUpdateId();

								if (!recomputationReferenceLinkerIt->isOntologyDataActive()) {
									mStatIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount++;
									statIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount++;

									bool ontologyDataUpdateIdSlotReferred = false;
									for (CBackendRepresentativeMemoryCacheSlotItem* slotLinkerIt = mSlotLinker; slotLinkerIt && !ontologyDataUpdateIdSlotReferred; slotLinkerIt = slotLinkerIt->getNext()) {
										CBackendRepresentativeMemoryCacheOntologyData* referredOntologyData = slotLinkerIt->getOntologyData(ontologyIdentifier);
										cint64 ontologyUpdateId = referredOntologyData->getOntologyDataUpdateId();
										if (ontologyUpdateId == ontologyDataUpdateId) {
											ontologyDataUpdateIdSlotReferred = true;
										}
									}

									if (ontologyDataUpdateIdSlotReferred) {
										noneOntologyDataUpdateIdSlotReferred = false;
									}
									maxReferedRecomputationId = qMax(recomputationReferenceLinkerIt->getMaxUsedRecomputationId(), maxReferedRecomputationId);
									minReferedRecomputationId = qMin(recomputationReferenceLinkerIt->getMinUsedRecomputationId(), minReferedRecomputationId);
								} else {
									noneOntologyDataUpdateIdSlotReferred = false;
								}
							}

							if (noneOntologyDataUpdateIdSlotReferred) {
								--mStatMemoryManagmentQueuedCheckingCount;
								++mStatMemoryManagmentScheduledReleasingCount;
								mStatIndividualAssociationSeparateMemoryManagmentUnreferredSlotCount++;
								// move to recomputation id releasing map
								if (!recomputationIdReleasingIndividualAssociationContextMap || !recomputationIdReleasingIndividualAssociationContextMapLocated) {
									CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* tmpMap = CObjectParameterizingAllocator< CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*> , CContext* >::allocateAndConstructAndParameterize(ontologyContext->getMemoryAllocationManager(), ontologyContext);
									if (recomputationIdReleasingIndividualAssociationContextMap) {
										*tmpMap = *recomputationIdReleasingIndividualAssociationContextMap;
									}
									recomputationIdReleasingIndividualAssociationContextMap = tmpMap;
									recomputationIdReleasingIndividualAssociationContextMapLocated = true;
									ontologyData->setRecomputationIdReleasingIndividualAssociationContextMap(recomputationIdReleasingIndividualAssociationContextMap);
								}

								if (!prevReleaseQueuedIndividualAssociationContextLinker) {
									ontologyData->setReleaseQueuedIndividualAssociationContextLinker(nextReleaseQueuedIndividualAssociationContextLinker);
								} else {
									prevReleaseQueuedIndividualAssociationContextLinker->setNext(nextReleaseQueuedIndividualAssociationContextLinker);
								}
								releaseQueuedIndividualAssociationContextLinkerIt->setNext(nullptr);

								CBackendRepresentativeMemoryCacheIndividualAssociationContext* prevReleasingIndividualAssociationContextLinker = recomputationIdReleasingIndividualAssociationContextMap->value(maxReferedRecomputationId);
								releaseQueuedIndividualAssociationContextLinkerIt = releaseQueuedIndividualAssociationContextLinkerIt->append(prevReleasingIndividualAssociationContextLinker);

								recomputationIdReleasingIndividualAssociationContextMap->insert(maxReferedRecomputationId, releaseQueuedIndividualAssociationContextLinkerIt);
							} else {
								prevReleaseQueuedIndividualAssociationContextLinker = releaseQueuedIndividualAssociationContextLinkerIt;
							}
							releaseQueuedIndividualAssociationContextLinkerIt = nextReleaseQueuedIndividualAssociationContextLinker;
						}
					}


					if (recomputationIdReleasingIndividualAssociationContextMap && !recomputationIdReleasingIndividualAssociationContextMap->isEmpty()) {
						cint64 minValidRecompId = getMinimumSlotReferreringInstalledValidRecomputationId(ontologyData);
						mLastMemoryContextDeletionMinValidRecompId = minValidRecompId;

						cint64 statDeletedMemoryPoolsCount = 0;
						cint64 statDeletedMemoryPoolSize = 0;

						CMemoryPoolAllocationManager* memMan = context->getMemoryPoolAllocationManager();
						while (!recomputationIdReleasingIndividualAssociationContextMap->isEmpty() && recomputationIdReleasingIndividualAssociationContextMap->constBegin().key() < minValidRecompId) {
							CBackendRepresentativeMemoryCacheIndividualAssociationContext* unusedIndiAssContextLinker = recomputationIdReleasingIndividualAssociationContextMap->begin().value();
							for (CBackendRepresentativeMemoryCacheIndividualAssociationContext* unusedIndiAssContextLinkerIt = unusedIndiAssContextLinker; unusedIndiAssContextLinkerIt; ) {
								CBackendRepresentativeMemoryCacheIndividualAssociationContext* nextUnusedIndiAssContextLinker = unusedIndiAssContextLinkerIt->getNext();
								CMemoryPoolContainer* memPoolContainer = unusedIndiAssContextLinkerIt->getMemoryPoolContainer();
								CMemoryPool* memoryPools = memPoolContainer->getMemoryPools();
								statDeletedMemoryPoolsCount += memPoolContainer->getMemoryPoolCount();
								statDeletedMemoryPoolSize += memPoolContainer->getMemorySize();

								if (mStatCollectStatistics) {
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
									mIndiContextCountHash[unusedIndiAssContextLinkerIt->mIndiId]--;
#endif
								}
								--mStatMemoryManagmentScheduledReleasingCount;

//#ifndef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
								memMan->releaseTemporaryMemoryPools(memoryPools);
//#endif

								mStatIndividualAssociationSeparateMemoryManagmentDeletionCount++;
								statIndividualAssociationSeparateMemoryManagmentDeletionCount++;
								unusedIndiAssContextLinkerIt = nextUnusedIndiAssContextLinker;
							}

							if (!recomputationIdReleasingIndividualAssociationContextMapLocated) {
								CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>* tmpMap = CObjectParameterizingAllocator< CCACHINGMAP<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationContext*>, CContext* >::allocateAndConstructAndParameterize(ontologyContext->getMemoryAllocationManager(), ontologyContext);
								if (recomputationIdReleasingIndividualAssociationContextMap) {
									*tmpMap = *recomputationIdReleasingIndividualAssociationContextMap;
								}
								recomputationIdReleasingIndividualAssociationContextMap = tmpMap;
								recomputationIdReleasingIndividualAssociationContextMapLocated = true;
								ontologyData->setRecomputationIdReleasingIndividualAssociationContextMap(recomputationIdReleasingIndividualAssociationContextMap);
							}
							recomputationIdReleasingIndividualAssociationContextMap->erase(recomputationIdReleasingIndividualAssociationContextMap->begin());
						}

						if (statDeletedMemoryPoolsCount > 0) {
							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Deleted %1 memory pools for %2 individual associations, %5 with separate memory management remaining, checked %3 (overall %4) slot reference usages.")
								.arg(statDeletedMemoryPoolsCount).arg(statIndividualAssociationSeparateMemoryManagmentDeletionCount).arg(statIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount).arg(mStatIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount).arg(mStatIndividualAssociationSeparateMemoryManagmentContextCreationCount - mStatIndividualAssociationSeparateMemoryManagmentDeletionCount), this);
						}


						if (mStatCollectStatistics) {
							cint64 maxIndi = -1;
							cint64 maxCount = -1;
							for (QHash<cint64, cint64>::const_iterator it = mIndiContextCountHash.constBegin(), itEnd = mIndiContextCountHash.constEnd(); it != itEnd; ++it) {
								if (it.value() > maxCount) {
									maxCount = it.value();
									maxIndi = it.key();
								}
							}

							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("%1 individuals with separate memory management, max memory contexts (%2) for individual %3, %4 remaining deletion queued, %5 remaining deletion scheduled.")
								.arg(mIndiContextCountHash.size()).arg(maxCount).arg(maxIndi).arg(mStatMemoryManagmentQueuedCheckingCount).arg(mStatMemoryManagmentScheduledReleasingCount), this);
						}


					}
				}



				void CBackendRepresentativeMemoryCache::queueIndividualAssociationMemoryContextDeletion(CBackendRepresentativeMemoryCacheIndividualAssociationContext* indiAssMemContext, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					ontologyData->addReleaseQueuedIndividualAssociationContextLinker(indiAssMemContext);
					++mStatIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingQueuingCount;
					++mStatMemoryManagmentQueuedCheckingCount;
				}



				void CBackendRepresentativeMemoryCache::createReaderSlotUpdate(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, CBackendRepresentativeMemoryCacheBaseContext* context) {
					ontologyData->setMinimumValidRecomputationId(ontologyData->getNextUpdateMinimumValidRecomputationId());
					ontologyData->setSlotUpdateIntegrated(true);
					CMemoryPoolProvider* memProv = context->getMemoryPoolAllocationManager();
					CBackendRepresentativeMemoryCacheSlotItem* slot = CObjectMemoryPoolAllocator<CBackendRepresentativeMemoryCacheSlotItem>::allocateAndConstructWithMemroyPool(memProv);
					CMemoryPoolContainerAllocationManager slotMemMan(slot, memProv);
					CMemoryAllocationManager* memMan = &slotMemMan;
					CContextBase tmpContext(memMan);


					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>* ontologyIdentifierDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>, CContext* >::allocateAndConstructAndParameterize(memMan, &tmpContext);
					*ontologyIdentifierDataHash = *mOntologyIdentifierDataHash;
					ontologyIdentifierDataHash->detach();
					slot->setOntologyIdentifierDataHash(ontologyIdentifierDataHash);

					mReaderSlotUpdateCount++;

					mLastUpdatedSlotLinker = slot;
					if (mSlotLinker) {
						mSlotLinker->append(slot);
					} else {
						mSlotLinker = slot;
					}


					for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = ontologyIdentifierDataHash->constBegin(), itEnd = ontologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
						ontologyData->incUsageCount();
					}

					CBackendRepresentativeMemoryCacheReader* readerLinkerIt = mReaderLinker;
					while (readerLinkerIt) {
						slot->incReader();
						readerLinkerIt->updateSlot(slot);
						readerLinkerIt = readerLinkerIt->getNext();
					}
				}






				CBackendRepresentativeMemoryCacheOntologyData* CBackendRepresentativeMemoryCache::prepareOntologyDataUpdate(cint64 ontologyIdentifier, cint64 minIndiCount) {
					CBackendRepresentativeMemoryCacheOntologyData*& ontologyData = (*mOntologyIdentifierDataHash)[ontologyIdentifier];
					if (!ontologyData || ontologyData->isSlotUpdateIntegrated()) {
						CBackendRepresentativeMemoryCacheOntologyData* prevOntologyData = ontologyData;

						ontologyData = new CBackendRepresentativeMemoryCacheOntologyData(&mContext);
						ontologyData->initOntologyData(ontologyIdentifier, !mConfLateIndividualLabelAssociationIndexing);


						mOntologyDataUpdateCount++;

						CBackendRepresentativeMemoryCacheContext* tmpContext = ontologyData->getTemporaryContext();
						CMemoryAllocationManager* tmpMemMan = tmpContext->getMemoryAllocationManager();
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* slotSigLabelItemHashArray[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* ontoSigLabelItemHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(tmpMemMan, tmpContext);
							slotSigLabelItemHashArray[i] = ontoSigLabelItemHash;
							ontologyData->setSignatureLabelItemHash(i, ontoSigLabelItemHash);

							if (prevOntologyData) {
								*ontoSigLabelItemHash = *prevOntologyData->getSignatureLabelItemHash(i);
								ontoSigLabelItemHash->detach();
							}
						}

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* ontologyNominalIndiIdIndirectConnectionDataHash = CObjectParameterizingAllocator< CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(tmpMemMan, tmpContext);
						if (prevOntologyData) {
							*ontologyNominalIndiIdIndirectConnectionDataHash = *prevOntologyData->getNominaIIndividualdIndirectConnectionDataHash();
							ontologyNominalIndiIdIndirectConnectionDataHash->detach();
						}
						ontologyData->setNominaIIndividualdIndirectConnectionDataHash(ontologyNominalIndiIdIndirectConnectionDataHash);



						if (!prevOntologyData) {
							CBackendRepresentativeMemoryCacheOntologyContext* ontologyContext = new CBackendRepresentativeMemoryCacheOntologyContext(&mContext);
							ontologyData->setOntologyContext(ontologyContext);
						} else {
							ontologyData->setOntologyContext(prevOntologyData->getOntologyContext());
						}



						if (!prevOntologyData) {
							cint64 indiIdAssoDataVectorSize = qMax((cint64)1000, minIndiCount+1);
							CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(ontologyData->getOntologyContext()->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
							for (cint64 i = 0; i < indiIdAssoDataVectorSize; ++i) {
								indiIdAssoDataVector[i] = nullptr;
							}
							ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
						} else {
							cint64 indiIdAssoDataVectorSize = prevOntologyData->getIndividualIdAssoiationDataVectorSize();
							CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = prevOntologyData->getIndividualIdAssoiationDataVector();
							ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
						}

						cint64 indiIdAssoDataVectorSize = ontologyData->getIndividualIdAssoiationDataVectorSize();
						if (minIndiCount > indiIdAssoDataVectorSize) {
							cint64 prevSize = indiIdAssoDataVectorSize;
							CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = ontologyData->getIndividualIdAssoiationDataVector();
							CBackendRepresentativeMemoryCacheIndividualAssociationData** prevIndiIdAssoDataVector = indiIdAssoDataVector;
							indiIdAssoDataVectorSize = minIndiCount+1;
							indiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(ontologyData->getOntologyContext()->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
							for (cint64 i = 0; i < prevSize; ++i) {
								indiIdAssoDataVector[i] = prevIndiIdAssoDataVector[i];
							}
							for (cint64 i = prevSize; i < indiIdAssoDataVectorSize; ++i) {
								indiIdAssoDataVector[i] = nullptr;
							}
							ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
						}



						if (prevOntologyData) {
							ontologyData->copyOntologyData(prevOntologyData);
						}

						CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker* linker = new CBackendRepresentativeMemoryCacheOntologyDataRecomputationReferenceLinker(ontologyData->getOntologyDataUpdateId());
						ontologyData->setRecomputationReferenceLinker(linker);


						ontologyData->incUsageCount();
						if (prevOntologyData) {
							prevOntologyData->decUsageCount();
							if (prevOntologyData->getUsageCount() <= 0) {
								++mOntologyDataReleasedCount;
								++mOntologyDataReleasedWhileNewCreationCount;
								CMemoryPoolAllocationManager* memMan = mContext.getMemoryPoolAllocationManager();
								CMemoryPool* memoryPools = prevOntologyData->getTemporaryContext()->getMemoryPoolContainer()->getMemoryPools();
								memMan->releaseTemporaryMemoryPools(memoryPools);
								delete prevOntologyData;
							}

						}
					}

					return ontologyData;
				}






				void CBackendRepresentativeMemoryCache::cleanUnusedSlots(CBackendRepresentativeMemoryCacheBaseContext* context) {
					CMemoryPoolAllocationManager* memMan = context->getMemoryPoolAllocationManager();
					CBackendRepresentativeMemoryCacheSlotItem* slotLinkerIt = mSlotLinker;
					CBackendRepresentativeMemoryCacheSlotItem* lastSlotLinker = nullptr;
					while (slotLinkerIt) {
						CBackendRepresentativeMemoryCacheSlotItem* tmpSlotLinker = nullptr;
						if (!slotLinkerIt->hasCacheReaders()) {
							tmpSlotLinker = slotLinkerIt;
						}
						CBackendRepresentativeMemoryCacheSlotItem* nextSlotLinker = slotLinkerIt->getNext();
						if (tmpSlotLinker) {
							if (!lastSlotLinker) {
								mSlotLinker = nextSlotLinker;
							} else {
								lastSlotLinker->setNext(nextSlotLinker);
							}

							CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>* ontologyIdentifierDataHash = slotLinkerIt->getOntologyIdentifierDataHash();
							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = ontologyIdentifierDataHash->constBegin(), itEnd = ontologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
								CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
								ontologyData->decUsageCount();
								if (ontologyData->getUsageCount() <= 0) {
									++mOntologyDataReleasedCount;
									++mOntologyDataReleasedWhileSlotUpdateCount;

									ontologyData->getRecomputationReferenceLinker()->setOntologyDataInactive();

									CMemoryPool* memoryPools = ontologyData->getTemporaryContext()->getMemoryPoolContainer()->getMemoryPools();
									memMan->releaseTemporaryMemoryPools(memoryPools);
									delete ontologyData;
								}
							}

							CMemoryPool* memoryPools = tmpSlotLinker->getMemoryPools();
							memMan->releaseTemporaryMemoryPools(memoryPools);
							++mReaderSlotReleasedCount;
						}
						else {
							lastSlotLinker = slotLinkerIt;
						}
						slotLinkerIt = nextSlotLinker;
					}
				}



				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCache::createCacheReader() {
					CBackendRepresentativeMemoryCacheReader* reader = new CBackendRepresentativeMemoryCacheReader();
					mReaderSyncMutex.lock();
					mReaderLinker = reader->append(mReaderLinker);
					mReaderSyncMutex.unlock();
					return reader;
				}


				CBackendRepresentativeMemoryCacheReader* CBackendRepresentativeMemoryCache::createOntologyFixedCacheReader(cint64 ontologyIdentifier) {
					CBackendRepresentativeMemoryCacheReader* reader = new CBackendRepresentativeMemoryCacheReader();
					mFixedOntologyIdentifierDataHashLock.lockForRead();
					CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mFixedOntologyIdentifierDataHash.value(ontologyIdentifier);
					if (ontologyData) {
						ontologyData->incUsageCount();
					}
					reader->fixOntologyData(ontologyData);
					mFixedOntologyIdentifierDataHashLock.unlock();
					ontologyData->waitIndividualLabelAssociationIndexed();
					return reader;
				}


				CBackendRepresentativeMemoryCacheWriter* CBackendRepresentativeMemoryCache::createCacheWriter() {
					CBackendRepresentativeMemoryCacheWriter* writer = new CBackendRepresentativeMemoryCacheWriter(this);
					return writer;
				}



				CBackendRepresentativeMemoryCache* CBackendRepresentativeMemoryCache::writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools) {
					if (mStatCollectStatistics) {
						mPendingUpdateCount.ref();
					}
					if (mLimitRemainingWritePending) {
						mRemainingWritePendingSemaphore.acquire();
					}
					if (mConfDirectUpdateSynchronization) {
						mDirectUpdateSyncMutex.lock();
						CWriteBackendAssociationCachedEvent* procEvent = new CWriteBackendAssociationCachedEvent(writeData, memoryPools);
						processCustomsEvents(CWriteBackendAssociationCachedEvent::EVENTTYPE, procEvent);
						mDirectUpdateSyncMutex.unlock();
					} else {
						postEvent(new CWriteBackendAssociationCachedEvent(writeData, memoryPools));
					}
					return this;
				}




				void CBackendRepresentativeMemoryCache::installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					for (CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinkerIt = tempCardWriteDataLinker; tempCardWriteDataLinkerIt; tempCardWriteDataLinkerIt = tempCardWriteDataLinkerIt->getNext()) {

						CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* labelWriteDataLinker = tempCardWriteDataLinkerIt->getLabelWriteDataLinker();
						CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)labelWriteDataLinker->getTemporaryData();

						CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinker = tempCardWriteDataLinkerIt->getCardinalityCacheValueLinker();

						bool handled = false;
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)labelCacheItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						if (extensionData) {
							handled = true;
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
								cint64 roleTag = cardValueLinkerIt->getRoleTag();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* roleCardData = extensionData->getRoleCardinalityData(roleTag);
								if (roleCardData) {
									roleCardData->updateExistentialMaxUsedCardinality(cardValueLinkerIt->getExistentialMaxUsedCardinality());
									roleCardData->updateMinimumRestrictingCardinality(cardValueLinkerIt->getMinimalRestrictingCardinality());
								} else {
									handled = false;
								}
							}
						}

						if (!handled) {
							CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* newExtensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newExtensionData->initCardinalityExtensionData();
							for (CBackendRepresentativeMemoryCardinalityValueLinker* cardValueLinkerIt = cardValueLinker; cardValueLinkerIt; cardValueLinkerIt = cardValueLinkerIt->getNext()) {
								cint64 roleTag = cardValueLinkerIt->getRoleTag();
								cint64 cardCount = cardValueLinkerIt->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardValueLinkerIt->getMinimalRestrictingCardinality();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
								cardData->initCardinalityData(cardCount, minRestCount);
								if (extensionData) {
									CBackendRepresentativeMemoryLabelCacheItemCardinalityData* roleCardData = extensionData->getRoleCardinalityData(roleTag);
									cardData->updateExistentialMaxUsedCardinality(roleCardData->getExistentialMaxUsedCardinality());
									cardData->updateMinimumRestrictingCardinality(roleCardData->getMinimumRestrictingCardinality());
								}
								newExtensionData->setRoleCardinalityData(roleTag, cardData);
							}

							labelCacheItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH, newExtensionData);
						}
					}
				}






				void CBackendRepresentativeMemoryCache::installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					for (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinkerIt = tempLabelWriteDataLinker; tempLabelWriteDataLinkerIt; tempLabelWriteDataLinkerIt = tempLabelWriteDataLinkerIt->getNext()) {

						cint64 signature = tempLabelWriteDataLinkerIt->getSignature();
						if (tempLabelWriteDataLinkerIt->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
							signature = 0;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = tempLabelWriteDataLinkerIt->getCacheValueLinker(); labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
								if (cacheValue.getCacheValueIdentifier() == CCacheValue::CACHE_VALUE_TAG_AND_TEMPORARY_ENTRY) {
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpLabelWriteData = (CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker*)cacheValue.getIdentification();
									CBackendRepresentativeMemoryLabelCacheItem* labelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpLabelWriteData->getTemporaryData();
									cacheValue.initCacheValue(labelCacheItem->getCacheEntryID(), (cint64)labelCacheItem, CCacheValue::CACHE_VALUE_TAG_AND_ENTRY);
									labelValueLinkerIt->setCacheValue(cacheValue);
								}
								signature = CBackendRepresentativeMemoryCacheUtilities::getSignatureExtensionByCacheValue(signature, cacheValue);
							}
							// update signature
							tempLabelWriteDataLinkerIt->setSignature(signature);
						}

						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = tempLabelWriteDataLinkerIt->getCacheValueLinker();
						cint64 labelValueCount = tempLabelWriteDataLinkerIt->getCacheValueCount();
						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(tempLabelWriteDataLinkerIt->getLabelType()))[signature];

						CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = nullptr;
						for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt && !refLabelItem; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
							if (labelItemLinkerIt->getCacheValueCount() == labelValueCount) {
								CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
								bool identical = true;
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
									cint64 valueTag = cacheValue.getTag();
									CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
									if (!labelValue || labelValue->getCacheValue() != cacheValue) {
										identical = false;
									}
								}

								if (identical) {
									refLabelItem = labelItemLinkerIt;
								}
							}
						}

						if (!refLabelItem) {
							refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)tempLabelWriteDataLinkerIt->getLabelType());

							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

								CBackendRepresentativeMemoryLabelValueLinker*& newValueLinker = (*labelValueHash)[labelValueLinkerIt->getCacheValue().getTag()];
								if (!newValueLinker) {
									CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
									newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
									labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
									refLabelItem->addCacheValueLinker(newValueLinker);
								}
							}
							addCreatedLabelStatistics(tempLabelWriteDataLinkerIt->getLabelType(), refLabelItem, ontologyData);
							sigResolveItem.appendLabelItem(refLabelItem);
						}

						if (!refLabelItem->isCompletelyHandled() && tempLabelWriteDataLinkerIt->isCompletelyHandled()) {
							refLabelItem->setCompletelyHandled(true);
						}
						if (!refLabelItem->isCompletelySaturated() && tempLabelWriteDataLinkerIt->isCompletelySaturated()) {
							refLabelItem->setCompletelySaturated(true);
						}
						if (!refLabelItem->hasNondeterministicElements() && tempLabelWriteDataLinkerIt->hasNondeterministicElements()) {
							refLabelItem->setNondeterministicElements(true);
						}


						if (tempLabelWriteDataLinkerIt->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
							getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
						}

						tempLabelWriteDataLinkerIt->setTemporaryData(refLabelItem);
					}

				}


				CBackendRepresentativeMemoryCache* CBackendRepresentativeMemoryCache::addCreatedLabelStatistics(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					++mStatLabelCount;
					mStatMaxLabelValueCount = qMax(mStatMaxLabelValueCount, label->getCacheValueCount());
					mStatLabelTypeCount[labelType]++;
					mStatLabelTypeMaxValueCount[labelType] = qMax(mStatLabelTypeMaxValueCount[labelType], label->getCacheValueCount());
					mStatLabelTypeAllValueCount[labelType] += label->getCacheValueCount();
					return this;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCache::getExtendedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, CCacheValue extendingCacheValue, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					if (label->getTagCacheValueHash(false)->contains(extendingCacheValue.getTag())) {
						return label;
					}
					CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = label->getCacheValueLinker();
					cint64 signature = label->getSignature() + qHash((qint64)extendingCacheValue.getTag());
					cint64 newCount = label->getCacheValueCount() + 1;

					// test with signature whether extended label already exists
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(labelType))[signature];

					for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
						if (labelItemLinkerIt->getCacheValueCount() == newCount) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
							bool identical = true;

							CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(extendingCacheValue.getTag());
							if (!labelValue || labelValue->getCacheValue() != extendingCacheValue) {
								identical = false;
							}

							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
								cint64 valueTag = cacheValue.getTag();
								CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
								if (!labelValue || labelValue->getCacheValue() != cacheValue) {
									identical = false;
								}
							}

							if (identical) {
								return labelItemLinkerIt;
							}
						}
					}


					// create new label item
					CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
					refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)labelType);

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

						const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
						CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
						newValueLinker->initLabelValueLinker(cacheValue);
						labelValueHash->insert(cacheValue.getTag(), newValueLinker);
						refLabelItem->addCacheValueLinker(newValueLinker);
					}

					CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
					newValueLinker->initLabelValueLinker(extendingCacheValue);
					labelValueHash->insert(extendingCacheValue.getTag(), newValueLinker);
					refLabelItem->addCacheValueLinker(newValueLinker);

					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
					}
					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {


						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)label->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						if (extensionData) {

							CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* newExtensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newExtensionData->initCardinalityExtensionData();
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* newExtensionDataHash = newExtensionData->getRoleCardinalityDataHash();

							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = extensionData->getRoleCardinalityDataHash()->constBegin(), itEnd = extensionData->getRoleCardinalityDataHash()->constEnd(); it != itEnd; ++it) {
								cint64 roleTag = it.key();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
								cint64 cardCount = cardData->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardData->getMinimumRestrictingCardinality();

								CBackendRepresentativeMemoryLabelCacheItemCardinalityData*& roleCardData = (*newExtensionDataHash)[roleTag];
								if (!roleCardData) {
									roleCardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
									roleCardData->initCardinalityData(cardCount, minRestCount);
								} else {
									roleCardData->updateExistentialMaxUsedCardinality(cardCount);
									roleCardData->updateMinimumRestrictingCardinality(minRestCount);
								}
							}

							refLabelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH, newExtensionData);


						}

					}

					addCreatedLabelStatistics(labelType, refLabelItem, ontologyData);
					sigResolveItem.appendLabelItem(refLabelItem);
					return refLabelItem;
				}




				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCache::getReducedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, function<bool(const CCacheValue& cacheValue)> reduceCheckFunction, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = label->getCacheValueLinker();
					cint64 signature = 0;
					cint64 newCount = 0;
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
						CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
						if (!reduceCheckFunction(cacheValue)) {
							// extend signature/hash value
							signature += qHash((qint64)cacheValue.getTag());
							newCount++;
						}
					}


					if (newCount == label->getCacheValueCount()) {
						return label;
					}

					// test with signature whether reduced label already exists
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(labelType))[signature];

					for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
						if (labelItemLinkerIt->getCacheValueCount() == newCount) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
							bool identical = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
								if (!reduceCheckFunction(cacheValue)) {
									cint64 valueTag = cacheValue.getTag();
									CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
									if (!labelValue || labelValue->getCacheValue() != cacheValue) {
										identical = false;
									}
								}
							}

							if (identical) {
								return labelItemLinkerIt;
							}
						}
					}


					// create new label item
					CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
					refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)labelType);

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

						const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
						if (!reduceCheckFunction(cacheValue)) {
							CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
							newValueLinker->initLabelValueLinker(cacheValue);
							labelValueHash->insert(cacheValue.getTag(), newValueLinker);
							refLabelItem->addCacheValueLinker(newValueLinker);
						}
					}

					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
					}


					addCreatedLabelStatistics(labelType, refLabelItem, ontologyData);
					sigResolveItem.appendLabelItem(refLabelItem);
					return refLabelItem;
				}



				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCache::getAdditionMergedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* additionLabel, CBackendRepresentativeMemoryLabelCacheItem* associatedlabel, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					if (!additionLabel) {
						return associatedlabel;
					}
					if (!associatedlabel) {
						return additionLabel;
					}
					if (additionLabel == associatedlabel) {
						return associatedlabel;
					}
					CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinker = additionLabel->getCacheValueLinker();
					CBackendRepresentativeMemoryLabelValueLinker* associatedLabelValueLinker = associatedlabel->getCacheValueLinker();
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* associatedLabelValueHash = associatedlabel->getTagCacheValueHash(false);
					// check whether all values are already included
					bool valuesAlreadyIncluded = true;
					cint64 signature = associatedlabel->getSignature();
					cint64 newCount = associatedlabel->getCacheValueCount();
					for (CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinkerIt = additionLabelValueLinker; additionLabelValueLinkerIt; additionLabelValueLinkerIt = additionLabelValueLinkerIt->getNext()) {
						CCacheValue cacheValue = additionLabelValueLinkerIt->getCacheValue();
						if (!associatedLabelValueHash || !associatedLabelValueHash->contains(cacheValue.getTag())) {
							valuesAlreadyIncluded = false;
							// extend signature/hash value
							signature += qHash((qint64)cacheValue.getTag());
							newCount++;
						}
					}
					if (valuesAlreadyIncluded) {
						return associatedlabel;
					}
					// test with signature whether extended label already exists
					CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolveItem = (*ontologyData->getSignatureLabelItemHash(labelType))[signature];

					for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
						if (labelItemLinkerIt->getCacheValueCount() == newCount) {
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = labelItemLinkerIt->getTagCacheValueHash(false);
							bool identical = true;
							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = associatedLabelValueLinker; labelValueLinkerIt && identical; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = labelValueLinkerIt->getCacheValue();
								cint64 valueTag = cacheValue.getTag();
								CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
								if (!labelValue || labelValue->getCacheValue() != cacheValue) {
									identical = false;
								}
							}
							for (CBackendRepresentativeMemoryLabelValueLinker* additionLabelValueLinkerIt = additionLabelValueLinker; additionLabelValueLinkerIt && identical; additionLabelValueLinkerIt = additionLabelValueLinkerIt->getNext()) {
								const CCacheValue& cacheValue = additionLabelValueLinkerIt->getCacheValue();
								cint64 valueTag = cacheValue.getTag();
								CBackendRepresentativeMemoryLabelValueLinker* labelValue = labelValueHash->value(valueTag);
								if (!labelValue || labelValue->getCacheValue() != cacheValue) {
									identical = false;
								}
							}

							if (identical) {
								return labelItemLinkerIt;
							}
						}
					}


					// create new label item
					CBackendRepresentativeMemoryLabelCacheItem* refLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
					refLabelItem->initCacheEntry(signature, ontologyData->getNextEntryID(), (CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE)labelType);

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = refLabelItem->getTagCacheValueHash(true);
					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = associatedLabelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {

						CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
						newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
						labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
						refLabelItem->addCacheValueLinker(newValueLinker);
					}

					for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = additionLabelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
						CBackendRepresentativeMemoryLabelValueLinker*& newValueLinker = (*labelValueHash)[labelValueLinkerIt->getCacheValue().getTag()];
						if (!newValueLinker) {
							newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
							newValueLinker->initLabelValueLinker(labelValueLinkerIt->getCacheValue());
							labelValueHash->insert(labelValueLinkerIt->getCacheValue().getTag(), newValueLinker);
							refLabelItem->addCacheValueLinker(newValueLinker);
						}
					}

					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						getNeighbourArrayRoleTagResolvingLabelExtensionData(refLabelItem, ontologyData);
					}
					if (refLabelItem->getLabelType() == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {


						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData1 = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)additionLabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData2 = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)associatedlabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
						if (extensionData1 || extensionData2) {


							CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* newExtensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newExtensionData->initCardinalityExtensionData();
							CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* newExtensionDataHash = newExtensionData->getRoleCardinalityDataHash();

							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = extensionData1->getRoleCardinalityDataHash()->constBegin(), itEnd = extensionData1->getRoleCardinalityDataHash()->constEnd(); it != itEnd; ++it) {
								cint64 roleTag = it.key();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
								cint64 cardCount = cardData->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardData->getMinimumRestrictingCardinality();

								CBackendRepresentativeMemoryLabelCacheItemCardinalityData*& roleCardData = (*newExtensionDataHash)[roleTag];
								if (!roleCardData) {
									roleCardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
									roleCardData->initCardinalityData(cardCount, minRestCount);
								} else {
									roleCardData->updateExistentialMaxUsedCardinality(cardCount);
									roleCardData->updateMinimumRestrictingCardinality(minRestCount);
								}
							}
							for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = extensionData2->getRoleCardinalityDataHash()->constBegin(), itEnd = extensionData2->getRoleCardinalityDataHash()->constEnd(); it != itEnd; ++it) {
								cint64 roleTag = it.key();
								CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
								cint64 cardCount = cardData->getExistentialMaxUsedCardinality();
								cint64 minRestCount = cardData->getMinimumRestrictingCardinality();

								CBackendRepresentativeMemoryLabelCacheItemCardinalityData*& roleCardData = (*newExtensionDataHash)[roleTag];
								if (!roleCardData) {
									roleCardData = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemCardinalityData >::allocateAndConstruct(context->getMemoryAllocationManager());
									roleCardData->initCardinalityData(cardCount, minRestCount);
								} else {
									roleCardData->updateExistentialMaxUsedCardinality(cardCount);
									roleCardData->updateMinimumRestrictingCardinality(minRestCount);
								}
							}

							refLabelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH, newExtensionData);


						}	

					}

					addCreatedLabelStatistics(labelType, refLabelItem, ontologyData);
					sigResolveItem.appendLabelItem(refLabelItem);
					return refLabelItem;

				}





				bool CBackendRepresentativeMemoryCache::checkAssociationUsage(CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinkerIt = tempAssUseDataLinker; tempAssUseDataLinkerIt; tempAssUseDataLinkerIt = tempAssUseDataLinkerIt->getNext()) {
						cint64 individualID = tempAssUseDataLinkerIt->getIndividualID();
						cint64 usedUpateID = tempAssUseDataLinkerIt->getUsedAssociationUpdateId();

						CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
						if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
							associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
						}

						mCheckedIndiCount++;

						if (!associationData || (associationData->getAssociationDataUpdateId() != usedUpateID && associationData->isCompletelyHandled())) {

							associationsUpdated |= markRepresentativeReferencedIndividualAssociationIncompletelyHandled(individualID, associationData, ontologyData);

							mCheckIncompatibleIndiCount++;


						}
					}
					return associationsUpdated;
				}






				bool CBackendRepresentativeMemoryCache::markRepresentativeReferencedIndividualAssociationIncompletelyHandled(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool associationsUpdated = false;
					if (!associationData || !associationData->hasRepresentativeSameIndividualMerging()) {
						DEBUG_HANDLING_STRING(QString("Marking %1 incompletly handled").arg(associationData->getAssociatedIndividualId()))
						associationsUpdated = markIndividualAssociationIncompletelyHandled(indiId, associationData, ontologyData);
					} else {
						DEBUG_HANDLING_STRING(QString("Marking same representative individual %2 incompletly handled for %1").arg(associationData->getAssociatedIndividualId()).arg(associationData->getRepresentativeSameIndividualId()))
						CBackendRepresentativeMemoryCacheIndividualAssociationData* repAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[associationData->getRepresentativeSameIndividualId()];
						associationsUpdated = markIndividualAssociationIncompletelyHandled(repAssociationData->getRepresentativeSameIndividualId(), repAssociationData, ontologyData);
					}
					return associationsUpdated;
				}



				bool CBackendRepresentativeMemoryCache::markIndividualAssociationIncompletelyHandled(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool associationsUpdated = false;

					if (!associationData || associationData->isCompletelyHandled()) {

						CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
						locAssociationData = createLocalizedIndividualAssociationData(individualID, associationData, ontologyData, false);
						CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locAssociationData, ontologyData);
						locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						locAssociationData->mDebugUpdateFunction = QString("markIndividualAssociationIncompletelyHandled");
						locAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif


						if (associationData && associationData->getNeighbourRoleSetHash()) {

							locAssociationData->setNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash());
							locAssociationData->setRoleSetNeighbourArray(associationData->getRoleSetNeighbourArray());
						}

						locAssociationData->setCompletelyHandled(false);

						associationsUpdated = true;

						storeIndividualIncompletelyMarked(locAssociationData, !locAssociationData->isCompletelyHandled(), ontologyData);
						setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);

					}
					return associationsUpdated;
				}



				bool CBackendRepresentativeMemoryCache::markIndividualAssociationCompletelyHandled(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool associationsUpdated = false;

					if (!associationData || !associationData->isCompletelyHandled()) {

						CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
						locAssociationData = createLocalizedIndividualAssociationData(individualID, associationData, ontologyData, false);
						CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locAssociationData, ontologyData);
						locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						locAssociationData->mDebugUpdateFunction = QString("markIndividualAssociationCompletelyHandled");
						locAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif


						if (associationData && associationData->getNeighbourRoleSetHash()) {

							locAssociationData->setNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash());
							locAssociationData->setRoleSetNeighbourArray(associationData->getRoleSetNeighbourArray());
						}

						locAssociationData->setCompletelyHandled(true);

						associationsUpdated = true;

						storeIndividualIncompletelyMarked(locAssociationData, !locAssociationData->isCompletelyHandled(), ontologyData);
						setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);

					}
					return associationsUpdated;
				}



				bool CBackendRepresentativeMemoryCache::checkUpdateRejection(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool rejection = false;

					cint64 indiUpdateCount = 0;
					cint64 incompatibleIndiUpdateCount = 0;
					cint64 neighbourLinkCount = 0;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt && !rejection; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();
						cint64 sameAsIndividualId = tempAssWriteDataLinkerIt->getDeterministicSameIndividualId();
						cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();

						if (tempAssWriteDataLinkerIt->getDeterministicSameIndividualId() == individualID) {
							++indiUpdateCount;

							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
							}

							if (associationData && associationData->getAssociationDataUpdateId() != usedUpdateId) {
								++incompatibleIndiUpdateCount;

								if (associationData->getLastPropagationCuttingUpdateId() > usedUpdateId) {
									neighbourLinkCount += associationData->getNeighbourRoleSetHash()->getNeighbourCount();
									cint64 totalIndiCount = ontologyData->getIndividualAssociationsCount();
									if (mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourCount >= 0 && neighbourLinkCount > mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourCount) {
										rejection = true;
									}
									double propCutLinkRatio = (double)neighbourLinkCount / (double)totalIndiCount;
									if (propCutLinkRatio > mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourRatio) {
										rejection = true;
									}
								}
							}

						}
					}

					double incomUpdateRatio = (double)incompatibleIndiUpdateCount / (double)indiUpdateCount;
					if (incomUpdateRatio > mConfUpdateRejectingIncompatibleIndividualAssociationsRatio) {
						rejection = true;
					}

					return rejection;
				}


				bool CBackendRepresentativeMemoryCache::handleUpdateRejection(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();

						if (tempAssWriteDataLinkerIt->isScheduledIndividual()) {
							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
							}
							if (associationData) {
								associationData->setCacheTouchId(mNextIndiUpdateId++);
							}
						}

					}
					return true;
				}



				bool CBackendRepresentativeMemoryCache::analyseDeterministicSameAsAssociationInstallation(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool installable = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();
						cint64 sameAsIndividualId = tempAssWriteDataLinkerIt->getDeterministicSameIndividualId();
						cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();

						if (tempAssWriteDataLinkerIt->getDeterministicSameIndividualId() != individualID) {

							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
							}

							if (!associationData || (associationData->getAssociationDataUpdateId() == usedUpdateId && !associationData->hasDeterministicSameIndividualMerging())) {

								CDeterministicSameHandlingInstallationData& data = mDeterministicSameHandlingInstallationDataHash[sameAsIndividualId];
								data.mIdPossibleInstallationSet.insert(individualID);
								if (!associationData || !associationData->hasDeterministicSameIndividualMerging()) {
									data.mIdFirstPossibleInstallationSet.insert(individualID);
									DEBUG_HANDLING_STRING(QString("Adding %1 as first deterministic same handling installation possiblity to %2").arg(individualID).arg(sameAsIndividualId))
								} else {
									DEBUG_HANDLING_STRING(QString("Adding %1 as deterministic same handling installation possiblity to %2").arg(individualID).arg(sameAsIndividualId))
								}
								installable = true;
								if (associationData) {
									CBackendRepresentativeMemoryLabelCacheItem* detSameHandledLabel = associationData->getDeterministicMergedSameConsideredLabelCacheEntry();
									if (detSameHandledLabel) {
										for (CBackendRepresentativeMemoryLabelValueLinker* valueLinkerIt = detSameHandledLabel->getCacheValueLinker(); valueLinkerIt; valueLinkerIt = valueLinkerIt->getNext()) {
											cint64 otherId = valueLinkerIt->getData().getTag();
											data.mIdPossibleInstallationSet.insert(otherId);
											DEBUG_HANDLING_STRING(QString("Adding %1 referenced from %3 as deterministic same handling installation possiblity to %2").arg(otherId).arg(sameAsIndividualId).arg(individualID))
										}
									}
								}

							}
						}
					}
					return installable;
				}


				bool CBackendRepresentativeMemoryCache::installDeterministicSameAsAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();
						cint64 sameAsIndividualId = tempAssWriteDataLinkerIt->getDeterministicSameIndividualId();
						cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();

						if (tempAssWriteDataLinkerIt->getDeterministicSameIndividualId() != individualID) {
							ontologyData->setSameIndividualsMergings(true);

							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
							}

							if (associationData) {
								associationData->setCacheTouchId(mNextIndiUpdateId++);
							}
							DEBUG_HANDLING_STRING(QString("Checking determinitic same reference update for %1").arg(individualID))


							if (!associationData || tempAssWriteDataLinkerIt->getDeterministicSameIndividualId() != associationData->getDeterministicSameIndividualId()) {

								CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAsAssociationData = nullptr;

								if (ontologyData->getIndividualIdAssoiationDataVector() && sameAsIndividualId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
									detSameAsAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[sameAsIndividualId];
								}

								//if (detSameAsAssociationData && detSameAsAssociationData->hasDeterministicSameIndividualMerging()) {
								//	sameAsIndividualId = detSameAsAssociationData->getDeterministicSameIndividualId();
								//	detSameAsAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[sameAsIndividualId];
								//}
								if (detSameAsAssociationData) {


									CBackendRepresentativeMemoryLabelCacheItem* detSameHandledLabel = detSameAsAssociationData->getDeterministicMergedSameConsideredLabelCacheEntry();


									//if (!detSameAsAssociationData->hasRepresentativeSameIndividualMerging() && detSameAsAssociationData->isCompletelyHandled() && (!associationData || associationData->getAssociationDataUpdateId() == usedUpdateId || associationData->hasDeterministicSameIndividualMerging())) {
									if (detSameHandledLabel && detSameHandledLabel->hasCachedTagValue(individualID)) {

										associationsUpdated = installDeterministicSameAsAssociationUpdate(associationData, individualID, detSameAsAssociationData, sameAsIndividualId, ontologyData);
										mStatDetSameAssociationInstallCount++;



									} else {
										DEBUG_HANDLING_STRING(QString("Determinitic same reference update for %1 failed").arg(individualID))

										++mStatDetSameAssociationFailedCount;
										if (detSameAsAssociationData->hasRepresentativeSameIndividualMerging()) {
											++mStatDetSameAssociationRepMergedDestFailedCount;
										} else if (!detSameAsAssociationData->isCompletelyHandled()) {
											++mStatDetSameAssociationIncompleteHandledDestFailedCount;
										} else {
											if (associationData && associationData->getAssociationDataUpdateId() == usedUpdateId) {
												++mStatDetSameAssociationDifferentUpdateIdFailedCount;
											}
											if (associationData && !associationData->hasDeterministicSameIndividualMerging()) {
												++mStatDetSameAssociationDifferentDestIdFailedCount;
											}
										}

										if (associationData && detSameAsAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) != associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL)) {
											mTmpDetSameMergingCompletionReferenceHash.insert(individualID, sameAsIndividualId);
										} else if (!associationData || !associationData->isCompletelyHandled()) {

											markIndividualAssociationCompletelyHandled(individualID, associationData, ontologyData);
										}
										markRepresentativeReferencedIndividualAssociationIncompletelyHandled(sameAsIndividualId, detSameAsAssociationData, ontologyData);

									}
								}
							} else if (associationData && associationData->getAssociationDataUpdateId() == usedUpdateId && !associationData->isCompletelyHandled()) {
								markIndividualAssociationCompletelyHandled(individualID, associationData, ontologyData);
							}
						}
					}
					return associationsUpdated;
				}



				bool CBackendRepresentativeMemoryCache::checkRequiresDeterministicSameAsAssociationUpdateInstallation(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAsAssociationData, cint64 sameAsIndividualId, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					if (associationData->getDeterministicSameIndividualId() != sameAsIndividualId) {
						return true;
					}
					if (associationData->getRepresentativeSameIndividualId() != detSameAsAssociationData->getRepresentativeSameIndividualId()) {
						return true;
					}
					if (associationData->getRoleSetNeighbourArray() != detSameAsAssociationData->getRoleSetNeighbourArray()) {
						return true;
					}
					if (!associationData->isCompletelyHandled()) {
						return true;
					}
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						if (associationData->getLabelCacheEntry(i) != detSameAsAssociationData->getLabelCacheEntry(i)) {
							return true;
						}
					}

					return false;
				}


				bool CBackendRepresentativeMemoryCache::installDeterministicSameAsAssociationUpdate(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAsAssociationData, cint64 sameAsIndividualId, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {

					DEBUG_HANDLING_STRING(QString("Updating deterministic same handled references for %1 to %2").arg(associationData->getAssociatedIndividualId()).arg(sameAsIndividualId))

					CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = createLocalizedIndividualAssociationData(individualID, associationData, ontologyData, false);
					CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locAssociationData, ontologyData);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					locAssociationData->mDebugUpdateFunction = QString("installDeterministicSameAsAssociationUpdate");
					locAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif
					locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
					locAssociationData->setDeterministicSameIndividualId(sameAsIndividualId);
					locAssociationData->setRepresentativeSameIndividualId(detSameAsAssociationData->getRepresentativeSameIndividualId());
					if (locAssociationData->hasRepresentativeSameIndividualMerging()) {
						if (!associationData || !associationData->hasRepresentativeSameIndividualMerging()) {
							mStatDetSameRepresentativeMergingCount++;
							ontologyData->incIndividualAssociationMergingCount();
						}
					}
					locAssociationData->setRoleSetNeighbourArray(detSameAsAssociationData->getRoleSetNeighbourArray());
					locAssociationData->setNeighbourRoleSetHash(detSameAsAssociationData->getNeighbourRoleSetHash());
					locAssociationData->setCompletelyHandled(true);
					locAssociationData->setDeterministicConceptSetLabelCacheEntry(nullptr);


					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						locAssociationData->setLabelCacheEntry(i, detSameAsAssociationData->getLabelCacheEntry(i));
						updateIndexedAssociationCount(locAssociationData, associationData, i, ontologyData);
					}

					CBackendRepresentativeMemoryLabelCacheItem* mergedSameAsLabelItem = nullptr;
					if (associationData->hasDeterministicSameIndividualMerging() && associationData->getDeterministicSameIndividualId() != sameAsIndividualId) {
						CBackendRepresentativeMemoryLabelCacheItem* detSameAsLabelItem = detSameAsAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
						CBackendRepresentativeMemoryLabelCacheItem* prevSameAsLabelItem = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);

						mergedSameAsLabelItem = getAdditionMergedLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, prevSameAsLabelItem, detSameAsLabelItem, ontologyData);
						if (mergedSameAsLabelItem != detSameAsLabelItem) {
							locAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, mergedSameAsLabelItem);
							updateIndexedAssociationCount(locAssociationData, detSameAsLabelItem, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, ontologyData);
							mTmpDetSameMergingCompletionReferenceHash.insert(individualID, sameAsIndividualId);
						}
					}

					storeIndividualIncompletelyMarked(locAssociationData, !locAssociationData->isCompletelyHandled(), ontologyData);
					setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);
					return true;
				}



				bool CBackendRepresentativeMemoryCache::installAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool associationsUpdated = false;
					for (CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt = tempAssWriteDataLinker; tempAssWriteDataLinkerIt; tempAssWriteDataLinkerIt = tempAssWriteDataLinkerIt->getNext()) {
						cint64 individualID = tempAssWriteDataLinkerIt->getIndividualID();


						DEBUG_HANDLING_STRING(QString("Handling update data for individual %1").arg(individualID))


						if (tempAssWriteDataLinkerIt->getDeterministicSameIndividualId() == individualID) {

							cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();
							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && individualID < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
							}
							if (associationData && associationData->hasDeterministicSameIndividualMerging()) {
								if (!associationData->isCompletelyHandled()) {
									markIndividualAssociationCompletelyHandled(individualID, associationData, ontologyData);
								}
								individualID = associationData->getDeterministicSameIndividualId();
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[individualID];
								DEBUG_HANDLING_STRING(QString("Resolving update data for individual %1 to %2").arg(tempAssWriteDataLinkerIt->getIndividualID()).arg(individualID))
							}

							associationsUpdated |= installAssociationUpdate(individualID, associationData, tempAssWriteDataLinkerIt, ontologyData);
						}
					}
					return associationsUpdated;
				}



				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCache::createLocalizedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool allowSeparatedManagement, bool incrementUpdateId) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();

					bool separatedMemoryManagementReusing = false;
					bool separatedMemoryManagementNew = false;
					CBackendRepresentativeMemoryCacheIndividualAssociationContext* existingIndiAssContext = nullptr;
					if (associationData) {
						existingIndiAssContext = associationData->getIndividualAssociationMemoryContext();
					}
					if (associationData && existingIndiAssContext) {
						CMemoryPoolContainer* memPoolContainer = existingIndiAssContext->getMemoryPoolContainer();
						if (existingIndiAssContext->getIndividualAssociationDataUsageCount() < 50 && memPoolContainer->getMemoryPoolCount() < 10 && memPoolContainer->getMemorySize() <= 10000000) {
							separatedMemoryManagementReusing = true;
						}
					}
					if (associationData && existingIndiAssContext && !allowSeparatedManagement) {
						separatedMemoryManagementReusing = true;
					}
					if (!separatedMemoryManagementReusing && associationData && existingIndiAssContext && allowSeparatedManagement) {
						separatedMemoryManagementNew = true;
					}
					if (!separatedMemoryManagementReusing && !separatedMemoryManagementNew && associationData && associationData->getAssociationDataUpdateId() > 10 && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved() && allowSeparatedManagement) {
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighArray = associationData->getRoleSetNeighbourArray();
						cint64 neighbourCount = 0;
						if (roleSetNeighArray) {
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* roleSetNeighArrayInd = roleSetNeighArray->getIndexData();
							if (roleSetNeighArrayInd) {
								for (cint64 i = 0; i < roleSetNeighArrayInd->getArraySize(); ++i) {
									neighbourCount += roleSetNeighArray->at(i).getIndividualCount();
								}
							}
						}
						if (neighbourCount > 100) {
							separatedMemoryManagementNew = true;
						}
					}
					if (!separatedMemoryManagementReusing && !separatedMemoryManagementNew && associationData && associationData->getAssociationDataUpdateId() > 20 && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved() && !existingIndiAssContext) {
						separatedMemoryManagementNew = true;
					}


					CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
					if (separatedMemoryManagementReusing) {
						locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(existingIndiAssContext->getMemoryAllocationManager());
						locAssociationData->setIndividualAssociationMemoryContext(existingIndiAssContext);
						existingIndiAssContext->incIndividualAssociationDataUsageCount();
						existingIndiAssContext->setLastRecomputationReferenceLinker(ontologyData->getRecomputationReferenceLinker());
						++mStatIndividualAssociationSeparateMemoryManagmentContextReuseCount;
					} else if (separatedMemoryManagementNew) {
						CBackendRepresentativeMemoryCacheIndividualAssociationContext* indiAssContext = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationContext, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						if (mStatCollectStatistics) {
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
							indiAssContext->mIndiId = individualID;
							mIndiContextCountHash[individualID]++;
#endif
						}

						locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(indiAssContext->getMemoryAllocationManager());
						locAssociationData->setIndividualAssociationMemoryContext(indiAssContext);
						indiAssContext->incIndividualAssociationDataUsageCount();
						indiAssContext->setFirstRecomputationReferenceLinker(ontologyData->getLastActiveRecomputationReferenceLinker());
						indiAssContext->setLastRecomputationReferenceLinker(ontologyData->getRecomputationReferenceLinker());
						++mStatIndividualAssociationSeparateMemoryManagmentContextCreationCount;

						if (associationData && existingIndiAssContext) {
							indiAssContext->setPreviousMemoryManagementCount(existingIndiAssContext->getPreviousMemoryManagementCount() + 1);
							existingIndiAssContext->setLastRecomputationReferenceLinker(ontologyData->getRecomputationReferenceLinker());
							queueIndividualAssociationMemoryContextDeletion(existingIndiAssContext, ontologyData);
						}
					} else {
						locAssociationData = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData >::allocateAndConstruct(context->getMemoryAllocationManager());
						++mStatIndividualAssociationWithoutSeparateMemoryManagmentCount;
					}

					if (associationData) {
						locAssociationData->initAssociationData(associationData, incrementUpdateId);
					} else {
						locAssociationData->initAssociationData(individualID);
					}
					return locAssociationData;

				}

				CBackendRepresentativeMemoryCacheContext* CBackendRepresentativeMemoryCache::getIndividualAssociationDataMemoryContext(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool* requiresDataCopying) {
					CBackendRepresentativeMemoryCacheIndividualAssociationContext* context = associationData->getIndividualAssociationMemoryContext();
					if (context) {		
						if (requiresDataCopying && context->getIndividualAssociationDataUsageCount() <= 1 && context->getPreviousMemoryManagementCount() > 0) {
							*requiresDataCopying = true;
						}
						return context;
					}
					return ontologyData->getOntologyContext();
				}






























				bool CBackendRepresentativeMemoryCache::isCacheValueRoleInverse(const CCacheValue& cacheValue) {
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = cacheValue.getCacheValueIdentifier();
					return cacheValueIdentifier == CCacheValue::CACHEVALTAGANDINVERSEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGANDINVERSEDASSERTEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGANDINVERSED_NOMINAL_CONNECTED_ROLE
						|| cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;
				}


				bool CBackendRepresentativeMemoryCache::isCacheValueRoleNondeterministic(const CCacheValue& cacheValue) {
					CCacheValue::CACHEVALUEIDENTIFIER cacheValueIdentifier = cacheValue.getCacheValueIdentifier();
					return cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE
						|| cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || cacheValueIdentifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;
				}



				bool CBackendRepresentativeMemoryCache::isRoleNeighbourLinkLabelItemCompatibility(CBackendRepresentativeMemoryLabelCacheItem* itemPrev, CBackendRepresentativeMemoryLabelCacheItem* itemNew) {
					if (itemPrev->getCacheValueCount() != itemNew->getCacheValueCount()) {
						return false;
					}

					bool sameRolesCompatible = true;
					for (CBackendRepresentativeMemoryLabelValueLinker* linkerIt = itemNew->getCacheValueLinker(); linkerIt && sameRolesCompatible; linkerIt = linkerIt->getNext()) {
						cint64 tag = linkerIt->getCacheValue().getTag();
						if (!itemPrev->getTagCacheValueHash(false)->contains(tag)) {
							sameRolesCompatible = false;
						} else {
							const CCacheValue& prevCacheValue = itemPrev->getTagCacheValueHash(false)->value(tag)->getCacheValue();
							const CCacheValue& newCacheValue = linkerIt->getCacheValue();
							if (isCacheValueRoleInverse(prevCacheValue) != isCacheValueRoleInverse(newCacheValue)) {
								sameRolesCompatible = false;
							}
							bool prevNondeterministic = isCacheValueRoleNondeterministic(prevCacheValue);
							bool newNondeterministic = isCacheValueRoleNondeterministic(newCacheValue);
							if (!prevNondeterministic && newNondeterministic) {
								sameRolesCompatible = false;
							}
						}
					}
					return sameRolesCompatible;
				}







				bool CBackendRepresentativeMemoryCache::installAssociationUpdate(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinkerIt, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {

					bool associationsUpdated = false;
					CIndividual* individual = tempAssWriteDataLinkerIt->getIndividual();

					cint64 usedUpdateId = tempAssWriteDataLinkerIt->getUsedAssociationUpdateId();


					if (associationData) {
						associationData->setCacheTouchId(mNextIndiUpdateId++);
					}

					bool labelAddition = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION;
					bool labelReplacement = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT;
					bool labelRemoval = tempAssWriteDataLinkerIt->getLabelUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REMOVAL;


					bool linksAddition = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::ADDITION;
					bool linksReplacement = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REPLACEMENT;
					bool linksRemoval = tempAssWriteDataLinkerIt->getLinksUpdateType() == CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::REMOVAL;




					CBackendRepresentativeMemoryLabelCacheItem* referredLabels[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT];
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = tempAssWriteDataLinkerIt->getReferredLabelData(i);
						if (!referredLabelCacheItem) {
							CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = tempAssWriteDataLinkerIt->getReferredTemporaryLabelData(i);
							if (tmpRefferedLabelItem) {
								referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
							}
						}
						referredLabels[i] = referredLabelCacheItem;
					}


					bool propCut = mPropagationCutIndiSet.contains(individualID);
					cint64 integratedIndirectlyConnectedIndividualsChangeId = tempAssWriteDataLinkerIt->getIntegratedIndirectlyConnectedIndividualsChangeId();

					bool incompatibleChanges = false;
					bool detSameNeighbourCompletion = tempAssWriteDataLinkerIt->requireSameAsNeighboursCompletion();
					if (associationData && associationData->getAssociationDataUpdateId() != usedUpdateId || associationData && associationData->getAssociatedIndividualId() != tempAssWriteDataLinkerIt->getIndividualID()) {



						bool labelCompatible = false;
						if (associationData && mConfInterpretUnchangedLabelsAsCompatible && associationData->isCompletelyHandled()) {
							CBackendRepresentativeMemoryCacheIndividualAssociationData* usedReferredAssociationData = associationData->getPreviousData();
							while (usedReferredAssociationData && usedReferredAssociationData->getAssociationDataUpdateId() > usedUpdateId) {
								usedReferredAssociationData = usedReferredAssociationData->getPreviousData();
							}
							if (usedReferredAssociationData && usedReferredAssociationData->getAssociationDataUpdateId() != usedUpdateId) {
								usedReferredAssociationData = nullptr;
							}

							if (usedReferredAssociationData) {
								labelCompatible = true;
								for (cint64 i = 0; associationData && labelCompatible && i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
									if (referredLabels[i] != associationData->getLabelCacheEntry(i)) {
										if (i == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
											if (labelReplacement && (linksAddition || propCut)) {
												labelAddition = true;
												labelReplacement = false;
											}
										} else if (i == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL || i == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
											labelAddition = true;
											labelReplacement = false;
										} else {
											labelCompatible = false;
										}

									}
								}

								if (labelCompatible && (linksReplacement && propCut || linksAddition)) {
									// only replacing/adding linkers must be checked and the same as for the referred association

									CBackendRepresentativeMemoryLabelCacheItem* conSetLabel = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
									CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)conSetLabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
									if (extensionData) {
										labelCompatible = false;
									}
									CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* currentNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();
									CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* referredNeighbourRoleSetHash = usedReferredAssociationData->getNeighbourRoleSetHash();
									for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker(); labelCompatible && roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
										CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
										cint64 neighbourId = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference().getIndividualID();
										CBackendRepresentativeMemoryLabelCacheItem* newNeighbourLabel = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
										if (!newNeighbourLabel) {
											CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
											if (tmpRefferedLabelItem) {
												newNeighbourLabel = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
											}
										}
										CBackendRepresentativeMemoryLabelCacheItem* referredNeighbourLabel = nullptr;
										if (referredNeighbourRoleSetHash) {
											referredNeighbourLabel = referredNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId);
										}
										CBackendRepresentativeMemoryLabelCacheItem* currentNeighbourLabel = nullptr;
										if (currentNeighbourRoleSetHash) {
											currentNeighbourLabel = currentNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId);
										}
										if (currentNeighbourLabel && referredNeighbourLabel != currentNeighbourLabel && !isRoleNeighbourLinkLabelItemCompatibility(currentNeighbourLabel, newNeighbourLabel)) {
											labelCompatible = false;
										} else if (!currentNeighbourLabel && referredNeighbourLabel) {
											labelCompatible = false;
										}

									}

								} else {
									labelCompatible = false;
								}

							}
						}

						if (!labelCompatible) {
							labelAddition = true;
							labelRemoval = false;
							labelReplacement = false;
							linksAddition = true;
							linksRemoval = false;
							linksReplacement = false;
							incompatibleChanges = true;
							DEBUG_HANDLING_STRING(QString("Installing incompatible update data to %1").arg(individualID))
						}
					}




					

					bool repLabelUpdateRequired = false;
					for (cint64 i = 0; associationData && !repLabelUpdateRequired && i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						if ((referredLabels[i] || labelReplacement || labelRemoval) && (!associationData || referredLabels[i] != associationData->getLabelCacheEntry(i))) {
							DEBUG_HANDLING_STRING(QString("Requiring label %1 update for %2").arg(getRepresentativeCacheLabelItemString(i)).arg(individualID))
							repLabelUpdateRequired = true;
						}
					}


					bool statusFlagsUpdateRequired = false;
					if ((tempAssWriteDataLinkerIt->getStatusFlags() != 0 || labelReplacement || labelRemoval) && (!associationData || associationData->getStatusFlags() != tempAssWriteDataLinkerIt->getStatusFlags()) || incompatibleChanges) {
						statusFlagsUpdateRequired = true;
					}



					bool linksUpdateRequired = false;
					if ((tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker() != nullptr || linksReplacement || linksRemoval)) {
						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* existingNeighbourRoleSetHash = nullptr;
						if (associationData) {
							existingNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();
						}
						cint64 updateLinksCount = 0;
						for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker(); !linksUpdateRequired && roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
							CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
							cint64 neighbourId = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference().getIndividualID();
							CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
							if (!referredLabelCacheItem) {
								CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
								if (tmpRefferedLabelItem) {
									referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
								}
							}
							if (linksAddition) {
								if (!existingNeighbourRoleSetHash) {
									linksUpdateRequired = true;
								} else if (existingNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId) != referredLabelCacheItem) {
									linksUpdateRequired = true;
								}
							}
							if (linksRemoval) {
								if (existingNeighbourRoleSetHash && existingNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourId)) {
									linksUpdateRequired = true;
								}
							}
							updateLinksCount++;
						}
						if (!linksUpdateRequired && (updateLinksCount > 0 && !existingNeighbourRoleSetHash || existingNeighbourRoleSetHash && updateLinksCount != existingNeighbourRoleSetHash->getNeighbourCount())) {
							linksUpdateRequired = true;
						}
					}
					QSet<cint64> newCompletingDetSameNeighbours;
					QSet<cint64> newlyCompletedDetSameNeighbours;
					bool completeOnlyCompatibleNeighbourRoleLabels = false;
					if (detSameNeighbourCompletion) {
						CDeterministicSameHandlingInstallationData& installData = mDeterministicSameHandlingInstallationDataHash[individualID];
						CBackendRepresentativeMemoryLabelCacheItem* existDetSameHandlIdLabel = nullptr;
						if (associationData) {
							existDetSameHandlIdLabel = associationData->getDeterministicMergedSameConsideredLabelCacheEntry();
						}
						if (!existDetSameHandlIdLabel) {
							newCompletingDetSameNeighbours = installData.mIdPossibleInstallationSet;
						} else {
							CBackendRepresentativeMemoryLabelCacheItem* newDetSameLabel = referredLabels[CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL];
							for (CBackendRepresentativeMemoryLabelValueLinker* valueLinkerIt = existDetSameHandlIdLabel->getCacheValueLinker(); valueLinkerIt && detSameNeighbourCompletion; valueLinkerIt = valueLinkerIt->getNext()) {
								if (!newDetSameLabel->hasCachedTagValue(valueLinkerIt->getCacheValue().getTag())) {
									detSameNeighbourCompletion = false;
									DEBUG_HANDLING_STRING(QString("Previous deterministic same handled individual %1 for %2 not newly suggested, installation update not possible").arg(valueLinkerIt->getCacheValue().getTag()).arg(individualID))
								}
							}

							for (cint64 possDetSameInstallId : installData.mIdPossibleInstallationSet) {
								if (!existDetSameHandlIdLabel->hasCachedTagValue(possDetSameInstallId)) {
									newCompletingDetSameNeighbours.insert(possDetSameInstallId);
								}
							}
						}
						if (!newCompletingDetSameNeighbours.isEmpty()) {

							cint64 unchangedDetMergedUpdateCount = 0;
							CBackendRepresentativeMemoryCacheIndividualAssociationData* prevAssociationData = associationData;
							while (prevAssociationData && prevAssociationData->getRepresentativeSameIndividualId() == tempAssWriteDataLinkerIt->getRepresentativeSameIndividualId() && unchangedDetMergedUpdateCount < mConfUnchangedDeterministicSameMergeUpdatesForDeterministicSameNeighbourCompletion) {
								++unchangedDetMergedUpdateCount;
								prevAssociationData = prevAssociationData->getPreviousData();
							}

							if (!incompatibleChanges && newCompletingDetSameNeighbours.size() < mConfMinRequiredDeterministicSameMergedHandledInstallationPossiblitiesForNeighbourCompletion && unchangedDetMergedUpdateCount < mConfUnchangedDeterministicSameMergeUpdatesForDeterministicSameNeighbourCompletion) {
								incompatibleChanges = true;
								statusFlagsUpdateRequired = true;
								detSameNeighbourCompletion = false;
							} else {
								if (incompatibleChanges && (!mConfInstallingDeterministicSameHandlingLargeDifferenceReached || installData.mIdFirstPossibleInstallationSet.size() < mConfInstallingDeterministicSameHandlingLargeDifference)) {
									detSameNeighbourCompletion = false;
								} else {
									DEBUG_HANDLING_STRING(QString("Scheduling installation of %1 newly deterministic same handled individuals for %2").arg(installData.mIdFirstPossibleInstallationSet.size()).arg(individualID))
									linksUpdateRequired = true;
								}
								if (incompatibleChanges && detSameNeighbourCompletion) {
									completeOnlyCompatibleNeighbourRoleLabels = true;
								}
							}
						} else {
							detSameNeighbourCompletion = false;
						}
					}


					bool integrationUpdated = false;
					if (!associationData || (associationData && (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration() && !associationData->hasIndirectlyConnectedIndividualIntegration() || 
																tempAssWriteDataLinkerIt->isIndirectlyConnectedNominalIndividual() && !associationData->isIndirectlyConnectedNominalIndividual() ||
																integratedIndirectlyConnectedIndividualsChangeId > 0 && integratedIndirectlyConnectedIndividualsChangeId != associationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId()))) {
						integrationUpdated = true;
					}


					if (!associationData || repLabelUpdateRequired || statusFlagsUpdateRequired || linksUpdateRequired || integrationUpdated) {

						++mUpdatedIndiCount;
						++mAssociationUpdatedIndiCount;


						CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = nullptr;
						locAssociationData = createLocalizedIndividualAssociationData(individualID, associationData, ontologyData, true);
						bool requiesDataCopying = false;
						CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locAssociationData, ontologyData, &requiesDataCopying);


						if (propCut) {
							locAssociationData->setLastPropagationCuttingUpdateId(locAssociationData->getAssociationDataUpdateId());
						}

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						locAssociationData->mDebugUpdateFunction = QString("installAssociationUpdate");
						if (!locAssociationData->mDebugIndi) {
							locAssociationData->mDebugIndi = individual;
						}
						locAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
						locAssociationData->mDebugNeighbourCompletionRequested = tempAssWriteDataLinkerIt->requireSameAsNeighboursCompletion();
#endif
						DEBUG_HANDLING_STRING(QString("Updating data for %1").arg(individualID))

						locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
						ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

						locAssociationData->setRepresentativeSameIndividualId(qMin(tempAssWriteDataLinkerIt->getRepresentativeSameIndividualId(), locAssociationData->getRepresentativeSameIndividualId()));

						bool sameIndiMergedChanged = false;
						if (locAssociationData->getRepresentativeSameIndividualId() != locAssociationData->getAssociatedIndividualId()) {
							ontologyData->setSameIndividualsMergings(true);
							if (!associationData || !associationData->hasRepresentativeSameIndividualMerging()) {
								mStatDetSameRepresentativeMergingCount++;
								ontologyData->incIndividualAssociationMergingCount();
							}
						}
						if (!associationData || locAssociationData->getRepresentativeSameIndividualId() != associationData->getRepresentativeSameIndividualId()) {
							sameIndiMergedChanged = true;
						}

						if (incompatibleChanges && locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) && locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) != referredLabels[CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL]) {
							DEBUG_HANDLING_STRING(QString("Remembering deterministic same mergings required for %1").arg(locAssociationData->getAssociatedIndividualId()))
							mTmpDetSameMergingCompletionReferenceHash.insert(locAssociationData->getAssociatedIndividualId(), locAssociationData->getAssociatedIndividualId());
						}

						bool labelsUpdated = false;
						if (!associationData || repLabelUpdateRequired) {

							DEBUG_HANDLING_STRING(QString("Updating labels for %1").arg(individualID))
							for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
								if (referredLabels[i] != locAssociationData->getLabelCacheEntry(i)) {
									if (referredLabels[i] || labelReplacement || labelRemoval) {
										if (locAssociationData->getLabelCacheEntry(i) && labelAddition) {
											CBackendRepresentativeMemoryLabelCacheItem* mergedLabel = getAdditionMergedLabel(i, referredLabels[i], locAssociationData->getLabelCacheEntry(i), ontologyData);
											if (mergedLabel != locAssociationData->getLabelCacheEntry(i)) {
												DEBUG_HANDLING_STRING(QString("Replacing label %1 for %4 with %5 (merged from %2 and %3)").arg(getRepresentativeCacheLabelItemString(i)).arg(locAssociationData->getLabelCacheEntry(i) ? locAssociationData->getLabelCacheEntry(i)->getCacheEntryID() : 0).arg(referredLabels[i] ? referredLabels[i]->getCacheEntryID() : 0).arg(locAssociationData->getAssociatedIndividualId()).arg(mergedLabel->getCacheEntryID()))
												locAssociationData->setLabelCacheEntry(i, mergedLabel);
											}
										} else {
											DEBUG_HANDLING_STRING(QString("Replacing label %1 for %4 with %2 from %3").arg(getRepresentativeCacheLabelItemString(i)).arg(locAssociationData->getLabelCacheEntry(i) ? locAssociationData->getLabelCacheEntry(i)->getCacheEntryID() : 0).arg(referredLabels[i] ? referredLabels[i]->getCacheEntryID() : 0).arg(locAssociationData->getAssociatedIndividualId()))
											locAssociationData->setLabelCacheEntry(i, referredLabels[i]);
										}
									}
								}
								updateIndexedAssociationCount(locAssociationData, associationData, i, ontologyData);
							}
							labelsUpdated = true;

							associationsUpdated = true;
							if (associationData) {
								CBackendRepresentativeMemoryLabelCacheItem* indConnNomLabelItem = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
								if (indConnNomLabelItem) {
									mTmpIndiIndirectlyConnNomLabelItemHash.insert(individualID, indConnNomLabelItem);
								}
							}
						} else if (sameIndiMergedChanged) {
							for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
								bool exactIndiAssocTracking = requiresIndividualAssociations(i);
								if (exactIndiAssocTracking && !mConfLateIndividualLabelAssociationIndexing) {
									CBackendRepresentativeMemoryLabelCacheItem* labelItem = locAssociationData->getLabelCacheEntry(i);
									CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(labelItem, ontologyData);
									indiAssoExtData->removeIndividualIdAssociation(locAssociationData);
									indiAssoExtData->addIndividualIdAssociation(locAssociationData);
								}
							}
						}

						if (integrationUpdated) {
							if (integratedIndirectlyConnectedIndividualsChangeId > 0) {
								locAssociationData->setLastIntegratedIndirectlyConnectedIndividualsChangeId(integratedIndirectlyConnectedIndividualsChangeId);
							}
							if (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration()) {
								locAssociationData->setIndirectlyConnectedIndividualIntegration(true);
							}
							if (tempAssWriteDataLinkerIt->isIndirectlyConnectedNominalIndividual()) {
								locAssociationData->setIndirectlyConnectedNominalIndividual(true);
							}
							associationsUpdated = true;
						}

						if (statusFlagsUpdateRequired) {
							locAssociationData->setStatusFlags(tempAssWriteDataLinkerIt->getStatusFlags());
							associationsUpdated = true;
						}


						CBackendRepresentativeMemoryLabelCacheItem* newNeighbourRoleSetCompLabel = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
						CBackendRepresentativeMemoryLabelCacheItem* prevNeighbourRoleSetCompLabel = nullptr;
						if (associationData) {
							prevNeighbourRoleSetCompLabel = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
						}

						if (!linksUpdateRequired && prevNeighbourRoleSetCompLabel && newNeighbourRoleSetCompLabel != prevNeighbourRoleSetCompLabel) {
							linksUpdateRequired = true;
							associationsUpdated = true;
						}


						QSet< cint64 >* updateOrRemovalNeighbourIndiIdSet = nullptr;
						bool needsNeighCompLabelCleanUp = false;
						bool neighbourRoleSetCompLabelReductionRequired = false;
						bool neighbourRoleSetCompLabelReductionChecking = false;
						bool propCutReadding = false;
						QSet<cint64>* propCutReaddingArrayIds = nullptr;
						QSet<cint64>* propCutRemovingArrayIds = nullptr;
						bool propCutRemovedNeighbourIndiLinkerCopied = false;
						QHash<cint64,cint64>* propCutArrayNewPrevReductionIdHash = nullptr;
						bool hasProblematicLeveledNeighbours = locAssociationData->hasProblematicLeveledNeigbour();
						//QHash< cint64, cint64 >* idProblematicLeveledNeighboursHash = nullptr;
						QSet< cint64 >** idProblematicLeveledNeighboursSetArray = nullptr;
						// update neighbour data
						bool linksUpdated = false;
						if (linksUpdateRequired) {
							linksUpdated = true;
							DEBUG_HANDLING_STRING(QString("Updating links for %1").arg(individualID))
							CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem = ontologyData->getPrioritizedPropagationMarkedNeighbourLabelItem();
							CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinker = tempAssWriteDataLinkerIt->getRoleSetNeighbourUpdateDataLinker();
							// create role set neighbour array and neighbour role set hash, if addition, then copy previous data

							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* newArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(newNeighbourRoleSetCompLabel, ontologyData);

							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* prevNeighbourRoleSetHash = nullptr;
							if (associationData) {
								prevNeighbourRoleSetHash = associationData->getNeighbourRoleSetHash();
							}

							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* prevArray = nullptr;
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = nullptr;
							if (associationData) {
								prevArray = associationData->getRoleSetNeighbourArray();
							}
							if (prevArray) {
								prevArrayIndexData = prevArray->getIndexData();
							}

							if ((linksRemoval || linksReplacement || tempAssWriteDataLinkerIt->requireSameAsNeighboursCompletion() && !detSameNeighbourCompletion) && prevArray) {
								updateOrRemovalNeighbourIndiIdSet = new QSet< cint64 >();
								++mStatUpdatedOrRemovedNeighbourLinksAssociationUpdateCount;
							} else {
								++mStatAddingNeighbourLinksAssociationUpdateCount;
							}

							if (updateOrRemovalNeighbourIndiIdSet && mTmpPropCutIndiArrayNeighboursHandlingDataHash) {
								CPropagationCutNeighbourArrayHandlingData* handlingData = mTmpPropCutIndiArrayNeighboursHandlingDataHash->value(individualID);
								if (handlingData) {
									propCutReadding = true;
									if (!handlingData->mReaddingArrayPosSet.isEmpty()) {
										propCutReaddingArrayIds = &handlingData->mReaddingArrayPosSet;
									}
									if (!handlingData->mReductionArrayPosSet.isEmpty()) {

										for (cint64 i : handlingData->mReductionArrayPosSet) {
											 CBackendRepresentativeMemoryLabelCacheItem* prevNeighLabelItem = prevArrayIndexData->getNeighbourRoleSetLabel(i);
											 CBackendRepresentativeMemoryLabelCacheItem* reducedNeighLabelItem = getReducedLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL, prevNeighLabelItem, [&](const CCacheValue& cacheValue)->bool {
												 if (isCacheValueRoleNondeterministic(cacheValue)) {
													 return true;
												 } else {
													 return false;
												 }
											 }, ontologyData);

											 cint64 newIndex = newArrayIndexData->getIndex(reducedNeighLabelItem);
											 //KONCLUDE_ASSERT_X(newIndex < 0, "BackendCache", "reduction label not available");
											 if (newIndex >= 0) {
												 if (!propCutArrayNewPrevReductionIdHash) {
													 propCutArrayNewPrevReductionIdHash = new QHash<cint64, cint64>();
												 }
												 propCutArrayNewPrevReductionIdHash->insertMulti(newIndex, i);
											 }
										}

									}
									if (!handlingData->mRemovalArrayPosSet.isEmpty()) {
										propCutRemovingArrayIds = &handlingData->mRemovalArrayPosSet;
									}

								}
							}


							for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinker; roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
								CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
								CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
								if (!referredLabelCacheItem) {
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
									if (tmpRefferedLabelItem) {
										referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
									}
								}
								if (referredLabelCacheItem && prevNeighbourRoleSetHash) {
									if (!updateOrRemovalNeighbourIndiIdSet && prevNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID())) {
										updateOrRemovalNeighbourIndiIdSet = new QSet< cint64 >();
										++mStatUpdatedOrRemovedNeighbourLinksAssociationUpdateCount;
										--mStatAddingNeighbourLinksAssociationUpdateCount;
									} else if (!hasProblematicLeveledNeighbours && ontologyData->hasInvolvedIndividuals()) {
										CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiRef.getIndividualID()];
										if (neighbourAssociationData && neighbourAssociationData->hasProblematicLevel()) {
											locAssociationData->setProblematicLeveledNeigbour(true);
											hasProblematicLeveledNeighbours = true;
										}
									}
								}
							}
							cint64 prevPrioPropMarkIndex = -1;
							cint64 newPrioPropMarkIndex = -1;
							bool readdingPrioPropMarkNeighbours = false;
							if (prevNeighbourRoleSetCompLabel && prevArray && propMarkLabelItem && linksAddition && incompatibleChanges) {
								prevPrioPropMarkIndex = prevArrayIndexData->getIndex(propMarkLabelItem);
								if (prevPrioPropMarkIndex >= 0 && prevArray->at(prevPrioPropMarkIndex).getIndividualCount() > 0) {
									readdingPrioPropMarkNeighbours = true;
								}
							}
							if (readdingPrioPropMarkNeighbours) {
								newPrioPropMarkIndex = newArrayIndexData->getIndex(propMarkLabelItem);
								if (newPrioPropMarkIndex < 0) {
									CCacheValue extendingCacheValue;
									extendingCacheValue.initCacheValue(propMarkLabelItem->getCacheEntryID(), (cint64)propMarkLabelItem, CCacheValue::CACHE_VALUE_TAG_AND_ENTRY);
									CBackendRepresentativeMemoryLabelCacheItem* prevNewNeighbourRoleSetCompLabel = newNeighbourRoleSetCompLabel;
									newNeighbourRoleSetCompLabel = getExtendedLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, prevNewNeighbourRoleSetCompLabel, extendingCacheValue, ontologyData);
									locAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, newNeighbourRoleSetCompLabel);
									updateIndexedAssociationCount(locAssociationData, prevNewNeighbourRoleSetCompLabel, CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, ontologyData);
									newArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(newNeighbourRoleSetCompLabel, ontologyData);
									newPrioPropMarkIndex = newArrayIndexData->getIndex(propMarkLabelItem);
								}
							}


							if (hasProblematicLeveledNeighbours) {
								//idProblematicLeveledNeighboursHash = new QHash< cint64, cint64 >();
								idProblematicLeveledNeighboursSetArray = new QSet<cint64>*[newArrayIndexData->getArraySize()];
								for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
									idProblematicLeveledNeighboursSetArray[i] = new QSet<cint64>();
								}
							}


							if (prevNeighbourRoleSetCompLabel == newNeighbourRoleSetCompLabel && prevArray && linksAddition && !updateOrRemovalNeighbourIndiIdSet) {

								newArray->initNeighbourArray(prevArray);
								newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash, requiesDataCopying);
								if (requiesDataCopying) {
									for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(i);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* firstNewIndiIdLinker = copyNeighbourIndividualIdLinkers(neighbourData.getIndividualIdLinker(), context, locAssociationData, i);
										neighbourData.setIndividualIdLinker(firstNewIndiIdLinker, false);
									}
								}

								if (hasProblematicLeveledNeighbours) {
									for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
										if (i != newPrioPropMarkIndex) {
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(i);
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker = neighbourData.getIndividualIdLinker();
											for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinkerIt = indiIdLinker; indiIdLinkerIt; indiIdLinkerIt = indiIdLinkerIt->getNext()) {
												cint64 neighbourId = indiIdLinkerIt->getIndividualId();
												CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourId];
												if (neighbourAssociationData->hasProblematicLevel()) {
													//idProblematicLeveledNeighboursHash->insertMulti(i, neighbourId);
													idProblematicLeveledNeighboursSetArray[i]->insert(neighbourId);
													indiIdLinker = indiIdLinkerIt->getNext();
												} else {
													break;
												}
											}
											neighbourData.setIndividualIdLinker(indiIdLinker, false);
										}
									}
								}


							} else {

								newArray->initNeighbourArray(newArrayIndexData);
								// copy data from previous array
								if (linksAddition && prevArray && !updateOrRemovalNeighbourIndiIdSet) {
									CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArray->getIndexData();
									for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
										cint64 prevIndex = prevArrayIndexData->getIndex(newArrayIndexData->getNeighbourRoleSetLabel(i));
										if (prevIndex >= 0 && i != newPrioPropMarkIndex) {
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(i);
											neighbourData = prevArray->at(prevIndex);
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker = neighbourData.getIndividualIdLinker();
											if (hasProblematicLeveledNeighbours) {
												for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinkerIt = indiIdLinker; indiIdLinkerIt; indiIdLinkerIt = indiIdLinkerIt->getNext()) {
													cint64 neighbourId = indiIdLinkerIt->getIndividualId();
													CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourId];
													if (neighbourAssociationData->hasProblematicLevel()) {
														//idProblematicLeveledNeighboursHash->insertMulti(i, neighbourId);
														idProblematicLeveledNeighboursSetArray[i]->insert(neighbourId);
														indiIdLinker = indiIdLinkerIt->getNext();
													} else {
														break;
													}
												}
											}
											if (requiesDataCopying) {
												indiIdLinker = copyNeighbourIndividualIdLinkers(indiIdLinker, context, locAssociationData, i);
											}
											neighbourData.setIndividualIdLinker(indiIdLinker, false);
										}
									}
									newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash, requiesDataCopying);
								} else {
									if (!linksReplacement) {
										newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevNeighbourRoleSetHash, requiesDataCopying);
									} else {
										newNeighbourRoleSetHash->initNeighbourRoleSetHash(nullptr);
									}
								}
							}

							QHash<cint64,cint64> updatedPrevArrayPosCountHash;
							// add/update new data, TODO: first add non-problematic leveled neighbours
							for (CBackendRepresentativeMemoryCacheTemporaryIndividualRoleSetNeighbourUpdateDataLinker* roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinker; roleSetNeighbourUpdateDataLinkerIt; roleSetNeighbourUpdateDataLinkerIt = roleSetNeighbourUpdateDataLinkerIt->getNext()) {
								CIndividualReference neighbourIndiRef = roleSetNeighbourUpdateDataLinkerIt->getNeighbourIndividualReference();
								CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredLabelData();
								if (!referredLabelCacheItem) {
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tmpRefferedLabelItem = roleSetNeighbourUpdateDataLinkerIt->getNeighbourRoleInstantiatedCompinationLabelReference().getReferredTemporaryLabelData();
									if (tmpRefferedLabelItem) {
										referredLabelCacheItem = (CBackendRepresentativeMemoryLabelCacheItem*)tmpRefferedLabelItem->getTemporaryData();
									}
								}
								if (referredLabelCacheItem) {
									cint64 index = newArrayIndexData->getIndex(referredLabelCacheItem);

									CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = nullptr;
									if (hasProblematicLeveledNeighbours) {
										neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiRef.getIndividualID()];
									}
									if (hasProblematicLeveledNeighbours && neighbourAssociationData && neighbourAssociationData->hasProblematicLevel()) {
										cint64 prevIndex = newArrayIndexData->getIndex(newNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID()));
										if (prevIndex >= 0) {
											idProblematicLeveledNeighboursSetArray[prevIndex]->remove(neighbourIndiRef.getIndividualID());
											newArray->at(prevIndex).decIndividualCount();
										}
										//idProblematicLeveledNeighboursHash->insertMulti(index, neighbourIndiRef.getIndividualID());
										idProblematicLeveledNeighboursSetArray[index]->insert(neighbourIndiRef.getIndividualID());
										newArray->at(index).incIndividualCount();


									} else {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
										++mStatCreatedNeighbourLinks;
										newLinker->initIndividualIdLinker(neighbourIndiRef.getIndividualID());
										newArray->at(index).addIndividualIdLinker(newLinker);
									}
									newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID(), referredLabelCacheItem);
									if (updateOrRemovalNeighbourIndiIdSet) {
										cint64 prevIndex = -1;
										if (prevArrayIndexData && prevNeighbourRoleSetHash) {
											prevIndex = prevArrayIndexData->getIndex(prevNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiRef.getIndividualID()));
											if (prevIndex >= 0) {
												updatedPrevArrayPosCountHash[prevIndex]++;
											}
										}
										updateOrRemovalNeighbourIndiIdSet->insert(neighbourIndiRef.getIndividualID());
									}
								}
							}

							// copy previous data
							if (updateOrRemovalNeighbourIndiIdSet && (!linksReplacement || propCutReadding)) {

								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArray->getIndexData();
								for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
									CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = newArrayIndexData->getNeighbourRoleSetLabel(i);
									cint64 prevIndex = prevArrayIndexData->getIndex(referredLabelCacheItem);
									if (prevIndex >= 0 && i != newPrioPropMarkIndex && (!propCutReadding || propCutReaddingArrayIds->contains(prevIndex))) {
										bool firstNonProblematicNeighbourIndiFound = false;
										cint64 remainingPrevUpdatedRemovedCount = updatedPrevArrayPosCountHash.value(prevIndex);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& prevNeighbourData = prevArray->at(prevIndex);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& newNeighbourData = newArray->at(i);
										cint64 remainingNeighbourCount = prevNeighbourData.getIndividualCount();
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* addingNeighbourLinker = newNeighbourData.getIndividualIdLinker();
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* lastAddingNeighbourLinker = nullptr;
										if (newNeighbourData.getIndividualIdLinker()) {
											lastAddingNeighbourLinker = newNeighbourData.getIndividualIdLinker()->getLastListLink();
										}
										cint64 addingNeighbourCount = 0;
										for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourLinkerIt = prevNeighbourData.getIndividualIdLinker(); neighbourLinkerIt; neighbourLinkerIt = neighbourLinkerIt->getNext()) {
											cint64 neighbourIndiId = neighbourLinkerIt->getIndividualId();

											if (!updateOrRemovalNeighbourIndiIdSet->contains(neighbourIndiId)) {

												newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiId, referredLabelCacheItem);
												CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = nullptr;
												if (!firstNonProblematicNeighbourIndiFound && hasProblematicLeveledNeighbours) {
													neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];
												}
												if (!firstNonProblematicNeighbourIndiFound && hasProblematicLeveledNeighbours && neighbourAssociationData && neighbourAssociationData->hasProblematicLevel()) {
													idProblematicLeveledNeighboursSetArray[i]->insert(neighbourIndiId);
													//idProblematicLeveledNeighboursHash->insertMulti(i, neighbourIndiId);
													addingNeighbourCount++;
													--remainingNeighbourCount;
												} else {
													firstNonProblematicNeighbourIndiFound = true;

													if (remainingPrevUpdatedRemovedCount <= 0) {
														CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* readdingNeighbourLinker = neighbourLinkerIt;
														if (propCutReadding) {
															for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* readdingNeighbourLinkerIt = readdingNeighbourLinker; readdingNeighbourLinkerIt; readdingNeighbourLinkerIt = readdingNeighbourLinkerIt->getNext()) {
																cint64 neighbourIndiId = readdingNeighbourLinkerIt->getIndividualId();
																newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiId, referredLabelCacheItem);
															}
														}
														if (requiesDataCopying) {
															readdingNeighbourLinker = copyNeighbourIndividualIdLinkers(readdingNeighbourLinker, context, locAssociationData, i);
														}
														if (!addingNeighbourLinker) {
															addingNeighbourLinker = readdingNeighbourLinker;
														} else {
															lastAddingNeighbourLinker->setNext(readdingNeighbourLinker);
														}
														addingNeighbourCount += remainingNeighbourCount;
														remainingNeighbourCount = 0;
														break;
													} else {
														--remainingNeighbourCount;
														CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
														++mStatCreatedNeighbourLinks;
														newLinker->initIndividualIdLinker(neighbourIndiId);
														addingNeighbourCount++;
														if (!addingNeighbourLinker) {
															lastAddingNeighbourLinker = addingNeighbourLinker = newLinker;
														} else {
															lastAddingNeighbourLinker->setNext(newLinker);
															lastAddingNeighbourLinker = newLinker;
														}
													}
												}
											} else {
												remainingPrevUpdatedRemovedCount--;
												--remainingNeighbourCount;
											}

											//prevArray->at(prevIndex).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {
											//	if (!updateOrRemovalNeighbourIndiIdSet->contains(neighbourIndiId)) {

											//		newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiId, referredLabelCacheItem);
											//		CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = nullptr;
											//		if (!firstNonProblematicNeighbourIndiFound && hasProblematicLeveledNeighbours) {
											//			neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];
											//		}
											//		if (!firstNonProblematicNeighbourIndiFound && hasProblematicLeveledNeighbours && neighbourAssociationData && neighbourAssociationData->hasProblematicLevel()) {
											//			idProblematicLeveledNeighboursSetArray[i]->insert(neighbourIndiId);
											//			//idProblematicLeveledNeighboursHash->insertMulti(i, neighbourIndiId);
											//			newArray->at(i).incIndividualCount();
											//		} else {
											//			firstNonProblematicNeighbourIndiFound = true;
											//			CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
											//			++mStatCreatedNeighbourLinks;
											//			newLinker->initIndividualIdLinker(neighbourIndiId);
											//			newArray->at(i).addIndividualIdLinker(newLinker);
											//		}
											//	}
											//	return true;
											//});
										}
										newNeighbourData.setIndividualIdLinker(addingNeighbourLinker, false);
										newNeighbourData.incIndividualCount(addingNeighbourCount);

									}

								}

								if (propCutArrayNewPrevReductionIdHash) {

									for (QHash<cint64, cint64>::const_iterator it = propCutArrayNewPrevReductionIdHash->constBegin(), itEnd = propCutArrayNewPrevReductionIdHash->constEnd(); it != itEnd;  ++it) {
										cint64 newArrayPos = it.key();
										cint64 prevIndex = it.value();
										if (prevIndex >= 0 && newArrayPos >= 0) {
											CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = newArrayIndexData->getNeighbourRoleSetLabel(newArrayPos);
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& newNeighbourData = newArray->at(newArrayPos);
											CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& prevNeighbourData = prevArray->at(prevIndex);
											if (newNeighbourData.getIndividualCount() <= 0) {
												newNeighbourData = prevNeighbourData;
											} else {
												bool firstNonProblematicNeighbourIndiFound = false;
												for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourLinkerIt = prevNeighbourData.getIndividualIdLinker(); neighbourLinkerIt; neighbourLinkerIt = neighbourLinkerIt->getNext()) {
													cint64 neighbourIndiId = neighbourLinkerIt->getIndividualId();
													if (!updateOrRemovalNeighbourIndiIdSet->contains(neighbourIndiId)) {
														newNeighbourRoleSetHash->setNeighbourRoleSetLabel(neighbourIndiId, referredLabelCacheItem);
														CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];

														if (!firstNonProblematicNeighbourIndiFound && hasProblematicLeveledNeighbours && neighbourAssociationData && neighbourAssociationData->hasProblematicLevel()) {
															idProblematicLeveledNeighboursSetArray[newArrayPos]->insert(neighbourIndiId);
															newNeighbourData.incIndividualCount();
														} else {
															firstNonProblematicNeighbourIndiFound = true;
															CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
															++mStatCreatedNeighbourLinks;
															newLinker->initIndividualIdLinker(neighbourIndiId);
															newNeighbourData.addIndividualIdLinker(newLinker);
														}
													}
												}

											}
										}
									}
								}


								//if (propCutReaddingArrayIds) {
								//	delete propCutReaddingArrayIds;
								//}
								if (propCutArrayNewPrevReductionIdHash) {
									delete propCutArrayNewPrevReductionIdHash;
								}
								delete updateOrRemovalNeighbourIndiIdSet;

								if (propCutRemovingArrayIds) {
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* prevRemNeighIndiLinker = locAssociationData->getPropagationCutRemovedNeighbourIndividualLinker();
									if (requiesDataCopying) {
										propCutRemovedNeighbourIndiLinkerCopied = true;
										prevRemNeighIndiLinker = copyNeighbourIndividualIdLinkers(prevRemNeighIndiLinker, context, locAssociationData, -1);
									}
									QSet<cint64> prevRemNeighIndiSet;

									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newRemNeighIndiLinker = nullptr;
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLastRemNeighIndiLinker = nullptr;
									for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourLinkerIt = prevRemNeighIndiLinker; neighbourLinkerIt; ) {
										cint64 neighbourIndiId = neighbourLinkerIt->getIndividualId();
										CBackendRepresentativeMemoryLabelCacheItem* neighbourLabelItem = newNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId);
										if (!neighbourLabelItem) {
											prevRemNeighIndiSet.insert(neighbourIndiId);
											if (!newLastRemNeighIndiLinker) {
												newLastRemNeighIndiLinker = newRemNeighIndiLinker = neighbourLinkerIt;
											} else {
												newLastRemNeighIndiLinker->setNext(neighbourLinkerIt);
												newLastRemNeighIndiLinker = neighbourLinkerIt;
											}
											neighbourLinkerIt = neighbourLinkerIt->getNext();
											if (newLastRemNeighIndiLinker) {
												newLastRemNeighIndiLinker->setNext(nullptr);
											}
										} else {
											neighbourLinkerIt = neighbourLinkerIt->getNext();
										}
									}


									for (cint64 prevIndex : *propCutRemovingArrayIds) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& prevNeighbourData = prevArray->at(prevIndex);

										for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourLinkerIt = prevNeighbourData.getIndividualIdLinker(); neighbourLinkerIt; neighbourLinkerIt = neighbourLinkerIt->getNext()) {
											cint64 neighbourIndiId = neighbourLinkerIt->getIndividualId();
											CBackendRepresentativeMemoryLabelCacheItem* neighbourLabelItem = newNeighbourRoleSetHash->getNeighbourRoleSetLabel(neighbourIndiId);
											if (!prevRemNeighIndiSet.contains(neighbourIndiId) && !neighbourLabelItem) {
												CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
												++mStatCreatedNeighbourLinks;
												newLinker->initIndividualIdLinker(neighbourIndiId);
												newRemNeighIndiLinker = newLinker->append(newRemNeighIndiLinker);
											}
										}
									}
									locAssociationData->setPropagationCutRemovedNeighbourIndividualLinker(newRemNeighIndiLinker);
								}


								if (readdingPrioPropMarkNeighbours) {
									newArray->at(newPrioPropMarkIndex) = prevArray->at(prevPrioPropMarkIndex);
									if (requiesDataCopying) {
										newArray->at(newPrioPropMarkIndex).setIndividualIdLinker(copyNeighbourIndividualIdLinkers(newArray->at(newPrioPropMarkIndex).getIndividualIdLinker(), context, locAssociationData, newPrioPropMarkIndex), false);
									}
								}


								if (detSameNeighbourCompletion) {
									locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
									locAssociationData->setRoleSetNeighbourArray(newArray);
									if (completeSameAsNeighbours(locAssociationData, newCompletingDetSameNeighbours, newlyCompletedDetSameNeighbours, completeOnlyCompatibleNeighbourRoleLabels, ontologyData)) {
										mTmpCompleteNeighbourSameIndiMergingSet.insert(locAssociationData->getAssociatedIndividualId());
									}
								}

								neighbourRoleSetCompLabelReductionChecking = true;


							} else {
								if (detSameNeighbourCompletion) {
									locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
									locAssociationData->setRoleSetNeighbourArray(newArray);
									if (completeSameAsNeighbours(locAssociationData, newCompletingDetSameNeighbours, newlyCompletedDetSameNeighbours, completeOnlyCompatibleNeighbourRoleLabels, ontologyData)) {
										mTmpCompleteNeighbourSameIndiMergingSet.insert(locAssociationData->getAssociatedIndividualId());
									}
								}
								if (incompatibleChanges || detSameNeighbourCompletion) {
									neighbourRoleSetCompLabelReductionChecking = true;
								}

							}




							if (hasProblematicLeveledNeighbours) {
								//for (QHash< cint64, cint64 >::const_iterator idProbNeighIt = idProblematicLeveledNeighboursHash->constBegin(), idProbNeighItEnd = idProblematicLeveledNeighboursHash->constEnd(); idProbNeighIt != idProbNeighItEnd; ++idProbNeighIt) {
								//	cint64 idx = idProbNeighIt.key();
								//	cint64 neighbourId = idProbNeighIt.value();

								for (cint64 idx = 0; idx < newArrayIndexData->getArraySize(); ++idx) {
									for (cint64 neighbourId : *(idProblematicLeveledNeighboursSetArray[idx])) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(idx);

										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
										++mStatCreatedNeighbourLinks;
										newLinker->initIndividualIdLinker(neighbourId);
										neighbourData.addIndividualIdLinker(newLinker, false);
									}
									delete (idProblematicLeveledNeighboursSetArray[idx]);
								}
								//delete idProblematicLeveledNeighboursHash;
								delete[] idProblematicLeveledNeighboursSetArray;
							}


							if (neighbourRoleSetCompLabelReductionChecking) {
								for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
									if (!newArray->at(i).getIndividualIdLinker()) {
										//if (!newArray->at(i).getIndividualIdLinker() && (!idProblematicLeveledNeighboursHash || !idProblematicLeveledNeighboursHash->contains(i))) {
										neighbourRoleSetCompLabelReductionRequired = true;
									}
								}
								if (neighbourRoleSetCompLabelReductionRequired) {
									++mReducedNeighbourArrayCount;
									CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCombinationLabelSet = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);

									CBackendRepresentativeMemoryLabelCacheItem* reducedNeighbourRoleSetCombinationLabelSet = getReducedLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, neighbourRoleSetCombinationLabelSet, [&](const CCacheValue& cacheValue)->bool {
										CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabel = (CBackendRepresentativeMemoryLabelCacheItem*)cacheValue.getIdentification();
										cint64 index = newArrayIndexData->getIndex(neighbourRoleSetLabel);
										if (index < 0) {
											return true;
										}
										//if (!newArray->at(index).getIndividualIdLinker() && (!idProblematicLeveledNeighboursHash || !idProblematicLeveledNeighboursHash->contains(index))) {
										if (!newArray->at(index).getIndividualIdLinker()) {
											return true;
										}
										return false;
									}, ontologyData);

									if (neighbourRoleSetCombinationLabelSet != reducedNeighbourRoleSetCombinationLabelSet) {
										locAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, reducedNeighbourRoleSetCombinationLabelSet);
										updateIndexedAssociationCount(locAssociationData, neighbourRoleSetCombinationLabelSet, CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, ontologyData);


										CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* reducedArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(reducedNeighbourRoleSetCombinationLabelSet, ontologyData);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* reducedArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
										reducedArray->initNeighbourArray(reducedArrayIndexData);
										// copy data into reduced array
										for (cint64 i = 0; i < reducedArrayIndexData->getArraySize(); ++i) {
											cint64 prevIndex = newArrayIndexData->getIndex(reducedArrayIndexData->getNeighbourRoleSetLabel(i));
											if (prevIndex >= 0) {
												reducedArray->at(i) = newArray->at(prevIndex);
											}
										}
										newArray = reducedArray;
										newArrayIndexData = reducedArrayIndexData;

									}

								}
							}



							locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
							locAssociationData->setRoleSetNeighbourArray(newArray);
							associationsUpdated = true;


						} else if (associationData && associationData->getNeighbourRoleSetHash()) {

							if (detSameNeighbourCompletion) {
								linksUpdated = true;
								CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								newArray->initNeighbourArray(associationData->getRoleSetNeighbourArray());
								newNeighbourRoleSetHash->initNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash(), requiesDataCopying);
								if (requiesDataCopying) {
									CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = newArray->getIndexData();
									for (cint64 i = 0; i < arrayIndexData->getArraySize(); ++i) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(i);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* firstNewIndiIdLinker = copyNeighbourIndividualIdLinkers(neighbourData.getIndividualIdLinker(), context, locAssociationData, i);
										neighbourData.setIndividualIdLinker(firstNewIndiIdLinker, false);
									}
								}

								locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
								locAssociationData->setRoleSetNeighbourArray(newArray);
								bool completed = completeSameAsNeighbours(locAssociationData, newCompletingDetSameNeighbours, newlyCompletedDetSameNeighbours, completeOnlyCompatibleNeighbourRoleLabels, ontologyData);
								if (completed || requiesDataCopying) {
									associationsUpdated = true;
									mTmpCompleteNeighbourSameIndiMergingSet.insert(locAssociationData->getAssociatedIndividualId());
								} else {
									locAssociationData->setNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash());
									locAssociationData->setRoleSetNeighbourArray(associationData->getRoleSetNeighbourArray());
								}


							} else {

								if (requiesDataCopying) {
									CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
									newArray->initNeighbourArray(associationData->getRoleSetNeighbourArray());
									newNeighbourRoleSetHash->initNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash(), requiesDataCopying);
									CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = newArray->getIndexData();
									for (cint64 i = 0; i < arrayIndexData->getArraySize(); ++i) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = newArray->at(i);
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* firstNewIndiIdLinker = copyNeighbourIndividualIdLinkers(neighbourData.getIndividualIdLinker(), context, locAssociationData, i);
										neighbourData.setIndividualIdLinker(firstNewIndiIdLinker, false);
									}
									locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
									locAssociationData->setRoleSetNeighbourArray(newArray);

								} else {

									locAssociationData->setNeighbourRoleSetHash(associationData->getNeighbourRoleSetHash());
									locAssociationData->setRoleSetNeighbourArray(associationData->getRoleSetNeighbourArray());
								}

							}
													}



						if (requiesDataCopying && !propCutRemovedNeighbourIndiLinkerCopied) {
							propCutRemovedNeighbourIndiLinkerCopied = true;
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* prevRemNeighIndiLinker = locAssociationData->getPropagationCutRemovedNeighbourIndividualLinker();
							prevRemNeighIndiLinker = copyNeighbourIndividualIdLinkers(prevRemNeighIndiLinker, context, locAssociationData, -1);
							locAssociationData->setPropagationCutRemovedNeighbourIndividualLinker(prevRemNeighIndiLinker);
						}


						CBackendRepresentativeMemoryLabelCacheItem* detSameLabItem = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
						if (detSameLabItem) {
							mStatMaxSameAsMergedCount = qMax(mStatMaxSameAsMergedCount, detSameLabItem->getCacheValueCount());
						}
						if (locAssociationData->getNeighbourRoleSetHash()) {
							mStatMaxNeighbourLinksCount = qMax(mStatMaxNeighbourLinksCount, locAssociationData->getNeighbourRoleSetHash()->getNeighbourCount());
						}
						mStatMaxAssociationUpdateCount = qMax(mStatMaxAssociationUpdateCount, locAssociationData->getAssociationDataUpdateId());


						if (tempAssWriteDataLinkerIt->hasIndirectlyConnectedIndividualIntegration()) {
							CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData*& nomConnData = (*ontologyData->getNominaIIndividualdIndirectConnectionDataHash())[individualID];
							if (nomConnData || integratedIndirectlyConnectedIndividualsChangeId > 0) {
								if (integratedIndirectlyConnectedIndividualsChangeId != nomConnData->getLastChangeId()) {
									locAssociationData->setCompletelyHandled(false);
									associationsUpdated = true;
								}
							}
						}


						if (incompatibleChanges && locAssociationData->isCompletelyHandled() && (labelsUpdated || linksUpdated || tempAssWriteDataLinkerIt->requireSameAsNeighboursCompletion())) {
							// TODO: may optimize by checking whether something has actually been updated, e.g., labels, links, etc., but requires neighbours completion then
							mCheckIncompatibleIndiCount++;
							if (locAssociationData->hasRepresentativeSameIndividualMerging()) {
								markRepresentativeReferencedIndividualAssociationIncompletelyHandled(individualID, locAssociationData, ontologyData);
							} else {
								locAssociationData->setCompletelyHandled(false);
								associationsUpdated = true;
							}
						}

						if (locAssociationData->isCompletelyPropagated() && !tempAssWriteDataLinkerIt->isCompletelyPropagated()) {
							locAssociationData->setCompletelyPropagated(false);
							associationsUpdated = true;
						}


						if (!locAssociationData->isCompletelyHandled() && locAssociationData->hasRepresentativeSameIndividualMerging()) {
							locAssociationData->setCompletelyHandled(true);
							markRepresentativeReferencedIndividualAssociationIncompletelyHandled(individualID, locAssociationData, ontologyData);
							associationsUpdated = true;
						}

						if (detSameNeighbourCompletion) {
							if (referredLabels[CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL] != locAssociationData->getDeterministicMergedSameConsideredLabelCacheEntry()) {

								CBackendRepresentativeMemoryLabelCacheItem* prevDetSameLabel = locAssociationData->getDeterministicMergedSameConsideredLabelCacheEntry();
								CBackendRepresentativeMemoryLabelCacheItem* newDetSameLabel = referredLabels[CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL];
								CBackendRepresentativeMemoryLabelCacheItem* reducedNewDetSameLabel = getReducedLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, newDetSameLabel, [&](const CCacheValue& cacheValue)-> bool {
									cint64 detSameHandledId = cacheValue.getTag();
									if (detSameHandledId == individualID || prevDetSameLabel && prevDetSameLabel->hasCachedTagValue(detSameHandledId) || newlyCompletedDetSameNeighbours.contains(detSameHandledId)) {
										return false;
									} else {
										return true;
									}
								}, ontologyData);
								locAssociationData->setDeterministicMergedSameConsideredLabelCacheEntry(reducedNewDetSameLabel);
								cint64 prevDetSameLabelCacheEntryId = -1;
								if (prevDetSameLabel) {
									prevDetSameLabelCacheEntryId = prevDetSameLabel->getCacheEntryID();
								}
								DEBUG_HANDLING_STRING(QString("Updating deterministically same handled label for %1 to label %2 from label %3").arg(locAssociationData->getAssociatedIndividualId()).arg(locAssociationData->getDeterministicMergedSameConsideredLabelCacheEntry()->getCacheEntryID()).arg(prevDetSameLabelCacheEntryId))
								associationsUpdated = true;
							}
						}



						associationsUpdated |= storeIndividualIncompletelyMarked(locAssociationData, !locAssociationData->isCompletelyHandled(), ontologyData);



						//if (mIncompletelyHandledIndiIdCount != mIncompletelyAssociatedIndividualSet.size()) {
						//	bool debug = true;
						//}
						//if (locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) && locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL)->getCacheValueCount() > 0) {
						//	if (!locAssociationData->getRoleSetNeighbourArray()) {
						//		bool debug = true;
						//	}
						//}


						if (associationsUpdated) {
							if (locAssociationData->getPreviousData() && !locAssociationData->getPreviousData()->getPreviousData()) {
								ontologyData->incIndividualAssociationDataDirectUpdateCount();
							}
							setUpdatedIndividualAssociationData(individualID, locAssociationData, ontologyData);
							if (!locAssociationData->hasRepresentativeSameIndividualMerging()) {
								udateDeterministicSameAssociations(locAssociationData, ontologyData);
							}
						}

						DEBUG_HANDLING_STRING(QString("Finishing updating data for %1").arg(locAssociationData->getAssociatedIndividualId()))

					}
					return associationsUpdated;
				}



































				bool CBackendRepresentativeMemoryCache::updateIndexedAssociationCount(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryLabelCacheItem* prevLabelItem, cint64 i, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;
					bool exactIndiAssocTracking = requiresIndividualAssociations(i);

					CBackendRepresentativeMemoryLabelCacheItem* newLabelItem = locAssociationData->getLabelCacheEntry(i);

					if (prevLabelItem != newLabelItem) {
						if (prevLabelItem) {
							updated = true;
							prevLabelItem->decIndividualAssociationCount();
							if (exactIndiAssocTracking && !mConfLateIndividualLabelAssociationIndexing) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(prevLabelItem, ontologyData);
								indiAssoExtData->removeIndividualIdAssociation(locAssociationData);
							}
						}
						if (newLabelItem) {
							updated = true;
							newLabelItem->incIndividualAssociationCount();
							if (exactIndiAssocTracking && !mConfLateIndividualLabelAssociationIndexing) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(newLabelItem, ontologyData);
								indiAssoExtData->addIndividualIdAssociation(locAssociationData);
							}
						}
					}
					return true;
				}


				bool CBackendRepresentativeMemoryCache::updateIndexedAssociationCount(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 i, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;


					bool exactIndiAssocTracking = requiresIndividualAssociations(i);
					bool sameIndiMergedChanged = false;
					if (!associationData || associationData->getRepresentativeSameIndividualId() != locAssociationData->getRepresentativeSameIndividualId()) {
						sameIndiMergedChanged = true;
					}

					CBackendRepresentativeMemoryLabelCacheItem* prevLabelItem = nullptr;
					CBackendRepresentativeMemoryLabelCacheItem* newLabelItem = locAssociationData->getLabelCacheEntry(i);
					if (associationData) {
						prevLabelItem = associationData->getLabelCacheEntry(i);
					}

					if (prevLabelItem != newLabelItem || sameIndiMergedChanged) {
						if (prevLabelItem) {
							updated = true;
							prevLabelItem->decIndividualAssociationCount();
							if (exactIndiAssocTracking && !mConfLateIndividualLabelAssociationIndexing) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(prevLabelItem, ontologyData);
								indiAssoExtData->removeIndividualIdAssociation(associationData);
							}
						}
						if (newLabelItem) {
							updated = true;
							newLabelItem->incIndividualAssociationCount();
							if (exactIndiAssocTracking && !mConfLateIndividualLabelAssociationIndexing) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = getIndividualAssociationsExtensionData(newLabelItem, ontologyData);
								indiAssoExtData->addIndividualIdAssociation(locAssociationData);
							}
						}
					}
					return true;
				}


				bool CBackendRepresentativeMemoryCache::udateDeterministicSameAssociations(CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;
					DEBUG_HANDLING_STRING(QString("Updating deterministic same references for %1").arg(locNeighbourAssociationData->getAssociatedIndividualId()))
					CBackendRepresentativeMemoryLabelCacheItem* detSameLabItem = locNeighbourAssociationData->getDeterministicMergedSameConsideredLabelCacheEntry();
					if (detSameLabItem) {
						for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = detSameLabItem->getCacheValueLinker(); detSameValueLinkerIt; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
							cint64 detSameNeighbourId = detSameValueLinkerIt->getCacheValue().getTag();

							if (detSameNeighbourId != locNeighbourAssociationData->getAssociatedIndividualId()) {
								CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameNeighbourAssociationData = nullptr;
								if (ontologyData->getIndividualIdAssoiationDataVector() && detSameNeighbourId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
									detSameNeighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[detSameNeighbourId];
								}

								if (detSameNeighbourAssociationData && detSameNeighbourAssociationData->getDeterministicSameIndividualId() == locNeighbourAssociationData->getAssociatedIndividualId()) {
									if (checkRequiresDeterministicSameAsAssociationUpdateInstallation(detSameNeighbourAssociationData, detSameNeighbourId, locNeighbourAssociationData, locNeighbourAssociationData->getAssociatedIndividualId(), ontologyData)) {
										updated |= installDeterministicSameAsAssociationUpdate(detSameNeighbourAssociationData, detSameNeighbourId, locNeighbourAssociationData, locNeighbourAssociationData->getAssociatedIndividualId(), ontologyData);
									}
								}
							}
						}
					}
					return updated;
				}


				bool CBackendRepresentativeMemoryCache::completeSameAsNeighbours(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, QSet<cint64> newCompletingDetSameNeighbours, QSet<cint64>& newlyCompletedDetSameNeighbours, bool completeOnlyCompatibleNeighbourRoleLabels, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool completed = false;
					DEBUG_HANDLING_STRING(QString("Completing links for %1").arg(locAssociationData->getAssociatedIndividualId()))
					cint64 linksCompleted = 0;
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItem* detSameLabItem = locAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetLabelItemHash = locAssociationData->getNeighbourRoleSetHash();
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourArray = locAssociationData->getRoleSetNeighbourArray();
					CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = neighbourArray->getIndexData();
					if (detSameLabItem) {
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						locAssociationData->mDebugNeighbourCompletionPerformed = true;
#endif
						for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = detSameLabItem->getCacheValueLinker(); detSameValueLinkerIt; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
							cint64 detSameId = detSameValueLinkerIt->getCacheValue().getTag();
							if (detSameId != locAssociationData->getAssociatedIndividualId() && newCompletingDetSameNeighbours.contains(detSameId)) {
								CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[detSameId];
								if (!detSameAssociationData->hasDeterministicSameIndividualMerging()) {
									DEBUG_HANDLING_STRING(QString("Trying links completion for %1 with %2").arg(locAssociationData->getAssociatedIndividualId()).arg(detSameId))
									bool completedDetSameId = true;
									bool oneChangedDetSameIdNeighbourLabelCompletion = false;
									bool oneIncompatibleDetSameIdNeighbourLabelCompletion = false;
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* detSameNeighbourArray = detSameAssociationData->getRoleSetNeighbourArray();
									if (detSameNeighbourArray) {
										CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* detSameArrayIndexData = detSameNeighbourArray->getIndexData();
										for (cint64 i = 0; i < detSameArrayIndexData->getArraySize() && completedDetSameId; ++i) {
											CBackendRepresentativeMemoryLabelCacheItem* detSameNeighbourRoleSetLabelItem = detSameArrayIndexData->getNeighbourRoleSetLabel(i);
											cint64 arrayIndex = arrayIndexData->getIndex(detSameNeighbourRoleSetLabelItem);
											if (arrayIndex >= 0) {
												CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourData& neighbourData = neighbourArray->at(arrayIndex);
												CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinker = detSameNeighbourArray->at(i).getIndividualIdLinker();
												for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinkerIt = neighbourIndiLinker; neighbourIndiLinkerIt && completedDetSameId; neighbourIndiLinkerIt = neighbourIndiLinkerIt->getNext()) {
													cint64 neighbourIndiId = neighbourIndiLinkerIt->getIndividualId();
													CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = neighbourRoleSetLabelItemHash->getNeighbourRoleSetLabel(neighbourIndiId);
													if (!neighbourRoleSetLabelItem) {
														neighbourRoleSetLabelItemHash->setNeighbourRoleSetLabel(neighbourIndiId, detSameNeighbourRoleSetLabelItem);
														CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
														++mStatCreatedNeighbourLinks;
														++linksCompleted;
														newLinker->initIndividualIdLinker(neighbourIndiId);
														neighbourData.addIndividualIdLinker(newLinker, true);
													} else if (neighbourRoleSetLabelItem != detSameNeighbourRoleSetLabelItem) {
														oneChangedDetSameIdNeighbourLabelCompletion = true;
														++mStatChangedLabelNeighbourCompletionCount;
														if (completeOnlyCompatibleNeighbourRoleLabels) {
															completedDetSameId = false;
															++mStatIncompatibleLabelNeighbourCompletionCount;
															oneIncompatibleDetSameIdNeighbourLabelCompletion = true;
														}
													}
												}
											}
										}
									}
									if (completedDetSameId) {
										DEBUG_HANDLING_STRING(QString("Links completed for %1 with %2").arg(locAssociationData->getAssociatedIndividualId()).arg(detSameId))
										newlyCompletedDetSameNeighbours.insert(detSameId);
										completed = true;
										++mStatNeighbourCompletionDetSameSuccededCount;
									}
									if (oneChangedDetSameIdNeighbourLabelCompletion) {
										++mStatNeighbourCompletionDetSameChangedCount;
									}
									if (oneIncompatibleDetSameIdNeighbourLabelCompletion) {
										++mStatNeighbourCompletionDetSameIncompatibleCount;
									}
									if (!oneChangedDetSameIdNeighbourLabelCompletion) {
										++mStatNeighbourCompletionDetSameUnchangedCount;
									}

								} else {
									newlyCompletedDetSameNeighbours.insert(detSameId);
								}
							}
						}
					}
					DEBUG_HANDLING_STRING(QString("Completed %2 links for %1").arg(locAssociationData->getAssociatedIndividualId()).arg(linksCompleted))
					return completed;
				}




				bool CBackendRepresentativeMemoryCache::completeDeterministicSameAsMergingInformation(QHash<cint64, cint64>& tmpDetSameMergingCompletionReferenceHash, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;
					cint64 completedDetSameLabelItems = 0;
					cint64 completedDetSameIntegratedIndividuals = 0;
					cint64 completedIndividualUpdates = 0;

					DEBUG_HANDLING_STRING(QString("Starting deterministic same mergings completions for %1 entries").arg(tmpDetSameMergingCompletionReferenceHash.size()))

					QSet<cint64> mergedHandledIndiIdSet;
					for (QHash<cint64, cint64>::const_iterator it = tmpDetSameMergingCompletionReferenceHash.constBegin(), itEnd = tmpDetSameMergingCompletionReferenceHash.constEnd(); it != itEnd; ++it) {
						cint64 refIndiId = it.key();
						cint64 destIndiId = it.value();
						DEBUG_HANDLING_STRING(QString("Completing deterministic same mergings for %1 to %2").arg(refIndiId).arg(destIndiId))
						if (!mergedHandledIndiIdSet.contains(destIndiId)) {
							mergedHandledIndiIdSet.insert(destIndiId);

							++completedDetSameLabelItems;

							CBackendRepresentativeMemoryCacheIndividualAssociationData* refIndiAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[destIndiId];
							CBackendRepresentativeMemoryLabelCacheItem* detSameLabItem = refIndiAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
							if (detSameLabItem) {
								QSet<CBackendRepresentativeMemoryLabelCacheItem*> mergingDetSameLabelItemSet;
								QList<CBackendRepresentativeMemoryLabelCacheItem*> processingDetSameLabelItemList;
								processingDetSameLabelItemList.append(detSameLabItem);
								mergingDetSameLabelItemSet.insert(detSameLabItem);

								cint64 minRepIndiId = destIndiId;

								while (!processingDetSameLabelItemList.isEmpty()) {
									CBackendRepresentativeMemoryLabelCacheItem* procDetSameLabItem = processingDetSameLabelItemList.takeFirst();

									for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = procDetSameLabItem->getCacheValueLinker(); detSameValueLinkerIt; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
										cint64 detSameId = detSameValueLinkerIt->getCacheValue().getTag();
										mergedHandledIndiIdSet.insert(detSameId);
										minRepIndiId = qMin(detSameId, minRepIndiId);
										++completedDetSameIntegratedIndividuals;

										CBackendRepresentativeMemoryCacheIndividualAssociationData* sameIndiAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[detSameId];
										CBackendRepresentativeMemoryLabelCacheItem* otherDetSameLabItem = sameIndiAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
										if (otherDetSameLabItem && otherDetSameLabItem != procDetSameLabItem && !mergingDetSameLabelItemSet.contains(otherDetSameLabItem)) {
											mergingDetSameLabelItemSet.insert(otherDetSameLabItem);
											processingDetSameLabelItemList.append(otherDetSameLabItem);
										}


										cint64 otherDestIndiId = tmpDetSameMergingCompletionReferenceHash.value(detSameId);
										CBackendRepresentativeMemoryCacheIndividualAssociationData* otherDestIndiAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[otherDestIndiId];
										CBackendRepresentativeMemoryLabelCacheItem* otherDestDetSameLabItem = otherDestIndiAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
										if (otherDestDetSameLabItem && otherDestDetSameLabItem != procDetSameLabItem && !mergingDetSameLabelItemSet.contains(otherDestDetSameLabItem)) {
											mergingDetSameLabelItemSet.insert(otherDestDetSameLabItem);
											processingDetSameLabelItemList.append(otherDestDetSameLabItem);
										}
									}
								}



								CBackendRepresentativeMemoryLabelCacheItem* mergedDetSameLabItem = detSameLabItem;
								for (CBackendRepresentativeMemoryLabelCacheItem* mergingLabelItem : mergingDetSameLabelItemSet) {
									if (mergingLabelItem != detSameLabItem) {
										mergedDetSameLabItem = getAdditionMergedLabel(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, mergingLabelItem, mergedDetSameLabItem, ontologyData);
									}
								}

								if (mergedDetSameLabItem) {
									DEBUG_HANDLING_STRING(QString("Found new deterministic same mergings for %1 to %2 stored in label item %3").arg(refIndiId).arg(destIndiId).arg(mergedDetSameLabItem->getCacheEntryID()))
								}
								
								for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = mergedDetSameLabItem->getCacheValueLinker(); detSameValueLinkerIt; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
									cint64 detSameId = detSameValueLinkerIt->getCacheValue().getTag();
									CBackendRepresentativeMemoryCacheIndividualAssociationData* sameIndiAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[detSameId];
									if (sameIndiAssociationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) != mergedDetSameLabItem || sameIndiAssociationData->getRepresentativeSameIndividualId() != minRepIndiId || sameIndiAssociationData->getAssociatedIndividualId() == minRepIndiId && sameIndiAssociationData->isCompletelyHandled()) {

										bool incrementUpdateIdForDeterministicSameAsCompletion = mConfIncrementUpdateIdForDeterministicSameAsCompletion;
										if (sameIndiAssociationData->getAssociatedIndividualId() == minRepIndiId) {
											incrementUpdateIdForDeterministicSameAsCompletion = true;
										}

										CBackendRepresentativeMemoryCacheIndividualAssociationData* locSameIndiAssociationData = createLocalizedIndividualAssociationData(sameIndiAssociationData->getAssociatedIndividualId(), sameIndiAssociationData, ontologyData, false, incrementUpdateIdForDeterministicSameAsCompletion);
										CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locSameIndiAssociationData, ontologyData);

										locSameIndiAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
										locSameIndiAssociationData->setNeighbourRoleSetHash(sameIndiAssociationData->getNeighbourRoleSetHash());
										locSameIndiAssociationData->setRoleSetNeighbourArray(sameIndiAssociationData->getRoleSetNeighbourArray());

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
										locSameIndiAssociationData->mDebugUpdateFunction = QString("completeDeterministicSameAsMergingInformation");
										locSameIndiAssociationData->mDebugSameIndividualsCompletion = true;
										locSameIndiAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif
										DEBUG_HANDLING_STRING(QString("Updating new deterministic same mergings for %1").arg(detSameId))

										
										locSameIndiAssociationData->setRepresentativeSameIndividualId(minRepIndiId);
										if (locSameIndiAssociationData->hasRepresentativeSameIndividualMerging()) {
											if (!sameIndiAssociationData || !sameIndiAssociationData->hasRepresentativeSameIndividualMerging()) {
												mStatDetSameRepresentativeMergingCount++;
												ontologyData->incIndividualAssociationMergingCount();
											}
										}

										//locSameIndiAssociationData->setDeterministicSameIndividualId(minRepIndiId);
										locSameIndiAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, mergedDetSameLabItem);
										updateIndexedAssociationCount(locSameIndiAssociationData, sameIndiAssociationData, CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL, ontologyData);
										if (detSameId != minRepIndiId) {
											locSameIndiAssociationData->setCompletelyHandled(true);
										} else {
											locSameIndiAssociationData->setCompletelyHandled(false);
										}

										++mUpdatedIndiCount;
										++completedIndividualUpdates;
										ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locSameIndiAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

										storeIndividualIncompletelyMarked(locSameIndiAssociationData, !locSameIndiAssociationData->isCompletelyHandled(), ontologyData);
										setUpdatedIndividualAssociationData(locSameIndiAssociationData->getAssociatedIndividualId(), locSameIndiAssociationData, ontologyData);

										updated = true;
									}
								}



							}

						}
					}
					LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Completed %1 deterministic same individual labels over %2 individuals with %3 updated association data.").arg(completedDetSameLabelItems).arg(completedDetSameIntegratedIndividuals).arg(completedIndividualUpdates), this);

					return updated;
				}



				bool CBackendRepresentativeMemoryCache::completeNeighboursForSameAsMerging(QSet<cint64>& tmpCompleteNeighbourSameIndiMergingSet, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					cint64 checkingNeighbours = 0;
					cint64 completedNeighbours = 0;
					cint64 completedNeighbourLinks = 0;
					bool updated = false;
					QHash<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*> tmpLocalIndiAssocDataHash;
					for (cint64 mergeIndiId : mTmpCompleteNeighbourSameIndiMergingSet) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = ontologyData->getIndividualIdAssoiationDataVector()[mergeIndiId];
						CBackendRepresentativeMemoryLabelCacheItem* detSameLabItem = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
						if (detSameLabItem) {
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* neighbourRoleSetLabelItemHash = associationData->getNeighbourRoleSetHash();
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighbourArray = associationData->getRoleSetNeighbourArray();
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = neighbourArray->getIndexData();

							for (cint64 i = 0; i < arrayIndexData->getArraySize(); ++i) {
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinker = neighbourArray->at(i).getIndividualIdLinker();
								for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinkerIt = neighbourIndiLinker; neighbourIndiLinkerIt; neighbourIndiLinkerIt = neighbourIndiLinkerIt->getNext()) {
									cint64 neighbourIndiId = neighbourIndiLinkerIt->getIndividualId();
									++checkingNeighbours;

									CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];

									if (!neighbourAssociationData->hasDeterministicSameIndividualMerging()) {

										CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData = nullptr;
										CBackendRepresentativeMemoryLabelCacheItem* sameNeighbourRoleSetLabelItem = nullptr;
										cint64 sameNeighbourRoleSetLabelArrayId = -1;
										for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = detSameLabItem->getCacheValueLinker(); detSameValueLinkerIt && !sameNeighbourRoleSetLabelItem; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
											cint64 detSameId = detSameValueLinkerIt->getCacheValue().getTag();

											if (neighbourAssociationData->getNeighbourRoleSetHash()) {
												CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = neighbourAssociationData->getNeighbourRoleSetHash()->getNeighbourRoleSetLabel(detSameId);
												if (neighbourRoleSetLabelItem) {
													sameNeighbourRoleSetLabelItem = neighbourRoleSetLabelItem;
													sameNeighbourRoleSetLabelArrayId = neighbourAssociationData->getRoleSetNeighbourArray()->getIndexData()->getIndex(sameNeighbourRoleSetLabelItem);
												}
											}
										}

										if (sameNeighbourRoleSetLabelItem && sameNeighbourRoleSetLabelArrayId >= 0) {
											for (CBackendRepresentativeMemoryLabelValueLinker* detSameValueLinkerIt = detSameLabItem->getCacheValueLinker(); detSameValueLinkerIt; detSameValueLinkerIt = detSameValueLinkerIt->getNext()) {
												cint64 detSameId = detSameValueLinkerIt->getCacheValue().getTag();

												CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = neighbourAssociationData->getNeighbourRoleSetHash()->getNeighbourRoleSetLabel(detSameId);
												if (!neighbourRoleSetLabelItem) {

													if (!locNeighbourAssociationData) {
														CBackendRepresentativeMemoryCacheIndividualAssociationData*& tmpLlocNeighbourAssociationData = tmpLocalIndiAssocDataHash[neighbourIndiId];
														if (!tmpLlocNeighbourAssociationData) {
															tmpLlocNeighbourAssociationData = createLocalizedIndividualAssociationData(neighbourAssociationData->getAssociatedIndividualId(), neighbourAssociationData, ontologyData, false);
															bool requiresDataCopying = false;
															CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(tmpLlocNeighbourAssociationData, ontologyData, &requiresDataCopying);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
															tmpLlocNeighbourAssociationData->mDebugUpdateFunction = QString("completeNeighboursForSameAsMerging");
															tmpLlocNeighbourAssociationData->mDebugNeighbourCompletionExtended = true;
															tmpLlocNeighbourAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif


															CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
															CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
															newNeighbourRoleSetHash->initNeighbourRoleSetHash(neighbourAssociationData->getNeighbourRoleSetHash(), requiresDataCopying);
															newArray->initNeighbourArray(neighbourAssociationData->getRoleSetNeighbourArray());
															tmpLlocNeighbourAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
															tmpLlocNeighbourAssociationData->setRoleSetNeighbourArray(newArray);
															tmpLlocNeighbourAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
															ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(tmpLlocNeighbourAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));
															++mUpdatedIndiCount;
															completedNeighbours++;
														}
														locNeighbourAssociationData = tmpLlocNeighbourAssociationData;
													}

													CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* locNeighbourRoleSetLabelItemHash = locNeighbourAssociationData->getNeighbourRoleSetHash();
													CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* locArray = locNeighbourAssociationData->getRoleSetNeighbourArray();
													CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
													++mStatCreatedNeighbourLinks;
													newLinker->initIndividualIdLinker(detSameId);
													locArray->at(sameNeighbourRoleSetLabelArrayId).addIndividualIdLinker(newLinker);
													locNeighbourRoleSetLabelItemHash->setNeighbourRoleSetLabel(detSameId, sameNeighbourRoleSetLabelItem);
													completedNeighbourLinks++;

												}
											}
										}
									}
								}

							}
						}
					}
					for (QHash<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>::const_iterator it = tmpLocalIndiAssocDataHash.constBegin(), itEnd = tmpLocalIndiAssocDataHash.constEnd(); it != itEnd; ++it) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData = it.value();
						storeIndividualIncompletelyMarked(locNeighbourAssociationData, !locNeighbourAssociationData->isCompletelyHandled(), ontologyData);
						setUpdatedIndividualAssociationData(locNeighbourAssociationData->getAssociatedIndividualId(), locNeighbourAssociationData, ontologyData);

						updated |= udateDeterministicSameAssociations(locNeighbourAssociationData, ontologyData);
					}
					LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Completed %1 neighbouring links for %2 neighbours (%4 checked) of %3 same as merged individuals.").arg(completedNeighbourLinks).arg(completedNeighbours).arg(mTmpCompleteNeighbourSameIndiMergingSet.size()).arg(checkingNeighbours), this);
					return updated;
				}


				bool CBackendRepresentativeMemoryCache::storeIndividualIncompletelyMarked(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, bool markIncompletelyHandled, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool associationsUpdated = false;
					cint64 individualID = locAssociationData->getAssociatedIndividualId();
					if (!markIncompletelyHandled) {
						if (locAssociationData->isIncompletelyMarked()) {
							ontologyData->decIncompletelyHandledIndividualIdCount();
							//if (!mIncompletelyAssociatedIndividualSet.contains(CIndividualReference(individualID))) {
							//	bool debug = true;
							//}
							//mIncompletelyAssociatedIndividualSet.remove(CIndividualReference(individualID));
							locAssociationData->setIncompletelyMarked(false);
							associationsUpdated = true;

							if (locAssociationData->hasProblematicLevel()) {
								CCACHINGSET<cint64>* propIndiSet = ontologyData->getProblematicIncompletelyHandledIndividualSet();
								if (propIndiSet) {
									propIndiSet->remove(individualID);
								}
							}
						}
					} else {				
						ontologyData->setLastMinIncompletelyHandledIndvidualiId(qMin(ontologyData->getLastMinIncompletelyHandledIndvidualiId(), individualID));
						if (!locAssociationData->isIncompletelyMarked()) {
							ontologyData->incIncompletelyHandledIndividualIdCount();
							//if (mIncompletelyAssociatedIndividualSet.contains(CIndividualReference(individualID))) {
							//	bool debug = true;
							//}
							//mIncompletelyAssociatedIndividualSet.insert(CIndividualReference(individualID));
							locAssociationData->setIncompletelyMarked(true);
							associationsUpdated = true;

							if (locAssociationData->hasProblematicLevel()) {
								CCACHINGSET<cint64>* propIndiSet = ontologyData->getProblematicIncompletelyHandledIndividualSet();
								if (!propIndiSet) {
									CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
									propIndiSet = CObjectParameterizingAllocator < CCACHINGSET<cint64>, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
								}
								propIndiSet->insert(individualID);
							}
						}
					}
					return associationsUpdated;
				}




				bool CBackendRepresentativeMemoryCache::installNominalIndirectConncetionUpdates(CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					bool connectionDataUpdated = false;

					for (CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinkerIt = tempNomIndirectConnDataLinker; tempNomIndirectConnDataLinkerIt; tempNomIndirectConnDataLinkerIt = tempNomIndirectConnDataLinkerIt->getNext()) {
						cint64 nomIndiId = tempNomIndirectConnDataLinkerIt->getIndividualID();
						CXLinker<cint64>* indirectlyConnectedIndividualIdLinker = tempNomIndirectConnDataLinkerIt->getIndirectlyConnectedIndividualIdLinker();
						cint64 lastChangeIntegrationId = tempNomIndirectConnDataLinkerIt->getLastChangeIntegrationId();

						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData*& nomConnData = (*ontologyData->getNominaIIndividualdIndirectConnectionDataHash())[nomIndiId];
						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* prevNomConnData = nomConnData;


						CBackendRepresentativeMemoryCacheIndividualAssociationData* nominalAssociationData = nullptr;
						if (ontologyData->getIndividualIdAssoiationDataVector() && nomIndiId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
							nominalAssociationData = ontologyData->getIndividualIdAssoiationDataVector()[nomIndiId];
						}
						if (nominalAssociationData && nominalAssociationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId() != lastChangeIntegrationId && lastChangeIntegrationId > 0 ||
								nominalAssociationData && prevNomConnData && lastChangeIntegrationId == 0 && nominalAssociationData->hasIndirectlyConnectedIndividualIntegration() && prevNomConnData->getLastChangeId() != nominalAssociationData->getLastIntegratedIndirectlyConnectedIndividualsChangeId()) {
							// mark as incompletely handled
							connectionDataUpdated |= markRepresentativeReferencedIndividualAssociationIncompletelyHandled(nomIndiId, nominalAssociationData, ontologyData);
						}


						CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* newNomConnData = CObjectAllocator< CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData >::allocateAndConstruct(context->getMemoryAllocationManager());
						newNomConnData->initNominalIndividualIndirectConnectionData(prevNomConnData);
						nomConnData = newNomConnData;
						newNomConnData->setLastChangeId(mNextNomConnUpdateId++);

						for (CXLinker<cint64>* indirectlyConnectedIndividualIdLinkerIt = indirectlyConnectedIndividualIdLinker; indirectlyConnectedIndividualIdLinkerIt; indirectlyConnectedIndividualIdLinkerIt = indirectlyConnectedIndividualIdLinkerIt->getNext()) {
							cint64 indiId = indirectlyConnectedIndividualIdLinkerIt->getData();
							// avoid duplicates

							bool alreadyPresent = false;
							CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData = nullptr;
							if (ontologyData->getIndividualIdAssoiationDataVector() && indiId < ontologyData->getIndividualIdAssoiationDataVectorSize()) {
								associationData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
							}

							CBackendRepresentativeMemoryLabelCacheItem* indiConnNomIndiIdSetLabel = nullptr;
							if (associationData) {
								if (associationData->getCacheUpdateId() < mTmpIndiAssocPrevUpdateId) {
									indiConnNomIndiIdSetLabel = associationData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL);
								} else {
									indiConnNomIndiIdSetLabel = mTmpIndiIndirectlyConnNomLabelItemHash.value(indiId);
								}
							}

							if (indiConnNomIndiIdSetLabel && indiConnNomIndiIdSetLabel->hasCachedTagValue(nomIndiId)) {
								alreadyPresent = true;
							}


							if (!alreadyPresent) {
								CXLinker<cint64>* indiLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(context->getMemoryAllocationManager());
								indiLinker->initLinker(indiId);
								nomConnData->addIndirectlyConnectedIndividualIdLinker(indiLinker);
								connectionDataUpdated = true;
							}

						}
					}

					return connectionDataUpdated;
				}



				void CBackendRepresentativeMemoryCache::setUpdatedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = ontologyData->getIndividualIdAssoiationDataVector();
					cint64 indiIdAssoDataVectorSize = ontologyData->getIndividualIdAssoiationDataVectorSize();
					if (individualID >= indiIdAssoDataVectorSize) {
						cint64 prevSize = indiIdAssoDataVectorSize;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** prevIndiIdAssoDataVector = indiIdAssoDataVector;
						indiIdAssoDataVectorSize = qMax(indiIdAssoDataVectorSize * 10, individualID);
						indiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(context->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
						for (cint64 i = 0; i < prevSize; ++i) {
							indiIdAssoDataVector[i] = prevIndiIdAssoDataVector[i];
						}
						for (cint64 i = prevSize; i < indiIdAssoDataVectorSize; ++i) {
							indiIdAssoDataVector[i] = nullptr;
						}
						ontologyData->setIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, indiIdAssoDataVector);
					}
					ontologyData->updateMaxStoredIndvidualiId(individualID);
					if (!indiIdAssoDataVector[individualID]) {
						ontologyData->incIndividualAssociationsCount();
					}
					if (locAssociationData->getPreviousData() && !locAssociationData->getPreviousData()->getPreviousData()) {
						ontologyData->incIndividualAssociationDataUpdateCount();
					}
					indiIdAssoDataVector[individualID] = locAssociationData;
					//mIndiIdAssoDataHash->insert(individualID, locAssociationData);
					DEBUG_HANDLING_STRING(QString("Stored update data for %1").arg(locAssociationData->getAssociatedIndividualId()))
				}


				bool CBackendRepresentativeMemoryCache::getIncompletlyAssociationCachedIndividuals(cint64 ontologyIdentifier, CBackendIndividualRetrievalComputationUpdateCoordinationHash* prevCoordHash, CBackendIndividualRetrievalComputationUpdateCoordinationHash* newCoordHash, bool allIndividualsAdded, bool refillRetrievalCoordHash, cint64 limit, CCallbackData* callbackData) {
					if (!callbackData) {
						CBlockingCallbackData blockingCallbackData;
						if (mConfDirectUpdateSynchronization) {
							mDirectUpdateSyncMutex.lock();
							CRetrieveIncompletelyAssociationCachedEvent* procEvent = new CRetrieveIncompletelyAssociationCachedEvent(&blockingCallbackData, ontologyIdentifier, prevCoordHash, newCoordHash, allIndividualsAdded, refillRetrievalCoordHash, limit);
							processCustomsEvents(CRetrieveIncompletelyAssociationCachedEvent::EVENTTYPE, procEvent);
							mDirectUpdateSyncMutex.unlock();
						} else {
							postEvent(new CRetrieveIncompletelyAssociationCachedEvent(&blockingCallbackData, ontologyIdentifier, prevCoordHash, newCoordHash, allIndividualsAdded, refillRetrievalCoordHash, limit));
						}
						blockingCallbackData.waitForCallback();
					} else {
						if (mConfDirectUpdateSynchronization) {
							mDirectUpdateSyncMutex.lock();
							CRetrieveIncompletelyAssociationCachedEvent* procEvent = new CRetrieveIncompletelyAssociationCachedEvent(callbackData, ontologyIdentifier, prevCoordHash, newCoordHash, allIndividualsAdded, refillRetrievalCoordHash, limit);
							processCustomsEvents(CRetrieveIncompletelyAssociationCachedEvent::EVENTTYPE, procEvent);
							mDirectUpdateSyncMutex.unlock();
						} else {
							postEvent(new CRetrieveIncompletelyAssociationCachedEvent(callbackData, ontologyIdentifier, prevCoordHash, newCoordHash, allIndividualsAdded, refillRetrievalCoordHash, limit));
						}
					}
					return true;
				}

				bool CBackendRepresentativeMemoryCache::initializeIndividualsAssociationCaching(cint64 ontologyIdentifier, cint64 individualCount) {
					if (mConfDirectUpdateSynchronization) {
						mDirectUpdateSyncMutex.lock();
						CInitializeIndividualAssociationsCacheEvent* procEvent = new CInitializeIndividualAssociationsCacheEvent(ontologyIdentifier, individualCount);
						processCustomsEvents(CInitializeIndividualAssociationsCacheEvent::EVENTTYPE, procEvent);
						mDirectUpdateSyncMutex.unlock();
					} else {
						postEvent(new CInitializeIndividualAssociationsCacheEvent(ontologyIdentifier, individualCount));
					}
					return true;
				}

				bool CBackendRepresentativeMemoryCache::reportMaximumHandledRecomputationId(cint64 ontologyIdentifier, cint64 maximumRecomputationId) {
					if (mConfDirectUpdateSynchronization) {
						mDirectUpdateSyncMutex.lock();
						CReportMaximumHandledRecomputationIdsEvent* procEvent = new CReportMaximumHandledRecomputationIdsEvent(ontologyIdentifier, maximumRecomputationId);
						processCustomsEvents(CReportMaximumHandledRecomputationIdsEvent::EVENTTYPE, procEvent);
						mDirectUpdateSyncMutex.unlock();
					} else {
						postEvent(new CReportMaximumHandledRecomputationIdsEvent(ontologyIdentifier, maximumRecomputationId));
					}
					return true;
				}

				bool CBackendRepresentativeMemoryCache::requiresIndividualAssociations(cint64 labelType) {
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL ||
						labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL ||
						labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL || labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						return true;
					}
					return false;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* CBackendRepresentativeMemoryCache::getIndividualAssociationsExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP, extensionData);
					}
					return extensionData;
				}


				CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* CBackendRepresentativeMemoryCache::getIndividualNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData::INDIVIDUAL_NEIGHBOUR_ARRAY_INDEX, extensionData);
						extensionData->initNeighbourArrayIndexData(labelItem);
					}
					return extensionData;
				}


				CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* CBackendRepresentativeMemoryCache::getNeighbourArrayRoleTagResolvingLabelExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
					CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH);
					if (!extensionData) {
						extensionData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData::TAG_RESOLVING_HASH, extensionData);
						extensionData->initTagLabelResolvingExtensionData();

						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexExtensionData = getIndividualNeighbourArrayIndexExtensionData(labelItem, ontologyData);
						for (cint64 i = 0; i < indexExtensionData->getArraySize(); ++i) {
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexExtensionData->getNeighbourRoleSetLabel(i);

							for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = neighbourRoleSetLabelItem->getCacheValueLinker(); labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
								CCacheValue cacheValue = labelValueLinkerIt->getData();


								CCacheValue::CACHEVALUEIDENTIFIER identifier = cacheValue.getCacheValueIdentifier();
								bool nondeterministc = identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_ASSERTEDROLE ||
									identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSEDASSERTEDROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_NOMINAL_CONNECTED_ROLE || identifier == CCacheValue::CACHEVALTAGAND_NONDETERMINISTIC_INVERSED_NOMINAL_CONNECTED_ROLE;


								CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker* linker = CObjectAllocator< CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingDataLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								linker->initTagLabelResolvingData(neighbourRoleSetLabelItem, i, !nondeterministc);
								extensionData->appendTagLabelResolvingDataLinker(cacheValue.getTag(), linker);
							}
						}
						
					}
					return extensionData;
				}


				cint64 CBackendRepresentativeMemoryCache::getMinimumSlotReferreringInstalledValidRecomputationId(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					cint64 minInstalledValidRecId = CINT64_MAX;
					cint64 ontologyIdentifier = ontologyData->getOntologyIdentifer();

					for (CBackendRepresentativeMemoryCacheSlotItem* slotLinkerIt = mSlotLinker; slotLinkerIt; slotLinkerIt = slotLinkerIt->getNext()) {
						CBackendRepresentativeMemoryCacheOntologyData* referredOntologyData = slotLinkerIt->getOntologyData(ontologyIdentifier);
						cint64 minValidRecId = referredOntologyData->getMinimumValidRecomputationId();
						minInstalledValidRecId = qMin(minInstalledValidRecId, minValidRecId);
					}
					return minInstalledValidRecId;
				}






































				bool CBackendRepresentativeMemoryCache::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
					if (CThread::processCustomsEvents(type, event)) {
						return true;

					}
					else if (type == CReportMaximumHandledRecomputationIdsEvent::EVENTTYPE) {
						CReportMaximumHandledRecomputationIdsEvent* rmhrie = (CReportMaximumHandledRecomputationIdsEvent*)event;
						cint64 maxHandledRecomId = rmhrie->getMaximumHandledRecomputationId();
						cint64 ontologyIdentifier = rmhrie->getOntologyIdentifier();

						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mOntologyIdentifierDataHash->value(ontologyIdentifier);
						if (ontologyData) {
							ontologyData->setNextUpdateMinimumValidRecomputationId(maxHandledRecomId+1);
							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Received report of handled recomputation id %1.").arg(maxHandledRecomId), this);
						}
						++mStatReportedMaximumHandledRecomputationIdCount;

						return true;

					}  else if (type == CInitializeIndividualAssociationsCacheEvent::EVENTTYPE) {
						CInitializeIndividualAssociationsCacheEvent* iiace = (CInitializeIndividualAssociationsCacheEvent*)event;

						cint64 indiCount = iiace->getIndividualCount();
						cint64 ontologyIdentifier = iiace->getOntologyIdentifier();

						prepareOntologyDataUpdate(ontologyIdentifier, indiCount);

						return true;

					} else if (type == CRetrieveIncompletelyAssociationCachedEvent::EVENTTYPE) {
						CRetrieveIncompletelyAssociationCachedEvent* iace = (CRetrieveIncompletelyAssociationCachedEvent*)event;
						cint64 limit = iace->getRetrievalLimit();
						cint64 ontologyIdentifier = iace->getOntologyIdentifier();

						bool allIndividualsAdded = iace->hasAllIndividualsAdded();
						bool refillRetrievalCoordHash = iace->hasRefillRetrievalCoordHashOrdered();


						++mCheckingRemainingIncompletelyHandledCount;
						bool updateSlot = false;

						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mOntologyIdentifierDataHash->value(ontologyIdentifier);
						if (ontologyData && !ontologyData->isAssociationCompleted()) {
							ontologyData->incIncompletelyHandledIndividualsRetrievalCount();
							cint64 count = 0;
							cint64 prevLastMinIncompletelyHandledIndiId = ontologyData->getLastMinIncompletelyHandledIndvidualiId();

							CBackendIndividualRetrievalComputationUpdateCoordinationHash* lastRetrievalHash = iace->getLastIndividualsRetrievalCoordinationHash();
							CBackendIndividualRetrievalComputationUpdateCoordinationHash* newRetrievalHash = iace->getNewIndividualsRetrievalCoordinationHash();





							newRetrievalHash->setApproximateTotalIncompletelyHandledCount(ontologyData->getIncompletelyHandledIndividualIdCount());
							if (lastRetrievalHash) {

								cint64 corr = lastRetrievalHash->getApproximateCorrectionIncompletelyHandledCount();
								//LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("last corr %1").arg(corr), this);
								corr += ontologyData->getIncompletelyHandledIndividualIdCount() - lastRetrievalHash->getApproximateRemainingIncompletelyHandledCount();
								//LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("remain diff %1").arg(ontologyData->getIncompletelyHandledIndividualIdCount() - lastRetrievalHash->getApproximateRemainingIncompletelyHandledCount()), this);
								corr += lastRetrievalHash->getApproximateTotalIncompletelyHandledCount() - ontologyData->getIncompletelyHandledIndividualIdCount();
								//LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("last diff %1").arg(lastRetrievalHash->getApproximateTotalIncompletelyHandledCount() - ontologyData->getIncompletelyHandledIndividualIdCount()), this);
								newRetrievalHash->setApproximateCorrectionIncompletelyHandledCount(corr);
								//LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("new corr %1").arg(corr), this);
							}



							if (lastRetrievalHash && !refillRetrievalCoordHash) {
								for (CBackendIndividualRetrievalComputationUpdateCoordinationHash::const_iterator it = lastRetrievalHash->constBegin(), itEnd = lastRetrievalHash->constEnd(); it != itEnd; ++it) {
									cint64 indiId = it.key();
									CBackendIndividualRetrievalComputationUpdateCoordinationHashData* refData = it.value();
									bool keepIndi = false;
									CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
									if (!refData->isComputationIntegrated() && !refData->isComputationOrdered()) {
										keepIndi = true;
										//newRetrievalHash->incHashRemainingCount();
										//if (!indiAssData->isIncompletelyMarked()) {
										//	newRetrievalHash->incApproximateCorrectionIncompletelyHandledCount();
										//}
									} else if (!refData->isProcessed()) {
										//newRetrievalHash->incHashComputationCount();
										keepIndi = true;
									} else {
										if (indiAssData->getCacheTouchId() == refData->getAssociationUpdateId()) {
											//newRetrievalHash->incHashProcessedCount();
											keepIndi = true;
										}
									}
									if (keepIndi) {
										refData->incUsageCount();
										newRetrievalHash->insert(indiId, refData);
										
									} else {
										//newRetrievalHash->incHashRemovedCount();
									}
								}
							}

							

							if (ontologyData->isBasicPrecomputationMode()) {


								if (newRetrievalHash) {
									cint64 pos = ontologyData->getBasicPrecompuationRetrievalIndividualIdPosition();
									for (; pos < ontologyData->getBasicPrecomputationIndividualIdAssoiationDataVectorSize() && count < ontologyData->getIncompletelyHandledIndividualIdCount() && (count < limit || limit < 0); ++pos) {
										CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[pos];
										if (indiAssData && !indiAssData->isCompletelyHandled() && indiAssData->getAssociationDataUpdateId() <= 1) {

											CBackendIndividualRetrievalComputationUpdateCoordinationHashData*& refData = (*newRetrievalHash)[pos];
											if (!refData) {
												refData = newRetrievalHash->createCoordinationData();
												refData->setAssociationUpdateId(indiAssData->getCacheTouchId());
												refData->incUsageCount();
												newRetrievalHash->incHashRemainingCount();
												count++;
											}

										}
									}
									ontologyData->setBasicPrecompuationRetrievalIndividualIdPosition(pos);
								}

								if (newRetrievalHash->isEmpty()) {
									// deactivate basic precompuation mode
									ontologyData->setBasicPrecomputationMode(false);
									updateSlot = true;
									newRetrievalHash->setBasicPrecomputationFinished(true);
								} else {
									newRetrievalHash->setBasicPrecomputationMode(true);
								}

							} 
							if (!ontologyData->isBasicPrecomputationMode()) {

								CCACHINGSET<cint64>* propIndiSet = ontologyData->getProblematicIncompletelyHandledIndividualSet();
								if (propIndiSet) {
									for (CCACHINGSET<cint64>::const_iterator it = propIndiSet->constBegin(), itEnd = propIndiSet->constEnd(); it != itEnd && count < ontologyData->getIncompletelyHandledIndividualIdCount() && (count < limit || limit < 0); ++it) {
										cint64 indiId = *it;
										CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];									

										CBackendIndividualRetrievalComputationUpdateCoordinationHashData*& refData = (*newRetrievalHash)[indiId];
										if (!refData) {
											refData = newRetrievalHash->createCoordinationData();
											refData->setAssociationUpdateId(indiAssData->getCacheTouchId());
											refData->incUsageCount();
											newRetrievalHash->incHashRemainingCount();
											count++;
										}

									}
								}



								if (newRetrievalHash) {
									for (cint64 i = ontologyData->getLastMinIncompletelyHandledIndvidualiId(); i < ontologyData->getIndividualIdAssoiationDataVectorSize() && count < ontologyData->getIncompletelyHandledIndividualIdCount() && (count < limit || limit < 0); ++i) {
										CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[i];
										if (indiAssData && !indiAssData->isCompletelyHandled()) {

											CBackendIndividualRetrievalComputationUpdateCoordinationHashData*& refData = (*newRetrievalHash)[i];
											if (!refData) {
												refData = newRetrievalHash->createCoordinationData();
												refData->setAssociationUpdateId(indiAssData->getCacheTouchId());
												refData->incUsageCount();
												newRetrievalHash->incHashRemainingCount();
												count++;
											}

										} else if (i == ontologyData->getLastMinIncompletelyHandledIndvidualiId()) {
											ontologyData->incLastMinIncompletelyHandledIndvidualiId();
										}
									}

									/*if (limit < 0 || limit > mIncompletelyAssociatedIndividualSet.size()) {
										*individualSet = mIncompletelyAssociatedIndividualSet;
									} else {
										cint64 count = 0;
										for (QSet<CIndividualReference>::const_iterator it = mIncompletelyAssociatedIndividualSet.constBegin(), itEnd = mIncompletelyAssociatedIndividualSet.constEnd(); it != itEnd && count < limit; ++it) {
											individualSet->insert(*it);
											++count;
										}
									}*/


									if (newRetrievalHash->size() <= 0 && ontologyData->getIncompletelyHandledIndividualIdCount() > 0) {
										cint64 foundIncompleteCount = 0;
										cint64 prevIncompleteCount = ontologyData->getIncompletelyHandledIndividualIdCount();
										for (cint64 i = 0; i < ontologyData->getIndividualIdAssoiationDataVectorSize() && count < ontologyData->getIncompletelyHandledIndividualIdCount() && (count < limit || limit < 0); ++i) {
											CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[i];
											if (indiAssData && !indiAssData->isCompletelyHandled()) {

												if (foundIncompleteCount <= 0) {
													LOG(WARN, "::Konclude::RepresentativeBackendCache", logTr("Corrected first insufficiently handled individual from %2 to %1.").arg(ontologyData->getLastMinIncompletelyHandledIndvidualiId()).arg(i), this);
													ontologyData->setLastMinIncompletelyHandledIndvidualiId(i);
												}
												foundIncompleteCount++;
												CBackendIndividualRetrievalComputationUpdateCoordinationHashData*& refData = (*newRetrievalHash)[i];
												if (!refData) {
													refData = newRetrievalHash->createCoordinationData();
													refData->setAssociationUpdateId(indiAssData->getCacheTouchId());
													refData->incUsageCount();
													newRetrievalHash->incHashRemainingCount();
													count++;
												}

											}
										}
										if (foundIncompleteCount <= 0) {
											ontologyData->setIncompletelyHandledIndividualIdCount(0);
										}
										LOG(WARN, "::Konclude::RepresentativeBackendCache", logTr("Corrected insufficiently handled individuals from %2 to %1.").arg(foundIncompleteCount).arg(prevIncompleteCount), this);

									}

								}
							}


							if (allIndividualsAdded) {
								if (!ontologyData->isFirstIncompletelyHandledIndividualsRetrieved()) {
									if (!ontologyData->isSlotUpdateIntegrated()) {
										updateSlot = true;
									}
								}
								ontologyData->setFirstIncompletelyHandledIndividualsRetrieved(true);
							}

							bool forceCompletion = false;
							if (mConfMaxIncompletelyHandledIndividualsRetrievalCount > 0 && ontologyData->getIncompletelyHandledIndividualsRetrievalCount() > mConfMaxIncompletelyHandledIndividualsRetrievalCount) {
								LOG(WARN, "::Konclude::RepresentativeBackendCache", logTr("Forcing association completion with reached maximum of %1 retrievals.").arg(mConfMaxIncompletelyHandledIndividualsRetrievalCount), this);
								forceCompletion = true;
								count = 0;
								if (newRetrievalHash) {
									newRetrievalHash->clear();
								}
							}
							checkAssociationComplete(ontologyData, forceCompletion);

							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Retrieval %3 returning %1 new of %2 insufficiently handled individuals for reprocessing.").arg(count).arg(ontologyData->getIncompletelyHandledIndividualIdCount()).arg(ontologyData->getIncompletelyHandledIndividualsRetrievalCount()), this);

							//if (mIncompletelyHandledIndiIdCount != mIncompletelyAssociatedIndividualSet.size()) {
							//	bool debug = true;
							//}

							//if (mIncompletelyHandledIndiIdCount > 0 && individualSet->isEmpty()) {
							//	cint64 count = 0;
							//	for (cint64 i = 0; i < mIndiIdAssoDataVectorSize && count < mIncompletelyHandledIndiIdCount && count < limit; ++i) {
							//		CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = mIndiIdAssoDataVector[i];
							//		if (indiAssData && !indiAssData->isCompletelyHandled()) {
							//			individualSet->insert(i);
							//			count++;
							//		}
							//	}

							//	count = count;
							//	bool debug = true;
							//}


							
						}


						CCallbackData* callbackData = iace->getCallback();
						if (callbackData) {
							callbackData->doCallback();
						}


						if (updateSlot) {
							ontologyData->setNextSlotUpdateWaitingCount(mSlotUpdateWaitingIncreaseCount);
							createReaderSlotUpdate(ontologyData, &mContext);
							cleanUnusedSlots(&mContext);
						}
						return true;


					}
					else if (type == CWriteBackendAssociationCachedEvent::EVENTTYPE) {
						CWriteBackendAssociationCachedEvent* wcde = (CWriteBackendAssociationCachedEvent*)event;
						CMemoryPool* memoryPools = wcde->getMemoryPools();
						CBackendRepresentativeMemoryCacheWriteData* newWriteData = (CBackendRepresentativeMemoryCacheWriteData*)wcde->getWriteData();
						cint64 dataWriteCount = newWriteData->getCount();

						CBackendRepresentativeMemoryCacheWriteData* dataLinkerIt = newWriteData;

						cint64 ontologyIdentifier = dataLinkerIt->getOntologyIdentifier();

						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = mOntologyIdentifierDataHash->value(ontologyIdentifier);
						mLastHandledOntologyContext = ontologyData;

						if (mStatCollectStatistics) {
							mPendingUpdateCount.deref();
						}
						mCurrentUpdateHandlingRecomputationId = -1;

						if (!ontologyData || !ontologyData->isAssociationCompleted()) {
							ontologyData->incCacheDataUpdateWritingCount();
							++mWriteDataCount;
							ontologyData = prepareOntologyDataUpdate(ontologyIdentifier);
							cint64 prevUpdatedIndiCount = mUpdatedIndiCount;
							cint64 prevAssociationUpdatedIndiCount = mAssociationUpdatedIndiCount;
							bool allCachingSuccess = true;
							bool oneCachingSuccess = false;
							bool oneCachingExpSuccess = false;
							while (dataLinkerIt) {
								mCurrentUpdateHandlingRecomputationId = -1;
								if (dataLinkerIt->getCacheWriteDataType() == CBackendRepresentativeMemoryCacheWriteData::BACKENDASSOCIATIONWRITEDATATYPE) {
									CBackendRepresentativeMemoryCacheLabelAssociationWriteData* baAsWrDa = (CBackendRepresentativeMemoryCacheLabelAssociationWriteData*)dataLinkerIt;

									mCurrentUpdateHandlingRecomputationId = baAsWrDa->getRecompuationId();

									CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker = baAsWrDa->getTemporaryAssociationWriteDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker = baAsWrDa->getTemporaryLabelWriteDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker = baAsWrDa->getTemporaryCardinaltyWriteDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker = baAsWrDa->getTemporaryAssociationUseDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker = baAsWrDa->getTemporaryNominalIndirectConnectionDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryInvolvedIndividualDataLinker* tmpInvolvedIndiDataLinker = baAsWrDa->getTemporaryInvolvedIndividualIdDataLinker();
									CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* tmpPropCutDataLinker = baAsWrDa->getTemporaryPropagationCutDataLinker();

									installTemporaryLabels(tempLabelWriteDataLinker, ontologyData);
									installTemporaryCardinalities(tempCardWriteDataLinker, ontologyData);

									mTmpIndiAssocPrevUpdateId = mNextIndiUpdateId;
									mTmpIndiIndirectlyConnNomLabelItemHash.clear();
								

									bool cached = false;

									if (checkUpdateRejection(tempAssWriteDataLinker, ontologyData)) {
										cached |= handleUpdateRejection(tempAssWriteDataLinker, ontologyData);
										LOG(WARN, "::Konclude::RepresentativeBackendCache", logTr("Recjecting installation of update from recomputation id %1.").arg(mCurrentUpdateHandlingRecomputationId), this);

									} else {

										cached |= integratePropagationCut(tmpPropCutDataLinker, ontologyData);
										analyseDeterministicSameAsAssociationInstallation(tempAssWriteDataLinker, ontologyData);
										cached |= installAssociationUpdates(tempAssWriteDataLinker, ontologyData);
										cached |= installNominalIndirectConncetionUpdates(tempNomIndirectConnDataLinker, ontologyData);
										cached |= checkAssociationUsage(tempAssUseDataLinker, ontologyData);
										cached |= updateInvolvedIndividuals(tmpInvolvedIndiDataLinker, ontologyData);
										cached |= installDeterministicSameAsAssociationUpdates(tempAssWriteDataLinker, ontologyData);
										mDeterministicSameHandlingInstallationDataHash.clear();
										mPropagationCutIndiSet.clear();


										if (!mTmpDetSameMergingCompletionReferenceHash.isEmpty()) {
											cached |= completeDeterministicSameAsMergingInformation(mTmpDetSameMergingCompletionReferenceHash, ontologyData);
											mTmpDetSameMergingCompletionReferenceHash.clear();
										}
										if (!mTmpCompleteNeighbourSameIndiMergingSet.isEmpty()) {
											cached |= completeNeighboursForSameAsMerging(mTmpCompleteNeighbourSameIndiMergingSet, ontologyData);
											mTmpCompleteNeighbourSameIndiMergingSet.clear();
										}

										if (mTmpPropCutIndiArrayNeighboursHandlingDataHash) {
											for (QHash<cint64, CPropagationCutNeighbourArrayHandlingData*>::const_iterator it = mTmpPropCutIndiArrayNeighboursHandlingDataHash->constBegin(), itEnd = mTmpPropCutIndiArrayNeighboursHandlingDataHash->constBegin(); it != itEnd; ++it) {
												delete it.value();
											}
											delete mTmpPropCutIndiArrayNeighboursHandlingDataHash;
											mTmpPropCutIndiArrayNeighboursHandlingDataHash = nullptr;
										}
									}

									allCachingSuccess &= cached;
									oneCachingSuccess |= cached;
									oneCachingExpSuccess |= cached;
								}
								dataLinkerIt = (CBackendRepresentativeMemoryCacheWriteData*)dataLinkerIt->getNext();
							}

							if (!oneCachingSuccess) {
								++mEmptyWriteDataCount;
							}

							bool forceCompletion = false;
							if (mConfMaxCacheDataUpdateWritingCount > 0 && ontologyData->getCacheDataUpdateWritingCount() > mConfMaxCacheDataUpdateWritingCount) {
								LOG(WARN, "::Konclude::RepresentativeBackendCache", logTr("Forcing association completion with reached maximum of %1 updates.").arg(mConfMaxCacheDataUpdateWritingCount), this);
								forceCompletion = true;
							}
							checkAssociationComplete(ontologyData, forceCompletion);

							bool basicPrecomputationModeActivated = false;
							if (checkBasicPrecompuationModeActivation(ontologyData)) {
								if (activateBasicPrecompuationMode(ontologyData)) {
									basicPrecomputationModeActivated = true;
								}
							}

							bool slotUpdate = false;
							if (ontologyData->getNextSlotUpdateWaitingCount() <= 0 || ontologyData->isAssociationCompleted() || basicPrecomputationModeActivated) {
								deleteExpiredIndividualAssociationMemoryContexts(ontologyData, &mContext);
								createReaderSlotUpdate(ontologyData, &mContext);
								cleanUnusedSlots(&mContext);
								ontologyData->setNextSlotUpdateWaitingCount(mSlotUpdateWaitingIncreaseCount++);
								if (mSlotUpdateWaitingMaxCount >= 0 && mSlotUpdateWaitingIncreaseCount > mSlotUpdateWaitingMaxCount) {
									mSlotUpdateWaitingIncreaseCount = mSlotUpdateWaitingMaxCount;
								}
								slotUpdate = true;
							} else {
								ontologyData->setNextSlotUpdateWaitingCount(ontologyData->getNextSlotUpdateWaitingCount() - 1);
							}

							cint64 updatedIndiCount = mUpdatedIndiCount - prevUpdatedIndiCount;
							cint64 directUpdatedIndiCount = mAssociationUpdatedIndiCount - prevAssociationUpdatedIndiCount;
							QString updateType = QString("intern");
							if (slotUpdate) {
								updateType = QString("published");
							}
							QString pendingString;
							if (mStatCollectStatistics) {
								pendingString = QString(", %1 pending").arg(mPendingUpdateCount);
							}
							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Update %1 (%5%6) for representative backend cache, updated %2 (%7 direct) of overall %3 individual associations, %4 remaining insufficiently handled individuals.")
								.arg(mWriteDataCount).arg(updatedIndiCount).arg(ontologyData->getIndividualAssociationsCount()).arg(ontologyData->getIncompletelyHandledIndividualIdCount()).arg(updateType).arg(pendingString).arg(directUpdatedIndiCount), this);

							if (mStatCollectStatistics) {
								LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Overall %1 updated invididual associations (max %5), %2 created neighbour links (%3 only adding link updates, %4 changed/removed link udpates, %6 max links).")
									.arg(mUpdatedIndiCount).arg(mStatCreatedNeighbourLinks).arg(mStatAddingNeighbourLinksAssociationUpdateCount).arg(mStatUpdatedOrRemovedNeighbourLinksAssociationUpdateCount).arg(mStatMaxAssociationUpdateCount).arg(mStatMaxNeighbourLinksCount), this);



								LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Successfully installed %1 deterministic same association data (of %6 deterministic merges), failed installation for %2 (%3 due to incompletely handled destination, %5 due to merged destination, %4 due to different update id).")
									.arg(mStatDetSameAssociationInstallCount).arg(mStatDetSameAssociationFailedCount).arg(mStatDetSameAssociationIncompleteHandledDestFailedCount).arg(mStatDetSameAssociationDifferentUpdateIdFailedCount).arg(mStatDetSameAssociationRepMergedDestFailedCount).arg(mStatDetSameRepresentativeMergingCount), this);

								QStringList labelsStringList;
								for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++i) {
									QString labelName = getRepresentativeCacheLabelItemString(i);
									QString labelString = QString("\t%2 of label type %1 (max size %3, all value %4)").arg(labelName).arg(mStatLabelTypeCount[i]).arg(mStatLabelTypeMaxValueCount[i]).arg(mStatLabelTypeAllValueCount[i]);
									labelsStringList.append(labelString);
								}

								LOG(INFO, "::Konclude::RepresentativeBackendCache", QString("Containing %2 labels, %3 max label size, %4 max same merged individuals,\r\n%1").arg(labelsStringList.join(",\r\n"))
									.arg(mStatLabelCount).arg(mStatMaxLabelValueCount).arg(mStatMaxSameAsMergedCount), this);



							}


						} else {
							LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Ignoring update since individual label association is already complete."), this);
						}


						//mContext.releaseTemporaryMemoryPools(memoryPools);
						mCacheStat.setMemoryConsumption(mContext.getMemoryConsumption());

						//if (ontologyData->isFirstIncompletelyHandledIndividualsRetrieved() && (mWriteDataCount % 50 == 0)) {
						//	writeStringifiedRepresentativeCacheToFile();
						//}

						if (mLimitRemainingWritePending) {
							mRemainingWritePendingSemaphore.release();
						}

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						if (mConfDebugWriteRepresentativeCache && (mWriteDataCount <= 50 || mWriteDataCount % 500 == 0)) {
							//writeStringifiedRepresentativeCacheToFile();
						}
#endif


						return true;
					}
					return false;
				}




				bool CBackendRepresentativeMemoryCache::checkBasicPrecompuationModeActivation(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					if (!ontologyData->isAssociationCompleted() && !ontologyData->hasBasicPrecomputationModeActivation() && ontologyData->getIncompletelyHandledIndividualIdCount() > 0 && mConfBasicPrecomputationModeActivationUpdateMergesRatio > 0) {
						double basicIndiUpdateCount = ontologyData->getIndividualAssociationDataDirectUpdateCount();
						double indiMergesCount = ontologyData->getIndividualAssociationMergingCount();

						if (indiMergesCount > 0) {
							double ratio = indiMergesCount / basicIndiUpdateCount;
							if (ratio > mConfBasicPrecomputationModeActivationUpdateMergesRatio) {
								return true;
							}
						}
					}
					return false;
				}



				bool CBackendRepresentativeMemoryCache::activateBasicPrecompuationMode(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					if (!ontologyData->isBasicPrecomputationMode() && !ontologyData->hasBasicPrecomputationModeActivation()) {
						ontologyData->setBasicPrecomputationModeActivation(true);
						ontologyData->setBasicPrecomputationMode(true);

						CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();

						cint64 basicIndiUpdateCount = ontologyData->getIndividualAssociationDataDirectUpdateCount();
						cint64 indiMergesCount = ontologyData->getIndividualAssociationMergingCount();

						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiIdAssoDataVector = ontologyData->getIndividualIdAssoiationDataVector();
						cint64 indiIdAssoDataVectorSize = ontologyData->getIndividualIdAssoiationDataVectorSize();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** basicPrecompIndiIdAssoDataVector = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualAssociationData* >::allocateAndConstructArray(context->getMemoryAllocationManager(), indiIdAssoDataVectorSize);
						for (cint64 i = 0; i < indiIdAssoDataVectorSize; ++i) {
							basicPrecompIndiIdAssoDataVector[i] = indiIdAssoDataVector[i];
						}
						ontologyData->setBasicPrecomputationIndividualIdAssoiationDataVector(indiIdAssoDataVectorSize, basicPrecompIndiIdAssoDataVector);

						LOG(INFO, "::Konclude::RepresentativeBackendCache", QString("Activated basic precompuation mode for %1 merges at %2 updates\r\n").arg(indiMergesCount).arg(basicIndiUpdateCount), this);

						return true;
					}
					return false;
				}



				void CBackendRepresentativeMemoryCache::indexIndividualLabelAssociations(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					cint64 indexingCount = 0;
					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						if (requiresIndividualAssociations(i)) {
							indexingCount++;
						}
					}
					ontologyData->setIndividualLabelAssociationIndexingCount(indexingCount);
					cint64 indiCount = ontologyData->getMaxStoredIndvidualiId();
					cint64 vecSize = ontologyData->getIndividualIdAssoiationDataVectorSize();
					LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Starting concurrent indexing of %1 label types for %2 individuals.").arg(indexingCount).arg(indiCount), this);

					for (cint64 i = 0; i < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++i) {
						bool exactIndiAssocTracking = requiresIndividualAssociations(i);
						if (exactIndiAssocTracking) {
							CConcurrentTaskScheduler::run([=] {


								CMemoryPoolNewAllocationIncreasingContext newAllocContext;

								for (cint64 indiId = 0; indiId <= indiCount && indiId < vecSize; ++indiId) {
									CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
									if (indiAssData) {

										CBackendRepresentativeMemoryLabelCacheItem* labelItem = indiAssData->getLabelCacheEntry(i);
										if (labelItem) {
											CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* indiAssoExtData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
											if (!indiAssoExtData) {
												indiAssoExtData = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData, CMemoryPoolNewAllocationIncreasingContext* >::allocateAndConstructAndParameterize(newAllocContext.getMemoryAllocationManager(), &newAllocContext);
												labelItem->setExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP, indiAssoExtData);
											}
											indiAssoExtData->addIndividualIdAssociation(indiAssData);
										}
									}
								}
								CMemoryPool* memoryPools = newAllocContext.takeMemoryPools();
								cint64 remainingCount = ontologyData->updateIndividualLabelAssociationIndexed(true, memoryPools);
								if (remainingCount <= 0) {
									LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Finished indexing all label types for %1 individuals.").arg(indiCount), this);
								} else {
									LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Finished indexing %1 label types.").arg(indexingCount - remainingCount), this);
								}


							});
						}
					}

					if (mConfWaitIndividualLabelAssociationIndexed) {
						LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Waiting indexing finishing."), this);
						ontologyData->waitIndividualLabelAssociationIndexed();
					}

				}





				bool CBackendRepresentativeMemoryCache::integratePropagationCut(CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* tmpPropCutDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;
					if (tmpPropCutDataLinker) {
						mLastHandledPropCutDataLinker = tmpPropCutDataLinker;
						cint64 propCutIndiCount = 0;
						cint64 propCutInfluencedIndiCount = 0;
						cint64 propCutAllNeighbourRelationsInfluencingCount = 0;
						cint64 propCutNeighbourNonDetermininisticRelationInfluencingCount = 0;
						cint64 propCutNeighbourRelationPropagatedConceptMissingInfluencingCount = 0;
						cint64 propCutNeighbourDirectConceptInfluenceUpdatingCount = 0;
						cint64 propCutNeighbourRelationUnspecifiedInfluencingCount = 0;
						CBackendRepresentativeMemoryCacheContext* context = ontologyData->getOntologyContext();
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiAssDataVector = ontologyData->getIndividualIdAssoiationDataVector();

						QSet<cint64> allNeighbourRelationsConsideredIndiSet;
						QHash<cint64, cint64> incompletelyUpdatingNeighbourPropCutIndisHash;
						QSet<QPair<cint64, cint64>> incompletelyUpdatingNeighbourPropCutIndiPairSet;

						QSet<cint64> expandedIndiSet;
						CXLinker<cint64>* lastInvestigatedExpandedIndiLinker = nullptr;

						QHash<cint64, cint64> propagationCutIndiReferredNeighbourArrayIdHash;

						for (CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* tmpPropCutDataLinkerIt = tmpPropCutDataLinker; tmpPropCutDataLinkerIt; tmpPropCutDataLinkerIt = tmpPropCutDataLinkerIt->getNext()) {
							CXLinker<cint64>* expandedIndiLinker = tmpPropCutDataLinkerIt->getExpandedIndividualIdsLinker();
							if (lastInvestigatedExpandedIndiLinker != expandedIndiLinker) {
								lastInvestigatedExpandedIndiLinker = expandedIndiLinker;
								expandedIndiSet.clear();
								for (CXLinker<cint64>* expandedIndiLinkerIt = expandedIndiLinker; expandedIndiLinkerIt; expandedIndiLinkerIt = expandedIndiLinkerIt->getNext()) {
									cint64 expIndiId = expandedIndiLinkerIt->getData();
									expandedIndiSet.insert(expIndiId);
								}
							}
							cint64 propCutIndiId = tmpPropCutDataLinkerIt->getIndividualID();

							mPropagationCutIndiSet.insert(propCutIndiId);
							cint64 assocUpdateId = tmpPropCutDataLinkerIt->getAssociationUpdateId();
							cint64 visitingCursor = tmpPropCutDataLinkerIt->getNeighbourPropagationCutCursor();
							CCacheValue propConValue = tmpPropCutDataLinkerIt->getConceptPropagationValue();
							bool missingNondeterministicExpansionPropagation = tmpPropCutDataLinkerIt->isMissingNondeterministicExpansionPropagation();
							bool allNeighbourRelationsConsiderationRequired = false;
							CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData = indiAssDataVector[propCutIndiId];
							if (indiAssocData->getAssociationDataUpdateId() != assocUpdateId) {
								allNeighbourRelationsConsiderationRequired = true;
							}
							if (propConValue.getIdentification() == 0) {
								allNeighbourRelationsConsiderationRequired = true;
							}
							if (!allNeighbourRelationsConsiderationRequired || !allNeighbourRelationsConsideredIndiSet.contains(propCutIndiId)) {
								if (allNeighbourRelationsConsiderationRequired) {
									if (!missingNondeterministicExpansionPropagation) {
										allNeighbourRelationsConsideredIndiSet.insert(propCutIndiId);
									}


									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighArray = indiAssocData->getRoleSetNeighbourArray();
									for (cint64 i = 0; i < neighArray->getIndexData()->getArraySize(); ++i) {
										bool hasNonExpanded = false;
										neighArray->at(i).visitNeighbourIndividualIdsFromCursor([&](cint64 neighbourId, cint64 nextCursor)->bool {
											if (!expandedIndiSet.contains(neighbourId)) {
												hasNonExpanded = true;
												CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssocData = indiAssDataVector[neighbourId];
												// set as incompletely handled and add non-deterministic top property link

												bool neighbourPropagationAffected = false;
												if (missingNondeterministicExpansionPropagation) {
													CBackendRepresentativeMemoryLabelCacheItem* neighbourConSetLabel = neighbourIndiAssocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
													CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* cardExtData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)neighbourConSetLabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
													if (cardExtData || neighbourConSetLabel->hasNondeterministicElements()) {
														neighbourPropagationAffected = true;
													}
												} else {
													neighbourPropagationAffected = true;
												}
												if (neighbourPropagationAffected && !incompletelyUpdatingNeighbourPropCutIndiPairSet.contains(QPair<cint64, cint64>(neighbourId, propCutIndiId))) {
													propCutAllNeighbourRelationsInfluencingCount++;
													incompletelyUpdatingNeighbourPropCutIndiPairSet.insert(QPair<cint64, cint64>(neighbourId, propCutIndiId));
													incompletelyUpdatingNeighbourPropCutIndisHash.insertMulti(neighbourId, propCutIndiId);
												}
											}
											return true;
										}, 0);
										if (hasNonExpanded) {
											bool hasNonDetElements = neighArray->getIndexData()->getNeighbourRoleSetLabel(i)->hasNondeterministicElements();
											if (!hasNonDetElements) {
												propagationCutIndiReferredNeighbourArrayIdHash.insertMulti(propCutIndiId, i);
											}
										}
									}


								} else {
									cint64 arrayIndexId = tmpPropCutDataLinkerIt->getArrayIndex();
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighArray = indiAssocData->getRoleSetNeighbourArray();
									bool hasNonExpanded = false;
									bool hasNonDetElements = neighArray->getIndexData()->getNeighbourRoleSetLabel(arrayIndexId)->hasNondeterministicElements();
									neighArray->at(arrayIndexId).visitNeighbourIndividualIdsFromCursor([&](cint64 neighbourId, cint64 nextCursor)->bool {
										if (!expandedIndiSet.contains(neighbourId)) {
											hasNonExpanded = true;
											CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssocData = indiAssDataVector[neighbourId];
											bool neighbourPropagationAffected = false;
											if (hasNonDetElements) {
												neighbourPropagationAffected = true;
												++propCutNeighbourNonDetermininisticRelationInfluencingCount;
											} else if (propConValue.getIdentification() != 0) {
												CBackendRepresentativeMemoryLabelCacheItem* neighbourConSetLabel = neighbourIndiAssocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
												CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinker = neighbourConSetLabel->getTagCacheValueHash(false)->value(propConValue.getTag());
												if (!cacheValueLinker) {
													// only for deterministically propagated concepts
													if (mConfPropagationCutPropagatedConceptDirectInstallation && (propConValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDCONCEPT || propConValue.getCacheValueIdentifier() == CCacheValue::CACHEVALTAGANDNEGATEDCONCEPT)) {
														updatePropagationCutIndividualIncompletelyHandled(neighbourId, propConValue, ontologyData);
														propCutNeighbourDirectConceptInfluenceUpdatingCount++;
														++propCutInfluencedIndiCount;
														updated = true;
													} else {
														neighbourPropagationAffected = true;
														propCutNeighbourRelationPropagatedConceptMissingInfluencingCount++;
													}
												}
											} else if (missingNondeterministicExpansionPropagation) {											
												CBackendRepresentativeMemoryLabelCacheItem* neighbourConSetLabel = neighbourIndiAssocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);
												CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* cardExtData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)neighbourConSetLabel->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
												if (cardExtData || neighbourConSetLabel->hasNondeterministicElements()) {
													propCutNeighbourRelationUnspecifiedInfluencingCount++;
													neighbourPropagationAffected = true;
												}
											} else {
												propCutNeighbourRelationUnspecifiedInfluencingCount++;
												neighbourPropagationAffected = true;
											}

											if (neighbourPropagationAffected) {
												// set as incompletely handled and add non-deterministic top property link
												if (!incompletelyUpdatingNeighbourPropCutIndiPairSet.contains(QPair<cint64, cint64>(neighbourId, propCutIndiId))) {
													incompletelyUpdatingNeighbourPropCutIndiPairSet.insert(QPair<cint64, cint64>(neighbourId, propCutIndiId));
													incompletelyUpdatingNeighbourPropCutIndisHash.insertMulti(neighbourId, propCutIndiId);
												}
											}

										}
										return true;
									}, visitingCursor);
									if (!hasNonDetElements && hasNonExpanded) {
										propagationCutIndiReferredNeighbourArrayIdHash.insertMulti(propCutIndiId, arrayIndexId);
									}
								}
							
							}
						}


						propCutIndiCount = mPropagationCutIndiSet.count();


						cint64 lastPropCutIndiId = -1;
						for (QHash<cint64, cint64>::const_iterator it = propagationCutIndiReferredNeighbourArrayIdHash.constBegin(), itEnd = propagationCutIndiReferredNeighbourArrayIdHash.constEnd(); it != itEnd; ++it) {
							cint64 propCutIndiId = it.key();
							if (lastPropCutIndiId != propCutIndiId) {
								lastPropCutIndiId = propCutIndiId;



								QList<cint64> referredNeighbourArrayIdList = propagationCutIndiReferredNeighbourArrayIdHash.values(propCutIndiId);
								QSet<cint64> referredNeighbourArrayIdSet = referredNeighbourArrayIdList.toSet();
								CBackendRepresentativeMemoryCacheIndividualAssociationData* propCutIndiAssocData = indiAssDataVector[propCutIndiId];
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* neighArray = propCutIndiAssocData->getRoleSetNeighbourArray();
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = neighArray->getIndexData();
								CPropagationCutNeighbourArrayHandlingData* handlingData = nullptr;
								for (cint64 i = 0; i < arrayIndexData->getArraySize(); ++i) {
									bool hasExpandableNeighbours = referredNeighbourArrayIdSet.contains(i);
									CBackendRepresentativeMemoryLabelCacheItem* neighbourLabelItem = neighArray->getIndexData()->getNeighbourRoleSetLabel(i);
									bool hasNonDetElements = neighbourLabelItem->hasNondeterministicElements();
									if (!hasExpandableNeighbours) {
										neighArray->at(i).visitNeighbourIndividualIdsFromCursor([&](cint64 neighbourId, cint64 nextCursor)->bool {
											if (!expandedIndiSet.contains(neighbourId)) {
												hasExpandableNeighbours = true;
												return false;
											}
											return true;
										}, 0);
									}


									if (hasExpandableNeighbours) {
										if (!mTmpPropCutIndiArrayNeighboursHandlingDataHash) {
											mTmpPropCutIndiArrayNeighboursHandlingDataHash = new QHash<cint64, CPropagationCutNeighbourArrayHandlingData *>();
										}
										if (!handlingData) {
											CPropagationCutNeighbourArrayHandlingData*& tmpHandlingData = (*mTmpPropCutIndiArrayNeighboursHandlingDataHash)[propCutIndiId];
											if (!tmpHandlingData) {
												tmpHandlingData = new CPropagationCutNeighbourArrayHandlingData();
											}
											handlingData = tmpHandlingData;
										}
										if (handlingData) {
											if (!hasNonDetElements) {
												handlingData->mReaddingArrayPosSet.insert(i);
											} else if (neighbourLabelItem->getCacheValueCount() > 1 || neighbourLabelItem->getCacheValueLinker()->getCacheValue().getTag() != 1) {
												bool allNondeterministic = true;
												for (CBackendRepresentativeMemoryLabelValueLinker* cacheValueLinkerIt = neighbourLabelItem->getCacheValueLinker(); cacheValueLinkerIt && allNondeterministic; cacheValueLinkerIt = cacheValueLinkerIt->getNext()) {
													if (!isCacheValueRoleNondeterministic(cacheValueLinkerIt->getCacheValue())) {
														allNondeterministic = false;
													}
												}
												if (allNondeterministic) {
													handlingData->mRemovalArrayPosSet.insert(i);
												} else {
													handlingData->mReductionArrayPosSet.insert(i);
												}
											}
										}
									}
								}
							}
						}


						if (!incompletelyUpdatingNeighbourPropCutIndisHash.isEmpty()) {

							CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem = ontologyData->getPrioritizedPropagationMarkedNeighbourLabelItem();
							CCacheValue propMarkRoleCacheValue = tmpPropCutDataLinker->getPropagationMarkingRoleValue();
							if (!propMarkLabelItem) {
								CBackendRepresentativeMemoryLabelSignatureResolveCacheItem& sigResolCacheItem = (*ontologyData->getSignatureLabelItemHash(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL))[propMarkRoleCacheValue.getTag()];
								for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolCacheItem.getLabelItems(); labelItemLinkerIt && !propMarkLabelItem; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
									if (labelItemLinkerIt->getCacheValueCount() == 1) {
										if (labelItemLinkerIt->getCacheValueLinker() && labelItemLinkerIt->getCacheValueLinker()->getCacheValue() == propMarkRoleCacheValue) {
											propMarkLabelItem = labelItemLinkerIt;
										}
									}
								}
								if (!propMarkLabelItem) {
									propMarkLabelItem = CObjectParameterizingAllocator< CBackendRepresentativeMemoryLabelCacheItem, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
									propMarkLabelItem->initCacheEntry(propMarkRoleCacheValue.getTag(), ontologyData->getNextEntryID(), CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL);

									CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelValueLinker*>* labelValueHash = propMarkLabelItem->getTagCacheValueHash(true);
									CBackendRepresentativeMemoryLabelValueLinker* newValueLinker = CObjectAllocator< CBackendRepresentativeMemoryLabelValueLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
									newValueLinker->initLabelValueLinker(propMarkRoleCacheValue);
									labelValueHash->insert(propMarkRoleCacheValue.getTag(), newValueLinker);
									propMarkLabelItem->addCacheValueLinker(newValueLinker);

									sigResolCacheItem.appendLabelItem(propMarkLabelItem);
									ontologyData->setPrioritizedPropagationMarkedNeighbourLabelItem(propMarkLabelItem);
								}
							}


							cint64 lastNeighbour = -1;
							for (QHash<cint64, cint64>::const_iterator it = incompletelyUpdatingNeighbourPropCutIndisHash.constBegin(), itEnd = incompletelyUpdatingNeighbourPropCutIndisHash.constEnd(); it != itEnd; ++it) {
								cint64 neighbourId = it.key();
								if (lastNeighbour != neighbourId) {
									lastNeighbour = neighbourId;
									QList<cint64> propCutIndiIds = incompletelyUpdatingNeighbourPropCutIndisHash.values(neighbourId);
									if (updatePropagationCutIndividualIncompletelyHandled(neighbourId, propCutIndiIds, propMarkLabelItem, ontologyData)) {
										++propCutInfluencedIndiCount;
										updated = true;
									}
								}
							}

						}

						QString propCutIndisString = QString("%1 individuals").arg(propCutIndiCount);
						bool debuggingIndi = false;
						if (mStatCollectStatistics && propCutIndiCount < 5) {
							QStringList propCutIndStringList;
							for (cint64 propCutIndiId : mPropagationCutIndiSet) {
								QString indiString = QString::number(propCutIndiId);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
								CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData = indiAssDataVector[propCutIndiId];
								if (indiAssocData->mDebugIndi) {
									QString indiName = CIRIName::getRecentIRIName(indiAssocData->mDebugIndi->getIndividualNameLinker());
									indiString = QString("%2 with id %1").arg(propCutIndiId).arg(indiName);
								}
#endif
								propCutIndStringList.append(indiString);
							}
							propCutIndisString += QString(" (%1)").arg(propCutIndStringList.join(", "));
						}


#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						if (debuggingIndi) {
							QStringList modelStringList = getRepresentativeCacheString();
							modelStringList.append(QString("\r\n\r\nReceived propagation cut for %1 influencing %2 neighbour individuals (%3 from all relations, %4 due to non deterministic relations, %5 due to propagated concept missing, %7 direct added propagated concept, %6 unspecified)\r\n")
								.arg(propCutIndisString).arg(propCutInfluencedIndiCount).arg(propCutAllNeighbourRelationsInfluencingCount).arg(propCutNeighbourNonDetermininisticRelationInfluencingCount).arg(propCutNeighbourRelationPropagatedConceptMissingInfluencingCount).arg(propCutNeighbourRelationUnspecifiedInfluencingCount).arg(propCutNeighbourDirectConceptInfluenceUpdatingCount));

							for (QHash<cint64, cint64>::const_iterator it = incompletelyUpdatingNeighbourPropCutIndisHash.constBegin(), itEnd = incompletelyUpdatingNeighbourPropCutIndisHash.constEnd(); it != itEnd; ++it) {
								modelStringList.append(QString("propagation cut individual %1 influenced %2\r\n").arg(it.key()).arg(it.value()));
							}

							QFile modelFile(QString("./Debugging/RepresentativeCache/individual-label-association-data-%1-propagation-cut.txt").arg(mWriteDataCount));
							if (modelFile.open(QIODevice::WriteOnly)) {
								for (const QString& modelString : modelStringList) {
									QByteArray data = modelString.toUtf8();
									modelFile.write(data);
								}
								modelFile.close();
							}
						}
#endif

						LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Received propagation cut for %1 influencing %2 neighbour individuals (%3 from all relations, %4 due to non deterministic relations, %5 due to propagated concept missing, %7 direct added propagated concept, %6 unspecified).")
							.arg(propCutIndisString).arg(propCutInfluencedIndiCount).arg(propCutAllNeighbourRelationsInfluencingCount).arg(propCutNeighbourNonDetermininisticRelationInfluencingCount).arg(propCutNeighbourRelationPropagatedConceptMissingInfluencingCount).arg(propCutNeighbourRelationUnspecifiedInfluencingCount).arg(propCutNeighbourDirectConceptInfluenceUpdatingCount), this);
					}
					return updated;
				}



				bool CBackendRepresentativeMemoryCache::updatePropagationCutIndividualIncompletelyHandled(cint64 neighbourId, const CCacheValue& propConValue, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData** indiAssDataVector = ontologyData->getIndividualIdAssoiationDataVector();
					CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssocData = indiAssDataVector[neighbourId];

					CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData = createLocalizedIndividualAssociationData(neighbourIndiAssocData->getAssociatedIndividualId(), neighbourIndiAssocData, ontologyData, false);
					CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locNeighbourAssociationData, ontologyData);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					locNeighbourAssociationData->mDebugUpdateFunction = QString("updatePropagationCutIndividualIncompletelyHandled");
					locNeighbourAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif

					locNeighbourAssociationData->setNeighbourRoleSetHash(neighbourIndiAssocData->getNeighbourRoleSetHash());
					locNeighbourAssociationData->setRoleSetNeighbourArray(neighbourIndiAssocData->getRoleSetNeighbourArray());

					CBackendRepresentativeMemoryLabelCacheItem* conSetLabel = neighbourIndiAssocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL);

					CBackendRepresentativeMemoryLabelCacheItem* newConSetLabel = getExtendedLabel(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, conSetLabel, propConValue, ontologyData);
					locNeighbourAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL, newConSetLabel);
					locNeighbourAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
					ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locNeighbourAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

					++mUpdatedIndiCount;

					if (locNeighbourAssociationData->isCompletelyHandled() && locNeighbourAssociationData->hasRepresentativeSameIndividualMerging()) {
						markRepresentativeReferencedIndividualAssociationIncompletelyHandled(neighbourId, locNeighbourAssociationData, ontologyData);
					} else if (locNeighbourAssociationData->isCompletelyHandled()) {
						locNeighbourAssociationData->setCompletelyHandled(false);
						storeIndividualIncompletelyMarked(locNeighbourAssociationData, true, ontologyData);
					}
					setUpdatedIndividualAssociationData(locNeighbourAssociationData->getAssociatedIndividualId(), locNeighbourAssociationData, ontologyData);

					return true;
				}



				bool CBackendRepresentativeMemoryCache::updatePropagationCutIndividualIncompletelyHandled(cint64 neighbourId, const QList<cint64>& propCutIndiIdList, CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					CBackendRepresentativeMemoryCacheIndividualAssociationData** indiAssDataVector = ontologyData->getIndividualIdAssoiationDataVector();
					CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssocData = indiAssDataVector[neighbourId];
					if (!neighbourIndiAssocData->hasRepresentativeSameIndividualMerging()) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData = createLocalizedIndividualAssociationData(neighbourIndiAssocData->getAssociatedIndividualId(), neighbourIndiAssocData, ontologyData, false);
						bool requiresDataCopying = false;
						CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locNeighbourAssociationData, ontologyData, &requiresDataCopying);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						locNeighbourAssociationData->mDebugUpdateFunction = QString("updatePropagationCutIndividualIncompletelyHandled");
						locNeighbourAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif
						CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetCompLabel = neighbourIndiAssocData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
						CBackendRepresentativeMemoryLabelValueLinker* propMarkTagCacheValueLinker = neighbourRoleSetCompLabel->getTagCacheValueHash()->value(propMarkLabelItem->getCacheValueLinker()->getData().getTag());
						CBackendRepresentativeMemoryLabelCacheItem* newNeighbourRoleSetCompLabel = neighbourRoleSetCompLabel;
						if (!propMarkTagCacheValueLinker || propMarkTagCacheValueLinker->getData() != propMarkLabelItem->getCacheValueLinker()->getData()) {
							CCacheValue extendingCacheValue;
							extendingCacheValue.initCacheValue(propMarkLabelItem->getCacheEntryID(), (cint64)propMarkLabelItem, CCacheValue::CACHE_VALUE_TAG_AND_ENTRY);
							newNeighbourRoleSetCompLabel = getExtendedLabel(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, neighbourRoleSetCompLabel, extendingCacheValue, ontologyData);
						}

						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* prevArrayData = neighbourIndiAssocData->getRoleSetNeighbourArray();
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArrayData->getIndexData();
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* newArrayIndexData = prevArrayIndexData;
						if (newNeighbourRoleSetCompLabel == neighbourRoleSetCompLabel) {
							newArray->initNeighbourArray(neighbourIndiAssocData->getRoleSetNeighbourArray());
						} else {
							locNeighbourAssociationData->setLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, newNeighbourRoleSetCompLabel);
							updateIndexedAssociationCount(locNeighbourAssociationData, neighbourIndiAssocData, CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL, ontologyData);
							newArrayIndexData = getIndividualNeighbourArrayIndexExtensionData(newNeighbourRoleSetCompLabel, ontologyData);
							newArray->initNeighbourArray(newArrayIndexData);

							for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {
								CBackendRepresentativeMemoryLabelCacheItem* referredLabelCacheItem = newArrayIndexData->getNeighbourRoleSetLabel(i);
								cint64 prevIndex = prevArrayIndexData->getIndex(referredLabelCacheItem);
								if (prevIndex >= 0) {
									newArray->at(i) = prevArrayData->at(prevIndex);
								}
							}
						}
						QSet<cint64> existingPropCutIndiSet;
						cint64 propMarkIndex = newArrayIndexData->getIndex(propMarkLabelItem);
						newArray->at(propMarkIndex).visitNeighbourIndividualIds([&](cint64 indi)->bool {
							existingPropCutIndiSet.insert(indi);
							return true;
						});

						for (cint64 propCutIndi : propCutIndiIdList) {
							if (!existingPropCutIndiSet.contains(propCutIndi)) {
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
								++mStatCreatedNeighbourLinks;
								newLinker->initIndividualIdLinker(propCutIndi);
								newArray->at(propMarkIndex).addIndividualIdLinker(newLinker);
								existingPropCutIndiSet.insert(propCutIndi);
							}
						}

						newNeighbourRoleSetHash->initNeighbourRoleSetHash(neighbourIndiAssocData->getNeighbourRoleSetHash(), requiresDataCopying);

						locNeighbourAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
						locNeighbourAssociationData->setRoleSetNeighbourArray(newArray);

						locNeighbourAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
						ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locNeighbourAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

						++mUpdatedIndiCount;

						locNeighbourAssociationData->setCompletelyHandled(false);
						storeIndividualIncompletelyMarked(locNeighbourAssociationData, true, ontologyData);
						setUpdatedIndividualAssociationData(locNeighbourAssociationData->getAssociatedIndividualId(), locNeighbourAssociationData, ontologyData);

						return true;
					}

					return false;
				}



				bool CBackendRepresentativeMemoryCache::updateInvolvedIndividuals(CBackendRepresentativeMemoryCacheTemporaryInvolvedIndividualDataLinker* tmpInvolvedIndiDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool updated = false;
					if (tmpInvolvedIndiDataLinker) {
						QSet<cint64> newInvolvedIndiSet;
						QSet<cint64> addedInvolvedIndiStatusSet;
						cint64 involvedIndiCount = 0;
						cint64 newInvolvedIndiCount = 0;
						cint64 involvedIndiUpdatingCount = 0;
						CBackendRepresentativeMemoryCacheIndividualAssociationData** indiAssDataVector = ontologyData->getIndividualIdAssoiationDataVector();
						for (CXLinker<cint64>* indiLinkerIt = tmpInvolvedIndiDataLinker->getInvolvedIndividualIdsLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
							cint64 indiId = indiLinkerIt->getData();
							newInvolvedIndiSet.insert(indiId);

							CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData = indiAssDataVector[indiId];
							bool addedInvolvedStatus = false;
							if (!indiAssocData->hasProblematicLevel()) {
								ontologyData->incInvolvedIndividualCount();
								addedInvolvedStatus = true;
								addedInvolvedIndiStatusSet.insert(indiId);
								++newInvolvedIndiCount;
							}
							indiAssocData->incProblematicLevel();
							++involvedIndiCount;
						}


						QSet<cint64> neighbourInvolvedIndiArrayUpdateSet;
						for (CXLinker<cint64>* indiLinkerIt = tmpInvolvedIndiDataLinker->getInvolvedIndividualIdsLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
							cint64 indiId = indiLinkerIt->getData();
							if (addedInvolvedIndiStatusSet.contains(indiId)) {
								CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssocData = indiAssDataVector[indiId];
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleAssNeighArray = indiAssocData->getRoleSetNeighbourArray();
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* arrayIndexData = roleAssNeighArray->getIndexData();
								if (arrayIndexData) {
									for (cint64 i = 0; i < arrayIndexData->getArraySize(); ++i) {
										for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinkerIt = roleAssNeighArray->at(i).getIndividualIdLinker(); neighbourIndiLinkerIt; neighbourIndiLinkerIt = neighbourIndiLinkerIt->getNext()) {
											cint64 neighbourIndiId = neighbourIndiLinkerIt->getIndividualId();
											neighbourInvolvedIndiArrayUpdateSet.insert(neighbourIndiId);
										}
									}
								}
								for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* neighbourIndiLinkerIt = indiAssocData->getPropagationCutRemovedNeighbourIndividualLinker(); neighbourIndiLinkerIt; neighbourIndiLinkerIt = neighbourIndiLinkerIt->getNext()) {
									cint64 neighbourIndiId = neighbourIndiLinkerIt->getIndividualId();
									neighbourInvolvedIndiArrayUpdateSet.insert(neighbourIndiId);
								}
							}
						}


						for (QSet<cint64>::const_iterator it = neighbourInvolvedIndiArrayUpdateSet.constBegin(), itEnd = neighbourInvolvedIndiArrayUpdateSet.constEnd(); it != itEnd; ++it) {
							cint64 indiId = *it;
							++involvedIndiUpdatingCount;

							CBackendRepresentativeMemoryCacheIndividualAssociationData* prevIndiAssocData = indiAssDataVector[indiId];
							CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData = createLocalizedIndividualAssociationData(prevIndiAssocData->getAssociatedIndividualId(), prevIndiAssocData, ontologyData, false);
							bool requiresDataCopying = false;
							CBackendRepresentativeMemoryCacheContext* context = getIndividualAssociationDataMemoryContext(locAssociationData, ontologyData, &requiresDataCopying);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
							locAssociationData->mDebugUpdateFunction = QString("updateInvolvedIndividuals");
							locAssociationData->mDebugUpdateRecomputationId = mCurrentUpdateHandlingRecomputationId;
#endif
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* prevArrayData = prevIndiAssocData->getRoleSetNeighbourArray();
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* prevArrayIndexData = prevArrayData->getIndexData();

							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* newArrayIndexData = prevArrayIndexData;


							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* newArray = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash* newNeighbourRoleSetHash = CObjectParameterizingAllocator< CBackendRepresentativeMemoryCacheIndividualNeighbourRoleSetHash, CBackendRepresentativeMemoryCacheContext* >::allocateAndConstructAndParameterize(context->getMemoryAllocationManager(), context);
							newArray->initNeighbourArray(newArrayIndexData);
							newNeighbourRoleSetHash->initNeighbourRoleSetHash(prevIndiAssocData->getNeighbourRoleSetHash(), requiresDataCopying);

							for (cint64 i = 0; i < newArrayIndexData->getArraySize(); ++i) {

								QSet<cint64> updatingInolvedNeighbourIndiSet;
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* firstNewLinker = nullptr;
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* lastNewLinker = nullptr;

								prevArrayData->at(i).visitNeighbourIndividualIds([&](cint64 neighbourIndiId)->bool {

									if (!addedInvolvedIndiStatusSet.contains(neighbourIndiId)) {
										CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
										++mStatCreatedNeighbourLinks;
										newLinker->initIndividualIdLinker(neighbourIndiId);
										if (lastNewLinker) {
											lastNewLinker->append(newLinker);
											lastNewLinker = newLinker;
										} else {
											firstNewLinker = newLinker;
											lastNewLinker = newLinker;
										}
									} else {
										updatingInolvedNeighbourIndiSet.insert(neighbourIndiId);
									}
									return true;
								});


								// add problematic/involved neighbour ids to the beginning
								for (QSet<cint64>::const_iterator updateIt = updatingInolvedNeighbourIndiSet.constBegin(), updateItEnd = updatingInolvedNeighbourIndiSet.constEnd(); updateIt != updateItEnd; ++updateIt) {
									cint64 updatingIndiId = *updateIt;

									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
									++mStatCreatedNeighbourLinks;
									newLinker->initIndividualIdLinker(updatingIndiId);
									firstNewLinker = newLinker->append(firstNewLinker);
								}
								if (firstNewLinker) {
									newArray->at(i).addIndividualIdLinker(firstNewLinker);
								}

							}

							locAssociationData->setNeighbourRoleSetHash(newNeighbourRoleSetHash);
							locAssociationData->setRoleSetNeighbourArray(newArray);

							locAssociationData->setProblematicLeveledNeigbour(true);

							locAssociationData->setCacheUpdateId(mNextIndiUpdateId++);
							ontologyData->setMaxIndividualAssociationDataUpdateCount(qMax(locAssociationData->getAssociationDataUpdateId(), ontologyData->getMaxIndividualAssociationDataUpdateCount()));

							++mUpdatedIndiCount;

							storeIndividualIncompletelyMarked(locAssociationData, !locAssociationData->isCompletelyHandled(), ontologyData);
							setUpdatedIndividualAssociationData(locAssociationData->getAssociatedIndividualId(), locAssociationData, ontologyData);

							updated = true;
						}

						LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Received clash influence for %1 individuals (new for %2 individuals), reordered neighbours for %3 individuals.").arg(involvedIndiCount).arg(newInvolvedIndiCount).arg(involvedIndiUpdatingCount), this);

					}
					return updated;
				}




				void CBackendRepresentativeMemoryCache::writeStringifiedRepresentativeCacheToFile() {
					QStringList modelStringList = getRepresentativeCacheString();
					QFile modelFileLatest("./Debugging/RepresentativeCache/individual-label-association-data.txt");
					if (modelFileLatest.open(QIODevice::WriteOnly)) {
						for (const QString& modelString  : modelStringList) {
							QByteArray data = modelString.toUtf8();
							modelFileLatest.write(data);
						}
						modelFileLatest.close();
					}
					QFile modelFile(QString("./Debugging/RepresentativeCache/individual-label-association-data-%1.txt").arg(mWriteDataCount));
					if (modelFile.open(QIODevice::WriteOnly)) {
						for (const QString& modelString : modelStringList) {
							QByteArray data = modelString.toUtf8();
							modelFile.write(data);
						}
						modelFile.close();
					}

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					QFile handlingFile(QString("./Debugging/RepresentativeCache/handling-data.txt"));
					if (handlingFile.open(QIODevice::WriteOnly)) {
						for (const QString& modelString : mDebugHandlingStrings) {
							QByteArray data = modelString.toUtf8();
							handlingFile.write(data);
							handlingFile.write(QString("\r\n").toUtf8());
						}
						handlingFile.close();
					}
#endif
				}



				QString CBackendRepresentativeMemoryCache::getRepresentativeCacheLabelItemString(cint64 labelType) {
					QString labelTypeString;
					if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_CONCEPT_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_CONCEPT_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_CONCEPT_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_CONCEPT_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {
						labelTypeString = "FULL_CONCEPT_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_EXISTENTIAL_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL) {
						labelTypeString = "NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_DIFFRENT_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL) {
						labelTypeString = "INDIRECTLY_CONNECTED_NOMINAL_INDIVIDUAL_SET_LABEL";
					}
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NEIGHBOUR_INSTANTIATED_ROLE_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "DETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL";
					} 
					else if (labelType == CBackendRepresentativeMemoryLabelCacheItem::NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL) {
						labelTypeString = "NONDETERMINISTIC_COMBINED_DATA_INSTANTIATED_ROLE_SET_LABEL";
					}
					return labelTypeString;
				}

				QStringList CBackendRepresentativeMemoryCache::getRepresentativeCacheString() {
					QStringList cacheStringList;
					for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>::const_iterator it = mOntologyIdentifierDataHash->constBegin(), itEnd = mOntologyIdentifierDataHash->constEnd(); it != itEnd; ++it) {
						cint64 ontId = it.key();
						CBackendRepresentativeMemoryCacheOntologyData* ontologyData = it.value();
						cacheStringList.append(QString("Ontology: %1\r\n\r\n\r\n").arg(ontId));
						QStringList ontologyIdDataStringList = getRepresentativeCacheString(ontologyData);
						cacheStringList += ontologyIdDataStringList;
						cacheStringList.append(QString("\r\n\r\n\r\n\r\n\r\n\r\n"));
					}
					return cacheStringList;
				}




				void CBackendRepresentativeMemoryCache::debugCheckWriteMaxSameMergedIndividualsToFile(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {

					CBackendRepresentativeMemoryLabelCacheItem* maxSameMergedItem = nullptr;
					cint64 maxSameMergedCount = 0;

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigLabelItemHash = ontologyData->getSignatureLabelItemHash(CBackendRepresentativeMemoryLabelCacheItem::DETERMINISTIC_SAME_INDIVIDUAL_SET_LABEL);
					for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>::const_iterator it = sigLabelItemHash->constBegin(), itEnd = sigLabelItemHash->constEnd(); it != itEnd; ++it) {

						CBackendRepresentativeMemoryLabelSignatureResolveCacheItem sigResolveItem = it.value();
						for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
							cint64 itemId = labelItemLinkerIt->getCacheEntryID();
							if (labelItemLinkerIt->getCacheValueCount() > maxSameMergedCount) {
								maxSameMergedCount = labelItemLinkerIt->getCacheValueCount();
								maxSameMergedItem = labelItemLinkerIt;
							}
						}

					}


					if (maxSameMergedItem) {
						QSet<QString> indiSet;
						CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = maxSameMergedItem->getCacheValueLinker();
						for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
							CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
							cint64 tag = cacheValue.getTag();
							CIndividual* individual = (CIndividual*)mDebugOntology->getABox()->getIndividualVector()->getData(tag);
							QString indiName = CIRIName::getRecentIRIName(individual->getNameLinker());
							indiSet.insert(indiName);
						}





						QSet<QString> indi1Set = indiSet;

						QSet<QString> indi2Set;
						QFile file2("./Debugging/RepresentativeCache/max-deterministically-merged-data.txt");
						cint64 count2 = 0;
						if (file2.open(QIODevice::ReadOnly)) {
							while (!file2.atEnd()) {
								QString line = file2.readLine().trimmed();
								indi2Set.insert(line);
							}
						}
						file2.close();

						for (QString indi2 : indi2Set) {
							if (!indi1Set.contains(indi2)) {
								bool debug = true;
							}
						}

						for (QString indi1 : indi1Set) {
							if (!indi2Set.contains(indi1)) {
								bool debug = true;
							}
						}

						bool writeFile = false;
						if (writeFile) {
							QFile sameAsMergedFile("./Debugging/RepresentativeCache/max-deterministically-merged-data.txt");
							if (sameAsMergedFile.open(QIODevice::WriteOnly)) {
								for (const QString& indiString : indiSet) {
									QByteArray data = indiString.toUtf8();
									sameAsMergedFile.write(data);
									sameAsMergedFile.write("\r\n");
								}
							}
							sameAsMergedFile.close();
						}



					}

				}


				QStringList CBackendRepresentativeMemoryCache::getRepresentativeCacheString(CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					QStringList cacheStringList;
					QStringList labelStringList;
					for (cint64 labelType = 0; labelType < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT; ++labelType) {
						QString labelTypeString = getRepresentativeCacheLabelItemString(labelType);
						labelTypeString = QString("\r\n\r\n%1:\r\n").arg(labelTypeString);
						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* sigLabelItemHash = ontologyData->getSignatureLabelItemHash(labelType);
						for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>::const_iterator it = sigLabelItemHash->constBegin(), itEnd = sigLabelItemHash->constEnd(); it != itEnd; ++it) {
							cint64 signature = it.key();
							CBackendRepresentativeMemoryLabelSignatureResolveCacheItem sigResolveItem = it.value();
							for (CBackendRepresentativeMemoryLabelCacheItem* labelItemLinkerIt = sigResolveItem.getLabelItems(); labelItemLinkerIt; labelItemLinkerIt = labelItemLinkerIt->getNext()) {
								cint64 itemId = labelItemLinkerIt->getCacheEntryID();
								QString labelString = QString("Label %1 (with signature %2, association count %3): ").arg(itemId).arg(signature).arg(labelItemLinkerIt->getIndividualAssociationCount());
								CBackendRepresentativeMemoryLabelValueLinker* labelValueLinker = labelItemLinkerIt->getCacheValueLinker();
								for (CBackendRepresentativeMemoryLabelValueLinker* labelValueLinkerIt = labelValueLinker; labelValueLinkerIt; labelValueLinkerIt = labelValueLinkerIt->getNext()) {
									CCacheValue cacheValue = labelValueLinkerIt->getCacheValue();
									if (labelValueLinkerIt != labelValueLinker) {
										labelString += ", ";
									}
									labelString += QString("%1(%2)").arg(cacheValue.getTag()).arg((cint64)cacheValue.getCacheValueIdentifier());
								}
								if (labelType == CBackendRepresentativeMemoryLabelCacheItem::FULL_CONCEPT_SET_LABEL) {
									CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData* cardExtData = (CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData*)labelItemLinkerIt->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData::CARDINALITY_HASH);
									if (cardExtData) {
										QString cardDataString;
										CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>* roleTagCardDataHash = cardExtData->getRoleCardinalityDataHash();
										for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItemCardinalityData*>::const_iterator it = roleTagCardDataHash->constBegin(), itEnd = roleTagCardDataHash->constEnd(); it != itEnd; ++it) {
											cint64 roleTag = it.key();
											CBackendRepresentativeMemoryLabelCacheItemCardinalityData* cardData = it.value();
											if (!cardDataString.isEmpty()) {
												cardDataString += "; ";
											}
											cardDataString += QString("%1 -> # %2 with <= %3").arg(roleTag).arg(cardData->getExistentialMaxUsedCardinality()).arg(cardData->getMinimumRestrictingCardinality());
										}
										labelString += QString("\r\n\t\t%1").arg(cardDataString);
									}
								}




								labelTypeString += QString("%1\r\n").arg(labelString);
							}
						}
						labelStringList.append(labelTypeString);
					}


					QStringList indiStringList;
					for (cint64 i = 0; i <= ontologyData->getMaxStoredIndvidualiId() && i < ontologyData->getIndividualIdAssoiationDataVectorSize(); ++i) {
					//for (CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>::const_iterator it = mIndiIdAssoDataHash->constBegin(), itEnd = mIndiIdAssoDataHash->constEnd(); it != itEnd; ++it) {
						cint64 indiId = i;
						CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData = ontologyData->getIndividualIdAssoiationDataVector()[indiId];
						if (indiAssData) {
							//QString indiName = CIRIName::getRecentIRIName(indiAssData->mDebugIndi->getNameLinker());
							//if (i < 100 || indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Tennis" || indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#BasketBall" || indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Baseball"
							//	|| indiName == "http://www.Department1.University0.edu/StudentWhoLikesTennisButDislikesBasketBallAndBaseBall" || indiName == "http://www.Department1.University0.edu/StudentWhoLikesBasketBallButDislikesBaseBall"
							//	|| indiName == "http://www.Department1.University0.edu/StudentWhoLikesTennisAndBasketBallAndBaseBallAndShouldHaveManyHobbies" || mDebugIndiId == i) {

								if (indiAssData && indiAssData->getAssociationDataUpdateId() >= 0) {
									QString indiAssString = getIndividualAssociationDebugString(indiId, indiAssData, ontologyData, 0);
									indiStringList.append(indiAssString);

									cint64 prevCount = 0;
									CBackendRepresentativeMemoryCacheIndividualAssociationData* prevIndiAssData = indiAssData->getPreviousData();
									while (prevIndiAssData && !indiAssData->getIndividualAssociationMemoryContext() && prevCount <= 10) {
										QString prevIndiAssString = getIndividualAssociationDebugString(indiId, prevIndiAssData, ontologyData, ++prevCount);
										indiStringList.append(prevIndiAssString);

										prevIndiAssData = prevIndiAssData->getPreviousData();
									}
								}
							//}
						}

					}

					QString statusString = QString("\tcache writes: %1,\r\n\ttotal association changes: %2,\r\n\tincompletely handled individuals: %3,\r\n\tincompatible association changes: %4,\r\n\tincompatible association checks: %5,\r\n\tmax individual association changes: %6\r\n")
						.arg(mWriteDataCount).arg(mNextIndiUpdateId).arg(ontologyData->getIncompletelyHandledIndividualIdCount()).arg(mUpdateIncompatibleIndiCount).arg(mCheckIncompatibleIndiCount).arg(ontologyData->getMaxIndividualAssociationDataUpdateCount());

					cacheStringList.append(QString("STATISTICS:\r\n\r\n%1\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n").arg(statusString));
					cacheStringList.append(QString("REPRESENATIVE LABELS:\r\n\r\n"));
					for (const QString& stringLine : labelStringList) {
						cacheStringList.append(QString("%1\r\n").arg(stringLine));
					}
					cacheStringList.append(QString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\nINDIVIDUAL LABEL ASSOCIATIONS:\r\n"));
					for (const QString& stringLine : indiStringList) {
						cacheStringList.append(QString("%1\r\n").arg(stringLine));
					}

					cacheStringList.append(QString("\r\n"));

					return cacheStringList;
				}


				QString CBackendRepresentativeMemoryCache::getIndividualAssociationDebugString(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, cint64 prevTab) {
					QString prevTabString;
					for (cint64 i = 0; i < prevTab; ++i) {
						prevTabString += QString("\t");
					}
					QString indiAssString = QString("\r\nIndividual %1:\r\n").arg(indiId);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					if (indiAssData->mDebugIndi) {
						QString indiName = CIRIName::getRecentIRIName(indiAssData->mDebugIndi->getIndividualNameLinker());
						indiAssString = QString("\r\nIndividual (%2) %1:\r\n").arg(indiId).arg(indiName);
					}

#endif
					if (prevTab > 0) {
						indiAssString = QString("\r\n%3Individual %1 previous entry %2:\r\n").arg(indiId).arg(prevTab).arg(prevTabString);
					}
					prevTabString += QString("\t");


					QString updateString = QString("%5update ids: individual association change: %1, total association change: %2, total touch: %3, \r\n%5\t\tlast indirectly connected individuals integration: %4").arg(indiAssData->getAssociationDataUpdateId()).arg(indiAssData->getCacheUpdateId()).arg(indiAssData->getCacheTouchId()).arg(indiAssData->getLastIntegratedIndirectlyConnectedIndividualsChangeId()).arg(prevTabString);
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					updateString += QString(", update function: %2, depending recomputation id: %1").arg(indiAssData->mDebugUpdateRecomputationId).arg(indiAssData->mDebugUpdateFunction);
#endif
					updateString += QString("\r\n");

					indiAssString += updateString;


					QStringList statusStrings;
					if (indiAssData->isCompletelySaturated()) {
						statusStrings += "completely saturated";
					} else {
						statusStrings += "incompletely saturated";
					}

					if (indiAssData->isCompletelyPropagated()) {
						statusStrings += "completely propagated";
					} else {
						statusStrings += "incompletely propagated";
					}

					if (indiAssData->isCompletelyHandled()) {
						statusStrings += "completely handled";
					} else {
						statusStrings += "incompletely handled";
					}

					if (indiAssData->hasIndirectlyConnectedIndividualIntegration()) {
						statusStrings += "indirectly connected individuals integration";
					}


					if (indiAssData->isIndirectlyConnectedNominalIndividual()) {
						statusStrings += "indirectly connected nominal";
					}

					if (indiAssData->hasProblematicLevel()) {
						statusStrings += "self clashing involved";
					}

					if (indiAssData->hasProblematicLeveledNeigbour()) {
						statusStrings += "neighbour clashing involved";
					}

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					if (indiAssData->mDebugSameIndividualsCompletion) {
						statusStrings += "deterministic same individuals completion";
					}
					if (indiAssData->mDebugNeighbourCompletionPerformed) {
						statusStrings += "neighbours same individuals completed";
					}
					if (indiAssData->mDebugNeighbourCompletionRequested) {
						statusStrings += "neighbours same individuals completion requested";
					}

					if (indiAssData->mDebugNeighbourCompletionExtended) {
						statusStrings += "neighbours same individuals extended";
					}
#endif


					QString statusString = QString("%2status: %1\r\n").arg(statusStrings.join(", ")).arg(prevTabString);
					indiAssString += statusString;


					if (indiAssData->getRepresentativeSameIndividualId() != indiAssData->getAssociatedIndividualId()) {
						indiAssString += QString("%2representative same individual id: %1\r\n").arg(indiAssData->getRepresentativeSameIndividualId()).arg(prevTabString);
					}
					if (indiAssData->getDeterministicSameIndividualId() != indiAssData->getAssociatedIndividualId()) {
						indiAssString += QString("%2deterministic merged same individual id: %1\r\n").arg(indiAssData->getDeterministicSameIndividualId()).arg(prevTabString);
					}
					if (indiAssData->getDeterministicMergedSameConsideredLabelCacheEntry()) {
						indiAssString += QString("%2deterministic merged same considered individuals label: %1\r\n").arg(indiAssData->getDeterministicMergedSameConsideredLabelCacheEntry()->getCacheEntryID()).arg(prevTabString);
					}


					for (cint64 labelType = 0; labelType < CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_ASSOCIATABLE_TYPE_COUNT; ++labelType) {
						CBackendRepresentativeMemoryLabelCacheItem* labelItem = indiAssData->getLabelCacheEntry(labelType);
						if (labelItem) {
							cint64 itemId = labelItem->getCacheEntryID();
							QString labelTypeString = getRepresentativeCacheLabelItemString(labelType);
							QString indiLabelAssString = QString("%3associated %1 label: %2\r\n").arg(labelTypeString).arg(itemId).arg(prevTabString);
							indiAssString += indiLabelAssString;
						}
					}


					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
					if (roleSetNeighbourArray) {
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
						for (cint64 i = 0; i < indexData->getArraySize(); ++i) {
							CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexData->getNeighbourRoleSetLabel(i);
							QString roleSetNeighbourIndiIdsString;
							roleSetNeighbourArray->at(i).visitNeighbourIndividualIds([&](cint64 id)->bool {
								if (!roleSetNeighbourIndiIdsString.isEmpty()) {
									roleSetNeighbourIndiIdsString += ", ";
								}
								roleSetNeighbourIndiIdsString += QString::number(id);
								CBackendRepresentativeMemoryCacheIndividualAssociationData* neighIndiAssData = ontologyData->getIndividualIdAssoiationDataVector()[id];
								if (neighIndiAssData && neighIndiAssData->hasProblematicLevel()) {
									roleSetNeighbourIndiIdsString += "(ci)";
								}
								if (neighbourRoleSetLabelItem->getCacheValueCount() == 1 && neighbourRoleSetLabelItem->getCacheValueLinker()->getData().getTag() == 1) {
									roleSetNeighbourIndiIdsString += "(pc)";
								}
								return true;
							});
							QString indiArrayAssString = QString("%4neighbouring ids for label %1 (%3): %2\r\n").arg(neighbourRoleSetLabelItem->getCacheEntryID()).arg(roleSetNeighbourIndiIdsString).arg(roleSetNeighbourArray->at(i).getIndividualCount()).arg(prevTabString);
							indiAssString += indiArrayAssString;
						}
					}

					CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* indConnData = ontologyData->getNominaIIndividualdIndirectConnectionDataHash()->value(indiId);
					if (indConnData) {
						QStringList indConnIndiIds;
						for (CXLinker<cint64>* indiLinkerIt = indConnData->getIndirectlyConnectedIndividualIdLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
							cint64 indiId = indiLinkerIt->getData();
							indConnIndiIds.append(QString::number(indiId));
						}
						QString indiIndConnDatatring = QString("%3indirectly connected individuals with change id %1: %2\r\n").arg(indConnData->getLastChangeId()).arg(indConnIndiIds.join(", ")).arg(prevTabString);
						indiAssString += indiIndConnDatatring;
					}

					CBackendRepresentativeMemoryCacheIndividualAssociationContext* memManCon = indiAssData->getIndividualAssociationMemoryContext();
					if (memManCon) {
						QString indiIndMemManCon = QString("memory management context: %1\r\n").arg((cint64)memManCon);
						indiAssString += indiIndMemManCon;
					}


					return indiAssString;
				}







				bool CBackendRepresentativeMemoryCache::debugCheckPropagationCutRemainingNeighbours(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool error = false;
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					if (indiAssData && indiAssData->mDebugIndi && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved()) {

						QHash<cint64, cint64> prevNeighbourIndiArrayPosHash;
						CBackendRepresentativeMemoryCacheIndividualAssociationData* prevIndiAssData = indiAssData->getPreviousData();
						if (prevIndiAssData) {

							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = prevIndiAssData->getRoleSetNeighbourArray();
							if (roleSetNeighbourArray) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
								for (cint64 i = 0; i < indexData->getArraySize(); ++i) {

									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinker = roleSetNeighbourArray->at(i).getIndividualIdLinker();
									bool firstArrayPosNeighbour = true;
									for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinkerIt = indiLinker; indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
										cint64 neighbourIndiId = indiLinkerIt->getIndividualId();
										prevNeighbourIndiArrayPosHash.insert(neighbourIndiId, i);
									}
								}
							}							
						}

						QHash<cint64,cint64> newNeighbourIndiArrayPosHash;
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
						if (roleSetNeighbourArray) {
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
							for (cint64 i = 0; i < indexData->getArraySize(); ++i) {

								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinker = roleSetNeighbourArray->at(i).getIndividualIdLinker();
								for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinkerIt = indiLinker; indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
									cint64 neighbourIndiId = indiLinkerIt->getIndividualId();
									newNeighbourIndiArrayPosHash.insert(neighbourIndiId, i);
									if (!prevNeighbourIndiArrayPosHash.contains(neighbourIndiId)) {
										bool debug = true;
									}
								}
							}

							for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinkerIt = indiAssData->getPropagationCutRemovedNeighbourIndividualLinker(); indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
								cint64 neighbourIndiId = indiLinkerIt->getIndividualId();
								newNeighbourIndiArrayPosHash.insert(neighbourIndiId, -1);
								if (!prevNeighbourIndiArrayPosHash.contains(neighbourIndiId)) {
									bool debug = true;
									error = true;
								}
							}

						}


						for (QHash<cint64, cint64>::const_iterator it = prevNeighbourIndiArrayPosHash.constBegin(), itEnd = prevNeighbourIndiArrayPosHash.constEnd(); it != itEnd; ++it) {
							cint64 prevIndiId = it.key();
							cint64 prevIndiArrayPos = it.value();
							if (!newNeighbourIndiArrayPosHash.contains(prevIndiId)) {
								bool debug = true;
								error = true;
							}
						}
					}
#endif
					return error;
				}


				bool CBackendRepresentativeMemoryCache::debugCheckSingleNeighbourInvolvedOccurrences(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool error = false;
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					if (indiAssData && indiAssData->mDebugIndi && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved()) {
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
						auto hash = indiAssData->getNeighbourRoleSetHash();
						QSet<cint64> neighbourIndiSet;
						if (roleSetNeighbourArray) {
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
							for (cint64 i = 0; i < indexData->getArraySize(); ++i) {

								auto label = indexData->getNeighbourRoleSetLabel(i);
								if (label->getCacheValueCount() > 1 || label->getCacheValueLinker()->getCacheValue().getTag() != 1) {
									auto& data = roleSetNeighbourArray->at(i);
									CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexData->getNeighbourRoleSetLabel(i);
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinker = roleSetNeighbourArray->at(i).getIndividualIdLinker();
									bool firstArrayPosNeighbour = true;
									for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinkerIt = indiLinker; indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
										cint64 neighbourIndiId = indiLinkerIt->getIndividualId();
										auto neighbourLabel = hash->getNeighbourRoleSetLabel(neighbourIndiId);
										if (neighbourLabel != label) {
											bool debug = true;
											mDebugIndiId = indiAssData->getAssociatedIndividualId();
											error = true;
										}
										if (neighbourIndiSet.contains(neighbourIndiId)) {
											bool debug = true;
											mDebugIndiId = indiAssData->getAssociatedIndividualId();
											error = true;
										}
										neighbourIndiSet.insert(neighbourIndiId);
										CBackendRepresentativeMemoryCacheIndividualAssociationData* neighIndiAssData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];
										if (neighIndiAssData && neighIndiAssData->hasProblematicLevel()) {
											if (!firstArrayPosNeighbour) {
												bool debug = true;
												mDebugIndiId = indiAssData->getAssociatedIndividualId();
												error = true;
											}
										} else {
											firstArrayPosNeighbour = false;
										}
									}
								}
							}
							
						}
					}
#endif
					return error;
				}





				bool CBackendRepresentativeMemoryCache::debugCheckCorrectNeighbourOccurrence(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					bool missingOccurrence = false;
#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
					if (indiAssData && indiAssData->mDebugIndi && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved()) {
						QString indiName = CIRIName::getRecentIRIName(indiAssData->mDebugIndi->getNameLinker());
						if (indiName == "http://www.Department1.University0.edu/StudentWhoLikesTennisButDislikesBasketBallAndBaseBall") {
							s1 = true;
						}
						if (indiName == "http://www.Department1.University0.edu/StudentWhoLikesTennisAndBasketBallAndBaseBallAndShouldHaveManyHobbies") {
							s2 = true;
						}
						if (indiName == "http://www.Department1.University0.edu/StudentWhoLikesBasketBallButDislikesBaseBall") {
							s3 = true;
						}
						if (indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Tennis" || indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#BasketBall" || indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Baseball") {
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
							QSet<QString> requiredNeighbourNamesSet;
							if (indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Tennis") {
								if (s1) {
									requiredNeighbourNamesSet.insert("http://www.Department1.University0.edu/StudentWhoLikesTennisButDislikesBasketBallAndBaseBall");
								}
								if (s2) {
									requiredNeighbourNamesSet.insert("http://www.Department1.University0.edu/StudentWhoLikesTennisAndBasketBallAndBaseBallAndShouldHaveManyHobbies");
								}
							} else if (indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#BasketBall") {
								if (s2) {
									requiredNeighbourNamesSet.insert("http://www.Department1.University0.edu/StudentWhoLikesTennisAndBasketBallAndBaseBallAndShouldHaveManyHobbies");
								}
								if (s3) {
									requiredNeighbourNamesSet.insert("http://www.Department1.University0.edu/StudentWhoLikesBasketBallButDislikesBaseBall");
								}
							} else if (indiName == "http://semantics.crl.ibm.com/univ-bench-dl.owl#Baseball") {
								if (s2) {
									requiredNeighbourNamesSet.insert("http://www.Department1.University0.edu/StudentWhoLikesTennisAndBasketBallAndBaseBallAndShouldHaveManyHobbies");
								}
							}

							if (roleSetNeighbourArray && !requiredNeighbourNamesSet.isEmpty()) {
								CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
								for (cint64 i = 0; i < indexData->getArraySize(); ++i) {
									auto& data = roleSetNeighbourArray->at(i);
									CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexData->getNeighbourRoleSetLabel(i);
									CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinker = roleSetNeighbourArray->at(i).getIndividualIdLinker();
									for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinkerIt = indiLinker; indiLinkerIt; indiLinkerIt = indiLinkerIt->getNext()) {
										cint64 neighbourIndiId = indiLinkerIt->getIndividualId();
										CBackendRepresentativeMemoryCacheIndividualAssociationData* neighIndiAssData = ontologyData->getIndividualIdAssoiationDataVector()[neighbourIndiId];
										if (neighIndiAssData && neighIndiAssData->mDebugIndi) {
											requiredNeighbourNamesSet.remove(CIRIName::getRecentIRIName(neighIndiAssData->mDebugIndi->getNameLinker()));
										}
									}
								}
								if (!requiredNeighbourNamesSet.isEmpty()) {
									bool debug = true;
									missingOccurrence = true;
									if (debug) {
										writeStringifiedRepresentativeCacheToFile();
									}
								}
							}
						}
					}
#endif
					return missingOccurrence;
				}

				bool CBackendRepresentativeMemoryCache::debugCheckNeighoursCorrectlyCounted(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData) {
					if (indiAssData) {
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = indiAssData->getRoleSetNeighbourArray();
						if (roleSetNeighbourArray) {
							CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* indexData = roleSetNeighbourArray->getIndexData();
							for (cint64 i = 0; i < indexData->getArraySize(); ++i) {
								auto& data = roleSetNeighbourArray->at(i);
								CBackendRepresentativeMemoryLabelCacheItem* neighbourRoleSetLabelItem = indexData->getNeighbourRoleSetLabel(i);
								CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiLinker = roleSetNeighbourArray->at(i).getIndividualIdLinker();
								cint64 count = 0;
								if (!indiLinker) {
									bool debug = false;
									if (debug) {
										writeStringifiedRepresentativeCacheToFile();
									}
								}
								if (indiLinker) {
									count = indiLinker->getCount();
								}
								if (count != roleSetNeighbourArray->at(i).getIndividualCount()) {
									bool debug = false;
									if (debug) {
										writeStringifiedRepresentativeCacheToFile();
									}
									return false;
								}
							}
						}
					}
					return true;
				}

				void CBackendRepresentativeMemoryCache::checkAssociationComplete(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool forceCompletion) {
					if ((ontologyData->getIncompletelyHandledIndividualIdCount() <= 0 || forceCompletion) && !ontologyData->isAssociationCompleted() && ontologyData->isFirstIncompletelyHandledIndividualsRetrieved()) {
						cint64 ontologyIdentifier = ontologyData->getOntologyIdentifer();
						ontologyData->setAssociationCompleted(true);
						mFixedOntologyIdentifierDataHashLock.lockForWrite();
						ontologyData->incUsageCount();
						mFixedOntologyIdentifierDataHash.insert(ontologyIdentifier, ontologyData);
						mFixedOntologyIdentifierDataHashLock.unlock();

						LOG(INFO, "::Konclude::RepresentativeBackendCache", logTr("Associating %1 individuals with %2 labels completed.").arg(ontologyData->getMaxStoredIndvidualiId()).arg(ontologyData->getNextEntryID(false)), this);

						if (mConfLateIndividualLabelAssociationIndexing) {
							indexIndividualLabelAssociations(ontologyData);
						}

						if (mConfDebugWriteRepresentativeCache) {
							writeStringifiedRepresentativeCacheToFile();
						}
					}
				}

				bool CBackendRepresentativeMemoryCache::debugTestingPrioritizedExpansionLinkDuplicates(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* testingArray, CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem) {
					if (testingArray) {
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* testingArrayIndexData = testingArray->getIndexData();
						if (testingArrayIndexData) {
							cint64 testingPrioPropMarkIndex = testingArrayIndexData->getIndex(propMarkLabelItem);
							if (testingPrioPropMarkIndex >= 0) {
								QSet<cint64> neighbourIndiIdSet;
								testingArray->at(testingPrioPropMarkIndex).visitNeighbourIndividualIds([&](cint64 id)->bool {
									if (neighbourIndiIdSet.contains(id)) {
										bool debug = true;
									} else {
										neighbourIndiIdSet.insert(id);
									}
									return true;
								});
							}
						}
					}
					return true;
				}

				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* CBackendRepresentativeMemoryCache::copyNeighbourIndividualIdLinkers(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker, CBackendRepresentativeMemoryCacheContext* context, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, cint64 pos) {
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* firstNewIndiIdLinker = nullptr;
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* lastNewIndiIdLinker = nullptr;
					cint64 copiedLinkerCount = 0;
					for (CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinkerIt = indiIdLinker; indiIdLinkerIt; indiIdLinkerIt = indiIdLinkerIt->getNext()) {
						cint64 neighbourId = indiIdLinkerIt->getIndividualId();
						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* newLinker = CObjectAllocator< CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker >::allocateAndConstruct(context->getMemoryAllocationManager());
						newLinker->initIndividualIdLinker(neighbourId);
						++mStatIndividualAssociationSeparateMemoryManagmentNeighbourLinkCopyingCount;
						++copiedLinkerCount;

						if (!lastNewIndiIdLinker) {
							firstNewIndiIdLinker = lastNewIndiIdLinker = newLinker;
						} else {
							lastNewIndiIdLinker->setNext(newLinker);
							lastNewIndiIdLinker = newLinker;
						}

					}
					return firstNewIndiIdLinker;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
