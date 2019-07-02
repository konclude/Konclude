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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "CSatisfiableCache.h"
#include "CComputedConsequencesCacheEntry.h"
#include "CComputedConsequencesCacheContext.h"
#include "CComputedConsequencesCacheReader.h"
#include "CComputedConsequencesCacheWriteTypesData.h"
#include "CSaturationNodeCacheUpdater.h"
#include "CCacheStatistics.h"


// Other includes
#include "Concurrent/CThread.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Reasoner/Kernel/Cache/Events/CWriteComputedConcequencesCacheEntryEvent.h"

#include "Utilities/Memory/CObjectMemoryPoolAllocator.h"
#include "Utilities/Memory/CObjectParameterizingAllocator.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"

#include "Context/CContextBase.h"


// Logger includes
#include "Logger/CLogger.h"


//#define KONCLUCE_SATISFIBALE_EXPANDER_CACHE_DIRECT_WRITING


namespace Konclude {

	using namespace Config;
	using namespace Concurrent;
	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				using namespace Events;

				/*! 
				 *
				 *		\class		CComputedConsequencesCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CComputedConsequencesCache : public CThread, public CSatisfiableCache {
					// public methods
					public:
						//! Constructor
						CComputedConsequencesCache(CConfiguration* config = nullptr, QString threadIdentifierName = "Saturation-Node-Associated-Expansion-Cache", CWatchDog *watchDogThread = 0);

						//! Destructor
						virtual ~CComputedConsequencesCache();

						CComputedConsequencesCacheReader* createCacheReader();
						CComputedConsequencesCacheWriter* createCacheWriter();


						CComputedConsequencesCache* writeCacheData(CComputedConsequencesCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

					// protected methods
					protected:
						CComputedConsequencesCache* installWriteCacheData(CComputedConsequencesCacheWriteData* writeData, CComputedConsequencesCacheContext* context);
						
						CComputedConsequencesCache* addTypesExpansionData(CComputedConsequencesCacheWriteTypesData* cccwtd, CComputedConsequencesCacheContext* context);

						
						CComputedConsequencesTypesCacheEntry* getComputedTypesCacheEntryForNode(CIndividual* individual, CComputedConsequencesCacheContext* context, bool create = true);



						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					// protected variables
					protected:
						CComputedConsequencesCacheEntry* mEntryLinker;

						cint64 mConfAllowedNonDetExpansionCount;


						CComputedConsequencesCacheReader* mReaderLinker;
						QMutex mReaderSyncMutex;

						CCacheStatistics mCacheStat;
						CComputedConsequencesCacheContext mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCOMPUTEDCONSEQUENCESCACHE_H
