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
#ifdef OPTIMIZED_ANSWERER_DEBUG_STRINGS
				debugVarItemIndexMapping = nullptr;
#endif
			}


			COptimizedComplexVariableCompositionItem::~COptimizedComplexVariableCompositionItem() {
			}


			bool COptimizedComplexVariableCompositionItem::isVariableMappingsComputed() {
				return mVariableMappingsComputed;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsComputed(bool computed) {
				mVariableMappingsComputed = computed;
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


			COptimizedComplexVariableReductionItem*& COptimizedComplexVariableCompositionItem::getReductionItem(cint64 varIdx) {
				return mReductionItemHash[varIdx];
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
				return mVariableMappingsExpectedCount;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingsExpectedCount(double count) {
				mVariableMappingsExpectedCount = count;
				return this;
			}





			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableCompositionItem::getComputationUpdateItemList() {
				return &mComputationUpdateItemList;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::addComputationUpdateItem(COptimizedComplexVariableCompositionItem* updateCompItem) {
				mComputationUpdateItemList.append(updateCompItem);
				return this;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setComputationUpdateItemList(const QList<COptimizedComplexVariableCompositionItem*>& computationDependentItemList) {
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


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
