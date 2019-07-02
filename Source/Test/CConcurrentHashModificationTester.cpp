/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CConcurrentHashModificationTester.h"


namespace Konclude {

	namespace Test {



		CConcurrentHashModificationTester::CConcurrentHashModificationTester() {
		}


		CConcurrentHashModificationTester::~CConcurrentHashModificationTester() {
		}


		void CConcurrentHashModificationTester::generateTestingStructure(cint64 modificationThreadsCount, cint64 modificationCountPerStep, bool useModifiedHashes, bool extendBaseHashes) {
			mModificationThreadsCount = modificationThreadsCount;
			mModificationCountPerStep = modificationCountPerStep;
			mExtendBaseHashes = extendBaseHashes;

			cint64 initialFillCountMax = 100;
			cint64 initialFillCountMin = 50;
			cint64 maxCacheValue = 100;

			mHashesCount = 50;
			mMemoryPoolProvider = new CNewAllocationMemoryPoolProvider(1024*10-20 /*~ 10 KByte*/);
			mQtOriginalHashes = new CQtHash<cint64,cint64>*[mHashesCount];
			mQtModifiedHashes = new CQtManagedRestrictedModificationHash<cint64,cint64>*[mHashesCount];
			for (cint64 i = 0; i < mHashesCount; ++i) {
				CQtHash<cint64,cint64>* newQtOriginalHash = new CQtHash<cint64,cint64>();
				mQtOriginalHashes[i] = newQtOriginalHash;
				mHashContainer1.append(newQtOriginalHash);
				CQtManagedRestrictedModificationHash<cint64,cint64>* newQtManagedMutHash = new CQtManagedRestrictedModificationHash<cint64,cint64>();
				mQtModifiedHashes[i] = newQtManagedMutHash;
				mHashContainer2.append(newQtManagedMutHash);

				cint64 fillCount = (qrand() % (initialFillCountMax-initialFillCountMin))+initialFillCountMin;

				for (cint64 i = 0; i < fillCount; ++i) {
					qint64 modHashKey = qrand() % maxCacheValue;
					newQtOriginalHash->insert(modHashKey,i);
					newQtManagedMutHash->insert(modHashKey,i);
				}
			}

			for (qint64 i = 0; i < mModificationThreadsCount; ++i) {
				CHashModificationThread *modThread = 0;
				if (useModifiedHashes) {
					modThread = new CHashModificationThread(mQtModifiedHashes,mHashesCount,mMemoryPoolProvider,mModificationCountPerStep,mExtendBaseHashes);
				} else {
					modThread = new CHashModificationThread(mQtOriginalHashes,mHashesCount,mModificationCountPerStep,mExtendBaseHashes);
				}
				mModThreadList.append(modThread);
			}
		}

		void CConcurrentHashModificationTester::startHashModificationTest() {
			cForeach (CHashModificationThread* modThread, mModThreadList) {
				modThread->startModifications();
			}
		}

		void CConcurrentHashModificationTester::stopHashModificationTest() {
			cForeach (CHashModificationThread* modThread, mModThreadList) {
				modThread->stopModifications();
			}
			cForeach (CHashModificationThread* modThread, mModThreadList) {
				modThread->quit();
			}
		}

		void CConcurrentHashModificationTester::destroyTestingStructure() {
			cDeleteAll(mModThreadList);
			delete mMemoryPoolProvider;
			delete mQtOriginalHashes;
			delete mQtModifiedHashes;
			cDeleteAll(mHashContainer2);
			cDeleteAll(mHashContainer1);
			mHashContainer1.clear();
			mHashContainer2.clear();
			mModThreadList.clear();
		}


		cint64 CConcurrentHashModificationTester::getModificationStepsCount() {
			cint64 totalModStepsCount = 0;
			cForeach (CHashModificationThread* modThread, mModThreadList) {
				totalModStepsCount += modThread->getModificationStepsCount();
			}
			return totalModStepsCount;
		}



	}; // end namespace Test

}; // end namespace Konclude
