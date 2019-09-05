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

#include "CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::~CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser() {
			}



			cint64 CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::compareResultsSubsumptionWithOtherReasoners(const QString& groupString, const QString& requestString, const QString& reasonerString, CReasonerEvaluationStringListValue* stringListDataValue, const QStringList& reasonerList, const QString& detailedReportingOutputDirectory) {
				cint64 totalResultSubsumptionCount = 0;
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
							cint64 reasonerResultSubsumptionCount = 0;


							CReasonerEvaluationDataValue* otherReasonerEvalData = getGroupRequestReasonerEvaluationData(groupString,requestString,otherReasoner);
							CReasonerEvaluationStringListValue* otherReasonerStringListDataValue = dynamic_cast<CReasonerEvaluationStringListValue*>(otherReasonerEvalData);
							if (otherReasonerStringListDataValue) {
								const QStringList& otherResponseFileStringList = otherReasonerStringListDataValue->getValue();
								if (!otherResponseFileStringList.isEmpty()) {
									CReasonerEvaluationStringListValue* minVal = qMin(stringListDataValue,otherReasonerStringListDataValue);
									CReasonerEvaluationStringListValue* maxVal = qMax(stringListDataValue,otherReasonerStringListDataValue);

									if (mReasonerCompareResultSubsumptionCountHash.contains(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal))) {
										reasonerResultSubsumptionCount += mReasonerCompareResultSubsumptionCountHash.value(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal));
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

												} else if (resultElement.nodeName() == "results") {
													foundResult = true;
													CClassHierarchyResult* classHierResult = nullptr;
													if (lastClassHierResult && lastClassHierResultNumber == nodeNumber) {
														classHierResult = lastClassHierResult;
													}
													CBooleanQueryResult* booleanResult = nullptr;
													CClassesResult* classesResult = nullptr;
													CVariableBindingsAnswersSetResult* variableBindingsResult = nullptr;

													cint64 subsumingCount = 0;

													QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>*& compareDiffCountHash = mNumberCompareDiffCountHashHash[nodeNumber];
													if (!compareDiffCountHash) {
														compareDiffCountHash = new QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>();
													}

													if (compareDiffCountHash->contains(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal))) {
														subsumingCount += compareDiffCountHash->value(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal));
													} else {
														if (resultElement.nodeName() == "results") {
															// handle SPARQL binding results
															QHash<QString, cint64> variableNameIndexHash;
															if (!variableBindingsResult) {
																variableBindingsResult = mSPARQLResultParser.parseVariableBindings(&resultElement, &variableNameIndexHash, true);
															}
															QString otherResponseFileString = otherResponseFileStringList.first();															
															LOG(INFO, "::Konclude::Test::Evaluation::ResultDifferenceAnalyser", logTr("Comparing variable binding results between '%1' and '%2'.").arg(responseFileString).arg(otherResponseFileString), this);
															if (isVariableBindingResultSubsuming(variableBindingsResult, nodeNumber, otherResponseFileString, &variableNameIndexHash)) {
																subsumingCount = 1;
															}
														}
														compareDiffCountHash->insert(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal),subsumingCount);
													}
													reasonerResultSubsumptionCount += subsumingCount;
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

										mReasonerCompareResultSubsumptionCountHash.insert(QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>(minVal,maxVal),reasonerResultSubsumptionCount);

									}
								}
							}

							totalResultSubsumptionCount += reasonerResultSubsumptionCount;

						}
					}
					if (lastClassHierResult) {
						delete lastClassHierResult;
					}

				}
				return totalResultSubsumptionCount;
			}





			bool CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::isVariableBindingResultSubsuming(CVariableBindingsAnswersSetResult* varBindingResult, cint64 nodeNumber, const QString& otherResponseFileString, QHash<QString, cint64>* variableNameIndexHash) {
				bool resultSubsumption = false;
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
								CVariableBindingsAnswersSetResult* otherVarBindingResult = mSPARQLResultParser.parseVariableBindings(&resultElement, variableNameIndexHash, true);
								if (otherVarBindingResult) {

									if (varBindingResult && varBindingResult->isResultEquivalentTo(otherVarBindingResult)) {
										resultSubsumption = true;

									} else if (varBindingResult) {
										resultSubsumption = true;
										if (varBindingResult && otherVarBindingResult && resultSubsumption) {
											CVariableBindingsAnswersResultIterator* it = varBindingResult->getVariableBindingsAnswersIterator();
											while (it->hasNext()) {
												CVariableBindingsAnswerResult* answerResult = it->getNext();
												if (!otherVarBindingResult->hasResultVariableBindings(answerResult)) {
													resultSubsumption = false;
												}
											}
										}
									}

									delete otherVarBindingResult;

								} else if (varBindingResult) {
									resultSubsumption = false;
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
				return resultSubsumption;
			}



			bool CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector) {

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
				QString outputFileString(outputDirectory+mOutputFileName+"result-subsumption-over-other-reasoners");

				QString detailedOutputReportingDirectory = QString(outputDirectory + mOutputFileName + "/result-subsumption-reports/");
				outDir.mkpath(mOutputFileName + "/result-subsumption-reports");

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

											double subsumptionCount = compareResultsSubsumptionWithOtherReasoners(groupString,requestString,reasonerString,stringListDataValue,reasonerList, detailedOutputReportingDirectory);

											tableMultiFormatOutputWriter.addValuesToLastTableRow(QList<double>()<<subsumptionCount);

											mean += subsumptionCount;
											min = qMin(min,subsumptionCount);
											max = qMax(max,subsumptionCount); 
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
								context->getGNUPlotPlotter()->createReasonerComparisonPlot(plotFileString,context->getAdditionalTitleString()+title+"[# subsumptions]",reasonerList,outputFile);
							}
						}
					}
					if (context->isHighchartPlottingActivated()) {
						foreach (QString plotFileString, reasonerCompHighchartPlotFileList) {
							if (true /*plotFileString.contains("-by-Average-")*/) {
								QString outputFile = plotFileString;
								outputFile.remove(".csv");
								QString title = getPrettyTitleFromOutputFile(outputFile);
								context->getHighchartPlotter()->createReasonerDifferencePlot(plotFileString,context->getAdditionalTitleString()+title+"[# subsumptions]",reasonerList,outputFile,selector->getNameString()+" :: "+context->getAdditionalTitleString());
							}
						}
					}

				}

				return true;
			}



			QString CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser::getPrettyTitleFromOutputFile(const QString& outputFile) {
				QString title = outputFile;
				title.remove("table");
				cint64 slashPos = title.lastIndexOf("/");
				if (slashPos >= 0) {
					title = title.mid(slashPos+1);
				}
				title.replace("-"," ");
				title.replace("result subsumptions over other reasoners","Result Subsumptions over other Reasoners, ");
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
