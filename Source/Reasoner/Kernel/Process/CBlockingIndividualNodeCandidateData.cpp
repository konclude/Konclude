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
						mMaxValidIndividualID = -1;
						mCandidateIndiMap.clear();
					}
					return this;
				}


				CBlockingIndividualNodeCandidateData* CBlockingIndividualNodeCandidateData::insertBlockingCandidateIndividualNode(CIndividualProcessNode* candidateIndi) {
					cint64 candidateIndividualID = candidateIndi->getIndividualID();
					mCandidateIndiMap.insert(-candidateIndividualID,candidateIndi);
					return this;
				}

				CBlockingIndividualNodeCandidateIterator CBlockingIndividualNodeCandidateData::getBlockingCandidatesIndividualNodeIterator(cint64 candidateIndividualID) {
					return CBlockingIndividualNodeCandidateIterator(&mCandidateIndiMap,candidateIndividualID);
				}

				CBlockingIndividualNodeCandidateIterator CBlockingIndividualNodeCandidateData::getBlockingCandidatesIndividualNodeIterator(CIndividualProcessNode* candidateIndi) {
					return getBlockingCandidatesIndividualNodeIterator(candidateIndi->getIndividualID());
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
