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

#include "COptimizedComplexVariableJoiningHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningHasher::COptimizedComplexVariableJoiningHasher(TIndividualInstanceItemDataBinding* binding, CXLinker<cint64>* keyBindingLinker) {
				mKeyBindingLinker = keyBindingLinker;
				mBinding = binding;
				calculateHashValue();
			}


			cint64 COptimizedComplexVariableJoiningHasher::getHashValue() const {
				return mHashValue; 
			}


			COptimizedComplexVariableJoiningHasher* COptimizedComplexVariableJoiningHasher::calculateHashValue() {
				mHashValue = 0;
				for (CXLinker<cint64>* keyBindingLinkerIt = mKeyBindingLinker; keyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext()) {
					cint64 nextKeyIdx = keyBindingLinkerIt->getData();
					cint64 value = mBinding[nextKeyIdx].getHashValue();
					mHashValue = (value +mHashValue)*13+1;
				}
				return this;
			}

			CXLinker<cint64>* COptimizedComplexVariableJoiningHasher::getKeyBindingLinker() const {
				return mKeyBindingLinker;
			}

			TIndividualInstanceItemDataBinding* COptimizedComplexVariableJoiningHasher::getBinding() const {
				return mBinding;
			}



			bool COptimizedComplexVariableJoiningHasher::operator==(const COptimizedComplexVariableJoiningHasher& hasher) const {
				CXLinker<cint64>* otherKeyBindingLinkerIt = hasher.getKeyBindingLinker();
				CXLinker<cint64>* keyBindingLinkerIt = mKeyBindingLinker;
				for (; keyBindingLinkerIt && otherKeyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext(), otherKeyBindingLinkerIt = otherKeyBindingLinkerIt->getNext()) {
					cint64 keyIdx = keyBindingLinkerIt->getData();
					cint64 otherKeyIdx = otherKeyBindingLinkerIt->getData();
					if (mBinding[keyIdx] != hasher.mBinding[otherKeyIdx]) {
						return false;
					}
				}
				if (keyBindingLinkerIt != otherKeyBindingLinkerIt) {
					return false;
				}
				return true;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
