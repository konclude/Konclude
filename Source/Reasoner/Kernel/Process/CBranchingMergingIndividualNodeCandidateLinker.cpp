/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CBranchingMergingIndividualNodeCandidateLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBranchingMergingIndividualNodeCandidateLinker::CBranchingMergingIndividualNodeCandidateLinker(CProcessContext* processContext) {
					mProcessContext = processContext;
					mMergingIndiNodeCandidate = nullptr;
					mMergingLink = nullptr;
				}


				CBranchingMergingIndividualNodeCandidateLinker::~CBranchingMergingIndividualNodeCandidateLinker() {
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingIndividualNodeCandidateLinker::initBranchingMergingIndividualNodeCandidate(CBranchingMergingIndividualNodeCandidateLinker* prevRest) {
					if (prevRest) {
						mMergingIndiNodeCandidate = prevRest->mMergingIndiNodeCandidate;
						mMergingLink = prevRest->mMergingLink;
					} else {
						mMergingIndiNodeCandidate = nullptr;
						mMergingLink = nullptr;
					}
					return this;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingIndividualNodeCandidateLinker::initBranchingMergingIndividualNodeCandidate(CIndividualProcessNode* mergingIndiNodeCand, CIndividualLinkEdge* mergingLink) {
					mMergingLink = mergingLink;
					mMergingIndiNodeCandidate = mergingIndiNodeCand;
					return this;
				}


				CIndividualProcessNode* CBranchingMergingIndividualNodeCandidateLinker::getMergingIndividualNodeCandidate() {
					return mMergingIndiNodeCandidate;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingIndividualNodeCandidateLinker::setMergingIndividualNodeCandidate(CIndividualProcessNode* mergingIndiNodeCand) {
					mMergingIndiNodeCandidate = mergingIndiNodeCand;
					return this;
				}


				CIndividualLinkEdge* CBranchingMergingIndividualNodeCandidateLinker::getMergingIndividualLink() {
					return mMergingLink;
				}

				CBranchingMergingIndividualNodeCandidateLinker* CBranchingMergingIndividualNodeCandidateLinker::setMergingIndividualLink(CIndividualLinkEdge* mergingLink) {
					mMergingLink = mergingLink;
					return this;
				}

				bool CBranchingMergingIndividualNodeCandidateLinker::isCandidateBlockableAndCreator() {
					return mMergingIndiNodeCandidate && mMergingLink && mMergingIndiNodeCandidate->isBlockableIndividual() && mMergingLink->isCreatorIndividualID(mMergingIndiNodeCandidate);
				}


				bool CBranchingMergingIndividualNodeCandidateLinker::operator<=(const CBranchingMergingIndividualNodeCandidateLinker& otherIndiNodeCandLinker) {
					return true;
				}


			}; // end namespace Kernel

		}; // end namespace Process

	}; // end namespace Reasoner

}; // end namespace Konclude
