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

#include "CDatatypeValueSpaceFloatType.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatypeValueSpaceFloatType::CDatatypeValueSpaceFloatType() : CDatatypeValueSpaceCompareType(CDatatypeValueSpaceFloatType::VALUESPACEFLOATTYPE) {
				mMinValue = new CDataLiteralFloatValue();
				mMinValue->initValueFromInfinite(true);
				mMaxValue = new CDataLiteralFloatValue();
				mMaxValue->initValueFromInfinite(false);
			}


			CDataLiteralFloatValue* CDatatypeValueSpaceFloatType::getMinimumDataLiteralFloatValue() {
				return mMinValue;
			}

			CDataLiteralFloatValue* CDatatypeValueSpaceFloatType::getMaximumDataLiteralFloatValue() {
				return mMaxValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceFloatType::getMinimumDataLiteralCompareValue() {
				return mMinValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceFloatType::getMaximumDataLiteralCompareValue() {
				return mMaxValue;
			}

			bool CDatatypeValueSpaceFloatType::hasInfiniteManyValues() {
				return false;
			}

			cint64 CDatatypeValueSpaceFloatType::getMaximumValueCount() {
				return CINT_MAX-2;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
