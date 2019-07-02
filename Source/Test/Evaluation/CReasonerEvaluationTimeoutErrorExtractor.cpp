/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CReasonerEvaluationTimeoutErrorExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationTimeoutErrorExtractor::CReasonerEvaluationTimeoutErrorExtractor(double timelimit) {
				mTimelimit = timelimit;
			}


			CReasonerEvaluationTimeoutErrorExtractor::~CReasonerEvaluationTimeoutErrorExtractor() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationTimeoutErrorExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleErrorValue = nullptr;
				QFile responseFile(responseFileString);
				cint64 errorValue = 0;
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QString respTimeString = rootEl.attribute("response-time","timeout");
					if (respTimeString == "timeout") {
						errorValue = 1;
					}
					if (errorValue == 0) {
						QDomElement classResEl = rootEl.firstChildElement("Error");
						if (!classResEl.isNull()) {
							errorValue = 1;
						}
					}
					responseFile.close();
				}
				doubleErrorValue = new CReasonerEvaluationDoubleDataValue(errorValue);
				return doubleErrorValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
