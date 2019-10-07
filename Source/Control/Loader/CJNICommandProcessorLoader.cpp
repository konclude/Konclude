/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CJNICommandProcessorLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CJNICommandProcessorLoader::CJNICommandProcessorLoader() {
				mJNIInstanceManager = nullptr;
				mJNICommandProcessor = nullptr;
				mLoaderConfig = nullptr;
			}



			CJNICommandProcessorLoader::~CJNICommandProcessorLoader() {
			}


			CLoader *CJNICommandProcessorLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				mLoaderConfig = config;


				LOG(INFO,"::Konclude::Loader::JNICommandProcessorLoader","Getting JNI instance manager.",0);
				mJNIInstanceManager = CConfigJNIReader::readJNIInstanceManagerConfig(mLoaderConfig);


				if (mJNIInstanceManager) {

					LOG(INFO,"::Konclude::Loader::JNICommandProcessorLoader","Creating JNI command processor.",0);

					mJNICommandProcessor = new CJNICommandProcessor(mLoaderConfig);
					mJNIInstanceManager->setJNICommandProcessor(mJNICommandProcessor);
				}

				return this;
			}



			CLoader *CJNICommandProcessorLoader::load() {
				return this;
			}



			CLoader *CJNICommandProcessorLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
