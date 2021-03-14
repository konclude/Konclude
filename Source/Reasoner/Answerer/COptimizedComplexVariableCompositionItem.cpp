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

#include "COptimizedComplexVariableCompositionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableCompositionItem::COptimizedComplexVariableCompositionItem() {
				mVariableMappingsComputed = false;
				mVariableMappingsInitialized = false;
				mComputationQueued = false;
				mVariableMapping = nullptr;
				mVariableMappingsComputationRequirement = 1;
				mVariableMappingsExpectedCount = 0;
				mDependencyUpdatingCount = 0;
				mRepeatedRequirementInsufficientDependencyComputationIncreaseFactor = 1.;
				mComputationStepId = 0;
				mComputationAttempt = 1;
				mSplitComputationMode = false;
				mSplitComputations = false;
				mHadSplitComputations = false;
				mRequiresSplitComputationActivation = false;
				mLastComputation = false;
				mNextSplitComputationPos = 0;
				mCurrentSplitComputationPos = 0;
				mTotalSplitComputationCount = 0;
				mCompletedSplitComputationCount = 0;
				mSplitItem = nullptr;
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				debugVarItemIndexMapping = nullptr;
#endif
				mLastVariableMappingCacheReportedCount = -1;
				mLastVariableMappingCacheReportedSize = -1;
				mComputationItemDepth = 0;
				mCacheDescendantsIncompleteComputationCostSum = 0;
				mComputedVariableMappingsCount = -1;
				mVariableMappingsComputationStarted = false;
			}


			COptimizedComplexVariableCompositionItem::~COptimizedComplexVariableCompositionItem() {
			}


			bool COptimizedComplexVariableCompositionItem::isVariableMappingsComputed() {
				return mVariableMappingsComputed;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsComputed(bool computed) {
				mVariableMappingsComputed = computed;
				mComputedVariableMappingsCount = mVariableMapping->getBindingCount();
				return this;
			}

			bool COptimizedComplexVariableCompositionItem::isVariableMappingsInitialized() {
				return mVariableMappingsInitialized;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsInitialized(bool initialized) {
				mVariableMappingsInitialized = initialized;
				return this;
			}

			bool COptimizedComplexVariableCompositionItem::isComputationQueued() {
				return mComputationQueued;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationQueued(bool queued) {
				mComputationQueued = queued;
				return this;
			}


			QHash<CVariantTrible<CRole*, bool, cint64>, COptimizedComplexVariableRolePropagationItem*>* COptimizedComplexVariableCompositionItem::getRoleInverseItemHash() {
				return &mRoleInverseItemHash;
			}

			QHash<COptimizedComplexVariableJoiningBindingPositionMapping, COptimizedComplexVariableJoiningItem*>* COptimizedComplexVariableCompositionItem::getJoiningItemHash() {
				return &mJoiningItemHash;
			}


			COptimizedComplexVariableRolePropagationItem*& COptimizedComplexVariableCompositionItem::getRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx) {
				return mRoleInverseItemHash[CVariantTrible<CRole*, bool, cint64>(role, inversed, varIdx)];
			}




			COptimizedComplexVariableRolePropagationJoiningItem*& COptimizedComplexVariableCompositionItem::getRoleInversePropagationJoiningItem(CRole* role, bool inversed, cint64 varIdx, cint64 joinVarIdx) {
				return mRoleInverseJoinItemHash[CVariantQuad<CRole*, bool, cint64, cint64>(role, inversed, varIdx, joinVarIdx)];
			}



			COptimizedComplexVariableRolePropagationReplacementItem*& COptimizedComplexVariableCompositionItem::getRoleInversePropagationReplacementItem(CRole* role, bool inversed, cint64 varIdx, cint64 replaceVarIdx) {
				return mRoleInverseReplaceItemHash[CVariantQuad<CRole*, bool, cint64, cint64>(role, inversed, varIdx, replaceVarIdx)];
			}


			bool COptimizedComplexVariableCompositionItem::hasRoleInversePropagationItem(CRole* role, bool inversed, cint64 varIdx) {
				return mRoleInverseItemHash.contains(CVariantTrible<CRole*, bool, cint64>(role, inversed, varIdx));
			}

			COptimizedComplexVariableJoiningItem*& COptimizedComplexVariableCompositionItem::getJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping) {
				return mJoiningItemHash[bindingPosMapping];
			}

			bool COptimizedComplexVariableCompositionItem::hasJoiningItem(const COptimizedComplexVariableJoiningBindingPositionMapping& bindingPosMapping) {
				return mJoiningItemHash.contains(bindingPosMapping);
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableCompositionItem::getVariableMapping() {
				return mVariableMapping;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMapping(COptimizedComplexVariableIndividualMappings* variableMapping) {
				mVariableMapping = variableMapping;
				return this;
			}


			COptimizedComplexVariableDataLiteralExtensionItem*& COptimizedComplexVariableCompositionItem::getDataLiteralExtensionItem(CRole* dataRole, cint64 varIdx) {
				return mDataLiteralExtensionItemHash[QPair<CRole*, cint64>(dataRole, varIdx)];
			}


			COptimizedComplexVariableSubSuperClassesPropagationItem*& COptimizedComplexVariableCompositionItem::getSubSuperClassesPropagationItem(bool superClassesProp, cint64 varIdx) {
				return mSubSuperClassesPropagationItem[QPair<bool, cint64>(superClassesProp, varIdx)];
			}


			COptimizedComplexVariableSubSuperPropertiesPropagationItem*& COptimizedComplexVariableCompositionItem::getSubSuperPropertiesPropagationItem(bool superPropertiesProp, cint64 varIdx) {
				return mSubSuperPropertiesPropagationItem[QPair<bool, cint64>(superPropertiesProp, varIdx)];
			}



			COptimizedComplexVariableIndividualTypeExtensionItem*& COptimizedComplexVariableCompositionItem::getIndividualTypeExtensionItem(cint64 indiVarIdx, COptimizedComplexVariableCompositionItem* classVarCompItem, cint64 classVarIdx) {
				return mIndividualTypeExtensionItemHash[CVariantTrible<cint64, COptimizedComplexVariableCompositionItem*, cint64>(indiVarIdx, classVarCompItem, classVarIdx)];
			}



			COptimizedComplexVariableIndividualNeighboringPropertyExtensionItem*& COptimizedComplexVariableCompositionItem::getIndividualNeighboringPropertyExtensionItem(cint64 indiVarIdx, COptimizedComplexVariableCompositionItem* propVarCompItem, cint64 classVarIdx) {
				return mIndividualPropertyExtensionItemHash[CVariantTrible<cint64, COptimizedComplexVariableCompositionItem*, cint64>(indiVarIdx, propVarCompItem, classVarIdx)];
			}


			COptimizedComplexVariableReductionItem*& COptimizedComplexVariableCompositionItem::getReductionItem(cint64 varIdx) {
				return mReductionItemHash[varIdx];
			}

			COptimizedComplexVariableExtractionItem*& COptimizedComplexVariableCompositionItem::getExtractionItem(cint64 varIdx) {
				return mExtractionItemHash[varIdx];
			}

			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableCompositionItem::getComputationDependentItemList() {
				return &mComputationDependentItemList;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::addComputationDependentItem(COptimizedComplexVariableCompositionItem* depCompItem) {
				mComputationDependentItemList.append(depCompItem);
				return this;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationDependentItemList(const QList<COptimizedComplexVariableCompositionItem*>& computationDependentItemList) {
				mComputationDependentItemList = computationDependentItemList;
				return this;
			}

			cint64 COptimizedComplexVariableCompositionItem::getVariableMappingsComputationRequirement() {
				return mVariableMappingsComputationRequirement;
			}


			bool COptimizedComplexVariableCompositionItem::requiresMoreVariableMappingsComputation() {
				return getVariableMappingsComputationRequirement() < 0 || getVariableMappingsCurrentCount() < getVariableMappingsComputationRequirement();
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsComputationRequirement(cint64 limit) {
				mVariableMappingsComputationRequirement = limit;
				return this;
			}

			cint64 COptimizedComplexVariableCompositionItem::getVariableMappingsCurrentCount() {
				if (mVariableMapping) {
					return mVariableMapping->getBindingCount();
				} else {
					return 0;
				}
			}

			double COptimizedComplexVariableCompositionItem::getVariableMappingsRemainingCount() {
				return getVariableMappingsExpectedCount() - (double)getVariableMappingsCurrentCount();
			}

			double COptimizedComplexVariableCompositionItem::getVariableMappingsExpectedCount() {
				if (mComputedVariableMappingsCount >= 0) {
					return mComputedVariableMappingsCount;
				} else {
					return mVariableMappingsExpectedCount;
				}
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsExpectedCount(double count) {
				mVariableMappingsExpectedCount = count;
				return this;
			}





			QList<COptimizedComplexVariableCompositionItemUpdateData>* COptimizedComplexVariableCompositionItem::getComputationUpdateItemList() {
				return &mComputationUpdateItemList;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::addComputationUpdateItem(COptimizedComplexVariableCompositionItem* updateCompItem) {
				mComputationUpdateItemList.append(COptimizedComplexVariableCompositionItemUpdateData(updateCompItem, nullptr));
				return this;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::addComputationUpdateItem(COptimizedComplexVariableCompositionItem* updateCompItem, COptimizedComplexBuildingVariableCompositionsItem* buildingItem) {
				mComputationUpdateItemList.append(COptimizedComplexVariableCompositionItemUpdateData(updateCompItem, buildingItem));
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationUpdateItemList(const QList<COptimizedComplexVariableCompositionItemUpdateData>& computationDependentItemList) {
				mComputationUpdateItemList = computationDependentItemList;
				return this;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::decDependencyUpdatingCount(cint64 count) {
				mDependencyUpdatingCount -= count;
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::incDependencyUpdatingCount(cint64 count) {
				mDependencyUpdatingCount += count;
				return this;
			}

			cint64 COptimizedComplexVariableCompositionItem::getDependencyUpdatingCount() {
				return mDependencyUpdatingCount;
			}



			double COptimizedComplexVariableCompositionItem::repeatedRequirementInsufficientDependencyComputationIncreaseFactor() {
				return mRepeatedRequirementInsufficientDependencyComputationIncreaseFactor;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(double factor) {
				mRepeatedRequirementInsufficientDependencyComputationIncreaseFactor = factor;
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::increaseRepeatedRequirementInsufficientDependencyComputationIncreaseFactor(double increaseFactor) {
				mRepeatedRequirementInsufficientDependencyComputationIncreaseFactor *= increaseFactor;
				return this;
			}


			cint64 COptimizedComplexVariableCompositionItem::getComputationStepId() {
				return mComputationStepId;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationStepId(cint64 step) {
				mComputationStepId = step;
				return this;
			}

			cint64 COptimizedComplexVariableCompositionItem::getComputationAttempt(bool increase) {
				cint64 attempt = mComputationAttempt;
				if (increase) {
					++mComputationAttempt;
				}
				return attempt;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationAttempt(cint64 attempt) {
				mComputationAttempt = attempt;
				return this;
			}

			COptimizedComplexVariableCompositionItemRoleSamplingData& COptimizedComplexVariableCompositionItem::getRoleSamplingData(CRole* role) {
				return mRoleSamplingDataHash[role];
			}


			bool COptimizedComplexVariableCompositionItem::isLastComputation() {
				return mLastComputation;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setLastComputation(bool last) {
				mLastComputation = last;
				return this;
			}

			bool COptimizedComplexVariableCompositionItem::isSplitComputationMode() {
				return mSplitComputationMode;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setSplitComputationMode(bool splitMode) {
				mSplitComputationMode = splitMode;
				return this;
			}


			bool COptimizedComplexVariableCompositionItem::hasSplitComputations() {
				return mSplitComputations;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setSplitComputations(bool splitComputations) {
				mSplitComputations = splitComputations;
				mHadSplitComputations = true;
				return this;
			}

			bool COptimizedComplexVariableCompositionItem::requiresSplitComputationActivation() {
				return mRequiresSplitComputationActivation;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setRequiresSplitComputationActivation(bool splitActivation) {
				mRequiresSplitComputationActivation = splitActivation;
				return this;
			}



			cint64 COptimizedComplexVariableCompositionItem::getNextSplitComputationPosition() {
				return mNextSplitComputationPos;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setNextSplitComputationPosition(cint64 pos) {
				mNextSplitComputationPos = pos;
				return this;
			}


			cint64 COptimizedComplexVariableCompositionItem::getCurrentSplitComputationPosition() {
				return mCurrentSplitComputationPos;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setCurrentSplitComputationPosition(cint64 pos) {
				mCurrentSplitComputationPos = pos;
				return this;
			}


			cint64 COptimizedComplexVariableCompositionItem::getTotalSplitComputationCount() {
				return mTotalSplitComputationCount;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setTotalSplitComputationCount(cint64 count) {
				mTotalSplitComputationCount = count;
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::getCurrentSplitComputationItem() {
				return mSplitItem;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setCurrentSplitComputationItem(COptimizedComplexVariableCompositionItem* splitItem) {
				mSplitItem = splitItem;
				return this;
			}


			cint64 COptimizedComplexVariableCompositionItem::getCompletedSplitComputationCount() {
				return mCompletedSplitComputationCount;
			}

			cint64 COptimizedComplexVariableCompositionItem::getRemainingSplitComputationCount() {
				return mTotalSplitComputationCount - mCompletedSplitComputationCount;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setCompletedSplitComputationCount(cint64 count) {
				mCompletedSplitComputationCount = count;
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::incCompletedSplitComputationCount(cint64 count) {
				mCompletedSplitComputationCount += count;
				return this;
			}

			cint64 COptimizedComplexVariableCompositionItem::getLastVariableMappingCacheReportedCount() {
				return mLastVariableMappingCacheReportedCount;
			}

			cint64 COptimizedComplexVariableCompositionItem::getLastVariableMappingCacheReportedSize() {
				return mLastVariableMappingCacheReportedSize;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setLastVariableMappingCacheReportedCount(cint64 count) {
				mLastVariableMappingCacheReportedCount = count;
				return this;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setLastVariableMappingCacheReportedSize(cint64 size) {
				mLastVariableMappingCacheReportedSize = size;
				return this;
			}



			QSet<COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableCompositionItem::getComputationSuccessorItemSet() {
				return &mComputationSuccessorItemSet;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::addComputationSuccessorItem(COptimizedComplexVariableCompositionItem* succItem) {
				mComputationSuccessorItemSet.insert(succItem);
				return this;
			}


			cint64 COptimizedComplexVariableCompositionItem::getComputationItemDepth() {
				return mComputationItemDepth;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationItemDepth(cint64 depth) {
				mComputationItemDepth = depth;
				return this;
			}



			bool COptimizedComplexVariableCompositionItem::isVariableMappingsComputationStarted() {
				return mVariableMappingsComputationStarted;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsComputationStarted(bool started) {
				mVariableMappingsComputationStarted = started;
				return this;
			}



			bool COptimizedComplexVariableCompositionItem::clearComputation() {
				mVariableMapping->clearComputedMappings();
				mVariableMappingsComputationRequirement = 1;
				mVariableMappingsComputed = false;
				mVariableMappingsComputationStarted = false;
				mVariableMappingsInitialized = false;
				mSplitComputationMode = false;
				mSplitComputations = false;
				if (mSplitItem) {
					mSplitItem->clearComputation();
					delete mSplitItem;
				}
				mRequiresSplitComputationActivation = false;
				mCurrentSplitComputationPos = 0;
				mTotalSplitComputationCount = 0;
				mCompletedSplitComputationCount = 0;
				mNextSplitComputationPos = 0;
				mSplitItem = nullptr;
				return true;
			}

			double COptimizedComplexVariableCompositionItem::getCacheDescendantsIncompleteComputationCostSum() {
				return mCacheDescendantsIncompleteComputationCostSum;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setCacheDescendantsIncompleteComputationCostSum(double cost) {
				mCacheDescendantsIncompleteComputationCostSum = cost;
				return this;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
