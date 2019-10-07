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

#include "CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser(double bucketsStart, double bucketsEnd, double bucketsIncreaseFactor, double timeoutsEnd) {
				mTimeoutsEnd = timeoutsEnd;
				mBucketsStart = bucketsStart;
				mBucketsEnd = bucketsEnd;
				mBucketsIncreaseFactor = bucketsIncreaseFactor;
			}


			bool CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {


				QList<double> bucketsEndList;
				cint64 bucketCount = 1;
				bucketsEndList.append(mBucketsStart);
				double nextBucketsEnd = mBucketsStart;
				do {
					nextBucketsEnd *= mBucketsIncreaseFactor;
					bucketsEndList.append((cint64)qMin(nextBucketsEnd+0.5,mBucketsEnd));
					++bucketCount;
				} while (nextBucketsEnd < mBucketsEnd);
				cint64 bucketTimeoutErrorCount = bucketCount+2;

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				cint64 reasonerCount = reasonerList.count();
				bool hasSelectedRequestFile = false;

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);

				cint64** bucketTable = new cint64*[reasonerCount];
				for (cint64 i = 0; i < reasonerCount; ++i) {
					cint64* tmpVec = new cint64[bucketTimeoutErrorCount];
					bucketTable[i] = tmpVec;
					for (cint64 j = 0; j < bucketTimeoutErrorCount; ++j) {
						tmpVec[j] = 0;
					}
				}

				cint64** cummBucketTable = new cint64*[reasonerCount];
				for (cint64 i = 0; i < reasonerCount; ++i) {
					cint64* tmpVec = new cint64[bucketTimeoutErrorCount];
					cummBucketTable[i] = tmpVec;
					for (cint64 j = 0; j < bucketTimeoutErrorCount; ++j) {
						tmpVec[j] = 0;
					}
				}


				QHash<QString,cint64*> reasonerBucketHash;
				QHash<QString,cint64*> reasonerCummulativeBucketHash;
				cint64 index = 0;
				foreach (const QString& reasonerString, reasonerList) {
					reasonerBucketHash.insert(reasonerString,bucketTable[index]);
					reasonerCummulativeBucketHash.insert(reasonerString,cummBucketTable[index]);
					++index;
				}

				cint64 requestCount = 0;

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
									requestCount++;


									QString requestReasonerTabList(requestString);
									foreach (const QString& reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										CReasonerEvaluationDoubleDataValue* doubleEvalData = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleEvalData) {
											double doubleValue = doubleEvalData->getValue();
											cint64 bucketID = 0;
											if (doubleValue > mTimeoutsEnd) {
												bucketID = bucketTimeoutErrorCount-1;
											} else if (doubleValue > mBucketsEnd) {
												bucketID = bucketTimeoutErrorCount-2;
											} else {
												foreach (double bucketEnd, bucketsEndList) {
													if (doubleValue < bucketEnd) {
														break;
													}
													++bucketID;
												}
											}
											cint64* bucketVec = reasonerBucketHash.value(reasonerString);
											++bucketVec[bucketID];


											cint64* cummBucketVec = reasonerCummulativeBucketHash.value(reasonerString);
											++cummBucketVec[bucketID];
											for (cint64 nextBucketId = bucketID+1; nextBucketId < bucketCount; ++nextBucketId) {
												++cummBucketVec[nextBucketId];
											}

										}
									}
								}

							}
						}
					}
				}



				if (hasSelectedRequestFile) {

					QStringList bucketList;
					QStringList cumulativeBucketList;

					CReasonerEvaluationTableMultiFormatOutputWriter<cint64> bucketOutputWriter;

					for (cint64 i = 0; i < bucketTimeoutErrorCount; ++i) {
						QString title;
						QString cumulativeTitle;
						if (i < bucketCount) {
							double bucketStart = 0;
							if (i > 0) {
								bucketStart = bucketsEndList.at(i-1);
							}
							double bucketEnd = bucketsEndList.at(i);
							title = QString("%1-%2").arg(bucketStart).arg(bucketEnd);
							cumulativeTitle = QString("<%1").arg(bucketEnd);
						} else if (i == bucketCount) {
							title = "timeout";
							cumulativeTitle = "timeout";
						} else {
							title = "error";
							cumulativeTitle = "error";
						}
						bucketList.append(title);
						cumulativeBucketList.append(cumulativeTitle);
					}

					bucketOutputWriter.addColumnTitles(QStringList()<<"Reasoner"<<bucketList);
					foreach (const QString& reasonerString, reasonerList) {
						bucketOutputWriter.addNewTableRow(reasonerString);
						for (cint64 bucketID = 0; bucketID < bucketTimeoutErrorCount; ++bucketID) {
							cint64* bucketVec = reasonerBucketHash.value(reasonerString);
							cint64 bucketData = bucketVec[bucketID];
							bucketOutputWriter.addValuesToLastTableRow(QList<cint64>()<<bucketData);
						}
					}
					QStringList reasonerCompHighchartPlotFileList = bucketOutputWriter.writeCVSTable(QString(outputDirectory+mOutputFileName+"%1-stacked-buckets").arg(bucketCount));
					if (context->isHighchartPlottingActivated()) {
						foreach (const QString& plotFileString, reasonerCompHighchartPlotFileList) {
							QString outputFile = plotFileString;
							outputFile.remove(".csv");
							QString title = getPrettyTitleFromOutputFile(outputFile);
							context->getHighchartPlotter()->createReasonerStackedBucketPlot(plotFileString,context->getAdditionalTitleString()+" grouped by "+title+"",bucketList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
						}
					}








					// write in columns
					CReasonerEvaluationTableMultiFormatOutputWriter<cint64> bucketOutputWriter2;

					bucketOutputWriter2.addColumnTitles(QStringList()<<"Reasoner"<<reasonerList);

					for (cint64 i = 0; i < bucketTimeoutErrorCount; ++i) {
						QString title = bucketList.at(i);
						bucketOutputWriter2.addNewTableRow(title);
						foreach (const QString& reasonerString, reasonerList) {
							cint64* bucketVec = reasonerBucketHash.value(reasonerString);
							cint64 bucketData = bucketVec[i];
							bucketOutputWriter2.addValuesToLastTableRow(QList<cint64>()<<bucketData);
						}
					}
					QStringList reasonerColumnCompHighchartPlotFileList = bucketOutputWriter2.writeCVSTable(QString(outputDirectory+mOutputFileName+"%1-column-buckets").arg(bucketCount));
					if (context->isHighchartPlottingActivated()) {
						foreach (const QString& plotFileString, reasonerColumnCompHighchartPlotFileList) {
							QString outputFile = plotFileString;
							outputFile.remove(".csv");
							QString title = getPrettyTitleFromOutputFile(outputFile);
							context->getHighchartPlotter()->createReasonerBucketPlot(plotFileString,context->getAdditionalTitleString()+" grouped by "+title+"",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
						}
					}











					// write cumulative in columns
					CReasonerEvaluationTableMultiFormatOutputWriter<cint64> bucketOutputWriter3;

					bucketOutputWriter3.addColumnTitles(QStringList()<<"Reasoner"<<reasonerList);

					for (cint64 i = 0; i < bucketTimeoutErrorCount; ++i) {
						QString title = cumulativeBucketList.at(i);
						bucketOutputWriter3.addNewTableRow(title);
						foreach (const QString& reasonerString, reasonerList) {
							cint64* cummBucketVec = reasonerCummulativeBucketHash.value(reasonerString);
							cint64 bucketData = cummBucketVec[i];
							bucketOutputWriter3.addValuesToLastTableRow(QList<cint64>()<<bucketData);
						}
					}
					QStringList reasonerCumulativeColumnCompHighchartPlotFileList = bucketOutputWriter3.writeCVSTable(QString(outputDirectory+mOutputFileName+"%1-cumulative-column-buckets").arg(bucketCount));
					if (context->isHighchartPlottingActivated()) {
						foreach (const QString& plotFileString, reasonerCumulativeColumnCompHighchartPlotFileList) {
							QString outputFile = plotFileString;
							outputFile.remove(".csv");
							QString title = getPrettyTitleFromOutputFile(outputFile);
							context->getHighchartPlotter()->createReasonerCumulativeBucketPlot(plotFileString,context->getAdditionalTitleString()+" grouped by "+title+"",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
						}
					}
				}


				for (cint64 i = 0; i < reasonerCount; ++i) {
					delete [] bucketTable[i];
				}
				delete [] bucketTable;

				return true;
			}





			QString CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("stacked","Stacked");
				title.replace("column","Column");
				title.replace("buckets","Buckets");
				title.replace("cumulative","Cumulative");
				title.replace("grouped","Grouped");
				return title;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
