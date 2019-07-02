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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRY_H
#define KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRY_H

// Libraries includes
#include <QHash>
#include <QList>

// Namespace includes
#include "CCacheEntry.h"
#include "CCacheValue.h"
#include "COccurrenceUnsatisfiableCacheEntriesHash.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		COccurrenceUnsatisfiableCacheEntry
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COccurrenceUnsatisfiableCacheEntry : public CCacheEntry {
					// public methods
					public:
						//! Constructor
						COccurrenceUnsatisfiableCacheEntry(const CCacheValue &cacheValue, COccurrenceUnsatisfiableCacheEntry *prevUnsatCacheEntry, qint64 writeUpdateSlotCount, qint64 currentActiveSlot, qint64 readCountVecSize = 1);

						//! Destructor
						virtual ~COccurrenceUnsatisfiableCacheEntry();

						bool isUnsatisfiableTermination();

						void setMinimumCandidate(qint64 value);
						void setMaximumCandidate(qint64 value);

						void setActiveSlot(qint64 slotIndex);

						COccurrenceUnsatisfiableCacheEntriesHash *setCacheEntriesHashSlotGetPrevious(qint64 slotIndex, COccurrenceUnsatisfiableCacheEntriesHash *entriesHash);
						void removeSlotCacheEntriesHash(qint64 slotIndex);

						qint64 getMaxTag();
						qint64 getMinTag();

						COccurrenceUnsatisfiableCacheEntriesHash *getCacheEntriesHash();
						COccurrenceUnsatisfiableCacheEntriesHash *getSlotCacheEntriesHash(qint64 slotIndex);

						COccurrenceUnsatisfiableCacheEntriesHash *updateSlotCacheHashGetPrevious(qint64 slotIndex);

						CCacheValue getCacheValue();

						qint64 getTotalReadCount();
						void incReadCount(qint64 readCountIndex);

						COccurrenceUnsatisfiableCacheEntry *getPreviousUnsatisfiableCacheEntry();

						bool isSerialized();

						COccurrenceUnsatisfiableCacheEntry *setCacheTerminationValuesList(QList<CCacheValue> *cacheTerminationValuesList);
						COccurrenceUnsatisfiableCacheEntry *copyCacheTerminationValuesList(QList<CCacheValue> *cacheTerminationValuesList);
						QList<CCacheValue> *getCacheTerminationValuesList();


					// protected methods
					protected:

					// private methods
					private:

					// private variables
					private:
						COccurrenceUnsatisfiableCacheEntry *prevEntry;

						qint64 updateSlotCount;
						qint64 activeSlot;
						COccurrenceUnsatisfiableCacheEntriesHash **cacheEntriesHashes;
						COccurrenceUnsatisfiableCacheEntriesHash *lastCacheEntriesHash;

						qint64 maxItemEntry;
						qint64 minItemEntry;

						bool unsatTerm;
						bool serialized;

						qint64 readCountCount;
						qint64 *readCountVec;

						CCacheValue cacheVal;

						QList<CCacheValue> *cacheTermValuesList;

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_COCCURRENCEUNSATISFIABLECACHEENTRY_H
