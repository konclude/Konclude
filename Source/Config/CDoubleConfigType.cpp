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

#include "CDoubleConfigType.h"


namespace Konclude {

	namespace Config {

		CDoubleConfigType::CDoubleConfigType(double value) {
			mDoubleValue = value;
		}


		CDoubleConfigType::~CDoubleConfigType() {
		}


		CDoubleConfigType *CDoubleConfigType::getTypeCopy() {
			return new CDoubleConfigType(mDoubleValue);
		}

		bool CDoubleConfigType::supportsVariant() {
			return true;
		}

		CDoubleConfigType *CDoubleConfigType::readFrom(const QVariant &var) {
			mDoubleValue = var.toDouble();
			return this;
		}

		QVariant CDoubleConfigType::getVariant() {
			return QVariant(mDoubleValue);
		}

		CDoubleConfigType *CDoubleConfigType::setValue(double value) {
			mDoubleValue = value;
			return this;
		}

		double CDoubleConfigType::getValue() {
			return mDoubleValue;
		}

		bool CDoubleConfigType::equals(CConfigType *dataType) {
			CDoubleConfigType *doubleType = dynamic_cast<CDoubleConfigType *>(dataType);
			if (doubleType) {
				return (mDoubleValue == doubleType->mDoubleValue);
			}
			return false;
		}

		bool CDoubleConfigType::readFromString(const QString &string) {
			bool ok = true;
			double tmp = string.toDouble(&ok);
			if (ok) {
				mDoubleValue = tmp;
			}
			return ok;
		}

		QString CDoubleConfigType::getString() {
			return QString::number(mDoubleValue);
		}

		bool CDoubleConfigType::readFromDouble(double doubleValue) {
			mDoubleValue = doubleValue;
			return true;
		}

		double CDoubleConfigType::getDouble() {
			return mDoubleValue;
		}



	}; // end namespace Config

}; // end namespace Konclude
