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

#include "CBlockingIndividualNodeCandidateData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CBlockingIndividualNodeCandidateData::CBlockingIndividualNodeCandidateData(CProcessContext* context) : mContext(context),mCandidateIndiMap(context) {
					mMemMan = mContext->getUsedMemoryAllocationManager();
				}

				CBlockingIndividualNodeCandidateData::~CBlockingIndividualNodeCandidateData() {
				}

				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateData::initBlockingCandidateData(CBlockingIndividualNodeCandidateData* prevBlockCandHash) {
					if (prevBlockCandHash) {
						mCandidateIndiMap = prevBlockCandHash->mCandidateIndiMap;
						mMaxValidIndividualID = prevBlockCandHash->mMaxValidIndividualID;
					} else {					
						mMaxValidIndividualID = 0;
						mCandidateIndiMap.clear();
					}
					return this;
				}


				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateData::insertBlockingCandidateIndividualNode(CIndividualProcessNode* candidateIndi) {
					cint64 candidateIndividualID = candidateIndi->getIndividualNodeID();
					mCandidateIndiMap.insert(-candidateIndividualID,candidateIndi);
					return this;
				}

				CBlockingIndividualNodeCandidateIterator CBlockingIndividualNodeCandidateData::getBlockingCandidatesIndividualNodeIterator(cint64 candidateIndividualID) {
					return CBlockingIndividualNodeCandidateIterator(&mCandidateIndiMap,candidateIndividualID);
				}

				CBlockingIndividualNodeCandidateIterator CBlockingIndividualNodeCandidateData::getBlockingCandidatesIndividualNodeIterator(CIndividualProcessNode* candidateIndi) {
					return getBlockingCandidatesIndividualNodeIterator(candidateIndi->getIndividualNodeID());
				}

				cint64 CBlockingIndividualNodeCandidateData::getMaxValidIndividualID() {
					return mMaxValidIndividualID;
				}

				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateData::setMaxValidIndividualID(cint64 indiID) {
					mMaxValidIndividualID = indiID;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
