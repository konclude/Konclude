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

#include "CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem, const COptimizedComplexVariableIndividualBindings& binding, CSameRealization* sameRealization) 
					: mBinding(binding) {
				mAbsorptionPropagationItem = absorptionPropagationItem;
				mSameRealization = sameRealization;
			}


			CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::~CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController() {
			}
			



			bool CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::finalizeWithClashing() {
				return true;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::finalizeWithBindingExtraction() {
				return false;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi) {

				cint64 index = mAbsorptionPropagationItem->getVariableIndexHash()->value(variable);
				TIndividualInstanceItemDataBinding sameItemInstance = mBinding.getBinding(index);

				bool containsIndiFlag = false;
				CInstanceBindingIndividualCheckingVisitor visitor(&containsIndiFlag, indi);
				mSameRealization->visitSameIndividuals(sameItemInstance.reference, &visitor);

				return containsIndiFlag;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemBindingController::isPreparationBindingAllIndividuals(CVariable* variable) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData = mAbsorptionPropagationItem->getAbsorptionBasedHandlingData();
				CExpressionVariable* expVariable = absorptionData->getExpressionVariableVariableHash()->value(variable);
				if (!expVariable) {
					return true;
				}
				if (absorptionData->getAnonymousVariableSet()->contains(expVariable)) {
					return true;
				}
				return false;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
