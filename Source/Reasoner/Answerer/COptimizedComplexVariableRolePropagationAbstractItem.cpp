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

#include "COptimizedComplexVariableRolePropagationAbstractItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationAbstractItem::COptimizedComplexVariableRolePropagationAbstractItem(COptimizedComplexVariableCompositionItem* baseItem, CRole* role, bool inversed, cint64 propVarIdx) {
				mBaseItem = baseItem;
				mRole = role;
				mInversed = inversed;
				mPropagationVarIdx = propVarIdx;
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


			COptimizedComplexVariableIndividualMapping::const_iterator COptimizedComplexVariableRolePropagationAbstractItem::getVariableMappingPropagationIterator() {
				return mVariableMappingIterator;
			}

			COptimizedComplexVariableRolePropagationAbstractItem* COptimizedComplexVariableRolePropagationAbstractItem::setVariableMappingPropagationIterator(COptimizedComplexVariableIndividualMapping::const_iterator iterator) {
				mVariableMappingIterator = iterator;
				return this;
			}


			QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* COptimizedComplexVariableRolePropagationAbstractItem::getWaitingProcessedRealizationIteratorDataList() {
				return &mWaitingProcessedRealizationIteratorDataList;
			}

			QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* COptimizedComplexVariableRolePropagationAbstractItem::getInstanceItemRolePropagationInstanceIterationDataHash() {
				return &mInstItemRolePropInstIterationDataHash;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
