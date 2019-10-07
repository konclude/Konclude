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

#include "CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser::CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser(CReasonerEvaluationFiltering* filtering) {
				mFiltering = filtering;
			}


			CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser::~CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);
				QString outputFileString(outputDirectory+mOutputFileName+"request-statistics");


				QStringList titleList;
				titleList.append("Statistic");
				titleList.append("Minimum");
				titleList.append("Maximum");
				titleList.append("Mean");
				titleList.append("StandardDerivation");
				titleList.append("Median");
				titleList.append("Sum");


				CReasonerEvaluationTableMultiFormatOutputWriter<double> tableMultiFormatOutputWriter;
				tableMultiFormatOutputWriter.addColumnTitles(titleList);


				QHash<QString,CRequestStatisticsData> statisticNameDataHash;


				bool hasSelectedRequestFile = false;

				foreach (QString groupString, groubList) {

					if (selector->isGroupSelected(groupString)) {

						QSet<QString>* groupRequestList = getGroupRequests(groupString);
						foreach (QString requestString, *groupRequestList) {

							if (selector->isRequestSelected(groupString,requestString)) {



								bool allReasonerResultAvailable = true;
								QString pathRequestString;

								foreach (QString reasonerString, reasonerList) {
									CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
									if (!evalData || evalData->hasDataNotAvailableFlag()) {
										allReasonerResultAvailable = false;
									} else {
										CReasonerEvaluationStringListValue* stringListDataValue = dynamic_cast<CReasonerEvaluationStringListValue*>(evalData);
										if (stringListDataValue) {
											QStringList stringList = stringListDataValue->getValue();
											if (!stringList.isEmpty()) {
												pathRequestString = stringList.first();
											} else {
												allReasonerResultAvailable = false;
											}
										} else {
											allReasonerResultAvailable = false;
										}
									}
								}
								if (allReasonerResultAvailable && !pathRequestString.isEmpty()) {
									hasSelectedRequestFile = true;

									QStringList statisticList = mFiltering->loadStatistics(pathRequestString);
									if (statisticList.isEmpty()) {
										statisticList.append(QString("%1\t%2").arg("StatisticsMissing:").arg("1"));
										statisticList.append(QString("%1\t%2").arg("StatisticsAvailable:").arg("0"));
									} else {
										statisticList.append(QString("%1\t%2").arg("StatisticsMissing:").arg("0"));
										statisticList.append(QString("%1\t%2").arg("StatisticsAvailable:").arg("1"));
									}
									statisticList.append(QString("%1\t%2").arg("RequestFiles:").arg("1"));

									foreach (const QString& statisticString, statisticList) {
										QStringList statisticValueList = statisticString.split("\t");
										if (statisticValueList.count() == 2) {
											QString statisticNameString = statisticValueList.first().trimmed();
											QString statisticValueString = statisticValueList.last().trimmed();
											bool statisticValueConvertionSuccessfull = false;
											double statisticValue = statisticValueString.toDouble(&statisticValueConvertionSuccessfull);
											if (statisticValueConvertionSuccessfull) {
												CRequestStatisticsData& reqStatData = statisticNameDataHash[statisticNameString];
												if (!reqStatData.mInitialized) {
													reqStatData.setDataName(statisticNameString);
													reqStatData.mInitialized = true;
												}
												reqStatData.addData(statisticValue,pathRequestString);
											}
										}
									}
								}



							}
						}
					}
				}

				if (hasSelectedRequestFile) {


					for (QHash<QString,CRequestStatisticsData>::iterator it = statisticNameDataHash.begin(), itEnd = statisticNameDataHash.end(); it != itEnd; ++it) {
						const QString& statisticName(it.key());
						CRequestStatisticsData& statisticData(it.value());
						tableMultiFormatOutputWriter.addNewTableRow(statisticName);
						statisticData.calculateStatistics();
						QList<double> statisticValueList;
						statisticValueList.append(statisticData.mMinValue);
						statisticValueList.append(statisticData.mMaxValue);
						statisticValueList.append(statisticData.mMeanValue);
						statisticValueList.append(statisticData.mStandardDerivationValue);
						statisticValueList.append(statisticData.mMedianValue);
						statisticValueList.append(statisticData.mSumValue);
						tableMultiFormatOutputWriter.addValuesToLastTableRow(statisticValueList);
					}




					QStringList reasonerCompHighchartPlotFileList;

					tableMultiFormatOutputWriter.buildSortedIndex();
					reasonerCompHighchartPlotFileList += tableMultiFormatOutputWriter.writeCVSTable(outputFileString);

					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createRequestStatisticsPlot(plotFileString,context->getAdditionalTitleString()+title,titleList,outputFile,selector->getNameString());
							}
						}
					}


				}

				return true;
			}


			QString CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				return title;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
