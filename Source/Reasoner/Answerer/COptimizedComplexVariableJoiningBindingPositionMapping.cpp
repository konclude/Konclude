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

#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningBindingPositionMapping::COptimizedComplexVariableJoiningBindingPositionMapping(COptimizedComplexVariableCompositionItem* joiningVarItem) {
				mJoiningVarItem = joiningVarItem;
				mHashValue = 0;
				mHashValueCaluclated = false;
				mSelfSameVariableReduction = false;
			}


			QHash<cint64, cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::getJoiningItemPositionMapping() {
				return &mJoiningItemPositionMapping;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableJoiningBindingPositionMapping::getJoiningVariableItem() {
				return mJoiningVarItem;
			}

			bool COptimizedComplexVariableJoiningBindingPositionMapping::operator==(const COptimizedComplexVariableJoiningBindingPositionMapping& mapping) const {
				if (mSelfSameVariableReduction != mapping.mSelfSameVariableReduction) {
					return false;
				}
				if (mJoiningVarItem != mapping.mJoiningVarItem) {
					return false;
				}
				return mJoiningItemPositionMapping == mapping.mJoiningItemPositionMapping;
			}


			bool COptimizedComplexVariableJoiningBindingPositionMapping::isSelfSameVariableReduction() {
				return mSelfSameVariableReduction;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setSelfSameVariableReduction(bool selfSameVariableReduction) {
				mSelfSameVariableReduction = selfSameVariableReduction;
				return this;
			}


			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::addBindingPositionMapping(cint64 varIdx, cint64 mappedVarIdx) {
				mJoiningItemPositionMapping.insert(varIdx, mappedVarIdx);
				return this;
			}


			cint64 COptimizedComplexVariableJoiningBindingPositionMapping::getMappedBindingPosition(cint64 varIdx) {
				return mJoiningItemPositionMapping.value(varIdx);
			}


			cint64 COptimizedComplexVariableJoiningBindingPositionMapping::getHashValue() const {
				if (!mHashValueCaluclated) {
					mHashValue = 0;
					mHashValue += mJoiningItemPositionMapping.count();
					for (QHash<cint64, cint64>::const_iterator it = mJoiningItemPositionMapping.constBegin(), itEnd = mJoiningItemPositionMapping.constEnd(); it != itEnd; ++it) {
						cint64 idx = it.key();
						cint64 mapIdx = it.value();
						mHashValue += idx*13+mapIdx;
					}
					mHashValue ^= (cint64)mJoiningVarItem;
					if (mSelfSameVariableReduction) {
						mHashValue = -mHashValue;
					}
					mHashValueCaluclated = true;
				}
				return mHashValue;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
