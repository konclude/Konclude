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
					QDomElement classResEl = rootEl.firstChildElement();
					bool allRespTimeValid = true;
					bool oneRespTimeValid = false;
					if (!classResEl.isNull()) {
						while (!classResEl.isNull()) {
							if (classResEl.attribute("request-command") == "LoadOntologies") {
								QString respTimeString = classResEl.attribute("response-time","timeout");
								bool validRespTime = false;
								double respTime = respTimeString.toDouble(&validRespTime);
								allRespTimeValid &= validRespTime;
								oneRespTimeValid |= validRespTime;
								doubleValue += respTime;
							}
							classResEl = classResEl.nextSiblingElement();
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
