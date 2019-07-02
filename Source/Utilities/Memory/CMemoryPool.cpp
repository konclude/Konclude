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

#include "CMemoryPool.h"


namespace Konclude {

	namespace Utilities {

		namespace Memory {


			CMemoryPool::CMemoryPool() : CLinkerBase<CMemoryPool*,CMemoryPool>(this,nullptr) {
				mMemoryBlockBegin = 0;
				mMemoryBlockSize = 0;
				mMemoryBlockPointer = 0;
			}

			CMemoryPool::~CMemoryPool() {
			}

			char* CMemoryPool::getMemoryBlockData() {
				return mMemoryBlockBegin;
			}

			cint64 CMemoryPool::getMemoryBlockSize() {
				return mMemoryBlockSize;
			}

			char* CMemoryPool::getMemoryBlockPointer() {
				return mMemoryBlockPointer;
			}

			char* CMemoryPool::getMemoryBlockEnd() {
				return mMemoryBlockEnd;
			}

			CMemoryPool* CMemoryPool::setMemoryBlockData(char* memoryBlock, cint64 memoryBlockSize) {
				mMemoryBlockBegin = memoryBlock;
				mMemoryBlockSize = memoryBlockSize;
				mMemoryBlockEnd = mMemoryBlockBegin + mMemoryBlockSize;
				mMemoryBlockPointer = mMemoryBlockBegin;
				return this;
			}

			CMemoryPool* CMemoryPool::setMemoryBlockPointer(char* memoryBlockPointer) {
				mMemoryBlockPointer = memoryBlockPointer;
				return this;
			}

			CMemoryPool* CMemoryPool::incMemoryBlockPointer(cint64 pointerInc) {
				mMemoryBlockPointer = (char*)((cint64)mMemoryBlockPointer + pointerInc);
				return this;
			}


			CMemoryPool* CMemoryPool::resetMemoryBlockPointer() {
				mMemoryBlockPointer = mMemoryBlockBegin;
				return this;
			}

			CMemoryPool* CMemoryPool::getNextMemoryPool() {
				return getNext();
			}

			CMemoryPool* CMemoryPool::setNextMemoryPool(CMemoryPool* nextMemoryPool) {
				CLinkerBase<CMemoryPool*,CMemoryPool>::setNext(nextMemoryPool);
				return this;
			}


		}; // end namespace Memory

	}; // end namespace Utilities

}; // end namespace Konclude

