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

#include "CSignatureBlockingCandidateHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSignatureBlockingCandidateHash::CSignatureBlockingCandidateHash(CProcessContext* context) : mContext(context),mSigBlockCandidateHash(context) {
					mMemMan = mContext->getUsedMemoryAllocationManager();
				}

				CSignatureBlockingCandidateHash* CSignatureBlockingCandidateHash::initSignatureBlockingCandidateHash(CSignatureBlockingCandidateHash* prevSigBlockCandHash) {
					if (prevSigBlockCandHash) {
						mSigBlockCandidateHash = prevSigBlockCandHash->mSigBlockCandidateHash;
					} else {
						mSigBlockCandidateHash.clear();
					}
					return this;
				}

				CSignatureBlockingCandidateHash* CSignatureBlockingCandidateHash::insertSignatureBlockingCandidate(cint64 signature, CIndividualProcessNode* individualCandidate) {
					CSignatureBlockingCandidateData& data = mSigBlockCandidateHash[signature];
					CXLinker<cint64>* candLinker = CObjectAllocator< CXLinker<cint64> >::allocateAndConstruct(mMemMan);
					cint64 indiCandID = individualCandidate->getIndividualID();
					candLinker->initLinker(indiCandID,data.mCandidateIndiLinker);
					data.mCandidateIndiLinker = candLinker;
					data.mCandidateCount++;
					return this;
				}


				CSignatureBlockingCandidateHash* CSignatureBlockingCandidateHash::insertSignatureBlockingCandidates(cint64 signature, CXLinker<cint64>* candidateLinker) {
					CSignatureBlockingCandidateData& data = mSigBlockCandidateHash[signature];
					data.mCandidateCount += candidateLinker->getCount();
					if (data.mCandidateIndiLinker) {
						candidateLinker->append(data.mCandidateIndiLinker);
					}
					data.mCandidateIndiLinker = candidateLinker;
					return this;
				}


				CSignatureBlockingCandidateIterator CSignatureBlockingCandidateHash::getBlockingCandidatesIterator(cint64 signature) {
					CSignatureBlockingCandidateData* data = mSigBlockCandidateHash.valuePointer(signature);
					if (data) {
						return CSignatureBlockingCandidateIterator(signature,data->mCandidateIndiLinker);
					} else {
						return CSignatureBlockingCandidateIterator(signature,nullptr);
					}
				}

				cint64 CSignatureBlockingCandidateHash::getBlockingCandidatesCount(cint64 signature) {
					CSignatureBlockingCandidateData* data = mSigBlockCandidateHash.valuePointer(signature);
					if (data) {
						return data->mCandidateCount;
					}
					return 0;
				}

				cint64 CSignatureBlockingCandidateHash::getBlockingCandidatesCount(CConceptSetSignature* signature) {
					return getBlockingCandidatesCount(signature->getSignatureValue());
				}


				CSignatureBlockingCandidateHash* CSignatureBlockingCandidateHash::insertSignatureBlockingCandidate(CConceptSetSignature* signature, CIndividualProcessNode* individualCandidate) {
					return insertSignatureBlockingCandidate(signature->getSignatureValue(),individualCandidate);
				}

				CSignatureBlockingCandidateIterator CSignatureBlockingCandidateHash::getBlockingCandidatesIterator(CConceptSetSignature* signature) {
					return getBlockingCandidatesIterator(signature->getSignatureValue());
				}


				CSignatureIterator CSignatureBlockingCandidateHash::getSignatureIterator() {
					return CSignatureIterator(&mSigBlockCandidateHash);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
