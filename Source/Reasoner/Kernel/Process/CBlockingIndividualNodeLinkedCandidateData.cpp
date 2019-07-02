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

#include "CBlockingIndividualNodeLinkedCandidateData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingIndividualNodeLinkedCandidateData::CBlockingIndividualNodeLinkedCandidateData(CProcessContext* context) : mContext(context) {
					mMemMan = mContext->getUsedMemoryAllocationManager();
				}

				CBlockingIndividualNodeLinkedCandidateData::~CBlockingIndividualNodeLinkedCandidateData() {
				}

				CBlockingIndividualNodeLinkedCandidateData* CBlockingIndividualNodeLinkedCandidateData::initBlockingCandidateData(CBlockingIndividualNodeLinkedCandidateData* prevBlockCandData) {
					if (prevBlockCandData) {
						mCandidateCount = prevBlockCandData->mCandidateCount;
						mCandLinker = prevBlockCandData->mCandLinker;
					} else {					
						mCandidateCount = 0;
						mCandLinker = nullptr;
					}
					return this;
				}


				CBlockingIndividualNodeLinkedCandidateData* CBlockingIndividualNodeLinkedCandidateData::addBlockingCandidateIndividualNode(CIndividualProcessNode* candidateIndi) {
					CBlockingIndividualNodeLinker* linker = CObjectAllocator<CBlockingIndividualNodeLinker>::allocateAndConstruct(mMemMan);
					linker->initBlockingIndividualNodeLinker(candidateIndi);
					mCandLinker = linker->append(mCandLinker);
					++mCandidateCount;
					return this;
				}

				CBlockingIndividualNodeLinker* CBlockingIndividualNodeLinkedCandidateData::getBlockingCandidatesIndividualNodeLinker() {
					return mCandLinker;
				}


				cint64 CBlockingIndividualNodeLinkedCandidateData::getCandidateCount() {
					return mCandidateCount;
				}

				CBlockingIndividualNodeLinkedCandidateData* CBlockingIndividualNodeLinkedCandidateData::setCandidateCount(cint64 candCount) {
					mCandidateCount = candCount;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
