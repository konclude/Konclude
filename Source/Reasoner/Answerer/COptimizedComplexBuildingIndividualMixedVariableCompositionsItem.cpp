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

#include "COptimizedComplexBuildingIndividualMixedVariableCompositionsItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::COptimizedComplexBuildingIndividualMixedVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData) 
					: COptimizedComplexBuildingVariableCompositionsItem(queryProcessingData) {

				mInitializedVariableExtensions = false;
				mIndividualVariableTypeExtensionsProcessed = false;
				mLastIndiVarCompItem = nullptr;
				mLastIndiVarCompItemIndexMapping = nullptr;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::~COptimizedComplexBuildingIndividualMixedVariableCompositionsItem() {
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addIndividualVariableTypeVariableExtension(CIndividualVariableExpression* indiVarExp, CClassVariableExpression* classVarExp) {
				mRemainingVarExpSet.insert(indiVarExp);
				mAllVarExpSet.insert(indiVarExp);
				addIndividualVariableTypeExtension(indiVarExp, classVarExp);
				return this;
			}



			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addIndividualVariablePropertyVariableExtension(CIndividualVariableExpression* indiVarExp, CPropertyVariableExpression* propVarExp) {
				mRemainingVarExpSet.insert(indiVarExp);
				mAllVarExpSet.insert(indiVarExp);
				addIndividualVariablePropertyExtension(indiVarExp, propVarExp);
				return this;
			}



			bool COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::isWaitingComputation() {
				return hasWaitingVariableCompositionItems() || isWaitingSubVariableBuildingItems();
			}



			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getRemainingVariableExpressionSet() {
				return &mRemainingVarExpSet;
			}

			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getAllVariableSet() {
				return &mAllVarExpSet;
			}



			bool COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::isInitializedVariableExtensions() {
				return mInitializedVariableExtensions;
			}

			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setInitializedVariableExtensions(bool initialized) {
				mInitializedVariableExtensions = initialized;
				return this;
			}



			bool COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::isIndividualVariableTypeExtensionsProcessed() {
				return mIndividualVariableTypeExtensionsProcessed;
			}

			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setIndividualVariableTypeExtensionsProcessed(bool processed) {
				mIndividualVariableTypeExtensionsProcessed = processed;
				return this;
			}








			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addIndividualVariableSubQueryItem(CComplexAssertionsIndividualVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingIndividualVariableCompositionsItem* subQueryItem) {
				mSubQueryBuildingItemHash.insert(subQuery, subQueryItem);
				mSubQueryList.append(subQuery);
				mSubItemList.append(subQueryItem);
				mIndiSubItem = subQueryItem;
				return this;
			}



			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addClassVariableSubQueryItem(CComplexAxiomsClassVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingClassVariableCompositionsItem* subQueryItem) {
				mSubQueryBuildingItemHash.insert(subQuery, subQueryItem);
				mSubQueryList.append(subQuery);
				mSubItemList.append(subQueryItem);
				mSubClassItemList.append(subQueryItem);
				return this;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addPropertyVariableSubQueryItem(CComplexAxiomsObjectPropertyVariablesAnsweringQuery* subQuery, COptimizedComplexBuildingPropertyVariableCompositionsItem* subQueryItem) {
				mSubQueryBuildingItemHash.insert(subQuery, subQueryItem);
				mSubQueryList.append(subQuery);
				mSubItemList.append(subQueryItem);
				mSubPropertyItemList.append(subQueryItem);
				return this;
			}

			QList<CClassVariableExpression*> COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getIndividualVariableTypeExtensionVariables(CIndividualVariableExpression* extVar) {
				return mIndividualVariableTypeExtensionHash.values(extVar);
			}

			QList<CPropertyVariableExpression*> COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getIndividualVariablePropertyExtensionVariables(CIndividualVariableExpression* extVar) {
				return mIndividualVariablePropertyExtensionHash.values(extVar);
			}

			COptimizedComplexBuildingClassVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getExtensionClassVariableSubQueryBuildingItem(CClassVariableExpression* extVar) {
				return (COptimizedComplexBuildingClassVariableCompositionsItem*)mVariableItemHash.value(extVar);
			}

			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getExtensionPropertyVariableSubQueryBuildingItem(CPropertyVariableExpression* extVar) {
				return (COptimizedComplexBuildingPropertyVariableCompositionsItem*)mVariableItemHash.value(extVar);
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setExtensionClassVariableSubQueryBuildingItem(CClassVariableExpression* extVar, COptimizedComplexBuildingClassVariableCompositionsItem* item) {
				mVariableItemHash.insert(extVar, item);
				return this;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setExtensionPropertyVariableSubQueryBuildingItem(CPropertyVariableExpression* extVar, COptimizedComplexBuildingPropertyVariableCompositionsItem* item) {
				mVariableItemHash.insert(extVar, item);
				return this;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addIndividualVariableTypeExtension(CIndividualVariableExpression* indiVar, CClassVariableExpression* classVar) {
				mIndividualVariableTypeExtensionHash.insertMulti(indiVar, classVar);
				return this;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::addIndividualVariablePropertyExtension(CIndividualVariableExpression* indiVar, CPropertyVariableExpression* propVar) {
				mIndividualVariablePropertyExtensionHash.insertMulti(indiVar, propVar);
				return this;
			}


			QSet<QPair<CIndividualVariableExpression*, CClassVariableExpression*>>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getIndividualVariableTypeExtensionVariableHandledPairSet() {
				return &mHandledIndividualVariableTypeExtensionVariablePairSet;
			}

			QSet<QPair<CIndividualVariableExpression*, CPropertyVariableExpression*>>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getIndividualVariablePropertyExtensionVariableHandledPairSet() {
				return &mHandledIndividualVariablePropertyExtensionVariablePairSet;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getLastIndividualVariableCompositionItem() {
				return mLastIndiVarCompItem;
			}


			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setLastIndividualVariableCompositionItem(COptimizedComplexVariableCompositionItem* lastItem) {
				mLastIndiVarCompItem = lastItem;
				return this;
			}


			COptimizedComplexVariableCompositionItemVariableIndexMapping* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getLastIndividualVariableCompositionItemIndexMapping() {
				return mLastIndiVarCompItemIndexMapping;
			}

			COptimizedComplexBuildingIndividualMixedVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::setLastIndividualVariableCompositionItemIndexMapping(COptimizedComplexVariableCompositionItemVariableIndexMapping* indexMapping) {
				mLastIndiVarCompItemIndexMapping = indexMapping;
				return this;
			}



			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getIndividualVariableSubQueryBuildingItem() {
				return mIndiSubItem;
			}

			QList<COptimizedComplexBuildingClassVariableCompositionsItem*>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getSubQueryClassVariableBuildingItemList() {
				return &mSubClassItemList;
			}


			QSet<COptimizedComplexVariableCompositionItem*>* COptimizedComplexBuildingIndividualMixedVariableCompositionsItem::getJoiningVariableCompositionItemSet() {
				return &mJoiningVarCompItemSet;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
