/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CConfigurationGroup.h"


namespace Konclude {

	namespace Config {

		CConfigurationGroup::CConfigurationGroup() {
		}


		CConfigurationGroup::~CConfigurationGroup() {
			qDeleteAll(configDesContainer);
			qDeleteAll(configDataContainer);
		}


		qint64 CConfigurationGroup::addConfigProperty(CConfigDescription *takeConfigDescription, CConfigData *takeDefaultConfigValue) {
			readWriteSync.lockForWrite();

			if (takeConfigDescription){
				configDesContainer.append(takeConfigDescription);
			}
			if (takeDefaultConfigValue){
				configDataContainer.append(takeDefaultConfigValue);
			}
		
			qint64 configIdx = 0;
			if (!propertyNameIndexHash.contains(takeConfigDescription->getConfigName())) {
				configIdx = configDesVector.getItemCount();
				propertyNameIndexHash.insert(takeConfigDescription->getConfigName(),configIdx);
			} else {
				configIdx = propertyNameIndexHash.value(takeConfigDescription->getConfigName());
			}
			configDesVector.setData(configIdx,takeConfigDescription);
			configDataVector.setData(configIdx,takeDefaultConfigValue);

			takeConfigDescription->setConfigID(configIdx);
			readWriteSync.unlock();
			return configIdx;
		}


		CConfigurationGroup *CConfigurationGroup::setConfigDefaultData(qint64 index, CConfigData *takeDefaultConfigValue) {
			readWriteSync.lockForWrite();
			if (takeDefaultConfigValue){
				configDataVector.setData(index,takeDefaultConfigValue);
				configDataContainer.append(takeDefaultConfigValue);
			}
			readWriteSync.unlock();
			return this;
		}



		qint64 CConfigurationGroup::addConfigProperty(CConfigDescription *takeConfigDescription) {
			CConfigData *takeDefaultConfigValue = new CConfigData(takeConfigDescription,takeConfigDescription->getConfigType()->getTypeCopy()); 
			return addConfigProperty(takeConfigDescription,takeDefaultConfigValue);
		}


		qint64 CConfigurationGroup::addConfigProperty(CConfigDescription *takeConfigDescription, CConfigType *takeDefaultConfigType) {
			CConfigData *takeDefaultConfigValue = new CConfigData(takeConfigDescription,takeDefaultConfigType); 
			return addConfigProperty(takeConfigDescription,takeDefaultConfigValue);
		}

		CConfigDescription *CConfigurationGroup::getConfigDescription(qint64 index) {
			readWriteSync.lockForRead();
			CConfigDescription *des = 0;
			if (index >= 0 && index < configDesVector.getItemCount()) {
				des = configDesVector.getData(index);
			}
			readWriteSync.unlock();
			return des;
		}

		CConfigData *CConfigurationGroup::getConfigDefaultData(qint64 index) {
			readWriteSync.lockForRead();
			CConfigData *dat = 0;
			if (index >= 0 && index < configDataVector.getItemCount()) {
				dat = configDataVector.getData(index);
			}
			readWriteSync.unlock();
			return dat;
		}


		CConfigDescription *CConfigurationGroup::getConfigDescription(const QString &name) {
			qint64 index = getConfigIndex(name);
			return getConfigDescription(index);
		}

		CConfigData *CConfigurationGroup::getConfigDefaultData(const QString &name) {
			qint64 index = getConfigIndex(name);
			return getConfigDefaultData(index);
		}


		qint64 CConfigurationGroup::getConfigPropertyCount() {
			return configDesVector.getItemCount();
		}

		qint64 CConfigurationGroup::getConfigIndex(const QString &name) {
			qint64 index = propertyNameIndexHash.value(name,-1);
			return index;
		}


		CConfigurationGroup* CConfigurationGroup::getReferencedConfigurationGroup() {
			CConfigurationGroup *refConfGroup = new CConfigurationGroup();
			refConfGroup->propertyNameIndexHash = propertyNameIndexHash;
			refConfGroup->configDesVector.reference(&configDesVector);
			refConfGroup->configDataVector.reference(&configDataVector);
			return refConfGroup;
		}



		QList<CConfigDescription*> CConfigurationGroup::getConfigDescriptonList() {
			QList<CConfigDescription*> desList;
			for (cint64 i = configDesVector.getItemCount()-1; i >= 0; --i) {
				desList.append(configDesVector.getData(i));
			}
			return desList;
		}



	}; // end namespace Config

}; // end namespace Konclude
