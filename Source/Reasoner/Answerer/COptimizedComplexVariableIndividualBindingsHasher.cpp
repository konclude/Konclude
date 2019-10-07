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

#include "COptimizedComplexVariableIndividualBindingsHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualBindingsHasher::COptimizedComplexVariableIndividualBindingsHasher(COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinker) {
				mBindings = bindingsLinker->getBindings();
				calculateHashValue();
			}

			COptimizedComplexVariableIndividualBindingsHasher::COptimizedComplexVariableIndividualBindingsHasher(COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinker, cint64 calculatedHashValue) {
				mBindings = bindingsLinker->getBindings();
				mHashValue = calculatedHashValue;
			}

			COptimizedComplexVariableIndividualBindingsHasher::COptimizedComplexVariableIndividualBindingsHasher(COptimizedComplexVariableIndividualBindings* bindings) {
				mBindings = bindings;
				calculateHashValue();
			}


			COptimizedComplexVariableIndividualBindingsHasher* COptimizedComplexVariableIndividualBindingsHasher::calculateHashValue() {
				if (mBindings) {
					mHashValue = mBindings->getHashValue();
				} else {
					mHashValue = 0;
				}
				return this;
			}


			cint64 COptimizedComplexVariableIndividualBindingsHasher::getHashValue() const {
				return mHashValue;
			}

			COptimizedComplexVariableIndividualBindings* COptimizedComplexVariableIndividualBindingsHasher::getBindings() const {
				return mBindings;
			}

			bool COptimizedComplexVariableIndividualBindingsHasher::operator==(const COptimizedComplexVariableIndividualBindingsHasher& hasher) const {
				if (mHashValue != hasher.mHashValue) {
					return false;
				}
				COptimizedComplexVariableIndividualBindings* bindings = hasher.getBindings();
				return mBindings->operator==(*bindings);
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
