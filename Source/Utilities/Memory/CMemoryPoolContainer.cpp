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

#include "CMemoryPoolContainer.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CMemoryPoolContainer::CMemoryPoolContainer(CMemoryPool* memoryPool) {
				mMemoryPool = memoryPool;
				mMemoryPoolCount = 0;
				mMemorySize = 0;
				if (mMemoryPool) {
					mMemoryPoolCount = mMemoryPool->getCount();
					for (CMemoryPool* memoryPoolIt = mMemoryPool; memoryPoolIt; memoryPoolIt = memoryPoolIt->getNext()) {
						mMemorySize += memoryPoolIt->getMemoryBlockSize();
					}
				}
			}

			CMemoryPoolContainer::~CMemoryPoolContainer() {
			}

			CMemoryPoolContainer* CMemoryPoolContainer::appendMemoryPool(CMemoryPool* memoryPool) {
				if (memoryPool) {
					mMemoryPoolCount += memoryPool->getCount();
					for (CMemoryPool* memoryPoolIt = memoryPool; memoryPoolIt; memoryPoolIt = memoryPoolIt->getNext()) {
						mMemorySize += memoryPoolIt->getMemoryBlockSize();
					}
					memoryPool->getLastListLink()->setNextMemoryPool(mMemoryPool);
					mMemoryPool = memoryPool;
				}
				return this;
			}

			CMemoryPool* CMemoryPoolContainer::getMemoryPools() const {
				return mMemoryPool;
			}

			CMemoryPool* CMemoryPoolContainer::takeMemoryPools() {
				CMemoryPool* tmpMemoryPool = mMemoryPool;
				mMemoryPool = nullptr;
				mMemoryPoolCount = 0;
				mMemorySize = 0;
				return tmpMemoryPool;
			}

			cint64 CMemoryPoolContainer::getMemoryPoolCount() {
				return mMemoryPoolCount;
			}

			cint64 CMemoryPoolContainer::getMemorySize() {
				return mMemorySize;
			}

		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

