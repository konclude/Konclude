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

#include "CIndividualNodeBlockingTestData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualNodeBlockingTestData::CIndividualNodeBlockingTestData() {
					mBlockingIndiNode = nullptr;
					//mFirstEntailedAncestorDepth = 0;
					//mConceptDes = 0;
				}


				CIndividualNodeBlockingTestData::~CIndividualNodeBlockingTestData() {
				}

				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::initBlockData(CIndividualNodeBlockingTestData* prevBlockData) {
					if (prevBlockData) {
						mBlockingIndiNode = prevBlockData->mBlockingIndiNode;
						mLastAddedCoreConDes = prevBlockData->mLastAddedCoreConDes;
						mLastCoreBlockCandConDes = prevBlockData->mLastCoreBlockCandConDes;
						mLastCoreBlockCandNodeDiff = prevBlockData->mLastCoreBlockCandNodeDiff;
					} else {
						mBlockingIndiNode = nullptr;
						mLastAddedCoreConDes = nullptr;
						mLastCoreBlockCandConDes = nullptr;
						mLastCoreBlockCandNodeDiff = 0;
					}
					return this;
				}

				CIndividualProcessNode* CIndividualNodeBlockingTestData::getBlockingIndividualNode() {
					return mBlockingIndiNode;
				}


				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::clearBlockingIndividualNode() {
					mBlockingIndiNode = nullptr;
					return this;
				}


				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::setBlockingIndividualNode(CIndividualProcessNode* blockingIndividualNode) {
					mBlockingIndiNode = blockingIndividualNode;
					return this;
				}


				CConceptDescriptor* CIndividualNodeBlockingTestData::getLastCoreBlockingCandidateConceptDescriptor() {
					return mLastCoreBlockCandConDes;
				}

				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::setLastCoreBlockingCandidateConceptDescriptor(CConceptDescriptor* lastCoreBlockCandConDes) {
					mLastCoreBlockCandConDes = lastCoreBlockCandConDes;
					return this;
				}


				cint64 CIndividualNodeBlockingTestData::getLastCoreBlockingCandidateConceptNodeDifference() {
					return mLastCoreBlockCandNodeDiff;
				}

				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::setLastCoreBlockingCandidateConceptNodeDifference(cint64 diffCount) {
					mLastCoreBlockCandNodeDiff = diffCount;
					return this;
				}

				CCoreConceptDescriptor* CIndividualNodeBlockingTestData::getLastAddedCoreConceptDescriptor() {
					return mLastAddedCoreConDes;
				}

				CIndividualNodeBlockingTestData* CIndividualNodeBlockingTestData::setLastAddedCoreConceptDescriptor(CCoreConceptDescriptor* lastCoreConDes) {
					mLastAddedCoreConDes = lastCoreConDes;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
