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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H

// Libraries includes
#include <QHash>
#include <QSet>
#include <QMutex>
#include <QSemaphore>

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

#include "Reasoner/Ontology/CIndividualProcessData.h"
#include "Reasoner/Ontology/CIRIName.h"

#include "Reasoner/Kernel/Cache/Events/CWriteCachedDataEvent.h"
#include "Reasoner/Kernel/Cache/Events/CWriteBackendAssociationCachedEvent.h"
#include "Reasoner/Kernel/Cache/Events/CRetrieveIncompletelyAssociationCachedEvent.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"

#include "Context/CContextBase.h"

#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"




namespace Konclude {

	using namespace Concurrent;
	using namespace Context;
	using namespace Config;
	using namespace Utilities::Memory;

	namespace Reasoner {

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
						CBackendRepresentativeMemoryCacheReader* createFixedCacheReader();
						CBackendRepresentativeMemoryCacheWriter* createCacheWriter();


						CBackendRepresentativeMemoryCache* writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

						bool getIncompletlyAssociationCachedIndividuals(cint64 ontologyId, QSet<CIndividualReference>* individualSet, cint64 limit = -1);

						// protected methods
				protected:

					void createReaderSlotUpdate(CBackendRepresentativeMemoryCacheContext* context);
					void cleanUnusedSlots(CBackendRepresentativeMemoryCacheContext* context);


					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					void installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);
					void installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);
					bool installAssociationUpdates(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);
					bool installNominalIndirectConncetionUpdates(CBackendRepresentativeMemoryCacheTemporaryNominalIndirectConnectionDataLinker* tempNomIndirectConnDataLinker, CBackendRepresentativeMemoryCacheContext* context);
					bool checkAssociationUsage(CBackendRepresentativeMemoryCacheTemporaryAssociationUseDataLinker* tempAssUseDataLinker, CBackendRepresentativeMemoryCacheContext* context);


					CBackendRepresentativeMemoryLabelCacheItem* getAdditionMergedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* additionLabel, CBackendRepresentativeMemoryLabelCacheItem* associatedlabel, CBackendRepresentativeMemoryCacheContext* context);
					CBackendRepresentativeMemoryLabelCacheItem* getReducedLabel(cint64 labelType, CBackendRepresentativeMemoryLabelCacheItem* label, function<bool(const CCacheValue& cacheValue)> reduceCheckFunction, CBackendRepresentativeMemoryCacheContext* context);

					void setUpdatedIndividualAssociationData(cint64 individualID, CBackendRepresentativeMemoryCacheIndividualAssociationData* locAssociationData);

					bool markIndividualAssociationIncompletelyHandled(cint64 indiId, CBackendRepresentativeMemoryCacheIndividualAssociationData* associationData, CBackendRepresentativeMemoryCacheContext* context);


					QString getRepresentativeCacheString();
					QString getRepresentativeCacheLabelItemString(cint64 labelType);
					void writeStringifiedRepresentativeCacheToFile();


					bool requiresIndividualAssociations(cint64 labelType);
					CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* getIndividualAssociationsExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheContext* context);
					CBackendRepresentativeMemoryLabelCacheItemIndividualRoleSetNeighbourArrayIndexExtensionData* getIndividualNeighbourArrayIndexExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheContext* context);
					CBackendRepresentativeMemoryLabelCacheItemTagLabelResolvingExtensionData* getNeighbourArrayRoleTagResolvingLabelExtensionData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, CBackendRepresentativeMemoryCacheContext* context);

					// protected variables
				protected:
					CConfiguration* mConfig;


					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelSignatureResolveCacheItem>* mSigLabelItemHash[CBackendRepresentativeMemoryLabelCacheItem::LABEL_CACHE_ITEM_TYPE_COUNT];
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem>* mSigCardItemHash;
					cint64 mNextEntryID;

					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheIndividualAssociationData*>* mIndiIdAssoDataHash;
					CCACHINGHASH<cint64, CBackendRepresentativeMemoryCacheNominalIndividualIndirectConnectionData* >* mNominalIndiIdIndirectConnectionDataHash;



					CCACHINGHASH<cint64, CBackendRepresentativeMemoryLabelCacheItem*> mTmpIndiIndirectlyConnNomLabelItemHash;
					cint64 mTmpIndiAssocPrevUpdateId;


					cint64 mIndiIdAssoDataVectorSize;
					CBackendRepresentativeMemoryCacheIndividualAssociationData** mIndiIdAssoDataVector;

					cint64 mLastMinIncompletelyHandledIndiId;
					cint64 mIncompletelyHandledIndiIdCount;
					bool mZeroIncompletelyHandledIndiIdCountDebugWritten;

					cint64 mMaxIndiAssocDataUpdateCount = 0;

					bool mSameMergedIndisInCache;

					cint64 mWriteDataCount;
					cint64 mStartWriteCollectCount;
					cint64 mNextWriteCollectCount;
					cint64 mCollectCount;
					CMemoryPool* mCollectMemoryPools;
					CBackendRepresentativeMemoryCacheWriteData* mCollectWriteData;

					cint64 mLastOntologyId;

					cint64 mNextIndiUpdateId;
					cint64 mNextNomConnUpdateId;
					cint64 mUpdatedIndiCount = 0;
					cint64 mUpdateIncompatibleIndiCount = 0;
					cint64 mCheckedIndiCount = 0;
					cint64 mCheckIncompatibleIndiCount = 0;
					cint64 mReducedNeighbourArrayCount = 0;

					QSet<CIndividualReference> mIncompletelyAssociatedIndividualSet;
					CCacheStatistics mCacheStat;

					CBackendRepresentativeMemoryCacheSlotItem* mSlotLinker;
					CBackendRepresentativeMemoryCacheSlotItem* mLastUpdatedSlotLinker;
					CBackendRepresentativeMemoryCacheReader* mReaderLinker;

					QMutex mReaderSyncMutex;

					CBackendRepresentativeMemoryCacheContext mContext;

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
