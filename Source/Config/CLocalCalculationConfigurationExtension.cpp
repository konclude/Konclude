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
/*
 *
 *		Author:		Andreas Steigmiller
 *		Copyright:	2009 Andreas Steigmiller
 *		Project:	UUPR
 *
 */

#include "CLocalConfigurationExtension.h"


namespace UUPR {

	namespace Config {


		CLocalConfigurationExtension::CLocalConfigurationExtension(CConfigurationBase *previousConfiguration, qint64 configID) 
				: CConfigurationBase(previousConfiguration,configID) {
		}

		CLocalConfigurationExtension::~CLocalConfigurationExtension() {
		}

		CLocalConfigurationExtension *CLocalConfigurationExtension::addConfiguration(CConfigurationBase *configuration) {
			remConfigList.append(configuration);
			return this;
		}

		CConfigData *CLocalConfigurationExtension::getRelatedConfigChange(const QString &configName, bool defaultCompensatory) {
			CConfigData *data = 0;
			if (conGroup) {
				qint64 index = conGroup->getConfigIndex(configName,defaultCompensatory);
				data = getRelatedConfigChange(index);
			}
			return data;
		}

		CConfigData *CLocalConfigurationExtension::getRelatedConfigChange(qint64 configIndex, bool defaultCompensatory) {
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


	}; // end namespace Config

}; // end namespace UUPR
