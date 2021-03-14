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

#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged() : CMemoryPoolNewAllocationIncreasingContext(500, 500000, 2), CQtManagedRestrictedModificationHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>(this) {
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getRolePropagationProcessingRealizationIteratorData(CRealizationIndividualInstanceItemReference instItemRef) {
				COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*& itData = CQtManagedRestrictedModificationHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>::operator[](instItemRef);
				if (!itData) {
					itData = CObjectParameterizingAllocator< COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged, CContext* >::allocateAndConstructAndParameterize(getMemoryAllocationManager(), this);
					itData->initIteratorData(instItemRef);
				}
				return itData;
			}



			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getFillerInstanceItemCount() {
				return mFillerInstanceItemCount;
			}

			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getPropagationInstanceItemCount() {
				return CQtManagedRestrictedModificationHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>::size();
			}


			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getRealizationFinishedFillerInstanceItemCount() {
				return mRealizationFinishedFillerInstanceItemCount;
			}

			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getPropagationHandledInstanceItemCount() {
				return mPropagationHandledInstanceItemCount;
			}



			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getPropagatedInstanceItemCount() {
				return mPropagatedInstanceItemCount;
			}


			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::setFillerInstanceItemCount(cint64 count) {
				mFillerInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::setRealizationFinishedFillerInstanceItemCount(cint64 count) {
				mRealizationFinishedFillerInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::setPropagationHandledInstanceItemCount(cint64 count) {
				mPropagationHandledInstanceItemCount = count;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::setPropagatedInstanceItemCount(cint64 count) {
				mPropagatedInstanceItemCount = count;
				return this;
			}


			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::incPropagatedInstanceItemCount(cint64 count) {
				mPropagatedInstanceItemCount += count;
				return this;
			}



			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::incFillerInstanceItemCount(cint64 count) {
				mFillerInstanceItemCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::incRealizationFinishedFillerInstanceItemCount(cint64 count) {
				mRealizationFinishedFillerInstanceItemCount += count;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::incPropagationHandledInstanceItemCount(cint64 count) {
				mPropagationHandledInstanceItemCount += count;
				return this;
			}
			

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::incExpectedFillerAllPropagationItemCount(double count) {
				mExpectedFillerAllPropagationItemCount += count;
				return this;
			}

			double COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getExpectedFillerAllPropagationItemCount() {
				return mExpectedFillerAllPropagationItemCount;
			}

			double COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged::getExpectedFillerPerPropagationItemCount() {
				return mExpectedFillerAllPropagationItemCount / getPropagationInstanceItemCount();
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
