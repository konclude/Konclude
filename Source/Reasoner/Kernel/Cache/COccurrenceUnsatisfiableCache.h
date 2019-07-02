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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHE_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHE_H

// Libraries includes
#include <QHash>
#include <QMutex>
#include <QSemaphore>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QReadWriteLock>

// Namespace includes
#include "CacheSettings.h"
#include "COccurrenceUnsatisfiableCacheReader.h"
#include "COccurrenceUnsatisfiableCacheEntry.h"
#include "CUnsatisfiableCache.h"
#include "CCacheValue.h"
#include "COccurrenceUnsatisfiableCacheUpdateSlotItem.h"
#include "COccurrenceUnsatisfiableCacheEntriesHash.h"
#include "COccurrenceUnsatisfiableCacheWriter.h"
#include "CCacheTaggingPool.h"
#include "CCacheStatistics.h"


// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/CConceptProcessData.h"

#include "Concurrent/CThread.h"

#include "Events/CWriteUnsatisfiableCacheEntryEvent.h"

// Logger includes
#include "Logger/CLogger.h"

#include <iostream>

namespace Konclude {

	using namespace Concurrent;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				using namespace Events;

#ifdef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
#define KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(a)
#else

#define KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(a) a
//#define KONCLUCE_OCCURUNSATCACHE_CACHING_STRING_INSTRUCTION(a)

#endif

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCache
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCache : public CThread, public CUnsatisfiableCache {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCache(qint64 writeUpdateSlotCount, const QString &threadIdentifierName = QString("Unsatisfiable-Cache"), CWatchDog *watchDogThread = 0);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCache();

						COccurrenceUnsatisfiableCacheReader *getCacheReader();
						COccurrenceUnsatisfiableCacheWriter *getCacheWriter();
						

						COccurrenceUnsatisfiableCacheEntry *getPrimarCacheEntry();
						void addUnsatisfiableCacheEntry(QList<CCacheValue> &itemList);


						bool isCacheWritePending();


						cint64 getCurrentCachingTag();
						CCacheStatistics* getCacheStatistics();

					// protected methods
					protected:

						virtual void threadStarted();
						virtual void threadStopped();

						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


						bool writeCacheTags(CCacheValue* cacheValue, cint64 cachingTag, cint64 cachedTag, cint64 cachingSize);
						bool writeCacheValues(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot, QList<CCacheValue> *cacheValueList);

						bool waitCacheWritePrepared();
						bool activateCacheUpdate(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot);


						QString getCachingConceptsDebugString(QList<CCacheValue> &itemList);

						bool testAlreadyCached(COccurrenceUnsatisfiableCacheUpdateSlotItem *updateSlot, QList<CCacheValue> *cacheValueList);

					// private methods
					private:

					// private variables
					private:
						QMutex cacheReaderSyncMutex;
						QMutex cacheWriterSyncMutex;
						QList<COccurrenceUnsatisfiableCacheReader *> cacheReaderList;
						QList<COccurrenceUnsatisfiableCacheWriter *> cacheWriterList;

						COccurrenceUnsatisfiableCacheEntry *primarCacheEntry;
						QList<COccurrenceUnsatisfiableCacheEntry *> container;

						qint64 updateSlotCount;
						QVector<COccurrenceUnsatisfiableCacheUpdateSlotItem *> updatesSlotItemVector;
						QList<COccurrenceUnsatisfiableCacheUpdateSlotItem *> usedUpdatesSlotsList;
						QList<COccurrenceUnsatisfiableCacheUpdateSlotItem *> notusedUpdatesSlotsList;
						COccurrenceUnsatisfiableCacheUpdateSlotItem* lastUpdateSlot;

						QMutex lockFreeMutexSync;
						bool canGetLockFreeAccess;
						qint64 lockFreeAccessCount;
						QSemaphore lockFreeAccessLockSemaphore;


						bool cacheWritingRequested;

						qint64 writeOperationsCount;

						CCacheTaggingPool mCacheTaggingPool;
						cint64 mCachingTag;

						CCacheStatistics mCachStat;

						QString mCachingString;
						QStringList mCachedStringList;

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHE_H
