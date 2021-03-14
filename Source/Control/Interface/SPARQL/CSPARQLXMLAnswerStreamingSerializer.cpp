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

#include "CSPARQLXMLAnswerStreamingSerializer.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				CSPARQLXMLAnswerStreamingSerializer::CSPARQLXMLAnswerStreamingSerializer(QByteArray* buffer, bool writeHeaderAndFooter) {
					mWriteHeaderAndFooter = writeHeaderAndFooter;
					sparqlNS = "http://www.w3.org/2005/sparql-results#";
					owlNS = "http://www.w3.org/2002/07/owl#";
					xsdNS = "http://www.w3.org/2001/XMLSchema#";
					mSparqlHeaderWritten = false;
					mStreamWriter = new QXmlStreamWriter();
					mStreamWriter->setAutoFormatting(true);
					mStreamWriter->setAutoFormattingIndent(2);
					mCurrentBuffer = nullptr;
					mCurrentArray = nullptr;
					mVarBindIt = nullptr;
					mSparqlFooterWritten = false;
					if (buffer) {
						updateWriteBuffer(buffer);
					}
				}


				CSPARQLXMLAnswerStreamingSerializer::~CSPARQLXMLAnswerStreamingSerializer() {
					delete mStreamWriter;
					delete mVarBindIt;
					mVarBindIt = nullptr;
				}


				cint64 CSPARQLXMLAnswerStreamingSerializer::getBufferWrittenSize() {
					return mCurrentArray->size();
				}

				QByteArray* CSPARQLXMLAnswerStreamingSerializer::updateWriteBuffer(QByteArray* buffer) {
					QByteArray* tmpArray = mCurrentArray;
					QBuffer* tmpBuffer = mCurrentBuffer;
					mCurrentArray = buffer;
					mCurrentBuffer = nullptr;
					if (buffer) {
						mCurrentBuffer = new QBuffer(buffer);
						mCurrentBuffer->open(QIODevice::WriteOnly);
						mStreamWriter->setDevice(mCurrentBuffer);
					}
					delete tmpBuffer;
					return tmpArray;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultError(const QString& errorText, const QString &nodeString) {
					forceHeaderWritten();
					mStreamWriter->writeTextElement(nodeString, errorText);
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::forceHeaderWritten() {
					if (!mSparqlHeaderWritten && mWriteHeaderAndFooter) {
						mSparqlHeaderWritten = true;
						mStreamWriter->writeStartDocument();
						mStreamWriter->writeStartElement("sparql");
						mStreamWriter->writeAttribute("xmlns", sparqlNS);
						mStreamWriter->writeAttribute("xml:base", sparqlNS);
						mStreamWriter->writeAttribute("xmlns:owl", owlNS);
						mStreamWriter->writeAttribute("xmlns:xsd", xsdNS);
					}
					return this;
				}


				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::finish() {
					if (mSparqlHeaderWritten) {
						if (!mSparqlFooterWritten) {
							mSparqlFooterWritten = true;
							mStreamWriter->writeEndElement();
							mStreamWriter->writeEndDocument();
						}
					}
					return this;
				}




				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultSerialization(CBooleanQueryResult* booleanResult) {
					forceHeaderWritten();
					mStreamWriter->writeStartElement("head");
					mStreamWriter->writeEndElement();
					QString result;
					if (booleanResult->getResult()) {
						result = "true";
					} else {
						result = "false";
					}
					mStreamWriter->writeTextElement("boolean", result);
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultStreamingStart(const QStringList& varList) {
					mStreamWriter->writeStartElement("head");
					for (const QString& var : varList) {
						mStreamWriter->writeStartElement("variable");
						mStreamWriter->writeAttribute("name", var);
						mStreamWriter->writeEndElement();
					}
					mStreamWriter->writeEndElement();

					writeResultStreamingStart();
					return this;
				}


				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::writeResultStreamingStart() {
					mStreamWriter->writeStartElement("results");
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultStreamingBinding(const QStringList& varList, CVariableBindingsAnswerResult* bindings) {
					mStreamWriter->writeStartElement("result");

					mVarBindIt = bindings->getVariableBindingsIterator(mVarBindIt);
					QStringList::const_iterator varListIt = varList.constBegin();

					while (mVarBindIt->hasNext()) {
						QString varString = *varListIt;
						++varListIt;

						CVariableBindingResult* varBin = mVarBindIt->getNext();
						if (varBin) {
							mStreamWriter->writeStartElement("binding");
							mStreamWriter->writeAttribute("name", varString);
							if (varBin->isNamedIndividualBindingType() || varBin->isClassBindingType() || varBin->isPropertyBindingType()) {
								mStreamWriter->writeTextElement("uri", varBin->getQueryResultString());
							} else if (varBin->isAnonymousIndividualBindingType()) {
								mStreamWriter->writeTextElement("bnode", varBin->getQueryResultString());
							} else if (varBin->isLiteralBindingType()) {
								mStreamWriter->writeStartElement("literal");
								mStreamWriter->writeAttribute("datatype", varBin->getLiteralDatatypeBindingString());
								mStreamWriter->writeCharacters(varBin->getLiteralDatavalueBindingString());
								mStreamWriter->writeEndElement();
							}
							mStreamWriter->writeEndElement();
						}
					}
					mStreamWriter->writeEndElement();
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::writeResultStreamingEnd() {
					mStreamWriter->writeEndElement();
					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultSerialization(const QStringList& varList, CVariableBindingsAnswersResult* variableBindingsAnswersResult) {
					forceHeaderWritten();
					if (variableBindingsAnswersResult && dynamic_cast<CVariableBindingsAnswersStreamingResult*>(variableBindingsAnswersResult)) {
						writeResultStreamingEnd();
						delete mVarBindIt;
						mVarBindIt = nullptr;
					} else {
						addResultStreamingStart(varList);
						if (variableBindingsAnswersResult) {
							CVariableBindingsAnswersResultIterator* answersIt = variableBindingsAnswersResult->getVariableBindingsAnswersIterator();
							while (answersIt->hasNext()) {
								CVariableBindingsAnswerResult* varBindAnswer = answersIt->getNext();
								addResultStreamingBinding(varList, varBindAnswer);
							}
							delete answersIt;
							delete mVarBindIt;
							mVarBindIt = nullptr;

						}
						writeResultStreamingEnd();
					}

					return this;
				}



				CSPARQLXMLAnswerStreamingSerializer* CSPARQLXMLAnswerStreamingSerializer::addResultStatistics(CQueryStatistics* queryStats) {
#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
					forceHeaderWritten();
					if (queryStats) {
						QList<QString> statStrings(queryStats->getStatisticsNameStringList());
						if (statStrings.count() >= 1) {
							typedef QPair<QString, QString> TStatNameValuePair;
							QVector<TStatNameValuePair> statNodeVec(statStrings.count() + 1);
							QList<TStatNameValuePair> statMoreNodeList;
							QList<TStatNameValuePair> statClassNodeList;
							QList<TStatNameValuePair> statAnswNodeList;
							foreach(QString statName, statStrings) {
								cint64 index = CProcessingStatistics::getIndexFromProcessingStatisticDescriptionName(statName);
								TStatNameValuePair statNameValuePair(statName, QString::number(queryStats->getStatisticIntegerValue(statName)));
								if (index >= 0) {
									statNodeVec[index] = statNameValuePair;
								} else {
									if (statName.startsWith("answering")) {
										statAnswNodeList.append(statNameValuePair);
									} else if (statName.startsWith("class-classification")) {
										statClassNodeList.append(statNameValuePair);
									} else {
										statMoreNodeList.append(statNameValuePair);
									}
								}
							}


							mStreamWriter->writeStartElement("answering-statistics");
							foreach(TStatNameValuePair statValueNode, statMoreNodeList) {
								mStreamWriter->writeStartElement("calculation-statistic");
								mStreamWriter->writeAttribute("statistic-name", statValueNode.first);
								mStreamWriter->writeAttribute("statistic-value", statValueNode.second);
								mStreamWriter->writeEndElement();
							}
							foreach(TStatNameValuePair statValueNode, statClassNodeList) {
								mStreamWriter->writeStartElement("class-classification-statistic");
								mStreamWriter->writeAttribute("statistic-name", statValueNode.first);
								mStreamWriter->writeAttribute("statistic-value", statValueNode.second);
								mStreamWriter->writeEndElement();
							}
							foreach(TStatNameValuePair statValueNode, statAnswNodeList) {
								mStreamWriter->writeStartElement("answering-statistic");
								mStreamWriter->writeAttribute("statistic-name", statValueNode.first);
								mStreamWriter->writeAttribute("statistic-value", statValueNode.second);
								mStreamWriter->writeEndElement();
							}
							for (cint64 index = 0; index < statNodeVec.count(); ++index) {
								TStatNameValuePair statValueNode = statNodeVec[index];
								if (!statValueNode.first.isEmpty() && statValueNode.second.isEmpty()) {
									mStreamWriter->writeStartElement("counted-process-statistic");
									mStreamWriter->writeAttribute("statistic-name", statValueNode.first);
									mStreamWriter->writeAttribute("statistic-value", statValueNode.second);
									mStreamWriter->writeEndElement();
								}
							}
							mStreamWriter->writeEndElement();
						}
					}
#endif
					return this;
				}


			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
