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

#include "CSystemAllocatorTuning.h"
#include "CMemoryPoolRecycler.h"

#include <mutex>
#include <cstdlib>

#if defined(__GLIBC__)
#include <malloc.h>
#endif

#if defined(KONCLUDE_JEMALLOC_IN_LIBRARY)
// jemalloc's control interface; hidden in the library with the rest of jemalloc, see
// CLibraryAllocatorWrappers.cpp
extern "C" int mallctl(const char* name, void* oldp, size_t* oldlenp, void* newp, size_t newlen);
#endif


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			QString CSystemAllocatorTuning::applyTuning() {
				static std::once_flag onceFlag;
				static QString description;
				std::call_once(onceFlag, []() {
#if defined(KONCLUDE_JEMALLOC_IN_LIBRARY)
					// the library allocates with jemalloc, glibc's malloc serves only the program
					description = QString("jemalloc inside the library");
#elif defined(__GLIBC__)
					const char* setting = std::getenv("KONCLUDE_MALLOC_TUNING");
					if (setting) {
						QString value = QString::fromLatin1(setting).trimmed().toLower();
						if (value == "off" || value == "0" || value == "false" || value == "no") {
							return;
						}
					}
					// Setting any of the three also switches off glibc's own adaption of the
					// thresholds. A trim threshold beyond the 64 MB of a thread's heap means that
					// the heaps are never trimmed while the reasoner works; the mmap threshold is
					// glibc's maximum, 32 MB, so that the pools come from the heaps and not from
					// separate mappings, which are unmapped when freed.
					const int trimThreshold = 1024 * 1024 * 1024;
					const int topPad = 256 * 1024 * 1024;
					const int mmapThreshold = 32 * 1024 * 1024;
					bool trimSet = mallopt(M_TRIM_THRESHOLD, trimThreshold) == 1;
					bool padSet = mallopt(M_TOP_PAD, topPad) == 1;
					bool mmapSet = mallopt(M_MMAP_THRESHOLD, mmapThreshold) == 1;
					description = QString("glibc malloc tuned: trim threshold %1, top pad %2, mmap threshold %3")
							.arg(trimSet ? QString("%1 MB").arg(trimThreshold / (1024 * 1024)) : QString("unchanged"))
							.arg(padSet ? QString("%1 MB").arg(topPad / (1024 * 1024)) : QString("unchanged"))
							.arg(mmapSet ? QString("%1 MB").arg(mmapThreshold / (1024 * 1024)) : QString("unchanged"));
#endif
				});
				return description;
			}


			bool CSystemAllocatorTuning::releaseFreeMemory() {
				// the pools the recycler holds are the largest part of what a destroyed reasoner leaves
				// behind; mapped from the system, they go back to it here (issue #45)
				CMemoryPoolRecycler::getInstance()->releaseStoredPools();
#if defined(KONCLUDE_JEMALLOC_IN_LIBRARY)
				// returns the unused pages of every arena to the system; 4096 is MALLCTL_ARENAS_ALL
				return mallctl("arena.4096.purge", nullptr, nullptr, nullptr, 0) == 0;
#elif defined(__GLIBC__)
				return malloc_trim(0) == 1;
#else
				return false;
#endif
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude
