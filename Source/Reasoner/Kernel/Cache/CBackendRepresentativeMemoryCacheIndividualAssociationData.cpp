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

#include "CBackendRepresentativeMemoryCacheIndividualAssociationData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CBackendRepresentativeMemoryCacheIndividualAssociationData::CBackendRepresentativeMemoryCacheIndividualAssociationData() {
				}



				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::initAssociationData(CBackendRepresentativeMemoryCacheIndividualAssociationData* assData) {
					mIndividual = assData->mIndividual;
					mCompletelyHandled = assData->mCompletelyHandled;
					mCompletelySaturated = assData->mCompletelySaturated;
					mIncompletelyMarked = assData->mIncompletelyMarked;
					mCardinalityCacheEntry = assData->mCardinalityCacheEntry;
					mLabelCacheEntry = assData->mLabelCacheEntry;
					return this;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::initAssociationData(CIndividual* individual) {
					mIndividual = individual;
					mCompletelyHandled = false;
					mCompletelySaturated = false;
					mIncompletelyMarked = false;
					mCardinalityCacheEntry = nullptr;
					mLabelCacheEntry = nullptr;
					return this;
				}

				CBackendRepresentativeMemoryLabelCacheItem* CBackendRepresentativeMemoryCacheIndividualAssociationData::getBackendLabelCacheEntry() {
					return mLabelCacheEntry;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setBackendLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem* cacheEntry) {
					mLabelCacheEntry = cacheEntry;
					return this;
				}


				CBackendRepresentativeMemoryCardinalityCacheItem* CBackendRepresentativeMemoryCacheIndividualAssociationData::getBackendCardinalityCacheEntry() {
					return mCardinalityCacheEntry;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setBackendCardinalityCacheEntry(CBackendRepresentativeMemoryCardinalityCacheItem* cacheEntry) {
					mCardinalityCacheEntry = cacheEntry;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::isCompletelyHandled() {
					return mCompletelyHandled;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setCompletelyHandled(bool completelyHandled) {
					mCompletelyHandled = completelyHandled;
					return this;
				}


				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::isCompletelySaturated() {
					return mCompletelySaturated;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setCompletelySaturated(bool completelySaturated) {
					mCompletelySaturated = completelySaturated;
					return this;
				}

				bool CBackendRepresentativeMemoryCacheIndividualAssociationData::isIncompletelyMarked() {
					return mIncompletelyMarked;
				}

				CBackendRepresentativeMemoryCacheIndividualAssociationData* CBackendRepresentativeMemoryCacheIndividualAssociationData::setIncompletelyMarked(bool marked) {
					mIncompletelyMarked;
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
