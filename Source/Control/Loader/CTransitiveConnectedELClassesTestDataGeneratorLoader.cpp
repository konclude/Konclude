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

#include "CTransitiveConnectedELClassesTestDataGeneratorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CTransitiveConnectedELClassesTestDataGeneratorLoader::CTransitiveConnectedELClassesTestDataGeneratorLoader()  {
				mConfiguration = 0;
				generator = 0;
			}



			CTransitiveConnectedELClassesTestDataGeneratorLoader::~CTransitiveConnectedELClassesTestDataGeneratorLoader() {
			}


			CLoader* CTransitiveConnectedELClassesTestDataGeneratorLoader::init(CLoaderFactory* loaderFactory, CConfiguration* config) {
				mConfiguration = config;
				return this;
			}




			CLoader *CTransitiveConnectedELClassesTestDataGeneratorLoader::load() {
				generator = new COWLTransitiveConnectedELClassesTestDataGenerator();
				QString filename = CConfigDataReader::readConfigString(mConfiguration,"Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile","RecursionTestData.owl.xml");
				cint64 indiSize = CConfigDataReader::readConfigInteger(mConfiguration,"Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount",1000);
				generator->generateData(indiSize,filename);
				delete generator;
				return this;
			}


			CLoader* CTransitiveConnectedELClassesTestDataGeneratorLoader::exit() {
				return this;
			}




		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
