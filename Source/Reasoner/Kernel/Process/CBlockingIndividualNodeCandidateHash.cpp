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

#include "CBlockingIndividualNodeCandidateHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingIndividualNodeCandidateHash::CBlockingIndividualNodeCandidateHash(CProcessContext* context) : mContext(context) {
					mMemMan = mContext->getUsedMemoryAllocationManager();
					mBlockCandidateHash = nullptr;
					mUseBlockCandidateHash = nullptr;
					mPrevBlockCandidateHash = nullptr;
				}

				CBlockingIndividualNodeCandidateHash::~CBlockingIndividualNodeCandidateHash() {
				}


				CBlockingIndividualNodeCandidateHash* CBlockingIndividualNodeCandidateHash::initBlockingIndividualNodeCandidateHash(CBlockingIndividualNodeCandidateHash* prevBlockCandHash) {
					mBlockCandidateHash = nullptr;
					if (prevBlockCandHash) {
						mPrevBlockCandidateHash = prevBlockCandHash->mUseBlockCandidateHash;
						mUseBlockCandidateHash = mPrevBlockCandidateHash;
					} else {
						mPrevBlockCandidateHash = nullptr;
						mUseBlockCandidateHash = nullptr;
					}
					return this;
				}


				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateHash::getBlockingIndividualCandidateData(CConcept* initializationConcept, bool conceptNegation, bool create) {
					if (!mUseBlockCandidateHash || !mBlockCandidateHash && create) {
						mBlockCandidateHash = CObjectParameterizingAllocator< CPROCESSHASH< QPair<CConcept*,bool> ,CBlockingCandidateHashData>,CContext* >::allocateAndConstructAndParameterize(mMemMan,mContext);
						if (mPrevBlockCandidateHash) {
							*mBlockCandidateHash = *mPrevBlockCandidateHash;
						}
						mUseBlockCandidateHash = mBlockCandidateHash;
					}
					if (create) {
						CBlockingCandidateHashData& data = (*mUseBlockCandidateHash)[QPair<CConcept*,bool>(initializationConcept,conceptNegation)];
						if (!data.mCandidateIndiData) {
							data.mCandidateIndiData = CObjectParameterizingAllocator< CBlockingIndividualNodeCandidateData,CProcessContext* >::allocateAndConstructAndParameterize(mMemMan,mContext);
							data.mCandidateIndiData->initBlockingCandidateData(data.mPrevCandidateIndiData);
							data.mPrevCandidateIndiData = data.mCandidateIndiData;
						}
						return data.mPrevCandidateIndiData;
					} else {
						CBlockingCandidateHashData* data = nullptr;
						if (mUseBlockCandidateHash->tryGetValuePointer(QPair<CConcept*,bool>(initializationConcept,conceptNegation),data)) {
							return data->mPrevCandidateIndiData;
						}
					}
					return nullptr;
				}


				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateHash::getBlockingIndividualCandidateData(CConceptDescriptor* initializationConDes, bool create) {
					return getBlockingIndividualCandidateData(initializationConDes->getConcept(),initializationConDes->getNegation(),create);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
