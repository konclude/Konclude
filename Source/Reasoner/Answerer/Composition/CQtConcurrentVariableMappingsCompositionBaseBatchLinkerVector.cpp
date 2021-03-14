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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTOR_CPP
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTOR_CPP


#include "CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				template<class T>
				CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector(cint64 size) : QVector<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>*>(size), CMemoryPoolNewAllocationIncreasingContext(5000, 500000, 2) {
					mNextIndex = 0;
					mSize = size;
				}


				template<class T>
				CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector() {
					mNextIndex = 0;
					mSize = 0;
				}




				template<class T>
				CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>* CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::getVectorLinkerData(cint64 index, bool createIfNotExists) {
					CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>*& data = QVector<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>*>::operator[](index);
					if (!data && createIfNotExists) {
						data = CObjectParameterizingAllocator< CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>, CMemoryAllocationManager* >::allocateAndConstructAndParameterize(&mMemMan, &mMemMan);
					}
					return data;
				}


				template<class T>
				T* CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::createBatchLinker() {
					T* linker = CObjectAllocator<T>::allocateAndConstruct(&mMemMan);
					return linker;
				}





				template<class T>
				CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>* CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::addBatchLinker(cint64 index, T* linker) {
					getVectorLinkerData(index)->addBatchLinker(linker);
					return this;
				}

				template<class T>
				cint64 CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::getNextIndex(bool moveNext) {
					cint64 nextIndex = mNextIndex;
					if (moveNext) {
						++mNextIndex;
						if (mNextIndex >= mSize) {
							mNextIndex = 0;
						}
					}
					return nextIndex;
				}

				template<class T>
				cint64 CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T>::getIndexSize() {
					return mSize;
				}


			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTOR_CPP

