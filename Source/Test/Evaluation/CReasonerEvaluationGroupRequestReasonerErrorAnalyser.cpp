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

#include "CReasonerEvaluationGroupRequestReasonerErrorAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerErrorAnalyser::CReasonerEvaluationGroupRequestReasonerErrorAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerErrorAnalyser::~CReasonerEvaluationGroupRequestReasonerErrorAnalyser() {
			}



			bool CReasonerEvaluationGroupRequestReasonerErrorAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				bool hasSelectedRequestFile = false;

				QStringList titleList;
				titleList.append("Requests");
				titleList.append(reasonerList);
				titleList.append("Average");
				titleList.append("Min");
				titleList.append("Max");

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);
				QString outputFileString(outputDirectory+mOutputFileName+"error");

				CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
				tableMultiFormatOutputWriter.addColumnTitles(titleList);

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
											double errorValue = doubleDataValue->getValue();

											tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<errorValue);

											mean += errorValue;
											min = qMin(min,errorValue);
											max = qMax(max,errorValue); 
											count++;
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
					reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTablesSorted(outputFileString);
					tableMultiFormatOutputWriter.writeCVSTablesSeparateSorted(outputFileString);

					if (context->isGNUPlottingActivated()) {
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTable(outputFileString);
						reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTablesSorted(outputFileString);
						foreach (QString plotFileString, reasonerCompGNUPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".dat");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,context->getAdditionalTitleString()+title+"[error/timeout]",reasonerList,outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerErrorPlot(plotFileString,context->getAdditionalTitleString()+title+"[error/timeout]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}

				}

				return true;
			}



			QString CReasonerEvaluationGroupRequestReasonerErrorAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("error","Reaoner Errors and Timeouts, ");
				title.replace("sorted","Sorted");
				if (title.contains(" ascending")) {
					title.replace(" ascending","");
					title.append(" in Ascending Order ");
				}
				if (title.contains(" descending")) {
					title.replace(" descending","");
					title.append(" in Descending Order ");
				}
				return title;
			}
		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
