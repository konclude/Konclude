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

#include "CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);
				QString outputFileString(outputDirectory+mOutputFileName+context->getAdditionalFileString()+"time-mmm");
				QString outputBestFileString(outputDirectory+mOutputFileName+context->getAdditionalFileString()+"time-ratio");


				QStringList titleList;
				titleList.append("Requests");
				titleList.append(reasonerList);
				titleList.append("Average");
				titleList.append("Min");
				titleList.append("Max");

				QStringList bestTitleList;
				bestTitleList.append("Requests");
				foreach (const QString& reasonerString, reasonerList) {
					bestTitleList.append(reasonerString);
				}
				bestTitleList.append("Average");

				CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
				tableMultiFormatOutputWriter.addColumnTitles(titleList);

				CReasonerEvaluationTableMultiFormatOutputWriter<double> bestTableMultiFormatOutputWriter;
				bestTableMultiFormatOutputWriter.addColumnTitles(bestTitleList);

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
									bestTableMultiFormatOutputWriter.addNewTableRow(requestString);

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
										}
									}
									mean /= (double)count;
									tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<mean<<min<<max);


									foreach (QString reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										CReasonerEvaluationDoubleDataValue* doubleDataValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleDataValue) {
											double tmpValue = doubleDataValue->getValue();
											if (tmpValue > mean) {
												tmpValue = - (tmpValue / mean - 1);
											} else {
												tmpValue = (mean / tmpValue - 1);
											}
											bestTableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<tmpValue);
										}
									}
									bestTableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<mean);
								}
							}
						}
					}
				}

				if (hasSelectedRequestFile) {

					QStringList reasonerCompGNUPlotFileList;
					QStringList reasonerCompHighchartPlotFileList;
					QStringList reasonerCompHighchartSplitPlotFileList;

					tableMultiFormatOutputWriter.buildSortedIndex();
					reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTable(outputFileString);
					reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTablesSorted(outputFileString);

					reasonerCompHighchartSplitPlotFileList += tableMultiFormatOutputWriter.writeCVSTablesSortedSplitted(30,outputFileString);


					if (context->isGNUPlottingActivated()) {
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTable(outputFileString);
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTablesSorted(outputFileString);
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTablesSortedSplitted(30,outputFileString);
						foreach (QString plotFileString, reasonerCompGNUPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".dat");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",reasonerList,outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerComparisonPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
						if (!reasonerCompHighchartSplitPlotFileList.isEmpty()) {
							QStringList reasonerCompHighchartSplitPlotTitleList;
							foreach (QString plotFileString, reasonerCompHighchartSplitPlotFileList) {
								if (true /*plotFileString.contains("-by-Average-")*/) {
									QString outputFile = plotFileString;
									outputFile.remove(".csv");
									QString title = getPrettyTitleFromOutputFile(outputFile);
									reasonerCompHighchartSplitPlotTitleList.append(context->getAdditionalTitleString()+title+"[milliseconds]");
								}
							}
							context->getHighchartPlotter()->createReasonerComparisonSplitPlot(reasonerCompHighchartSplitPlotFileList,reasonerCompHighchartSplitPlotTitleList,reasonerList,selector->getNameString()+" :: "+context->getAdditionalTitleString());
						}			
					}



					QStringList reasonerCompHighchartBestPlotFileList;
					QStringList reasonerCompHighchartBestSplitPlotFileList;

					bestTableMultiFormatOutputWriter.buildSortedIndex();
					reasonerCompHighchartBestPlotFileList += bestTableMultiFormatOutputWriter.writeCVSTable(outputBestFileString);
					reasonerCompHighchartBestPlotFileList += bestTableMultiFormatOutputWriter.writeCVSTablesSorted(outputBestFileString);
					reasonerCompHighchartBestSplitPlotFileList += bestTableMultiFormatOutputWriter.writeCVSTablesSortedSplitted(30,outputBestFileString);

					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartBestPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerFasterPlot(plotFileString,context->getAdditionalTitleString()+title+"[times faster than Average]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
						if (!reasonerCompHighchartBestSplitPlotFileList.isEmpty()) {
							QStringList reasonerCompHighchartSplitPlotTitleList;
							foreach (QString plotFileString, reasonerCompHighchartBestSplitPlotFileList) {
								if (true /*plotFileString.contains("-by-Average-")*/) {
									QString outputFile = plotFileString;
									outputFile.remove(".csv");
									QString title = getPrettyTitleFromOutputFile(outputFile);
									reasonerCompHighchartSplitPlotTitleList.append(context->getAdditionalTitleString()+title+"[times faster than Average]");
								}
							}
							context->getHighchartPlotter()->createReasonerFasterSplitPlot(reasonerCompHighchartBestSplitPlotFileList,reasonerCompHighchartSplitPlotTitleList,reasonerList,selector->getNameString()+" :: "+context->getAdditionalTitleString());
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
								context->getGNUPlotPlotter()->createReasonerOverviewPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",reasonerList,outputFile);
								context->getGNUPlotPlotter()->createReasonerOverviewPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",mmmExtendedReasonerListGNU,outputFile+"-mmm");
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
								context->getHighchartPlotter()->createReasonerOverviewPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
								context->getHighchartPlotter()->createReasonerOverviewPlot(plotFileString,context->getAdditionalTitleString()+title+"[milliseconds]",mmmExtendedReasonerListHC,outputFile+"-mmm",selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}
				}

				return true;
			}


			QString CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				if (title.contains("sorted")) {
					title.replace("ratio","Ratio to Average,");
				} else {
					title.replace("ratio","Ratio to Average");
				}
				title.replace("time mmm","Time Comparision,");
				title.replace("sorted","Sorted");
				title.replace("time","Time,");
				title.replace("separate","Separate");
				if (title.contains(" ascending")) {
					title.replace(" ascending","");
					if (title.contains(" split")) {
						title.insert(title.indexOf(" split")," in Ascending Order");
					} else {
						title.append("in Ascending Order ");
					}
				}
				if (title.contains(" descending")) {
					title.replace(" descending","");
					if (title.contains(" split")) {
						title.insert(title.indexOf(" split")," in Descending Order");
					} else {
						title.append("in Descending Order ");
					}
				}
				return title;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
