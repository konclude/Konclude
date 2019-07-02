/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CComputedConsequencesCacheContext.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CComputedConsequencesCacheContext::CComputedConsequencesCacheContext() {
					mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
					mMemMan = new CLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider);
					mAddRelMemory = 0;
					mConLinker = nullptr;
				}


				CComputedConsequencesCacheContext::~CComputedConsequencesCacheContext() {
					delete mMemMan;
					delete mMemoryPoolProvider;
				}


				CMemoryPoolAllocationManager* CComputedConsequencesCacheContext::getMemoryPoolAllocationManager() {
					return mMemMan;
				}


				CMemoryAllocationManager* CComputedConsequencesCacheContext::getMemoryAllocationManager() {
					return mMemMan;
				}

				CMemoryPoolProvider* CComputedConsequencesCacheContext::getMemoryPoolProvider() {
					return mMemoryPoolProvider;
				}

				cint64 CComputedConsequencesCacheContext::getMemoryConsumption() {
					return mAddRelMemory + mMemoryPoolProvider->getAllocatedReleaseDifferencePoolSize();
				}

				CComputedConsequencesCacheContext* CComputedConsequencesCacheContext::releaseTemporaryMemoryPools(CMemoryPool* memoryPools) {
					CMemoryPool* memoryPoolIt = memoryPools;
					while (memoryPoolIt) {
						mAddRelMemory += memoryPoolIt->getMemoryBlockSize();
						memoryPoolIt = memoryPoolIt->getNext();
					}
					mMemMan->releaseTemporaryMemoryPools(memoryPools);
					return this;
				}


				CSortedNegLinker<CConcept*>* CComputedConsequencesCacheContext::createConceptLinker(bool create) {
					CSortedNegLinker<CConcept*>* conLinker = mConLinker;
					if (!conLinker && create) {
						conLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
					}
					conLinker->setNext(nullptr);
					if (mConLinker) {
						mConLinker = mConLinker->getNext();
					}
					return conLinker;
				}


				CComputedConsequencesCacheContext* CComputedConsequencesCacheContext::addConceptLinker(CSortedNegLinker<CConcept*>* linker) {
					if (linker) {
						linker->setNext(nullptr);
						linker = linker->append(mConLinker);
					}
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
