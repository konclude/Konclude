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

#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData() {
				mRealizationIterator = nullptr;
				mRealizationScheduled = false;
				mExpectedFillerCount = 0;
			}



			QList<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::getVariableMappingPropagationIteratorList() {
				return &mVariableMappingIteratorList;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::addVariableMappingPropagationIterator(COptimizedComplexVariableIndividualBindingsCardinalityLinker* iterator) {
				mVariableMappingIteratorList.append(iterator);
				return this;
			}





			QList<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::getCardinalityUpdatePropagationIteratorList() {
				return &mUpdateCardinalityIteratorList;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::addCardinalityUpdatePropagationIterator(COptimizedComplexVariableIndividualUpdateCardinalityLinker* iterator) {
				mUpdateCardinalityIteratorList.append(iterator);
				return this;
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::getRealizationIterator() {
				return mRealizationIterator;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::setRealizationIterator(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				mRealizationIterator = iterator;
				return this;
			}

			QSet<CRealizationIndividualInstanceItemReference>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::getPropagationInstanceItemSet() {
				return &mInstItemRolePropInstItemSet;
			}

			bool COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::isRealizationScheduled() {
				return mRealizationScheduled;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::setRealizationScheduled(bool scheduled) {
				mRealizationScheduled = scheduled;
				return this;
			}

			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::getExpectedFillerCount() {
				return mExpectedFillerCount;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData::setExpectedFillerCount(cint64 count) {
				mExpectedFillerCount = count;
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
