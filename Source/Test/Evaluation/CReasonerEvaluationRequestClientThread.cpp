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

#include "CReasonerEvaluationRequestClientThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestClientThread::CReasonerEvaluationRequestClientThread() : CIntervalThread("Reasoner-Evaluation-Request-Client"),CLogIdentifier("::Konclude::Test::ReasonerEvaluationRequestClient",this) {
				startThread();

			}


			CReasonerEvaluationRequestClientThread::~CReasonerEvaluationRequestClientThread() {
				stopThread();
			}


			bool CReasonerEvaluationRequestClientThread::evaluateReasoner(const QString& initFileString, const QString& testFileString, const QString& addressString, CConfiguration* configuration, CCallbackData* callback) {
				postEvent(new CReasonerEvaluationStartEvent(initFileString,testFileString,addressString,configuration,callback));
				return true;
			}

			CReasonerEvaluationRequestResult* CReasonerEvaluationRequestClientThread::getReasonerEvaluationResult() {
				return mResponse;
			}


			bool CReasonerEvaluationRequestClientThread::processTimer(qint64 timerID) {
				if (timerID == TIMERIDREQUESTTIMEOUT || timerID == TIMERIDTOTALTIMEOUT) {
					++mCurrentOperationNumber;
					closeAllRequestTimeout();
					return true;
				}
				return false;
			}


			void CReasonerEvaluationRequestClientThread::threadStarted() {
				CIntervalThread::threadStarted();
				mCurrentOperationNumber = 0;
				mModifyRequestAbsolutePaths = true;
				mReplaceLoadOntologiesWithTells = false;
				mResolveAppreviatedIRIsForReplacedTells = false;
				mResponse = nullptr;
				mTransManager = new CQtHttpTransactionManager(1000*60*60*24*14);
			}

			void CReasonerEvaluationRequestClientThread::threadStopped() {
				CIntervalThread::threadStopped();
				delete mTransManager;
			}


			bool CReasonerEvaluationRequestClientThread::processCustomsEvents(QEvent::Type type, CCustomEvent *event) {
				if (CIntervalThread::processCustomsEvents(type,event)) {
					return true;
				} else if (type == CReasonerEvaluationStartEvent::EVENTTYPE) {
					CReasonerEvaluationStartEvent* rese = (CReasonerEvaluationStartEvent *)event;

					mResponse = new CReasonerEvaluationRequestResult();
					mInitFileString = rese->getInitFileString();
					mTestFileString = rese->getTestFileString();
					mAddressString = rese->getAddressString();
					mConfig = rese->getConfiguration();
					mRequestFinishedCallback = rese->getCallback();
					++mCurrentOperationNumber;

					mRequestTimeout = CConfigDataReader::readConfigInteger(mConfig,"Konclude.Evaluation.RequestTimeout");
					mTotalTimeout = CConfigDataReader::readConfigInteger(mConfig,"Konclude.Evaluation.TestingTimeout");

					mModifyRequestAbsolutePaths = CConfigDataReader::readConfigInteger(mConfig,"Konclude.Evaluation.RequestFileAbsolutePathsModification");
					mReplaceLoadOntologiesWithTells = CConfigDataReader::readConfigInteger(mConfig,"Konclude.Evaluation.RequestReplaceLoadOntologiesWithTell");
					mResolveAppreviatedIRIsForReplacedTells = CConfigDataReader::readConfigInteger(mConfig,"Konclude.Evaluation.ResolveAppreviatedIRIsForReplacedTells");

					loadReasonerRequests(mInitFileString,mTestFileString);

					mTotalTiming.start();
					startTimerWithIntervalLimited(TIMERIDTOTALTIMEOUT,mTotalTimeout,1);

					bool sendedNextRequest = false;
					while (!sendedNextRequest) {
						if (hasNextRequest()) {
							sendedNextRequest = sendNextRequest();
						} else {
							finishReasonerRequests();
							break;
						}
					}

					return this;
				
				} else if (type == CReasonerEvaluationNextEvent::EVENTTYPE) {
					CReasonerEvaluationNextEvent* rene = (CReasonerEvaluationNextEvent *)event;

					if (rene->getOperationNumber() == mCurrentOperationNumber) {
						closeNextRequest();

						bool sendedNextRequest = false;
						while (!sendedNextRequest) {
							if (hasNextRequest()) {
								sendedNextRequest = sendNextRequest();
							} else {
								finishReasonerRequests();
								break;
							}
						}
					}

					return this;
				}
				return false;
			}


			bool CReasonerEvaluationRequestClientThread::finishReasonerRequests() {
				cint64 totalTime = mTotalTiming.elapsed();
				cint64 testingTime = mTestingTiming.elapsed();
				mResponse->setResponseTime(testingTime);
				stopTimer(TIMERIDREQUESTTIMEOUT);
				stopTimer(TIMERIDTOTALTIMEOUT);				
				if (mRequestFinishedCallback) {
					mRequestFinishedCallback->doCallback();
					mRequestFinishedCallback = nullptr;
				}
				return true;
			}


			bool CReasonerEvaluationRequestClientThread::closeAllRequestTimeout() {
				cint64 elapsedTime = mNextRequestTiming.elapsed();
				mNextReqRes->setResponseTime(elapsedTime);

				LOG(INFO,getLogDomain(),logTr("OWLlink request %1 of %2 timed out.").arg(1+mCurrentRequestCount++).arg(mTotalRequestCount),this);

				mNextReqRes->setTimedOut(true);
				mResponse->setResponsedInTime(false);
				mResponse->addResponse(mNextReqRes,mNumberInitRequests-- > 0);

				while (!mRemainingRequestList.isEmpty()) {
					CReasonerEvaluationRequestResponse* nextReqResp = mRemainingRequestList.takeFirst();
					nextReqResp->setResponseError(true);
					mResponse->addResponse(nextReqResp,mNumberInitRequests-- > 0);
				}

				finishReasonerRequests();
				return true;
			}


			bool CReasonerEvaluationRequestClientThread::closeNextRequest() {
				stopTimer(TIMERIDREQUESTTIMEOUT);

				cint64 elapsedTime = mNextRequestTiming.elapsed();
				mNextReqRes->setResponseTime(elapsedTime);

				LOG(INFO,getLogDomain(),logTr("Received OWLlink response %1 of %2 in %3 ms.").arg(1+mCurrentRequestCount++).arg(mTotalRequestCount).arg(elapsedTime),this);

				QString* siteText = mTransManager->getExtractedText(mWebResponse);
				bool responseError = false;
				if (siteText && !siteText->isEmpty()) {

					QDomDocument document;
					if (document.setContent(*siteText)) {

						QDomElement rootNode = document.documentElement();
						QDomElement responseNode = rootNode.firstChildElement();
						mNextReqRes->setResponseNode(responseNode);
						if (responseNode.isNull()) {
							responseError = true;
						}

					} else {
						responseError = true;
					}
					delete siteText;

				} else {
					responseError = true;
				}
				mNextReqRes->setResponseError(responseError);
				if (!responseError) {
					mResponse->setSucessfullReasonerCommunication(true);
				}

				mResponse->addResponse(mNextReqRes,mNumberInitRequests-- > 0);
				mNextReqRes = nullptr;

				mTransManager->releaseResponse(mWebResponse);
				return true;
			}


			bool CReasonerEvaluationRequestClientThread::hasNextRequest() {
				return !mRemainingRequestList.isEmpty();
			}

			bool CReasonerEvaluationRequestClientThread::sendNextRequest() {
				if (mRemainingRequestList.isEmpty()) {
					return false;
				}
				mNextReqRes = mRemainingRequestList.takeFirst();
				QDomElement requestElement = mNextReqRes->getModifiedNode();
				QString nodeTagString(requestElement.tagName());
				if (nodeTagString == "#comment" || nodeTagString == "#text") {
					LOG(INFO,getLogDomain(),logTr("Skipped OWLlink request %1 of %2, because it is text or comment.").arg(mCurrentRequestCount+1).arg(mTotalRequestCount),this);
					mNextReqRes->setResponseNode(requestElement);
					mResponse->addResponse(mNextReqRes,mNumberInitRequests-- > 0);
					return false;
				} else {
					QDomDocument document = createRequestDocument(getAdabtedRequestNode(requestElement));

					QByteArray transReqByteArray(document.toByteArray());

					if (mNumberInitRequests == 0) {
						mTestingTiming.start();
					}

					mWebRequest = mTransManager->createRequest(QString("http://%1").arg(mAddressString),transReqByteArray);
					mNextRequestTiming.start();
					startTimerWithIntervalLimited(TIMERIDREQUESTTIMEOUT,mRequestTimeout,1);

					mWebResponse = mTransManager->getResponse(mWebRequest);
					mTransManager->callbackFinishedRequest(mWebResponse,new CReasonerEvaluationNextEvent(this,mCurrentOperationNumber));
					LOG(INFO,getLogDomain(),logTr("Sending OWLlink request %1 of %2 with command '%3' to '%4'.").arg(mCurrentRequestCount+1).arg(mTotalRequestCount).arg(requestElement.tagName()).arg(mAddressString),this);
					return true;
				}
			}




			QDomElement CReasonerEvaluationRequestClientThread::getAdabtedRequestNode(QDomElement& el) {
				if (mModifyRequestAbsolutePaths && el.tagName() == "OntologyIRI") {
					QString ontoIRI = el.attribute("IRI");
					QString relFileString = ontoIRI;
					relFileString = relFileString.remove(0,5);
					QDir currFilePath(QDir::currentPath());
					QString absFileString = currFilePath.absoluteFilePath(relFileString);
					if (ontoIRI.startsWith("file:") && !ontoIRI.startsWith("file://")) {
						absFileString = QUrl::fromLocalFile(absFileString).toString();
						el.setAttribute("IRI",absFileString);
						LOG(INFO,getLogDomain(),logTr("Changed relative filepath '%1' to absolute filepath '%2'.").arg(ontoIRI).arg(absFileString),this);
					}
					return el;
				} 

				if (mReplaceLoadOntologiesWithTells && el.tagName() == "LoadOntologies") {
					QString kbString = el.attribute("kb");
					QDomDocument document;
					QString owllinkNS = "http://www.owllink.org/owllink#";
					QString owlNS = "http://www.w3.org/2002/07/owl#";
					QDomElement newTellNode;
					newTellNode = document.createElementNS(owllinkNS,"Tell");
					//newTellNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
					newTellNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
					newTellNode.setAttribute("xmlns:owl",owlNS);
					document.appendChild(newTellNode);
					newTellNode.setAttribute("kb",kbString);

					QDomElement ontNode = el.firstChildElement("OntologyIRI");
					while (!ontNode.isNull()) {
						QString ontoIRI = ontNode.attribute("IRI");

						QHash<QString,QString> appreviatePrefixHash;

						QString relFileString = ontoIRI;
						relFileString = relFileString.remove(0,5);
						QFile ontAxFile(relFileString);
						if (ontAxFile.open(QIODevice::ReadOnly)) {
							QDomDocument ontAxDoc;
							ontAxDoc.setContent(&ontAxFile,true);
							QDomElement rootNode = ontAxDoc.firstChildElement();
							QString xmlBase = rootNode.attribute("base");

							QDomElement prefixNode = rootNode.firstChildElement("Prefix");
							while (!prefixNode.isNull()) {
								const QString& prefixName = prefixNode.attribute("name");
								const QString& prefixIRI = prefixNode.attribute("IRI");
								appreviatePrefixHash.insert(prefixName,prefixIRI);
								prefixNode = prefixNode.nextSiblingElement("Prefix");
							}

							QDomElement axiomNode = rootNode.firstChildElement();
							while (!axiomNode.isNull()) {
								if (!mResolveAppreviatedIRIsForReplacedTells || axiomNode.tagName() != "Prefix") {
									QDomElement importedAxiomNode = document.importNode(axiomNode,true).toElement();
									if (mResolveAppreviatedIRIsForReplacedTells) {
										importedAxiomNode = getResolvedAbbreviatedIRIsNode(importedAxiomNode,appreviatePrefixHash,xmlBase);
									}
									newTellNode.appendChild(importedAxiomNode);
								}
								axiomNode = axiomNode.nextSiblingElement();
							}
							ontAxFile.close();
						}
						ontNode = ontNode.nextSiblingElement("OntologyIRI");
					}
					return newTellNode;
				}

				QDomElement child(el.firstChildElement());
				while (!child.isNull()) {
					getAdabtedRequestNode(child);
					child = child.nextSiblingElement();
				}
				return el;
			}




			bool CReasonerEvaluationRequestClientThread::startsWithScheme(const QString& uriString) {
				for (int i = 0; i < uriString.count()-1; ++i) {
					const QChar& c = uriString.at(i);
					if (!c.isLetterOrNumber() && c != '.') {
						if (c == ':') {
							return i != 0;
						}
						return false;
					}
				}
				return false;
			}

			QDomElement CReasonerEvaluationRequestClientThread::getResolvedAbbreviatedIRIsNode(QDomElement& el, const QHash<QString,QString>& appreviatePrefixHash, const QString& xmlBase) {
				QString nextXMLBase = xmlBase;
				if (el.hasAttribute("base")) {
					nextXMLBase = el.attribute("base");
				}
				if (el.hasAttribute("IRI")) {
					QString iriString = el.attribute("IRI");
					if (!startsWithScheme(iriString)) {
						QString iriStringWithXMLBase = nextXMLBase+iriString;
						el.setAttribute("IRI",iriStringWithXMLBase);
					}
				}
				if (el.hasAttribute("abbreviatedIRI")) {
					QString abbreviatedIRIString = el.attribute("abbreviatedIRI");
					el.removeAttribute("abbreviatedIRI");
					cint64 colonPos = abbreviatedIRIString.indexOf(":");
					QString prefixName = abbreviatedIRIString.mid(0,colonPos);
					QString extensionName = appreviatePrefixHash.value(prefixName);
					QString resolvedName = extensionName+abbreviatedIRIString.mid(colonPos+1);
					el.setAttribute("IRI",resolvedName);
				}
				QDomElement childNode = el.firstChildElement();
				while (!childNode.isNull()) {
					getResolvedAbbreviatedIRIsNode(childNode,appreviatePrefixHash,nextXMLBase);
					childNode = childNode.nextSiblingElement();
				}
				return el;
			}


			bool CReasonerEvaluationRequestClientThread::loadReasonerRequests(const QString& initFileString, const QString& testFileString) {
				mCurrentRequestCount = 0;
				mTotalRequestCount = 0;
				bool loaded = loadReasonerRequest(initFileString);
				mNumberInitRequests = mRemainingRequestList.count();
				loaded &= loadReasonerRequest(testFileString);
				return loaded;
			}


			bool CReasonerEvaluationRequestClientThread::loadReasonerRequest(const QString& requestFileString) {
				QFile file(requestFileString);
				if (file.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&file,false);
					file.close();
					QDomElement rootEl = document.documentElement();
					cint64 loadedRequestCount = 0;

					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						CReasonerEvaluationRequestResponse* requestRespone = new CReasonerEvaluationRequestResponse();
						requestRespone->setRequestNode(childEl);
						QDomElement modifiedEl = getModifiedRequestNode(childEl);
						requestRespone->setModifiedNode(modifiedEl);
						mRemainingRequestList.append(requestRespone);
						++mTotalRequestCount;
						++loadedRequestCount;
						childEl = childEl.nextSiblingElement();
					}
					LOG(INFO,getLogDomain(),logTr("Loaded %1 OWLlink requests from file '%2'.").arg(loadedRequestCount).arg(requestFileString),this);

				}
				return true;
			}


			QDomDocument CReasonerEvaluationRequestClientThread::createRequestDocument(const QDomElement& reqNode) {
				QDomDocument document;

				QString owllinkNS = "http://www.owllink.org/owllink#";
				QString owlNS = "http://www.w3.org/2002/07/owl#";

				QDomElement rootNode;

				rootNode = document.createElementNS(owllinkNS,"RequestMessage");
				//rootNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
				rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
				rootNode.setAttribute("xmlns:owl",owlNS);
				document.appendChild(rootNode);

				QDomNode importedReqNode = document.importNode(reqNode,true);

				rootNode.appendChild(importedReqNode);

				return document;
			}



			QDomElement CReasonerEvaluationRequestClientThread::getModifiedRequestNode(const QDomElement& el) {
				QDomElement modNode = el;
				QDomElement exResNode = modNode.firstChildElement("ExpectedResult");
				while (!exResNode.isNull()) {
					modNode.removeChild(exResNode);
					exResNode = modNode.firstChildElement("ExpectedResult");
				}
				return modNode;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
