/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CStringListConfigType.h"


namespace Konclude {

	namespace Config {

		CStringListConfigType::CStringListConfigType(const QString &value, const QString& separator) {
			mSeparator = separator;
			setValue(value);
		}

		CStringListConfigType::CStringListConfigType(const QStringList& value, const QString& separator) {
			mSeparator = separator;
			mStringListValue = value;
		}

		CStringListConfigType::~CStringListConfigType() {
		}


		CStringListConfigType *CStringListConfigType::getTypeCopy() {
			return new CStringListConfigType(mStringListValue);
		}

		bool CStringListConfigType::supportsVariant() {
			return true;
		}

		CStringListConfigType *CStringListConfigType::readFrom(const QVariant &var) {
			mStringListValue = var.toString().split(mSeparator);
			return this;
		}

		QVariant CStringListConfigType::getVariant() {
			return QVariant(mStringListValue.join(mSeparator));
		}

		CStringListConfigType *CStringListConfigType::setValue(QString value) {
			mStringListValue = value.split(mSeparator);
			return this;
		}

		QStringList CStringListConfigType::getValue() {
			return mStringListValue;
		}

		bool CStringListConfigType::equals(CConfigType *dataType) {
			CStringListConfigType *stringType = dynamic_cast<CStringListConfigType *>(dataType);
			if (stringType) {
				return (mStringListValue == stringType->mStringListValue);
			}
			return false;
		}


		bool CStringListConfigType::readFromString(const QString &string) {
			setValue(string);
			return true;
		}

		QString CStringListConfigType::getString() {
			return mStringListValue.join(mSeparator);
		}


		bool CStringListConfigType::supportsStringConvertion() {
			return true;
		}


		bool CStringListConfigType::supportsStringListConvertion() {
			return true;
		}

		bool CStringListConfigType::readFromStringList(const QStringList &stringList) {
			mStringListValue = stringList;
			return true;
		}

		QStringList CStringListConfigType::getStringList() {
			return mStringListValue;
		}


	}; // end namespace Config

}; // end namespace Konclude
