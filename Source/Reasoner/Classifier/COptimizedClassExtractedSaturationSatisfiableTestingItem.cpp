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

#include "COptimizedClassExtractedSaturationSatisfiableTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedClassExtractedSaturationSatisfiableTestingItem::COptimizedClassExtractedSaturationSatisfiableTestingItem() {
			}


			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::initSatisfiableTestingItem(CConcept* satTestConcept) {
				mTestConcept = satTestConcept;
				mExtractedSubsumerCount = 0;
				mConSatHierNode = nullptr;
				mDirectSubsumerItem = nullptr;
				mPredecessorItem = false;
				mEquiItem = false;
				return this;
			}


			CConcept* COptimizedClassExtractedSaturationSatisfiableTestingItem::getTestingConcept() {
				return mTestConcept;
			}


			CHierarchyNode* COptimizedClassExtractedSaturationSatisfiableTestingItem::getSatisfiableConceptHierarchyNode() {
				return mConSatHierNode;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode) {
				mConSatHierNode = hierNode;
				return this;
			}


			QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* COptimizedClassExtractedSaturationSatisfiableTestingItem::getSubsumingConceptItemList() {
				return &mSubsumingConceptItemList;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::setDirectSubsumer(COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumingItem) {
				mDirectSubsumerItem = subsumingItem;
				return this;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::getDirectSubsumer() {
				return mDirectSubsumerItem;
			}


			bool COptimizedClassExtractedSaturationSatisfiableTestingItem::hasDirectSubsumer() {
				return mDirectSubsumerItem != nullptr;
			}


			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::setExtractionSubsumerCount(cint64 subsumerCount) {
				mExtractedSubsumerCount = subsumerCount;
				return this;
			}

			cint64 COptimizedClassExtractedSaturationSatisfiableTestingItem::getExtractionSubsumerCount() const {
				return mExtractedSubsumerCount;
			}



			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::addSubsumingConceptItem(COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumingItem) {
				mSubsumingConceptItemList.append(subsumingItem);

				//QString iriClassNameString = CIRIName::getRecentIRIName(subsumingItem->getSatisfiableConcept()->getClassNameLinker());
				//mSubsumingConceptString += QString("%1\r\n").arg(iriClassNameString);

				return this;
			}


			CConceptSubsumerObserver* COptimizedClassExtractedSaturationSatisfiableTestingItem::tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept) {
				CConceptProcessData* conProData = (CConceptProcessData*)subsumerConcept->getConceptData();
				CConceptSatisfiableReferenceLinkingData* conSatRefLinking = (CConceptSatisfiableReferenceLinkingData*)conProData->getConceptReferenceLinking();
				if (conSatRefLinking) {
					COptimizedClassExtractedSaturationSatisfiableTestingItem* subsumerClassSaturationItem = (COptimizedClassExtractedSaturationSatisfiableTestingItem*)conSatRefLinking->getClassifierReferenceLinkingData();
					addSubsumingConceptItem(subsumerClassSaturationItem);
				}
				return this;
			}



			bool itemSortGreaterThan(const COptimizedClassExtractedSaturationSatisfiableTestingItem* item1, const COptimizedClassExtractedSaturationSatisfiableTestingItem* item2) {
				return item1->getExtractionSubsumerCount() > item2->getExtractionSubsumerCount();
			}

			QList<COptimizedClassExtractedSaturationSatisfiableTestingItem*>* COptimizedClassExtractedSaturationSatisfiableTestingItem::sortSubsumingConceptItemList() {
				qSort(mSubsumingConceptItemList.begin(),mSubsumingConceptItemList.end(),itemSortGreaterThan);
				return &mSubsumingConceptItemList;
			}

			bool COptimizedClassExtractedSaturationSatisfiableTestingItem::isEquivalentItem() {
				return mEquiItem;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::setEquivalentItem(bool equivalent) {
				mEquiItem = equivalent;
				return this;
			}


			bool COptimizedClassExtractedSaturationSatisfiableTestingItem::isPredecessorItem() {
				return mPredecessorItem;
			}

			COptimizedClassExtractedSaturationSatisfiableTestingItem* COptimizedClassExtractedSaturationSatisfiableTestingItem::setPredecessorItem(bool predecessorItem) {
				mPredecessorItem = predecessorItem;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
