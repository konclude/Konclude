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

#include "CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker() : CLinkerBase<COptimizedComplexVariableIndividualBindingsCardinalityLinker*, CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>(nullptr, nullptr) {
					mBatchBindingsCardinalityStartLinker = nullptr;
					mBatchBindingsCardinalityEndLinker = nullptr;

					mBatchCardinalityUpdateCurrentLinker = nullptr;
					mBatchCardinalityUpdateStartLinker = nullptr;
					mBatchCardinalityUpdateEndLinker = nullptr;

					mBatchContinueIterationDataLinker = nullptr;
				}



				bool CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::hasMoreLinkers() {
					return getNextBatchBindingsCardinalityLinker(false) || getNextCardinalityUpdateLinker(false);
				}


				CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::setBatchBindingsCardinalityLinkers(COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityStartLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityEndLinker) {
					setData(batchBindingsCardinalityStartLinker);
					mBatchBindingsCardinalityStartLinker = batchBindingsCardinalityStartLinker;
					mBatchBindingsCardinalityEndLinker = batchBindingsCardinalityEndLinker;
					return this;
				}


				CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::setBatchCardinalityUpdateLinkers(COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateStartLinker, COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateEndLinker) {
					mBatchCardinalityUpdateCurrentLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateStartLinker = batchCardinalityUpdateStartLinker;
					mBatchCardinalityUpdateEndLinker = batchCardinalityUpdateEndLinker;
					return this;
				}



				COptimizedComplexVariableIndividualBindingsCardinalityLinker* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::getNextBatchBindingsCardinalityLinker(bool moveNext) {
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* tmpBatchBindingsCardinalityCurrentLinker = getData();
					if (tmpBatchBindingsCardinalityCurrentLinker && moveNext) {
						if (getData() == mBatchBindingsCardinalityEndLinker) {
							setData(nullptr);
						} else {
							setData(getData()->getNext());
						}

					}
					return tmpBatchBindingsCardinalityCurrentLinker;
				}


				COptimizedComplexVariableIndividualUpdateCardinalityLinker* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::getNextCardinalityUpdateLinker(bool moveNext) {
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* tmpBatchCardinalityUpdateCurrentLinker = mBatchCardinalityUpdateCurrentLinker;
					if (tmpBatchCardinalityUpdateCurrentLinker && moveNext) {
						if (mBatchCardinalityUpdateCurrentLinker == mBatchCardinalityUpdateEndLinker) {
							mBatchCardinalityUpdateCurrentLinker = nullptr;
						} else {
							mBatchCardinalityUpdateCurrentLinker = mBatchCardinalityUpdateCurrentLinker->getNext();
						}
					}
					return tmpBatchCardinalityUpdateCurrentLinker;
				}

				COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::getNextContinueIterationData(bool moveNext) {
					CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* linker = mBatchContinueIterationDataLinker;
					if (mBatchContinueIterationDataLinker && moveNext) {
						mBatchContinueIterationDataLinker = mBatchContinueIterationDataLinker->getNext();
					}
					if (linker) {
						return linker->getData();
					}
					return nullptr;
				}

				CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker::addContinueIterationDataLinker(CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* batchContinueIterationDataLinker) {
					mBatchContinueIterationDataLinker = batchContinueIterationDataLinker->append(mBatchContinueIterationDataLinker);
					return this;
				}


			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
