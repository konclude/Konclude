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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEUPDATESLOTITEM_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEUPDATESLOTITEM_H

// Libraries includes
#include <QLinkedListIterator>
#include <QMutex>
#include <QSet>
#include <QHash>
#include <QAtomicInt>

// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "COccurrenceUnsatisfiableCacheEntry.h"
#include "COccurrenceUnsatisfiableCacheReader.h"


// Other includes
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDblLinker.hpp"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCacheUpdateSlotItem
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCacheUpdateSlotItem {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCacheUpdateSlotItem(qint64 updateSlotIndex);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCacheUpdateSlotItem();

						bool incReader();
						bool incReader(cint64 incCount);
						bool decReader();

						void addCacheEntry(COccurrenceUnsatisfiableCacheEntry *cacheEntry);
						void addCacheEntriesHash(COccurrenceUnsatisfiableCacheEntriesHash *cacheEntriesHash);

						void activateSlotUpdateItems();
						void cleanSlotUpdateItems();

						qint64 getSlotIndex();

						bool hasCacheReaders();


					// protected methods
					protected:


					// private methods
					private:

					// private variables
					private:
						QSet<COccurrenceUnsatisfiableCacheEntry *> updatedEntrySet;
						QLinkedList<COccurrenceUnsatisfiableCacheEntriesHash *> updatedHashesList;

						QAtomicInt mReaderSharingCount;
						bool mReaderUsing;

						qint64 readerCount;
						QMutex readerSyncMutex;

						qint64 slotIndex;

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEUPDATESLOTITEM_H
