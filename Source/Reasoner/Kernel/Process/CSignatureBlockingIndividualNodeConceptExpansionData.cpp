/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CSignatureBlockingIndividualNodeConceptExpansionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingIndividualNodeConceptExpansionData::CSignatureBlockingIndividualNodeConceptExpansionData() {
				}


				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::initBlockingExpansionData(CSignatureBlockingIndividualNodeConceptExpansionData* prevData) {
					if (prevData) {
						mSubsetTestedConDes = prevData->mSubsetTestedConDes;
						mBlockingConceptSignature = prevData->mBlockingConceptSignature;
						mBlockingConceptCount = prevData->mBlockingConceptCount;
						mExpandedContainedConceptCount = prevData->mExpandedContainedConceptCount;
						mLastUpdatedConExpCount = prevData->mLastUpdatedConExpCount;
						blockerIndiNode = prevData->blockerIndiNode;
						mLastUpdatedConCount = prevData->mLastUpdatedConCount;
						mReviewMarked = prevData->mReviewMarked;
						mReviewSubsetMarked = prevData->mReviewSubsetMarked;
						mIdenticConceptSetRequired = prevData->mIdenticConceptSetRequired;
						mStillConceptSetSubset = prevData->mStillConceptSetSubset;
					} else {
						mSubsetTestedConDes = nullptr;
						blockerIndiNode = nullptr;
						mBlockingConceptSignature = 0;
						mBlockingConceptCount = 0;
						mExpandedContainedConceptCount = 0;
						mLastUpdatedConExpCount = 0;
						mLastUpdatedConCount = 0;
						mReviewMarked = false;
						mReviewSubsetMarked = false;
						mIdenticConceptSetRequired = false;
						mStillConceptSetSubset = true;
					}
					return this;
				}


				CConceptDescriptor* CSignatureBlockingIndividualNodeConceptExpansionData::getLastSubsetTestedConceptDescriptor() {
					return mSubsetTestedConDes;
				}

				cint64 CSignatureBlockingIndividualNodeConceptExpansionData::setBlockingConceptSignature() {
					return mBlockingConceptSignature;
				}

				cint64 CSignatureBlockingIndividualNodeConceptExpansionData::getBlockingConceptCount() {
					return mBlockingConceptCount;
				}

				cint64 CSignatureBlockingIndividualNodeConceptExpansionData::getContinuousExpandedContainedConceptCount() {
					return mExpandedContainedConceptCount;
				}

				cint64 CSignatureBlockingIndividualNodeConceptExpansionData::getLastUpdatedConceptExpansionCount() {
					return mLastUpdatedConExpCount;
				}

				cint64 CSignatureBlockingIndividualNodeConceptExpansionData::getLastUpdatedConceptCount() {
					return mLastUpdatedConCount;
				}

				CIndividualProcessNode* CSignatureBlockingIndividualNodeConceptExpansionData::getBlockerIndividualNode() {
					return blockerIndiNode;
				}

				bool CSignatureBlockingIndividualNodeConceptExpansionData::isBlockingReviewMarked() {
					return mReviewMarked;
				}

				bool CSignatureBlockingIndividualNodeConceptExpansionData::isBlockingSubsetReviewMarked() {
					return mReviewSubsetMarked;
				}


				bool CSignatureBlockingIndividualNodeConceptExpansionData::isIdenticConceptSetRequired() {
					return mIdenticConceptSetRequired;
				}

				bool CSignatureBlockingIndividualNodeConceptExpansionData::isConceptSetStillSubset() {
					return mStillConceptSetSubset;
				}


				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setLastSubsetTestedConceptDescriptor(CConceptDescriptor* conDes) {
					mSubsetTestedConDes = conDes;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setBlockingConceptSignature(cint64 signature) {
					mBlockingConceptSignature = signature;
					return this;
				}


				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setBlockingConceptCount(cint64 conCount) {
					mBlockingConceptCount = conCount;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setContinuousExpandedContainedConceptCount(cint64 conCount) {
					mExpandedContainedConceptCount = conCount;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setLastUpdatedConceptExpansionCount(cint64 conCount) {
					mLastUpdatedConExpCount = conCount;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setLastUpdatedConceptCount(cint64 conCount) {
					mLastUpdatedConCount = conCount;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setBlockerIndividualNode(CIndividualProcessNode* node) {
					blockerIndiNode = node;
					return this;
				}


				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setBlockingReviewMarked(bool marked) {
					mReviewMarked = marked;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setBlockingSubsetReviewMarked(bool marked) {
					mReviewSubsetMarked = marked;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setIdenticConceptSetRequired(bool identicConceptSetRequired) {
					mIdenticConceptSetRequired = identicConceptSetRequired;
					return this;
				}

				CSignatureBlockingIndividualNodeConceptExpansionData* CSignatureBlockingIndividualNodeConceptExpansionData::setConceptSetStillSubset(bool stillSubset) {
					mStillConceptSetSubset = stillSubset;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
