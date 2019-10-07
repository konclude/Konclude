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

#include "CBlockingAlternativeSignatureBlockingCandidateData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				CBlockingAlternativeSignatureBlockingCandidateData::CBlockingAlternativeSignatureBlockingCandidateData() {
				}


				CBlockingAlternativeSignatureBlockingCandidateData* CBlockingAlternativeSignatureBlockingCandidateData::initSignatureBlockingCandidateData(CIndividualProcessNode* blockIndiCand, cint64 violatedResCount, cint64 violatedNonDetResCount, cint64 diffConceptCount) {
					mBlockIndiCandi = blockIndiCand;
					mViolatedResCount = violatedResCount;
					mViolatedNonDetResCount = violatedNonDetResCount;
					mDiffConceptCount = diffConceptCount;
					return this;
				}

				CBlockingAlternativeSignatureBlockingCandidateData* CBlockingAlternativeSignatureBlockingCandidateData::setSignatureBlockingCandidateNode(CIndividualProcessNode* blockIndiCan) {
					mBlockIndiCandi = blockIndiCan;
					return this;
				}

				CIndividualProcessNode* CBlockingAlternativeSignatureBlockingCandidateData::getSignatureBlockingCandidateNode() {
					return mBlockIndiCandi;
				}

				cint64 CBlockingAlternativeSignatureBlockingCandidateData::getConceptDifferenceCount() {
					return mDiffConceptCount;
				}

				cint64 CBlockingAlternativeSignatureBlockingCandidateData::getViolatedRestrictionCount() {
					return mViolatedResCount;
				}

				cint64 CBlockingAlternativeSignatureBlockingCandidateData::getViolatedNonDeterministicRestrictionCount() {
					return mViolatedNonDetResCount;
				}

				CBlockingAlternativeSignatureBlockingCandidateData* CBlockingAlternativeSignatureBlockingCandidateData::setConceptDifferenceCount(cint64 count) {
					mDiffConceptCount = count;
					return this;
				}

				CBlockingAlternativeSignatureBlockingCandidateData* CBlockingAlternativeSignatureBlockingCandidateData::setViolatedRestrictionCount(cint64 count) {
					mViolatedResCount = count;
					return this;
				}

				CBlockingAlternativeSignatureBlockingCandidateData* CBlockingAlternativeSignatureBlockingCandidateData::setViolatedNonDeterministicRestrictionCount(cint64 count) {
					mViolatedNonDetResCount = count;
					return this;
				}

				CBlockingAlternativeData::BLOCKINGALTERNATIVEDATA CBlockingAlternativeSignatureBlockingCandidateData::getBlockingAlternativeDataType() {
					return BADSIGNATUREBLOCKINGCANDIDATE;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
