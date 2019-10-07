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

#include "CSatisfiableTaskAnswererBindingPropagationAdapter.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {

				CSatisfiableTaskAnswererBindingPropagationAdapter::CSatisfiableTaskAnswererBindingPropagationAdapter(CConcept* testingConcept, CConcept* propagationFinalizationConcept, CAnsweringHandler* answeringHandler, CConcreteOntology* testingOntology, CAnsweringMessageDataObserver* observer, CAnsweringPropagationSteeringController* propagationSteering) {
					mTestingConcept = testingConcept;
					mOntology = testingOntology;
					mMessageObserver = observer;
					mAnsweringHandler = answeringHandler;
					mPropagationFinalizationConcept = propagationFinalizationConcept;
					mPropagationSteeringController = propagationSteering;
				}


				CConcept* CSatisfiableTaskAnswererBindingPropagationAdapter::getTestingConcept() {
					return mTestingConcept;
				}

				CConcept* CSatisfiableTaskAnswererBindingPropagationAdapter::getPropagationFinalizationConcept() {
					return mPropagationFinalizationConcept;
				}

				CConcreteOntology* CSatisfiableTaskAnswererBindingPropagationAdapter::getTestingOntology() {
					return mOntology;
				}

				CAnsweringHandler* CSatisfiableTaskAnswererBindingPropagationAdapter::getAnsweringHandler() {
					return mAnsweringHandler;
				}

				CAnsweringMessageDataObserver *CSatisfiableTaskAnswererBindingPropagationAdapter::getAnswererMessageDataObserver() {
					return mMessageObserver;
				}

				CAnsweringPropagationSteeringController* CSatisfiableTaskAnswererBindingPropagationAdapter::getAnswererPropagationSteeringController() {
					return mPropagationSteeringController;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
