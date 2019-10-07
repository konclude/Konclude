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

#include "CReasonerEvaluationEvaluatorSPARQL.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationEvaluatorSPARQL::CReasonerEvaluationEvaluatorSPARQL() {
			}


			CReasonerEvaluationEvaluatorSPARQL::~CReasonerEvaluationEvaluatorSPARQL() {
			}


			bool CReasonerEvaluationEvaluatorSPARQL::evaluateResults(CReasonerEvaluationRequestResult* requestResult, CReasonerEvaluationTerminationResult* terminationResult, const QString& inputFileString, const QString& responseFileString, CConfiguration* config) {
				try {
					evaluateResults(requestResult, terminationResult, inputFileString, responseFileString, config, true, true);
				} catch (...) {
					LOG(WARN, "::Konclude::Test::ReasonerEvaluationEvaluatorSPARQL", logTr("Writing evaluation result to '%1' failed, trying to write it without reasoner outputs.").arg(responseFileString), this);
					try {
						evaluateResults(requestResult, terminationResult, inputFileString, responseFileString, config, true, false);
					} catch (...) {
						LOG(WARN, "::Konclude::Test::ReasonerEvaluationEvaluatorSPARQL", logTr("Writing evaluation result to '%1' failed, trying to write it without full responses.").arg(responseFileString), this);
						try {
							evaluateResults(requestResult, terminationResult, inputFileString, responseFileString, config, false, true);
						} catch (...) {
							LOG(WARN, "::Konclude::Test::ReasonerEvaluationEvaluatorSPARQL", logTr("Writing evaluation result to '%1' failed, trying to write it without full responses and without reasoner outputs.").arg(responseFileString), this);
							try {
								evaluateResults(requestResult, terminationResult, inputFileString, responseFileString, config, false, false);
							} catch (...) {
								LOG(ERROR, "::Konclude::Test::ReasonerEvaluationEvaluatorSPARQL", logTr("Writing evaluation result to '%1' failed.").arg(responseFileString), this);
								return false;
							}
						}
					}
				}
				return true;
			}

			bool CReasonerEvaluationEvaluatorSPARQL::evaluateResults(CReasonerEvaluationRequestResult* requestResult, CReasonerEvaluationTerminationResult* terminationResult, const QString& inputFileString, const QString& responseFileString, CConfiguration* config, bool includeResponses, bool includeOutputs) {

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

				QString output = terminationResult->getStandardOutputText().replace("--", "##");
				QString errorOutput = terminationResult->getErrorOutputText().replace("--", "##");
				if (includeOutputs) {
					if (!output.isEmpty()) {
						responseNode.appendChild(document.createComment(QString("Reasoner output:\r\n%1").arg(output)));
					}
					if (!errorOutput.isEmpty()) {
						responseNode.appendChild(document.createComment(QString("Reasoner error output:\r\n%1").arg(errorOutput)));
					}
				} else {
					responseNode.appendChild(document.createComment(QString("Reasoner (error) output with length (%1) %2 omitted due to size limitations").arg(output.length()).arg(errorOutput.length())));
				}


				QList<CReasonerEvaluationRequestResponse*>* responseList = requestResult->getResponseList();
				foreach (CReasonerEvaluationRequestResponse* response, *responseList) {
					CReasonerEvaluationRequestResponseSPARQL* sparqlResponse = (CReasonerEvaluationRequestResponseSPARQL*)response;
					if (response->isTimedOut()) {
						QDomElement errorElm = document.createElement("Error");
						errorElm.setAttribute("timeout",1);
						cint64 responseTime = response->getResponseTime();
						errorElm.setAttribute("response-time",responseTime);
						errorElm.setAttribute("error","timeout");
						errorElm.setAttribute("request-command", sparqlResponse->getOperationName());
						responseNode.appendChild(errorElm);
					} else if (response->hasResponseError()) {
						QDomElement errorElm = document.createElement("Error");
						cint64 responseTime = response->getResponseTime();
						errorElm.setAttribute("response-time",responseTime);
						if (!response->getResponseErrorString().isEmpty()) {
							errorElm.setAttribute("error", response->getResponseErrorString());
						} else {
							errorElm.setAttribute("error", "unknown error");
						}
						errorElm.setAttribute("request-command", sparqlResponse->getOperationName());
						responseNode.appendChild(errorElm);
					} else if (response->hasEvaluationError()) {
						QDomElement errorElm = document.createElement("EvaluationError");
						cint64 responseTime = response->getResponseTime();
						errorElm.setAttribute("response-time", responseTime);
						QString evalErrorString = response->getEvaluationErrorString();
						if (!evalErrorString.isEmpty()) {
							errorElm.setAttribute("evaluation-error", QString("Could not process response due to '%1'").arg(response->getEvaluationErrorString()));
						} else {
							errorElm.setAttribute("evaluation-error", "Could not process response");
						}
						errorElm.setAttribute("request-command", sparqlResponse->getOperationName());
						responseNode.appendChild(errorElm);
					} else {
						QList<QDomElement> domElms = sparqlResponse->getResponseNodes();
						if (includeResponses || domElms.isEmpty()) {
							if (domElms.isEmpty()) {
								domElms.append(document.createElement("OK"));
							}
							bool first = true;
							for (const QDomElement& domElm : domElms) {
								cint64 responseTime = response->getResponseTime();
								QDomNode importedNode = document.importNode(domElm, true);
								QDomElement impDomElm = importedNode.toElement();
								if (first) {
									first = false;
									impDomElm.setAttribute("response-time", responseTime);
									impDomElm.setAttribute("request-command", sparqlResponse->getOperationName());
								}
								responseNode.appendChild(impDomElm);
							}
						} else {
							QDomElement errorElm = document.createElement("OK");
							errorElm.setAttribute("evaluation-error", "Response omitted due to size limitations");
							cint64 responseTime = response->getResponseTime();
							errorElm.setAttribute("response-time", responseTime);
							errorElm.setAttribute("request-command", sparqlResponse->getOperationName());
							responseNode.appendChild(errorElm);
						}
					}
				}

				QFile file(responseFileString);
				if (file.open(QIODevice::WriteOnly)) {
					QTextStream fileTextStream(&file);
					document.save(fileTextStream, 1);
					file.close();
				}

				return true;
			}

			QDomDocument CReasonerEvaluationEvaluatorSPARQL::createResponseDocument() {
				QDomDocument document;

				QString sparqlNS = "http://www.w3.org/2005/sparql-results#";
				QString owlNS = "http://www.w3.org/2002/07/owl#";

				QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
				document.appendChild(procInstHeader);
				QDomElement rootNode;
				rootNode = document.createElementNS(sparqlNS, "sparql");
				rootNode.setAttribute("xml:base", sparqlNS);
				rootNode.setAttribute("xmlns:owl", owlNS);
				rootNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema#");
				document.appendChild(rootNode);

				return document;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
