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

#include "CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Analyse {


			CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser::CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser(qint64 valueLimit, double minOffset, double maxOffset) : CMinMaxAvgLimitedCountValueAnalyser(valueLimit) {
				minOff = minOffset;
				maxOff = maxOffset;
			}



			CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser::~CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser() {
			}

			double CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser::getMaxValue() {
				double diff = maxVal-minVal;
				return maxVal+diff*maxOff;
			}

			double CMinOffsetMaxOffsetAvgLimitedCountValueAnalyser::getMinValue() {
				double diff = maxVal-minVal;
				return minVal-diff*minOff;
			}


		}; // end namespace Analyse

	}; // end namespace Test

}; // end namespace Konclude
