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

#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData) {
				mAbsorptionData = absorptionData;
				mPossibleVariableMapping = nullptr;
				mTestingVariableMapping = nullptr;
				mPropatationTestCreated = false;
				mDependentMappingsComputationScheduled = false;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::~COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem() {
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::COMPOSITION_TYPE COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getCompositionType() {
				return ABSOROPTION_BASED_EXTENSION;
			}


			QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableExpressionCompositionItemHash() {
				return &mVariableExpressionBaseItem;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableCompositionItem(CExpressionVariable* variableExpression) {
				return mVariableExpressionBaseItem.value(variableExpression);
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableCompositionItem(CVariable* variable) {
				return mVariableBaseItem.value(variable);
			}

			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getAbsorptionBasedHandlingData() {
				return mAbsorptionData;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setVariableExpressionCompositionItem(CExpressionVariable* variableExpression, COptimizedComplexVariableCompositionItem* compositionItem) {
				mVariableExpressionBaseItem.insert(variableExpression, compositionItem);
				return this;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setVariableCompositionItem(CVariable* variable, COptimizedComplexVariableCompositionItem* compositionItem) {
				mVariableBaseItem.insert(variable, compositionItem);
				return this;
			}

			QSet<CIndividualReference>*& COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableSteeringIndividualBindingSet(CVariable* variable) {
				return mVariableIndividualBindingSet[variable];
			}

			QSet<COptimizedComplexVariableCompositionItem*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getIntegratedVariableCompositionItemSet() {
				return &mIntegratedItemSet;
			}

			QList< QPair<COptimizedComplexVariableCompositionItem*, CExpressionVariable*> >* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getExtendingCompositionItemAssociatedVariablePairList() {
				return &mVariableExtendingItemList;
			}

			QHash<CVariable*, cint64>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getVariableIndexHash() {
				return &mVariableIndexHash;
			}

			COptimizedComplexVariableIndividualMappings* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getPossibleVariableMapping() {
				return mPossibleVariableMapping;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPossibleVariableMapping(COptimizedComplexVariableIndividualMappings* possibleVariableMapping) {
				mPossibleVariableMapping = possibleVariableMapping;
				return this;
			}

			QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::getTestingVariableMapping() {
				return mTestingVariableMapping;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setTestingVariableMapping(QHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* testingVariableMapping) {
				mTestingVariableMapping = testingVariableMapping;
				return this;
			}

			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isPropatationTestCreated() {
				return mPropatationTestCreated;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setPropatationTestCreated(bool created) {
				mPropatationTestCreated = created;
				return this;
			}


			bool COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::isDependentMappingsComputationSchedluled() {
				return mDependentMappingsComputationScheduled;
			}

			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem::setDependentMappingsComputationSchedluled(bool scheduled) {
				mDependentMappingsComputationScheduled = scheduled;
				return this;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
