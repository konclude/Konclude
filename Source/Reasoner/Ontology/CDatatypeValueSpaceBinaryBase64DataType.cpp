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

#include "CDatatypeValueSpaceBinaryBase64DataType.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatypeValueSpaceBinaryBase64DataType::CDatatypeValueSpaceBinaryBase64DataType() : CDatatypeValueSpaceCompareType(CDatatypeValueSpaceBinaryBase64DataType::VALUESPACEBASE64DATATYPE) {
				mMinValue = new CDataLiteralBinaryBase64DataValue(nullptr);
				mMinValue->initValueFromLength(0);
				mMaxValue = new CDataLiteralBinaryBase64DataValue(nullptr);
				mMaxValue->initValueFromInfiniteLength();
			}


			CDataLiteralBinaryBase64DataValue* CDatatypeValueSpaceBinaryBase64DataType::getMinimumDataLiteralBinaryBase64DataValue() {
				return mMinValue;
			}

			CDataLiteralBinaryBase64DataValue* CDatatypeValueSpaceBinaryBase64DataType::getMaximumDataLiteralBinaryBase64DataValue() {
				return mMaxValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceBinaryBase64DataType::getMinimumDataLiteralCompareValue() {
				return mMinValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceBinaryBase64DataType::getMaximumDataLiteralCompareValue() {
				return mMaxValue;
			}


			bool CDatatypeValueSpaceBinaryBase64DataType::hasInfiniteManyValues() {
				return true;
			}

			cint64 CDatatypeValueSpaceBinaryBase64DataType::getMaximumValueCount() {
				return CINT64_MAX;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
