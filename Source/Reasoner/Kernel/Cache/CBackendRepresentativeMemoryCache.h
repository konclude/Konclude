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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendCache.h"
#include "CBackendRepresentativeMemoryCacheSlotItem.h"
#include "CBackendRepresentativeMemoryCacheContext.h"
#include "CBackendRepresentativeMemoryCacheReader.h"
#include "CBackendRepresentativeMemoryCacheWriter.h"
#include "CBackendRepresentativeMemoryCacheLabelAssociationWriteData.h"
#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"
#include "CCacheStatistics.h"


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
						CBackendRepresentativeMemoryCacheWriter* createCacheWriter();


						CBackendRepresentativeMemoryCache* writeCachedData(CBackendRepresentativeMemoryCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

						bool getIncompletlyAssociationCachedIndividuals(QSet<CIndividual*>* individualSet);

					// protected methods
					protected:

						void createReaderSlotUpdate(CBackendRepresentativeMemoryCacheContext* context);
						void cleanUnusedSlots(CBackendRepresentativeMemoryCacheContext* context);
						
						
						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


						void installTemporaryCardinalities(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* tempCardWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);
						void installTemporaryLabels(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* tempLabelWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);
						bool installAssociations(CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* tempAssWriteDataLinker, CBackendRepresentativeMemoryCacheContext* context);

					// protected variables
					protected:
						CConfiguration* mConfig;

						CCACHINGHASH<cint64,CBackendRepresentativeMemoryLabelSignatureResolveCacheItem*>* mSigLabelItemHash;
						CCACHINGHASH<cint64,CBackendRepresentativeMemoryCardinalitySignatureResolveCacheItem*>* mSigCardItemHash;
						cint64 mNextEntryID;

						cint64 mWriteDataCount;
						cint64 mStartWriteCollectCount;
						cint64 mNextWriteCollectCount;
						cint64 mCollectCount;
						CMemoryPool* mCollectMemoryPools;
						CBackendRepresentativeMemoryCacheWriteData* mCollectWriteData;


						QSet<CIndividual*> mIncompletelyAssociatedIndividualSet;
						CCacheStatistics mCacheStat;

						CBackendRepresentativeMemoryCacheSlotItem* mSlotLinker;
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
