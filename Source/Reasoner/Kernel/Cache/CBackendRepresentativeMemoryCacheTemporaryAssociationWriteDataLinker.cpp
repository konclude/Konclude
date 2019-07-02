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

#include "CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker() {
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::initAccociationWriteData(CIndividual* individual) {
					mIndividual = individual;
					mIndividualID = individual->getIndividualID();
					mReferredLabelData = nullptr;
					mReferredTmpLabelData = nullptr;
					mReferredTmpCardData = nullptr;
					mReferredCardData = nullptr;
					mCompletelySaturated = true;
					mCompletelyHandled = true;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredTemporaryLabelData(CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* referredTmpLabelData) {
					if (!referredTmpLabelData->isCompletelyHandled()) {
						mCompletelyHandled = false;
					}
					if (!referredTmpLabelData->isCompletelySaturated()) {
						mCompletelySaturated = false;
					}
					mReferredTmpLabelData = referredTmpLabelData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredLabelData(CBackendRepresentativeMemoryLabelCacheItem* referredLabelData) {
					if (!referredLabelData->isCompletelyHandled()) {
						mCompletelyHandled = false;
					}
					if (!referredLabelData->isCompletelySaturated()) {
						mCompletelySaturated = false;
					}
					mReferredLabelData = referredLabelData;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredTemporaryCardinalityData(CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* referredTmpCardData) {
					mReferredTmpCardData = referredTmpCardData;
					return this;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setReferredCardinalityData(CBackendRepresentativeMemoryCardinalityCacheItem* referredCardData) {
					mReferredCardData = referredCardData;
					return this;
				}



				CIndividual* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getIndividual() {
					return mIndividual;
				}

				cint64 CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getIndividualID() {
					return mIndividualID;
				}


				CBackendRepresentativeMemoryCacheTemporaryLabelWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredTemporaryLabelData() {
					return mReferredTmpLabelData;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredLabelData() {
					return mReferredLabelData;
				}


				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::isCompletelyHandled() {
					return mCompletelyHandled;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setCompletelyHandled(bool completelyHandled) {
					mCompletelyHandled = completelyHandled;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::isCompletelySaturated() {
					return mCompletelySaturated;
				}

				CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::setCompletelySaturated(bool completelySaturated) {
					mCompletelySaturated = completelySaturated;
					return this;
				}


				CBackendRepresentativeMemoryCacheTemporaryCardinalityWriteDataLinker* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredTemporaryCardinalityData() {
					return mReferredTmpCardData;
				}

				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCacheTemporaryAssociationWriteDataLinker::getReferredCardinalityData() {
					return mReferredCardData;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
