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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H

// Libraries includes
#include <QHash>
#include <QSet>
#include <QMutex>
#include <QSemaphore>
#include <QtConcurrent>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendCache.h"
#include "CBackendRepresentativeMemoryCacheSlotItem.h"
#include "CBackendRepresentativeMemoryCacheContext.h"
#include "CBackendRepresentativeMemoryCacheBaseContext.h"
#include "CBackendRepresentativeMemoryCacheReader.h"
#include "CBackendRepresentativeMemoryCacheWriter.h"
#include "CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"
#include "CBackendRepresentativeMemoryCacheUtilities.h"
#include "CCacheStatistics.h"
#include "CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData.h"
#include "CBackendRepresentativeMemoryLabelCacheItemCardinalityExtensionData.h"

#include "CBackendRepresentativeMemoryCacheOntologyData.h"

// Other includes
#include "Concurrent/CThread.h"
#include "Concurrent/CConcurrentTaskScheduler.h"

#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Reasoner/Kernel/Cache/Events/CWriteCachedDataEvent.h"
#include "Reasoner/Kernel/Cache/Events/CWriteBackendAssociationCachedEvent.h"
#include "Reasoner/Kernel/Cache/Events/CRetrieveIncompletelyAssociationCachedEvent.h"
#include "Reasoner/Kernel/Cache/Events/CInitializeIndividualAssociationsCacheEvent.h"
#include "Reasoner/Kernel/Cache/Events/CReportMaximumHandledRecomputationIdsEvent.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"

#include "Context/CContextBase.h"
#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	using namespace Concurrent;
	using namespace Context;
	using namespace Config;
	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				using namespace Events;

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCache : public CThread, public CBackendCache {
					// public methods
					public:
						//! Constructor
						CBackendRepresentativeMemoryCache(CConfiguration* config = nullptr, QString threadIdentifierName = "Backend-Representative-Cache", CWatchDog *watchDogThread = nullptr);

						//! Destructor
						virtual ~CBackendRepresentativeMemoryCache();

						CBackendRepresentativeMemoryCacheReader* createCacheReader();
						CBackendRepresentativeMemoryCacheReader* createOntologyFixedCacheReader(cint64 ontologyIdentifier);
						CBackendRepresentativeMemoryCacheWriter* createCacheWriter();


						CBackendRepresentativeMemoryCache* writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

						bool getIncompletlyAssociationCachedIndividuals(cint64 ontologyIdentifier, CBackendIndividualRetrievalComputationUpdateCoordinationHash* prevCoordHash, CBackendIndividualRetrievalComputationUpdateCoordinationHash* newCoordHash, bool allIndividualsAdded, bool refillRetrievalCoordHash, cint64 limit = -1, CCallbackData* callbackData = nullptr);
						bool initializeIndividualsAssociationCaching(cint64 ontologyIdentifier, cint64 individualCount);
						bool reportMaximumHandledRecomputationId(cint64 ontologyIdentifier, cint64 maximumRecomputationId);

						void writeStringifiedRepresentativeCacheToFile();

						CConcreteOntology* mDebugOntology;

					// protected methods
					protected:

						void createReaderSlotUpdate(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, CBackendRepresentativeMemoryCacheBaseContext* context);
						void cleanUnusedSlots(CBackendRepresentativeMemoryCacheBaseContext* context);
						
						void deleteExpiredIndividualAssociationMemoryContexts(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, CBackendRepresentativeMemoryCacheBaseContext* context);
						void queueIndividualAssociationMemoryContextDeletion(CBackendRepresentativeMemoryCacheIndividualAssociationContext* indiAssMemContext, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						cint64 getMinimumSlotReferreringInstalledValidRecomputationId(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

						void checkAssociationComplete(CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool forceCompletion);


						void installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						void installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool installAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool installAssociationUpdate(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* copyNeighbourIndividualIdLinkers(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourIndividualIdLinker* indiIdLinker, CBackendRepresentativeMemoryCacheContext* context, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, cint64 pos);

						CBackendRepresentativeMemoryCacheIndividualAssociationData* createLocalizedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool allowSeparatedManagement, bool incrementUpdateId = true);
						CBackendRepresentativeMemoryCacheContext* getIndividualAssociationDataMemoryContext(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, bool* requiresDataCopying = nullptr);

						bool updateIndexedAssociationCount(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 i, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool updateIndexedAssociationCount(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryLabelCacheItem* prevLabelItem, cint64 i, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool installDeterministicSameAsAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool installDeterministicSameAsAssociationUpdate(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAsAssociationData, cint64 sameAsIndividualId, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool checkRequiresDeterministicSameAsAssociationUpdateInstallation(CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* detSameAsAssociationData, cint64 sameAsIndividualId, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						bool completeSameAsNeighbours(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, QSet<cint64> newCompletingDetSameNeighbours, QSet<cint64>& newlyCompletedDetSameNeighbours, bool completeOnlyCompatibleNeighbourRoleLabels, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool completeNeighboursForSameAsMerging(QSet<cint64>& tmpCompleteNeighbourSameIndiMergingSet, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool completeDeterministicSameAsMergingInformation(QHash<cint64, cint64>& tmpDetSameMergingCompletionReferenceHash, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool udateDeterministicSameAssociations(CBackendRepresentativeMemoryCacheIndividualAssociationData* locNeighbourAssociationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool debugTestingPrioritizedExpansionLinkDuplicates(CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* testingArray, CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem);

						bool installNominalIndirectConncetionUpdates(CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool checkAssociationUsage(CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						CBackendRepresentativeMemoryLabelCacheItem* getAdditionMergedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* additionLabel, CBackendRepresentativeMemoryLabelCacheItem* associatedlabel, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryLabelCacheItem* getReducedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, function<bool(const CCacheValue& cacheValue)> reduceCheckFunction, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryLabelCacheItem* getExtendedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, CCacheValue extendingCacheValue, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryCache* addCreatedLabelStatistics(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						void setUpdatedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool markRepresentativeReferencedIndividualAssociationIncompletelyHandled(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool markIndividualAssociationIncompletelyHandled(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool markIndividualAssociationCompletelyHandled(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						QStringList getRepresentativeCacheString();
						QStringList getRepresentativeCacheString(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						QString getIndividualAssociationDebugString(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData, cint64 prevTab);


						QString getRepresentativeCacheLabelItemString(cint64 labelType);


						bool requiresIndividualAssociations(cint64 labelType);
						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* getIndividualAssociationsExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* getIndividualNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* getNeighbourArrayRoleTagResolvingLabelExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);



						CBackendRepresentativeMemoryCacheOntologyData* prepareOntologyDataUpdate(cint64 ontologyIdentifier, cint64 minIndiCount = 0);


						void indexIndividualLabelAssociations(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool updateInvolvedIndividuals(CBackendRepresentativeMemoryCacheTemporaryInvolvedIndividualDataLinker* tmpInvolvedIndiDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool integratePropagationCut(CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* tmpPropCutDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool updatePropagationCutIndividualIncompletelyHandled(cint64 neighbourId, const CCacheValue& propConValue, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool updatePropagationCutIndividualIncompletelyHandled(cint64 neighbourId, const QList<cint64>& propCutIndiIdList, CBackendRepresentativeMemoryLabelCacheItem* propMarkLabelItem, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool storeIndividualIncompletelyMarked(CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData, bool markIncompletelyHandled, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);

						bool debugCheckPropagationCutRemainingNeighbours(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool debugCheckSingleNeighbourInvolvedOccurrences(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool debugCheckCorrectNeighbourOccurrence(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool debugCheckNeighoursCorrectlyCounted(CBackendRepresentativeMemoryCacheIndividualAssociationData* indiAssData, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						void debugCheckWriteMaxSameMergedIndividualsToFile(CBackendRepresentativeMemoryCacheOntologyData* ontData);


						bool checkUpdateRejection(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool handleUpdateRejection(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						
						bool analyseDeterministicSameAsAssociationInstallation(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheOntologyData* ontologyData);



						bool activateBasicPrecompuationMode(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);
						bool checkBasicPrecompuationModeActivation(CBackendRepresentativeMemoryCacheOntologyData* ontologyData);


						bool isRoleNeighbourLinkLabelItemCompatibility(CBackendRepresentativeMemoryLabelCacheItem* item1, CBackendRepresentativeMemoryLabelCacheItem* item2);
						bool isCacheValueRoleInverse(const CCacheValue& cacheValue);
						bool isCacheValueRoleNondeterministic(const CCacheValue& cacheValue);

					// protected variables
					protected:
						CConfiguration* mConfig;

						QHash<cint64, cint64> mIndiContextCountHash;
						cint64 mIndiContextDebuggingCount = 50;

						CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheOntologyData*>* mOntologyIdentifierDataHash;
						QHash<cint64, CBackendRepresentativeMemoryCacheOntologyData*> mFixedOntologyIdentifierDataHash;
						QReadWriteLock mFixedOntologyIdentifierDataHashLock;


						//CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* mSigLabelItemHash[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						//cint64 mNextEntryID;

						////CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* mIndiIdAssoDataHash;
						//CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* mNominalIndiIdIndirectConnectionDataHash;



						CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItem*> mTmpIndiIndirectlyConnNomLabelItemHash;
						cint64 mTmpIndiAssocPrevUpdateId;
						CCACHINGSET<cint64> mPropagationCutIndiSet;


						//cint64 mIndiIdAssoDataVectorSize;
						//CBackendRepresentativeMemoryCacheIndividualAssociationData** mIndiIdAssoDataVector;

						//cint64 mLastMinIncompletelyHandledIndiId;
						//cint64 mIncompletelyHandledIndiIdCount;
						//bool mZeroIncompletelyHandledIndiIdCountDebugWritten;

						//cint64 mMaxIndiAssocDataUpdateCount = 0;

						//bool mSameMergedIndisInCache;

						cint64 mCheckingRemainingIncompletelyHandledCount;
						cint64 mEmptyWriteDataCount;
						cint64 mWriteDataCount;
						cint64 mStartWriteCollectCount;
						cint64 mNextWriteCollectCount;
						cint64 mCollectCount;
						CMemoryPool* mCollectMemoryPools;
						CBackendRepresentativeMemoryCacheWriteData* mCollectWriteData;

						cint64 mNextIndiUpdateId;
						cint64 mNextNomConnUpdateId;
						cint64 mUpdatedIndiCount = 0;
						cint64 mAssociationUpdatedIndiCount = 0;
						cint64 mUpdateIncompatibleIndiCount = 0;
						cint64 mCheckedIndiCount = 0;
						cint64 mCheckIncompatibleIndiCount = 0;
						cint64 mReducedNeighbourArrayCount = 0;

						cint64 mCurrentUpdateHandlingRecomputationId = -1;

						cint64 mReaderSlotUpdateCount;
						cint64 mOntologyDataUpdateCount;
						cint64 mOntologyDataReleasedCount;
						cint64 mOntologyDataReleasedWhileNewCreationCount;
						cint64 mOntologyDataReleasedWhileSlotUpdateCount;
						cint64 mReaderSlotReleasedCount;

						QSet<CIndividualReference> mIncompletelyAssociatedIndividualSet;
						CCacheStatistics mCacheStat;

						class CPropagationCutNeighbourArrayHandlingData {
						public:
							QSet<cint64> mReaddingArrayPosSet;
							QSet<cint64> mReductionArrayPosSet;
							QSet<cint64> mRemovalArrayPosSet;
						};
						QHash<cint64, CPropagationCutNeighbourArrayHandlingData*>* mTmpPropCutIndiArrayNeighboursHandlingDataHash;
						QSet<cint64> mTmpCompleteNeighbourSameIndiMergingSet;
						QHash<cint64,cint64> mTmpDetSameMergingCompletionReferenceHash;

						CBackendRepresentativeMemoryCacheSlotItem* mSlotLinker;
						CBackendRepresentativeMemoryCacheSlotItem* mLastUpdatedSlotLinker;
						CBackendRepresentativeMemoryCacheReader* mReaderLinker;

						QMutex mReaderSyncMutex;
						bool mLimitRemainingWritePending;
						QSemaphore mRemainingWritePendingSemaphore;

						CBackendRepresentativeMemoryCacheBaseContext mContext;

						bool mConfLateIndividualLabelAssociationIndexing;
						bool mConfWaitIndividualLabelAssociationIndexed;
						bool mConfDebugWriteRepresentativeCache;

						bool mConfIncrementUpdateIdForDeterministicSameAsCompletion = true;
						cint64 mConfMinRequiredDeterministicSameMergedHandledInstallationPossiblitiesForNeighbourCompletion = 1;
						cint64 mConfUnchangedDeterministicSameMergeUpdatesForDeterministicSameNeighbourCompletion = 1;

						bool mConfInstallingDeterministicSameHandlingLargeDifferenceReached;
						cint64 mConfInstallingDeterministicSameHandlingLargeDifference;


						double mConfBasicPrecomputationModeActivationUpdateMergesRatio = 0.05;


						QAtomicInt mPendingUpdateCount;

						cint64 mSlotUpdateWaitingIncreaseCount;
						cint64 mSlotUpdateWaitingMaxCount;


						bool mConfDirectUpdateSynchronization;
						QMutex mDirectUpdateSyncMutex;



						class CDeterministicSameHandlingInstallationData {
						public:
							QSet<cint64> mIdPossibleInstallationSet;
							QSet<cint64> mIdFirstPossibleInstallationSet;
						};
						QHash<cint64, CDeterministicSameHandlingInstallationData> mDeterministicSameHandlingInstallationDataHash;						



						bool mConfPropagationCutPropagatedConceptDirectInstallation = true;


						cint64 mConfMaxIncompletelyHandledIndividualsRetrievalCount = -1;
						cint64 mConfMaxCacheDataUpdateWritingCount = -1;



						double mConfUpdateRejectingIncompatibleIndividualAssociationsRatio = 1.;
						double mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourRatio = 1;
						cint64 mConfUpdateRejectingIncompatiblePropagationCuttedIndividualLinkedNeighbourCount = -1;

						bool mConfInterpretUnchangedLabelsAsCompatible = false;


						bool mStatCollectStatistics;

						cint64 mStatAddingNeighbourLinksAssociationUpdateCount;
						cint64 mStatUpdatedOrRemovedNeighbourLinksAssociationUpdateCount;
						cint64 mStatMaxSameAsMergedCount;
						cint64 mStatMaxLabelValueCount;
						cint64 mStatMaxAssociationUpdateCount;
						cint64 mStatMaxNeighbourLinksCount;
						cint64 mStatLabelCount;

						cint64 mStatLabelTypeCount[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						cint64 mStatLabelTypeMaxValueCount[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
						cint64 mStatLabelTypeAllValueCount[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];

						cint64 mStatDetSameRepresentativeMergingCount;
						cint64 mStatDetSameAssociationInstallCount;
						cint64 mStatDetSameAssociationFailedCount;
						cint64 mStatDetSameAssociationDifferentUpdateIdFailedCount;
						cint64 mStatDetSameAssociationDifferentDestIdFailedCount;
						cint64 mStatDetSameAssociationIncompleteHandledDestFailedCount;
						cint64 mStatDetSameAssociationRepMergedDestFailedCount;

						cint64 mStatCreatedNeighbourLinks;

						cint64 mStatIncompatibleLabelNeighbourCompletionCount;
						cint64 mStatChangedLabelNeighbourCompletionCount;
						cint64 mStatNeighbourCompletionDetSameSuccededCount;
						cint64 mStatNeighbourCompletionDetSameUnchangedCount;
						cint64 mStatNeighbourCompletionDetSameChangedCount;
						cint64 mStatNeighbourCompletionDetSameIncompatibleCount;


						cint64 mStatIndividualAssociationSeparateMemoryManagmentContextCreationCount = 0;
						cint64 mStatIndividualAssociationSeparateMemoryManagmentContextReuseCount = 0;

						cint64 mStatIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingCount = 0;
						cint64 mStatIndividualAssociationSeparateMemoryManagmentUnreferredSlotCount = 0;
						cint64 mStatIndividualAssociationSeparateMemoryManagmentDeletionCount = 0;
						cint64 mStatIndividualAssociationSeparateMemoryManagmentSlotReferredCheckingQueuingCount = 0;

						cint64 mStatIndividualAssociationWithoutSeparateMemoryManagmentCount = 0;

						cint64 mStatIndividualAssociationSeparateMemoryManagmentNeighbourLinkCopyingCount = 0;
						cint64 mStatReportedMaximumHandledRecomputationIdCount = 0;

						cint64 mStatMemoryManagmentQueuedCheckingCount = 0;
						cint64 mStatMemoryManagmentScheduledReleasingCount = 0;


						CBackendRepresentativeMemoryCacheOntologyData* mLastHandledOntologyContext = nullptr;
						cint64 mLastMemoryContextDeletionMinValidRecompId = 0;
						CBackendRepresentativeMemoryCacheTemporaryPropagationCutDataLinker* mLastHandledPropCutDataLinker = nullptr;

						bool s1 = false;
						bool s2 = false;
						bool s3 = false;
						cint64 mDebugIndiId = -1;

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
						QStringList mDebugHandlingStrings;
#endif

#ifdef KONCLUDE_CACHE_DEBUGGING_DATA_GENERATION
//#define DEBUG_HANDLING_STRING(a)
#define DEBUG_HANDLING_STRING(a) mDebugHandlingStrings += QString("%1 at %2 with recomputation id %3").arg(a).arg(mNextIndiUpdateId).arg(mCurrentUpdateHandlingRecomputationId);
#else
#define DEBUG_HANDLING_STRING(a)
#endif

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H
