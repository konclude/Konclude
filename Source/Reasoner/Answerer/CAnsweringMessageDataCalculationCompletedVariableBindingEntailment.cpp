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

#include "CAnsweringMessageDataCalculationCompletedVariableBindingEntailment.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::CAnsweringMessageDataCalculationCompletedVariableBindingEntailment(CSatisfiableCalculationJob* calcJob, CAnsweringPropagationSteeringAbsorptionExtensionItemController* propagationSteeringController, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionPropagationData, CComplexQueryExpressionProcessingData* procData, COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absorptionPropagationExtension, CAnsweringSplitEntailmentTestingCancellationAdapter* cancellationAdapter)
					: CAnsweringMessageDataCalculationCompleted(calcJob, VARIABLEBINDINGENTAILMENTCALCULATION) {
				mAbsorptionHanldingQueryPart = absorptionPropagationData;
				mQueryProcData = procData;
				mPropagationSteeringController = propagationSteeringController;
				mAbsorptionPropagationExtension = absorptionPropagationExtension;
				mCancellationAdapter = cancellationAdapter;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::getAbsorbedQueryPartData() {
				return mAbsorptionHanldingQueryPart;
			}

			CComplexQueryExpressionProcessingData* CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::getQueryProcessingData() {
				return mQueryProcData;
			}


			CAnsweringPropagationSteeringAbsorptionExtensionItemController* CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::getPropagationSteeringController() {
				return mPropagationSteeringController;
			}


			COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::getAbsorptionPropagationExtension() {
				return mAbsorptionPropagationExtension;
			}


			CAnsweringSplitEntailmentTestingCancellationAdapter* CAnsweringMessageDataCalculationCompletedVariableBindingEntailment::getCancellationAdapter() {
				return mCancellationAdapter;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
