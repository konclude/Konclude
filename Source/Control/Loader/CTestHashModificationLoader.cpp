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

#include "CTestHashModificationLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CTestHashModificationLoader::CTestHashModificationLoader() : CIntervalThread("TestHashModificationThread") {
				mConfiguration = 0;
				tester = 0;
				startThread();
			}



			CTestHashModificationLoader::~CTestHashModificationLoader() {
			}


			CLoader* CTestHashModificationLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CTestHashModificationLoader::load() {
				tester = new CConcurrentHashModificationTester();
				mRemainingSteps = 1;
				mThreadCount = 1;
				mMaxThreadCount = 10;
				mUseModificatedHashes = false;
				mExtendBaseHashes = false;
				mHashModCountPerStep = 4;
				mMaxHashModCountPerStep = 16384;
				tester->generateTestingStructure(mThreadCount,mHashModCountPerStep,mUseModificatedHashes,mExtendBaseHashes);
				tester->startHashModificationTest();
				startTimerWithInterval(TESTTIMERID,10000);
				cout<<"Starting concurrent hash modification performance test.\n";
				cout<<"----------------------\n";
				return this;
			}


			bool CTestHashModificationLoader::processTimer(qint64 timerID) {
				if (timerID == TESTTIMERID) {
					if (mRemainingSteps > 0) {
						cint64 modSteps = tester->getModificationStepsCount();
						tester->stopHashModificationTest();
						tester->destroyTestingStructure();
						if (mThreadCount == 1) {
							mOneThreadScore = modSteps;
						}
						cout<<""<<mThreadCount<<" \tThreads,";
						if (mUseModificatedHashes) {
							cout<<"  using Modified-Qt-Hashes";
						} else {
							cout<<"  using Original-Qt-Hashes";
						}
						cout<<",  #Modifications per Step "<<mHashModCountPerStep;
						if (mExtendBaseHashes) {
							cout<<", \textending Hashes";
						} else {
							cout<<", \tbuild new Hashes";
						}
						cout<<" >> "<<"Hash Modification Score: \t"<<modSteps;
						double scaleFac = (double)modSteps / (double)mOneThreadScore;
						cout<<", \t"<<scaleFac<<"x"<<endl;


						if (mThreadCount < mMaxThreadCount) {
							++mThreadCount;
						} else {
							mThreadCount = 1;
							if (mHashModCountPerStep < mMaxHashModCountPerStep) {
								mHashModCountPerStep *= 4;
							} else {
								mHashModCountPerStep = 4;
								cout<<"----------------------\n";
								if (!mExtendBaseHashes) {
									mExtendBaseHashes = true;
								} else {
									mExtendBaseHashes = false;
									if (!mUseModificatedHashes) {
										mUseModificatedHashes = true;
									} else {
										// finish analyzing
										mRemainingSteps = 0;
									}
								}
							}
						}

						if (mRemainingSteps > 0) {
							tester->generateTestingStructure(mThreadCount,mHashModCountPerStep,mUseModificatedHashes,mExtendBaseHashes);
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


			CLoader* CTestHashModificationLoader::exit() {
				if (tester) {
					delete tester;
				}
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
