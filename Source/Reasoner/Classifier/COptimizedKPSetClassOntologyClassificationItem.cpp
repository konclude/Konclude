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

#include "COptimizedKPSetClassOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetClassOntologyClassificationItem::COptimizedKPSetClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COntologyClassificationItem(configuration,nextClassificationStatistics) {
				
				mTopSatTestItem = nullptr;
				mBottomSatTestItem = nullptr;

				mRemainingSatTests = 0;
				mRunningSatTests = 0;

				mSatisfiableTestingPhaseFinished = false;
				mPossibleSubsumptionTestingPhaseFinished = false;

				mRemainingPossibleSubsumptionTests = 0;
				mRunningPossibleSubsumptionTests = 0;

				mCalculatedPossibleSubsumCount = 0;
				mCalculatedTruePossibleSubsumCount = 0;
				mCalculatedFalsePossibleSubsumCount = 0;

				mPossibleSubsumCount = 0;
				mTruePossibleSubsumCount = 0;
				mFalsePossibleSubsumCount = 0;

			}

			COptimizedKPSetClassOntologyClassificationItem::~COptimizedKPSetClassOntologyClassificationItem() {
			}



			QHash<CConcept*,COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getConceptSatisfiableTestItemHash() {
				return &mConceptSatItemHash;
			}
			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getConceptSatisfiableTestItemContainer() {
				return &mSatTestItemContainer;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassOntologyClassificationItem::getTopConceptSatisfiableTestItem() {
				return mTopSatTestItem;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassOntologyClassificationItem::getBottomConceptSatisfiableTestItem() {
				return mBottomSatTestItem;
			}

			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getNextSatisfiableTestingItemList() {
				return &mNextItemList;
			}

			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getNextCandidateSatisfiableTestingItemSet() {
				return &mNextCandItemSet;
			}

			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getRemainingCandidateSatisfiableTestingItemSet() {
				return &mRemainingCandItemSet;
			}

			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getNextPossibleSubsumptionTestingItemList() {
				return &mNextPossSubsumItemList;
			}

			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getCurrentPossibleSubsumptionTestingItemSet() {
				return &mCurrentPossSubsumItemSet;
			}



			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::initTopBottomSatisfiableTestingItems(COptimizedKPSetClassTestingItem* topItem, COptimizedKPSetClassTestingItem* bottomItem) {
				mTopSatTestItem = topItem;
				mBottomSatTestItem = bottomItem;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassOntologyClassificationItem::getConceptSatisfiableTestItem(CConcept* satTestConcept, bool create) {
				COptimizedKPSetClassTestingItem* item = nullptr;
				if (satTestConcept->getOperatorCode() == CCEQCAND) {
					satTestConcept = satTestConcept->getOperandList()->getData();
				}
				item = mConceptSatItemHash.value(satTestConcept);
				if (!item && create) {
					item = new COptimizedKPSetClassTestingItem();
					item->initSatisfiableTestingItem(satTestConcept,&mConceptRefLinkData);
					mConceptSatItemHash.insert(satTestConcept,item);
					mSatTestItemContainer.append(item);
				}
				return item;
			}


			bool COptimizedKPSetClassOntologyClassificationItem::hasAllSatisfiableTestsCompleted() {
				return mRemainingSatTests <= 0 && mRunningSatTests <= 0;
			}

			bool COptimizedKPSetClassOntologyClassificationItem::hasRemainingSatisfiableTests() {
				return mRemainingSatTests > 0;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getRemainingSatisfiableTestsCount() {
				return mRemainingSatTests;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getRunningSatisfiableTestsCount() {
				return mRunningSatTests;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incRemainingSatisfiableTestsCount(cint64 incCount) {
				mRemainingSatTests += incCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incRunningSatisfiableTestsCount(cint64 incCount) {
				mRunningSatTests += incCount;
				return this;
			}


			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::decRemainingSatisfiableTestsCount(cint64 decCount) {
				mRemainingSatTests -= decCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::decRunningSatisfiableTestsCount(cint64 decCount) {
				mRunningSatTests -= decCount;
				return this;
			}


			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getSatisfiableConceptItemList() {
				return &mSatisfiableItemList;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::addSatisfiableConceptItem(COptimizedKPSetClassTestingItem* item) {
				mSatisfiableItemList.append(item);
				return this;
			}


			QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* COptimizedKPSetClassOntologyClassificationItem::getConceptReferenceLinkingDataHash() {
				return &mConceptRefLinkData;
			}

			bool COptimizedKPSetClassOntologyClassificationItem::hasSatisfiableTestingPhaseFinished() {
				return mSatisfiableTestingPhaseFinished;
			}

			bool COptimizedKPSetClassOntologyClassificationItem::hasPossibleSubsumptionTestingPhaseFinished() {
				return mPossibleSubsumptionTestingPhaseFinished;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setSatisfiableTestingPhaseFinished(bool finished) {
				mSatisfiableTestingPhaseFinished = finished;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setPossibleSubsumptionTestingPhaseFinished(bool finished) {
				mPossibleSubsumptionTestingPhaseFinished = finished;
				return this;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getRemainingPossibleSubsumptionTestsCount() {
				return mRemainingPossibleSubsumptionTests;
			}


			cint64 COptimizedKPSetClassOntologyClassificationItem::hasRemainingPossibleSubsumptionTests() {
				return mRemainingPossibleSubsumptionTests > 0;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incRemainingPossibleSubsumptionTestsCount(cint64 incCount) {
				mRemainingPossibleSubsumptionTests += incCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::decRemainingPossibleSubsumptionTestsCount(cint64 decCount) {
				mRemainingPossibleSubsumptionTests -= decCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setRemainingPossibleSubsumptionTestsCount(cint64 testCount) {
				mRemainingPossibleSubsumptionTests = testCount;
				return this;
			}

			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassOntologyClassificationItem::getRemainingPossibleSubsumptionClassTestingSet() {
				return &mRemPossClassTestingSet;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getRemainingPossibleSubsumptionClassTestingCount() {
				return mRemPossClassTestingSet.count();
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incRunningPossibleSubsumptionTestsCount(cint64 incCount) {
				mRunningPossibleSubsumptionTests += incCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::decRunningPossibleSubsumptionTestsCount(cint64 decCount) {
				mRunningPossibleSubsumptionTests -= decCount;
				return this;
			} 

			QSet<CConcept*>* COptimizedKPSetClassOntologyClassificationItem::getEquivaltentConceptNonCandidateSet() {
				return &mEquivConNonCandidateSet;
			}

			CBOXHASH<CConcept*,CConcept*>* COptimizedKPSetClassOntologyClassificationItem::getEquivalentConceptCandidateHash() {
				return &mEquivConCandidateHash;
			}


			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::addMemoryPools(CMemoryPool* memoryPools) {
				mMemoryPoolContainer.appendMemoryPool(memoryPools);
				return this;
			}

			CMemoryPool* COptimizedKPSetClassOntologyClassificationItem::takeMemoryPools() {
				return mMemoryPoolContainer.takeMemoryPools();
			}


			cint64 COptimizedKPSetClassOntologyClassificationItem::getCalculatedPossibleSubsumerCount() {
				return mCalculatedPossibleSubsumCount;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getCalculatedTruePossibleSubsumerCount() {
				return mCalculatedTruePossibleSubsumCount;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getCalculatedFalsePossibleSubsumerCount() {
				return mCalculatedFalsePossibleSubsumCount;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setCalculatedPossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedPossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setCalculatedTruePossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedTruePossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setCalculatedFalsePossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedFalsePossibleSubsumCount = subsumCount;
				return this;
			}


			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incCalculatedPossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedPossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incCalculatedTruePossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedTruePossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incCalculatedFalsePossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedFalsePossibleSubsumCount += incSubsumCount;
				return this;
			}



			cint64 COptimizedKPSetClassOntologyClassificationItem::getPossibleSubsumerCount() {
				return mPossibleSubsumCount;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getTruePossibleSubsumerCount() {
				return mTruePossibleSubsumCount;
			}

			cint64 COptimizedKPSetClassOntologyClassificationItem::getFalsePossibleSubsumerCount() {
				return mFalsePossibleSubsumCount;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setPossibleSubsumerCount(cint64 subsumCount) {
				mPossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setTruePossibleSubsumerCount(cint64 subsumCount) {
				mTruePossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::setFalsePossibleSubsumerCount(cint64 subsumCount) {
				mFalsePossibleSubsumCount = subsumCount;
				return this;
			}


			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incPossibleSubsumerCount(cint64 incSubsumCount) {
				mPossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incTruePossibleSubsumerCount(cint64 incSubsumCount) {
				mTruePossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetClassOntologyClassificationItem* COptimizedKPSetClassOntologyClassificationItem::incFalsePossibleSubsumerCount(cint64 incSubsumCount) {
				mFalsePossibleSubsumCount += incSubsumCount;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
