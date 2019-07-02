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

#include "CReasonerEvaluationLoadingTimeExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationLoadingTimeExtractor::CReasonerEvaluationLoadingTimeExtractor(double timelimit) {
				mTimelimit = timelimit;
			}


			CReasonerEvaluationLoadingTimeExtractor::~CReasonerEvaluationLoadingTimeExtractor() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationLoadingTimeExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;
				double doubleValue = 0;
				QFile responseFile(responseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QDomElement responseElement = rootEl.firstChildElement();
					bool allRespTimeValid = true;
					bool oneRespTimeValid = false;
					if (!responseElement.isNull()) {
						while (!responseElement.isNull()) {
							QString requestCommand = responseElement.attribute("request-command");
							if (requestCommand == "LoadOntologies" || requestCommand == "GraphLoad") {
								QString respTimeString = responseElement.attribute("response-time","timeout");
								bool validRespTime = false;
								double respTime = respTimeString.toDouble(&validRespTime);
								allRespTimeValid &= validRespTime;
								oneRespTimeValid |= validRespTime;
								doubleValue += respTime;
							}
							responseElement = responseElement.nextSiblingElement();
						}
					} 
					if (!oneRespTimeValid) {
						QString respTimeString = rootEl.attribute("response-time","timeout");
						bool validRespTime = false;
						double respTime = respTimeString.toDouble(&validRespTime);
						allRespTimeValid &= validRespTime;
						oneRespTimeValid |= validRespTime;
						doubleValue += respTime;
					}
					responseFile.close();
					if (oneRespTimeValid) {
						if (mTimelimit >= 0) {
							doubleValue = qMin(mTimelimit,doubleValue);
						}
						doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);
					}
				}
				return doubleEvalValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
