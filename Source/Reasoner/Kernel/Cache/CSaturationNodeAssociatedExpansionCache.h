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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "CSatisfiableCache.h"
#include "CSaturationNodeAssociatedExpansionCacheEntry.h"
#include "CSaturationNodeAssociatedExpansionCacheContext.h"
#include "CSaturationNodeAssociatedExpansionCacheReader.h"
#include "CSaturationNodeAssociatedExpansionCacheUnsatisfiabilityWriteData.h"
#include "CSaturationNodeAssociatedExpansionCacheExpansionWriteData.h"
#include "CSaturationNodeCacheUpdater.h"
#include "CCacheStatistics.h"


// Other includes
#include "Concurrent/CThread.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"

#include "Reasoner/Kernel/Cache/Events/CWriteSaturationCacheDataEvent.h"

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
				 *		\class		CSaturationNodeAssociatedExpansionCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CSaturationNodeAssociatedExpansionCache : public CThread, public CSatisfiableCache {
					// public methods
					public:
						//! Constructor
						CSaturationNodeAssociatedExpansionCache(CConfiguration* config = nullptr, QString threadIdentifierName = "Saturation-Node-Associated-Expansion-Cache", CWatchDog *watchDogThread = 0);

						//! Destructor
						virtual ~CSaturationNodeAssociatedExpansionCache();

						CSaturationNodeAssociatedExpansionCacheReader* createCacheReader();
						CSaturationNodeAssociatedExpansionCacheWriter* createCacheWriter();


						CSaturationNodeAssociatedExpansionCache* writeCacheData(CSaturationNodeAssociatedExpansionCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();

					// protected methods
					protected:
						CSaturationNodeAssociatedExpansionCache* installWriteCacheData(CSaturationNodeAssociatedExpansionCacheWriteData* writeData, CSaturationNodeAssociatedExpansionCacheContext* context);
						
						CSaturationNodeAssociatedExpansionCache* propagateUnsatisfibility(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context);
						CSaturationNodeAssociatedExpansionCache* addNodeExpansionData(CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context);

						
						CSaturationNodeAssociatedExpansionCacheEntry* getCacheEntryForNode(CIndividualSaturationProcessNode* node, CSaturationNodeAssociatedExpansionCacheContext* context, bool create = true);


						CSaturationNodeAssociatedExpansionCache* fillExpansionData(CSaturationNodeAssociatedConceptExpansion* conceptExpansion, CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context);
						CSaturationNodeAssociatedDeterministicConceptExpansion* extendDeterministicExpansionData(CSaturationNodeAssociatedConceptExpansion* conceptExpansion, CSaturationNodeAssociatedExpansionCacheExpansionWriteData* snaecewd, CSaturationNodeAssociatedExpansionCacheContext* context);



						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					// protected variables
					protected:
						CSaturationNodeAssociatedExpansionCacheEntry* mEntryLinker;

						CSaturationNodeCacheUpdater* mSaturationNodeCacheUpdate;

						cint64 mConfAllowedNonDetExpansionCount;


						CSaturationNodeAssociatedExpansionCacheReader* mReaderLinker;
						QMutex mReaderSyncMutex;

						CCacheStatistics mCacheStat;
						CSaturationNodeAssociatedExpansionCacheContext mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CSATURATIONNODEASSOCIATEDEXPANSIONCACHE_H
