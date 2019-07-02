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

#include "CSignatureSatisfiableExpanderCacheHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheHasher::CSignatureSatisfiableExpanderCacheHasher(CCACHINGSET<CCacheValue>* cacheValueSet) {
					mCacheValueSet = cacheValueSet;
					mHashValue = 0;
					mCacheValueCount = cacheValueSet->count();
					mCacheValueLinker = nullptr;
					calculateHashValue(mCacheValueSet);
				}


				CSignatureSatisfiableExpanderCacheHasher::CSignatureSatisfiableExpanderCacheHasher(CExpanderCacheValueLinker* cacheValueLinker, cint64 count) {
					mCacheValueSet = nullptr;
					mHashValue = 0;
					mCacheValueCount = count;
					mCacheValueLinker = cacheValueLinker;
					calculateHashValue(mCacheValueLinker,mCacheValueCount);
				}



				CSignatureSatisfiableExpanderCacheHasher::CSignatureSatisfiableExpanderCacheHasher(const CSignatureSatisfiableExpanderCacheHasher& hasher) {
					mHashValue = hasher.mHashValue;
					mCacheValueSet = hasher.mCacheValueSet;
					mCacheValueLinker = hasher.mCacheValueLinker;
					mCacheValueCount = hasher.mCacheValueCount;
				}


				cint64 CSignatureSatisfiableExpanderCacheHasher::getHashValue() const {
					return mHashValue;
				}

				bool CSignatureSatisfiableExpanderCacheHasher::operator==(const CSignatureSatisfiableExpanderCacheHasher& hasher) const {
					if (mHashValue != hasher.mHashValue) {
						return false;
					}
					if (mCacheValueCount != hasher.mCacheValueCount) {
						return false;
					}
					if (mCacheValueLinker && hasher.mCacheValueLinker) {
						if (!hasEqualCacheValues(mCacheValueLinker,hasher.mCacheValueLinker,mCacheValueCount)) {
							return false;
						}
					} else if (mCacheValueSet && hasher.mCacheValueSet) {
						if (!hasEqualCacheValues(mCacheValueSet,hasher.mCacheValueSet)) {
							return false;
						}
					} else if (mCacheValueLinker && hasher.mCacheValueSet) {
						if (!hasEqualCacheValues(mCacheValueLinker,hasher.mCacheValueSet,mCacheValueCount)) {
							return false;
						}
					} else {
						if (!hasEqualCacheValues(hasher.mCacheValueLinker,mCacheValueSet,mCacheValueCount)) {
							return false;
						}
					}
					return false;
				}


				void CSignatureSatisfiableExpanderCacheHasher::calculateHashValue(CCACHINGSET<CCacheValue>* cacheValueSet) {
					mHashValue = 0;
					for (CCACHINGSET<CCacheValue>::const_iterator it = cacheValueSet->constBegin(), itEnd = cacheValueSet->constEnd(); it != itEnd; ++it) {
						CCacheValue cacheValue(*it);
						extendHashValue(&cacheValue);
					}
				}

				void CSignatureSatisfiableExpanderCacheHasher::calculateHashValue(CExpanderCacheValueLinker* cacheValueLinker, cint64 count) {
					mHashValue = 0;
					CExpanderCacheValueLinker* cacheValueLinkerIt = cacheValueLinker;
					cint64 nr = 0;
					while (cacheValueLinkerIt && nr++ < count) {
						extendHashValue(cacheValueLinkerIt->getCacheValue());
						cacheValueLinkerIt = cacheValueLinkerIt->getNext();
					}
				}


				void CSignatureSatisfiableExpanderCacheHasher::extendHashValue(CCacheValue* cacheValue) {
					uint nextHashValue = qHash(*cacheValue);
					mHashValue += nextHashValue;
				}


				bool CSignatureSatisfiableExpanderCacheHasher::hasEqualCacheValues(CExpanderCacheValueLinker* cacheValueLinker, CExpanderCacheValueLinker* cacheValueLinker2, cint64 count) const {
					cint64 nr = 0;
					while (cacheValueLinker && cacheValueLinker2 && nr++ < count) {
						if (*cacheValueLinker->getCacheValue() != *cacheValueLinker2->getCacheValue()) {
							return false;
						}
						cacheValueLinker = cacheValueLinker->getNext();
						cacheValueLinker2 = cacheValueLinker2->getNext();
					}
					if (nr < count) {
						if (cacheValueLinker || cacheValueLinker2) {
							return false;
						}
					}
					return true;
				}


				bool CSignatureSatisfiableExpanderCacheHasher::hasEqualCacheValues(CCACHINGSET<CCacheValue>* cacheValueSet, CCACHINGSET<CCacheValue>* cacheValueSet2) const {
					CCACHINGSET<CCacheValue>::const_iterator it1 = cacheValueSet->constBegin(), it1End = cacheValueSet->constEnd();
					CCACHINGSET<CCacheValue>::const_iterator it2 = cacheValueSet2->constBegin(), it2End = cacheValueSet2->constEnd();
					while (it1 != it1End) {
						if (*it1 != *it2) {
							return false;
						}
						++it1;
						++it2;
					}
					return true;
				}


				bool CSignatureSatisfiableExpanderCacheHasher::hasEqualCacheValues(CExpanderCacheValueLinker* cacheValueLinker, CCACHINGSET<CCacheValue>* cacheValueSet, cint64 count) const {
					CExpanderCacheValueLinker* cacheValueLinkerIt = cacheValueLinker;
					cint64 nr = 0;
					while (cacheValueLinkerIt && nr++ < count) {
						if (!cacheValueSet->contains(*cacheValueLinkerIt->getCacheValue())) {
							return false;
						}
						cacheValueLinkerIt = cacheValueLinkerIt->getNext();
					}
					return true;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
