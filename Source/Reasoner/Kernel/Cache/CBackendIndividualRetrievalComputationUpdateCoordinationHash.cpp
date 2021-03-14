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

#include "CBackendIndividualRetrievalComputationUpdateCoordinationHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendIndividualRetrievalComputationUpdateCoordinationHash::CBackendIndividualRetrievalComputationUpdateCoordinationHash() {
					mTotalIncompletelyHandledCount = 0;
					mHashRemainingCount = 0;
					mHashRemovedCount = 0;
					mHashProcessedCount = 0;
					mHashComputationCount = 0;
					mUsageCount = 0;
					mCorrectionIncompletelyHandledCount = 0;
					mBasicPrecomputationMode = false;
					mBasicPrecomputationFinished = false;
				}

				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getHashProcessedCount() {
					return mHashProcessedCount;
				}



				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setBasicPrecomputationMode(bool basicMode) {
					mBasicPrecomputationMode = basicMode;
					return this;
				}
				bool CBackendIndividualRetrievalComputationUpdateCoordinationHash::hasBasicPrecomputationMode() {
					return mBasicPrecomputationMode;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incHashProcessedCount(cint64 count) {
					mHashProcessedCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decHashProcessedCount(cint64 count) {
					mHashProcessedCount -= count;
					return this;
				}


				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getHashComputationCount() {
					return mHashComputationCount;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incHashComputationCount(cint64 count) {
					mHashComputationCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decHashComputationCount(cint64 count) {
					mHashComputationCount -= count;
					return this;
				}


				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getHashRemainingCount() {
					return mHashRemainingCount;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incHashRemainingCount(cint64 count) {
					mHashRemainingCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decHashRemainingCount(cint64 count) {
					mHashRemainingCount -= count;
					return this;
				}
				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setHashRemainingCount(cint64 count) {
					mHashRemainingCount = count;
					return this;
				}







				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getHashRemovedCount() {
					return mHashRemovedCount;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incHashRemovedCount(cint64 count) {
					mHashRemovedCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decHashRemovedCount(cint64 count) {
					mHashRemovedCount -= count;
					return this;
				}
				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setHashRemovedCount(cint64 count) {
					mHashRemovedCount = count;
					return this;
				}





				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHash::createCoordinationData() {
					CBackendIndividualRetrievalComputationUpdateCoordinationHashData* data = new CBackendIndividualRetrievalComputationUpdateCoordinationHashData();
					return data;
				}

				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getUsageCount() {
					return mUsageCount;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incUsageCount(cint64 count) {
					mUsageCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decUsageCount(cint64 count) {
					mUsageCount -= count;
					return this;
				}




				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getApproximateRemainingIncompletelyHandledCount() {
					return qMax(mTotalIncompletelyHandledCount + mCorrectionIncompletelyHandledCount - (mHashProcessedCount - mHashRemovedCount) - mHashComputationCount, (qint64)0);
				}





				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getApproximateTotalIncompletelyHandledCount() {
					return mTotalIncompletelyHandledCount;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incApproximateTotalIncompletelyHandledCount(cint64 count) {
					mTotalIncompletelyHandledCount += count;
					return this; 
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decApproximateTotalIncompletelyHandledCount(cint64 count) {
					mTotalIncompletelyHandledCount -= count;
					return this;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setApproximateTotalIncompletelyHandledCount(cint64 count) {
					mTotalIncompletelyHandledCount = count;
					return this;
				}




				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHash::getApproximateCorrectionIncompletelyHandledCount() {
					return mCorrectionIncompletelyHandledCount;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::incApproximateCorrectionIncompletelyHandledCount(cint64 count) {
					mCorrectionIncompletelyHandledCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::decApproximateCorrectionIncompletelyHandledCount(cint64 count) {
					mCorrectionIncompletelyHandledCount -= count;
					return this;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setApproximateCorrectionIncompletelyHandledCount(cint64 count) {
					mCorrectionIncompletelyHandledCount = count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHash* CBackendIndividualRetrievalComputationUpdateCoordinationHash::setBasicPrecomputationFinished(bool basicModeFinished) {
					mBasicPrecomputationFinished = basicModeFinished;
					return this;
				}

				bool CBackendIndividualRetrievalComputationUpdateCoordinationHash::hasBasicPrecomputationFinished() {
					return mBasicPrecomputationFinished;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
