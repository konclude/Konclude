/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CReuseCompletionGraphCacheEntry.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CReuseCompletionGraphCacheEntry::CReuseCompletionGraphCacheEntry(CReuseCompletionGraphCacheContext* context) : mContext(context),mIncompatibleValues(context),mEntailedValues(context) {
					mJobInstance = nullptr;
					mEntryID = 0;
				}

				CCACHINGSET<CCacheValue>* CReuseCompletionGraphCacheEntry::getIncompatibleValues() {
					return &mIncompatibleValues;
				}

				CCACHINGSET<CCacheValue>* CReuseCompletionGraphCacheEntry::getEntailedValues() {
					return &mEntailedValues;
				}

				CCACHINGSET<CCacheValue>* CReuseCompletionGraphCacheEntry::getMinimalValues() {
					return &mMinimalValues;
				}

				CSatisfiableCalculationJobInstantiation* CReuseCompletionGraphCacheEntry::getJobInstantiation() {
					return mJobInstance;
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::setJobInstantiation(CSatisfiableCalculationJobInstantiation* jobInstantiation) {
					mJobInstance = jobInstantiation;
					return this;
				}

				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::setEntailedValues(CCACHINGSET<CCacheValue>* valueSet) {
					if (valueSet) {
						mEntailedValues += *valueSet;
					}
					return this;
				}

				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::addEntailedValue(const CCacheValue& cacheValue) {
					mEntailedValues.insert(cacheValue);
					return this;
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::setIncompatibleValues(CCACHINGSET<CCacheValue>* valueSet) {
					if (valueSet) {
						mIncompatibleValues += *valueSet;
					}
					return this;
				}

				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::addIncompatibleValue(const CCacheValue& cacheValue) {
					mIncompatibleValues.insert(cacheValue);
					return this;
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::setMinimalValues(CCACHINGSET<CCacheValue>* valueSet) {
					if (valueSet) {
						mMinimalValues += *valueSet;
					}
					return this;
				}

				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::addMinimalValue(const CCacheValue& cacheValue) {
					mMinimalValues.insert(cacheValue);
					return this;
				}

				bool CReuseCompletionGraphCacheEntry::hasMinimalValue(const CCacheValue& cacheValue) {
					return mMinimalValues.contains(cacheValue);
				}


				CReuseCompletionGraphCacheEntry* CReuseCompletionGraphCacheEntry::setEntryID(cint64 id) {
					mEntryID = id;
					return this;
				}

				cint64 CReuseCompletionGraphCacheEntry::getEntryID() {
					return mEntryID;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
