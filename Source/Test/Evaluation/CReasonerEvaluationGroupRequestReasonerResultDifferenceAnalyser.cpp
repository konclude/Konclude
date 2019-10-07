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

#include "CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::~CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser() {
			}



			cint64 CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::compareResultsSimilarityWithOtherReasoners(const QString& groupString, const QString& requestString, const QString& reasonerString, CReasonerEvaluationStringListValue* stringListDataValue, const QStringList& reasonerList, const QString& detailedReportingOutputDirectory) {
				cint64 totalDifferentResultCount = 0;
				const QStringList& responseFileStringList = stringListDataValue->getValue();
				if (!responseFileStringList.isEmpty()) {
					QString responseFileString = responseFileStringList.first();
					QFile responseFile(responseFileString);

					bool fileOpened = false;
					QDomDocument document;
					CClassHierarchyResult* lastClassHierResult = nullptr;
					cint64 lastClassHierResultNumber = 0;


					foreach (const QString& otherReasoner, reasonerList) {
						if (otherReasoner != reasonerString) {
							cint64 reasonerDifferentResultCount = 0;


							CReasonerEvaluationDataValue* otherReasonerEvalData = getGroupRequestReasonerEvaluationData(groupString,requestString,otherReasoner);
							CReasonerEvaluationStringListValue* otherReasonerStringListDataValue = dynamic_cast<CReasonerEvaluationStringListValue*>(otherReasonerEvalData);
							if (otherReasonerStringListDataValue) {
								const QStringList& otherResponseFileStringList = otherReasonerStringListDataValue->getValue();
								if (!otherResponseFileStringList.isEmpty()) {
									CReasonerEvaluationStringListValue* minVal = qMin(stringListDataValue,otherReasonerStringListDataValue);
									CReasonerEvaluationStringListValue* maxVal = qMax(stringListDataValue,otherReasonerStringListDataValue);

									if (mReasonerCompareDiffCountHash.contains(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal))) {
										reasonerDifferentResultCount += mReasonerCompareDiffCountHash.value(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal));
									} else {

										if (!fileOpened) {
											if (responseFile.open(QIODevice::ReadOnly)) {
												document.setContent(&responseFile,false);
												responseFile.close();
											}
											fileOpened = true;
										}
										QDomElement rootEl = document.documentElement();
										QDomElement resultElement = rootEl.firstChildElement();
										bool foundResult = false;
										bool foundError = false;
										cint64 nodeNumber = 0;
										if (!resultElement.isNull()) {
											while (!resultElement.isNull()) {

												if (resultElement.nodeName() == "Error") {
													foundError = true;
													break;

												} else if (resultElement.nodeName() == "ClassHierarchy" || resultElement.nodeName() == "BooleanResponse" || resultElement.nodeName() == "Classes" || resultElement.nodeName() == "results") {
													foundResult = true;
													CClassHierarchyResult* classHierResult = nullptr;
													if (lastClassHierResult && lastClassHierResultNumber == nodeNumber) {
														classHierResult = lastClassHierResult;
													}
													CBooleanQueryResult* booleanResult = nullptr;
													CClassesResult* classesResult = nullptr;
													CVariableBindingsAnswersSetResult* variableBindingsResult = nullptr;

													cint64 diffCount = 0;

													QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>*& compareDiffCountHash = mNumberCompareDiffCountHashHash[nodeNumber];
													if (!compareDiffCountHash) {
														compareDiffCountHash = new QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>();
													}

													if (compareDiffCountHash->contains(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal))) {
														diffCount += compareDiffCountHash->value(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal));
													} else {

														if (resultElement.nodeName() == "ClassHierarchy") {
															if (!classHierResult) {
																classHierResult = mOWLlinkResultParser.parseClassHierarchyResult(&resultElement);
															}

															QString otherResponseFileString = otherResponseFileStringList.first();
															LOG(INFO,"::Konclude::Test::Evaluation::ResultDifferenceAnalyser",logTr("Comparing classification results between '%1' and '%2'.").arg(responseFileString).arg(otherResponseFileString),this);
															if (!isClassHierarchyResultSimilarTo(classHierResult,nodeNumber,otherResponseFileString)) {
																diffCount = 1;
															}
														} else if (resultElement.nodeName() == "BooleanResponse") {
															if (!booleanResult) {
																booleanResult = mOWLlinkResultParser.parseBooleanQueryResult(&resultElement);
															}

															QString otherResponseFileString = otherResponseFileStringList.first();
															LOG(INFO,"::Konclude::Test::Evaluation::ResultDifferenceAnalyser",logTr("Comparing boolean results between '%1' and '%2'.").arg(responseFileString).arg(otherResponseFileString),this);
															if (!isBooleanResultSimilarTo(booleanResult,nodeNumber,otherResponseFileString)) {
																diffCount = 1;
															}
														} else if (resultElement.nodeName() == "Classes") {
															if (!classesResult) {
																classesResult = mOWLlinkResultParser.parseClassesResult(&resultElement);
															}

															QString otherResponseFileString = otherResponseFileStringList.first();
															LOG(INFO,"::Konclude::Test::Evaluation::ResultDifferenceAnalyser",logTr("Comparing classes results between '%1' and '%2'.").arg(responseFileString).arg(otherResponseFileString),this);
															if (!isClassesResultSimilarTo(classesResult,nodeNumber,otherResponseFileString)) {
																diffCount = 1;
															}
														} else if (resultElement.nodeName() == "results") {
															// handle SPARQL binding results
															QHash<QString, cint64> variableNameIndexHash;
															if (!variableBindingsResult) {
																variableBindingsResult = mSPARQLResultParser.parseVariableBindingsSet(&resultElement, &variableNameIndexHash, true);
															}

															QString otherResponseFileString = otherResponseFileStringList.first();
															QString reportingFileName = responseFileString + "--vs--" + otherResponseFileString + ".txt";
															reportingFileName = reportingFileName.replace("/", "_").replace(":", "_");
															reportingFileName = detailedReportingOutputDirectory + reportingFileName;
															LOG(INFO, "::Konclude::Test::Evaluation::ResultDifferenceAnalyser", logTr("Comparing variable binding results between '%1' and '%2', reporting details to '%3'.").arg(responseFileString).arg(otherResponseFileString).arg(reportingFileName), this);
															if (!isVariableBindingResultSimilarTo(variableBindingsResult, nodeNumber, otherResponseFileString, &variableNameIndexHash, reportingFileName)) {
																diffCount = 1;
															}
														}
														compareDiffCountHash->insert(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal),diffCount);
													}
													reasonerDifferentResultCount += diffCount;
													if (classHierResult) {
														if (!lastClassHierResult) {
															lastClassHierResult = classHierResult;
															lastClassHierResultNumber = nodeNumber;
														} else {
															if (lastClassHierResult != classHierResult) {
																delete classHierResult;
															}
														}
													}
													if (booleanResult) {
														delete booleanResult;
													}
													if (classesResult) {
														delete classesResult;
													}
													if (variableBindingsResult) {
														delete variableBindingsResult;
													}

												} 
												if (resultElement.nodeName() != "answering-statistics") {
													++nodeNumber;
												}
												resultElement = resultElement.nextSiblingElement();
											}
										}

										mReasonerCompareDiffCountHash.insert(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal),reasonerDifferentResultCount);

									}
								}
							}

							totalDifferentResultCount += reasonerDifferentResultCount;

						}
					}
					if (lastClassHierResult) {
						delete lastClassHierResult;
					}

				}
				return totalDifferentResultCount;
			}



			bool CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::isClassHierarchyResultSimilarTo(CClassHierarchyResult* classHierResult, cint64 nodeNumber, const QString& otherResponseFileString) {
				bool resultSimilar = true;
				QFile responseFile(otherResponseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QDomElement classResEl = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					if (!classResEl.isNull()) {
						while (!classResEl.isNull()) {
							if (classResEl.nodeName() == "Error") {
								break;

							} else if (currNodeNumber == nodeNumber && classResEl.nodeName() == "ClassHierarchy") {
								CClassHierarchyResult* otherClassHierResult = mOWLlinkResultParser.parseClassHierarchyResult(&classResEl);
								if (otherClassHierResult) {

									if (!classHierResult->isResultEquivalentTo(otherClassHierResult)) {
										resultSimilar = false;
									}

									delete otherClassHierResult;
								}
								break;

							} else {
								++currNodeNumber;
								if (currNodeNumber > nodeNumber) {
									break;
								}
								classResEl = classResEl.nextSiblingElement();
							}
						}
					}
				}
				return resultSimilar;
			}



			bool CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::isBooleanResultSimilarTo(CBooleanQueryResult* booleanResult, cint64 nodeNumber, const QString& otherResponseFileString) {
				bool resultSimilar = true;
				QFile responseFile(otherResponseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QDomElement classResEl = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					if (!classResEl.isNull()) {
						while (!classResEl.isNull()) {
							if (classResEl.nodeName() == "Error") {
								break;

							} else if (currNodeNumber == nodeNumber && classResEl.nodeName() == "BooleanResponse") {
								CBooleanQueryResult* otherBooleanResult = mOWLlinkResultParser.parseBooleanQueryResult(&classResEl);
								if (otherBooleanResult) {

									if (!booleanResult->isResultEquivalentTo(otherBooleanResult)) {
										resultSimilar = false;
									}

									delete otherBooleanResult;
								}
								break;

							} else {
								++currNodeNumber;
								if (currNodeNumber > nodeNumber) {
									break;
								}
								classResEl = classResEl.nextSiblingElement();
							}
						}
					}
				}
				return resultSimilar;
			}



			bool CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::isVariableBindingResultSimilarTo(CVariableBindingsAnswersSetResult* varBindingResult, cint64 nodeNumber, const QString& otherResponseFileString, QHash<QString, cint64>* variableNameIndexHash, const QString& reportingFileName) {
				bool resultSimilar = true;
				QFile responseFile(otherResponseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QDomElement resultElement = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					if (!resultElement.isNull()) {
						while (!resultElement.isNull()) {
							if (resultElement.nodeName() == "Error") {
								break;

							} else if (currNodeNumber == nodeNumber && resultElement.nodeName() == "results") {
								CVariableBindingsAnswersSetResult* otherVarBindingResult = mSPARQLResultParser.parseVariableBindingsSet(&resultElement, variableNameIndexHash, true);
								if (otherVarBindingResult) {

									if (!varBindingResult || !varBindingResult->isResultEquivalentTo(otherVarBindingResult)) {
										resultSimilar = false;

										if (varBindingResult && otherVarBindingResult) {
											CVariableBindingsAnswersResultIterator* it = varBindingResult->getVariableBindingsAnswersIterator();
											while (it->hasNext()) {
												CVariableBindingsAnswerResult* answerResult = it->getNext();
												if (!otherVarBindingResult->hasResultVariableBindings(answerResult)) {
													QFile reportingFile(reportingFileName);
													if (reportingFile.open(QFile::Append)) {
														reportingFile.write(QString("Difference (additional answer) for " + answerResult->getQueryResultString() + "\r\n").toLocal8Bit());
														reportingFile.close();
													}
												}
											}
											CVariableBindingsAnswersResultIterator* otherIt = otherVarBindingResult->getVariableBindingsAnswersIterator();
											while (otherIt->hasNext()) {
												CVariableBindingsAnswerResult* answerResult = otherIt->getNext();
												if (!varBindingResult->hasResultVariableBindings(answerResult)) {
													QFile reportingFile(reportingFileName);
													if (reportingFile.open(QFile::Append)) {
														reportingFile.write(QString("Difference (missing answer) for " + answerResult->getQueryResultString() + "\r\n").toLocal8Bit());
														reportingFile.close();
													}
												}
											}
										}
									}

									delete otherVarBindingResult;

								} else if (varBindingResult) {
									resultSimilar = false;
								}
								break;

							} else {
								if (resultElement.nodeName() != "answering-statistics") {
									++currNodeNumber;
								}
								if (currNodeNumber > nodeNumber) {
									break;
								}
								resultElement = resultElement.nextSiblingElement();
							}
						}
					}
				}
				return resultSimilar;
			}



			bool CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::isClassesResultSimilarTo(CClassesResult* classesResult, cint64 nodeNumber, const QString& otherResponseFileString) {
				bool resultSimilar = true;
				QFile responseFile(otherResponseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile,false);
					QDomElement rootEl = document.documentElement();
					QDomElement classResEl = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					if (!classResEl.isNull()) {
						while (!classResEl.isNull()) {
							if (classResEl.nodeName() == "Error") {
								break;

							} else if (currNodeNumber == nodeNumber && classResEl.nodeName() == "Classes") {
								CClassesResult* otherClassesResult = mOWLlinkResultParser.parseClassesResult(&classResEl);
								if (otherClassesResult) {

									if (!classesResult->isResultEquivalentTo(otherClassesResult)) {
										resultSimilar = false;
									}

									delete otherClassesResult;
								}
								break;

							} else {
								++currNodeNumber;
								if (currNodeNumber > nodeNumber) {
									break;
								}
								classResEl = classResEl.nextSiblingElement();
							}
						}
					}
				}
				return resultSimilar;
			}

			bool CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

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
				QString outputFileString(outputDirectory+mOutputFileName+"result-difference-to-other-reasoners");

				QString detailedOutputReportingDirectory = QString(outputDirectory + mOutputFileName + "/result-differences-reports/");
				outDir.mkpath(mOutputFileName + "/result-differences-reports");

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
										CReasonerEvaluationStringListValue* stringListDataValue = dynamic_cast<CReasonerEvaluationStringListValue*>(evalData);
										if (stringListDataValue) {

											QStringList responseFiles = stringListDataValue->getValue();

											double diffCount = compareResultsSimilarityWithOtherReasoners(groupString,requestString,reasonerString,stringListDataValue,reasonerList, detailedOutputReportingDirectory);

											tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<diffCount);

											mean += diffCount;
											min = qMin(min,diffCount);
											max = qMax(max,diffCount); 
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
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,context->getAdditionalTitleString()+title+"[# differences]",reasonerList,outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerDifferencePlot(plotFileString,context->getAdditionalTitleString()+title+"[# differences]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}

				}

				return true;
			}



			QString CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("result difference to other reasoners","Result Differences to other Reasoners, ");
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
