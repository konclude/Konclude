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

#include "COptimizedComplexVariableIndividualBindings.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableIndividualBindings::COptimizedComplexVariableIndividualBindings(cint64 bindingSize) {
				mBindingArray = nullptr;
				mBindingSize = bindingSize;
				initIndividualBindings(mBindingSize);
			}


			COptimizedComplexVariableIndividualBindings::COptimizedComplexVariableIndividualBindings(const COptimizedComplexVariableIndividualBindings& varBindings) {
				mBindingArray = nullptr;
				mBindingSize = varBindings.mBindingSize;
				initIndividualBindings(mBindingSize);
				for (cint64 i = 0; i < mBindingSize; ++i) {
					mBindingArray[i] = varBindings.mBindingArray[i];
				}
			}



			COptimizedComplexVariableIndividualBindings::~COptimizedComplexVariableIndividualBindings() {
				delete[] mBindingArray;
			}



			COptimizedComplexVariableIndividualBindings* COptimizedComplexVariableIndividualBindings::initIndividualBindings(cint64 bindingSize) {
				if (mBindingArray) {
					delete[] mBindingArray;
				}
				mBindingArray = new TIndividualInstanceItemDataBinding[bindingSize];
				mBindingSize = bindingSize;
				return this;
			}


			TIndividualInstanceItemDataBinding* COptimizedComplexVariableIndividualBindings::getBindingArray() const {
				return mBindingArray;
			}

			TIndividualInstanceItemDataBinding& COptimizedComplexVariableIndividualBindings::getBinding(cint64 idx) const {
				return mBindingArray[idx];
			}

			COptimizedComplexVariableIndividualBindings* COptimizedComplexVariableIndividualBindings::setBinding(cint64 idx, const TIndividualInstanceItemDataBinding& instItem) {
				mBindingArray[idx] = instItem;
				return this;
			}


			cint64 COptimizedComplexVariableIndividualBindings::getBindingSize() const {
				return mBindingSize;
			}


			cint64 COptimizedComplexVariableIndividualBindings::getHashValue() const {
				cint64 hashValue = mBindingSize;
				cint64 multiplier = 13;
				for (cint64 i = 0; i < mBindingSize; ++i) {
					multiplier = multiplier * 2 + 1;
					hashValue += multiplier*(cint64)mBindingArray[i].getHashValue();
				}
				return hashValue;
			}


			bool COptimizedComplexVariableIndividualBindings::operator==(const COptimizedComplexVariableIndividualBindings& bindings) const {
				if (mBindingSize != bindings.mBindingSize) {
					return false;
				}
				for (cint64 i = 0; i < mBindingSize; ++i) {
					if (mBindingArray[i] != bindings.mBindingArray[i]) {
						return false;
					}
				}
				return true;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
