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

#include "CDataLiteralDateTimeValue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CDataLiteralDateTimeValue::CDataLiteralDateTimeValue() {
				clearValue();
			}


			CDataLiteralValue::DATA_LITERAL_VALUE_TYPE CDataLiteralDateTimeValue::getDataValueType() {
				return CDataLiteralValue::DLVT_DATETIME;
			}


			CDataLiteralCompareValue* CDataLiteralDateTimeValue::initValue(CDataLiteralCompareValue* value) {
				clearValue();
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value); 
				if (dateTimeValue) {
					mFirstDateTime = dateTimeValue->mFirstDateTime;
					mLastDateTime = dateTimeValue->mLastDateTime;
					mDateTimeValue = dateTimeValue->mDateTimeValue;
					mZoneOffset = dateTimeValue->mZoneOffset;
				}
				return this;
			}


			CDataLiteralDateTimeValue* CDataLiteralDateTimeValue::initValueFromFirstDateTime() {
				clearValue();
				mFirstDateTime = true;
				return this;
			}


			CDataLiteralDateTimeValue* CDataLiteralDateTimeValue::initValueFromLastDateTime() {
				clearValue();
				mLastDateTime = true;
				return this;
			}

			CDataLiteralDateTimeValue* CDataLiteralDateTimeValue::initValueFromDataTime(const QDateTime& dateTime, cint64 zoneOffset) {
				clearValue();
				mDateTimeValue = dateTime;
				mZoneOffset = zoneOffset;
				return this;
			}



			CDataLiteralDateTimeValue* CDataLiteralDateTimeValue::initValue(CDataLiteralDateTimeValue* value) {
				if (value) {
					mFirstDateTime = value->mFirstDateTime;
					mLastDateTime = value->mLastDateTime;
					mDateTimeValue = value->mDateTimeValue;
					mZoneOffset = value->mZoneOffset;
				}
				return this;
			}


			CDataLiteralDateTimeValue* CDataLiteralDateTimeValue::clearValue() {
				mFirstDateTime = false;
				mLastDateTime = false;
				mDateTimeValue = QDateTime();
				mZoneOffset = 0;
				return this;
			}


			bool CDataLiteralDateTimeValue::isFirstDateTime() {
				return mFirstDateTime;
			}

			bool CDataLiteralDateTimeValue::isLastDateTime() {
				return mLastDateTime;
			}

			QDateTime CDataLiteralDateTimeValue::getDateTimeValue() {
				return mDateTimeValue;
			}

			cint64 CDataLiteralDateTimeValue::getTimeZoneOffset() {
				return mZoneOffset;
			}

			bool CDataLiteralDateTimeValue::isEqualTo(CDataLiteralValue* value) {
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value);
				if (dateTimeValue) {
					return isEqualTo(dateTimeValue);
				}
				return false;
			}



			bool CDataLiteralDateTimeValue::isLessEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value);
				if (dateTimeValue) {
					return isEqualTo(dateTimeValue) || isLessThan(dateTimeValue);
				}
				return false;
			}

			bool CDataLiteralDateTimeValue::isLessEqualThan(CDataLiteralDateTimeValue* dateTimeValue) {
				return isEqualTo(dateTimeValue) || isLessThan(dateTimeValue);
			}


			bool CDataLiteralDateTimeValue::isEqualTo(CDataLiteralCompareValue* value) {
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value);
				if (dateTimeValue) {
					return isEqualTo(dateTimeValue);
				}
				return false;
			}

			bool CDataLiteralDateTimeValue::isEqualTo(CDataLiteralDateTimeValue* dateTimeValue) {
				if (mFirstDateTime && dateTimeValue->mFirstDateTime) {
					return true;
				}
				if (mFirstDateTime || dateTimeValue->mFirstDateTime) {
					return false;
				}
				if (mLastDateTime && dateTimeValue->mLastDateTime) {
					return true;
				}
				if (mLastDateTime || dateTimeValue->mLastDateTime) {
					return false;
				}
				if (mDateTimeValue == dateTimeValue->mDateTimeValue && mZoneOffset == dateTimeValue->mZoneOffset) {
					return true;
				}
				return false;
			}

			bool CDataLiteralDateTimeValue::isGreaterEqualThan(CDataLiteralDateTimeValue* dateTimeValue) {
				return isEqualTo(dateTimeValue) || isGreaterThan(dateTimeValue);
			}

			bool CDataLiteralDateTimeValue::isGreaterEqualThan(CDataLiteralCompareValue* value) {
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value);
				if (dateTimeValue) {
					return isEqualTo(dateTimeValue) || isGreaterThan(dateTimeValue);
				}
				return false;
			}

			bool CDataLiteralDateTimeValue::isGreaterThan(CDataLiteralCompareValue* value) {
				return value->isLessThan(this);
			}

			bool CDataLiteralDateTimeValue::isGreaterThan(CDataLiteralDateTimeValue* dateTimeValue) {
				return dateTimeValue->isLessThan(this);
			}


			bool CDataLiteralDateTimeValue::isLessThan(CDataLiteralCompareValue* value) {
				CDataLiteralDateTimeValue* dateTimeValue = dynamic_cast<CDataLiteralDateTimeValue*>(value);
				if (dateTimeValue) {
					return isLessThan(dateTimeValue);
				}
				return false;
			}


			bool CDataLiteralDateTimeValue::incrementDateTimeValue() {
				if (mFirstDateTime || mLastDateTime) {
					return false;
				}
				if (mZoneOffset >= 840) {
					QDateTime tmpDateTime = mDateTimeValue.addSecs(1);
					if (!tmpDateTime.isValid()) {
						return false;
					}
					mDateTimeValue = tmpDateTime;
					mZoneOffset = -840;
				} else {
					mZoneOffset++;
				}
				return true;
			}



			bool CDataLiteralDateTimeValue::isLessThan(CDataLiteralDateTimeValue* dateTimeValue) {
				if (!mLastDateTime && dateTimeValue->mLastDateTime) {
					return true;
				}
				if (mLastDateTime) {
					return false;
				}
				if (dateTimeValue->mFirstDateTime) {
					return false;
				}
				if (mFirstDateTime) {
					return true;
				}
				if (mDateTimeValue < dateTimeValue->mDateTimeValue) {
					return true;
				} 
				if (mDateTimeValue > dateTimeValue->mDateTimeValue) {
					return false;
				}
				return mZoneOffset < dateTimeValue->mZoneOffset;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
