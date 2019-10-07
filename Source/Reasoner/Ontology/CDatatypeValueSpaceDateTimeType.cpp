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

#include "CDatatypeValueSpaceDateTimeType.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatypeValueSpaceDateTimeType::CDatatypeValueSpaceDateTimeType() : CDatatypeValueSpaceCompareType(CDatatypeValueSpaceDateTimeType::VALUESPACEDATETIMETYPE) {
				mMinValue = new CDataLiteralDateTimeValue();
				mMinValue->initValueFromFirstDateTime();
				mMaxValue = new CDataLiteralDateTimeValue();
				mMaxValue->initValueFromLastDateTime();
			}


			CDataLiteralDateTimeValue* CDatatypeValueSpaceDateTimeType::getMinimumDataLiteralDateTimeValue() {
				return mMinValue;
			}

			CDataLiteralDateTimeValue* CDatatypeValueSpaceDateTimeType::getMaximumDataLiteralDateTimeValue() {
				return mMaxValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceDateTimeType::getMinimumDataLiteralCompareValue() {
				return mMinValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceDateTimeType::getMaximumDataLiteralCompareValue() {
				return mMaxValue;
			}

			bool CDatatypeValueSpaceDateTimeType::hasInfiniteManyValues() {
				return true;
			}

			cint64 CDatatypeValueSpaceDateTimeType::getMaximumValueCount() {
				return CINT64_MAX;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
