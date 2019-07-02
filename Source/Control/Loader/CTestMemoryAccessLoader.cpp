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

#include "CTestMemoryAccessLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CTestMemoryAccessLoader::CTestMemoryAccessLoader() : CIntervalThread("TestMemoryAccessThread") {
				mConfiguration = 0;
				tester = 0;
				startThread();
			}



			CTestMemoryAccessLoader::~CTestMemoryAccessLoader() {
			}


			CLoader* CTestMemoryAccessLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CTestMemoryAccessLoader::load() {
				tester = new CConcurrentMemoryAccessTester();
				mRemainingSteps = 1;
				mThreadCount = 1;
				mMaxThreadCount = 10;

				mReadWriteRatioInit = mReadWriteRatio = 1;
				mReadWriteRatioMax = 256;
				mReadWriteRatioStepMultiplier = 4;

				mAtomicOperations = false;

				mArraySizeInit = mArraySize = 2621440;
				mArraySizeStepMultiplier = 4;
				mArraySizeMax = Q_INT64_C(671088640);

				mArraySizeInit = mArraySize = 1310720;
				mArraySizeStepMultiplier = 4;
				mArraySizeMax = Q_INT64_C(335544320);

				startTimerWithInterval(TESTTIMERID,5000);
				tester->generateTestingStructure(mArraySize,mReadWriteRatio,mAtomicOperations,mThreadCount);
				tester->startHashModificationTest();
				cout<<"Starting concurrent memory access performance test.\n";
				cout<<"----------------------\n";
				return this;
			}


			bool CTestMemoryAccessLoader::processTimer(qint64 timerID) {
				if (timerID == TESTTIMERID) {
					if (mRemainingSteps > 0) {
						cint64 modSteps = tester->getModificationStepsCount();
						tester->stopHashModificationTest();
						tester->destroyTestingStructure();
						if (mThreadCount == 1) {
							mOneThreadScore = modSteps;
						}
						cout<<""<<mThreadCount<<" \tThreads,"<<"    Read Write Ratio: "<<mReadWriteRatio<<", \tMemory Size: "<<mArraySize*4/1024/1024;
						cout<<" MByte";
						//if (mAtomicOperations) {
						//	cout<<", \t+";
						//} else {
						//	cout<<", \t-";
						//}
						//cout<<"Atomic Operations";
						cout<<" \t>> Modification Score: \t"<<modSteps;
						double scaleFac = (double)modSteps / (double)mOneThreadScore;
						cout<<", \t"<<scaleFac<<"x"<<endl;


						if (mThreadCount < mMaxThreadCount) {
							++mThreadCount;
						} else {
							mThreadCount = 1;
							//if (!mAtomicOperations) {
							//	mAtomicOperations = true;
							//} else {
							//	mAtomicOperations = false;
								if (mReadWriteRatio < mReadWriteRatioMax) {
									mReadWriteRatio *= mReadWriteRatioStepMultiplier;
								} else {
									mReadWriteRatio = mReadWriteRatioInit;
									cout<<"----------------------\n";
									if (mArraySize < mArraySizeMax) {
										mArraySize *= mArraySizeStepMultiplier;
									} else {
										mArraySize = mArraySizeInit;
										// finish analyzing
										mRemainingSteps = 0;
									}
								}
							//}
						}

						if (mRemainingSteps > 0) {
							tester->generateTestingStructure(mArraySize,mReadWriteRatio,mAtomicOperations,mThreadCount);
							tester->startHashModificationTest();
						}

					} else {
						cout<<"Test finished.\n";
						stopThread(false);
					}
					return true;
				}
				return false;
			}


			CLoader* CTestMemoryAccessLoader::exit() {
				if (tester) {
					delete tester;
				}
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
