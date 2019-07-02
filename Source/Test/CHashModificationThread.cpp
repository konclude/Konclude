/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CHashModificationThread.h"


namespace Konclude {

	namespace Test {



		CHashModificationThread::CHashModificationThread(CQtHash<cint64,cint64>** qtOriginalHashes, cint64 hashesCount, cint64 modificationCountPerStep, bool extendBaseHashes) {
			mModificationSteps = 0;
			mQtOriginalHashes = qtOriginalHashes;
			mQtModifiedHashes = 0;
			mMemoryPoolProvider = 0;
			mUseModifiedHashes = false;
			mHashesCount = hashesCount;
			mContextBase = 0;
			mMemoryPoolAllocationMan = 0;
			mCanceled = false;
			mModificationCountPerStep = modificationCountPerStep;
			mExtendBaseHashes = extendBaseHashes;
		}

		CHashModificationThread::CHashModificationThread(CQtManagedRestrictedModificationHash<cint64,cint64>** qtModifiedHashes, cint64 hashesCount, CMemoryPoolProvider* memoryPoolProvider, cint64 modificationCountPerStep, bool extendBaseHashes) {
			mModificationSteps = 0;
			mQtOriginalHashes = 0;
			mQtModifiedHashes = qtModifiedHashes;
			mMemoryPoolProvider = memoryPoolProvider;
			mUseModifiedHashes = true;
			mMemoryPoolAllocationMan = new CMemoryPoolAllocationManager(mMemoryPoolProvider);
			mContextBase = new CContextBase(mMemoryPoolAllocationMan);
			mHashesCount = hashesCount;
			mCanceled = false;
			mModificationCountPerStep = modificationCountPerStep;
			mExtendBaseHashes = extendBaseHashes;
		}

		CHashModificationThread::~CHashModificationThread() {
			quit();
			wait();
			delete mMemoryPoolAllocationMan;
			delete mContextBase;
		}


		void CHashModificationThread::startModifications() {
			start();
			moveToThread(this);
		}

		void CHashModificationThread::stopModifications() {
			mCanceled = true;
		}

		cint64 CHashModificationThread::getModificationStepsCount() {
			return mModificationSteps;
		}


		void CHashModificationThread::run() {
			qint64 modCountPerStep = mModificationCountPerStep;
			qint64 maxCacheValue = 100;

			while (!mCanceled) {
				qint64 modHashIndex = qrand() % mHashesCount;
				if (!mUseModifiedHashes) {
					CQtHash<cint64,cint64> *baseHash = mQtOriginalHashes[modHashIndex];
					CQtHash<cint64,cint64> *modHash = 0;
					if (mExtendBaseHashes) {
						modHash = new CQtHash<cint64,cint64>(*baseHash);
					} else {
						modHash = new CQtHash<cint64,cint64>();
					}
					for (cint64 i = 0; i < modCountPerStep; ++i) {
						qint64 modHashKey = qrand() % maxCacheValue;
						cint64 modHashValue = modHash->value(modHashKey,0);
						modHash->insert(modHashKey,++modHashValue);
					}
					delete modHash;
					++mModificationSteps;
				} else {
					CQtManagedRestrictedModificationHash<cint64,cint64> *baseHash = mQtModifiedHashes[modHashIndex];
					CQtManagedRestrictedModificationHash<cint64,cint64> *modHash = CObjectAllocator<CQtManagedRestrictedModificationHash<cint64,cint64> >::allocateAndConstruct(mMemoryPoolAllocationMan);
					if (mExtendBaseHashes) {
						modHash->init(baseHash,mContextBase);
					} else {
						modHash->init(mContextBase);
					}
					for (cint64 i = 0; i < modCountPerStep; ++i) {
						qint64 modHashKey = qrand() % maxCacheValue;
						cint64 modHashValue = modHash->value(modHashKey,0);
						modHash->insert(modHashKey,++modHashValue);
						modHashValue = modHash->value(modHashKey,0);
					}
					mMemoryPoolAllocationMan->releaseAllMemory();
					++mModificationSteps;
				}
			}
		}




	}; // end namespace Test

}; // end namespace Konclude
