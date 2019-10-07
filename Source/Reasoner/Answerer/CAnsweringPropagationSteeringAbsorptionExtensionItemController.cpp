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

#include "CAnsweringPropagationSteeringAbsorptionExtensionItemController.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CAnsweringPropagationSteeringAbsorptionExtensionItemController::CAnsweringPropagationSteeringAbsorptionExtensionItemController(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationItem) {
				mAbsorptionPropagationItem = absorptionPropagationItem;
			}


			CAnsweringPropagationSteeringAbsorptionExtensionItemController::~CAnsweringPropagationSteeringAbsorptionExtensionItemController() {
			}
			



			bool CAnsweringPropagationSteeringAbsorptionExtensionItemController::finalizeWithClashing() {
				return false;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemController::finalizeWithBindingExtraction() {
				return true;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemController::isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi) {
				QSet<CIndividualReference>* individualBindingSet = mAbsorptionPropagationItem->getVariableSteeringIndividualBindingSet(variable);
				if (individualBindingSet) {
					return individualBindingSet->contains(indi);
				}
				return false;
			}

			bool CAnsweringPropagationSteeringAbsorptionExtensionItemController::isPreparationBindingAllIndividuals(CVariable* variable) {
				COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData = mAbsorptionPropagationItem->getAbsorptionBasedHandlingData();
				CExpressionVariable* expVariable = absorptionData->getExpressionVariableVariableHash()->value(variable);
				if (absorptionData->getAnonymousVariableSet()->contains(expVariable)) {
					return true;
				}
				return false;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
