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

#include "COptimizedComplexVariableJoiningData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningData::COptimizedComplexVariableJoiningData() {
				mLeftBindingLinker = nullptr;
				mRightBindingLinker = nullptr;
			}


			COptimizedComplexVariableJoiningData* COptimizedComplexVariableJoiningData::addLeftBindingLinker(COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker) {
				mLeftBindingLinker = linker->append(mLeftBindingLinker);
				return this;
			}

			COptimizedComplexVariableJoiningData* COptimizedComplexVariableJoiningData::addRightBindingLinker(COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker) {
				mRightBindingLinker = linker->append(mRightBindingLinker);
				return this;
			}


			COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* COptimizedComplexVariableJoiningData::getLeftBindingLinker() const {
				return mLeftBindingLinker;
			}

			COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* COptimizedComplexVariableJoiningData::getRightBindingLinker() const {
				return mRightBindingLinker;
			}


			COptimizedComplexVariableJoiningData* COptimizedComplexVariableJoiningData::addBindingLinker(bool left, COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker) {
				if (left) {
					return addLeftBindingLinker(linker);
				} else {
					return addRightBindingLinker(linker);
				}
			}

			COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* COptimizedComplexVariableJoiningData::getBindingLinker(bool left) const {
				if (left) {
					return getLeftBindingLinker();
				} else {
					return getRightBindingLinker();
				}
			}

			bool COptimizedComplexVariableJoiningData::hasBindings(bool left) const {
				if (left) {
					return mLeftBindingLinker != nullptr;
				} else {
					return mRightBindingLinker != nullptr;
				}
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
