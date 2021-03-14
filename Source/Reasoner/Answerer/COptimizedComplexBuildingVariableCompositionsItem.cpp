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

#include "COptimizedComplexBuildingVariableCompositionsItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexBuildingVariableCompositionsItem::COptimizedComplexBuildingVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData) {
				mQueryProcessingData = queryProcessingData;
				mProcessingQueued = false;
				mSatisfiable = true;
				mWaitingComputationStepsCount = 0;
				mLastHandVarExp = nullptr;
				mVarCompItemWaitingCount = 0;
				mSubVarBuildItemWaitingCount = 0;
			}


			COptimizedComplexBuildingVariableCompositionsItem::~COptimizedComplexBuildingVariableCompositionsItem() {
				for (COptimizedComplexVariableCompositionItem* usedComplexVariableCompositionItem : mUsedComplexVariableCompositionItemSet) {
					usedComplexVariableCompositionItem->decQueryUsageCount();
				}
				for (COptimizedComplexConceptItem* usedComplexConceptItem : mUsedComplexConceptItemSet) {
					usedComplexConceptItem->decQueryUsageCount();
				}
			}



			QHash<CExpressionVariable*, COptimizedComplexConceptItem*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableConceptItemHash() {
				return &mVarConItemHash;
			}

			QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableExpressionVariableCompositionItemHash() {
				return &mVarVarCompItemHash;
			}


			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexBuildingVariableCompositionsItem::getComputeVariableMappingItemList() {
				return &mComputeMappingItemList;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addComputeVariableMappingItem(COptimizedComplexVariableCompositionItem* item) {
				mComputeMappingItemList.append(item);
				return this;
			}


			QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* COptimizedComplexBuildingVariableCompositionsItem::getVariableItemIndexMappingHash() {
				return &mVarItemIndexMappingHash;
			}


			COptimizedComplexVariableCompositionItemVariableExpressionMapping& COptimizedComplexBuildingVariableCompositionsItem::getVariableItemIndexMapping(CExpressionVariable* varExp) {
				return mVarItemIndexMappingHash[varExp];
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::isProcessingQueued() {
				return mProcessingQueued;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setProcessingQueued(bool queued) {
				mProcessingQueued = queued;
				return this;
			}







			QHash<CExpressionVariable*, CBuildExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableClassTermExpressionHash() {
				return &mVariableClassTermExpressionHash;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setVariableClassTermExpressionHash(const QHash<CExpressionVariable*, CBuildExpression*>& hash) {
				mVariableClassTermExpressionHash = hash;
				return this;
			}





			bool COptimizedComplexBuildingVariableCompositionsItem::isSatisfiable() {
				return mSatisfiable;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setSatisfiability(bool satisfiable) {
				mSatisfiable = satisfiable;
				return this;
			}





			CComplexQueryExpressionProcessingData* COptimizedComplexBuildingVariableCompositionsItem::getQueryProcessingData() {
				return mQueryProcessingData;
			}





			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingSubVariableBuildingItems() {
				return mSubVarBuildItemWaitingCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getWaitingSubVariableBuildingItemCount() {
				return mSubVarBuildItemWaitingCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incWaitingSubVariableBuildingItemCount(cint64 incCount) {
				mSubVarBuildItemWaitingCount += incCount;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decWaitingSubVariableBuildingItemCount(cint64 decCount) {
				mSubVarBuildItemWaitingCount -= decCount;
				return this;
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::hasWaitingVariableCompositionItems() {
				return mVarCompItemWaitingCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getVariableCompositionItemWaitingCount() {
				return mVarCompItemWaitingCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incVariableCompositionItemWaitingCount(cint64 incCount) {
				mVarCompItemWaitingCount += incCount;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decVariableCompositionItemWaitingCount(cint64 decCount) {
				mVarCompItemWaitingCount -= decCount;
				return this;
			}




			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingComputationStep() {
				return mWaitingComputationStepsCount > 0;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount--;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount++;
				return this;
			}






			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::updateLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp, COptimizedComplexVariableCompositionItem* lastHandledItem) {
				COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping = mVarItemIndexMappingHash.value(lastHandledVarExp).value(lastHandledItem);
				if (mapping) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = mapping->constBegin(), itEnd = mapping->constEnd(); it != itEnd; ++it) {
						CExpressionVariable* variable = it.value();
						mVarLastItemHash.insert(variable, lastHandledItem);
						mVarLastItemAssociatedVariableHash.insert(variable, lastHandledVarExp);
					}
					mVarLastItemHash.insert(lastHandledVarExp, lastHandledItem);
				}
				return this;
			}



			COptimizedComplexVariableCompositionItem* COptimizedComplexBuildingVariableCompositionsItem::getVariableLastCompositionItem(CExpressionVariable* variableExp) {
				return mVarLastItemHash.value(variableExp);
			}



			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::clearLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp) {
				mVarLastItemHash.insert(lastHandledVarExp, nullptr);
				mVarLastItemAssociatedVariableHash.insert(lastHandledVarExp, nullptr);
				return this;
			}


			CExpressionVariable* COptimizedComplexBuildingVariableCompositionsItem::getVariableLastCompositionItemAssociatedVariableExpression(CExpressionVariable* variableExp) {
				return mVarLastItemAssociatedVariableHash.value(variableExp);
			}



			CExpressionVariable* COptimizedComplexBuildingVariableCompositionsItem::getLastHandledVariableExpression() {
				return mLastHandVarExp;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setLastHandledVariableExpression(CExpressionVariable* varExp) {
				mLastHandVarExp = varExp;
				return this;
			}


			QSet<COptimizedComplexVariableCompositionItem*>* COptimizedComplexBuildingVariableCompositionsItem::getUsedComplexVariableCompositionItemSet() {
				return &mUsedComplexVariableCompositionItemSet;
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::addUsedComplexVariableCompositionItem(COptimizedComplexVariableCompositionItem* item) {
				if (!mUsedComplexVariableCompositionItemSet.contains(item)) {
					mUsedComplexVariableCompositionItemSet.insert(item);
					item->incQueryUsageCount();
					return true;
				}
				return false;
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::addUsedComplexConceptItem(COptimizedComplexConceptItem* item) {
				if (!mUsedComplexConceptItemSet.contains(item)) {
					mUsedComplexConceptItemSet.insert(item);
					item->incQueryUsageCount();
					return true;
				}
				return false;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
