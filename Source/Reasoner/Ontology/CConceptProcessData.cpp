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

#include "CConceptProcessData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CConceptProcessData::CConceptProcessData() {
				mConceptRoleBranchTrigger = nullptr;
				mReplacementData = nullptr;
				mRefLinking = nullptr;
				mUnsatCachingTags[0] = nullptr;
				mUnsatCachingTags[1] = nullptr;
				mInvalidatedRefLinking = false;
				mCoreConceptFlags[0] = false;
				mCoreConceptFlags[1] = false;
				mPropagationIntoCreationDirection = false;
				mInferRelevantFlag = false;
				mBranchingStatistics = nullptr;
			}


			CConceptProcessData* CConceptProcessData::initConceptProcessExtensionData(CConceptRoleBranchingTrigger* conceptRoleBranchTrigger, CReplacementData* replacementData) {
				mConceptRoleBranchTrigger = conceptRoleBranchTrigger;
				mReplacementData = replacementData;
				return this;
			}

			CConceptRoleBranchingTrigger* CConceptProcessData::getConceptRoleBranchTrigger() {
				return mConceptRoleBranchTrigger;
			}

			bool CConceptProcessData::hasConceptRoleBranchTrigger() {
				return mConceptRoleBranchTrigger != nullptr;
			}

			CConceptProcessData* CConceptProcessData::setConceptRoleBranchTrigger(CConceptRoleBranchingTrigger* conceptRoleBranchTrigger) {
				mConceptRoleBranchTrigger = conceptRoleBranchTrigger;
				return this;
			}




			CUnsatisfiableCachingTags* CConceptProcessData::getUnsatisfiableCachingTags(bool conceptNegation) {
				return mUnsatCachingTags[conceptNegation];
			}



			CConceptProcessData* CConceptProcessData::setUnsatisfiableCachingTags(bool conceptNegation, CUnsatisfiableCachingTags* tags) {
				mUnsatCachingTags[conceptNegation] = tags;
				return this;
			}

			bool CConceptProcessData::hasUnsatisfiableCachingTags(bool conceptNegation) {
				return mUnsatCachingTags[conceptNegation] != nullptr;
			}


			CReplacementData* CConceptProcessData::getReplacementData() {
				return mReplacementData;
			}

			CConceptProcessData* CConceptProcessData::setReplacementData(CReplacementData* replacementData) {
				mReplacementData = replacementData;
				return this;
			}

			CConceptReferenceLinking* CConceptProcessData::getConceptReferenceLinking() {
				return mRefLinking;
			}

			CConceptProcessData* CConceptProcessData::setConceptReferenceLinking(CConceptReferenceLinking* refLinking) {
				mRefLinking = refLinking;
				return this;
			}

			bool CConceptProcessData::isInvalidatedReferenceLinking() {
				return mInvalidatedRefLinking;
			}

			CConceptProcessData* CConceptProcessData::setInvalidatedReferenceLinking(bool invalidatedReferenceLinking) {
				mInvalidatedRefLinking = invalidatedReferenceLinking;
				return this;
			}



			bool CConceptProcessData::isCoreBlockingConcept(bool negated) {
				return mCoreConceptFlags[negated];
			}

			CConceptProcessData* CConceptProcessData::setCoreBlockingConcept(bool negated, bool coreBlockingConcept) {
				mCoreConceptFlags[negated] = coreBlockingConcept;
				return this;
			}

			bool CConceptProcessData::hasPropagationIntoCreationDirection() {
				return mPropagationIntoCreationDirection;
			}

			CConceptProcessData* CConceptProcessData::setPropagationIntoCreationDirection(bool propagationIntoCreationDirection) {
				mPropagationIntoCreationDirection = propagationIntoCreationDirection;
				return this;
			}

			bool CConceptProcessData::hasInferRelevantFlag() {
				return mInferRelevantFlag;
			}

			CConceptProcessData* CConceptProcessData::setInferRelevantFlag(bool flag) {
				mInferRelevantFlag = flag;
				return this;
			}


			CBranchingStatistics* CConceptProcessData::getBranchingStatistics() {
				return mBranchingStatistics;
			}

			CConceptProcessData* CConceptProcessData::setBranchingStatistics(CBranchingStatistics* branchingStatistics) {
				mBranchingStatistics = branchingStatistics;
				return this;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
