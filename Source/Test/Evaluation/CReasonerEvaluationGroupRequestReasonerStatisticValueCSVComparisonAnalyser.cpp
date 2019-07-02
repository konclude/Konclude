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

#include "CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);
				QString outputFileString(outputDirectory+mOutputFileName+context->getAdditionalFileString()+"values");


				QStringList titleList;
				titleList.append("Requests");
				titleList.append(reasonerList);
				titleList.append("Average");
				titleList.append("Min");
				titleList.append("Max");

				CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
				tableMultiFormatOutputWriter.addColumnTitles(titleList);


				bool hasSelectedRequestFile = false;

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
									double min = DOUBLE_MAX;
									double max = DOUBLE_MIN;
									double mean = 0.;
									cint64 count = 0;

									tableMultiFormatOutputWriter.addNewTableRow(requestString);

									foreach (QString reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										CReasonerEvaluationDoubleDataValue* doubleDataValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleDataValue) {

											double tmpValue = doubleDataValue->getValue();
											tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<tmpValue);

											mean += tmpValue;
											min = qMin(min,tmpValue);
											max = qMax(max,tmpValue); 
											count++;
										} else {
											tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<0);
										}
									}
									mean /= (double)count;
									tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<mean<<min<<max);

								}
							}
						}
					}
				}

				if (hasSelectedRequestFile) {

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
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,title,reasonerList,outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerComparisonPlot(plotFileString,title,reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}




					QStringList mmmExtendedReasonerListGNU(reasonerList);
					mmmExtendedReasonerListGNU<<"Average"<<"Max"<<"Min";
					QStringList reasonerOverPlotFileList;
					if (context->isGNUPlottingActivated()) {
						reasonerOverPlotFileList = tableMultiFormatOutputWriter.writeGNUPlotDataTablesSeparateSorted(outputFileString);
						foreach (QString plotFileString, reasonerOverPlotFileList) {
							if (true) {
								QString outputFile = plotFileString;
								outputFile.remove(".dat");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getGNUPlotPlotter()->createReasonerOverviewPlot(plotFileString,title,reasonerList,outputFile);
								context->getGNUPlotPlotter()->createReasonerOverviewPlot(plotFileString,title,mmmExtendedReasonerListGNU,outputFile+"-mmm");
							}
						}
					}

					QStringList mmmExtendedReasonerListHC(reasonerList);
					mmmExtendedReasonerListHC<<"Max"<<"Min";
					QStringList reasonerHCOverPlotFileList;
					if (context->isHighchartPlottingActivated()) {
						reasonerHCOverPlotFileList = tableMultiFormatOutputWriter.writeCVSTablesSeparateSorted(outputFileString);
						foreach (QString plotFileString, reasonerHCOverPlotFileList) {
							if (true) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerOverviewPlot(plotFileString,title,reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
								context->getHighchartPlotter()->createReasonerOverviewPlot(plotFileString,title,mmmExtendedReasonerListHC,outputFile+"-mmm",selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}
				}

				return true;
			}


			QString CReasonerEvaluationGroupRequestReasonerStatisticValueCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("mmm","Comparision,");
				title.replace("sorted","Sorted");
				title.replace("separate","Separate");

				return title;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
