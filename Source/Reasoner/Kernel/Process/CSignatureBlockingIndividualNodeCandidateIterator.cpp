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

#include "CSignatureBlockingIndividualNodeCandidateIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingIndividualNodeCandidateIterator::CSignatureBlockingIndividualNodeCandidateIterator(CPROCESSSET<CIndividualProcessNode*>* candidateSet)
						: mCandidateSet(candidateSet) {

					if (mCandidateSet) {
						mItBegin = mCandidateSet->begin();
						mItEnd = mCandidateSet->end();
						mItLast = mItBegin;
					}
				}



				bool CSignatureBlockingIndividualNodeCandidateIterator::hasNext() {
					return mCandidateSet && mItBegin != mItEnd;
				}


				bool CSignatureBlockingIndividualNodeCandidateIterator::hasIndividualCandidates() {
					return mCandidateSet && !mCandidateSet->isEmpty();
				}

				cint64 CSignatureBlockingIndividualNodeCandidateIterator::next(bool moveNext) {
					CIndividualProcessNode* indi = nextIndividualCandidate(moveNext);
					if (indi) {
						return indi->getIndividualID();
					}
					return 0;
				}


				CIndividualProcessNode* CSignatureBlockingIndividualNodeCandidateIterator::nextIndividualCandidate(bool moveNext) {
					CIndividualProcessNode* indi = nullptr;
					if (mCandidateSet && mItBegin != mItEnd) {
						indi = *mItBegin;
						mItLast = mItBegin;
						if (moveNext) {
							++mItBegin;
						}
					}
					return indi;
				}


				bool CSignatureBlockingIndividualNodeCandidateIterator::removeLastIndividualCandidate() {
					if (mItLast != mItEnd) {						
						mItBegin = mCandidateSet->erase(mItLast);
						mItLast = mItBegin;
						return true;
					}
					return false;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
