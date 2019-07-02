/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CLocalConfigurationFixedExtension.h"


namespace Konclude {

	namespace Config {


		CLocalConfigurationFixedExtension::CLocalConfigurationFixedExtension(CConfigurationGroup *configGroup, qint64 configID) 
				: CConfigurationBase(configGroup,configID) {
		}

		CLocalConfigurationFixedExtension::CLocalConfigurationFixedExtension(CConfigurationBase *previousConfiguration, qint64 configID) 
				: CConfigurationBase(previousConfiguration,configID) {
		}

		CLocalConfigurationFixedExtension::~CLocalConfigurationFixedExtension() {
		}

		CLocalConfigurationFixedExtension *CLocalConfigurationFixedExtension::addConfiguration(CConfigurationBase *configuration) {
			remConfigList.append(configuration);
			return this;
		}

		CLocalConfigurationFixedExtension *CLocalConfigurationFixedExtension::addConfigurationList(const QLinkedList<CConfigurationBase *> &configList) {
			remConfigList += configList;
			return this;
		}


		CLocalConfigurationFixedExtension *CLocalConfigurationFixedExtension::addCollectedConfigurations(CConfigurationBase *collectFromConfiguration) {
			if (collectFromConfiguration) {
				collectFromConfiguration->appendConfigurationsToList(&remConfigList);
			}
			return this;
		}


		qint64 CLocalConfigurationFixedExtension::getConfigurationLevel() {
			return remConfigList.count();
		}


		CConfigData *CLocalConfigurationFixedExtension::getRelatedConfigChange(const QString &configName, bool defaultCompensation) {
			return CConfigurationBase::getRelatedConfigChange(configName,defaultCompensation);
		}

		CConfigData *CLocalConfigurationFixedExtension::getRelatedConfigChange(qint64 configIndex, bool defaultCompensation) {
			CConfigData *data = 0;
			data = getLastConfigChange(configIndex,false);			
			for (QLinkedList<CConfigurationBase *>::const_iterator it = remConfigList.constBegin(); it != remConfigList.constEnd() && !data; ++it) {
				CConfigurationBase *nextConf = (*it);
				data = nextConf->getLastConfigChange(configIndex,false);
			}
			if (!data) {
				if (conGroup) {
					data = conGroup->getConfigDefaultData(configIndex);
				}
			}
			return data;
		}

		QLinkedList<CConfigurationBase *> CLocalConfigurationFixedExtension::getAdditionalConfigurationList() {
			return remConfigList;
		}

		CConfigurationBase *CLocalConfigurationFixedExtension::appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList) {
			CConfigurationBase::appendConfigurationsToList(configurationList);
			if (configurationList) {
				*configurationList += remConfigList;
			}
			return this;
		}


	}; // end namespace Config

}; // end namespace Konclude
