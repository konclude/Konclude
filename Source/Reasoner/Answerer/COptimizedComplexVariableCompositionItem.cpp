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

#include "COptimizedComplexVariableCompositionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableCompositionItem::COptimizedComplexVariableCompositionItem() {
				mVariableMappingComputed = false;
				mComputationQueued = false;
				mVariableMapping = nullptr;
			}


			COptimizedComplexVariableCompositionItem::~COptimizedComplexVariableCompositionItem() {
			}


			bool COptimizedComplexVariableCompositionItem::isVariableMappingComputed() {
				return mVariableMappingComputed;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMappingComputed(bool computed) {
				mVariableMappingComputed = computed;
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

			COptimizedComplexVariableIndividualMapping* COptimizedComplexVariableCompositionItem::getVariableMapping() {
				return mVariableMapping;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItem::setVariableMapping(COptimizedComplexVariableIndividualMapping* variableMapping) {
				mVariableMapping = variableMapping;
				return this;
			}


			COptimizedComplexVariableDataLiteralExtensionItem*& COptimizedComplexVariableCompositionItem::getDataLiteralExtensionItem(CRole* dataRole, cint64 varIdx) {
				return mDataLiteralExtensionItemHash[QPair<CRole*, cint64>(dataRole, varIdx)];
			}


			COptimizedComplexVariableReductionItem*& COptimizedComplexVariableCompositionItem::getReductionItem(cint64 varIdx) {
				return mReductionItemHash[varIdx];
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
