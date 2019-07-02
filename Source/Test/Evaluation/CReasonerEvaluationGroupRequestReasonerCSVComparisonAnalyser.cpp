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

#include "CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());
				bool hasSelectedRequestFile = false;


				foreach (QString groupString, groubList) {

					if (selector->isGroupSelected(groupString)) {

						QSet<QString>* groupRequestList = getGroupRequests(groupString);
						foreach (QString requestString, *groupRequestList) {
							if (selector->isRequestSelected(groupString,requestString)) {
								hasSelectedRequestFile = true;
							}
						}
					}
				}

				if (hasSelectedRequestFile) {

					QDir outDir(outputDirectory);
					outDir.mkpath(mOutputFileName);
					QFile outputFile(outputDirectory+mOutputFileName+context->getAdditionalFileString()+"time-table.csv");
					if (outputFile.open(QIODevice::WriteOnly)) {

						QString reasonerTabList("OntologyRequest");
						foreach (QString reasonerString, reasonerList) {
							reasonerTabList += QString("\t%1").arg(reasonerString);
						}
						reasonerTabList += QString("\r\n");
						outputFile.write(reasonerTabList.toLocal8Bit());

						foreach (QString groupString, groubList) {

							QSet<QString>* groupRequestList = getGroupRequests(groupString);
							foreach (QString requestString, *groupRequestList) {

								if (selector->isRequestSelected(groupString,requestString)) {

									bool allDataAvailable = true;
									foreach (QString reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										if (!evalData || evalData->hasDataNotAvailableFlag()) {
											allDataAvailable = false;
										}
									}


									if (allDataAvailable) {
										QString requestReasonerTabList(requestString);
										foreach (QString reasonerString, reasonerList) {
											CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
											if (evalData) {
												requestReasonerTabList += QString("\t%1").arg(evalData->generateString());
											} else {
												requestReasonerTabList += QString("\t-");
											}
										}
										requestReasonerTabList += QString("\r\n");
										outputFile.write(requestReasonerTabList.toLocal8Bit());
									}
								}
							}
						}

						outputFile.close();
					}
				}

				return true;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
