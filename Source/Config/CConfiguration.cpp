/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CConfiguration.h"


namespace Konclude {

	namespace Config {

		CConfiguration::CConfiguration(CConfigurationGroup *configGroup) {
			conGroup = configGroup;
			qint64 conCount = 0;
			if (conGroup) {
				conCount = conGroup->getConfigPropertyCount();
			}
			conData = new QVector<CConfigData *>(conCount,0);
			checkedRefConData = new QVector<bool>(conCount,false);
			hasRefConData = new QVector<bool>(conCount,false);
			prevConf = 0;
		}

		CConfiguration::CConfiguration(CConfiguration *previousConfiguration) {
			prevConf = previousConfiguration;
			conGroup = 0;
			if (prevConf) {
				conGroup = prevConf->conGroup;
			}
			qint64 conCount = 0;
			if (conGroup) {
				conCount = conGroup->getConfigPropertyCount();
			}
			conData = new QVector<CConfigData *>(conCount,0);
			checkedRefConData = new QVector<bool>(conCount,false);
			hasRefConData = new QVector<bool>(conCount,false);
		}

		CConfiguration::~CConfiguration() {
			delete conData;
			delete checkedRefConData;
			delete hasRefConData;
			qDeleteAll(conDataContainer);
		}


		bool CConfiguration::setConfig(qint64 configIndex, CConfigData *takeData) {
			if (configIndex >= 0 && configIndex < conData->count()) {
				conDataContainer.append(takeData);
				(*conData)[configIndex] = takeData;
				return true;
			}
			return false;
		}

		bool CConfiguration::setConfig(const QString &configName, CConfigData *takeData) {
			if (conGroup) {
				qint64 index = conGroup->getConfigIndex(configName);
				return setConfig(index,takeData);
			}
			return false;
		}


		CConfigData *CConfiguration::createAndSetConfig(const QString &configName) {
			CConfigData *confData = createConfig(configName);
			if (confData) {
				setConfig(configName,confData);
			}
			return confData;
		}


		CConfigData *CConfiguration::createConfig(const QString &configName) {
			CConfigData *data = 0;
			if (conGroup) {
				CConfigDescription *conDes = conGroup->getConfigDescription(configName);
				if (conDes) {
					data = new CConfigData(conDes,conDes->getConfigType()->getTypeCopy());
				}
			}
			return data;
		}


		CConfigData *CConfiguration::getRelatedConfigChange(const QString &configName, bool defaultCompensation) {
			return getLastConfigChange(configName,defaultCompensation);
		}

		CConfigData *CConfiguration::getRelatedConfigChange(qint64 configIndex, bool defaultCompensation) {
			return getLastConfigChange(configIndex,defaultCompensation);
		}



		CConfigData *CConfiguration::getLastConfigChange(const QString &configName, bool defaultCompensation) {
			CConfigData *data = 0;
			if (conGroup) {
				qint64 index = conGroup->getConfigIndex(configName);
				data = getLastConfigChange(index,defaultCompensation);
			}
			return data;
		}

		CConfigData *CConfiguration::getLastConfigChange(qint64 configIndex, bool defaultCompensation) {
			CConfigData *data = 0;
			if (configIndex >= 0 && configIndex < conData->count()) {
				data = (*conData)[configIndex];
				if (!data) {
					if (!(*checkedRefConData)[configIndex]) {
						if (prevConf) {
							data = prevConf->getLastConfigChange(configIndex,defaultCompensation);
						} 						
						if (!data && conGroup && defaultCompensation) {
							data = conGroup->getConfigDefaultData(configIndex);
						}
						(*checkedRefConData)[configIndex] = true;
						if (data) {
							(*conData)[configIndex] = data;
							(*hasRefConData)[configIndex] = true;
						}
					}
				}
			}
			return data;
		}



		CConfigurationGroup *CConfiguration::getConfigurationGroup() {
			return conGroup;
		}


		CConfiguration *CConfiguration::extendConfigurationGroup(CConfigurationGroup *referencedConfigGroup) {
			qint64 conCount = referencedConfigGroup->getConfigPropertyCount();
			conData->resize(conCount);
			checkedRefConData->resize(conCount);
			hasRefConData->resize(conCount);

			conGroup = referencedConfigGroup;

			return this;
		}



	}; // end namespace Config

}; // end namespace Konclude
