/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "CCompletionGraphCache.h"
#include "CReuseCompletionGraphCacheReader.h"
#include "CReuseCompletionGraphCacheWriter.h"
#include "CReuseCompletionGraphCacheContext.h"
#include "CReuseCompletionGraphCacheSlotItem.h"
#include "CReuseCompletionGraphCacheEntryWriteData.h"
#include "CReuseCompletionGraphCacheEntryExpandWriteData.h"


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
				 *		\class		CReuseCompletionGraphCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReuseCompletionGraphCache : public CThread, public CCompletionGraphCache {
					// public methods
					public:
						//! Constructor
						CReuseCompletionGraphCache(QString threadIdentifierName = "Reuse-Completion-Graph-Cache", CWatchDog *watchDogThread = 0);

						//! Destructor
						virtual ~CReuseCompletionGraphCache();

						CReuseCompletionGraphCacheReader* createCacheReader();
						CReuseCompletionGraphCacheWriter* createCacheWriter();

						CReuseCompletionGraphCache* writeExpandCache(CReuseCompletionGraphCacheEntryWriteData* writeData, CMemoryPool* memoryPools);


					// protected methods
					protected:
						void createReaderSlotUpdate(CReuseCompletionGraphCacheContext* context);
						void cleanUnusedSlots(CReuseCompletionGraphCacheContext* context);
						
						
						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

						void writeExpandCacheData(CReuseCompletionGraphCacheContext* context, CCACHINGLIST<CCacheValue>* cacheList, CCACHINGLIST<CCacheValue>* minimalCacheList, CSatisfiableCalculationJobInstantiation* jobInst);

					// protected variables
					protected:
						CReuseCompletionGraphCacheSlotItem* mSlotLinker;
						CReuseCompletionGraphCacheReader* mReaderLinker;
						QMutex mReaderSyncMutex;

						CReuseCompletionGraphCompatibilityEntryHash* mEntyHash;
						CCACHINGLIST<CReuseCompletionGraphCacheEntry*>* mEntyList;
						cint64 mEntryCount;


						CReuseCompletionGraphCacheContext mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHE_H
