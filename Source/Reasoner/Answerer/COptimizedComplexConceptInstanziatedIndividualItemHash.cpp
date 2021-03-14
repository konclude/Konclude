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

#include "COptimizedComplexConceptInstanziatedIndividualItemHash.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexConceptInstanziatedIndividualItemHash::COptimizedComplexConceptInstanziatedIndividualItemHash() {
				mAddedLinker = nullptr;
				mTmpLinker = nullptr;
				mMemManContext = new CMemoryPoolNewAllocationIncreasingContext(500, 500000, 2);
				mIndiRefLinkerHash = CObjectParameterizingAllocator< CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>, CContext* >::allocateAndConstructAndParameterize(getInstancesMemoryAllocationManager(), mMemManContext);
			}

			COptimizedComplexConceptInstanziatedIndividualItemHash::~COptimizedComplexConceptInstanziatedIndividualItemHash() {
				if (mMemManContext) {
					CMemoryPool* memPools = mMemManContext->getMemoryPools();
					CMemoryPool* memPoolsIt = memPools;
					while (memPoolsIt) {
						CMemoryPool* tmpMemPool = memPoolsIt;
						memPoolsIt = memPoolsIt->getNext();
						delete[] tmpMemPool->getMemoryBlockData();
						delete tmpMemPool;
					}
					delete mMemManContext;
					mMemManContext = nullptr;
				}
			}


			CMemoryAllocationManager* COptimizedComplexConceptInstanziatedIndividualItemHash::getInstancesMemoryAllocationManager() {
				return mMemManContext->getMemoryAllocationManager();
			}


			COptimizedComplexConceptInstanziatedIndividualItemHash* COptimizedComplexConceptInstanziatedIndividualItemHash::addRealizationIndividualInstanceItemReference(const CRealizationIndividualInstanceItemReference& itemRef) {
				if (!mTmpLinker) {
					mTmpLinker = CObjectAllocator<COptimizedComplexConceptInstanziatedIndividualItemLinker>::allocateAndConstruct(getInstancesMemoryAllocationManager());
					mTmpLinker->initLinker(itemRef);
				} else {
					mTmpLinker->initLinker(itemRef);
				}
				COptimizedComplexConceptInstanziatedIndividualItemLinker*& insertedLinker = mIndiRefLinkerHash->operator[](mTmpLinker);
				if (!insertedLinker) {
					insertedLinker = mTmpLinker;
					mAddedLinker = mTmpLinker->append(mAddedLinker);
					mTmpLinker = nullptr;
				}
				return this;
			}


			CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>* COptimizedComplexConceptInstanziatedIndividualItemHash::getIndividualInstanceItemReferenceLinkerHash() {
				return mIndiRefLinkerHash;
			}

			COptimizedComplexConceptInstanziatedIndividualItemHash* COptimizedComplexConceptInstanziatedIndividualItemHash::remove(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker) {
				mIndiRefLinkerHash->remove(linker);
				return this;
			}

			COptimizedComplexConceptInstanziatedIndividualItemLinker* COptimizedComplexConceptInstanziatedIndividualItemHash::getAddedRealizationIndividualInstanceItemReferenceLinker() {
				return mAddedLinker;
			}

			cint64 COptimizedComplexConceptInstanziatedIndividualItemHash::getMemoryConsumption() {
				return mMemManContext->getMemorySize();
			}

			cint64 COptimizedComplexConceptInstanziatedIndividualItemHash::size() {
				return mIndiRefLinkerHash->size();
			}

			bool COptimizedComplexConceptInstanziatedIndividualItemHash::isEmpty() {
				return mIndiRefLinkerHash->isEmpty();
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
