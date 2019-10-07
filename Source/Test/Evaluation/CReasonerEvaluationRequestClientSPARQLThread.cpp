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

#include "CReasonerEvaluationRequestClientSPARQLThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestClientSPARQLThread::CReasonerEvaluationRequestClientSPARQLThread() : CReasonerEvaluationRequestClientBaseThread("Reasoner-Evaluation-SPARQL-Request-Client", "::Konclude::Test::ReasonerEvaluationSPARQLRequestClient") {
				mSPARQLOperationKeywordNameSet.insert("LOAD", "GraphLoad");
				mSPARQLOperationKeywordNameSet.insert("SELECT", "SelectQuery");
				mSPARQLOperationKeywordNameSet.insert("ASK", "AskQuery");
				mSPARQLOperationKeywordNameSet.insert("PREPARE", "GraphLoad");

				mConfSendPrepareBeforeQuerying = false;
				mConfSupportsPrepareCommand = false;

				mConfResponseSizeLimit = -1;
			}


			CReasonerEvaluationRequestClientSPARQLThread::~CReasonerEvaluationRequestClientSPARQLThread() {
			}


			bool CReasonerEvaluationRequestClientSPARQLThread::canHandleRequestFile(const QString& testFileString) {
				if (testFileString.endsWith(".sparql")) {
					return true;
				}
				return false;
			}



			CReasonerEvaluationRequestClientBaseThread* CReasonerEvaluationRequestClientSPARQLThread::readConfig(CConfiguration* config) {
				CReasonerEvaluationRequestClientBaseThread::readConfig(config);

				mConfSendPrepareBeforeQuerying = CConfigDataReader::readConfigBoolean(config, "Konclude.Evaluation.SPARQLSendPrepareBeforeQuerying");
				mConfSupportsPrepareCommand = CConfigDataReader::readConfigBoolean(config, "Konclude.Evaluation.SPARQLSupportsPrepareOperation");

				mConfResponseSizeLimit = CConfigDataReader::readConfigInteger(config, "Konclude.Evaluation.SPARQLResponseSizeLimit", -1);

				return this;
			}



			bool CReasonerEvaluationRequestClientSPARQLThread::parseResponse(QIODevice* device) {
				bool sparqlQuery = ((CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes)->getRequestOperationData()->getOperationType() == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_QUERY;
				bool responseError = false;
				QDomDocument document;
				QString errorMessage;
				int errorLine = 0;
				int errorColumn = 0;

				cint64 size = device->size();
				if (mConfResponseSizeLimit != -1 && size > mConfResponseSizeLimit) {
					((CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes)->setEvaluationError(true);
					((CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes)->setEvaluationErrorString(QString("Response with %1 bytes exceeds limit of %2 bytes").arg(size).arg(mConfResponseSizeLimit));
					LOG(WARN, getLogDomain(), logTr("Response with %1 bytes exceeds limit of %2 bytes, skipping response analysis.").arg(size).arg(mConfResponseSizeLimit), this);
					return false;
				}

				if (document.setContent(device, &errorMessage, &errorLine, &errorColumn)) {

					QDomElement rootNode = document.documentElement();
					QDomElement responseNode = rootNode.firstChildElement();
					while (!responseNode.isNull()) {
						((CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes)->addResponseNode(responseNode);
						responseNode = responseNode.nextSiblingElement();
					}

					if (rootNode.isNull() && sparqlQuery) {
						responseError = true;
					}

				} else if (sparqlQuery) {
					responseError = true;
					if (!errorMessage.isEmpty()) {
						device->reset();
						cint64 available = device->bytesAvailable();
						QString responsePart = device->read(qMin((int)available, 10000));
						((CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes)->setResponseErrorString(QString("Parsing error (line %2, column %3) '%1' of response '%4...' .").arg(errorMessage).arg(errorLine).arg(errorColumn).arg(responsePart));
						LOG(INFO, getLogDomain(), logTr("Parsing of response failed due to error (line %2, column %3): %1.").arg(errorMessage).arg(errorLine).arg(errorColumn), this);
					}
				}
				return responseError;
			}


			bool CReasonerEvaluationRequestClientSPARQLThread::sendNextRequest() {
				if (mRemainingRequestList.isEmpty()) {
					return false;
				}
				mNextReqRes = mRemainingRequestList.takeFirst();
				CReasonerEvaluationRequestResponseSPARQL* owllinkNextReqRes = (CReasonerEvaluationRequestResponseSPARQL*)mNextReqRes;

				QString requestString = getAdaptedRequestNode(owllinkNextReqRes->getRequestString(), owllinkNextReqRes->getRequestOperationData());
				owllinkNextReqRes->setModifiedNode(requestString);

				QByteArray transReqByteArray(requestString.toUtf8());
				QString operationName = owllinkNextReqRes->getOperationName();


				if (mNumberInitRequests == 0) {
					mTestingTiming.start();
				}

				mWebRequest = mTransManager->createRequest(QString("http://%1").arg(mAddressString), transReqByteArray);
				mNextRequestTiming.start();
				startTimerWithIntervalLimited(TIMERIDREQUESTTIMEOUT, mRequestTimeout, 1);

				mWebResponse = mTransManager->getResponse(mWebRequest);
				mTransManager->callbackFinishedRequest(mWebResponse,new CReasonerEvaluationNextEvent(this,mCurrentOperationNumber));
				LOG(INFO,getLogDomain(),logTr("Sending SPARQL operation %1 of %2 with command '%3' to '%4'.").arg(mCurrentRequestCount+1).arg(mTotalRequestCount).arg(operationName).arg(mAddressString),this);
				return true;
			}




			QString CReasonerEvaluationRequestClientSPARQLThread::getAdaptedRequestNode(const QString& reqeustString, CSPARQLKnowledgeBaseSplittingOperationData* operationData) {
				QStringList partStringList(operationData->getOperationPartStringList());
				if (operationData->getOperationType() == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE) {
					QStringList modifiedPartStringList;
					while (!partStringList.isEmpty()) {
						const QString& part = partStringList.takeFirst();
						if (!part.startsWith("#")) {
							modifiedPartStringList.append(part);
							QString keyword = part.toUpper();
							if (keyword == "LOAD") {
								bool loadingIRIPartFound = false;
								while (!partStringList.isEmpty() && !loadingIRIPartFound) {
									const QString& loadingIRIPart = partStringList.takeFirst();
									if (loadingIRIPart.toUpper() != "SILENT") {
										loadingIRIPartFound = true;
										QString iriString = getIRI(loadingIRIPart);

										if (iriString.startsWith("file:") && !iriString.startsWith("file://")) {
											QString relFileString = iriString;
											relFileString = relFileString.remove(0, 5);
											QDir currFilePath(QDir::currentPath());
											QString absFileString = currFilePath.absoluteFilePath(relFileString);
											absFileString = QUrl::fromPercentEncoding(QUrl::fromLocalFile(absFileString).toString().toUtf8());

											modifiedPartStringList.append(QString("<%1>").arg(absFileString));
											LOG(INFO, getLogDomain(), logTr("Changed relative filepath '%1' to absolute filepath '%2'.").arg(iriString).arg(absFileString), this);
										} else {
											modifiedPartStringList.append(loadingIRIPart);
										}
									} else {
										modifiedPartStringList.append(loadingIRIPart);
									}
								}
							} else if (keyword == "PREFIX" || keyword == "@PREFIX") {
								if (partStringList.size() >= 2) {
									QString prefix = partStringList.takeFirst();
									modifiedPartStringList.append(prefix);
									QString resolvedPrefix = prefix;
									while (resolvedPrefix.endsWith(":")) {
										resolvedPrefix = resolvedPrefix.mid(0, resolvedPrefix.length() - 1);
									}
									QString iriString = partStringList.takeFirst();
									modifiedPartStringList.append(iriString);
									QString iri = getIRI(iriString);
									mPrefixHash.insert(resolvedPrefix, iri);
								}
							}
						}

					}
					return modifiedPartStringList.join(" ");
				} else {
					return reqeustString;
				}
			}


			bool CReasonerEvaluationRequestClientSPARQLThread::loadReasonerRequests(const QString& initFileString, const QString& testFileString) {
				mCurrentRequestCount = 0;
				mTotalRequestCount = 0;
				bool loaded = loadReasonerRequest(initFileString);
				mNumberInitRequests = mRemainingRequestList.count();
				loaded &= loadReasonerRequest(testFileString);
				return loaded;
			}


			QString CReasonerEvaluationRequestClientSPARQLThread::getIRI(const QString& iriString) {
				if (iriString.startsWith("<") && iriString.endsWith(">")) {
					return iriString.mid(1, iriString.length() - 2);
				} else {
					int colonPos = iriString.indexOf(':');
					if (colonPos != -1) {
						QString prefix = iriString.mid(0, colonPos);
						QString prefixResolvedString = mPrefixHash.value(prefix);
						if (!prefixResolvedString.isEmpty()) {
							QString remainingIRI = iriString.mid(colonPos + 1);
							if (!remainingIRI.isEmpty()) {
								return prefixResolvedString + remainingIRI;
							}
						}
					}

					return mBaseIRI + iriString;
				}
			}


			bool CReasonerEvaluationRequestClientSPARQLThread::loadReasonerRequest(const QString& requestFileString) {
				CSPARQLKnowledgeBaseSplittingOperationParser parser;
				parser.parseQueryFile(requestFileString);
				mBaseIRI = requestFileString;
				cint64 loadedRequestCount = 0;
				QList<CSPARQLKnowledgeBaseSplittingOperationData*> operationDataList = parser.getSPARQLOperationList();
				bool hasUpdatingOperation = false;
				for (CSPARQLKnowledgeBaseSplittingOperationData* operationData : operationDataList) {
					CReasonerEvaluationRequestResponseSPARQL* requestRespone = new CReasonerEvaluationRequestResponseSPARQL(*operationData);
					QStringList partStringList(operationData->getOperationPartStringList());
					requestRespone->setRequestNode(partStringList.join(" "));


					bool queryingOperation = false;
					bool updatingOperation = false;
					QString operationMode;
					QString operationName;
					if (requestRespone->getRequestOperationData()->getOperationType() == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_QUERY) {
						operationMode = "QUERY";
						queryingOperation = true;
					} else if (requestRespone->getRequestOperationData()->getOperationType() == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE) {
						operationMode = "UPDATE";
						updatingOperation = true;
					} else if (requestRespone->getRequestOperationData()->getOperationType() == CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MODIFY) {
						operationMode = "UPDATE";
						updatingOperation = true;
					}
					requestRespone->setOperationMode(operationMode);
					for (const QString& partString : partStringList) {
						if (mSPARQLOperationKeywordNameSet.contains(partString.toUpper())) {
							operationName = mSPARQLOperationKeywordNameSet.value(partString.toUpper());
							break;
						}
					}
					if (!operationName.isEmpty()) {
						requestRespone->setOperationName(operationName);
					} else {
						requestRespone->setOperationMode(operationMode);
					}

					if (mConfSendPrepareBeforeQuerying && mConfSupportsPrepareCommand) {
						if (hasUpdatingOperation && queryingOperation) {
							CSPARQLKnowledgeBaseSplittingOperationData prepareOperation(CSPARQLKnowledgeBaseSplittingOperationData::SPARQL_UPDATE_MANAGE, operationData->getKnowledgeBaseString(), QStringList() << "PREPARE" << "QUERYING" << operationData->getKnowledgeBaseString());
							CReasonerEvaluationRequestResponseSPARQL* prepareRequestRespone = new CReasonerEvaluationRequestResponseSPARQL(prepareOperation);
							prepareRequestRespone->setOperationMode("PREPARE");
							prepareRequestRespone->setOperationName(mSPARQLOperationKeywordNameSet.value("PREPARE"));
							mRemainingRequestList.append(prepareRequestRespone);
							++mTotalRequestCount;
							++loadedRequestCount;
							hasUpdatingOperation = false;
						} else {
							hasUpdatingOperation |= updatingOperation;
						}
					}


					mRemainingRequestList.append(requestRespone);


					++mTotalRequestCount;
					++loadedRequestCount;
				}
				LOG(INFO, getLogDomain(), logTr("Loaded %1 SPARQL operations from file '%2'.").arg(loadedRequestCount).arg(requestFileString), this);

				return true;
			}






		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
