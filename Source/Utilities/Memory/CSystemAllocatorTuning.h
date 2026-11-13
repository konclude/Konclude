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

#ifndef KONCLUDE_UTILITIES_MEMORY_CSYSTEMALLOCATORTUNING_H
#define KONCLUDE_UTILITIES_MEMORY_CSYSTEMALLOCATORTUNING_H

// Libraries includes
#include <QString>

// Namespace includes
#include "MemorySettings.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {

			/*!
			 *
			 *		\class		CSystemAllocatorTuning
			 *		\version	0.1
			 *		\brief		Adjusts the malloc of the GNU C library to the allocation pattern of the
			 *					reasoner, and hands its free memory back to the system on request.
			 *
			 *					By default glibc gives the free top of every thread's heap back to the
			 *					kernel as soon as it exceeds a threshold that it adapts to the sizes it has
			 *					seen, and the next allocation faults the pages in again. The reasoner frees
			 *					and allocates pools of tasks on many threads all the time, so on Linux the
			 *					heaps shrink and grow continuously; with transparent huge pages every fault
			 *					also clears a 2 MB page. That cost a fifth of the classification time of
			 *					SNOMED CT on 32 processors (about 90 s against 75 s with fixed thresholds). The
			 *					tuning fixes the trim threshold, the top pad and the mmap threshold, so that
			 *					the heaps keep what they have while the reasoner works, and
			 *					releaseFreeMemory gives it back once a reasoner has been destroyed.
			 *
			 *					Only the GNU C library is affected; with another C library, or when jemalloc
			 *					replaces malloc, both methods do nothing that matters. The environment
			 *					variable KONCLUDE_MALLOC_TUNING=off leaves glibc's thresholds at their
			 *					defaults; releaseFreeMemory trims the heaps either way.
			 *
			 */
			class CSystemAllocatorTuning {
				// public methods
				public:
					//! applies the tuning once per process, returns a description for the log, empty if nothing was changed
					static QString applyTuning();

					//! hands the free memory of all heaps back to the system, true if some was released
					static bool releaseFreeMemory();

				// private methods
				private:
					CSystemAllocatorTuning();

			};

		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

#endif // KONCLUDE_UTILITIES_MEMORY_CSYSTEMALLOCATORTUNING_H
