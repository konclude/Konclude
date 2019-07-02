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

#include "CSignatureSatisfiableExpanderCacheEntry.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheEntry::CSignatureSatisfiableExpanderCacheEntry(CSignatureSatisfiableExpanderCacheContext* context) : mContext(context) {
					mDetExpandValueLinker = nullptr;
					mExpandBranchedLinker = nullptr;
					mTagExpanderCacheValueHash = nullptr;
					mDetExpandCount = 0;
					mSatisfiable = false;
					mSatisfiableWithoutBranchedConcept = false;
					mMultipleExpanded = false;
				}



				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::appendExpanderCacheValueLinker(CExpanderCacheValueLinker* linker) {
					CExpanderCacheValueLinker* linkerIt = linker;
					while (linkerIt) {
						++mDetExpandCount;
						linkerIt = linkerIt->getNext();
					}
					if (!mDetExpandValueLinker) {
						mDetExpandValueLinker = linker;
					} else {
						mDetExpandValueLinker->append(linker);
					}
					return this;
				}


				CExpanderCacheValueLinker* CSignatureSatisfiableExpanderCacheEntry::getExpanderCacheValueLinker() {
					return mDetExpandValueLinker;
				}


				
				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::appendExpanderBranchedLinker(CExpanderBranchedLinker* linker) {
					if (mExpandBranchedLinker) {
						mExpandBranchedLinker = linker->append(mExpandBranchedLinker);
					} else {
						mExpandBranchedLinker = linker;
					}
					return this;
				}


				CExpanderBranchedLinker* CSignatureSatisfiableExpanderCacheEntry::getExpanderBranchedLinker() {
					return mExpandBranchedLinker;
				}

				CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* CSignatureSatisfiableExpanderCacheEntry::getTagExpanderCacheValueHash() {
					return mTagExpanderCacheValueHash;
				}


				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::setTagExpanderCacheValueHash(CCACHINGHASH<cint64,CExpanderCacheValueLinker*>* hash) {
					mTagExpanderCacheValueHash = hash;
					return this;
				}


				cint64 CSignatureSatisfiableExpanderCacheEntry::getExpanderCacheValueCount() {
					return mDetExpandCount;
				}

				bool CSignatureSatisfiableExpanderCacheEntry::isSatisfiable() {
					return mSatisfiable;
				}

				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::setSatisfiable(bool satisfiable) {
					mSatisfiable = satisfiable;
					return this;
				}


				bool CSignatureSatisfiableExpanderCacheEntry::isSatisfiableWithoutBranchedConcepts() {
					return mSatisfiableWithoutBranchedConcept;
				}

				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::setSatisfiableWithoutBranchedConcepts(bool sat) {
					mSatisfiableWithoutBranchedConcept = sat;
					return this;
				}

				bool CSignatureSatisfiableExpanderCacheEntry::hasMultipleExpanded() {
					return mMultipleExpanded;
				}

				CSignatureSatisfiableExpanderCacheEntry* CSignatureSatisfiableExpanderCacheEntry::setMultipleExpanded(bool expanded) {
					mMultipleExpanded = expanded;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
