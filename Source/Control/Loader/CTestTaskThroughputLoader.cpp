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

#include "CTestTaskThroughputLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CTestTaskThroughputLoader::CTestTaskThroughputLoader() {
				mConfiguration = 0;
				tester = 0;
			}



			CTestTaskThroughputLoader::~CTestTaskThroughputLoader() {
			}


			CLoader* CTestTaskThroughputLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CTestTaskThroughputLoader::load() {
				tester = new CConcurrentThroughputTestTaskTester();
				tester->generateTestingStructure(20,2);
				cout<<"Starting concurrent Task-Throughput Test.\n";
				tester->startTesting();
				return this;
			}


			CLoader* CTestTaskThroughputLoader::exit() {
				tester->stopTesting();
				if (tester) {
					delete tester;
				}
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
