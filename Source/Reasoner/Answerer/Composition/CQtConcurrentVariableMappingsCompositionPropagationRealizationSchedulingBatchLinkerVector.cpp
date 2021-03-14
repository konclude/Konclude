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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_CPP
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_CPP


#include "CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				template<class T>
				CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector(cint64 size) : CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>(size) {
					mFirstRealizationSchedulingIterationDataLinker = nullptr;
					mLastRealizationSchedulingIterationDataLinker = nullptr;
				}


				template<class T>
				CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector() {
					mFirstRealizationSchedulingIterationDataLinker = nullptr;
					mLastRealizationSchedulingIterationDataLinker = nullptr;
				}


				template<class T>
				CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::getFirstRealizationSchedulingIterationDataLinker() const {
					return mFirstRealizationSchedulingIterationDataLinker;
				}



				template<class T>
				CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::getLastRealizationSchedulingIterationDataLinker() const {
					return mLastRealizationSchedulingIterationDataLinker;
				}



				template<class T>
				CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>* CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::addRealizationSchedulingIterationData(COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iterationData) {
					CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* linker = CObjectAllocator< CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*> >::allocateAndConstruct(CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::getMemoryAllocationManager());
					linker->initLinker(iterationData);
					mFirstRealizationSchedulingIterationDataLinker = linker->append(mFirstRealizationSchedulingIterationDataLinker);
					if (!mLastRealizationSchedulingIterationDataLinker) {
						mLastRealizationSchedulingIterationDataLinker = linker;
					}
					return this;
				}


				
				template<class T>
				CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>* CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>::mergeRealizationSchedulingIterationDataLinkers(const CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>& reduceVec) {

					CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* reduceFirstRealizationSchedulingIterationDataLinker = reduceVec.mFirstRealizationSchedulingIterationDataLinker;
					CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* reduceLastRealizationSchedulingIterationDataLinker = reduceVec.mLastRealizationSchedulingIterationDataLinker;

					if (reduceFirstRealizationSchedulingIterationDataLinker) {
						if (mLastRealizationSchedulingIterationDataLinker) {
							mLastRealizationSchedulingIterationDataLinker->setNext(reduceFirstRealizationSchedulingIterationDataLinker);
						} else {
							mFirstRealizationSchedulingIterationDataLinker = reduceFirstRealizationSchedulingIterationDataLinker;
						}
						mLastRealizationSchedulingIterationDataLinker = reduceLastRealizationSchedulingIterationDataLinker;
					}

					return this;
				}



			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_CPP

