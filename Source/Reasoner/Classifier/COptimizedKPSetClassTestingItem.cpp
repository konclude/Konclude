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

#include "COptimizedKPSetClassTestingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COptimizedKPSetClassTestingItem::COptimizedKPSetClassTestingItem() {
			}


			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::initSatisfiableTestingItem(CConcept* satTestConcept, QHash<CConcept*,CClassificationSatisfiableCalculationConceptReferenceLinking*>* conRefLinkDataHash) {
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

				mPossibleSubsumptionMap = nullptr;
				mPossSubsumMapInitialized = false;
				mPropagationConnected = false;
				mPseudoModelInitialized = false;

				mPossibleSubsumedList = nullptr;
				mPossibleSubsumedSet = nullptr;

				mFastSatCacheEntry = nullptr;
				mSuccFastSatTested = false;
				mIndiDepTracked = false;
				mConRefLinkDataHash = conRefLinkDataHash;
				return this;
			}

			COptimizedKPSetClassTestingItem::~COptimizedKPSetClassTestingItem() {
				delete mPossibleSubsumedList;
				delete mPossibleSubsumedSet;
			}

			CConcept* COptimizedKPSetClassTestingItem::getTestingConcept() {
				return mConceptSat;
			}

			CHierarchyNode* COptimizedKPSetClassTestingItem::getSatisfiableConceptHierarchyNode() {
				return mConSatHierNode;
			}


			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setSatisfiableConceptHierarchyNode(CHierarchyNode* hierNode) {
				mConSatHierNode = hierNode;
				return this;
			}


			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getSubsumingConceptItemSet() {
				return &mSubsumingConceptItemSet;
			}

			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getSubsumingConceptItemList() {
				return &mSubsumingConceptItemList;
			}


			cint64 COptimizedKPSetClassTestingItem::getSubsumingConceptItemCount() const {
				return mSubsumingConceptItemSet.count();
			}

			bool COptimizedKPSetClassTestingItem::hasSubsumerConceptItem(COptimizedKPSetClassTestingItem* item) {
				return mSubsumingConceptItemSet.contains(item);
			}


			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getSuccessorItemList() {
				return &mSuccList;
			}

			cint64 COptimizedKPSetClassTestingItem::getUnprocessedPredecessorItemCount() {
				return mPredCounter;
			}

			bool COptimizedKPSetClassTestingItem::hasOnlyProcessedPredecessorItems() {
				return mPredCounter <= 0;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::decUnprocessedPredecessorItems(cint64 decCount) {
				mPredCounter -= decCount;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::incUnprocessedPredecessorItems(cint64 incCount) {
				mPredCounter += incCount;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setUnprocessedPredecessorItems(cint64 predCount) {
				mPredCounter = predCount;
				return this;
			}


			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::addSuccessorSatisfiableTestItem(COptimizedKPSetClassTestingItem* succItem) {
				mSuccList.append(succItem);
				return this;
			}


			bool COptimizedKPSetClassTestingItem::isSatisfiableTestOrdered() {
				return mSatTestOrdered;
			}

			bool COptimizedKPSetClassTestingItem::isSatisfiableTested() {
				return mTestedSat;
			}

			bool COptimizedKPSetClassTestingItem::isResultUnsatisfiableDerivated() {
				return mUnsatDerivated;
			}

			bool COptimizedKPSetClassTestingItem::isResultSatisfiableDerivated() {
				return mSatDerivated;
			}

			bool COptimizedKPSetClassTestingItem::getSatisfiableTestedResult() {
				return mSatTestedResult;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setSatisfiableTestOrdered(bool satTestOrdered) {
				mSatTestOrdered = satTestOrdered;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setSatisfiableTested(bool satTested) {
				mTestedSat = satTested;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setSatisfiableTestedResult(bool satTestedResult) {
				mSatTestedResult = satTestedResult;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setResultUnsatisfiableDerivated(bool unsatDerivated) {
				mUnsatDerivated = unsatDerivated;
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setResultSatisfiableDerivated(bool satDerivated) {
				mSatDerivated = satDerivated;
				return this;
			}


			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::addSubsumingConceptItem(COptimizedKPSetClassTestingItem* subsumingItem) {
				if (!mSubsumingConceptItemSet.contains(subsumingItem)) {
					mSubsumingConceptItemList.append(subsumingItem);
					mSubsumingConceptItemSet.insert(subsumingItem);

					//QString iriClassNameString = CIRIName::getRecentIRIName(subsumingItem->getTestingConcept()->getClassNameLinker());
					//mSubsumingConceptString += QString("%1\r\n").arg(iriClassNameString);
				}
				return this;
			}



			CConceptSubsumerObserver* COptimizedKPSetClassTestingItem::tellConceptSupsumption(CConcept* subsumedConcept, CConcept* subsumerConcept) {
				if (subsumerConcept->getOperatorCode() != CCTOP) {
					CConceptProcessData* conProData = (CConceptProcessData*)subsumerConcept->getConceptData();
					if (!conProData->isInvalidatedReferenceLinking()) {
						CConceptSatisfiableReferenceLinkingData* conSatRefLinking = (CConceptSatisfiableReferenceLinkingData*)conProData->getConceptReferenceLinking();
						if (conSatRefLinking) {
							COptimizedKPSetClassTestingItem* subsumerClassSaturationItem = (COptimizedKPSetClassTestingItem*)conSatRefLinking->getClassifierReferenceLinkingData();
							addSubsumingConceptItem(subsumerClassSaturationItem);
						}
					} else if (mConRefLinkDataHash) {
						COptimizedKPSetClassTestingItem* subsumerClasItem = (COptimizedKPSetClassTestingItem*)mConRefLinkDataHash->value(subsumerConcept);
						addSubsumingConceptItem(subsumerClasItem);
					}
				}
				return this;
			}


			bool itemSortLessThan(const COptimizedKPSetClassTestingItem* item1, const COptimizedKPSetClassTestingItem* item2) {
				return item1->getSubsumingConceptItemCount() > item2->getSubsumingConceptItemCount();
			}

			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::sortSubsumingConceptItemList() {
				qSort(mSubsumingConceptItemList.begin(),mSubsumingConceptItemList.end(),itemSortLessThan);
				return &mSubsumingConceptItemList;
			}

			bool COptimizedKPSetClassTestingItem::isEquivalentItem() {
				return mEquiItem;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setEquivalentItem(bool equivalent) {
				mEquiItem = equivalent;
				return this;
			}


			bool COptimizedKPSetClassTestingItem::isPredecessorItem() {
				return mPredOfItem;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setPredecessorItem(bool isPredecessorOfOneItem) {
				mPredOfItem = isPredecessorOfOneItem;
				return this;
			}

			bool COptimizedKPSetClassTestingItem::isMoreConceptClassificationInformationRequired() {
				if (mSatTestOrdered || mTestedSat || mUnsatDerivated || mSatDerivated) {
					return false;
				}
				return true;
			}

			COptimizedKPSetClassPossibleSubsumptionMap* COptimizedKPSetClassTestingItem::getPossibleSubsumptionMap(bool create) {
				if (!mPossibleSubsumptionMap && create) {
					mPossibleSubsumptionMap = new COptimizedKPSetClassPossibleSubsumptionMap();
				}
				return mPossibleSubsumptionMap;
			}


			CClassPossibleSubsumptionMap* COptimizedKPSetClassTestingItem::getClassPossibleSubsumptionMap() {
				return getPossibleSubsumptionMap(false);
			}


			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getUpPropagationItemSet() {
				return &mUpPropagationItemSet;
			}

			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getDownPropagationItemSet() {
				return &mDownPropagationItemSet;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::addUpPropagationItem(COptimizedKPSetClassTestingItem* item) {
				mUpPropagationItemSet.insert(item);
				return this;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::addDownPropagationItem(COptimizedKPSetClassTestingItem* item) {
				mDownPropagationItemSet.insert(item);
				return this;
			}


			bool COptimizedKPSetClassTestingItem::isPossibleSubsumptionMapInitialized() {
				return mPossSubsumMapInitialized;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setPossibleSubsumptionMapInitialized(bool initialized) {
				mPossSubsumMapInitialized = initialized;
				return this;
			}

			bool COptimizedKPSetClassTestingItem::isPropagationConnected() {
				return mPropagationConnected;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setPropagationConnected(bool connected) {
				mPropagationConnected = connected;
				return this;
			}

			CClassificationClassPseudoModel* COptimizedKPSetClassTestingItem::getClassPseudoModel() {
				return &mPseudoModel;
			}

			bool COptimizedKPSetClassTestingItem::isClassPseudoModelInitalized() {
				return mPseudoModelInitialized;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setClassPseudoModelInitalized(bool initialized) {
				mPseudoModelInitialized = initialized;
				return this;
			}


			QSet<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getPossibleSubsumedSet(bool create) {
				if (create && !mPossibleSubsumedSet) {
					mPossibleSubsumedSet = new QSet<COptimizedKPSetClassTestingItem*>();
				}
				return mPossibleSubsumedSet;
			}

			QList<COptimizedKPSetClassTestingItem*>* COptimizedKPSetClassTestingItem::getPossibleSubsumedList() {
				return mPossibleSubsumedList;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setPossibleSubsumedList(QList<COptimizedKPSetClassTestingItem*>* possSubsumedList) {
				mPossibleSubsumedList = possSubsumedList;
				return this;
			}

			bool COptimizedKPSetClassTestingItem::hasRemainingPossibleSubsumedItems() {
				return mPossibleSubsumedList && !mPossibleSubsumedList->isEmpty();
			}



			CCacheEntry* COptimizedKPSetClassTestingItem::getFastSatisfiabilityTestedSaturationCacheEntry() {
				return mFastSatCacheEntry;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setFastSatisfiabilityTestedSaturationCacheEntry(CCacheEntry* cacheEntry) {
				mFastSatCacheEntry = cacheEntry;
				return this;
			}

			bool COptimizedKPSetClassTestingItem::hasSuccessfullyFastSatisfiabilityTested() {
				return mSuccFastSatTested;
			}

			COptimizedKPSetClassTestingItem* COptimizedKPSetClassTestingItem::setSuccessfullyFastSatisfiabilityTested(bool successfullyTested) {
				mSuccFastSatTested = successfullyTested;
				return this;
			}

			CIndividualDependenceTrackingMarker* COptimizedKPSetClassTestingItem::setIndividualDependenceTracked() {
				mIndiDepTracked = true;
				return this;
			}

			bool COptimizedKPSetClassTestingItem::hasIndividualDependenceTracked() {
				return mIndiDepTracked;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
