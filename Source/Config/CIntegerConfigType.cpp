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

#include "CIntegerConfigType.h"


namespace Konclude {

	namespace Config {

		CIntegerConfigType::CIntegerConfigType(qint64 value) {
			intValue = value;
		}


		CIntegerConfigType::~CIntegerConfigType() {
		}


		CIntegerConfigType *CIntegerConfigType::getTypeCopy() {
			return new CIntegerConfigType(intValue);
		}

		bool CIntegerConfigType::supportsVariant() {
			return true;
		}

		CIntegerConfigType *CIntegerConfigType::readFrom(const QVariant &var) {
			intValue = var.toInt();
			return this;
		}

		QVariant CIntegerConfigType::getVariant() {
			return QVariant(intValue);
		}

		CIntegerConfigType *CIntegerConfigType::setValue(qint64 value) {
			intValue = value;
			return this;
		}

		qint64 CIntegerConfigType::getValue() {
			return intValue;
		}

		bool CIntegerConfigType::equals(CConfigType *dataType) {
			CIntegerConfigType *intType = dynamic_cast<CIntegerConfigType *>(dataType);
			if (intType) {
				return (intValue == intType->intValue);
			}
			return false;
		}

		bool CIntegerConfigType::readFromString(const QString &string) {
			bool ok = true;
			qint64 tmp = string.toLongLong(&ok);
			if (ok) {
				intValue = tmp;
			}
			return ok;
		}

		QString CIntegerConfigType::getString() {
			return QString::number(intValue);
		}

		bool CIntegerConfigType::readFromInteger(qint64 integer) {
			intValue = integer;
			return true;
		}

		qint64 CIntegerConfigType::getInteger() {
			return intValue;
		}



	}; // end namespace Config

}; // end namespace Konclude
