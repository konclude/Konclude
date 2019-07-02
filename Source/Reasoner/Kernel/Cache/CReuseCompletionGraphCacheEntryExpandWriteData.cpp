/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CReuseCompletionGraphCacheEntryExpandWriteData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CReuseCompletionGraphCacheEntryExpandWriteData::CReuseCompletionGraphCacheEntryExpandWriteData() {
				}

				CReuseCompletionGraphCacheEntryExpandWriteData* CReuseCompletionGraphCacheEntryExpandWriteData::initExpandWriteData(CCACHINGLIST<CCacheValue>* cacheValueList, CCACHINGLIST<CCacheValue>* minimalCacheValueList, CSatisfiableCalculationJobInstantiation* jobInstantiation) {
					mJobInstantiation = jobInstantiation;
					mCacheValueList = cacheValueList;
					mMinimalValueList = minimalCacheValueList;
					mType = REUSECOMPLETIONGRAPHWRITEEXPANDDATATYPE;
					return this;
				}

				CCACHINGLIST<CCacheValue>* CReuseCompletionGraphCacheEntryExpandWriteData::getCacheValueList() {
					return mCacheValueList;
				}

				CCACHINGLIST<CCacheValue>* CReuseCompletionGraphCacheEntryExpandWriteData::getMinimalCacheValueList() {
					return mMinimalValueList;
				}


				CSatisfiableCalculationJobInstantiation* CReuseCompletionGraphCacheEntryExpandWriteData::getJobInstantiation() {
					return mJobInstantiation;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
