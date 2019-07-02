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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCESTATISTICSCACHE_H

// Libraries includes
#include <QHash>
#include <QSet>
#include <QMutex>
#include <QSemaphore>

// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceStatisticsCacheData.h"
#include "COccurrenceStatisticsCacheContext.h"
#include "COccurrenceStatisticsCacheReader.h"
#include "COccurrenceStatisticsCacheWriter.h"
#include "CCacheStatistics.h"
#include "COccurrenceStatisticsCacheWriteData.h"


// Other includes
#include "Concurrent/CThread.h"

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
				 *		\class		COccurrenceStatisticsCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceStatisticsCache : public CThread {
					// public methods
					public:
						//! Constructor
						COccurrenceStatisticsCache(CConfiguration* config = nullptr, QString threadIdentifierName = "Occurrence-Statistics-Cache", CWatchDog *watchDogThread = nullptr);

						//! Destructor
						virtual ~COccurrenceStatisticsCache();

						COccurrenceStatisticsCacheReader* createCacheReader();
						COccurrenceStatisticsCacheWriter* createCacheWriter();


						COccurrenceStatisticsCache* writeCachedData(COccurrenceStatisticsCacheWriteData* writeData, CMemoryPool* memoryPools);

						CCacheStatistics* getCacheStatistics();


					// protected methods
					protected:

						
						
						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					// protected variables
					protected:
						CConfiguration* mConfig;

						COccurrenceStatisticsCacheData* mCacheData;


						cint64 mWriteDataCount;


						CCacheStatistics mCacheStat;
						COccurrenceStatisticsCacheContext mContext;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COccurrenceStatisticsCache_H
