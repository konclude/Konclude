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

#include "COptimizedComplexVariableRolePropagationAbstractItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationAbstractItem::COptimizedComplexVariableRolePropagationAbstractItem(COptimizedComplexVariableCompositionItem* baseItem, CRole* role, bool inversed, cint64 propVarIdx) : COptimizedComplexVariableCompositionSingleDependenceItem(baseItem) {
				mBaseItem = baseItem;
				mRole = role;
				mInversed = inversed;
				mPropagationVarIdx = propVarIdx;
				mComputationDependentItemList.append(mBaseItem);

				mFillerInstanceItemCount = 0;
				mRealizationFinishedFillerInstanceItemCount = 0;
				mPropagationHandledInstanceItemCount = 0;
				mScheduledRealizationCount = 0;
				mPropagatedInstanceItemCount = 0;
				mExpectedFillerAllPropagationItemCount = 0;
			}


			COptimizedComplexVariableRolePropagationAbstractItem::~COptimizedComplexVariableRolePropagationAbstractItem() {
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableRolePropagationAbstractItem::getBaseItem() {
				return mBaseItem;
			}

			CRole* COptimizedComplexVariableRolePropagationAbstractItem::getRole() {
				return mRole;
			}

			bool COptimizedComplexVariableRolePropagationAbstractItem::getInversed() {
				return mInversed;
			}

			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getPropagationVariableIndex() {
				return mPropagationVarIdx;
			}




			QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* COptimizedComplexVariableRolePropagationAbstractItem::getWaitingProcessedRealizationIteratorDataList() {
				return &mWaitingProcessedRealizationIteratorDataList;
			}


			QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* COptimizedComplexVariableRolePropagationAbstractItem::getSchedulingRealizationIteratorDataList() {
				return &mSchedulingRealizationIteratorDataList;
			}


			QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* COptimizedComplexVariableRolePropagationAbstractItem::getInstanceItemRolePropagationInstanceIterationDataHash() {
				return &mInstItemRolePropInstIterationDataHash;
			}


			QList<COntologyProcessingRequirement*>* COptimizedComplexVariableRolePropagationAbstractItem::geSchedulingRealizationRequirementIteratorDataList() {
				return &mSchedulingRealizationRequirementIteratorDataList;
			}


			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getFillerInstanceItemCount() {
				return mFillerInstanceItemCount;
			}

			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getPropagationInstanceItemCount() {
				return mInstItemRolePropInstIterationDataHash.size();
			}


			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getRealizationFinishedFillerInstanceItemCount() {
				return mRealizationFinishedFillerInstanceItemCount;
			}

			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getPropagationHandledInstanceItemCount() {
				return mPropagationHandledInstanceItemCount;
			}


			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getScheduledRealizationCount() {
				return mScheduledRealizationCount;
			}


			cint64 COptimizedComplexVariableRolePropagationAbstractItem::getPropagatedInstanceItemCount() {
				return mPropagatedInstanceItemCount;
			}


			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::setFillerInstanceItemCount(cint64 count) {
				mFillerInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::setRealizationFinishedFillerInstanceItemCount(cint64 count) {
				mRealizationFinishedFillerInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::setPropagationHandledInstanceItemCount(cint64 count) {
				mPropagationHandledInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::setScheduledRealizationCount(cint64 count) {
				mScheduledRealizationCount = count;
				return this;
			}


			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incPropagatedInstanceItemCount(cint64 count) {
				mPropagatedInstanceItemCount += count;
				return this;
			}



			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incFillerInstanceItemCount(cint64 count) {
				mFillerInstanceItemCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incRealizationFinishedFillerInstanceItemCount(cint64 count) {
				mRealizationFinishedFillerInstanceItemCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incPropagationHandledInstanceItemCount(cint64 count) {
				mPropagationHandledInstanceItemCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incScheduledRealizationCount(cint64 count) {
				mScheduledRealizationCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::decScheduledRealizationCount(cint64 count) {
				mScheduledRealizationCount -= count;
				return this;
			}


			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::incExpectedFillerAllPropagationItemCount(double count) {
				mExpectedFillerAllPropagationItemCount += count;
				return this;
			}

			double COptimizedComplexVariableRolePropagationAbstractItem::getExpectedFillerAllPropagationItemCount() {
				return mExpectedFillerAllPropagationItemCount;
			}

			double COptimizedComplexVariableRolePropagationAbstractItem::getExpectedFillerPerPropagationItemCount() {
				return mExpectedFillerAllPropagationItemCount / getPropagationInstanceItemCount();
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
