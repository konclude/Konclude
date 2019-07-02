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

#include "COptimizedKPSetConceptInstantiatedItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetConceptInstantiatedItem::COptimizedKPSetConceptInstantiatedItem() {
			}


			CIndividual* COptimizedKPSetConceptInstantiatedItem::getIndividual() {
				return mIndividual;
			}

			COptimizedKPSetConceptInstancesHash* COptimizedKPSetConceptInstantiatedItem::getKnownPossibleInstancesHash() {
				return &mKnownPossibleInstancesHash;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::addKnownSameIndividual(CIndividual* individual) {
				mKnownSameIndividualSet.insert(individual);
				return this;
			}

			bool COptimizedKPSetConceptInstantiatedItem::hasKnownSameIndividual(CIndividual* individual) {
				return mKnownSameIndividualSet.contains(individual);
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::addPossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem) {
				mPossibleSameInstantiatedItemSet.insert(individualItem);
				return this;
			}


			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::removePossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem) {
				mPossibleSameInstantiatedItemSet.remove(individualItem);
				return this;
			}


			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleSameIndividualItem(COptimizedKPSetConceptInstantiatedItem* individualItem) {
				return mPossibleSameInstantiatedItemSet.contains(individualItem);
			}


			QSet<CIndividual*>* COptimizedKPSetConceptInstantiatedItem::getKnownSameIndividualSet() {
				return &mKnownSameIndividualSet;
			}

			QSet<COptimizedKPSetConceptInstantiatedItem*>* COptimizedKPSetConceptInstantiatedItem::getPossibleSameInstantiatedItemSet() {
				return &mPossibleSameInstantiatedItemSet;
			}

			cint64 COptimizedKPSetConceptInstantiatedItem::getPossibleSameInstantiatedItemCount() {
				return mPossibleSameInstantiatedItemSet.count();
			}

			cint64 COptimizedKPSetConceptInstantiatedItem::getTestingPossibleInstantiatedCount() {
				return mTestingPossibleInstantiatedCount;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setTestingPossibleInstantiatedCount(cint64 testCount) {
				mTestingPossibleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::incTestingPossibleInstantiatedCount(cint64 incCount) {
				mTestingPossibleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::decTestingPossibleInstantiatedCount(cint64 decCount) {
				mTestingPossibleInstantiatedCount -= decCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::initInstantiatedItem(CIndividual* individual) {
				mIndividual = individual;
				mKnownSameIndividualSet.insert(mIndividual);
				mTestingPossibleInstantiatedCount = 0;
				mPossibleInstantiatedCount = 0;
				mPossibleSameIndividualCount = 0;
				mToProcessPossibleSameIndividualsFlag = false;
				mPossibleSameIndividualsProcessingQueuedFlag = false;
				mItemSameIndividualMerged = false;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setTestingPossibleInstantiated(COptimizedKPSetConceptInstancesItem* item) {
				COptimizedKPSetConceptInstancesHashData& hashData = mKnownPossibleInstancesHash[item];
				hashData.mInstanceItemData->mTestingInstance = true;
				return this;
			}


			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleInstantiatedTesting() {
				return mTestingPossibleInstantiatedCount > 0;
			}

			cint64 COptimizedKPSetConceptInstantiatedItem::getPossibleInstantiatedCount() {
				return mPossibleInstantiatedCount;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setPossibleInstantiatedCount(cint64 testCount) {
				mPossibleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::incPossibleInstantiatedCount(cint64 incCount) {
				mPossibleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::decPossibleInstantiatedCount(cint64 decCount) {
				mPossibleInstantiatedCount -= decCount;
				return this;
			}


			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleInstantiated() {
				return mPossibleInstantiatedCount > 0;
			}


			cint64 COptimizedKPSetConceptInstantiatedItem::getTestingPossibleSameIndividualCount() {
				return mPossibleSameIndividualCount;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setTestingPossibleSameIndividualCount(cint64 testCount) {
				mPossibleSameIndividualCount = testCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::incTestingPossibleSameIndividualCount(cint64 incCount) {
				mPossibleSameIndividualCount += incCount;
				return this;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::decTestingPossibleSameIndividualCount(cint64 decCount) {
				mPossibleSameIndividualCount -= decCount;
				return this;
			}

			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleSameIndividuals() {
				return !mPossibleSameInstantiatedItemSet.isEmpty();
			}

			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleSameIndividualsTesting() {
				return mPossibleSameIndividualCount >= 0;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::takeTestingPossibleSameIndividualItem() {
				COptimizedKPSetConceptInstantiatedItem* possibleSameIndividualTestingItem = nullptr;
				if (!mPossibleSameInstantiatedItemSet.isEmpty()) {
					QSet<COptimizedKPSetConceptInstantiatedItem*>::iterator it = mPossibleSameInstantiatedItemSet.begin();
					possibleSameIndividualTestingItem = *it;
					mPossibleSameInstantiatedItemSet.erase(it);
				}
				return possibleSameIndividualTestingItem;
			}

			bool COptimizedKPSetConceptInstantiatedItem::hasToProcessPossibleSameIndividualsFlag() {
				return mToProcessPossibleSameIndividualsFlag;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setToProcessPossibleSameIndividualsFlag(bool toProcess) {
				mToProcessPossibleSameIndividualsFlag = toProcess;
				return this;
			}

			bool COptimizedKPSetConceptInstantiatedItem::hasPossibleSameIndividualsProcessingQueuedFlag() {
				return mPossibleSameIndividualsProcessingQueuedFlag;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setPossibleSameIndividualsProcessingQueuedFlag(bool processingQueued) {
				mPossibleSameIndividualsProcessingQueuedFlag = processingQueued;
				return this;
			}

			bool COptimizedKPSetConceptInstantiatedItem::isItemSameIndividualMerged() {
				return mItemSameIndividualMerged;
			}

			COptimizedKPSetConceptInstantiatedItem* COptimizedKPSetConceptInstantiatedItem::setItemSameIndividualMerged(bool merged) {
				mItemSameIndividualMerged = merged;
				return this;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
