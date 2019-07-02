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

#include "CReasonerEvaluationClassificationTimeExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationClassificationTimeExtractor::CReasonerEvaluationClassificationTimeExtractor(double timelimit, double errorPunishmentTime, CReasonerEvaluationDataValueCacher* cacher) {
				mTimelimit = timelimit;
				mErrorPunishmentTime = errorPunishmentTime;
				mCacher = cacher;

				mExtractorType = CLASSIFICATIONTIMEEXTRACTOR;
			}


			CReasonerEvaluationClassificationTimeExtractor::~CReasonerEvaluationClassificationTimeExtractor() {
			}

			CReasonerEvaluationDataValue* CReasonerEvaluationClassificationTimeExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)mExtractorType));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;
					bool allRespTimeValid = true;
					QFile responseFile(responseFileString);
					if (responseFile.open(QIODevice::ReadOnly)) {
						QDomDocument document;
						document.setContent(&responseFile,false);
						QDomElement rootEl = document.documentElement();
						responseFile.close();


						QString timeoutString = rootEl.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}

						QDomElement childEl = rootEl.firstChildElement();
						while (!childEl.isNull()) {
							timeoutString = rootEl.attribute("timeout");
							if (timeoutString == "1") {
								timeoutOccured = true;
							}

							QString requestCommandString = childEl.attribute("request-command");
							if (requestCommandString == "GetSubClassHierarchy" || requestCommandString == "Classify" || requestCommandString == "GetSubClasses" || requestCommandString == "GetSuperClasses" || requestCommandString == "GetEquivalentClasses") {
								bool validResponseTime = false;
								if (childEl.tagName() != "Error") {
									QString respTimeString = childEl.attribute("response-time","timeout");
									double respTime = respTimeString.toDouble(&validResponseTime);
									if (validResponseTime) {
										doubleValue += respTime;
									}
								} else {
									validResponseTime = false;
								}
								allRespTimeValid &= validResponseTime;
							}
							childEl = childEl.nextSiblingElement();
						}

						if (timeoutOccured) {
							doubleValue = mTimelimit;
						}
						if (mTimelimit >= 0) {
							doubleValue = qMin(mTimelimit,doubleValue);
						}
						if (errorOccured || !allRespTimeValid) {
							if (mTimelimit >= 0) {
								doubleValue = mTimelimit+mErrorPunishmentTime;
							} else {
								doubleValue = mErrorPunishmentTime;
							}
						}
						doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);

						if (mCacher) {
							mCacher->addDataValueToCache(responseFileString,(cint64)mExtractorType,doubleEvalValue);
						}
					}

				}
				return doubleEvalValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
