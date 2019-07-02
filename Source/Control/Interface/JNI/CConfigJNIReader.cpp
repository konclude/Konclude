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

#include "CConfigJNIReader.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CConfigJNIReader::CConfigJNIReader() {
				}


				CConfigJNIReader::~CConfigJNIReader() {
				}


				CJNIInstanceManager *CConfigJNIReader::readJNIInstanceManagerConfig(CConfiguration *config, bool *errorFlag, CJNIInstanceManager *defaultValue, const QString &configString) {
					CJNIInstanceManager *manager = defaultValue;
					if (config) {
						CConfigData *configData = config->getRelatedConfigChange(configString,true);
						if (configData) {
							CJNIInstanceManagerConfigType *managerConfig = dynamic_cast<CJNIInstanceManagerConfigType *>(configData->getConfigType());
							if (managerConfig) {
								manager = managerConfig->getJNIInstanceManager();
							}
						} else {
							if (errorFlag) {
								*errorFlag = true;
							}
						}
					} else {
						if (errorFlag) {
							*errorFlag = true;
						}
					}
					return manager;
				}

				CJNIInstanceManager *CConfigJNIReader::readJNIInstanceManagerConfig(CConfigurationProvider *configProv, bool *errorFlag, CJNIInstanceManager *defaultValue, const QString &configString) {
					CConfiguration *config = 0;
					if (configProv) {
						config = configProv->getCurrentConfiguration();
					}
					return readJNIInstanceManagerConfig(config,errorFlag,defaultValue,configString);
				}


			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
