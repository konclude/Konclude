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

#include "COptimizedComplexVariableJoiningWithRemainingHasher.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningWithRemainingHasher::COptimizedComplexVariableJoiningWithRemainingHasher(TIndividualInstanceItemDataBinding* binding, CXLinker<cint64>* keyBindingLinker, CXLinker<cint64>* remainingBindingLinker)
				: COptimizedComplexVariableJoiningHasher(binding, keyBindingLinker) {
				mRemainingBindingLinker = remainingBindingLinker;
				calculateRemainingHashValue();
			}


			cint64 COptimizedComplexVariableJoiningWithRemainingHasher::getRemainingHashValue() const {
				return mRemainingHashValue;
			}


			COptimizedComplexVariableJoiningWithRemainingHasher* COptimizedComplexVariableJoiningWithRemainingHasher::calculateRemainingHashValue() {
				mRemainingHashValue = 0;
				for (CXLinker<cint64>* keyBindingLinkerIt = mRemainingBindingLinker; keyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext()) {
					cint64 nextKeyIdx = keyBindingLinkerIt->getData();
					cint64 value = mBinding[nextKeyIdx].getHashValue();
					mRemainingHashValue = (value + mRemainingHashValue)*13+1;
				}
				return this;
			}

			CXLinker<cint64>* COptimizedComplexVariableJoiningWithRemainingHasher::getRemainingBindingLinker() const {
				return mRemainingBindingLinker;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
