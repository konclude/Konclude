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

#include "CReasonerEvaluationSpecifiedTimeExtractor.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationSpecifiedTimeExtractor::CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::EXTRACTORTYPE extractorType, double timelimit, double errorPunishmentTime, CReasonerEvaluationDataValueCacher* cacher) {
				mTimelimit = timelimit;
				mErrorPunishmentTime = errorPunishmentTime;
				mCacher = cacher;

				mExtractorType = extractorType;

				mClassificationExtractionTagList << "Classify";
				mConsistencyExtractionTagList << "IsKBSatisfiable";
				mSatisfiabilityExtractionTagList << "IsClassSatisfiable";
				mRealizationExtractionTagList << "Realize";
				mComplexQueryingExtractionTagList << "SelectQuery" << "AskQuery";

				mCalculationExtractionTagList += mClassificationExtractionTagList;
				mCalculationExtractionTagList += mConsistencyExtractionTagList;
				mCalculationExtractionTagList += mSatisfiabilityExtractionTagList;
				mCalculationExtractionTagList += mRealizationExtractionTagList;
			}




			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractSpecifiedTimesEvaluationData(QDomDocument& document, const QString& responseFileString, const QStringList& extractionTagList, CReasonerEvaluationExtractor::EXTRACTORTYPE extractionType) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)extractionType));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;
					bool allRespTimeValid = true;
					bool lastTimeoutOccured = false;
					bool dataAvailable = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							lastTimeoutOccured = true;
						}

						QString requestCommandString = childEl.attribute("request-command");
						bool useTagTime = false;
						foreach (const QString& extractionTag, extractionTagList) {
							if (requestCommandString == extractionTag) {
								useTagTime = true;
								dataAvailable = true;
								break;
							}
						}
						if (useTagTime) {
							if (lastTimeoutOccured) {
								timeoutOccured = true;
							}
							if (!timeoutOccured) {
								bool validResponseTime = false;
								QString respTimeString = childEl.attribute("response-time","timeout");
								double respTime = respTimeString.toDouble(&validResponseTime);
								if (validResponseTime) {
									doubleValue += respTime;
								}
								if (childEl.tagName() == "Error") {
									validResponseTime = false;
								}
								allRespTimeValid &= validResponseTime;
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					if (timeoutOccured) {
						doubleValue = mTimelimit;
					}
					if (mTimelimit >= 0) {
						doubleValue = qMin(mTimelimit,doubleValue);
					}
					//if (errorOccured || !allRespTimeValid) {
					//	if (mTimelimit >= 0) {
					//		doubleValue = mTimelimit+mErrorPunishmentTime;
					//	} else {
					//		doubleValue = mErrorPunishmentTime;
					//	}
					//}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);
					if (!dataAvailable) {
						doubleEvalValue->setDataNotAvailableFlag();
					}

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)extractionType,doubleEvalValue);
					}

				}
				return doubleEvalValue;		
			}


			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractClassificationEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document,responseFileString,mClassificationExtractionTagList,CReasonerEvaluationExtractor::CLASSIFICATIONTIMEEXTRACTOR);
			}


			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractComplexQueryingEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document, responseFileString, mComplexQueryingExtractionTagList, CReasonerEvaluationExtractor::COMPLEXQUERYINGTIMEEXTRACTOR);
			}


			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractRealizationEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document,responseFileString,mRealizationExtractionTagList,CReasonerEvaluationExtractor::REALIZATIONTIMEEXTRACTOR);
			}



			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractConsistencyEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document,responseFileString,mConsistencyExtractionTagList,CReasonerEvaluationExtractor::CONSISTENCYTIMEEXTRACTOR);
			}



			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractSatisfiabilityEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document,responseFileString,mSatisfiabilityExtractionTagList,CReasonerEvaluationExtractor::SATISFIABILITYTIMEEXTRACTOR);
			}



			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractCalculationEvaluationData(QDomDocument& document, const QString& responseFileString) {
				return extractSpecifiedTimesEvaluationData(document,responseFileString,mCalculationExtractionTagList,CReasonerEvaluationExtractor::CALCULATIONTIMEEXTRACTOR);
			}



			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractLoadingEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::LOADINGTIMEEXTRACTOR));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;
					bool allRespTimeValid = true;
					bool lastTimeoutOccured = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							lastTimeoutOccured = true;
						}

						QString requestCommandString = childEl.attribute("request-command");
						if (requestCommandString == "Tell" || requestCommandString == "LoadOntologies" || requestCommandString == "LoadGraph" || requestCommandString == "GraphLoad") {
							if (lastTimeoutOccured) {
								timeoutOccured = true;
							}
							if (!timeoutOccured) {
								bool validResponseTime = false;
								QString respTimeString = childEl.attribute("response-time","timeout");
								double respTime = respTimeString.toDouble(&validResponseTime);
								if (validResponseTime) {
									doubleValue += respTime;
								}
								if (childEl.tagName() != "OK") {
									validResponseTime = false;
								}
								allRespTimeValid &= validResponseTime;
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					if (timeoutOccured) {
						doubleValue = mTimelimit;
					}
					if (mTimelimit >= 0) {
						doubleValue = qMin(mTimelimit,doubleValue);
					}
					//if (errorOccured || !allRespTimeValid) {
					//	if (mTimelimit >= 0) {
					//		doubleValue = mTimelimit+mErrorPunishmentTime;
					//	} else {
					//		doubleValue = mErrorPunishmentTime;
					//	}
					//}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationExtractor::LOADINGTIMEEXTRACTOR,doubleEvalValue);
					}

				}
				return doubleEvalValue;			
			}


			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractResultCountingEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString, (cint64)CReasonerEvaluationExtractor::RESPONSECOUNTEXTRACTOR));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool allRespTimeValid = true;
					bool lastTimeoutOccured = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							lastTimeoutOccured = true;
						}

						QString requestCommandString = childEl.attribute("request-command");
						if (requestCommandString == "SelectQuery") {
							bool validResponseCount = false;
							QString respTimeString = childEl.attribute("response-count", "");
							double reportedResCount = respTimeString.toDouble(&validResponseCount);
							if (validResponseCount) {
								doubleValue += reportedResCount;
							}
							QDomElement resultsEl = childEl.nextSiblingElement("results");
							while (!resultsEl.isNull()) {

								QDomElement resultEl = resultsEl.firstChildElement("result");
								while (!resultEl.isNull()) {
									doubleValue++;
									resultEl = resultEl.nextSiblingElement("result");
								}

								resultsEl = resultsEl.nextSiblingElement("results");
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					//if (errorOccured || !allRespTimeValid) {
					//	if (mTimelimit >= 0) {
					//		doubleValue = mTimelimit+mErrorPunishmentTime;
					//	} else {
					//		doubleValue = mErrorPunishmentTime;
					//	}
					//}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString, (cint64)CReasonerEvaluationExtractor::RESPONSECOUNTEXTRACTOR, doubleEvalValue);
					}

				}
				return doubleEvalValue;
			}





			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractTotalResponseEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::TOTALRESPONSETIMEEXTRACTOR));
				}

				if (!doubleEvalValue) {
					double doubleValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;
					bool validResponseTime = false;

					QDomElement rootEl = document.documentElement();
					QString respTimeString = rootEl.attribute("response-time");
					double respTime = respTimeString.toDouble(&validResponseTime);
					if (validResponseTime) {
						doubleValue += respTime;
					}


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}

						if (!timeoutOccured) {
							QString requestCommandString = childEl.tagName();
							if (requestCommandString == "Error") {
								errorOccured = true;
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					if (timeoutOccured) {
						doubleValue = mTimelimit;
					}
					if (mTimelimit >= 0) {
						doubleValue = qMin(mTimelimit,doubleValue);
					}
					//if (errorOccured || !validResponseTime) {
					//	if (mTimelimit >= 0) {
					//		doubleValue = mTimelimit+mErrorPunishmentTime;
					//	} else {
					//		doubleValue = mErrorPunishmentTime;
					//	}
					//}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(doubleValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationExtractor::TOTALRESPONSETIMEEXTRACTOR,doubleEvalValue);
					}

				}
				return doubleEvalValue;			
			}






			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractTimeoutErrorEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::TIMEOUTERROREXTRACTOR));
				}

				if (!doubleEvalValue) {
					cint64 errorTimeoutValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}

						if (!timeoutOccured) {
							QString requestCommandString = childEl.tagName();
							if (requestCommandString == "Error") {
								errorOccured = true;
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					if (timeoutOccured || errorOccured) {
						++errorTimeoutValue;
					}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(errorTimeoutValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationSpecifiedTimeExtractor::TIMEOUTERROREXTRACTOR,doubleEvalValue);
					}

				}
				return doubleEvalValue;			
			}





			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractErrorEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::ERROREXTRACTOR));
				}

				if (!doubleEvalValue) {
					cint64 errorTimeoutValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}

						if (!timeoutOccured) {
							QString requestCommandString = childEl.tagName();
							if (requestCommandString == "Error") {
								errorOccured = true;
							}
						}
						childEl = childEl.nextSiblingElement();
					}

					if (errorOccured) {
						++errorTimeoutValue;
					}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(errorTimeoutValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationSpecifiedTimeExtractor::ERROREXTRACTOR,doubleEvalValue);
					}

				}
				return doubleEvalValue;			
			}



			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractMemoryUsageEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString, (cint64)CReasonerEvaluationExtractor::MEMORYUSAGEEXTRACTOR));
				}

				if (!doubleEvalValue) {

					QDomElement rootEl = document.documentElement();
					QString memoryUsageString = rootEl.attribute("maxMemoryUsage");


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull() && memoryUsageString.isEmpty()) {
						memoryUsageString = childEl.attribute("maxMemoryUsage");
						childEl = childEl.nextSiblingElement();
					}

					bool parsedOk = false;
					cint64 memoryUsage = memoryUsageString.toLongLong(&parsedOk);
					if (parsedOk) {
						doubleEvalValue = new CReasonerEvaluationDoubleDataValue(memoryUsage);
					}

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString, (cint64)CReasonerEvaluationSpecifiedTimeExtractor::MEMORYUSAGEEXTRACTOR, doubleEvalValue);
					}

				}
				return doubleEvalValue;
			}





			CReasonerEvaluationDoubleDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractTimeoutEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationDoubleDataValue* doubleEvalValue = nullptr;

				if (mCacher) {
					doubleEvalValue = dynamic_cast<CReasonerEvaluationDoubleDataValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::TIMEOUTEXTRACTOR));
				}

				if (!doubleEvalValue) {
					cint64 errorTimeoutValue = 0;
					bool errorOccured = false;
					bool timeoutOccured = false;

					QDomElement rootEl = document.documentElement();


					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						QString timeoutString = childEl.attribute("timeout");
						if (timeoutString == "1") {
							timeoutOccured = true;
						}
						childEl = childEl.nextSiblingElement();
					}

					if (timeoutOccured) {
						++errorTimeoutValue;
					}
					doubleEvalValue = new CReasonerEvaluationDoubleDataValue(errorTimeoutValue);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationSpecifiedTimeExtractor::TIMEOUTEXTRACTOR,doubleEvalValue);
					}

				}
				return doubleEvalValue;			
			}



			CReasonerEvaluationStringValue* CReasonerEvaluationSpecifiedTimeExtractor::extractResponseFileEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationStringValue* responseFileEvalValue = nullptr;

				if (mCacher) {
					responseFileEvalValue = dynamic_cast<CReasonerEvaluationStringValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::RESPONSEFILEEXTRACTOR));
				}

				if (!responseFileEvalValue) {
					responseFileEvalValue = new CReasonerEvaluationStringValue(responseFileString);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationExtractor::RESPONSEFILEEXTRACTOR,responseFileEvalValue);
					}

				}
				return responseFileEvalValue;			
			}




			CReasonerEvaluationStringValue* CReasonerEvaluationSpecifiedTimeExtractor::extractRequestFileEvaluationData(QDomDocument& document, const QString& responseFileString) {
				CReasonerEvaluationStringValue* requestFileEvalValue = nullptr;

				if (mCacher) {
					requestFileEvalValue = dynamic_cast<CReasonerEvaluationStringValue*>(mCacher->getCachedDataValue(responseFileString,(cint64)CReasonerEvaluationExtractor::REQUESTFILEEXTRACTOR));
				}

				if (!requestFileEvalValue) {

					QDomElement rootEl = document.documentElement();
					QString requestFileString = rootEl.attribute("test");
					requestFileString = requestFileString.replace("./","");
					requestFileString = requestFileString.replace("Evaluation/Requests/","");

					requestFileEvalValue = new CReasonerEvaluationStringValue(requestFileString);

					if (mCacher) {
						mCacher->addDataValueToCache(responseFileString,(cint64)CReasonerEvaluationExtractor::REQUESTFILEEXTRACTOR,requestFileEvalValue);
					}

				}
				return requestFileEvalValue;			
			}




			CReasonerEvaluationDataValue* CReasonerEvaluationSpecifiedTimeExtractor::extractEvaluationData(const QString& responseFileString) {
				CReasonerEvaluationDataValue* evalValue = nullptr;

				if (mCacher) {
					evalValue = mCacher->getCachedDataValue(responseFileString,(cint64)mExtractorType);
				}

				if (!evalValue) {

					QFile responseFile(responseFileString);
					if (responseFile.open(QIODevice::ReadOnly)) {
						QDomDocument document;
						document.setContent(&responseFile,false);
						QDomElement rootEl = document.documentElement();
						responseFile.close();

						CReasonerEvaluationDoubleDataValue* doubleClassificationEvalValue = extractClassificationEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::CLASSIFICATIONTIMEEXTRACTOR) {
							evalValue = doubleClassificationEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleComplexQueryEvalValue = extractComplexQueryingEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::COMPLEXQUERYINGTIMEEXTRACTOR) {
							evalValue = doubleComplexQueryEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleRealizationEvalValue = extractRealizationEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::REALIZATIONTIMEEXTRACTOR) {
							evalValue = doubleRealizationEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleConsistencyEvalValue = extractConsistencyEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::CONSISTENCYTIMEEXTRACTOR) {
							evalValue = doubleConsistencyEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleSatisfiabilityEvalValue = extractSatisfiabilityEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::SATISFIABILITYTIMEEXTRACTOR) {
							evalValue = doubleSatisfiabilityEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleCalculationEvalValue = extractCalculationEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::CALCULATIONTIMEEXTRACTOR) {
							evalValue = doubleCalculationEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleLoadingEvalValue = extractLoadingEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::LOADINGTIMEEXTRACTOR) {
							evalValue = doubleLoadingEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleResponseCountEvalValue = extractResultCountingEvaluationData(document, responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::RESPONSECOUNTEXTRACTOR) {
							evalValue = doubleResponseCountEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleTotalEvalValue = extractTotalResponseEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::TOTALRESPONSETIMEEXTRACTOR) {
							evalValue = doubleTotalEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleTimeoutErrorEvalValue = extractTimeoutErrorEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::TIMEOUTERROREXTRACTOR) {
							evalValue = doubleTimeoutErrorEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleErrorEvalValue = extractErrorEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::ERROREXTRACTOR) {
							evalValue = doubleErrorEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleMemoryUsageEvalValue = extractMemoryUsageEvaluationData(document, responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::MEMORYUSAGEEXTRACTOR) {
							evalValue = doubleMemoryUsageEvalValue;
						}
						CReasonerEvaluationDoubleDataValue* doubleTimeoutEvalValue = extractTimeoutEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::TIMEOUTEXTRACTOR) {
							evalValue = doubleTimeoutEvalValue;
						}
						CReasonerEvaluationStringValue* stringResponseFileEvalValue = extractResponseFileEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::RESPONSEFILEEXTRACTOR) {
							evalValue = stringResponseFileEvalValue;
						}
						CReasonerEvaluationStringValue* stringRequestFileEvalValue = extractRequestFileEvaluationData(document,responseFileString);
						if (mExtractorType == CReasonerEvaluationExtractor::REQUESTFILEEXTRACTOR) {
							evalValue = stringRequestFileEvalValue;
						}
					}

				}
				if (evalValue) {
					evalValue = evalValue->getCopy();
				}
				return evalValue;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
