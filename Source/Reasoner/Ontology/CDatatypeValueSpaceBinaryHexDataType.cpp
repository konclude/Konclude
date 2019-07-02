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

#include "CDatatypeValueSpaceBinaryHexDataType.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			CDatatypeValueSpaceBinaryHexDataType::CDatatypeValueSpaceBinaryHexDataType() : CDatatypeValueSpaceCompareType(CDatatypeValueSpaceBinaryHexDataType::VALUESPACEHEXDATATYPE) {
				mMinValue = new CDataLiteralBinaryHexDataValue(nullptr);
				mMinValue->initValueFromLength(0);
				mMaxValue = new CDataLiteralBinaryHexDataValue(nullptr);
				mMaxValue->initValueFromInfiniteLength();
			}


			CDataLiteralBinaryHexDataValue* CDatatypeValueSpaceBinaryHexDataType::getMinimumDataLiteralBinaryHexDataValue() {
				return mMinValue;
			}

			CDataLiteralBinaryHexDataValue* CDatatypeValueSpaceBinaryHexDataType::getMaximumDataLiteralBinaryHexDataValue() {
				return mMaxValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceBinaryHexDataType::getMinimumDataLiteralCompareValue() {
				return mMinValue;
			}

			CDataLiteralCompareValue* CDatatypeValueSpaceBinaryHexDataType::getMaximumDataLiteralCompareValue() {
				return mMaxValue;
			}

			bool CDatatypeValueSpaceBinaryHexDataType::hasInfiniteManyValues() {
				return true;
			}

			cint64 CDatatypeValueSpaceBinaryHexDataType::getMaximumValueCount() {
				return CINT64_MAX;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
