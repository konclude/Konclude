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

#include "CJNICommandLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CJNICommandLoader::CJNICommandLoader(CJNIInstanceManager* instanceManager) : CCommandLineLoader() {
				mInstanceManager = instanceManager;
			}


			CJNICommandLoader::CJNICommandLoader(CJNIInstanceManager* instanceManager, int argc, char *argv[]) : CCommandLineLoader(argc,argv,false) {
				mInstanceManager = instanceManager;
			}


			CJNICommandLoader::CJNICommandLoader(CJNIInstanceManager* instanceManager, const QStringList &argList) : CCommandLineLoader(argList,false) {
				mInstanceManager = instanceManager;
			}


			CJNICommandLoader::~CJNICommandLoader() {
			}


			CLoader* CJNICommandLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				configuration = config;


				CConfigurationGroup *reasonerConfigGroup = nullptr;
				if (configuration) {
					reasonerConfigGroup = configuration->getConfigurationGroup();
				} else {
					reasonerConfigGroup = CSingletonProvider<CReasonerConfigurationGroup>::getInstance()->getReferencedConfigurationGroup();
					configuration = new CConfiguration(reasonerConfigGroup);
				}

				CConfigDescription* conDes = new CConfigDescription("Konclude.JNI.JNIInstanceManager","",new CJNIInstanceManagerConfigType());
				reasonerConfigGroup->addConfigProperty(conDes);

				configuration->extendConfigurationGroup(reasonerConfigGroup);
				CConfigData *confData = configuration->createAndSetConfig("Konclude.JNI.JNIInstanceManager");
				if (confData) {
					CJNIInstanceManagerConfigType* confType = dynamic_cast<CJNIInstanceManagerConfigType*>(confData->getConfigType());
					if (confType) {
						confType->setJNIInstanceManager(mInstanceManager);
					}
				}


				CCommandLineLoader::init(loaderFactory,configuration);
				return this;
			}


		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
