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

#include "CCachingTags.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CCachingTags::CCachingTags() {
				mLastCachingTag = 0;
				mMinCachedTag = CINT64_MAX;
				mMaxCachedTag = CINT64_MIN;
			}


			cint64 CCachingTags::getLastCachingTag() {
				return mLastCachingTag;
			}

			cint64 CCachingTags::getMaxCachedTag() {
				return mMaxCachedTag;
			}

			cint64 CCachingTags::getMinCachedTag() {
				return mMinCachedTag;
			}

			CCachingTags* CCachingTags::setLastCachingTag(cint64 tag) {
				mLastCachingTag = tag;
				return this;
			}

			CCachingTags* CCachingTags::setMaxCachedTag(cint64 tag) {
				mMaxCachedTag = tag;
				return this;
			}

			CCachingTags* CCachingTags::setMinCachedTag(cint64 tag) {
				mMinCachedTag = tag;
				return this;
			}

			bool CCachingTags::setMaxCachedTagCandidate(cint64 tag) {
				if (tag > mMaxCachedTag) {
					mMaxCachedTag = tag;
					return true;
				}
				return false;
			}

			bool CCachingTags::setMinCachedTagCandidate(cint64 tag) {
				if (tag < mMinCachedTag) {
					mMinCachedTag = tag;
					return true;
				}
				return false;
			}


		}; // end namespace Ontology


	}; // end namespace Reasoner

}; // end namespace Konclude
