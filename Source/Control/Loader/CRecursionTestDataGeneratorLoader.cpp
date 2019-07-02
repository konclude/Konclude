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

#include "CRecursionTestDataGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CRecursionTestDataGeneratorLoader::CRecursionTestDataGeneratorLoader()  {
				mConfiguration = 0;
				generator = 0;
			}



			CRecursionTestDataGeneratorLoader::~CRecursionTestDataGeneratorLoader() {
			}


			CLoader* CRecursionTestDataGeneratorLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CRecursionTestDataGeneratorLoader::load() {
				generator = new COWLRecursionTestDataGenerator();
				QString filename = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Test.RecursionTestDataGenerator.OWLlinkOutputFile","RecursionTestData.owl.xml");
				cint64 indiSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.RecursionTestDataGenerator.IndividualCount",1000);
				cint64 probSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.RecursionTestDataGenerator.PropertyAssertionCount",200);
				double ratio = (double)probSize/(double)indiSize;
				generator->generateData(indiSize,ratio,filename);
				delete generator;
				return this;
			}


			CLoader* CRecursionTestDataGeneratorLoader::exit() {
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
