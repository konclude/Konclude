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

#include "CReasonerEvaluationHighchartPlotter.h"
#include "CReasonerEvaluationAnalyseContext.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationHighchartPlotter::CReasonerEvaluationHighchartPlotter(CReasonerEvaluationAnalyseContext* context) {
				mContext = context;

				mReasCompTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/horizontal-chart.html"));
				mReasCompTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/vertical-chart.html"));

				mReasFasterTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/horizontal-chart.html"));
				mReasFasterTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/vertical-chart.html"));
			
				mReasDiffTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerDifference/horizontal-chart.html"));
				mReasDiffTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerDifference/vertical-chart.html"));

				mReasSubsumptionTemplateStringHash.insert("-horizontal-chart", loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerSubsumption/horizontal-chart.html"));
				mReasSubsumptionTemplateStringHash.insert("-vertical-chart", loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerSubsumption/vertical-chart.html"));

				mReasCountingTemplateStringHash.insert("-horizontal-chart", loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerCounting/horizontal-chart.html"));
				mReasCountingTemplateStringHash.insert("-vertical-chart", loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerCounting/vertical-chart.html"));

				mReasErrorTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerError/horizontal-chart.html"));
				mReasErrorTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerError/vertical-chart.html"));

				mReasAccumTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerAccumulatedTimes/horizontal-chart.html"));
				mReasAccumTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerAccumulatedTimes/vertical-chart.html"));

				mReasAccumCountTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerAccumulatedCount/horizontal-chart.html"));
				mReasAccumCountTemplateStringHash.insert("-vertical-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerAccumulatedCount/vertical-chart.html"));

				mRequestStatisticsTemplateStringHash.insert("-without-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/RequestStatistics/without-chart.html"));

				mReasOverviewTemplateStringHash.insert("-horizontal-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasoenrOverview/horizontal-chart.html"));
				
				mReasWinnerTemplateStringHash.insert("-stacked-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerWinnerCount/stacked-chart.html"));
				mReasWinnerTemplateStringHash.insert("-stacked-percent-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerWinnerCount/stacked-percent-chart.html"));

				mReasStackedBucketTemplateStringHash.insert("-stacked-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerBucket/stacked-chart.html"));
				mReasStackedBucketTemplateStringHash.insert("-stacked-percent-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerBucket/stacked-percent-chart.html"));

				mReasBucketTemplateStringHash.insert("-column-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerBucket/column-chart.html"));
				
				mReasCumulativeBucketTemplateStringHash.insert("-column-chart",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerBucket/cumulative-column-chart.html"));

				mReasCompSplitOverTemplateStringHash.insert("-table-horizontal-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/horizontal-chart-split-merged.html"));
				mReasCompSplitContentTemplateStringHash.insert("-table-horizontal-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/horizontal-chart-split-content.html"));

				mReasCompSplitOverTemplateStringHash.insert("-table-vertical-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/vertical-chart-split-merged.html"));
				mReasCompSplitContentTemplateStringHash.insert("-table-vertical-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerComparison/vertical-chart-split-content.html"));



				mReasFasterSplitOverTemplateStringHash.insert("-table-horizontal-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/horizontal-chart-split-merged.html"));
				mReasFasterSplitContentTemplateStringHash.insert("-table-horizontal-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/horizontal-chart-split-content.html"));

				mReasFasterSplitOverTemplateStringHash.insert("-table-vertical-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/vertical-chart-split-merged.html"));
				mReasFasterSplitContentTemplateStringHash.insert("-table-vertical-chart-split-merged",loadTemplateFromFile("Evaluation/Templates/Highcharts/Charts/ReasonerFasterRatio/vertical-chart-split-content.html"));


				loadReasonerColorData("Evaluation/Templates/Highcharts/Charts/colors.txt",mReasColorHash);
				//mReasColorHash.insert("FaCT++","#E4FECA");
				//mReasColorHash.insert("HermiT","#BBD2FF");
				//mReasColorHash.insert("Pellet","#404040");
				//mReasColorHash.insert("Konclude","#3784A0");
				//mReasColorHash.insert("Average","#F7C298");
				//mReasColorHash.insert("Min","#D5803D");
				//mReasColorHash.insert("Max","#603326");
			}


			CReasonerEvaluationHighchartPlotter::~CReasonerEvaluationHighchartPlotter() {
			}


			QString CReasonerEvaluationHighchartPlotter::loadTemplateFromFile(const QString& fileString) {
				QString dataString;
				QFile file(fileString);
				if (file.open(QIODevice::ReadOnly)) {
					dataString = file.readAll();
				}
				return dataString;
			}

			QMap<QString,cint64> CReasonerEvaluationHighchartPlotter::loadReasonerStringIndexMap(const QString& dataFileString) {
				QFile dataFile(dataFileString);
				QMap<QString,cint64> reasonerStringIndexMap;
				if (dataFile.open(QIODevice::ReadOnly)) {
					QString titleLineString = dataFile.readLine();
					cint64 nextIndex = 0;
					cint64 nextStartPos = 0;
					cint64 nextEndPos = 0;

					bool hasNextTitle = true;
					while (hasNextTitle) {
						nextEndPos = titleLineString.indexOf("\t",nextStartPos);
						if (nextEndPos <= -1) {
							nextEndPos = titleLineString.length();
							hasNextTitle = false;
						}

						QString nextTitle = titleLineString.mid(nextStartPos,nextEndPos-nextStartPos);
						nextTitle.replace("\"","");
						if (!nextTitle.isEmpty()) {
							reasonerStringIndexMap.insert(nextTitle,nextIndex++);
						}
						nextStartPos = nextEndPos+1;

					}
					dataFile.close();
				}
				return reasonerStringIndexMap;
			}


			bool CReasonerEvaluationHighchartPlotter::hasValidColorsForAllReasoenrs(const QStringList& reasonerList) {
				bool validColorsForAllReasoners = true;
				QSet<QString> usedReasonerColorSet;
				foreach (QString reasonerString, reasonerList) {
					QString colorString = getColorString(reasonerString,&usedReasonerColorSet);
					if (colorString.isEmpty()) {
						validColorsForAllReasoners = false;
					}
				}
				return validColorsForAllReasoners;
			}


			bool CReasonerEvaluationHighchartPlotter::loadReasonerColorData(const QString& dataFileString, QHash<QString,QString>& reasColorHash) {
				QFile dataFile(dataFileString);
				if (dataFile.open(QIODevice::ReadOnly)) {
					while (!dataFile.atEnd()) {
						QString dataString = dataFile.readLine();
						if (!dataString.startsWith("#")) {
							QStringList dataStringList = dataString.split("\t");
							if (dataStringList.count() >= 2) {
								QString nameMatchString = dataStringList.takeFirst();
								QString colorString = dataStringList.takeFirst();
								reasColorHash.insert(nameMatchString,colorString);
							}
						}
					}
					return true;
				}
				return false;
			}


			bool CReasonerEvaluationHighchartPlotter::loadReasonerComparisonData(const QString& dataFileString, QStringList& labelList, QMap<QString,QStringList>& valueListMap) {
				QFile dataFile(dataFileString);
				if (dataFile.open(QIODevice::ReadOnly)) {

					QStringList titleList;
					QString titleLineString = dataFile.readLine();
					cint64 nextIndex = 0;
					cint64 nextStartPos = 0;
					cint64 nextEndPos = 0;

					bool hasNextTitle = true;
					while (hasNextTitle) {
						nextEndPos = titleLineString.indexOf("\t",nextStartPos);
						if (nextEndPos <= -1) {
							nextEndPos = titleLineString.length();
							hasNextTitle = false;
						}
						QString nextTitle = titleLineString.mid(nextStartPos,nextEndPos-nextStartPos);
						titleList.append(nextTitle.trimmed());
						nextStartPos = nextEndPos+1;
					}

					QList< QStringList > dataList;
					while (!dataFile.atEnd()) {
						QString dataString = dataFile.readLine();
						QStringList dataStringList = dataString.split("\t");
						QList< QStringList >::iterator it = dataList.begin(), itEnd = dataList.end();
						if (!dataStringList.isEmpty()) {
							QString valueString = dataStringList.takeFirst();
							labelList.append(valueString.replace("-request.xml",""));
						}
						while (!dataStringList.isEmpty()) {
							QString valueString = dataStringList.takeFirst();
							if (it == itEnd) {
								dataList.append(QStringList()<<valueString);
							} else {
								QStringList& valueList = *it;
								valueList.append(valueString);
								++it;
							}
						}
					}
					titleList.removeFirst();
					foreach (QStringList valueList, dataList) {
						if (!titleList.isEmpty()) {
							QString title = titleList.takeFirst();
							valueListMap.insert(title,valueList);
						}
					}
					return true;
				}
				return false;
			}





			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerComparisonSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QString& selectionString) {
				return sortAndCreateReasonerSplitPlot(dataFileStringList,plotTitleList,reasonerNameList,mReasCompSplitOverTemplateStringHash,mReasCompSplitContentTemplateStringHash,selectionString);
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerFasterSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QString& selectionString) {
				return sortAndCreateReasonerSplitPlot(dataFileStringList,plotTitleList,reasonerNameList,mReasFasterSplitOverTemplateStringHash,mReasFasterSplitContentTemplateStringHash,selectionString);
			}


			bool caseInsensitiveLessThan(const QString &s1, const QString &s2) {
				return s1.toLower() < s2.toLower();
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::sortAndCreateReasonerSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QHash<QString,QString>& reasTemplateOverStringHash, const QHash<QString,QString>& reasTemplateContStringHash, const QString& selectionString) {
				QHash<QString,QStringList> sortDataFileHash;
				QHash<QString,QString> sortPlotTitleHash;
				for (QStringList::const_iterator it = dataFileStringList.constBegin(), itEnd = dataFileStringList.constEnd(), itT = plotTitleList.constBegin(), itTEnd = plotTitleList.constEnd(); it != itEnd && itT != itTEnd; ++it, ++itT) {
					const QString& dataFileString(*it);
					QString plotTitleString(*itT);
					QString sortString = dataFileString.mid(0,dataFileString.indexOf("-split"));
					sortDataFileHash[sortString].prepend(dataFileString);
					plotTitleString = plotTitleString.mid(0,plotTitleString.indexOf("split"));
					sortPlotTitleHash[sortString] = plotTitleString;
				}
				for (QHash<QString,QStringList>::const_iterator it = sortDataFileHash.constBegin(), itEnd = sortDataFileHash.constEnd(); it != itEnd; ++it) {
					QString sortString(it.key());
					QStringList sortDataFileString(it.value());
					const QString plotTitleString = sortPlotTitleHash.value(sortString);
					qSort(sortDataFileString.begin(),sortDataFileString.end(),caseInsensitiveLessThan);
					createReasonerSplitPlot(sortDataFileString,plotTitleString,reasonerNameList,sortString,reasTemplateOverStringHash,reasTemplateContStringHash,selectionString);
				}
				return this;
			}



			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerSplitPlot(const QStringList& dataFileStringList, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QHash<QString,QString>& reasTemplateOverStringHash, const QHash<QString,QString>& reasTemplateContStringHash, const QString& selectionString) {


				for (QHash<QString,QString>::const_iterator itTemplate = reasTemplateOverStringHash.constBegin(), itTemplateEnd = reasTemplateOverStringHash.constEnd(); itTemplate != itTemplateEnd; ++itTemplate) {


					QString plotName = itTemplate.key();
					QString plotOverviewString = itTemplate.value();

					QString outputFileString(outputFile+plotName+".html");


					QString plotContentMergedString;
					cint64 partNumber = 1;


					foreach (const QString& dataFileString, dataFileStringList) {
						QMap<QString,cint64> reasonerStringIndexMap = loadReasonerStringIndexMap(dataFileString);

						QString plotContentString = reasTemplateContStringHash.value(plotName);

						plotContentString = plotContentString.replace("$$_PLOT_TITLE_$$",plotTitle);
						plotContentString = plotContentString.replace("$$_PLOT_SUB_TITLE_$$",QString("Part %1 of %2").arg(partNumber).arg(dataFileStringList.count()));
						plotContentString = plotContentString.replace("$$_PLOT_CONTAINER_ID_$$",QString::number(partNumber++));

						QStringList reasonerAndAverageList(reasonerNameList);
						if (reasonerStringIndexMap.contains("Average")) {
							reasonerAndAverageList.append(QString("Average"));
						}

						bool validColorsForAllReasoners = hasValidColorsForAllReasoenrs(reasonerAndAverageList);


						QStringList labelList;
						QMap<QString,QStringList> valueListMap;

						if (loadReasonerComparisonData(dataFileString,labelList,valueListMap)) {


							QString seriesDataString;
							QString labelString = QString("'%1'").arg(labelList.join("', '"));
							QString colorString;
							foreach (QString reasonerString, reasonerNameList) {
								QString plotReasonerString;
								QStringList valueList = valueListMap.value(reasonerString);
								plotReasonerString = QString("{type: 'column',name: '%1',data: [%2]},").arg(reasonerString).arg(valueList.join(", "));
								seriesDataString += plotReasonerString;

								if (validColorsForAllReasoners) {
									colorString += QString("'%1',").arg(getColorString(reasonerString));
								}

							}

							if (reasonerStringIndexMap.contains("Average")) {
								QString plotReasonerString;
								QStringList valueList = valueListMap.value("Average");
								plotReasonerString = QString("{type: 'spline',name: '%1',data: [%2]}").arg("Average").arg(valueList.join(", "));
								seriesDataString += plotReasonerString;
								if (validColorsForAllReasoners) {
									colorString += QString("'%1',").arg(getColorString("Average"));
								}
							}

							if (validColorsForAllReasoners) {
								colorString = QString("colors: [%1], ").arg(colorString);
							}

							plotContentString.replace("$$_PLOT_DATA_SERIES_$$",seriesDataString);
							plotContentString.replace("$$_PLOT_LABELS_$$",labelString);
							plotContentString.replace("$$_PLOT_COLORS_$$",colorString);
							plotContentString.replace("$$_PLOT_DATE_$$",QDateTime::currentDateTime().toString());
							plotContentString.replace("$$_PLOT_DATA_DOWNLOAD_CSV_$$",getFileNameWithoutDirectory(dataFileString));
							plotContentString.replace("$$_PLOT_DATA_DOWNLOAD_HTML_$$",getFileNameWithoutDirectory(outputFileString));
							plotContentString.replace("$$_PLOT_DATA_SELECTION_$$",selectionString);

							cint64 dataCount50n60 = labelList.count()*60+50;
							plotContentString.replace("$$_PLOT_DATA_COUNT_50+n*60_$$",QString::number(dataCount50n60));

							plotContentMergedString += plotContentString;

						}
					}

					if (!plotContentMergedString.isEmpty()) {

						plotOverviewString.replace("$$_DOCUMENT_TITLE_$$",plotTitle);
						plotOverviewString.replace("$$_DOCUMENT_CONTENT_$$",plotContentMergedString);
						plotOverviewString.replace("$$_PLOT_DATE_$$",QDateTime::currentDateTime().toString());
						plotOverviewString.replace("$$_PLOT_DATA_DOWNLOAD_HTML_$$",getFileNameWithoutDirectory(outputFileString));
						plotOverviewString.replace("$$_PLOT_DATA_SELECTION_$$",selectionString);

						QFile outputScriptFile(outputFileString);
						if (outputScriptFile.open(QIODevice::WriteOnly)) {
							outputScriptFile.write(plotOverviewString.toLocal8Bit());
							outputScriptFile.close();

						}
					}
				}

				return this;
			}





			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerFasterPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasFasterTemplateStringHash,selectionString);
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerComparisonPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasCompTemplateStringHash,selectionString);
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerDifferencePlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasDiffTemplateStringHash,selectionString);
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerSubsumptionPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString, plotTitle, reasonerNameList, outputFile, mReasSubsumptionTemplateStringHash, selectionString);
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerCountingPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString, plotTitle, reasonerNameList, outputFile, mReasCountingTemplateStringHash, selectionString);
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerErrorPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasErrorTemplateStringHash,selectionString);
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerAccumulatedPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasAccumTemplateStringHash,selectionString);
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerAccumulatedCountPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasAccumCountTemplateStringHash,selectionString);
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createRequestStatisticsPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mRequestStatisticsTemplateStringHash,selectionString);
			}


			QString CReasonerEvaluationHighchartPlotter::getFileNameWithoutDirectory(const QString& fileNameString) {
				QString fileNameWDString = fileNameString;
				cint64 lastSlash = fileNameWDString.lastIndexOf("/");
				if (lastSlash != -1) {
					fileNameWDString = fileNameWDString.mid(lastSlash+1);
				}
				return fileNameWDString;
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QHash<QString,QString>& reasTemplateStringHash, const QString& selectionString, const QString& curveString) {

				QMap<QString,cint64> reasonerStringIndexMap = loadReasonerStringIndexMap(dataFileString);


				for (QHash<QString,QString>::const_iterator itTemplate = reasTemplateStringHash.constBegin(), itTemplateEnd = reasTemplateStringHash.constEnd(); itTemplate != itTemplateEnd; ++itTemplate) {

					QString plotName = itTemplate.key();
					QString plotContentString = itTemplate.value();

					QString outputFileString(outputFile+plotName+".html");

					plotContentString.replace("$$_PLOT_TITLE_$$",plotTitle);

					QStringList reasonerAndAverageList(reasonerNameList);
					if (reasonerStringIndexMap.contains("Average")) {
						reasonerAndAverageList.append(QString("Average"));
					}

					bool validColorsForAllReasoners = hasValidColorsForAllReasoenrs(reasonerAndAverageList);


					QStringList labelList;
					QMap<QString,QStringList> valueListMap;

					if (loadReasonerComparisonData(dataFileString,labelList,valueListMap)) {

						QFile outputScriptFile(outputFileString);
						if (outputScriptFile.open(QIODevice::WriteOnly)) {

							QString seriesDataString;
							QString labelString = QString("'%1'").arg(labelList.join("', '"));
							QString colorString;
							foreach (QString reasonerString, reasonerNameList) {
								QString plotReasonerString;
								QStringList valueList = valueListMap.value(reasonerString);
								plotReasonerString = QString("{%1name: '%2',data: [%4]},").arg(curveString).arg(reasonerString).arg(valueList.join(", "));
								seriesDataString += plotReasonerString;

								if (validColorsForAllReasoners) {
									colorString += QString("'%1',").arg(getColorString(reasonerString));
								}

							}

							if (reasonerStringIndexMap.contains("Average")) {
								QString plotReasonerString;
								QStringList valueList = valueListMap.value("Average");
								plotReasonerString = QString("{type: 'spline',name: '%1',data: [%2]}").arg("Average").arg(valueList.join(", "));
								seriesDataString += plotReasonerString;
								if (validColorsForAllReasoners) {
									colorString += QString("'%1',").arg(getColorString("Average"));
								}
							}

							if (validColorsForAllReasoners) {
								colorString = QString("colors: [%1], ").arg(colorString);
							}

							plotContentString.replace("$$_PLOT_DATA_SERIES_$$",seriesDataString);
							plotContentString.replace("$$_PLOT_LABELS_$$",labelString);
							plotContentString.replace("$$_PLOT_COLORS_$$",colorString);
							plotContentString.replace("$$_PLOT_DATE_$$",QDateTime::currentDateTime().toString());
							plotContentString.replace("$$_PLOT_DATA_DOWNLOAD_CSV_$$",getFileNameWithoutDirectory(dataFileString));
							plotContentString.replace("$$_PLOT_DATA_DOWNLOAD_HTML_$$",getFileNameWithoutDirectory(outputFileString));
							plotContentString.replace("$$_PLOT_DATA_SELECTION_$$",selectionString);

							cint64 dataCount50n60 = labelList.count()*60+50;
							plotContentString.replace("$$_PLOT_DATA_COUNT_50+n*60_$$",QString::number(dataCount50n60));

							outputScriptFile.write(plotContentString.toLocal8Bit());

							outputScriptFile.close();
						}
					}
				}


				return this;
			}


			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerWinnerCountPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasWinnerTemplateStringHash,selectionString,"");
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerStackedBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasStackedBucketTemplateStringHash,selectionString,"");
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasBucketTemplateStringHash,selectionString,"");
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerCumulativeBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasCumulativeBucketTemplateStringHash,selectionString,"");
			}

			CReasonerEvaluationHighchartPlotter* CReasonerEvaluationHighchartPlotter::createReasonerOverviewPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QString& selectionString) {
				return createReasonerPlot(dataFileString,plotTitle,reasonerNameList,outputFile,mReasOverviewTemplateStringHash,selectionString,"type: 'spline',");
			}



			QString CReasonerEvaluationHighchartPlotter::getColorString(const QString& reasonerString, QSet<QString>* usedReasonerColorSet) {
				QString reasonerColorString(reasonerString);
				QString colorString;
				if (mReasColorHash.contains(reasonerColorString)) {
					colorString = mReasColorHash.value(reasonerColorString);
				} else {
					cint64 versionSplitSpacePos = reasonerColorString.indexOf(" ");
					if (versionSplitSpacePos >= 0) {
						reasonerColorString = reasonerColorString.mid(0,versionSplitSpacePos);
						if (mReasColorHash.contains(reasonerColorString)) {
							colorString = mReasColorHash.value(reasonerColorString);
						}
					}
				}
				if (usedReasonerColorSet && !colorString.isEmpty()) {
					if (usedReasonerColorSet->contains(reasonerColorString)) {
						colorString.clear();
					} else {
						usedReasonerColorSet->insert(reasonerColorString);
					}
				}
				return colorString;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
