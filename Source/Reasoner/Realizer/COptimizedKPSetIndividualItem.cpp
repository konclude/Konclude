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

#include "COptimizedKPSetIndividualItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedKPSetIndividualItem::COptimizedKPSetIndividualItem() {
			}


			CIndividual* COptimizedKPSetIndividualItem::getIndividual() {
				return mIndividual;
			}

			COptimizedKPSetConceptInstancesHash* COptimizedKPSetIndividualItem::getKnownPossibleInstancesHash() {
				return &mKnownPossibleInstancesHash;
			}

			COptimizedKPSetRoleNeighbourInstancesHash* COptimizedKPSetIndividualItem::getKnownPossibleRoleNeighboursInstancesHash() {
				return &mKnownPossibleRoleNeigbourInstancesHash;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::addKnownSameIndividual(CIndividual* individual) {
				mKnownSameIndividualSet.insert(individual);
				return this;
			}

			bool COptimizedKPSetIndividualItem::hasKnownSameIndividual(CIndividual* individual) {
				return mKnownSameIndividualSet.contains(individual);
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::addPossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem) {
				mPossibleSameInstantiatedItemSet.insert(individualItem);
				return this;
			}


			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::removePossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem) {
				mPossibleSameInstantiatedItemSet.remove(individualItem);
				return this;
			}


			bool COptimizedKPSetIndividualItem::hasPossibleSameIndividualItem(COptimizedKPSetIndividualItem* individualItem) {
				return mPossibleSameInstantiatedItemSet.contains(individualItem);
			}


			QSet<CIndividual*>* COptimizedKPSetIndividualItem::getKnownSameIndividualSet() {
				return &mKnownSameIndividualSet;
			}

			QSet<COptimizedKPSetIndividualItem*>* COptimizedKPSetIndividualItem::getPossibleSameInstantiatedItemSet() {
				return &mPossibleSameInstantiatedItemSet;
			}

			cint64 COptimizedKPSetIndividualItem::getPossibleSameInstantiatedItemCount() {
				return mPossibleSameInstantiatedItemSet.count();
			}

			cint64 COptimizedKPSetIndividualItem::getTestingPossibleInstantiatedCount() {
				return mTestingPossibleInstantiatedCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTestingPossibleInstantiatedCount(cint64 testCount) {
				mTestingPossibleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incTestingPossibleInstantiatedCount(cint64 incCount) {
				mTestingPossibleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decTestingPossibleInstantiatedCount(cint64 decCount) {
				mTestingPossibleInstantiatedCount -= decCount;
				return this;
			}

			cint64 COptimizedKPSetIndividualItem::getTestingPossibleRoleInstantiatedCount() {
				return mTestingPossibleRoleInstantiatedCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTestingPossibleRoleInstantiatedCount(cint64 testCount) {
				mTestingPossibleRoleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incTestingPossibleRoleInstantiatedCount(cint64 incCount) {
				mTestingPossibleRoleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decTestingPossibleRoleInstantiatedCount(cint64 decCount) {
				mTestingPossibleRoleInstantiatedCount -= decCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::initInstantiatedItem(CIndividual* individual) {
				mIndividual = individual;
				mKnownSameIndividualSet.insert(mIndividual);
				mTestingPossibleInstantiatedCount = 0;
				mTestingPossibleRoleInstantiatedCount = 0;
				mPossibleInstantiatedCount = 0;
				mPossibleRoleInstantiatedCount = 0;
				mPossibleSameIndividualCount = 0;
				mToProcessPossibleSameIndividualsFlag = false;
				mPossibleSameIndividualsProcessingQueuedFlag = false;
				mItemSameIndividualMerged = false;
				mInitializingRoleCandidateCount = 0;
				mInitializingRoleCandidatesQueuedFlag = false;
				mAllRoleCandidatesInitializedFlag = false;
				mTmpNominalConcept = nullptr;
				mIndiDepTrackingCollector = nullptr;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTestingPossibleInstantiated(COptimizedKPSetConceptInstancesItem* item) {
				COptimizedKPSetConceptInstancesHashData& hashData = mKnownPossibleInstancesHash[item];
				hashData.mInstanceItemData->mTestingInstance = true;
				return this;
			}


			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTestingPossibleInstantiated(COptimizedKPSetRoleInstancesItem* roleItem, COptimizedKPSetIndividualItem* successorItem) {
				COptimizedKPSetRoleInstancesData* instanceItem = mKnownPossibleRoleNeigbourInstancesHash.getRoleSuccessorInstanceItemData(roleItem,successorItem);
				if (instanceItem) {
					instanceItem->mTestingInstance = true;
				}
				return this;
			}

			bool COptimizedKPSetIndividualItem::hasPossibleInstantiatedTesting() {
				return mTestingPossibleInstantiatedCount > 0;
			}

			bool COptimizedKPSetIndividualItem::hasPossibleRoleInstantiatedTesting() {
				return mTestingPossibleRoleInstantiatedCount > 0;
			}

			cint64 COptimizedKPSetIndividualItem::getPossibleInstantiatedCount() {
				return mPossibleInstantiatedCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setPossibleInstantiatedCount(cint64 testCount) {
				mPossibleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incPossibleInstantiatedCount(cint64 incCount) {
				mPossibleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decPossibleInstantiatedCount(cint64 decCount) {
				mPossibleInstantiatedCount -= decCount;
				return this;
			}


			bool COptimizedKPSetIndividualItem::hasPossibleInstantiated() {
				return mPossibleInstantiatedCount > 0;
			}



			cint64 COptimizedKPSetIndividualItem::getPossibleRoleInstantiatedCount() {
				return mPossibleRoleInstantiatedCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setPossibleRoleInstantiatedCount(cint64 testCount) {
				mPossibleRoleInstantiatedCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incPossibleRoleInstantiatedCount(cint64 incCount) {
				mPossibleRoleInstantiatedCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decPossibleRoleInstantiatedCount(cint64 decCount) {
				mPossibleRoleInstantiatedCount -= decCount;
				return this;
			}


			bool COptimizedKPSetIndividualItem::hasPossibleRoleInstantiated() {
				return mPossibleRoleInstantiatedCount > 0;
			}


			cint64 COptimizedKPSetIndividualItem::getTestingPossibleSameIndividualCount() {
				return mPossibleSameIndividualCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTestingPossibleSameIndividualCount(cint64 testCount) {
				mPossibleSameIndividualCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incTestingPossibleSameIndividualCount(cint64 incCount) {
				mPossibleSameIndividualCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decTestingPossibleSameIndividualCount(cint64 decCount) {
				mPossibleSameIndividualCount -= decCount;
				return this;
			}

			bool COptimizedKPSetIndividualItem::hasPossibleSameIndividuals() {
				return !mPossibleSameInstantiatedItemSet.isEmpty();
			}

			bool COptimizedKPSetIndividualItem::hasPossibleSameIndividualsTesting() {
				return mPossibleSameIndividualCount >= 0;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::takeTestingPossibleSameIndividualItem() {
				COptimizedKPSetIndividualItem* possibleSameIndividualTestingItem = nullptr;
				if (!mPossibleSameInstantiatedItemSet.isEmpty()) {
					QSet<COptimizedKPSetIndividualItem*>::iterator it = mPossibleSameInstantiatedItemSet.begin();
					possibleSameIndividualTestingItem = *it;
					mPossibleSameInstantiatedItemSet.erase(it);
				}
				return possibleSameIndividualTestingItem;
			}

			bool COptimizedKPSetIndividualItem::hasToProcessPossibleSameIndividualsFlag() {
				return mToProcessPossibleSameIndividualsFlag;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setToProcessPossibleSameIndividualsFlag(bool toProcess) {
				mToProcessPossibleSameIndividualsFlag = toProcess;
				return this;
			}

			bool COptimizedKPSetIndividualItem::hasPossibleSameIndividualsProcessingQueuedFlag() {
				return mPossibleSameIndividualsProcessingQueuedFlag;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setPossibleSameIndividualsProcessingQueuedFlag(bool processingQueued) {
				mPossibleSameIndividualsProcessingQueuedFlag = processingQueued;
				return this;
			}

			bool COptimizedKPSetIndividualItem::isItemSameIndividualMerged() {
				return mItemSameIndividualMerged;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setItemSameIndividualMerged(bool merged) {
				mItemSameIndividualMerged = merged;
				return this;
			}


			cint64 COptimizedKPSetIndividualItem::getInitializingRoleCandidateCount() {
				return mInitializingRoleCandidateCount;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setInitializingRoleCandidateCount(cint64 testCount) {
				mInitializingRoleCandidateCount = testCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::incInitializingRoleCandidateCount(cint64 incCount) {
				mInitializingRoleCandidateCount += incCount;
				return this;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::decInitializingRoleCandidateCount(cint64 decCount) {
				mInitializingRoleCandidateCount -= decCount;
				return this;
			}

			bool COptimizedKPSetIndividualItem::hasInitializingRoleCandidatesQueuedFlag() {
				return mInitializingRoleCandidatesQueuedFlag;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setInitializingRoleCandidatesQueuedFlag(bool initializingQueued) {
				mInitializingRoleCandidatesQueuedFlag = initializingQueued;
				return this;
			}


			bool COptimizedKPSetIndividualItem::hasAllRoleCandidatesInitializedFlag() {
				return mAllRoleCandidatesInitializedFlag;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setAllRoleCandidatesInitializedFlag(bool allInitalized) {
				mAllRoleCandidatesInitializedFlag = allInitalized;
				return this;
			}



			CConcept* COptimizedKPSetIndividualItem::getTemporaryIndividualNominalConcept() {
				return mTmpNominalConcept;
			}

			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setTemporaryIndividualNominalConcept(CConcept* concept) {
				mTmpNominalConcept = concept;
				return this;
			}



			CIndividualDependenceTrackingCollector* COptimizedKPSetIndividualItem::getIndividualDependenceTrackingCollector() {
				return mIndiDepTrackingCollector;
			}


			COptimizedKPSetIndividualItem* COptimizedKPSetIndividualItem::setIndividualDependenceTrackingCollector(CIndividualDependenceTrackingCollector* indiDepTrackColl) {
				mIndiDepTrackingCollector = indiDepTrackColl;
				return this;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
