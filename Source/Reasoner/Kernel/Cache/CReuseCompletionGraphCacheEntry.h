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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEENTRY_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEENTRY_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CCacheValue.h"
#include "CReuseCompletionGraphCacheContext.h"


// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationJobInstantiation.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Task;

			namespace Cache {

				/*! 
				 *
				 *		\class		CReuseCompletionGraphCacheEntry
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReuseCompletionGraphCacheEntry {
					// public methods
					public:
						//! Constructor
						CReuseCompletionGraphCacheEntry(CReuseCompletionGraphCacheContext* context);

						CCACHINGSET<CCacheValue>* getIncompatibleValues();
						CCACHINGSET<CCacheValue>* getEntailedValues();
						CCACHINGSET<CCacheValue>* getMinimalValues();
						CSatisfiableCalculationJobInstantiation* getJobInstantiation();

						CReuseCompletionGraphCacheEntry* setJobInstantiation(CSatisfiableCalculationJobInstantiation* jobInstantiation);

						CReuseCompletionGraphCacheEntry* setEntailedValues(CCACHINGSET<CCacheValue>* valueSet);
						CReuseCompletionGraphCacheEntry* addEntailedValue(const CCacheValue& cacheValue);

						CReuseCompletionGraphCacheEntry* setIncompatibleValues(CCACHINGSET<CCacheValue>* valueSet);
						CReuseCompletionGraphCacheEntry* addIncompatibleValue(const CCacheValue& cacheValue);

						CReuseCompletionGraphCacheEntry* setMinimalValues(CCACHINGSET<CCacheValue>* valueSet);
						CReuseCompletionGraphCacheEntry* addMinimalValue(const CCacheValue& cacheValue);
						bool hasMinimalValue(const CCacheValue& cacheValue);

						CReuseCompletionGraphCacheEntry* setEntryID(cint64 id);
						cint64 getEntryID();

					// protected methods
					protected:

					// protected variables
					protected:
						CReuseCompletionGraphCacheContext* mContext;
						cint64 mEntryID;
						bool mMinimal;

						CCACHINGSET<CCacheValue> mIncompatibleValues;
						CCACHINGSET<CCacheValue> mEntailedValues;
						CCACHINGSET<CCacheValue> mMinimalValues;

						CSatisfiableCalculationJobInstantiation* mJobInstance;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CREUSECOMPLETIONGRAPHCACHEENTRY_H
