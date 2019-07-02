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

#include "COptimizedSubClassOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedSubClassOntologyClassificationItem::COptimizedSubClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COntologyClassificationItem(configuration,nextClassificationStatistics) {
				
				mTopSatTestItem = nullptr;
				mBottomSatTestItem = nullptr;

				mRemainingSatTests = 0;
				mRunningSatTests = 0;
			}

			COptimizedSubClassOntologyClassificationItem::~COptimizedSubClassOntologyClassificationItem() {
			}

			QHash<CConcept*,COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getConceptSatisfiableTestItemHash() {
				return &mConceptSatItemHash;
			}
			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getConceptSatisfiableTestItemContainer() {
				return &mSatTestItemContainer;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassOntologyClassificationItem::getTopConceptSatisfiableTestItem() {
				return mTopSatTestItem;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassOntologyClassificationItem::getBottomConceptSatisfiableTestItem() {
				return mBottomSatTestItem;
			}

			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getNextSatisfiableTestingItemList() {
				return &mNextItemList;
			}


			QSet<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getNextCandidateSatisfiableTestingItemSet() {
				return &mNextCandItemSet;
			}

			QSet<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getRemainingCandidateSatisfiableTestingItemSet() {
				return &mRemainingCandItemSet;
			}


			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::initTopBottomSatisfiableTestingItems(COptimizedSubClassSatisfiableTestingItem* topItem, COptimizedSubClassSatisfiableTestingItem* bottomItem) {
				mTopSatTestItem = topItem;
				mBottomSatTestItem = bottomItem;
				return this;
			}

			COptimizedSubClassSatisfiableTestingItem* COptimizedSubClassOntologyClassificationItem::getConceptSatisfiableTestItem(CConcept* satTestConcept, bool create) {
				COptimizedSubClassSatisfiableTestingItem* item = nullptr;
				item = mConceptSatItemHash.value(satTestConcept);
				if (!item && create) {
					item = new COptimizedSubClassSatisfiableTestingItem();
					item->initSatisfiableTestingItem(satTestConcept);
					mConceptSatItemHash.insert(satTestConcept,item);
					mSatTestItemContainer.append(item);
				}
				return item;
			}


			bool COptimizedSubClassOntologyClassificationItem::hasAllSatisfiableTestsCompleted() {
				return mRemainingSatTests <= 0 && mRunningSatTests <= 0;
			}

			bool COptimizedSubClassOntologyClassificationItem::hasRemainingSatisfiableTests() {
				return mRemainingSatTests > 0;
			}

			cint64 COptimizedSubClassOntologyClassificationItem::getRemainingSatisfiableTestsCount() {
				return mRemainingSatTests;
			}

			cint64 COptimizedSubClassOntologyClassificationItem::getRunningSatisfiableTestsCount() {
				return mRunningSatTests;
			}

			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::incRemainingSatisfiableTestsCount(cint64 incCount) {
				mRemainingSatTests += incCount;
				return this;
			}

			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::incRunningSatisfiableTestsCount(cint64 incCount) {
				mRunningSatTests += incCount;
				return this;
			}


			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::decRemainingSatisfiableTestsCount(cint64 decCount) {
				mRemainingSatTests -= decCount;
				return this;
			}

			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::decRunningSatisfiableTestsCount(cint64 decCount) {
				mRunningSatTests -= decCount;
				return this;
			}


			QList<COptimizedSubClassSatisfiableTestingItem*>* COptimizedSubClassOntologyClassificationItem::getSatisfiableConceptItemList() {
				return &mSatisfiableItemList;
			}

			COptimizedSubClassOntologyClassificationItem* COptimizedSubClassOntologyClassificationItem::addSatisfiableConceptItem(COptimizedSubClassSatisfiableTestingItem* item) {
				mSatisfiableItemList.append(item);
				return this;
			}


			QHash<CConcept*,CConceptReferenceLinking*>* COptimizedSubClassOntologyClassificationItem::getConceptReferenceLinkingDataHash() {
				return &mConceptRefLinkData;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
