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

#include "CLocalConfigurationExtension.h"


namespace Konclude {

	namespace Config {


		CLocalConfigurationExtension::CLocalConfigurationExtension(CConfigurationBase *previousConfiguration, qint64 configID) 
				: CConfigurationBase(previousConfiguration,configID) {
		}

		CLocalConfigurationExtension::~CLocalConfigurationExtension() {
		}

		CLocalConfigurationExtension *CLocalConfigurationExtension::addConfigurationProvider(CConfigurationProvider *configurationProvider) {
			remConfigList.append(configurationProvider);
			return this;
		}


		qint64 CLocalConfigurationExtension::getConfigurationLevel() {
			return remConfigList.count();
		}


		CConfigData *CLocalConfigurationExtension::getRelatedConfigChange(qint64 configIndex, bool defaultCompensation) {
			CConfigData *data = 0;
			data = getLastConfigChange(configIndex,false);			
			for (QLinkedList<CConfigurationProvider *>::const_iterator it = remConfigList.constBegin(); it != remConfigList.constEnd() && !data; ++it) {
				CConfigurationProvider *nextConfProv = (*it);
				CConfigurationBase *nextConf = nextConfProv->getCurrentConfiguration();
				if (nextConf) {
					data = nextConf->getRelatedConfigChange(configIndex,false);
				}
			}
			if (!data) {
				if (conGroup) {
					data = conGroup->getConfigDefaultData(configIndex);
				}
			}
			return data;
		}


		CConfigurationBase *CLocalConfigurationExtension::appendConfigurationsToList(QLinkedList<CConfigurationBase *> *configurationList) {
			CConfigurationBase::appendConfigurationsToList(configurationList);
			if (configurationList) {
				for (QLinkedList<CConfigurationProvider *>::const_iterator it = remConfigList.constBegin(); it != remConfigList.constEnd(); ++it) {
					CConfigurationProvider *nextConfProv = (*it);
					CConfigurationBase *nextConf = nextConfProv->getCurrentConfiguration();
					if (nextConf) {
						nextConf->appendConfigurationsToList(configurationList);
					}
				}
			}
			return this;
		}


	}; // end namespace Config

}; // end namespace Konclude
