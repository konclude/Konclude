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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEREADER_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEREADER_H

// Libraries includes
#include <QListIterator>
#include <QVectorIterator>
#include <QAtomicPointer>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "COccurrenceUnsatisfiableCache.h"
#include "COccurrenceUnsatisfiableCacheEntry.h"
#include "COccurrenceUnsatisfiableCacheUpdateSlotItem.h"
#include "CIncrementalUnsatisfiableCacheReader.h"


// Other includes
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CDblLinker.hpp"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCacheReader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCacheReader : public CIncrementalUnsatisfiableCacheReader {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCacheReader(COccurrenceUnsatisfiableCache *unsatisfiableCache);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCacheReader();

						virtual bool isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count);
						virtual CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *incrementalUnsatisfiableTest(CCacheValue *cacheValue, CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *cacheEntries, bool *unsatisfiable = 0, bool *continueTesting = 0);

						virtual QList<CCacheValue> getUnsatisfiableItems(QVector<CCacheValue> &itemVec, qint64 count);
						virtual bool incrementUnsatisfiableTest(CCacheValue *cacheValue, bool *continueTestingUseful = 0);
						virtual void incrementUnsatisfiableTestReset();

						virtual bool isUnsatisfiable(QVector<CCacheValue> &itemVec, qint64 count, QList<CCacheValue> *unsatisfiableItemList);

						virtual COccurrenceUnsatisfiableCacheUpdateSlotItem *changeUpdateSlot(COccurrenceUnsatisfiableCacheUpdateSlotItem *nextUpdateSlot);

						virtual QList<CCacheValue> getLastTestedUnsatisfiableItems();


						CXLinker<CCacheValue*>* getUnsatisfiableItems(CXLinker<CCacheValue*>* cacheValueTestLinker, CMemoryAllocationManager* memMan);
						cint64 getCurrentCachingTag();


					// protected methods
					protected:


					// private methods
					private:
						bool moveToNextSlot();

					// private variables
					private:
						COccurrenceUnsatisfiableCache *cache;
						COccurrenceUnsatisfiableCacheUpdateSlotItem *cacheSlotItem;
						QAtomicPointer<COccurrenceUnsatisfiableCacheUpdateSlotItem> mNextCacheSlotItemPointer;

						CDynamicExpandingMemoryManager<CDblLinker<COccurrenceUnsatisfiableCacheEntry *> > memManCacheEntryList;

						CDblLinker<COccurrenceUnsatisfiableCacheEntry *> *incCacheEntriesLinker;

						QList<CCacheValue> lastUnsatItems;

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEREADER_H
