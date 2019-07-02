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

#include "CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser() {
			}



			CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser* CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser::addNextStatisticName(const QString& statName) {
				mStatNameList.append(statName);
				return this;
			}


			bool CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);


				QHash< QPair<QString,QString>,double >* reasonerRequestValueHash = mSelectorReasonerRequestValueHash[selector];
				if (!reasonerRequestValueHash) {
					reasonerRequestValueHash = new QHash< QPair<QString,QString>,double >();
					mSelectorReasonerRequestValueHash[selector] = reasonerRequestValueHash;
				}

				bool hasDataSelected = false;

				foreach (QString groupString, groubList) {

					if (selector->isGroupSelected(groupString)) {

						QSet<QString>* groupRequestList = getGroupRequests(groupString);
						foreach (QString requestString, *groupRequestList) {

							if (selector->isRequestSelected(groupString,requestString)) {

								bool allDataAvailable = true;
								foreach (QString reasonerString, reasonerList) {

									bool dataAvailable = false;
									CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
									if (!evalData || evalData->hasDataNotAvailableFlag()) {
										allDataAvailable = false;
									} else if (evalData) {
										CReasonerEvaluationDoubleDataValue* doubleDataValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleDataValue) {
											hasDataSelected = true;
											dataAvailable = true;
											reasonerRequestValueHash->insertMulti(QPair<QString,QString>(reasonerString,groupString+requestString),doubleDataValue->getValue());
										}
									}

									if (!dataAvailable) {
										reasonerRequestValueHash->insertMulti(QPair<QString,QString>(reasonerString,groupString+requestString),0.);
									}
								}

							}
						}
					}
				}





				if (hasDataSelected) {
					foreach (QString reasonerString, reasonerList) {


						QString outputFileString(outputDirectory+mOutputFileName+reasonerString+"-statistic-values");

						CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
						QStringList titleList;
						titleList.append("Request");
						titleList.append(mStatNameList);
						tableMultiFormatOutputWriter.addColumnTitles(titleList);
						bool tableFilled = false;



						QString outputFileGroupString(outputDirectory+mOutputFileName+reasonerString+"-included-grouping-statistic-values");

						CReasonerEvaluationTableMultiFormatOutputWriter<double> groupTableMultiFormatOutputWriter;
						QStringList groupTitleList;
						groupTitleList.append("Request\tGroup");
						groupTitleList.append(mStatNameList);
						groupTableMultiFormatOutputWriter.addColumnTitles(groupTitleList);


						foreach (const QString& groupString, groubList) {

							if (selector->isGroupSelected(groupString)) {

								QSet<QString>* groupRequestList = getGroupRequests(groupString);
								foreach (QString requestString, *groupRequestList) {

									if (selector->isRequestSelected(groupString,requestString)) {

										QList<double> valueList(reasonerRequestValueHash->values(QPair<QString,QString>(reasonerString,groupString+requestString)));
										if (valueList.count() >= mStatNameList.count()) {
											groupTableMultiFormatOutputWriter.addNewTableRow(requestString+"\t"+groupString); 
											tableMultiFormatOutputWriter.addNewTableRow(requestString); 
											QList<double> reverseDoubleValueList;
											foreach (double doubleDataValue, valueList) {
												reverseDoubleValueList.prepend(doubleDataValue);
												tableFilled = true;
											}

											foreach (double doubleValue, reverseDoubleValueList) {
												tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<doubleValue);
												groupTableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<doubleValue);
											}

										}
									}

								}
							}
						}





						if (tableFilled) {
							QStringList reasonerCompGNUPlotFileList;
							QStringList reasonerCompHighchartPlotFileList;

							tableMultiFormatOutputWriter.buildSortedIndex();
							reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTable(outputFileString);
							reasonerCompHighchartPlotFileList += groupTableMultiFormatOutputWriter.writeCVSTable(outputFileGroupString);

							if (context->isGNUPlottingActivated()) {
								reasonerCompGNUPlotFileList += tableMultiFormatOutputWriter.writeGNUPlotDataTable(outputFileString);
								foreach (QString plotFileString, reasonerCompGNUPlotFileList) {
									if (true /*plotFileString.contains("-by-Average-")*/) {
										QString outputFile = plotFileString;
										outputFile.remove(".dat");
										QString title = getPrettyTitleFromOutputFile(outputFile);
										context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,title,mStatNameList,outputFile);
									}
								}
							}
							if (context->isHighchartPlottingActivated()) {
								foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
									if (true /*plotFileString.contains("-by-Average-")*/) {
										QString outputFile = plotFileString;
										outputFile.remove(".csv");
										QString title = getPrettyTitleFromOutputFile(outputFile);
										context->getHighchartPlotter()->createReasonerComparisonPlot(plotFileString,title,mStatNameList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
									}
								}
							}
						}
					}
				}


				return true;
			}


			QString CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
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
