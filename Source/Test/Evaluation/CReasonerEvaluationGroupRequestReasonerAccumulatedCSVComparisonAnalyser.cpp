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

#include "CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());
				bool hasSelectedRequestFile = false;

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);
				QString outputFileString(outputDirectory+mOutputFileName+context->getAdditionalFileString()+"accumulated-time");

				QVector<double> accumTimeVec;
				foreach (QString reasonerString, reasonerList) {
					accumTimeVec.append(0.);
				}

				foreach (QString groupString, groubList) {

					if (selector->isGroupSelected(groupString)) {


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

									hasSelectedRequestFile = true;

									cint64 reasonerNumber = 0;

									foreach (QString reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										CReasonerEvaluationDoubleDataValue* doubleDataValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleDataValue) {
											double tmpValue = doubleDataValue->getValue();
											accumTimeVec[reasonerNumber] += tmpValue;
											reasonerNumber++;
										}
									}
								}
							}
						}
					}
				}



				if (hasSelectedRequestFile) {
					QStringList titleList;
					titleList.append("Reasoner");
					titleList.append("Accumulated Time");

					CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
					tableMultiFormatOutputWriter.addColumnTitles(titleList);

					cint64 reasonerNumber = 0;
					foreach (QString reasonerString, reasonerList) {

						tableMultiFormatOutputWriter.addNewTableRow(reasonerString);
						double reasonerAccTime = accumTimeVec[reasonerNumber++];
						tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<reasonerAccTime);
					}


					QStringList reasonerCompGNUPlotFileList;
					QStringList reasonerCompHighchartPlotFileList;

					tableMultiFormatOutputWriter.buildSortedIndex();
					reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTable(outputFileString);

					if (context->isGNUPlottingActivated()) {
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTable(outputFileString);
						foreach (QString plotFileString, reasonerCompGNUPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".dat");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,title+"[milliseconds]",QStringList()<<"Accumulated Time",outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerAccumulatedPlot(plotFileString,title+"[milliseconds]",QStringList()<<"Accumulated Time",outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}

				}

				return true;
			}


			QString CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("classification","Classification");
				title.replace("loading","Loading");
				title.replace("response","Response");
				title.replace("accumulated ","");
				title.insert(0,"Accumulated ");
				title.replace("time","Time");
				return title;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
