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

#include "CConcurrentMemoryAccessTester.h"


namespace Konclude {

	namespace Test {



		CConcurrentMemoryAccessTester::CConcurrentMemoryAccessTester() {
		}


		CConcurrentMemoryAccessTester::~CConcurrentMemoryAccessTester() {
		}


		void CConcurrentMemoryAccessTester::generateTestingStructure(cint64 arraySize, cint64 readWriteRatio, bool atomicOperations, cint64 threadCount) {
			mArraySize = arraySize;
			mReadWriteRatio = readWriteRatio;
			mThreadCount = threadCount;
			mAtomicOperations = atomicOperations;

			mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider(1024*10-20 /*~ 10 KByte*/);

			mMemoryArray = new int[mArraySize];

			mSharedWrite = false;

			cint64 threadSize = arraySize / mThreadCount;
			cint64 nextStart = 0;

			if (mSharedWrite) {
				threadSize = mThreadCount;
			}

			for (qint64 i = 0; i < mThreadCount; ++i) {
				CMemoryAccessThread *modThread = 0;
				modThread = new CMemoryAccessThread(mReadWriteRatio,mMemoryArray,mArraySize,mAtomicOperations,nextStart,nextStart+threadSize,mMemoryPoolProvider);
				if (!mSharedWrite) {
					nextStart += threadSize;
				}
				mModThreadList.append(modThread);
			}
		}

		void CConcurrentMemoryAccessTester::startHashModificationTest() {
			cForeach (CMemoryAccessThread* modThread, mModThreadList) {
				modThread->startModifications();
			}
		}

		void CConcurrentMemoryAccessTester::stopHashModificationTest() {
			cForeach (CMemoryAccessThread* modThread, mModThreadList) {
				modThread->stopModifications();
			}
			cForeach (CMemoryAccessThread* modThread, mModThreadList) {
				modThread->quit();
			}
		}

		void CConcurrentMemoryAccessTester::destroyTestingStructure() {
			cDeleteAll(mModThreadList);
			delete mMemoryPoolProvider;
			delete [] mMemoryArray;
			mModThreadList.clear();
		}


		cint64 CConcurrentMemoryAccessTester::getModificationStepsCount() {
			cint64 totalModStepsCount = 0;
			cForeach (CMemoryAccessThread* modThread, mModThreadList) {
				totalModStepsCount += modThread->getModificationStepsCount();
			}
			return totalModStepsCount;
		}



	}; // end namespace Test

}; // end namespace Konclude
