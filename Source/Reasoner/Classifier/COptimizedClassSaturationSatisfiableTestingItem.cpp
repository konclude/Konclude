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

#include "COptimizedClassSaturationSatisfiableTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedClassSaturationSatisfiableTestingItem::COptimizedClassSaturationSatisfiableTestingItem() {
			}


			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::initSatisfiableTestingItem(CConcept* satTestConcept) {
				initConceptSatisfiableTestingItem(satTestConcept);
				mConSatHierNode = nullptr;
				mPredCounter = 0;
				mSatTestedResult = false;
				mSatTestOrdered = false;
				mTestedSat = false;
				mEquiItem = false;
				mPredOfItem = false;
				return this;
			}


			bool COptimizedClassSaturationSatisfiableTestingItem::isClassSatisfiableTestingItem() {
				return true;
			}


			CHierarchyNode* COptimizedClassSaturationSatisfiableTestingItem::getSatisfiableConceptHierarchyNode() {
				return mConSatHierNode;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode) {
				mConSatHierNode = hierNode;
				return this;
			}


			QSet<COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationSatisfiableTestingItem::getSubsumingConceptItemSet() {
				return &mSubsumingConceptItemSet;
			}

			QList<COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationSatisfiableTestingItem::getSubsumingConceptItemList() {
				return &mSubsumingConceptItemList;
			}


			cint64 COptimizedClassSaturationSatisfiableTestingItem::getSubsumingConceptItemCount() const {
				return mSubsumingConceptItemSet.count();
			}


			QList<COptimizedConceptSaturationSatisfiableTestingItem*>* COptimizedClassSaturationSatisfiableTestingItem::getSuccessorItemList() {
				return &mSuccList;
			}

			cint64 COptimizedClassSaturationSatisfiableTestingItem::getUnprocessedPredecessorItemCount() {
				return mPredCounter;
			}

			bool COptimizedClassSaturationSatisfiableTestingItem::hasOnlyProcessedPredecessorItems() {
				return mPredCounter <= 0;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::decUnprocessedPredecessorItems(cint64 decCount) {
				mPredCounter -= decCount;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::incUnprocessedPredecessorItems(cint64 incCount) {
				mPredCounter += incCount;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::addSuccessorSatisfiableTestItem(COptimizedConceptSaturationSatisfiableTestingItem* succItem) {
				mSuccList.append(succItem);
				return this;
			}


			bool COptimizedClassSaturationSatisfiableTestingItem::isSatisfiableTestOrdered() {
				return mSatTestOrdered;
			}

			bool COptimizedClassSaturationSatisfiableTestingItem::isSatisfiableTested() {
				return mTestedSat;
			}

			bool COptimizedClassSaturationSatisfiableTestingItem::getSatisfiableTestedResult() {
				return mSatTestedResult;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setSatisfiableTestOrdered(bool satTestOrdered) {
				mSatTestOrdered = satTestOrdered;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setSatisfiableTested(bool satTested) {
				mTestedSat = satTested;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setSatisfiableTestedResult(bool satTestedResult) {
				mSatTestedResult = satTestedResult;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::addSubsumingConceptItem(COptimizedClassSaturationSatisfiableTestingItem* subsumingItem) {
				mSubsumingConceptItemList.append(subsumingItem);
				mSubsumingConceptItemSet.insert(subsumingItem);

				//QString iriClassNameString = CIRIName::getRecentIRIName(subsumingItem->getSatisfiableConcept()->getClassNameLinker());
				//mSubsumingConceptString += QString("%1\r\n").arg(iriClassNameString);

				return this;
			}


			CConceptSubsumerObserver* COptimizedClassSaturationSatisfiableTestingItem::tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept) {
				CConceptProcessData* conProData = (CConceptProcessData*)subsumerConcept->getConceptData();
				CConceptSatisfiableReferenceLinkingData* conSatRefLinking = (CConceptSatisfiableReferenceLinkingData*)conProData->getConceptReferenceLinking();
				if (conSatRefLinking) {
					COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSaturationItem = (COptimizedClassSaturationSatisfiableTestingItem*)conSatRefLinking->getClassifierReferenceLinkingData();
					addSubsumingConceptItem(subsumerClassSaturationItem);
				}
				return this;
			}



			bool itemSortLessThan(const COptimizedClassSaturationSatisfiableTestingItem* item1, const COptimizedClassSaturationSatisfiableTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() > item2->getSubsumingConceptItemCount();
			}

			QList<COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationSatisfiableTestingItem::sortSubsumingConceptItemList() {
				qSort(mSubsumingConceptItemList.begin(),mSubsumingConceptItemList.end(),itemSortLessThan);
				return &mSubsumingConceptItemList;
			}

			bool COptimizedClassSaturationSatisfiableTestingItem::isEquivalentItem() {
				return mEquiItem;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setEquivalentItem(bool equivalent) {
				mEquiItem = equivalent;
				return this;
			}


			bool COptimizedClassSaturationSatisfiableTestingItem::isPredecessorItem() {
				return mPredOfItem;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationSatisfiableTestingItem::setPredecessorItem(bool isPredecessorOfOneItem) {
				mPredOfItem = isPredecessorOfOneItem;
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
