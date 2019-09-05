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

#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualBindingsCardinalityLinker::COptimizedComplexVariableIndividualBindingsCardinalityLinker() {
				initBindingsCardinalityLinker(nullptr, nullptr);
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker::COptimizedComplexVariableIndividualBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				initBindingsCardinalityLinker(bindings, cardinalities);
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityLinker::initBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				setData(bindings);
				mCurrentCardinality = cardinalities;
				mInitialCardinality = cardinalities;
				return this;
			}


			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityLinker::updateCardinality(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				mCurrentCardinality = cardinalities;
				return this;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityLinker::setInitialCardinality(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities) {
				mCurrentCardinality = cardinalities;
				mInitialCardinality = cardinalities;
				return this;
			}

			COptimizedComplexVariableIndividualBindings* COptimizedComplexVariableIndividualBindingsCardinalityLinker::getBindings() const {
				return data;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinalityLinker::getCurrentCardinalities() const {
				return mCurrentCardinality;
			}

			COptimizedComplexVariableIndividualBindingsCardinality* COptimizedComplexVariableIndividualBindingsCardinalityLinker::getInitialCardinalities() const {
				return mInitialCardinality;
			}

			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableIndividualBindingsCardinalityLinker::clearCardinalities() {
				mCurrentCardinality = nullptr;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
