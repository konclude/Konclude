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

#include "COptimizedKPSetRoleTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRoleTestingItem::COptimizedKPSetRoleTestingItem() {
			}


			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::initSatisfiableTestingItem(CRole* role) {
				mRole = role;
				mConSatHierNode = nullptr;
				mPredCounter = 0;
				mSatTestedResult = false;
				mSatTestOrdered = false;
				mTestedSat = false;
				mUnsatDerivated = false;
				mSatDerivated = false;
				mEquiItem = false;
				mPredOfItem = false;

				mPossibleSubsumptionMap = nullptr;
				mPossSubsumMapInitialized = false;
				mPropagationConnected = false;

				mPossibleSubsumedList = nullptr;
				mPossibleSubsumedSet = nullptr;

				mTmpMarkerConcept = nullptr;
				mTmpPropagationConcept = nullptr;
				mTmpExistConcept = nullptr;

				return this;
			}

			COptimizedKPSetRoleTestingItem::~COptimizedKPSetRoleTestingItem() {
				delete mPossibleSubsumedList;
				delete mPossibleSubsumedSet;
			}



			CConcept* COptimizedKPSetRoleTestingItem::getTemporaryMarkerConcept() {
				return mTmpMarkerConcept;
			}

			CConcept* COptimizedKPSetRoleTestingItem::getTemporaryPropagationConcept() {
				return mTmpPropagationConcept;
			}

			CConcept* COptimizedKPSetRoleTestingItem::getTemporaryExistConcept() {
				return mTmpExistConcept;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setTemporaryMarkerConcept(CConcept* markerConcept) {
				mTmpMarkerConcept = markerConcept;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setTemporaryPropagationConcept(CConcept* propConcept) {
				mTmpPropagationConcept = propConcept;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setTemporaryExistConcept(CConcept* existConcept) {
				mTmpExistConcept = existConcept;
				return this;
			}



			CRole* COptimizedKPSetRoleTestingItem::getTestingRole() {
				return mRole;
			}


			CRolePropertiesHierarchyNode* COptimizedKPSetRoleTestingItem::getSatisfiableRoleHierarchyNode() {
				return mConSatHierNode;
			}


			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setSatisfiableRoleHierarchyNode(CRolePropertiesHierarchyNode* hierNode) {
				mConSatHierNode = hierNode;
				return this;
			}


			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getSubsumerRoleItemSet() {
				return &mSubsumingConceptItemSet;
			}

			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getSubsumerRoleItemList() {
				return &mSubsumingConceptItemList;
			}


			cint64 COptimizedKPSetRoleTestingItem::getSubsumerRoleItemCount() const {
				return mSubsumingConceptItemSet.count();
			}

			bool COptimizedKPSetRoleTestingItem::hasSubsumerRoleItem(COptimizedKPSetRoleTestingItem* item) {
				return mSubsumingConceptItemSet.contains(item);
			}


			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getSuccessorItemList() {
				return &mSuccList;
			}

			cint64 COptimizedKPSetRoleTestingItem::getUnprocessedPredecessorItemCount() {
				return mPredCounter;
			}

			bool COptimizedKPSetRoleTestingItem::hasOnlyProcessedPredecessorItems() {
				return mPredCounter <= 0;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::decUnprocessedPredecessorItems(cint64 decCount) {
				mPredCounter -= decCount;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::incUnprocessedPredecessorItems(cint64 incCount) {
				mPredCounter += incCount;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setUnprocessedPredecessorItems(cint64 predCount) {
				mPredCounter = predCount;
				return this;
			}


			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::addSuccessorSatisfiableTestItem(COptimizedKPSetRoleTestingItem* succItem) {
				mSuccList.append(succItem);
				return this;
			}


			bool COptimizedKPSetRoleTestingItem::isSatisfiableTestOrdered() {
				return mSatTestOrdered;
			}

			bool COptimizedKPSetRoleTestingItem::isSatisfiableTested() {
				return mTestedSat;
			}

			bool COptimizedKPSetRoleTestingItem::isResultUnsatisfiableDerivated() {
				return mUnsatDerivated;
			}

			bool COptimizedKPSetRoleTestingItem::isResultSatisfiableDerivated() {
				return mSatDerivated;
			}

			bool COptimizedKPSetRoleTestingItem::getSatisfiableTestedResult() {
				return mSatTestedResult;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setSatisfiableTestOrdered(bool satTestOrdered) {
				mSatTestOrdered = satTestOrdered;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setSatisfiableTested(bool satTested) {
				mTestedSat = satTested;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setSatisfiableTestedResult(bool satTestedResult) {
				mSatTestedResult = satTestedResult;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setResultUnsatisfiableDerivated(bool unsatDerivated) {
				mUnsatDerivated = unsatDerivated;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setResultSatisfiableDerivated(bool satDerivated) {
				mSatDerivated = satDerivated;
				return this;
			}


			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::addSubsumerRoleItem(COptimizedKPSetRoleTestingItem* subsumingItem) {
				if (!mSubsumingConceptItemSet.contains(subsumingItem)) {
					mSubsumingConceptItemList.append(subsumingItem);
					mSubsumingConceptItemSet.insert(subsumingItem);

					//QString iriClassNameString = CIRIName::getRecentIRIName(subsumingItem->getTestingConcept()->getClassNameLinker());
					//mSubsumingConceptString += QString("%1\r\n").arg(iriClassNameString);
				}
				return this;
			}



			CConceptSubsumerObserver* COptimizedKPSetRoleTestingItem::tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept) {
				if (subsumerConcept->getOperatorCode() != CCTOP) {
					CConceptProcessData* conProData = (CConceptProcessData*)subsumerConcept->getConceptData();
					CConceptSatisfiableReferenceLinkingData* conSatRefLinking = (CConceptSatisfiableReferenceLinkingData*)conProData->getConceptReferenceLinking();
					if (conSatRefLinking) {
						COptimizedKPSetRoleTestingItem* subsumerClassSaturationItem = (COptimizedKPSetRoleTestingItem*)conSatRefLinking->getClassifierReferenceLinkingData();
						addSubsumerRoleItem(subsumerClassSaturationItem);
					}
				}
				return this;
			}


			bool itemSortLessThan(const COptimizedKPSetRoleTestingItem* item1, const COptimizedKPSetRoleTestingItem* item2) {
				return item1->getSubsumerRoleItemCount() > item2->getSubsumerRoleItemCount();
			}

			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::sortSubsumingConceptItemList() {
				qSort(mSubsumingConceptItemList.begin(),mSubsumingConceptItemList.end(),itemSortLessThan);
				return &mSubsumingConceptItemList;
			}

			bool COptimizedKPSetRoleTestingItem::isEquivalentItem() {
				return mEquiItem;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setEquivalentItem(bool equivalent) {
				mEquiItem = equivalent;
				return this;
			}


			bool COptimizedKPSetRoleTestingItem::isPredecessorItem() {
				return mPredOfItem;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setPredecessorItem(bool isPredecessorOfOneItem) {
				mPredOfItem = isPredecessorOfOneItem;
				return this;
			}

			bool COptimizedKPSetRoleTestingItem::isMoreConceptClassificationInformationRequired() {
				if (mSatTestOrdered || mTestedSat || mUnsatDerivated || mSatDerivated) {
					return false;
				}
				return true;
			}

			COptimizedKPSetRolePossibleSubsumptionMap* COptimizedKPSetRoleTestingItem::getPossibleSubsumptionMap(bool create) {
				if (!mPossibleSubsumptionMap && create) {
					mPossibleSubsumptionMap = new COptimizedKPSetRolePossibleSubsumptionMap();
				}
				return mPossibleSubsumptionMap;
			}


			CPropertyPossibleSubsumptionMap* COptimizedKPSetRoleTestingItem::getPropertyPossibleSubsumptionMap() {
				return getPossibleSubsumptionMap(false);
			}


			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getUpPropagationItemSet() {
				return &mUpPropagationItemSet;
			}

			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getDownPropagationItemSet() {
				return &mDownPropagationItemSet;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::addUpPropagationItem(COptimizedKPSetRoleTestingItem* item) {
				mUpPropagationItemSet.insert(item);
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::addDownPropagationItem(COptimizedKPSetRoleTestingItem* item) {
				mDownPropagationItemSet.insert(item);
				return this;
			}


			bool COptimizedKPSetRoleTestingItem::isPossibleSubsumptionMapInitialized() {
				return mPossSubsumMapInitialized;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setPossibleSubsumptionMapInitialized(bool initialized) {
				mPossSubsumMapInitialized = initialized;
				return this;
			}

			bool COptimizedKPSetRoleTestingItem::isPropagationConnected() {
				return mPropagationConnected;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setPropagationConnected(bool connected) {
				mPropagationConnected = connected;
				return this;
			}


			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getPossibleSubsumerSet(bool create) {
				if (create && !mPossibleSubsumedSet) {
					mPossibleSubsumedSet = new QSet<COptimizedKPSetRoleTestingItem*>();
				}
				return mPossibleSubsumedSet;
			}

			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleTestingItem::getPossibleSubsumerList() {
				return mPossibleSubsumedList;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleTestingItem::setPossibleSubsumedList(QList<COptimizedKPSetRoleTestingItem*>* possSubsumedList) {
				mPossibleSubsumedList = possSubsumedList;
				return this;
			}

			bool COptimizedKPSetRoleTestingItem::hasRemainingPossibleSubsumedItems() {
				return mPossibleSubsumedList && !mPossibleSubsumedList->isEmpty();
			}





		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
