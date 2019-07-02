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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHINGFLAGS_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHINGFLAGS_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CBackendRepresentativeMemoryCachingFlags
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendRepresentativeMemoryCachingFlags {
					// public methods
					public:

						//! Constructor
						CBackendRepresentativeMemoryCachingFlags();


						CBackendRepresentativeMemoryCachingFlags* initCachingStatusFlags(cint64 flags = 0);


						bool isCompletelyHandled();
						CBackendRepresentativeMemoryCachingFlags* setCompletelyHandled(bool completelyHandled);

						bool isCompletelySaturated();
						CBackendRepresentativeMemoryCachingFlags* setCompletelySaturated(bool completelySaturated);

						bool hasNondeterministicElements();
						CBackendRepresentativeMemoryCachingFlags* setNondeterministicElements(bool nondeterministicElements);


						static const cint64 FLAG_COMPLETELY_HANDLED								= 0x0001;
						static const cint64 FLAG_COMPLETELY_SATURATED							= 0x0002;
						static const cint64 FLAG_NONDETERMINISTIC_ELEMENTS						= 0x0004;


						bool hasStatusFlags(cint64 flags);
						bool hasStatusFlagsPartially(cint64 flags);
						CBackendRepresentativeMemoryCachingFlags* setStatusFlags(cint64 flags);
						CBackendRepresentativeMemoryCachingFlags* addStatusFlags(cint64 flags);
						CBackendRepresentativeMemoryCachingFlags* clearStatusFlags(cint64 flags);
						cint64 getStatusFlags();



					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mStatusFlags;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDREPRESENTATIVEMEMORYCACHINGFLAGS_H
