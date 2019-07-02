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

#include "CMemoryAccessThread.h"


namespace Konclude {

	namespace Test {



		CMemoryAccessThread::CMemoryAccessThread(cint64 readWriteRatio, int* accessArray, cint64 arraySize, bool atomicOperations, cint64 minWriteIndex, cint64 maxWriteIndex, CMemoryPoolProvider* memoryPoolProvider) {
			mModificationSteps = 0;

			mContextBase = 0;
			mMemoryPoolAllocationMan = 0;
			mCanceled = false;

			mMemoryAccessArray = accessArray;
			mReadWriteRatio = readWriteRatio;
			mArraySize = arraySize;

			mMemoryPoolProvider = memoryPoolProvider;
			mMemoryPoolAllocationMan = new CMemoryPoolAllocationManager(mMemoryPoolProvider);
			mContextBase = new CContextBase(mMemoryPoolAllocationMan);

			mAtomicIncrements = atomicOperations;

			mLocalWriteStep = 3;
			mLocalWrite = true;

			mMinWriteIndex = minWriteIndex;
			mMaxWriteIndex = maxWriteIndex;

			mLocalWriteIndex = mMinWriteIndex;
		}

		CMemoryAccessThread::~CMemoryAccessThread() {
			quit();
			wait();
			delete mContextBase;
			delete mMemoryPoolAllocationMan;
		}


		void CMemoryAccessThread::startModifications() {
			start();
			moveToThread(this);
		}

		void CMemoryAccessThread::stopModifications() {
			mCanceled = true;
		}

		cint64 CMemoryAccessThread::getModificationStepsCount() {
			return mModificationSteps;
		}


		void CMemoryAccessThread::run() {
			while (!mCanceled) {
				int newValue = 0;
				for (cint64 i = 0; i < mReadWriteRatio; ++i) {
					qint64 arrayIndex = qrand() % mArraySize;
					int arrayValue = mMemoryAccessArray[arrayIndex];
					newValue += arrayValue;
				}
				qint64 arrayIndex = 0;
				if (!mLocalWrite) {
					arrayIndex = qrand() % (mArraySize-100);
				} else {
					mLocalWriteIndex += mLocalWriteStep; 
					if (mLocalWriteIndex >= mMaxWriteIndex) {
						mLocalWriteIndex = mMinWriteIndex;
					}
					arrayIndex = mLocalWriteIndex;
				}
				if (mAtomicIncrements) {
					QAtomicInt* atomicInt = (QAtomicInt*)&(mMemoryAccessArray[arrayIndex]);
					atomicInt->fetchAndAddOrdered(newValue);
				} else {
					mMemoryAccessArray[arrayIndex] += newValue;
				}
				++mModificationSteps;
			}
		}




	}; // end namespace Test

}; // end namespace Konclude
