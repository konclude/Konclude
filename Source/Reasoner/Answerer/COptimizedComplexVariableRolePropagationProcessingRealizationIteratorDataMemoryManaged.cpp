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

#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged(CContext* context) {
				mContext = context;
			}



			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::initIteratorData(const CRealizationIndividualInstanceItemReference& propagationInstanceItemReference) {
				mInstItemRef = propagationInstanceItemReference;
				mRealizationIterator = nullptr;
				mRealizationScheduled = false;
				mRealizationIterationInitialized = false;
				mExpectedFillerCount = 0;
				mVariableMappingIteratorLinker = nullptr;
				mUpdateCardinalityIteratorLinker = nullptr;
				mInstItemRolePropInstItemLinker = nullptr;
				mVariableMappingIteratorCount = 0;
				mInstItemRolePropInstItemCount = 0;
				return this;
			}

			CRealizationIndividualInstanceItemReference COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getPropagationInstanceItemReference() {
				return mInstItemRef;
			}



			CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getVariableMappingPropagationIteratorLinker() {
				return mVariableMappingIteratorLinker;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::addVariableMappingPropagationIterator(COptimizedComplexVariableIndividualBindingsCardinalityLinker* iterator) {
				CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* linker = CObjectAllocator< CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*> >::allocateAndConstruct(mContext->getMemoryAllocationManager());
				linker->initLinker(iterator);
				mVariableMappingIteratorLinker = linker->append(mVariableMappingIteratorLinker);
				++mVariableMappingIteratorCount;
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::clearVariableMappingPropagationIterator() {
				mVariableMappingIteratorLinker = nullptr;
				return this;
			}


			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getVariableMappingIteratorCount() {
				return mVariableMappingIteratorCount;
			}


			CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getCardinalityUpdatePropagationIteratorLinker() {
				return mUpdateCardinalityIteratorLinker;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::addCardinalityUpdatePropagationIterator(COptimizedComplexVariableIndividualUpdateCardinalityLinker* iterator) {
				CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* linker = CObjectAllocator< CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*> >::allocateAndConstruct(mContext->getMemoryAllocationManager());
				linker->initLinker(iterator);
				mUpdateCardinalityIteratorLinker = linker->append(mUpdateCardinalityIteratorLinker);
				return this;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::clearCardinalityUpdatePropagationIterator() {
				mUpdateCardinalityIteratorLinker = nullptr;
				return this;
			}



			CXLinker<CRealizationIndividualInstanceItemReference>* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getPropagationInstanceItemLinker() {
				return mInstItemRolePropInstItemLinker;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::addPropagationInstanceItemLinker(CRealizationIndividualInstanceItemReference indiInstItemRef) {
				CXLinker<CRealizationIndividualInstanceItemReference>* linker = CObjectAllocator< CXLinker<CRealizationIndividualInstanceItemReference> >::allocateAndConstruct(mContext->getMemoryAllocationManager());
				linker->initLinker(indiInstItemRef);
				mInstItemRolePropInstItemLinker = linker->append(mInstItemRolePropInstItemLinker);
				++mInstItemRolePropInstItemCount;
				return this;
			}

			cint64 COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::getPropagationInstanceItemCount() {
				return mInstItemRolePropInstItemCount;
			}







			bool COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::isRealizationIterationInitialized() {
				return mRealizationIterationInitialized;
			}

			COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged::setRealizationIterationInitialized(bool initialized) {
				mRealizationIterationInitialized = initialized;
				return this;
			}




		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
