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

#include "CSPARQLQtXMLResultParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLQtXMLResultParser::CSPARQLQtXMLResultParser() {
		}


		CSPARQLQtXMLResultParser::~CSPARQLQtXMLResultParser() {
		}




		CQueryResult *CSPARQLQtXMLResultParser::parseQueryResult(QDomElement *resultNode) {
			QDomElement node = resultNode->firstChildElement();
			CQueryResult *queryResult = nullptr;
			while (!node.isNull() && !queryResult) {
				QString docElemName = node.tagName();
				if (docElemName == "results") {
					queryResult = parseVariableBindingsSet(&node);
				}

			}
			return queryResult;
		}




		CVariableBindingsAnswersSetResult* CSPARQLQtXMLResultParser::parseVariableBindingsSet(QDomElement *node, QHash<QString, cint64>* variableNameIndexHash, bool locallyRenameBlankNamesBindings) {
			return (CVariableBindingsAnswersSetResult*)parseVariableBindings(node, variableNameIndexHash, locallyRenameBlankNamesBindings, true);
		}

		CVariableBindingsAnswersListResult* CSPARQLQtXMLResultParser::parseVariableBindingsList(QDomElement *node, QHash<QString, cint64>* variableNameIndexHash, bool locallyRenameBlankNamesBindings) {
			return (CVariableBindingsAnswersListResult*)parseVariableBindings(node, variableNameIndexHash, locallyRenameBlankNamesBindings, false);
		}



		CVariableBindingsAnswersResult* CSPARQLQtXMLResultParser::parseVariableBindings(QDomElement *node, QHash<QString, cint64>* variableNameIndexHash, bool locallyRenameBlankNamesBindings, bool parseAsSet) {

			QStringList classNameList;
			bool err = node->isNull();

			QString tagName = node->tagName();

			CVariableBindingsAnswersResult* bindingAnswersResult = nullptr;

			QStringList variableNameList;
			cint64 nextVariableIndex = 0;

			QHash<QString, cint64>* deleteVariableNameIndexHash = nullptr;
			if (!variableNameIndexHash) {
				deleteVariableNameIndexHash = variableNameIndexHash = new QHash<QString, cint64>();
			}

			QDomElement resultNode = node->firstChildElement();
			while (!resultNode.isNull()) {
				if (resultNode.tagName() == "result") {

					CVariableBindingsListAnswerResult* bindingResult = new CVariableBindingsListAnswerResult();
					QVector<CVariableBindingResult*> bindingsVector(nextVariableIndex);


					QDomElement bindingNode = resultNode.firstChildElement();
					while (!bindingNode.isNull()) {
						if (bindingNode.tagName() == "binding") {

							QString variableName = bindingNode.attribute("name");
							if (!variableName.isEmpty()) {

								if (!variableNameIndexHash->contains(variableName)) {
									variableNameIndexHash->insert(variableName, nextVariableIndex++);
									variableNameList.append(variableName);
								}


								cint64 index = variableNameIndexHash->value(variableName);
								if (index >= bindingsVector.size()) {
									bindingsVector.resize(index + 1);
								}
													


								QDomElement boundNode = bindingNode.firstChildElement();
								while (!boundNode.isNull()) {
									if (boundNode.tagName() == "uri") {
										QString uri = boundNode.text();
										CVariableBindingResult* varBindingResult = new CVariableBindingStringResult(uri, CVariableBindingResult::VBTNAMEDINDIVIDUAL);
										bindingsVector[index] = varBindingResult;
									} else if (boundNode.tagName() == "literal") {
										QString lexicalValue = boundNode.text();
										QString datatypeIRI = boundNode.attribute("datatypeIRI", PREFIX_XML_PLAINLITERAL_DATATYPE);
										CVariableBindingResult* varBindingResult = new CVariableBindingStringResult("\"" + lexicalValue + "\"^^" + datatypeIRI, CVariableBindingResult::VBTLITERAL);
										bindingsVector[index] = varBindingResult;
									} else if (boundNode.tagName() == "bnode") {
										QString bname = boundNode.text();
										if (locallyRenameBlankNamesBindings) {
											bname = variableName + "_bound_blank_node";
										}
										CVariableBindingResult* varBindingResult = new CVariableBindingStringResult(bname, CVariableBindingResult::VBTANONYMOUSINDIVIDUAL);
										bindingsVector[index] = varBindingResult;
									}

									boundNode = boundNode.nextSiblingElement();
								}
							}
						}
						bindingNode = bindingNode.nextSiblingElement();
					}

					if (bindingsVector.count() > 0) {
						bindingResult->setResultVariableBindings(bindingsVector.toList());
						if (!bindingAnswersResult) {
							if (parseAsSet) {
								bindingAnswersResult = new CVariableBindingsAnswersSetResult(variableNameList);
							} else {
								bindingAnswersResult = new CVariableBindingsAnswersListResult(variableNameList);
							}
						}
						bindingAnswersResult->addResultVariableBindings(bindingResult);
					}

				}
				resultNode = resultNode.nextSiblingElement();
			}

			if (!bindingAnswersResult) {
				if (parseAsSet) {
					bindingAnswersResult = new CVariableBindingsAnswersSetResult(QStringList());
				} else {
					bindingAnswersResult = new CVariableBindingsAnswersListResult(QStringList());
				}
			}

			if (deleteVariableNameIndexHash) {
				delete deleteVariableNameIndexHash;
			}

			return bindingAnswersResult;
		}




	}; // end namespace Parser

}; // end namespace Konclude
