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

#include "CLargeJoinTestDataGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CLargeJoinTestDataGeneratorLoader::CLargeJoinTestDataGeneratorLoader()  {
				mConfiguration = 0;
				generator = 0;
			}



			CLargeJoinTestDataGeneratorLoader::~CLargeJoinTestDataGeneratorLoader() {
			}


			CLoader* CLargeJoinTestDataGeneratorLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CLargeJoinTestDataGeneratorLoader::load() {
				generator = new COWLLargeJoinTestDataGenerator();
				QString filename = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Test.LargeJoinTestDataGenerator.OWLlinkOutputFile","LargeJoinTestData.owl.xml");
				cint64 indiSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.LargeJoinTestDataGenerator.IndividualCount",1000);
				cint64 probSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.LargeJoinTestDataGenerator.PropertyAssertionCount",200);
				double ratio = (double)probSize/(double)indiSize;
				generator->generateData(indiSize,ratio,filename);
				delete generator;
				return this;
			}


			CLoader* CLargeJoinTestDataGeneratorLoader::exit() {
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
