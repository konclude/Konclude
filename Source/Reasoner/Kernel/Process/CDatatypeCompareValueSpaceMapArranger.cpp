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

#include "CDatatypeCompareValueSpaceMapArranger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CDatatypeCompareValueSpaceMapArranger::CDatatypeCompareValueSpaceMapArranger(CDataLiteralCompareValue* value) {
					mValue = value;
				}


				CDataLiteralCompareValue* CDatatypeCompareValueSpaceMapArranger::getValue() const {
					return mValue;
				}

				bool CDatatypeCompareValueSpaceMapArranger::operator<(const CDatatypeCompareValueSpaceMapArranger& valueData) const {
					return mValue->isLessThan(valueData.mValue);
				}

				bool CDatatypeCompareValueSpaceMapArranger::operator<=(const CDatatypeCompareValueSpaceMapArranger& valueData) const {
					return mValue->isLessEqualThan(valueData.mValue);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
