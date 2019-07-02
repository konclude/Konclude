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

#include "CConfigManagerReader.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CConfigManagerReader::CConfigManagerReader() {
			}


			CConfigManagerReader::~CConfigManagerReader() {
			}


			CCommanderManager *CConfigManagerReader::readCommanderManagerConfig(CConfiguration *config, bool *errorFlag, CCommanderManager *defaultValue, const QString &configString) {
				CCommanderManager *manager = defaultValue;
				if (config) {
					CConfigData *configData = config->getRelatedConfigChange(configString,true);
					if (configData) {
						CCommanderManagerConfigType *managerConfig = dynamic_cast<CCommanderManagerConfigType *>(configData->getConfigType());
						if (managerConfig) {
							manager = managerConfig->getCommanderManager();
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

			CCommanderManager *CConfigManagerReader::readCommanderManagerConfig(CConfigurationProvider *configProv, bool *errorFlag, CCommanderManager *defaultValue, const QString &configString) {
				CConfiguration *config = 0;
				if (configProv) {
					config = configProv->getCurrentConfiguration();
				}
				return readCommanderManagerConfig(config,errorFlag,defaultValue,configString);
			}


			CReasonerManager *CConfigManagerReader::readReasonerManagerConfig(CConfiguration *config, bool *errorFlag, CReasonerManager *defaultValue, const QString &configString) {
				CReasonerManager *manager = defaultValue;
				if (config) {
					CConfigData *configData = config->getRelatedConfigChange(configString,true);
					if (configData) {
						CReasonerManagerConfigType *managerConfig = dynamic_cast<CReasonerManagerConfigType *>(configData->getConfigType());
						if (managerConfig) {
							manager = managerConfig->getReasonerManager();
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

			CReasonerManager *CConfigManagerReader::readReasonerManagerConfig(CConfigurationProvider *configProv, bool *errorFlag, CReasonerManager *defaultValue, const QString &configString) {
				CConfiguration *config = 0;
				if (configProv) {
					config = configProv->getCurrentConfiguration();
				}
				return readReasonerManagerConfig(config,errorFlag,defaultValue,configString);
			}


			CClassificationManager *CConfigManagerReader::readClassificationManagerConfig(CConfiguration *config, bool *errorFlag, CClassificationManager *defaultValue, const QString &configString) {
				CClassificationManager *manager = defaultValue;
				if (config) {
					CConfigData *configData = config->getRelatedConfigChange(configString,true);
					if (configData) {
						CClassificationManagerConfigType *managerConfig = dynamic_cast<CClassificationManagerConfigType *>(configData->getConfigType());
						if (managerConfig) {
							manager = managerConfig->getClassificationManager();
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

			CClassificationManager *CConfigManagerReader::readClassificationManagerConfig(CConfigurationProvider *configProv, bool *errorFlag, CClassificationManager *defaultValue, const QString &configString) {
				CConfiguration *config = 0;
				if (configProv) {
					config = configProv->getCurrentConfiguration();
				}
				return readClassificationManagerConfig(config,errorFlag,defaultValue,configString);
			}


			COntologyRevisionManager *CConfigManagerReader::readOntologyRevisionConfig(CConfiguration *config, bool *errorFlag, COntologyRevisionManager *defaultValue, const QString &configString) {
				COntologyRevisionManager *manager = defaultValue;
				if (config) {
					CConfigData *configData = config->getRelatedConfigChange(configString,true);
					if (configData) {
						COntologyRevisionManagerConfigType *managerConfig = dynamic_cast<COntologyRevisionManagerConfigType *>(configData->getConfigType());
						if (managerConfig) {
							manager = managerConfig->getOntologyRevisionManager();
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

			COntologyRevisionManager *CConfigManagerReader::readOntologyRevisionConfig(CConfigurationProvider *configProv, bool *errorFlag, COntologyRevisionManager *defaultValue, const QString &configString) {
				CConfiguration *config = 0;
				if (configProv) {
					config = configProv->getCurrentConfiguration();
				}
				return readOntologyRevisionConfig(config,errorFlag,defaultValue,configString);
			}


		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
