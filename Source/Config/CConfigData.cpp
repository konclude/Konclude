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

#include "CConfigData.h"


namespace Konclude {

	namespace Config {

		CConfigData::CConfigData(CConfigDescription *configDesciption, CConfigType *takeConfigValue) {
			configDes = configDesciption;
			configVal = takeConfigValue;
			lockLevel = -1;
		}


		CConfigData::CConfigData(CConfigDescription *configDesciption) {
			configDes = configDesciption;
			configVal = configDes->getConfigType()->getTypeCopy();
		}



		CConfigData::~CConfigData() {
			delete configVal;
		}


		bool CConfigData::equals(CConfigData *data) {
			return (configDes == data->configDes && configVal->equals(data->configVal));
		}


		bool CConfigData::readFromString(const QString &string) {
			CConvertStringConfigType *conStringData = dynamic_cast<CConvertStringConfigType *>(configVal);
			if (conStringData) {
				return conStringData->readFromString(string);
			}
			return false;
		}

		QString CConfigData::getString() {
			QString string;
			CConvertStringConfigType *conStringData = dynamic_cast<CConvertStringConfigType *>(configVal);
			if (conStringData) {
				string = conStringData->getString();
			}
			return string;
		}


		bool CConfigData::readFromInteger(qint64 integer) {
			CConvertIntegerConfigType *conIntegerData = dynamic_cast<CConvertIntegerConfigType *>(configVal);
			if (conIntegerData) {
				return conIntegerData->readFromInteger(integer);
			}
			return false;
		}

		qint64 CConfigData::getInteger() {
			qint64 integer = 0;
			CConvertIntegerConfigType *conIntegerData = dynamic_cast<CConvertIntegerConfigType *>(configVal);
			if (conIntegerData) {
				integer = conIntegerData->getInteger();
			}
			return integer;
		}


		bool CConfigData::readFromBoolean(bool boolean) {
			CConvertBooleanConfigType *conBooleanData = dynamic_cast<CConvertBooleanConfigType *>(configVal);
			if (conBooleanData) {
				return conBooleanData->readFromBoolean(boolean);
			}
			return false;
		}

		bool CConfigData::getBoolean() {
			bool boolean = 0;
			CConvertBooleanConfigType *conBooleanData = dynamic_cast<CConvertBooleanConfigType *>(configVal);
			if (conBooleanData) {
				boolean = conBooleanData->getBoolean();
			}
			return boolean;
		}


		CConfigDescription *CConfigData::getConfigDescription() {
			return configDes;
		}

		CConfigType *CConfigData::getConfigType() {
			return configVal;
		}


		bool CConfigData::supportsStringConvertion() {
			return configDes->supportsStringConvertion();
		}


		bool CConfigData::supportsIntegerConvertion() {
			return configDes->supportsIntegerConvertion();
		}

		bool CConfigData::supportsBooleanConvertion() {
			return configDes->supportsBooleanConvertion();
		}


		qint64 CConfigData::getLevelLock() {
			return lockLevel;
		}
		bool CConfigData::hasLock() {
			return lockLevel >= 0;
		}

		CConfigData *CConfigData::setLevelLock(qint64 levelLock) {
			lockLevel = levelLock;
			return this;
		}



	}; // end namespace Config

}; // end namespace Konclude
