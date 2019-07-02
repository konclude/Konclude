/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
