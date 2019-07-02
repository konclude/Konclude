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

#include "CReasonerEvaluationStatisticsValueExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationStatisticsValueExtractor::CReasonerEvaluationStatisticsValueExtractor(const QString& statName, QSet<QString>* statNameSet, double timelimit, double errorPunishmentTime, CReasonerEvaluationDataValueCacher* cacher) {
				mTimelimit = timelimit;
				mErrorPunishmentTime = errorPunishmentTime;
				mCacher = cacher;

				mStatName = statName;
				mStatNameSet = statNameSet;
			}




			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationStatisticsValueExtractor::extractSpecifiedStatisticsValueEvaluationData(QDomDocument& document, const QString& responseFileString, const QString& statName) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,statName));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;
					bool allRespTimeValid = true;
					bool dataAvailable = false;


					QDomElement rootEl = document.documentElement();

					cint64 queryNumber = 0;

					QList<QDomElement> elementList;
					elementList.append(rootEl);

					while (!elementList.isEmpty()) {

						QDomElement element = elementList.takeFirst();


						if (element.hasAttribute("request-command")) {
							QString statName = element.attribute("request-command");
							++queryNumber;
							QString foundStatName;
							if (statName == "Classify") {
								foundStatName = QString("query-%1-classify-response-time").arg(queryNumber);
							} else if (statName == "Realize") {
								foundStatName = QString("query-%1-realize-response-time").arg(queryNumber);
							} else if (statName == "IsKBSatisfiable") {
								foundStatName = QString("query-%1-consistency-response-time").arg(queryNumber);
							}
							if (foundStatName == statName) {
								dataAvailable = true;

								QString statValueString = element.attribute("response-time","0");
								bool validStatValue = false;
								double statValue = statValueString.toDouble(&validStatValue);
								if (validStatValue) {
									doubleValue += statValue;
								}
								allRespTimeValid &= validStatValue;
							}
						}


						if (element.hasAttribute("statistic-name")) {
							QString foundStatName = element.attribute("statistic-name");
							if (foundStatName == statName) {
								dataAvailable = true;

								QString statValueString = element.attribute("statistic-value","0");
								bool validStatValue = false;
								double statValue = statValueString.toDouble(&validStatValue);
								if (validStatValue) {
									doubleValue += statValue;
								}
								allRespTimeValid &= validStatValue;
							}
						}

						if (element.tagName() == "Error") {
							errorOccured = false;
						}
						QString timeoutString = element.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}

						QDomElement childElement = element.firstChildElement();
						while (!childElement.isNull()) {
							elementList.append(childElement);
							childElement = childElement.nextSiblingElement();
						}
					}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);
					if (!dataAvailable) {
						doubleEvalValue->setDataNotAvailableFlag();
					}

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,statName,doubleEvalValue);
					}

				}
				return doubleEvalValue;		
			}




			CReasonerEvaluationDataValue* CReasonerEvaluationStatisticsValueExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDataValue* evalValue = nullptr;

				if (mCacher) {
					evalValue = mCacher->getCachedDataValue(responseFileString,mStatName);
				}

				if (!evalValue) {

					QFile responseFile(responseFileString);
					if (responseFile.open(QIODevice::ReadOnly)) {
						QDomDocument document;
						document.setContent(&responseFile,false);
						QDomElement rootEl = document.documentElement();
						responseFile.close();

						evalValue = extractSpecifiedStatisticsValueEvaluationData(document,responseFileString,mStatName);

						for (QSet<QString>::const_iterator it = mStatNameSet->constBegin(), itEnd = mStatNameSet->constEnd(); it != itEnd; ++it) {
							QString statName(*it);
							if (statName != mStatName) {
								extractSpecifiedStatisticsValueEvaluationData(document,responseFileString,statName);
							}
						}
					}

				}
				if (evalValue) {
					evalValue = evalValue->getCopy();
				}
				return evalValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
