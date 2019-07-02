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

#include "CBooleanConfigType.h"


namespace Konclude {

	namespace Config {

		CBooleanConfigType::CBooleanConfigType(bool value) {
			boolValue = value;
		}


		CBooleanConfigType::~CBooleanConfigType() {
		}


		CBooleanConfigType *CBooleanConfigType::getTypeCopy() {
			return new CBooleanConfigType(boolValue);
		}

		bool CBooleanConfigType::supportsVariant() {
			return true;
		}

		CBooleanConfigType *CBooleanConfigType::readFrom(const QVariant &var) {
			boolValue = var.toBool();
			return this;
		}

		QVariant CBooleanConfigType::getVariant() {
			return QVariant(boolValue);
		}

		CBooleanConfigType *CBooleanConfigType::setValue(bool value) {
			boolValue = value;
			return this;
		}

		bool CBooleanConfigType::getValue() {
			return boolValue;
		}

		bool CBooleanConfigType::equals(CConfigType *dataType) {
			CBooleanConfigType *boolType = dynamic_cast<CBooleanConfigType *>(dataType);
			if (boolType) {
				return (boolValue == boolType->boolValue);
			}
			return false;
		}

		bool CBooleanConfigType::readFromString(const QString &string) {
			bool ok = true;
			qint64 tmp = string.toInt(&ok);
			if (ok) {
				boolValue = tmp;
			} else {
				if (string.toUpper() == "TRUE") {
					boolValue = true;
					ok = true;
				}
				if (string.toUpper() == "FALSE") {
					boolValue = false;
					ok = true;
				}
			}
			return ok;
		}

		QString CBooleanConfigType::getString() {
			if (boolValue) {
				return QString("TRUE");
			} else {
				return QString("FALSE");
			}
		}

		bool CBooleanConfigType::readFromInteger(qint64 integer) {
			boolValue = integer;
			return true;
		}

		qint64 CBooleanConfigType::getInteger() {
			return boolValue;
		}

		bool CBooleanConfigType::readFromBoolean(bool boolean) {
			boolValue = boolean;
			return true;
		}

		bool CBooleanConfigType::getBoolean() {
			return boolValue;
		}


	}; // end namespace Config

}; // end namespace Konclude
