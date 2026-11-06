/*
 *		Copyright (C) 2013-2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLRECYCLER_H
#define KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLRECYCLER_H

// Libraries includes
#include <QMutex>

// Namespace includes
#include "MemorySettings.h"
#include "CMemoryPool.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {

			/*!
			 *
			 *		\class		CMemoryPoolRecycler
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		A process wide store of released memory pools of the default size, from
			 *					which the pool providers serve their next acquisitions before they allocate.
			 *
			 *					The pools of a task are acquired on the thread that creates the task and
			 *					released on the thread that completes it. Without this store every provider
			 *					hands released pools straight back to the allocator, which keeps them in the
			 *					cache of the releasing thread, while the acquiring thread keeps taking fresh
			 *					memory from the system; a long sequence of tests then grows the process by
			 *					the memory of every pool it ever acquired, most of it idle. The store is
			 *					bounded by a number of pools, and anything beyond that is freed as before.
			 *
			 */
			class CMemoryPoolRecycler {
				// public methods
				public:
					//! the one store of the process
					static CMemoryPoolRecycler* getInstance();

					//! a released pool of the default size, null if none is stored
					CMemoryPool* takeMemoryPool(cint64 minPoolSize);
					//! stores the pool if it has the default size and the store is not full, false if the caller has to free it
					bool giveMemoryPool(CMemoryPool* memoryPool);

					//! frees every stored pool, for the moment a reasoner is destroyed and its memory is to go back to the system (issue #45)
					void releaseStoredPools();
					cint64 getLimit();
					void setLimit(cint64 limit);
					cint64 getStoredCount();

				// protected methods
				protected:
					CMemoryPoolRecycler();
					~CMemoryPoolRecycler();

				// protected variables
				protected:
					QMutex mMutex;
					CMemoryPool* mPoolList;
					cint64 mStoredCount;
					cint64 mLimit;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CMEMORYPOOLRECYCLER_H
