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

#include "CAnsweringPropagationSteeringAbsorptionPropagationController.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			
			CAnsweringPropagationSteeringAbsorptionPropagationController::CAnsweringPropagationSteeringAbsorptionPropagationController(CVariable* var, cint64 indiModuloCount, cint64 indiModuloExp) {
				mVariable = var;
				mIndiModuloCount = indiModuloCount;
				mIndiModuloExp = indiModuloExp;
			}


			CAnsweringPropagationSteeringAbsorptionPropagationController::~CAnsweringPropagationSteeringAbsorptionPropagationController() {
			}
			



			bool CAnsweringPropagationSteeringAbsorptionPropagationController::finalizeWithClashing() {
				return false;
			}

			bool CAnsweringPropagationSteeringAbsorptionPropagationController::finalizeWithBindingExtraction() {
				return false;
			}

			bool CAnsweringPropagationSteeringAbsorptionPropagationController::isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi) {
				return true;
			}

			bool CAnsweringPropagationSteeringAbsorptionPropagationController::isPreparationBindingNominalIndividual(CVariable* variable, cint64 indiId) {
				if (mVariable == variable) {
					if (indiId % mIndiModuloCount == mIndiModuloExp) {
						return true;
					} else {
						return false;
					}
				}
				return true;
			}


			bool CAnsweringPropagationSteeringAbsorptionPropagationController::isPreparationBindingAllIndividuals(CVariable* variable) {
				if (mVariable != variable) {
					return true;
				} else {
					return false;
				}
			}

			bool CAnsweringPropagationSteeringAbsorptionPropagationController::isRestrictedTopPropagation(CConcept* concept) {
				return false;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
