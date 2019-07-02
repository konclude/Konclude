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

#include "COptimizedClassSaturationOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedClassSaturationOntologyClassificationItem::COptimizedClassSaturationOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COntologyClassificationItem(configuration,nextClassificationStatistics) {
				
				mTopSatTestItem = nullptr;
				mBottomSatTestItem = nullptr;

				mSaturationCalculated = false;
				mRemainingSaturationCalculated = true;
				mSaturationCalculationJob = nullptr;
			}

			QHash<CConcept*,COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationOntologyClassificationItem::getClassSatisfiableTestItemHash() {
				return &mClassSatItemHash;
			}
			QList<COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationOntologyClassificationItem::getClassSatisfiableTestItemList() {
				return &mClassSatTestItemContainer;
			}

			QHash<CConcept*,COptimizedConceptSaturationSatisfiableTestingItem*>* COptimizedClassSaturationOntologyClassificationItem::getConceptSatisfiableTestItemHash() {
				return &mConceptSatItemHash;
			}
			QList<COptimizedConceptSaturationSatisfiableTestingItem*>* COptimizedClassSaturationOntologyClassificationItem::getConceptSatisfiableTestItemList() {
				return &mConceptSatTestItemContainer;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationOntologyClassificationItem::getTopConceptSatisfiableTestItem() {
				return mTopSatTestItem;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationOntologyClassificationItem::getBottomConceptSatisfiableTestItem() {
				return mBottomSatTestItem;
			}


			COptimizedClassSaturationOntologyClassificationItem* COptimizedClassSaturationOntologyClassificationItem::initTopBottomSatisfiableTestingItems(COptimizedClassSaturationSatisfiableTestingItem* topItem, COptimizedClassSaturationSatisfiableTestingItem* bottomItem) {
				mTopSatTestItem = topItem;
				mBottomSatTestItem = bottomItem;
				return this;
			}

			COptimizedClassSaturationSatisfiableTestingItem* COptimizedClassSaturationOntologyClassificationItem::getClassSatisfiableTestItem(CConcept* satTestConcept, bool create) {
				COptimizedClassSaturationSatisfiableTestingItem* item = nullptr;
				item = mClassSatItemHash.value(satTestConcept);
				if (!item && create) {
					item = new COptimizedClassSaturationSatisfiableTestingItem();
					item->initSatisfiableTestingItem(satTestConcept);
					mClassSatItemHash.insert(satTestConcept,item);
					mConceptSatItemHash.insert(satTestConcept,item);
					mClassSatTestItemContainer.append(item);
				}
				return item;
			}

			COptimizedConceptSaturationSatisfiableTestingItem* COptimizedClassSaturationOntologyClassificationItem::getConceptSatisfiableTestItem(CConcept* satTestConcept, bool create) {
				COptimizedConceptSaturationSatisfiableTestingItem* item = nullptr;
				item = mConceptSatItemHash.value(satTestConcept);
				if (!item && create) {
					item = new COptimizedConceptSaturationSatisfiableTestingItem();
					item->initConceptSatisfiableTestingItem(satTestConcept);
					mConceptSatItemHash.insert(satTestConcept,item);
					mConceptSatTestItemContainer.append(item);
				}
				return item;
			}


			bool COptimizedClassSaturationOntologyClassificationItem::hasSaturationCalculated() {
				return mSaturationCalculated;
			}

			bool COptimizedClassSaturationOntologyClassificationItem::hasRemainingSaturationCalculation() {
				return mRemainingSaturationCalculated;
			}

			COptimizedClassSaturationOntologyClassificationItem* COptimizedClassSaturationOntologyClassificationItem::setSaturationCalculated(bool calculated) {
				mSaturationCalculated = calculated;
				return this;
			}

			COptimizedClassSaturationOntologyClassificationItem* COptimizedClassSaturationOntologyClassificationItem::setRemainingSaturationCalculation(bool calculationRemaining) {
				mRemainingSaturationCalculated = calculationRemaining;
				return this;
			}

			CSaturationCalculationJob* COptimizedClassSaturationOntologyClassificationItem::getSaturationCalculationJob() {
				return mSaturationCalculationJob;
			}

			COptimizedClassSaturationOntologyClassificationItem* COptimizedClassSaturationOntologyClassificationItem::setSaturationCalculationJob(CSaturationCalculationJob* calculationJob) {
				mSaturationCalculationJob = calculationJob;
				return this;
			}


			QList<COptimizedClassSaturationSatisfiableTestingItem*>* COptimizedClassSaturationOntologyClassificationItem::getSatisfiableConceptItemList() {
				return &mSatisfiableItemList;
			}

			COptimizedClassSaturationOntologyClassificationItem* COptimizedClassSaturationOntologyClassificationItem::addSatisfiableConceptItem(COptimizedClassSaturationSatisfiableTestingItem* item) {
				mSatisfiableItemList.append(item);
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
