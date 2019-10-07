/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CCACHESTATISTICS_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CCACHESTATISTICS_H

// Libraries includes


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CCacheStatistics
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CCacheStatistics {
					// public methods
					public:
						CCacheStatistics();

						CCacheStatistics* reset();

						CCacheStatistics* incCacheEntriesCount(cint64 incCount = 1);
						CCacheStatistics* setMemoryConsumption(cint64 memoryConsumption);

						cint64 getCacheEntriesCount();
						cint64 getMemoryConsumption();

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mMemoryConsumption;
						cint64 mCacheEntriesCount;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CCACHESTATISTICS_H
