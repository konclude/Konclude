/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser::CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser() {
			}


			CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser::~CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser() {
			}


			bool CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

				mOutputFileName = QString("%1").arg(selector->getOutputString());

				cint64 reasonerCount = reasonerList.count();
				bool hasSelectedRequestFile = false;

				QDir outDir(outputDirectory);
				outDir.mkpath(mOutputFileName);

				cint64** winnerTable = new cint64*[reasonerCount];
				for (cint64 i = 0; i < reasonerCount; ++i) {
					cint64* tmpVec = new cint64[reasonerCount];
					winnerTable[i] = tmpVec;
					for (cint64 j = 0; j < reasonerCount; ++j) {
						tmpVec[j] = 0;
					}
				}

				cint64** timesFasterTable = new cint64*[reasonerCount];
				for (cint64 i = 0; i < reasonerCount; ++i) {
					cint64* tmpVec = new cint64[reasonerCount];
					timesFasterTable[i] = tmpVec;
					for (cint64 j = 0; j < reasonerCount; ++j) {
						tmpVec[j] = 0;
					}
				}

				double** fasterTable = new double*[reasonerCount];
				for (cint64 i = 0; i < reasonerCount; ++i) {
					double* tmpVec = new double[reasonerCount];
					fasterTable[i] = tmpVec;
					for (cint64 j = 0; j < reasonerCount; ++j) {
						tmpVec[j] = 0.;
					}
				}

				QHash<QString,cint64*> reasonerWinnerCountHash;
				QHash<QString,cint64*> reasonerTimesFasterCountHash;
				QHash<QString,double*> reasonerFasterHash;
				QHash<QString,cint64> reasonerIndexHash;
				cint64 index = 0;
				foreach (QString reasonerString, reasonerList) {
					reasonerWinnerCountHash.insert(reasonerString,winnerTable[index]);
					reasonerTimesFasterCountHash.insert(reasonerString,timesFasterTable[index]);
					reasonerFasterHash.insert(reasonerString,fasterTable[index]);
					reasonerIndexHash.insert(reasonerString,index++);
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

									QMap<double,QString> timeReasonerMap;

									QString requestReasonerTabList(requestString);
									foreach (QString reasonerString, reasonerList) {
										CReasonerEvaluationDataValue* evalData = getGroupRequestReasonerEvaluationData(groupString,requestString,reasonerString);
										CReasonerEvaluationDoubleDataValue* doubleEvalData = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(evalData);
										if (doubleEvalData) {
											timeReasonerMap.insertMulti(doubleEvalData->getValue(),reasonerString);
										}
									}

									cint64 index = 0;
									for (QMap<double,QString>::const_iterator it = timeReasonerMap.constBegin(), itEnd = timeReasonerMap.constEnd(); it != itEnd; ++it) {
										QString reasonerString = it.value();
										cint64* tmpWinnerVec = reasonerWinnerCountHash.value(reasonerString);
										++tmpWinnerVec[index];

										cint64* tmpTimesFasterVec = reasonerTimesFasterCountHash.value(reasonerString);
										QMap<double,QString>::const_iterator it2 = it;
										for (++it2; it2 != itEnd; ++it2) {
											QString slowerReasonerString = it2.value();
											cint64 slowerReasonerIndex = reasonerIndexHash.value(slowerReasonerString);
											++tmpTimesFasterVec[slowerReasonerIndex];
										}

										double* tmpFasterVec = reasonerFasterHash.value(reasonerString);
										for (QMap<double,QString>::const_iterator it2 = timeReasonerMap.constBegin(); it2 != itEnd; ++it2) {
											if (it != it2) {
												double val1 = it.key();
												double val2 = it2.key();
												QString otherReasonerString = it2.value();
												cint64 otherReasonerIndex = reasonerIndexHash.value(otherReasonerString);
												double faster = (val2 / val1)-1;
												if (val2 < val1) {
													faster = -((val1 / val2)-1);
												}
												if (val2 == val1 || val1 == 0 || val2 == 0) {
													faster = 0.;
												}
												tmpFasterVec[otherReasonerIndex] += faster;
											}
										}


										++index;
									}
								}
							}
						}
					}
				}


				for (cint64 i = 0; i < reasonerCount; ++i) {
					for (cint64 j = 0; j < reasonerCount; ++j) {
						fasterTable[i][j] = fasterTable[i][j] / (double)requestCount;
					}
				}

				if (hasSelectedRequestFile) {

					CReasonerEvaluationTableMultiFormatOutputWriter<cint64> winnerCountOutputWriter;
					winnerCountOutputWriter.addColumnTitles(QStringList()<<"Ranking"<<reasonerList);
					for (cint64 i = 0; i < reasonerCount; ++i) {
						QString title = QString("# Rank %1").arg(i+1);
						winnerCountOutputWriter.addNewTableRow(title);
						foreach (QString reasonerString, reasonerList) {
							cint64* tmpVec = reasonerWinnerCountHash.value(reasonerString);
							cint64 rankCount = tmpVec[i];
							winnerCountOutputWriter.addValuesToLastTableRow(QList<cint64>()<<rankCount);
						}
					}
					QStringList reasonerCompHighchartPlotFileList = winnerCountOutputWriter.writeCVSTable(outputDirectory+mOutputFileName+"winner-count");
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							QString outputFile = plotFileString;
							outputFile.remove(".csv");
							QString title = getPrettyTitleFromOutputFile(outputFile);
							context->getHighchartPlotter()->createReasonerWinnerCountPlot(plotFileString,context->getAdditionalTitleString()+title+" and Rankings",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
						}
					}


					CReasonerEvaluationTableMultiFormatOutputWriter<cint64> fasterCountOutputWriter;
					fasterCountOutputWriter.addColumnTitles(QStringList()<<"Reasoner"<<reasonerList);
					foreach (QString fasterReasonerString, reasonerList) {
						QString title = QString("# %1 is faster").arg(fasterReasonerString);
						fasterCountOutputWriter.addNewTableRow(title);
						cint64* tmpVec = reasonerTimesFasterCountHash.value(fasterReasonerString);
						foreach (QString reasonerString, reasonerList) {
							cint64 otherReasonerIndex = reasonerIndexHash.value(reasonerString);
							cint64 fasterCount = tmpVec[otherReasonerIndex];
							fasterCountOutputWriter.addValuesToLastTableRow(QList<cint64>()<<fasterCount);
						}
					}
					fasterCountOutputWriter.writeCVSTable(outputDirectory+mOutputFileName+"faster-count");


					CReasonerEvaluationTableMultiFormatOutputWriter<double> avgFasterOutputWriter;
					avgFasterOutputWriter.addColumnTitles(QStringList()<<"Reasoner"<<reasonerList);
					foreach (QString fasterReasonerString, reasonerList) {
						QString title = QString("average faster ratio for %1").arg(fasterReasonerString);
						avgFasterOutputWriter.addNewTableRow(title);
						double* tmpVec = reasonerFasterHash.value(fasterReasonerString);
						foreach (QString reasonerString, reasonerList) {
							cint64 otherReasonerIndex = reasonerIndexHash.value(reasonerString);
							double fasterRatio = tmpVec[otherReasonerIndex];
							avgFasterOutputWriter.addValuesToLastTableRow(QList<double>()<<fasterRatio);
						}
					}
					avgFasterOutputWriter.writeCVSTable(outputDirectory+mOutputFileName+"faster-average");
				}


				for (cint64 i = 0; i < reasonerCount; ++i) {
					delete [] winnerTable[i];
					delete [] fasterTable[i];
					delete [] timesFasterTable[i];
				}
				delete [] winnerTable;
				delete [] fasterTable;
				delete [] timesFasterTable;

				return true;
			}





			QString CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("winner","Winner");
				title.replace("count","Count");
				return title;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
