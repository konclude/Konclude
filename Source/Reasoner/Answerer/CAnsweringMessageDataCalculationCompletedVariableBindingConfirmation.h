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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDVARIABLEBINDINGCONFIRMATION_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDVARIABLEBINDINGCONFIRMATION_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringMessageDataCalculationCompleted.h"
#include "COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem.h"
#include "COptimizedComplexVariableIndividualBindings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation : public CAnsweringMessageDataCalculationCompleted {
				// public methods
				public:
					//! Constructor
					CAnsweringMessageDataCalculationCompletedVariableBindingConfirmation(CSatisfiableCalculationJob* calcJob, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, COptimizedComplexVariableIndividualBindingsCardinalityLinker* testingVarIndiBindingCardLinker);


					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* getAbsorptionPropagationItem();
					COptimizedComplexBuildingVariableCompositionsItem* getVariableBuildingItem();

					COptimizedComplexVariableIndividualBindings* getPropagatedBinding();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getPropagatedBindingCardinalityLinker();

				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* mAbsorptionPropagationExtension;
					COptimizedComplexBuildingVariableCompositionsItem* mBuildingVarItem;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mPropagatedBindingCardinalityLinker;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMESSAGEDATACALCULATIONCOMPLETEDVARIABLEBINDINGCONFIRMATION_H
