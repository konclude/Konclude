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

#include "CStringConfigType.h"


namespace Konclude {

	namespace Config {

		CStringConfigType::CStringConfigType(const QString &value) {
			stringValue = value;
		}


		CStringConfigType::~CStringConfigType() {
		}


		CStringConfigType *CStringConfigType::getTypeCopy() {
			return new CStringConfigType(stringValue);
		}

		bool CStringConfigType::supportsVariant() {
			return true;
		}

		CStringConfigType *CStringConfigType::readFrom(const QVariant &var) {
			stringValue = var.toString();
			return this;
		}

		QVariant CStringConfigType::getVariant() {
			return QVariant(stringValue);
		}

		CStringConfigType *CStringConfigType::setValue(QString value) {
			stringValue = value;
			return this;
		}

		QString CStringConfigType::getValue() {
			return stringValue;
		}

		bool CStringConfigType::equals(CConfigType *dataType) {
			CStringConfigType *stringType = dynamic_cast<CStringConfigType *>(dataType);
			if (stringType) {
				return (stringValue == stringType->stringValue);
			}
			return false;
		}


		bool CStringConfigType::readFromString(const QString &string) {
			stringValue = string;
			return true;
		}

		QString CStringConfigType::getString() {
			return stringValue;
		}


	}; // end namespace Config

}; // end namespace Konclude
