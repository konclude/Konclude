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

#include "CReasonerEvaluationAvaragerSummarizer.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationAvaragerSummarizer::CReasonerEvaluationAvaragerSummarizer() {
			}


			CReasonerEvaluationAvaragerSummarizer::~CReasonerEvaluationAvaragerSummarizer() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationAvaragerSummarizer::summarizeEvaluationData(CReasonerEvaluationDataValue* dataValues) {
				CReasonerEvaluationDoubleDataValue* doubDataValues = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(dataValues);
				if (doubDataValues) {
					cint64 averagedDataFlags = 0;
					double doubleValue = 0;
					cint64 valueCount = 0;
					CReasonerEvaluationDoubleDataValue* doubDataValueIt = doubDataValues;
					while (doubDataValueIt) {
						doubleValue += doubDataValueIt->getValue();
						averagedDataFlags |= doubDataValueIt->getDataFlags();
						++valueCount;
						CReasonerEvaluationDoubleDataValue* tmpDoubDataValue = doubDataValueIt;
						doubDataValueIt = (CReasonerEvaluationDoubleDataValue*)doubDataValueIt->getNext();
					}
					if (valueCount != 0) {
						doubleValue /= valueCount;
					}
					CReasonerEvaluationDoubleDataValue* summarizedDoubDataValues = new CReasonerEvaluationDoubleDataValue(doubleValue);
					summarizedDoubDataValues->setDataFlags(averagedDataFlags);
					return summarizedDoubDataValues;
				}
				return nullptr;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
