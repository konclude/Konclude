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

#include "COptimizedComplexVariableJoiningItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableJoiningItem::COptimizedComplexVariableJoiningItem(COptimizedComplexVariableCompositionItem* itemLeft, COptimizedComplexVariableCompositionItem* itemRight, const COptimizedComplexVariableJoiningBindingPositionMapping& positionMapping) : mPositionMapping(positionMapping), mLeftItemDep(itemLeft), mRightItemDep(itemRight) {
				mItemLeft = itemLeft;
				mItemRight = itemRight;
				mComputationDependentItemList.append(mItemLeft);
				mComputationDependentItemList.append(mItemRight);

				mJoiningHash = nullptr;

				mLeftSampleKeyCount = 0;
				mRightSampleKeyCount = 0;
				mLeftSampleInsertionCount = 0;
				mRightSampleInsertionCount = 0;

				mSamplingCompleted = false;


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



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
