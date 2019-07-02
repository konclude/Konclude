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

#include "CSignatureSatisfiableExpanderCacheContext.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CSignatureSatisfiableExpanderCacheContext::CSignatureSatisfiableExpanderCacheContext() {
					mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider();
					mMemMan = new CLimitedReserveMemoryPoolAllocationManager(mMemoryPoolProvider);
					mAddRelMemory = 0;
				}


				CSignatureSatisfiableExpanderCacheContext::~CSignatureSatisfiableExpanderCacheContext() {
					delete mMemMan;
					delete mMemoryPoolProvider;
				}


				CMemoryPoolAllocationManager* CSignatureSatisfiableExpanderCacheContext::getMemoryPoolAllocationManager() {
					return mMemMan;
				}


				CMemoryAllocationManager* CSignatureSatisfiableExpanderCacheContext::getMemoryAllocationManager() {
					return mMemMan;
				}

				CMemoryPoolProvider* CSignatureSatisfiableExpanderCacheContext::getMemoryPoolProvider() {
					return mMemoryPoolProvider;
				}

				cint64 CSignatureSatisfiableExpanderCacheContext::getMemoryConsumption() {
					return mAddRelMemory + mMemoryPoolProvider->getAllocatedReleaseDifferencePoolSize();
				}

				CSignatureSatisfiableExpanderCacheContext* CSignatureSatisfiableExpanderCacheContext::releaseTemporaryMemoryPools(CMemoryPool* memoryPools) {
					CMemoryPool* memoryPoolIt = memoryPools;
					while (memoryPoolIt) {
						mAddRelMemory += memoryPoolIt->getMemoryBlockSize();
						memoryPoolIt = memoryPoolIt->getNext();
					}
					mMemMan->releaseTemporaryMemoryPools(memoryPools);
					return this;
				}


			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
