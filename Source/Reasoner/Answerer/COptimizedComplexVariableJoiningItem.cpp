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

#include "COptimizedComplexVariableJoiningItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningItem::COptimizedComplexVariableJoiningItem(COptimizedComplexVariableCompositionItem* itemLeft, COptimizedComplexVariableCompositionItem* itemRight, const COptimizedComplexVariableJoiningBindingPositionMapping& positionMapping) : mPositionMapping(positionMapping), mLeftItemDep(itemLeft), mRightItemDep(itemRight) {
				mItemLeft = itemLeft;
				mItemRight = itemRight;
				mComputationDependentItemList.append(mItemLeft);
				mComputationDependentItemList.append(mItemRight);
				mItemLeft->addComputationSuccessorItem(this);
				mItemRight->addComputationSuccessorItem(this);
				mComputationItemDepth = qMax(mItemLeft->getComputationItemDepth(), mItemRight->getComputationItemDepth()) + 1;

				mJoiningHash = nullptr;

				mLeftSampleKeyCount = 0;
				mRightSampleKeyCount = 0;
				mLeftSampleInsertionCount = 0;
				mRightSampleInsertionCount = 0;

				mSamplingCompleted = false;

				mLeftSplitPosition = 0;
				mRightSplitPosition = 0;
				mLeftSplitCount = 0;
				mRightSplitCount = 0;


				mInsertionSideDecided = false;
				mInsertionSideLeft = false;
			}


			COptimizedComplexVariableJoiningItem::~COptimizedComplexVariableJoiningItem() {
			}

			COptimizedComplexVariableJoiningItem::COMPOSITION_TYPE COptimizedComplexVariableJoiningItem::getCompositionType() {
				return JOINING;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableJoiningItem::getLeftItem() {
				return mItemLeft;
			}

			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableJoiningItem::getRightItem() {
				return mItemRight;
			}

			COptimizedComplexVariableCompositionItemDependence* COptimizedComplexVariableJoiningItem::getLeftItemDependence() {
				return &mLeftItemDep;
			}

			COptimizedComplexVariableCompositionItemDependence* COptimizedComplexVariableJoiningItem::getRightItemDependence() {
				return &mRightItemDep;
			}

			COptimizedComplexVariableJoiningBindingPositionMapping* COptimizedComplexVariableJoiningItem::getPositionMapping() {
				return &mPositionMapping;
			}

			QVector<COptimizedComplexVariableJoiningHashMemoryManaged*>& COptimizedComplexVariableJoiningItem::getJoiningHashVector() {
				return mJoiningHashVector;
			}


			COptimizedComplexVariableJoiningHash*& COptimizedComplexVariableJoiningItem::getJoiningHash() {
				return mJoiningHash;
			}


			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::resetJoiningHash() {
				if (mJoiningHash) {
					delete mJoiningHash;
				}
				mJoiningHash = nullptr;

				for (cint64 i = 0; i < mJoiningHashVector.size(); ++i) {
					COptimizedComplexVariableJoiningHashMemoryManaged*& hash = mJoiningHashVector[i];
					if (hash) {
						CMemoryPool* memPoolIt = hash->getMemoryPools();
						while (memPoolIt) {
							CMemoryPool* tmpMemPool = memPoolIt;
							memPoolIt = memPoolIt->getNext();
							delete[] tmpMemPool->getMemoryBlockData();
							delete tmpMemPool;
						}
						hash = nullptr;
					}
				}

				return this;
			}


			cint64& COptimizedComplexVariableJoiningItem::getLeftSampleKeyCount() {
				return mLeftSampleKeyCount;
			}

			cint64& COptimizedComplexVariableJoiningItem::getRightSampleKeyCount() {
				return mRightSampleKeyCount;
			}

			cint64& COptimizedComplexVariableJoiningItem::getLeftSampleInsertionCount() {
				return mLeftSampleInsertionCount;
			}

			cint64& COptimizedComplexVariableJoiningItem::getRightSampleInsertionCount() {
				return mRightSampleInsertionCount;
			}

			bool COptimizedComplexVariableJoiningItem::isSamplingCompleted() {
				return mSamplingCompleted;
			}

			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::setSamplingCompleted(bool completed) {
				mSamplingCompleted = completed;
				return this;
			}


			bool COptimizedComplexVariableJoiningItem::isInsertionSideDecided() {
				return mInsertionSideDecided;
			}

			bool COptimizedComplexVariableJoiningItem::isInsertionSideLeft() {
				return mInsertionSideLeft;
			}

			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::setInsertionSideDecided(bool decided) {
				mInsertionSideDecided = decided;
				return this;
			}

			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::setInsertionSideLeft(bool left) {
				mInsertionSideLeft = left;
				return this;
			}

			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::resetSampling() {
				mLeftSampleKeyCount = 0;
				mRightSampleKeyCount = 0;
				mLeftSampleInsertionCount = 0;
				mRightSampleInsertionCount = 0;
				return this;
			}


			COptimizedComplexVariableJoiningItem* COptimizedComplexVariableJoiningItem::createSplitComputationItem() {
				if (!mSplitItem) {
					COptimizedComplexVariableJoiningItem* splitItem = new COptimizedComplexVariableJoiningItem(mItemLeft, mItemRight, mPositionMapping);
					splitItem->mSamplingCompleted = true;
					splitItem->mInsertionSideDecided = true;
					splitItem->mInsertionSideLeft = mInsertionSideLeft;
					mSplitItem = splitItem;
					return splitItem;
				} else {
					return (COptimizedComplexVariableJoiningItem*)mSplitItem;
				}
			}


			cint64& COptimizedComplexVariableJoiningItem::getLeftSplitPosition() {
				return mLeftSplitPosition;
			}

			cint64& COptimizedComplexVariableJoiningItem::getRightSplitPosition() {
				return mRightSplitPosition;
			}


			cint64& COptimizedComplexVariableJoiningItem::getLeftSplitCount() {
				return mLeftSplitCount;
			}

			cint64& COptimizedComplexVariableJoiningItem::getRightSplitCount() {
				return mRightSplitCount;
			}

			bool COptimizedComplexVariableJoiningItem::clearComputation() {
				COptimizedComplexVariableCompositionItem::clearComputation();
				resetJoiningHash();
				mRightItemDep.reset();
				mLeftItemDep.reset();
				return true;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
