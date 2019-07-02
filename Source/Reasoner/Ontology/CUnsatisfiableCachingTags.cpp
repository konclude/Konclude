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

#include "CUnsatisfiableCachingTags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CUnsatisfiableCachingTags::CUnsatisfiableCachingTags() {
				mMinUnsatCachedSize = CINT64_MAX;
			}

			cint64 CUnsatisfiableCachingTags::getMinUnsatisfiableCachedSize() {
				return mMinUnsatCachedSize;
			}

			CCachingTags* CUnsatisfiableCachingTags::setMinUnsatisfiableCachedSize(cint64 size) {
				mMinUnsatCachedSize = size;
				return this;
			}

			bool CUnsatisfiableCachingTags::setMinUnsatisfiableCachedSizeCandidate(cint64 size) {
				if (size < mMinUnsatCachedSize) {
					mMinUnsatCachedSize = size;
					return true;
				}
				return false;
			}

			bool CUnsatisfiableCachingTags::updateCachingTags(cint64 cachedTagCandidate, cint64 cachingNumberTag, cint64 sizeCandidate) {
				bool changed = false;
				changed |= setMinUnsatisfiableCachedSizeCandidate(sizeCandidate);
				changed |= setMaxCachedTagCandidate(cachedTagCandidate);
				changed |= setMinCachedTagCandidate(cachedTagCandidate);
				setLastCachingTag(cachingNumberTag);
				return changed;
			}


			bool CUnsatisfiableCachingTags::candidateTags(cint64& minMaxCachedTag, cint64& maxMinCachedTag, cint64& minUnsatCachedSize, cint64 requiredLastCachingTag) {
				if (mLastCachingTag >= requiredLastCachingTag) {
					minMaxCachedTag = qMin(minMaxCachedTag,mMaxCachedTag);
					maxMinCachedTag = qMax(maxMinCachedTag,mMinCachedTag);
					minUnsatCachedSize = qMin(minUnsatCachedSize,mMinUnsatCachedSize);
					return true;
				}
				return false;
			}

			bool CUnsatisfiableCachingTags::hasCandidateTags(cint64 minMaxCachedTag, cint64 maxMinCachedTag, cint64 requiredLastCachingTag) {
				if (mLastCachingTag >= requiredLastCachingTag) {
					if (minMaxCachedTag == mMaxCachedTag || maxMinCachedTag == mMinCachedTag) {
						return true;
					}
				}
				return false;
			}


			bool CUnsatisfiableCachingTags::candidateMinUnsatisfiableSize(cint64& minUnsatCachedSize, cint64 cachedTag) {
				if (mMinCachedTag == cachedTag && mMaxCachedTag == cachedTag) {
					minUnsatCachedSize = qMin(minUnsatCachedSize,mMinUnsatCachedSize);
					return true;
				}
				return false;
			}


		}; // end namespace Ontology


	}; // end namespace Reasoner

}; // end namespace Konclude
