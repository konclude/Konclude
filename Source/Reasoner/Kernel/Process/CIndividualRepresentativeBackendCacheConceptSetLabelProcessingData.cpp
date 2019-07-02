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

#include "CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData() {
					mInitializedNodeLinker = nullptr;
					mRootQueuingNodeLinker = nullptr;
					mBranchQueuingNodeLinker = nullptr;
					mOnlyConceptSetProcessedCount = 0;
					mQueuedNodeInitializingCount = 0;
				}

				CXLinker<CIndividualProcessNode*>* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::getInitializedNodeLinker() {
					return mInitializedNodeLinker;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::appendInitializedNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker) {
					if (indiNodeLinker) {
						mInitializedNodeLinker = indiNodeLinker->append(mInitializedNodeLinker);
					}
					return this;
				}

				cint64 CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::getOnlyConceptSetProcessedCount() {
					return mOnlyConceptSetProcessedCount;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::incOnlyConceptSetProcessedCount(cint64 count) {
					mOnlyConceptSetProcessedCount += count;
					return this;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::decOnlyConceptSetProcessedCount(cint64 count) {
					mOnlyConceptSetProcessedCount -= count;
					return this;
				}



				CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::getRootQueuingNodeLinker() {
					return mRootQueuingNodeLinker;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::appendRootQueuingNodeLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeLinker) {
					if (indiNodeLinker) {
						mRootQueuingNodeLinker = indiNodeLinker->append(mRootQueuingNodeLinker);
					}
					return this;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::setRootQueuingNodeLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeLinker) {
					mRootQueuingNodeLinker = indiNodeLinker;
					return this;
				}



				cint64 CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::getQueuedNodeInitializingCount() {
					return mQueuedNodeInitializingCount;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::incQueuedNodeInitializingCount(cint64 count) {
					mQueuedNodeInitializingCount += count;
					return this;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::decQueuedNodeInitializingCount(cint64 count) {
					if (mQueuedNodeInitializingCount >= count) {
						mQueuedNodeInitializingCount -= count;
					}
					return this;
				}


				CXLinker<CIndividualProcessNode*>* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::getBranchQueuingNodeLinker() {
					return mBranchQueuingNodeLinker;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::appendBranchQueuingNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker) {
					mBranchQueuingNodeLinker = indiNodeLinker->append(mBranchQueuingNodeLinker);
					return this;
				}

				CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData::setBranchQueuingNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker) {
					mBranchQueuingNodeLinker = indiNodeLinker;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
