/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CReasonerEvaluationStatisticsNameCollectingExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationStatisticsNameCollectingExtractor::CReasonerEvaluationStatisticsNameCollectingExtractor(QSet<QString>* statisticsNamesSet, CReasonerEvaluationDataValueCacher* cacher) {
				mCacher = cacher;

				mStatisticsNamesSet = statisticsNamesSet;
			}




			bool CReasonerEvaluationStatisticsNameCollectingExtractor::extractEvaluationData(QDomDocument& document, const QString& responseFileString) {

				QDomElement rootEl = document.documentElement();


				QList<QDomElement> elementList;
				elementList.append(rootEl);

				cint64 queryNumber = 0;

				while (!elementList.isEmpty()) {

					QDomElement element = elementList.takeFirst();

					if (element.hasAttribute("statistic-name")) {
						QString statName = element.attribute("statistic-name");
						mStatisticsNamesSet->insert(statName);
					}
					if (element.hasAttribute("request-command")) {
						QString statName = element.attribute("request-command");
						++queryNumber;
						if (statName == "Classify") {
							mStatisticsNamesSet->insert(QString("query-%1-classify-response-time").arg(queryNumber));
						} else if (statName == "Realize") {
							mStatisticsNamesSet->insert(QString("query-%1-realize-response-time").arg(queryNumber));
						} else if (statName == "IsKBSatisfiable") {
							mStatisticsNamesSet->insert(QString("query-%1-consistency-response-time").arg(queryNumber));
						}

					}

					QDomElement childElement = element.firstChildElement();
					while (!childElement.isNull()) {
						elementList.append(childElement);
						childElement = childElement.nextSiblingElement();
					}
				}
				return true;		
			}


			CReasonerEvaluationDataValue* CReasonerEvaluationStatisticsNameCollectingExtractor::extractEvaluationData(const QString& responseFileString) {

				QFile responseFile(responseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					extractEvaluationData(document,responseFileString);
					responseFile.close();
				}


				return nullptr;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
