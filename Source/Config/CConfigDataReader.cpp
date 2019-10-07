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

#include "CConfigDataReader.h"


namespace Konclude {

	namespace Config {

		CConfigDataReader::CConfigDataReader() {
		}


		CConfigDataReader::~CConfigDataReader() {
		}


		QString CConfigDataReader::readConfigString(CConfiguration *config, const QString &configString, const QString &defaultValue, bool *errorFlag) {
			QString stringValue = defaultValue;
			if (config) {
				CConfigData *configData = config->getRelatedConfigChange(configString,true);
				if (configData && configData->supportsStringConvertion()) {
					stringValue = configData->getString();
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
			return stringValue;
		}


		qint64 CConfigDataReader::readConfigInteger(CConfiguration *config, const QString &configString, qint64 defaultValue, bool *errorFlag) {
			qint64 intValue = defaultValue;
			if (config) {
				CConfigData *configData = config->getRelatedConfigChange(configString,true);
				if (configData && configData->supportsIntegerConvertion()) {
					intValue = configData->getInteger();
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
			return intValue;
		}
		
		bool CConfigDataReader::readConfigBoolean(CConfiguration *config, const QString &configString, bool defaultValue, bool *errorFlag) {
			qint64 boolValue = defaultValue;
			if (config) {
				CConfigData *configData = config->getRelatedConfigChange(configString,true);
				if (configData && configData->supportsBooleanConvertion()) {
					boolValue = configData->getBoolean();
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
			return boolValue;
		}


		QString CConfigDataReader::readConfigString(CConfigurationProvider *configProv, const QString &configString, const QString &defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigString(config,configString,defaultValue,errorFlag);
		}

		qint64 CConfigDataReader::readConfigInteger(CConfigurationProvider *configProv, const QString &configString, qint64 defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigInteger(config,configString,defaultValue,errorFlag);
		}

		bool CConfigDataReader::readConfigBoolean(CConfigurationProvider *configProv, const QString &configString, bool defaultValue, bool *errorFlag) {
			CConfiguration *config = 0;
			if (configProv) {
				config = configProv->getCurrentConfiguration();
			}
			return readConfigBoolean(config,configString,defaultValue,errorFlag);
		}


	}; // end namespace Config

}; // end namespace Konclude
