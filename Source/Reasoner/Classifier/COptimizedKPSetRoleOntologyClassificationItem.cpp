/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "COptimizedKPSetRoleOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetRoleOntologyClassificationItem::COptimizedKPSetRoleOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COntologyPropertyRoleClassificationItem(configuration,nextClassificationStatistics) {
				
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

				mTmpRoleClassificationOnt = nullptr;

				mTmpAllPropConcept = nullptr;

				mTmpIndiPropagation = nullptr;
				mTmpIndiMarker = nullptr;

				mInitTime.start();
			}

			COptimizedKPSetRoleOntologyClassificationItem::~COptimizedKPSetRoleOntologyClassificationItem() {
			}



			QHash<CRole*,COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getRoleSatisfiableTestItemHash() {
				return &mRoleSatItemHash;
			}
			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getRoleSatisfiableTestItemList() {
				return &mSatTestItemContainer;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleOntologyClassificationItem::getTopRoleSatisfiableTestItem() {
				return mTopSatTestItem;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleOntologyClassificationItem::getBottomRoleSatisfiableTestItem() {
				return mBottomSatTestItem;
			}



			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTopRoleSatisfiableTestItem(COptimizedKPSetRoleTestingItem* item) {
				mTopSatTestItem = item;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setBottomRoleSatisfiableTestItem(COptimizedKPSetRoleTestingItem* item) {
				mBottomSatTestItem = item;
				return this;
			}


			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getNextSatisfiableTestingItemList() {
				return &mNextItemList;
			}

			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getNextCandidateSatisfiableTestingItemSet() {
				return &mNextCandItemSet;
			}

			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getRemainingCandidateSatisfiableTestingItemSet() {
				return &mRemainingCandItemSet;
			}

			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getNextPossibleSubsumptionTestingItemList() {
				return &mNextPossSubsumItemList;
			}

			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getCurrentPossibleSubsumptionTestingItemSet() {
				return &mCurrentPossSubsumItemSet;
			}



			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::initTopBottomSatisfiableTestingItems(COptimizedKPSetRoleTestingItem* topItem, COptimizedKPSetRoleTestingItem* bottomItem) {
				mTopSatTestItem = topItem;
				mBottomSatTestItem = bottomItem;
				return this;
			}

			COptimizedKPSetRoleTestingItem* COptimizedKPSetRoleOntologyClassificationItem::getRoleSatisfiableTestItem(CRole* satTestRole, bool create) {
				COptimizedKPSetRoleTestingItem* item = nullptr;
				item = mRoleSatItemHash.value(satTestRole);
				if (!item && create) {
					item = new COptimizedKPSetRoleTestingItem();
					item->initSatisfiableTestingItem(satTestRole);
					mRoleSatItemHash.insert(satTestRole,item);
					mSatTestItemContainer.append(item);
				}
				return item;
			}


			bool COptimizedKPSetRoleOntologyClassificationItem::hasAllSatisfiableTestsCompleted() {
				return mRemainingSatTests <= 0 && mRunningSatTests <= 0;
			}

			bool COptimizedKPSetRoleOntologyClassificationItem::hasRemainingSatisfiableTests() {
				return mRemainingSatTests > 0;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getRemainingSatisfiableTestsCount() {
				return mRemainingSatTests;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getRunningSatisfiableTestsCount() {
				return mRunningSatTests;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incRemainingSatisfiableTestsCount(cint64 incCount) {
				mRemainingSatTests += incCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incRunningSatisfiableTestsCount(cint64 incCount) {
				mRunningSatTests += incCount;
				return this;
			}


			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::decRemainingSatisfiableTestsCount(cint64 decCount) {
				mRemainingSatTests -= decCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::decRunningSatisfiableTestsCount(cint64 decCount) {
				mRunningSatTests -= decCount;
				return this;
			}


			QList<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getSatisfiableRoleItemList() {
				return &mSatisfiableItemList;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::addSatisfiableRoleItem(COptimizedKPSetRoleTestingItem* item) {
				mSatisfiableItemList.append(item);
				return this;
			}


			QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* COptimizedKPSetRoleOntologyClassificationItem::getConceptReferenceLinkingDataHash() {
				return &mConceptRefLinkData;
			}

			bool COptimizedKPSetRoleOntologyClassificationItem::hasSatisfiableTestingPhaseFinished() {
				return mSatisfiableTestingPhaseFinished;
			}

			bool COptimizedKPSetRoleOntologyClassificationItem::hasPossibleSubsumptionTestingPhaseFinished() {
				return mPossibleSubsumptionTestingPhaseFinished;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setSatisfiableTestingPhaseFinished(bool finished) {
				mSatisfiableTestingPhaseFinished = finished;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setPossibleSubsumptionTestingPhaseFinished(bool finished) {
				mPossibleSubsumptionTestingPhaseFinished = finished;
				return this;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getRemainingPossibleSubsumptionTestsCount() {
				return mRemainingPossibleSubsumptionTests;
			}


			cint64 COptimizedKPSetRoleOntologyClassificationItem::hasRemainingPossibleSubsumptionTests() {
				return mRemainingPossibleSubsumptionTests > 0;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incRemainingPossibleSubsumptionTestsCount(cint64 incCount) {
				mRemainingPossibleSubsumptionTests += incCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::decRemainingPossibleSubsumptionTestsCount(cint64 decCount) {
				mRemainingPossibleSubsumptionTests -= decCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setRemainingPossibleSubsumptionTestsCount(cint64 testCount) {
				mRemainingPossibleSubsumptionTests = testCount;
				return this;
			}

			QSet<COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getRemainingPossibleSubsumptionTestingSet() {
				return &mRemPossClassTestingSet;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getRemainingPossibleSubsumptionTestingCount() {
				return mRemPossClassTestingSet.count();
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incRunningPossibleSubsumptionTestsCount(cint64 incCount) {
				mRunningPossibleSubsumptionTests += incCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::decRunningPossibleSubsumptionTestsCount(cint64 decCount) {
				mRunningPossibleSubsumptionTests -= decCount;
				return this;
			} 



			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::addMemoryPools(CMemoryPool* memoryPools) {
				mMemoryPoolContainer.appendMemoryPool(memoryPools);
				return this;
			}

			CMemoryPool* COptimizedKPSetRoleOntologyClassificationItem::takeMemoryPools() {
				return mMemoryPoolContainer.takeMemoryPools();
			}


			cint64 COptimizedKPSetRoleOntologyClassificationItem::getCalculatedPossibleSubsumerCount() {
				return mCalculatedPossibleSubsumCount;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getCalculatedTruePossibleSubsumerCount() {
				return mCalculatedTruePossibleSubsumCount;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getCalculatedFalsePossibleSubsumerCount() {
				return mCalculatedFalsePossibleSubsumCount;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setCalculatedPossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedPossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setCalculatedTruePossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedTruePossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setCalculatedFalsePossibleSubsumerCount(cint64 subsumCount) {
				mCalculatedFalsePossibleSubsumCount = subsumCount;
				return this;
			}


			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incCalculatedPossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedPossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incCalculatedTruePossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedTruePossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incCalculatedFalsePossibleSubsumerCount(cint64 incSubsumCount) {
				mCalculatedFalsePossibleSubsumCount += incSubsumCount;
				return this;
			}



			cint64 COptimizedKPSetRoleOntologyClassificationItem::getPossibleSubsumerCount() {
				return mPossibleSubsumCount;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getTruePossibleSubsumerCount() {
				return mTruePossibleSubsumCount;
			}

			cint64 COptimizedKPSetRoleOntologyClassificationItem::getFalsePossibleSubsumerCount() {
				return mFalsePossibleSubsumCount;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setPossibleSubsumerCount(cint64 subsumCount) {
				mPossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTruePossibleSubsumerCount(cint64 subsumCount) {
				mTruePossibleSubsumCount = subsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setFalsePossibleSubsumerCount(cint64 subsumCount) {
				mFalsePossibleSubsumCount = subsumCount;
				return this;
			}


			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incPossibleSubsumerCount(cint64 incSubsumCount) {
				mPossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incTruePossibleSubsumerCount(cint64 incSubsumCount) {
				mTruePossibleSubsumCount += incSubsumCount;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::incFalsePossibleSubsumerCount(cint64 incSubsumCount) {
				mFalsePossibleSubsumCount += incSubsumCount;
				return this;
			}



			CConcreteOntology* COptimizedKPSetRoleOntologyClassificationItem::getTemporaryRoleClassificationOntology() {
				return mTmpRoleClassificationOnt;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTemporaryRoleClassificationOntology(CConcreteOntology* ont) {
				mTmpRoleClassificationOnt = ont;
				return this;
			}


			QHash<CConcept*,COptimizedKPSetRoleTestingItem*>* COptimizedKPSetRoleOntologyClassificationItem::getMarkerConceptInstancesItemHash() {
				return &mMarkerConRolInsItemHash;
			}

			CConcept* COptimizedKPSetRoleOntologyClassificationItem::getTemporaryAllPropagationConcept() {
				return mTmpAllPropConcept;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTemporaryAllPropagationConcept(CConcept* concept) {
				mTmpAllPropConcept = concept;
				return this;
			}

			CIndividual* COptimizedKPSetRoleOntologyClassificationItem::getTemporaryPropagationIndividual() {
				return mTmpIndiPropagation;
			}

			CIndividual* COptimizedKPSetRoleOntologyClassificationItem::getTemporaryMarkerIndividual() {
				return mTmpIndiMarker;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTemporaryPropagationIndividual(CIndividual* indi) {
				mTmpIndiPropagation = indi;
				return this;
			}

			COptimizedKPSetRoleOntologyClassificationItem* COptimizedKPSetRoleOntologyClassificationItem::setTemporaryMarkerIndividual(CIndividual* indi) {
				mTmpIndiMarker = indi;
				return this;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
