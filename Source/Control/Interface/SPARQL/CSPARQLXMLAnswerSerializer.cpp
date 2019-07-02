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

#include "CSPARQLXMLAnswerSerializer.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				CSPARQLXMLAnswerSerializer::CSPARQLXMLAnswerSerializer() {
					sparqlNS = "http://www.w3.org/2005/sparql-results#";
					owlNS = "http://www.w3.org/2002/07/owl#";
				}


				CSPARQLXMLAnswerSerializer::~CSPARQLXMLAnswerSerializer() {
				}



				QByteArray CSPARQLXMLAnswerSerializer::serializeResult(CBooleanQueryResult* booleanResult) {
					QDomDocument document;
					QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
					document.appendChild(procInstHeader);
					QDomElement rootNode;
					rootNode = document.createElementNS(sparqlNS, "sparql");
					rootNode.setAttribute("xml:base", sparqlNS);
					rootNode.setAttribute("xmlns:owl", owlNS);
					rootNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema#");
					document.appendChild(rootNode);

					QList<QDomElement> resultsElements = getResultElements(booleanResult, document);
					for (QDomElement element : resultsElements) {
						rootNode.appendChild(element);
					}

					return document.toString().toUtf8();

				}



				QByteArray CSPARQLXMLAnswerSerializer::serializeResult(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult) {
					QDomDocument document;
					QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
					document.appendChild(procInstHeader);
					QDomElement rootNode;
					rootNode = document.createElementNS(sparqlNS, "sparql");
					rootNode.setAttribute("xml:base", sparqlNS);
					rootNode.setAttribute("xmlns:owl", owlNS);
					rootNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema#");
					document.appendChild(rootNode);

					QList<QDomElement> resultsElements = getResultElements(varList, variableBindingsAnswersResult, document);
					for (QDomElement element : resultsElements) {
						rootNode.appendChild(element);
					}

					return document.toString().toUtf8();
				}





				CSPARQLXMLAnswerSerializer* CSPARQLXMLAnswerSerializer::addResultError(const QString& errorText, const QString &nodeString, cint64 pos) {
					initializeCombinedDocument();
					

					QDomElement errorNode = mCombinedDocument.createElement(nodeString);

					QDomElement errorTextNodePa = mCombinedDocument.createElement(QString("%1Text").arg(nodeString));

					QDomText errorTextNode = mCombinedDocument.createTextNode(errorText);
					errorNode.setAttribute("error", errorText);

					errorTextNodePa.appendChild(errorTextNode);
					errorNode.appendChild(errorTextNodePa);

					
					mPosSerializedResultHash.insertMulti(pos, QList<QDomElement>()<<errorNode);
					return this;
				}




				CSPARQLXMLAnswerSerializer* CSPARQLXMLAnswerSerializer::addResultSerialization(CBooleanQueryResult* booleanResult, cint64 pos) {
					initializeCombinedDocument();
					QList<QDomElement> elements = getResultElements(booleanResult, mCombinedDocument);
					mPosSerializedResultHash.insertMulti(pos, elements);
					return this;
				}

				CSPARQLXMLAnswerSerializer* CSPARQLXMLAnswerSerializer::addResultSerialization(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult, cint64 pos) {
					initializeCombinedDocument();
					QList<QDomElement> elements = getResultElements(varList, variableBindingsAnswersResult, mCombinedDocument);
					mPosSerializedResultHash.insertMulti(pos, elements);
					return this;
				}


				CSPARQLXMLAnswerSerializer* CSPARQLXMLAnswerSerializer::addResultStatistics(CQueryStatistics* queryStats, cint64 pos) {
					initializeCombinedDocument();
#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
					if (queryStats) {
						QList<QString> statStrings(queryStats->getStatisticsNameStringList());
						if (statStrings.count() >= 1) {
							QVector<QDomElement> statNodeVec(statStrings.count() + 1);
							QList<QDomElement> statMoreNodeList;
							QList<QDomElement> statClassNodeList;
							QList<QDomElement> statAnswNodeList;
							QDomElement statNode = mCombinedDocument.createElement("answering-statistics");
							foreach(QString statName, statStrings) {
								cint64 index = CProcessingStatistics::getIndexFromProcessingStatisticDescriptionName(statName);
								if (index >= 0) {
									QDomElement statValueNode = mCombinedDocument.createElement("counted-process-statistic");
									statValueNode.setAttribute("statistic-name", statName);
									statValueNode.setAttribute("statistic-value", queryStats->getStatisticIntegerValue(statName));
									statNodeVec[index] = statValueNode;
								} else {
									if (statName.startsWith("answering")) {
										QDomElement statValueNode = mCombinedDocument.createElement("answering-statistic");
										statValueNode.setAttribute("statistic-name", statName);
										statValueNode.setAttribute("statistic-value", queryStats->getStatisticIntegerValue(statName));
										statAnswNodeList.append(statValueNode);
									} else if (statName.startsWith("class-classification")) {
										QDomElement statValueNode = mCombinedDocument.createElement("class-classification-statistic");
										statValueNode.setAttribute("statistic-name", statName);
										statValueNode.setAttribute("statistic-value", queryStats->getStatisticIntegerValue(statName));
										statClassNodeList.append(statValueNode);
									} else {
										QDomElement statValueNode = mCombinedDocument.createElement("calculation-statistic");
										statValueNode.setAttribute("statistic-name", statName);
										statValueNode.setAttribute("statistic-value", queryStats->getStatisticIntegerValue(statName));
										statMoreNodeList.append(statValueNode);
									}
								}
							}
							foreach(QDomElement statValueNode, statMoreNodeList) {
								statNode.appendChild(statValueNode);
							}
							foreach(QDomElement statValueNode, statClassNodeList) {
								statNode.appendChild(statValueNode);
							}
							foreach(QDomElement statValueNode, statAnswNodeList) {
								statNode.appendChild(statValueNode);
							}
							for (cint64 index = 0; index < statNodeVec.count(); ++index) {
								QDomElement statValueNode = statNodeVec[index];
								if (!statValueNode.isNull()) {
									statNode.appendChild(statValueNode);
								}
							}
							QList<QDomElement>& elements = mPosSerializedResultHash[pos];
							elements.append(statNode);
						}
					}
#endif
					return this;
				}

				QByteArray CSPARQLXMLAnswerSerializer::getCombinedResultSerialization() {

					QDomElement rootNode = mCombinedDocument.documentElement();

					for (QMap<cint64, QList<QDomElement> >::const_iterator it = mPosSerializedResultHash.constBegin(), itEnd = mPosSerializedResultHash.constEnd(); it != itEnd; ++it) {
						const QList<QDomElement>& elements = it.value();
						for (QDomElement element : elements) {
							rootNode.appendChild(element);
						}
					}

					mPosSerializedResultHash.clear();
					mCombinedDocumentInitialized = false;
					return mCombinedDocument.toString().toUtf8();
				}




				CSPARQLXMLAnswerSerializer* CSPARQLXMLAnswerSerializer::initializeCombinedDocument() {

					if (!mCombinedDocumentInitialized) {
						mCombinedDocumentInitialized = true;
						QDomDocument document;
						QDomProcessingInstruction procInstHeader = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
						document.appendChild(procInstHeader);
						QDomElement rootNode;
						rootNode = document.createElementNS(sparqlNS, "sparql");
						rootNode.setAttribute("xml:base", sparqlNS);
						rootNode.setAttribute("xmlns:owl", owlNS);
						rootNode.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema#");
						document.appendChild(rootNode);

						mCombinedDocument = document;
					}

					return this;
				}




				QList<QDomElement> CSPARQLXMLAnswerSerializer::getResultElements(CBooleanQueryResult* booleanResult, QDomDocument& document) {
					QList<QDomElement> elements;
					QDomElement headerElement = document.createElement("head");
					elements.append(headerElement);


					QDomElement booleanElement = document.createElement("boolean");
					if (booleanResult->getResult()) {
						booleanElement.appendChild(document.createTextNode("true"));
					} else {
						booleanElement.appendChild(document.createTextNode("false"));
					}
					elements.append(booleanElement);
					return elements;
				}

				QList<QDomElement> CSPARQLXMLAnswerSerializer::getResultElements(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult, QDomDocument& document) {
					QList<QDomElement> elements;
					QDomElement headerElement = document.createElement("head");
					for (const QString& var : varList) {
						QDomElement varEl = document.createElement("variable");
						varEl.setAttribute("name", var);
						headerElement.appendChild(varEl);
					}
					elements.append(headerElement);

					QDomElement resultsElement = document.createElement("results");

					if (variableBindingsAnswersResult) {
						CVariableBindingsResultIterator* varBindIt = nullptr;
						CVariableBindingsAnswersResultIterator* answersIt = variableBindingsAnswersResult->getVariableBindingsAnswersIterator();
						while (answersIt->hasNext()) {


							CVariableBindingsAnswerResult* varBindAnswer = answersIt->getNext();
							varBindIt = varBindAnswer->getVariableBindingsIterator(varBindIt);
							QDomElement resultEl = document.createElement("result");

							QStringList::const_iterator varListIt = varList.constBegin();

							while (varBindIt->hasNext()) {

								QString varString = *varListIt;
								++varListIt;

								CVariableBindingResult* varBin = varBindIt->getNext();
								if (varBin) {
									QDomElement bindingEl = document.createElement("binding");
									bindingEl.setAttribute("name", varString);
									if (varBin->isNamedIndividualBindingType()) {
										QDomElement uriEl = document.createElement("uri");
										uriEl.appendChild(document.createTextNode(varBin->getQueryResultString()));
										bindingEl.appendChild(uriEl);
									}
									else if (varBin->isAnonymousIndividualBindingType()) {
										QDomElement uriEl = document.createElement("bnode");
										uriEl.appendChild(document.createTextNode(varBin->getQueryResultString()));
										bindingEl.appendChild(uriEl);
									}
									else if (varBin->isLiteralBindingType()) {
										QDomElement uriEl = document.createElement("literal");
										uriEl.setAttribute("datatype", varBin->getLiteralDatatypeBindingString());
										uriEl.appendChild(document.createTextNode(varBin->getLiteralDatavalueBindingString()));
										bindingEl.appendChild(uriEl);
									}
									resultEl.appendChild(bindingEl);
								}
							}
							resultsElement.appendChild(resultEl);
						}
						delete answersIt;
						delete varBindIt;
					}
					elements.append(resultsElement);

					return elements;
				}



			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
