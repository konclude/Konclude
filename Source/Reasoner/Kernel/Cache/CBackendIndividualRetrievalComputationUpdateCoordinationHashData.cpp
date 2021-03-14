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

#include "CBackendIndividualRetrievalComputationUpdateCoordinationHashData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendIndividualRetrievalComputationUpdateCoordinationHashData::CBackendIndividualRetrievalComputationUpdateCoordinationHashData() {
					mAssociationUpdateId = -1;
					mProcessed = false;
					mComputationIntegrated = false;
					mComputationOrdered = false;
					mNewlyRetrieved = false;
					mUsageCount = 0;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::setComputationOrdered(bool ordered) {
					mComputationOrdered = ordered;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::setComputationIntegrated(bool integrated) {
					mComputationIntegrated = integrated;
					return this;
				}

				bool CBackendIndividualRetrievalComputationUpdateCoordinationHashData::isComputationOrdered() {
					return mComputationOrdered;
				}

				bool CBackendIndividualRetrievalComputationUpdateCoordinationHashData::isComputationIntegrated() {
					return mComputationIntegrated;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::setNewlyRetrieved(bool retrieved) {
					mNewlyRetrieved = retrieved;
					return this;
				}

				bool CBackendIndividualRetrievalComputationUpdateCoordinationHashData::isNewlyRetrieved() {
					return mNewlyRetrieved;
				}


				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::incUsageCount(cint64 count) {
					mUsageCount += count;
					return this;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::decUsageCount(cint64 count) {
					mUsageCount -= count;
					return this;
				}

				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHashData::getUsageCount() {
					return mUsageCount;
				}


				cint64 CBackendIndividualRetrievalComputationUpdateCoordinationHashData::getAssociationUpdateId() {
					return mAssociationUpdateId;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::setAssociationUpdateId(cint64 updateId) {
					mAssociationUpdateId = updateId;
					return this;
				}

				bool CBackendIndividualRetrievalComputationUpdateCoordinationHashData::isProcessed() {
					return mProcessed;
				}

				CBackendIndividualRetrievalComputationUpdateCoordinationHashData* CBackendIndividualRetrievalComputationUpdateCoordinationHashData::setProcessed(bool processed) {
					mProcessed = processed;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
