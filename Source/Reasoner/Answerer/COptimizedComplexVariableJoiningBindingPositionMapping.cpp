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

#include "COptimizedComplexVariableJoiningBindingPositionMapping.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningBindingPositionMapping::COptimizedComplexVariableJoiningBindingPositionMapping(COptimizedComplexVariableCompositionItem* joiningVarItem, cint64 leftSize, cint64 rightSize) {
				mJoiningVarItem = joiningVarItem;
				mHashValue = 0;
				mHashValueCaluclated = false;
				mSelfSameVariableReduction = false;
				mVariableReduction = false;
				mLeftMapSize = leftSize;
				mRightMapSize = rightSize;
				mRightPosMapArray = new cint64[mRightMapSize];
				mLeftPosMapArray = new cint64[mLeftMapSize];
				mLeftKeyBindingLinker = nullptr;
				mRightKeyBindingLinker = nullptr;
				mLeftRemainingBindingLinker = nullptr;
				mRightRemainingBindingLinker = nullptr;
			}

			CXLinker<cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::copyKeyBindingLinker(CXLinker<cint64>* originalKeyBindingLinker) {
				CXLinker<cint64>* copiedKeyBindingLinker = nullptr;
				CXLinker<cint64>* lastRightKeyBindingLinker = nullptr;
				for (CXLinker<cint64>* keyBindingLinkerIt = originalKeyBindingLinker; keyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext()) {
					cint64 keyIdx = keyBindingLinkerIt->getData();
					CXLinker<cint64>* newKeyBindingLinker = new CXLinker<cint64>();
					newKeyBindingLinker->initLinker(keyIdx);
					if (lastRightKeyBindingLinker) {
						lastRightKeyBindingLinker->setNext(newKeyBindingLinker);
						lastRightKeyBindingLinker = newKeyBindingLinker;
					} else {
						lastRightKeyBindingLinker = newKeyBindingLinker;
						copiedKeyBindingLinker = newKeyBindingLinker;
					}
				}
				return copiedKeyBindingLinker;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping::~COptimizedComplexVariableJoiningBindingPositionMapping() {
				delete[] mRightPosMapArray;
				delete[] mLeftPosMapArray;

				deleteKeyBindingLinker(mLeftKeyBindingLinker);
				deleteKeyBindingLinker(mRightKeyBindingLinker);
				deleteKeyBindingLinker(mLeftRemainingBindingLinker);
				deleteKeyBindingLinker(mRightRemainingBindingLinker);

			}


			COptimizedComplexVariableJoiningBindingPositionMapping::COptimizedComplexVariableJoiningBindingPositionMapping(const COptimizedComplexVariableJoiningBindingPositionMapping& mapping) {
				mJoiningVarItem = mapping.mJoiningVarItem;
				mHashValue = mapping.mHashValue;
				mHashValueCaluclated = mapping.mHashValueCaluclated;
				mSelfSameVariableReduction = mapping.mSelfSameVariableReduction;
				mVariableReduction = mapping.mVariableReduction;
				mLeftMapSize = mapping.mLeftMapSize;
				mRightMapSize = mapping.mRightMapSize;
				mRightPosMapArray = new cint64[mRightMapSize];
				for (cint64 i = 0; i < mRightMapSize; ++i) {
					mRightPosMapArray[i] = mapping.mRightPosMapArray[i];
				}
				mLeftPosMapArray = new cint64[mLeftMapSize];
				for (cint64 i = 0; i < mLeftMapSize; ++i) {
					mLeftPosMapArray[i] = mapping.mLeftPosMapArray[i];
				}
				mLeftKeyBindingLinker = nullptr;
				mRightKeyBindingLinker = nullptr;

				mRightKeyBindingLinker = copyKeyBindingLinker(mapping.mRightKeyBindingLinker);
				mLeftKeyBindingLinker = copyKeyBindingLinker(mapping.mLeftKeyBindingLinker);

				mLeftRemainingBindingLinker = nullptr;
				mRightRemainingBindingLinker = nullptr;

				mRightRemainingBindingLinker = copyKeyBindingLinker(mapping.mRightRemainingBindingLinker);
				mLeftRemainingBindingLinker = copyKeyBindingLinker(mapping.mLeftRemainingBindingLinker);

			}



			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableJoiningBindingPositionMapping::getJoiningVariableItem() {
				return mJoiningVarItem;
			}

			bool COptimizedComplexVariableJoiningBindingPositionMapping::operator==(const COptimizedComplexVariableJoiningBindingPositionMapping& mapping) const {
				if (mSelfSameVariableReduction != mapping.mSelfSameVariableReduction) {
					return false;
				}
				if (mVariableReduction != mapping.mVariableReduction) {
					return false;
				}
				if (mJoiningVarItem != mapping.mJoiningVarItem) {
					return false;
				}
				if (mLeftMapSize != mapping.mLeftMapSize) {
					return false;
				}
				if (mRightMapSize != mapping.mRightMapSize) {
					return false;
				}
				for (cint64 i = 0; i < mLeftMapSize; ++i) {
					if (mLeftPosMapArray[i] != mapping.mLeftPosMapArray[i]) {
						return false;
					}
				}
				for (cint64 i = 0; i < mRightMapSize; ++i) {
					if (mRightPosMapArray[i] != mapping.mRightPosMapArray[i]) {
						return false;
					}
				}
				CXLinker<cint64>* leftKeyBindingLinkerIt = mLeftKeyBindingLinker;
				CXLinker<cint64>* leftKeyBindingLinkerIt2 = mapping.mLeftKeyBindingLinker;
				while (leftKeyBindingLinkerIt && leftKeyBindingLinkerIt2) {
					if (leftKeyBindingLinkerIt->getData() != leftKeyBindingLinkerIt2->getData()) {
						return false;
					}
					leftKeyBindingLinkerIt = leftKeyBindingLinkerIt->getNext();
					leftKeyBindingLinkerIt2 = leftKeyBindingLinkerIt2->getNext();
				}
				if (leftKeyBindingLinkerIt || leftKeyBindingLinkerIt2) {
					return false;
				}
				CXLinker<cint64>* rightKeyBindingLinkerIt = mRightKeyBindingLinker;
				CXLinker<cint64>* rightKeyBindingLinkerIt2 = mapping.mRightKeyBindingLinker;
				while (rightKeyBindingLinkerIt && rightKeyBindingLinkerIt2) {
					if (rightKeyBindingLinkerIt->getData() != rightKeyBindingLinkerIt2->getData()) {
						return false;
					}
					rightKeyBindingLinkerIt = rightKeyBindingLinkerIt->getNext();
					rightKeyBindingLinkerIt2 = rightKeyBindingLinkerIt2->getNext();
				}
				if (rightKeyBindingLinkerIt || rightKeyBindingLinkerIt2) {
					return false;
				}
				return true;
			}


			bool COptimizedComplexVariableJoiningBindingPositionMapping::isSelfSameVariableReduction() {
				return mSelfSameVariableReduction;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setSelfSameVariableReduction(bool selfSameVariableReduction) {
				mSelfSameVariableReduction = selfSameVariableReduction;
				return this;
			}

			bool COptimizedComplexVariableJoiningBindingPositionMapping::isVariableReduction() {
				return mVariableReduction;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setVariableReduction(bool variableReduction) {
				mVariableReduction = variableReduction;
				return this;
			}


			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::addBindingPositionMapping(bool left, cint64 varIdx, cint64 mappedVarIdx) {
				if (left) {
					mLeftPosMapArray[varIdx] = mappedVarIdx;
				} else {
					mRightPosMapArray[varIdx] = mappedVarIdx;
				}
				return this;
			}


			cint64 COptimizedComplexVariableJoiningBindingPositionMapping::getMappedBindingPosition(bool left, cint64 varIdx) {
				if (left) {
					return mLeftPosMapArray[varIdx];
				} else {
					return mRightPosMapArray[varIdx];
				}
			}


			cint64 COptimizedComplexVariableJoiningBindingPositionMapping::getHashValue() const {
				if (!mHashValueCaluclated) {
					mHashValue = 0;

					cint64 hashValue1 = mLeftMapSize;
					for (cint64 i = 0; i < mLeftMapSize; ++i) {
						hashValue1 += i * 13 + mLeftPosMapArray[i];
					}

					cint64 hashValue2 = mRightMapSize;
					for (cint64 i = 0; i < mRightMapSize; ++i) {
						hashValue2 += i * 13 + mRightPosMapArray[i];
					}

					cint64 hashValue3 = 0;
					for (CXLinker<cint64>* keyBindingLinkerIt = mLeftKeyBindingLinker; keyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext()) {
						cint64 keyIdx = keyBindingLinkerIt->getData();
						hashValue3 = hashValue3 * 13 + keyIdx;
					}

					cint64 hashValue4 = 0;
					for (CXLinker<cint64>* keyBindingLinkerIt = mRightKeyBindingLinker; keyBindingLinkerIt; keyBindingLinkerIt = keyBindingLinkerIt->getNext()) {
						cint64 keyIdx = keyBindingLinkerIt->getData();
						hashValue4 = hashValue4 * 13 + keyIdx;
					}

					mHashValue = 9973 * hashValue1 + 3701 * hashValue1 + 1759 * hashValue1 + 349 * hashValue1;

					mHashValue ^= (cint64)mJoiningVarItem;
					if (mSelfSameVariableReduction) {
						mHashValue = -mHashValue;
					}
					if (mVariableReduction) {
						mHashValue = -mHashValue;
					}
					mHashValueCaluclated = true;
				}
				return mHashValue;
			}


			CXLinker<cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::getLeftKeyBindingLinker() {
				return mLeftKeyBindingLinker;
			}

			CXLinker<cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::getRightKeyBindingLinker() {
				return mRightKeyBindingLinker;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setLeftKeyBindingLinker(CXLinker<cint64>* linker) {
				mLeftKeyBindingLinker = linker;
				return this;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setRightKeyBindingLinker(CXLinker<cint64>* linker) {
				mRightKeyBindingLinker = linker;
				return this;
			}

			cint64* COptimizedComplexVariableJoiningBindingPositionMapping::getRightPosMapArray() {
				return mRightPosMapArray;
			}

			cint64* COptimizedComplexVariableJoiningBindingPositionMapping::getLeftPosMapArray() {
				return mLeftPosMapArray;
			}



			CXLinker<cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::getLeftRemainingBindingLinker() {
				return mLeftRemainingBindingLinker;
			}

			CXLinker<cint64>* COptimizedComplexVariableJoiningBindingPositionMapping::getRightRemainingBindingLinker() {
				return mRightRemainingBindingLinker;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setLeftRemainingBindingLinker(CXLinker<cint64>* linker) {
				mLeftRemainingBindingLinker = linker;
				return this;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningBindingPositionMapping::setRightRemainingBindingLinker(CXLinker<cint64>* linker) {
				mRightRemainingBindingLinker = linker;
				return this;
			}

			void COptimizedComplexVariableJoiningBindingPositionMapping::deleteKeyBindingLinker(CXLinker<cint64>* keyBindingLinkerIt) {
				while (keyBindingLinkerIt) {
					CXLinker<cint64>* tmpKeyBindingLinkerIt = keyBindingLinkerIt;
					keyBindingLinkerIt = keyBindingLinkerIt->getNext();
					delete tmpKeyBindingLinkerIt;
				}
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
