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

#include "CConfigurationBase.h"


namespace Konclude {

	namespace Config {


		CConfigurationBase::CConfigurationBase(CConfiguration *previousConfiguration, qint64 configurationID) 
				: CConfiguration(previousConfiguration) {
			confID = configurationID;
		}

		CConfigurationBase::CConfigurationBase(CConfigurationBase *previousConfiguration, qint64 configurationID) 
				: CConfiguration(previousConfiguration) {
			confID = configurationID;
			prevConfBase = previousConfiguration;
		}

		CConfigurationBase::CConfigurationBase(CConfigurationGroup *configGroup, qint64 configurationID) 
				: CConfiguration(configGroup) {
			confID = configurationID;
			prevConfBase = 0;
		}


		CConfigurationBase::~CConfigurationBase() {
		}


		qint64 CConfigurationBase::getConfigurationLevel() {
			return 0;
		}

		bool CConfigurationBase::isConfigLevelLocked(CConfigData *data) {
			if (data) {
				return data->getLevelLock() >= getConfigurationLevel();
			}
			return false;
		}

		bool CConfigurationBase::isConfigLevelLocked(const QString &configName, bool defaultCompensation) {
			CConfigData *configData = getRelatedConfigChange(configName,defaultCompensation);
			return isConfigLevelLocked(configData);
		}


		bool CConfigurationBase::isConfigLevelLocked(qint64 configIndex, bool defaultCompensation) {
			CConfigData *configData = getRelatedConfigChange(configIndex,defaultCompensation);
			return isConfigLevelLocked(configData);
		}


		qint64 CConfigurationBase::getConfigurationID() {
			return confID;
		}

		bool CConfigurationBase::isSuccessor(qint64 configID) {
			return (confID == configID || (prevConfBase && prevConfBase->isSuccessor(configID)));
		}


		bool CConfigurationBase::hasConfigChange(qint64 configIndex, qint64 prevConfID, CConfigData *configData) {
			if (configIndex >= 0 && configIndex < conData->count()) {
				if (confID >= prevConfID) {
					CConfigData *data = (*conData)[configIndex];
					if (configData && data) {
						if (!configData->equals(data)) {
							return true;
						} 
					}
					if (configData) {
						data = configData;
					}
					if (prevConfBase) {
						return prevConfBase->hasConfigChange(configIndex,prevConfID,data);
					} else if (prevConfID <= 0) {
						data = 0;
						if (prevConf) {
							data = prevConf->getLastConfigChange(configIndex);
						} else if (conGroup) {
							data = conGroup->getConfigDefaultData(configIndex);
						}
						if (configData && data) {
							if (!configData->equals(data)) {
								return true;
							} 
						}
					}
				}
			}
			return false;		
		}


		CConfigData *CConfigurationBase::getRelatedConfigChange(const QString &configName, bool defaultCompensation) {
			CConfigData *data = 0;
			if (conGroup) {
				qint64 index = conGroup->getConfigIndex(configName);
				data = getRelatedConfigChange(index,defaultCompensation);
			}
			return data;
		}


		CConfigData *CConfigurationBase::getRelatedConfigChange(qint64 configIndex, bool defaultCompensation) {
			CConfigData *data = getLastConfigChange(configIndex,defaultCompensation);
			return data;
		}


		CConfigurationBase *CConfigurationBase::appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList) {
			if (configurationList) {
				configurationList->append(this);
			}
			return this;
		}


	}; // end namespace Config

}; // end namespace Konclude
