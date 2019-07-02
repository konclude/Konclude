/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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
						mLastCoreBlockCandNodeDiff = nullptr;
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
