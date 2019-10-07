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

#include "CReasonerEvaluationResponseFileListSummarizer.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationResponseFileListSummarizer::CReasonerEvaluationResponseFileListSummarizer() {
			}


			CReasonerEvaluationResponseFileListSummarizer::~CReasonerEvaluationResponseFileListSummarizer() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationResponseFileListSummarizer::summarizeEvaluationData(CReasonerEvaluationDataValue* dataValues) {
				CReasonerEvaluationStringValue* respFileValues = dynamic_cast<CReasonerEvaluationStringValue*>(dataValues);
				if (respFileValues) {
					cint64 averagedDataFlags = 0;
					QStringList responseFileList;
					CReasonerEvaluationStringValue* respFileValueIt = respFileValues;
					while (respFileValueIt) {
						responseFileList.append(respFileValueIt->getValue());
						averagedDataFlags |= respFileValueIt->getDataFlags();
						CReasonerEvaluationStringValue* tmpRespFileDataValue = respFileValueIt;
						respFileValueIt = (CReasonerEvaluationStringValue*)respFileValueIt->getNext();
						delete tmpRespFileDataValue;
					}
					CReasonerEvaluationStringListValue* summarizedRespFileDataValues = new CReasonerEvaluationStringListValue(responseFileList);
					summarizedRespFileDataValues->setDataFlags(averagedDataFlags);
					return summarizedRespFileDataValues;
				}
				return nullptr;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
