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

#include "CReasonerEvaluationRequestClientOWLlinkThread.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationRequestClientOWLlinkThread::CReasonerEvaluationRequestClientOWLlinkThread() : CReasonerEvaluationRequestClientBaseThread("Reasoner-Evaluation-OWLlink-Request-Client", "::Konclude::Test::ReasonerEvaluationOWLlinkRequestClient") {
			}


			CReasonerEvaluationRequestClientOWLlinkThread::~CReasonerEvaluationRequestClientOWLlinkThread() {
			}


			bool CReasonerEvaluationRequestClientOWLlinkThread::canHandleRequestFile(const QString& testFileString) {
				if (testFileString.endsWith(".xml")) {
					return true;
				}
				return false;
			}


			bool CReasonerEvaluationRequestClientOWLlinkThread::parseResponse(QIODevice* device) {
				bool responseError = false;
				QDomDocument document;
				if (document.setContent(device)) {

					QDomElement rootNode = document.documentElement();
					QDomElement responseNode = rootNode.firstChildElement();
					((CReasonerEvaluationRequestResponseOWLlink*)mNextReqRes)->setResponseNode(responseNode);
					if (responseNode.isNull()) {
						responseError = true;
					}

				} else {
					responseError = true;
				}
				return responseError;
			}


			bool CReasonerEvaluationRequestClientOWLlinkThread::sendNextRequest() {
				if (mRemainingRequestList.isEmpty()) {
					return false;
				}
				mNextReqRes = mRemainingRequestList.takeFirst();
				CReasonerEvaluationRequestResponseOWLlink* owllinkNextReqRes = (CReasonerEvaluationRequestResponseOWLlink*)mNextReqRes;
				QDomElement requestElement = owllinkNextReqRes->getModifiedNode();
				QString nodeTagString(requestElement.tagName());
				if (nodeTagString == "#comment" || nodeTagString == "#text") {
					LOG(INFO,getLogDomain(),logTr("Skipped OWLlink request %1 of %2, because it is text or comment.").arg(mCurrentRequestCount+1).arg(mTotalRequestCount),this);
					owllinkNextReqRes->setResponseNode(requestElement);
					mResponse->addResponse(owllinkNextReqRes,mNumberInitRequests-- > 0);
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




			QDomElement CReasonerEvaluationRequestClientOWLlinkThread::getAdabtedRequestNode(QDomElement& el) {
				if (mModifyRequestAbsolutePaths && el.tagName() == "OntologyIRI") {
					QString ontoIRI = el.attribute("IRI");
					QString relFileString = ontoIRI;
					relFileString = relFileString.remove(0,5);
					QDir currFilePath(QDir::currentPath());
					QString absFileString = currFilePath.absoluteFilePath(relFileString);
					if (ontoIRI.startsWith("file:") && !ontoIRI.startsWith("file://")) {
						absFileString = QUrl::fromPercentEncoding(QUrl::fromLocalFile(absFileString).toString().toUtf8());
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




			bool CReasonerEvaluationRequestClientOWLlinkThread::startsWithScheme(const QString& uriString) {
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

			QDomElement CReasonerEvaluationRequestClientOWLlinkThread::getResolvedAbbreviatedIRIsNode(QDomElement& el, const QHash<QString,QString>& appreviatePrefixHash, const QString& xmlBase) {
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


			bool CReasonerEvaluationRequestClientOWLlinkThread::loadReasonerRequests(const QString& initFileString, const QString& testFileString) {
				mCurrentRequestCount = 0;
				mTotalRequestCount = 0;
				bool loaded = loadReasonerRequest(initFileString);
				mNumberInitRequests = mRemainingRequestList.count();
				loaded &= loadReasonerRequest(testFileString);
				return loaded;
			}


			bool CReasonerEvaluationRequestClientOWLlinkThread::loadReasonerRequest(const QString& requestFileString) {
				QFile file(requestFileString);
				if (file.open(QIODevice::ReadOnly)) {
					QDomDocument document;
					document.setContent(&file,false);
					file.close();
					QDomElement rootEl = document.documentElement();
					cint64 loadedRequestCount = 0;

					QDomElement childEl = rootEl.firstChildElement();
					while (!childEl.isNull()) {
						CReasonerEvaluationRequestResponseOWLlink* requestRespone = new CReasonerEvaluationRequestResponseOWLlink();
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


			QDomDocument CReasonerEvaluationRequestClientOWLlinkThread::createRequestDocument(const QDomElement& reqNode) {
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



			QDomElement CReasonerEvaluationRequestClientOWLlinkThread::getModifiedRequestNode(const QDomElement& el) {
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
