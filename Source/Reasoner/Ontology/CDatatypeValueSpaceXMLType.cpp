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

#include "CDatatypeValueSpaceXMLType.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatypeValueSpaceXMLType::CDatatypeValueSpaceXMLType() : CDatatypeValueSpaceCompareType(CDatatypeValueSpaceXMLType::VALUESPACEXMLTYPE) {
				mMinValue = new CDataLiteralXMLValue();
				mMinValue->initValueFromLength(0);
				mMaxValue = new CDataLiteralXMLValue();
				mMaxValue->initValueFromInfiniteLength();
			}


			CDataLiteralXMLValue* CDatatypeValueSpaceXMLType::getMinimumDataLiteralXMLValue() {
				return mMinValue;
			}

			CDataLiteralXMLValue* CDatatypeValueSpaceXMLType::getMaximumDataLiteralXMLValue() {
				return mMaxValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceXMLType::getMinimumDataLiteralCompareValue() {
				return mMinValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceXMLType::getMaximumDataLiteralCompareValue() {
				return mMaxValue;
			}

			bool CDatatypeValueSpaceXMLType::hasInfiniteManyValues() {
				return true;
			}

			cint64 CDatatypeValueSpaceXMLType::getMaximumValueCount() {
				return CINT64_MAX;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
