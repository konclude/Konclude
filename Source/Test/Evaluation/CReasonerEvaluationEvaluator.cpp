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

#include "CReasonerEvaluationEvaluator.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationEvaluator::CReasonerEvaluationEvaluator() {
			}


			CReasonerEvaluationEvaluator::~CReasonerEvaluationEvaluator() {
			}


			bool CReasonerEvaluationEvaluator::evaluateResults(CReasonerEvaluationRequestResult* requestResult, CReasonerEvaluationTerminationResult* terminationResult, const QString& inputFileString, const QString& responseFileString, CConfiguration* config) {
				QDomDocument document = createResponseDocument();
				QDomElement responseNode = document.documentElement();
				responseNode.setAttribute("response-time",requestResult->getResponseTime());
				responseNode.setAttribute("test",inputFileString);
				if (!requestResult->getResponsedInTime()) {
					responseNode.setAttribute("timeout",1);
				} else {
					responseNode.setAttribute("timeout",0);
				}

				QDateTime dateTime = QDateTime::currentDateTime();
				responseNode.appendChild(document.createComment(QString("Executed date: %1").arg(dateTime.toString())));

				if (terminationResult->hasProcessError()) {
					responseNode.appendChild(document.createComment(QString("Reasoner error: %1").arg(terminationResult->getErrorText())));
				}

				QString output = terminationResult->getStandardOutputText();
				if (output.isEmpty()) {
					output = terminationResult->getErrorOutputText();
				}
				responseNode.appendChild(document.createComment(QString("Reasoner output:\r\n%1").arg(output)));


				QList<CReasonerEvaluationRequestResponse*>* responseList = requestResult->getResponseList();
				foreach (CReasonerEvaluationRequestResponse* response, *responseList) {
					if (response->isTimedOut()) {
						QDomElement errorElm = document.createElement("Error");
						errorElm.setAttribute("timeout",1);
						cint64 responseTime = response->getResponseTime();
						errorElm.setAttribute("response-time",responseTime);
						errorElm.setAttribute("error","timeout");
						errorElm.setAttribute("request-command",response->getRequestNode().tagName());
						responseNode.appendChild(errorElm);
					} else if (response->hasResponseError()) {
						QDomElement errorElm = document.createElement("Error");
						cint64 responseTime = response->getResponseTime();
						errorElm.setAttribute("response-time",responseTime);
						errorElm.setAttribute("error","unknown error");
						errorElm.setAttribute("request-command",response->getRequestNode().tagName());
						responseNode.appendChild(errorElm);
					} else {
						QDomElement domElm = response->getResponseNode();
						cint64 responseTime = response->getResponseTime();
						QDomNode importedNode = document.importNode(domElm,true);
						QDomElement impDomElm = importedNode.toElement();
						impDomElm.setAttribute("response-time",responseTime);
						impDomElm.setAttribute("request-command",response->getRequestNode().tagName());
						responseNode.appendChild(impDomElm);
					}
				}

				QFile file(responseFileString);
				if (file.open(QIODevice::WriteOnly)) {
					file.write(document.toByteArray());
					file.close();
				}

				return true;
			}

			QDomDocument CReasonerEvaluationEvaluator::createResponseDocument() {
				QDomDocument document;

				QString owllinkNS = "http://www.owllink.org/owllink#";
				QString owlNS = "http://www.w3.org/2002/07/owl#";

				QDomElement rootNode;

				rootNode = document.createElementNS(owllinkNS,"ResponseMessage");
				//rootNode.setAttributeNS("xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
				rootNode.setAttributeNS("http://www.w3.org/2001/XMLSchema-instance","xsi:schemaLocation","http://www.owllink.org/owllink# http://www.owllink.org/owllink-20091116.xsd");
				rootNode.setAttribute("xmlns:owl",owlNS);
				document.appendChild(rootNode);

				return document;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
