/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "CSatisfiableCache.h"
#include "CSignatureSatisfiableExpanderCacheHasher.h"
#include "CSignatureSatisfiableExpanderCacheSlotItem.h"
#include "CSignatureSatisfiableExpanderCacheEntry.h"
#include "CSignatureSatisfiableExpanderCacheContext.h"
#include "CSignatureSatisfiableExpanderCacheReader.h"
#include "CSignatureSatisfiableExpanderCacheEntryExpandWriteData.h"
#include "CSignatureSatisfiableExpanderCacheEntrySatisfiableBranchWriteData.h"
#include "CCacheStatistics.h"


// Other includes
#include "Concurrent/CThread.h"

#include "Reasoner/Kernel/Cache/Events/CWriteCachedDataEvent.h"
#include "Reasoner/Kernel/Cache/Events/CWriteExpandCachedEvent.h"
#include "Reasoner/Kernel/Cache/Events/CWriteSatisfiableBranchCachedEvent.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"

#include "Context/CContextBase.h"


// Logger includes
#include "Logger/CLogger.h"


//#define KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING


namespace Konclude {

	using namespace Concurrent;
	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				using namespace Events;

				/*! 
				 *
				 *		\class		CSignatureSatisfiableExpanderCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSignatureSatisfiableExpanderCache : public CThread, public CSatisfiableCache {
					// public methods
					public:
						//! Constructor
						CSignatureSatisfiableExpanderCache(QString threadIdentifierName = "Satisfiable-Expander-Cache", CWatchDog *watchDogThread = 0);

						//! Destructor
						virtual ~CSignatureSatisfiableExpanderCache();

						CSignatureSatisfiableExpanderCacheReader* createCacheReader();
						CSignatureSatisfiableExpanderCacheWriter* createCacheWriter();


						CSignatureSatisfiableExpanderCache* writeCachedData(CSignatureSatisfiableExpanderCacheEntryWriteData* writeData, CMemoryPool* memoryPools);
						CSignatureSatisfiableExpanderCache* writeExpandCached(cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash, CMemoryPool* memoryPools);
						CSignatureSatisfiableExpanderCache* writeSatisfiableBranchCached(cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedList, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

					// protected methods
					protected:
						bool isCachingDataExpandable(CSignatureSatisfiableExpanderCacheContext* context, CSignatureSatisfiableExpanderCacheEntry* entry, cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList);

						void writeExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, CSignatureSatisfiableExpanderCacheEntry* entry, bool extending, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash);
						bool writeExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, cint64 prevSignature, cint64 newSignature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGHASH<cint64,cint64>* depHash);
						void addExpanderCachingData(CSignatureSatisfiableExpanderCacheContext* context, const CCacheValue& cacheValue, CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* newTagHash, CCACHINGHASH<cint64,cint64>* depHash, CCACHINGHASH<cint64,CCacheValue>* tagCacheValueHash, CExpanderCacheValueLinker*& lastExpCacheValueLinker, CExpanderCacheValueLinker*& firstExpCacheValueLinker);

						bool writeSatisfiableBranchedCachingData(CSignatureSatisfiableExpanderCacheContext* context, cint64 signature, CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* branchedValueList);

						void createReaderSlotUpdate(CSignatureSatisfiableExpanderCacheContext* context);
						void cleanUnusedSlots(CSignatureSatisfiableExpanderCacheContext* context);
						
						
						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					// protected variables
					protected:
						CCACHINGHASH<cint64,CSignatureSatisfiableExpanderCacheRedirectionItem*>* mSigItemHash;
						CCACHINGSET<cint64>* mIncompatibleSigSet;
						CCACHINGSET<cint64>* mAlreadyExpSigSet;
						// currently not used
						CCACHINGHASH<CSignatureSatisfiableExpanderCacheHasher,CSignatureSatisfiableExpanderCacheRedirectionItem*>* mHasherItemHash;


						cint64 mWriteDataCount;
						cint64 mStartWriteCollectCount;
						cint64 mNextWriteCollectCount;
						cint64 mCollectCount;
						CMemoryPool* mCollectMemoryPools;
						CSignatureSatisfiableExpanderCacheEntryWriteData* mCollectWriteData;


						CCacheStatistics mCacheStat;

						CSignatureSatisfiableExpanderCacheSlotItem* mSlotLinker;
						CSignatureSatisfiableExpanderCacheReader* mReaderLinker;

						QMutex mReaderSyncMutex;

						CSignatureSatisfiableExpanderCacheContext mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSIGNATURESATISFIABLEEXPANDERCACHE_H
