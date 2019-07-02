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

#include "CIndividualNodeSatisfiableExpandingCacheStoringData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeSatisfiableExpandingCacheStoringData::CIndividualNodeSatisfiableExpandingCacheStoringData() {
				}


				CIndividualNodeSatisfiableExpandingCacheStoringData::~CIndividualNodeSatisfiableExpandingCacheStoringData() {
				}


				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::initSatisfiableExpandingCacheRetrievalData(CIndividualNodeSatisfiableExpandingCacheStoringData* prevData) {
					if (prevData) {
						mNodeSuccBranched = prevData->mNodeSuccBranched;
						mPrevCached = prevData->mPrevCached;
						mCachingError = prevData->mCachingError;
						mLastCachedSignature = prevData->mLastCachedSignature;
						mMinNodeBranchingTag = prevData->mMinNodeBranchingTag;
						mLastCachedConDes = prevData->mLastCachedConDes;
						mPrevSatCached = prevData->mPrevSatCached;
					} else {
						mNodeSuccBranched = false;
						mPrevCached = false;
						mCachingError = false;
						mPrevSatCached = false;
						mLastCachedSignature = 0;
						mMinNodeBranchingTag = -1;
						mLastCachedConDes = nullptr;
					}
					return this;
				}




				bool CIndividualNodeSatisfiableExpandingCacheStoringData::hasIndividualNodeOrSuccessorBranchedConcept() {
					return mNodeSuccBranched;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setIndividualNodeOrSuccessorBranchedConcept(bool branched) {
					mNodeSuccBranched = branched;
					return this;
				}

				bool CIndividualNodeSatisfiableExpandingCacheStoringData::hasPreviousCached() {
					return mPrevCached;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setPreviousCached(bool cached) {
					mPrevCached = cached;
					return this;
				}

				cint64 CIndividualNodeSatisfiableExpandingCacheStoringData::getLastCachedSignature() {
					return mLastCachedSignature;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setLastCachedSignature(cint64 signature) {
					mLastCachedSignature = signature;
					return this;
				}

				CConceptDescriptor* CIndividualNodeSatisfiableExpandingCacheStoringData::getLastCachedConceptDescriptor() {
					return mLastCachedConDes;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setLastCachedConceptDescriptor(CConceptDescriptor* conDes) {
					mLastCachedConDes = conDes;
					return this;
				}

				cint64 CIndividualNodeSatisfiableExpandingCacheStoringData::getMinimalIndividualNodeBranchingTag() {
					return mMinNodeBranchingTag;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setMinimalIndividualNodeBranchingTag(cint64 branchingTag) {
					mMinNodeBranchingTag = branchingTag;
					return this;
				}

				bool CIndividualNodeSatisfiableExpandingCacheStoringData::hasCachingError() {
					return mCachingError;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setCachingError(bool error) {
					mCachingError = error;
					return this;
				}

				bool CIndividualNodeSatisfiableExpandingCacheStoringData::hasPreviousSatisfiableCached() {
					return mPrevSatCached;
				}

				CIndividualNodeSatisfiableExpandingCacheStoringData* CIndividualNodeSatisfiableExpandingCacheStoringData::setPreviousSatisfiableCached(bool cached) {
					mPrevSatCached = cached;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
