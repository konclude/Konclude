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

#include "CSPARQLResultsComparisonLoader.h"


namespace Konclude {

	namespace Control {

		namespace Loader {


			CSPARQLResultsComparisonLoader::CSPARQLResultsComparisonLoader() : mLogIdentifier("::Konclude::Loader::SPARQLResultComparer", this) {
				loaderConfig = 0;
			}



			CSPARQLResultsComparisonLoader::~CSPARQLResultsComparisonLoader() {
			}


			CLoader *CSPARQLResultsComparisonLoader::init(CLoaderFactory *loaderFactory, CConfiguration *config) {
				loaderConfig = config;

				mReqFileString = CConfigDataReader::readConfigString(config, "Konclude.SPARQL.RequestFile");
				mCompFileString = CConfigDataReader::readConfigString(config, "Konclude.SPARQL.ComparisonFile");
				mResFileString = CConfigDataReader::readConfigString(config, "Konclude.SPARQL.ResponseFile");

				mDifferencesLoggingLimit = CConfigDataReader::readConfigInteger(config, "Konclude.SPARQL.Comparison.DifferencesLoggingLimit", 10);

				return this;
			}



			CLoader *CSPARQLResultsComparisonLoader::load() {
				if (!mReqFileString.isEmpty() && !mCompFileString.isEmpty()) {
					QString reportingFileLogString;
					if (!mResFileString.isEmpty()) {
						reportingFileLogString = QString(", reporting differences to '%1'").arg(mResFileString);
					}
					LOG(INFO, mLogIdentifier.getLogDomain(), logTr("Comparing request file '%1' with comparison file '%2'%3.").arg(mReqFileString).arg(mCompFileString).arg(reportingFileLogString), this);
					isVariableBindingResultSimilarTo(mReqFileString, mCompFileString, mResFileString);
				} else {
					LOG(ERROR, mLogIdentifier.getLogDomain(), logTr("Request file or comparison file not sprecified."), this);
				}
				return this;
			}


			bool CSPARQLResultsComparisonLoader::isVariableBindingResultSimilarTo(const QString& responseFileString, const QString& otherResponseFileString, const QString& reportingFileName) {
				bool resultSimilar = true;
				QFile responseFile(responseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile, false);
					QDomElement rootEl = document.documentElement();
					QDomElement resultElement = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					cint64 comparedResultCount = 0;
					cint64 differentResultCount = 0;
					QHash<QString, cint64>* variableNameIndexHash = nullptr;
					if (!resultElement.isNull()) {
						while (!resultElement.isNull()) {
							if (resultElement.nodeName() == "Error") {
								break;

							} else if (resultElement.nodeName() == "results") {
								CVariableBindingsAnswersSetResult* varBindingResult = mSPARQLResultParser.parseVariableBindingsSet(&resultElement, variableNameIndexHash, true);
								if (varBindingResult) {
									++comparedResultCount;
									if (!isVariableBindingResultSimilarTo(varBindingResult, currNodeNumber, otherResponseFileString, variableNameIndexHash, reportingFileName)) {
										differentResultCount++;
										resultSimilar = false;
									}
								} else {
									LOG(WARN, mLogIdentifier.getLogDomain(), logTr("Result %1 not parsable in request file.").arg(currNodeNumber + 1), this);
								}
							}
							if (resultElement.nodeName() != "answering-statistics") {
								++currNodeNumber;
							}
							resultElement = resultElement.nextSiblingElement();
						}
					}
					LOG(INFO, mLogIdentifier.getLogDomain(), logTr("Compared %1 results, found %2 differences.").arg(comparedResultCount).arg(differentResultCount), this);

					return resultSimilar;
				} else {
					LOG(ERROR, mLogIdentifier.getLogDomain(), logTr("Request file '%1' cannot be opened.").arg(responseFileString), this);
				}
				return false;
			}



			bool CSPARQLResultsComparisonLoader::isVariableBindingResultSimilarTo(CVariableBindingsAnswersSetResult* varBindingResult, cint64 nodeNumber, const QString& otherResponseFileString, QHash<QString, cint64>* variableNameIndexHash, const QString& reportingFileName) {
				bool resultSimilar = true;
				bool resultFound = false;
				QFile responseFile(otherResponseFileString);
				if (responseFile.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&responseFile, false);
					QDomElement rootEl = document.documentElement();
					QDomElement resultElement = rootEl.firstChildElement();
					cint64 currNodeNumber = 0;
					QFile reportingFile(reportingFileName);
					cint64 additionalAnswerCount = 0;
					cint64 missingAnswerCount = 0;
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
													QString differenceMessage = QString("Additional answer for result %1 in request file with bindings: %2.").arg(currNodeNumber + 1).arg(answerResult->getQueryResultString());
													if (mDifferencesLoggingLimit-- > 0) {
														LOG(WARN, mLogIdentifier.getLogDomain(), differenceMessage, this);
													}
													++additionalAnswerCount;
													if (reportingFile.open(QFile::Append)) {
														reportingFile.write(differenceMessage.toLocal8Bit());
														reportingFile.close();
													}
												}
											}
											CVariableBindingsAnswersResultIterator* otherIt = otherVarBindingResult->getVariableBindingsAnswersIterator();
											while (otherIt->hasNext()) {
												CVariableBindingsAnswerResult* answerResult = otherIt->getNext();
												if (!varBindingResult->hasResultVariableBindings(answerResult)) {
													QString differenceMessage = QString("Missing answer for result %1 in request file with bindings: %2").arg(currNodeNumber + 1).arg(answerResult->getQueryResultString());
													if (mDifferencesLoggingLimit-- > 0) {
														LOG(WARN, mLogIdentifier.getLogDomain(), differenceMessage, this);
													}
													missingAnswerCount++;
													if (reportingFile.open(QFile::Append)) {
														reportingFile.write(differenceMessage.toLocal8Bit());
														reportingFile.close();
													}
												}
											}
										}
									}

									if (additionalAnswerCount > 0 || missingAnswerCount > 0) {
										LOG(WARN, mLogIdentifier.getLogDomain(), logTr("Result %1 has %2 additional and %3 missing answers.").arg(nodeNumber + 1).arg(additionalAnswerCount).arg(missingAnswerCount), this);
									} else {
										LOG(INFO, mLogIdentifier.getLogDomain(), logTr("Result %1 seems identical.").arg(nodeNumber + 1), this);
									}

									resultFound = true;
									delete otherVarBindingResult;

								} else if (varBindingResult) {
									LOG(WARN, mLogIdentifier.getLogDomain(), logTr("Result %1 not found or not parsable in comparison file.").arg(nodeNumber + 1), this);
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

					if (!resultSimilar) {
						LOG(WARN, mLogIdentifier.getLogDomain(), logTr("Result %1 is not identical.").arg(nodeNumber + 1), this);
					}

				} else {
					LOG(ERROR, mLogIdentifier.getLogDomain(), logTr("Comparison file '%1' cannot be opened.").arg(otherResponseFileString), this);
				}

				if (!resultFound) {
					LOG(WARN, mLogIdentifier.getLogDomain(), logTr("Result %1 not found in comparison file.").arg(nodeNumber + 1), this);
					resultSimilar = false;
				}


				return resultSimilar;
			}




			CLoader *CSPARQLResultsComparisonLoader::exit() {
				return this;
			}



		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude
