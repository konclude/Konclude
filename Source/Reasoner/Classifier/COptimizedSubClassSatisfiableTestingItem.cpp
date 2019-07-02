/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COptimizedSubClassSatisfiableTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedSubClassSatisfiableTestingItem::COptimizedSubClassSatisfiableTestingItem() {
			}


			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::initSatisfiableTestingItem(CConcept* satTestConcept) {
				mConceptSat = satTestConcept;
				mConSatHierNode = nullptr;
				mPredCounter = 0;
				mSatTestedResult = false;
				mSatTestOrdered = false;
				mTestedSat = false;
				mUnsatDerivated = false;
				mSatDerivated = false;
				mEquiItem = false;
				mPredOfItem = false;
				return this;
			}

			CConcept* COptimizedSubClassSatisfiableTestingItem::getSatisfiableConcept() {
				return mConceptSat;
			}

			CHierarchyNode* COptimizedSubClassSatisfiableTestingItem::getSatisfiableConceptHierarchyNode() {
				return mConSatHierNode;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode) {
				mConSatHierNode = hierNode;
				return this;
			}


			QSet<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassSatisfiableTestingItem::getSubsumingConceptItemSet() {
				return &mSubsumingConceptItemSet;
			}

			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassSatisfiableTestingItem::getSubsumingConceptItemList() {
				return &mSubsumingConceptItemList;
			}


			cint64 COptimizedSubClassSatisfiableTestingItem::getSubsumingConceptItemCount() const {
				return mSubsumingConceptItemSet.count();
			}


			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassSatisfiableTestingItem::getSuccessorItemList() {
				return &mSuccList;
			}

			cint64 COptimizedSubClassSatisfiableTestingItem::getUnprocessedPredecessorItemCount() {
				return mPredCounter;
			}

			bool COptimizedSubClassSatisfiableTestingItem::hasOnlyProcessedPredecessorItems() {
				return mPredCounter <= 0;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::decUnprocessedPredecessorItems(cint64 decCount) {
				mPredCounter -= decCount;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::incUnprocessedPredecessorItems(cint64 incCount) {
				mPredCounter += incCount;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setUnprocessedPredecessorItems(cint64 count) {
				mPredCounter = count;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::addSuccessorSatisfiableTestItem(COptimizedSubClassSatisfiableTestingItem* succItem) {
				mSuccList.append(succItem);
				return this;
			}


			bool COptimizedSubClassSatisfiableTestingItem::isSatisfiableTestOrdered() {
				return mSatTestOrdered;
			}

			bool COptimizedSubClassSatisfiableTestingItem::isSatisfiableTested() {
				return mTestedSat;
			}

			bool COptimizedSubClassSatisfiableTestingItem::isResultUnsatisfiableDerivated() {
				return mUnsatDerivated;
			}

			bool COptimizedSubClassSatisfiableTestingItem::isResultSatisfiableDerivated() {
				return mSatDerivated;
			}

			bool COptimizedSubClassSatisfiableTestingItem::getSatisfiableTestedResult() {
				return mSatTestedResult;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setSatisfiableTestOrdered(bool satTestOrdered) {
				mSatTestOrdered = satTestOrdered;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setSatisfiableTested(bool satTested) {
				mTestedSat = satTested;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setSatisfiableTestedResult(bool satTestedResult) {
				mSatTestedResult = satTestedResult;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setResultUnsatisfiableDerivated(bool unsatDerivated) {
				mUnsatDerivated = unsatDerivated;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setResultSatisfiableDerivated(bool satDerivated) {
				mSatDerivated = satDerivated;
				return this;
			}


			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::addSubsumingConceptItem(COptimizedSubClassSatisfiableTestingItem* subsumingItem) {
				if (!mSubsumingConceptItemSet.contains(subsumingItem)) {
					mSubsumingConceptItemList.append(subsumingItem);
					mSubsumingConceptItemSet.insert(subsumingItem);

					//QString iriClassNameString = CIRIName::getRecentIRIName(subsumingItem->getSatisfiableConcept()->getClassNameLinker());
					//mSubsumingConceptString += QString("%1\r\n").arg(iriClassNameString);
				}
				return this;
			}



			CConceptSubsumerObserver* COptimizedSubClassSatisfiableTestingItem::tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept) {
				if (subsumerConcept->getOperatorCode() != CCTOP) {
					CConceptProcessData* conProData = (CConceptProcessData*)subsumerConcept->getConceptData();
					CConceptSatisfiableReferenceLinkingData* conSatRefLinking = (CConceptSatisfiableReferenceLinkingData*)conProData->getConceptReferenceLinking();
					if (conSatRefLinking) {
						COptimizedSubClassSatisfiableTestingItem* subsumerClassSaturationItem = (COptimizedSubClassSatisfiableTestingItem*)conSatRefLinking->getClassifierReferenceLinkingData();
						addSubsumingConceptItem(subsumerClassSaturationItem);
					}
				}
				return this;
			}


			bool itemSortLessThan(const COptimizedSubClassSatisfiableTestingItem* item1, const COptimizedSubClassSatisfiableTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() > item2->getSubsumingConceptItemCount();
			}

			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassSatisfiableTestingItem::sortSubsumingConceptItemList() {
				qSort(mSubsumingConceptItemList.begin(),mSubsumingConceptItemList.end(),itemSortLessThan);
				return &mSubsumingConceptItemList;
			}

			bool COptimizedSubClassSatisfiableTestingItem::isEquivalentItem() {
				return mEquiItem;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setEquivalentItem(bool equivalent) {
				mEquiItem = equivalent;
				return this;
			}


			bool COptimizedSubClassSatisfiableTestingItem::isPredecessorItem() {
				return mPredOfItem;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassSatisfiableTestingItem::setPredecessorItem(bool isPredecessorOfOneItem) {
				mPredOfItem = isPredecessorOfOneItem;
				return this;
			}

			bool COptimizedSubClassSatisfiableTestingItem::isMoreConceptClassificationInformationRequired() {
				if (mSatTestOrdered || mTestedSat || mUnsatDerivated || mSatDerivated) {
					return false;
				}
				return true;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
