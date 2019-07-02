/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "COWLlinkQtXMLResultParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQtXMLResultParser::COWLlinkQtXMLResultParser() {
		}


		COWLlinkQtXMLResultParser::~COWLlinkQtXMLResultParser() {
		}




		CQueryResult *COWLlinkQtXMLResultParser::parseQueryResult(QDomElement *resultNode) {
			QDomElement node = resultNode->firstChildElement();
			CQueryResult *queryResult = 0;
			while (!node.isNull() && !queryResult) {
				QString docElemName = node.tagName();
				if (docElemName == "ox:BooleanResponse" || docElemName == "BooleanResponse") {
					queryResult = parseBooleanQueryResult(&node);			
				} else if (docElemName == "ox:ClassHierarchy" || docElemName == "ClassHierarchy") {
					queryResult = parseClassHierarchyResult(&node);			
				}

			}
			return queryResult;
		}

		CBooleanQueryResult *COWLlinkQtXMLResultParser::parseBooleanQueryResult(QDomElement *node) {
			CBooleanQueryResult *booleanQueryResult = new CBooleanQueryResult(false);
			if (node->hasAttribute("result")) {
				QString resultValueString = node->attribute("result");
				if (CStringPrimitiveConverter::convertStringToBoolean(resultValueString,true)) {
					booleanQueryResult->setResult(true);
				}
			}
			return booleanQueryResult;
		}



		bool COWLlinkQtXMLResultParser::hasTopClass(CClassSynsetResult *classSynset) {
			return classSynset->hasClassName("http://www.w3.org/2002/07/owl#Thing");
		}


		CClassHierarchyResult *COWLlinkQtXMLResultParser::parseClassHierarchyResult(QDomElement *node) {
			CClassHierarchyResult *classHierResult = 0;
			QDomElement bottomClassNode = node->firstChildElement("ClassSynset");
			CClassSynsetResult *bottomClassSynset = nullptr;
			if (!bottomClassNode.isNull()) {
				bottomClassSynset = parseClassSynsetResult(&bottomClassNode);
			}
			if (!bottomClassSynset) {
				bottomClassSynset = new CClassSynsetResult(QStringList()<<QString("http://www.w3.org/2002/07/owl#Nothing"));
			}
			CClassSynsetResult *topClassSynset = 0;

			classHierResult = new CClassHierarchyResult(bottomClassSynset);

			QDomElement subClassNode = node->firstChildElement("ClassSubClassesPair");

			bool foundTop = false;
			while (!subClassNode.isNull()) {
				CClassSynsetResult *superClass = 0;

				QDomElement classSynsetElm(subClassNode.firstChildElement("ClassSynset"));
				QStringList classNames = parseClassNames(&classSynsetElm);
				if (!classNames.isEmpty()) {
					superClass = classHierResult->getClassSynset(classNames);

					if (!foundTop && hasTopClass(superClass)) {
						classHierResult->setTopClassSynset(superClass);
						foundTop = true;
					}

					QDomElement subClassSynsetNode = subClassNode.firstChildElement("SubClassSynsets");
					while (!subClassSynsetNode.isNull()) {
						QDomElement classSynsetNode = subClassSynsetNode.firstChildElement("ClassSynset");
						while (!classSynsetNode.isNull()) {
							QStringList classNames = parseClassNames(&classSynsetNode);
							if (!classNames.isEmpty()) {
								CClassSynsetResult *subClass = classHierResult->getClassSynset(classNames);
								if (subClass) {
									classHierResult->addSubClassRelation(superClass,subClass);
								}
							}
							classSynsetNode = classSynsetNode.nextSiblingElement("ClassSynset");
						}
						subClassSynsetNode = subClassSynsetNode.nextSiblingElement("SubClassSynsets");
					}
				}

				subClassNode = subClassNode.nextSiblingElement("ClassSubClassesPair");
			}
			return classHierResult;
		}


		QStringList COWLlinkQtXMLResultParser::parseClassNames(QDomElement *node) {
			QStringList classNameList;
			bool err = node->isNull();

			QString tagName = node->tagName();

			QDomElement classNode = node->firstChildElement();
			while (!classNode.isNull()) {
				if (classNode.tagName() == "Class" || classNode.tagName() == "owl:Class") {
					QString className = classNode.attribute("IRI");
					if (className.isEmpty()) {
						className = classNode.attribute("abbreviatedIRI");
						className = className.replace("owl:","http://www.w3.org/2002/07/owl#");
						className = className.replace("xsd:","http://www.w3.org/2001/XMLSchema#");
						className = className.replace("rdf:","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
						className = className.replace("rdfs:","http://www.w3.org/2000/01/rdf-schema#");
					}
					classNameList.append(className);
				}
				classNode = classNode.nextSiblingElement();
			}
			return classNameList;
		}

		CClassSynsetResult *COWLlinkQtXMLResultParser::parseClassSynsetResult(QDomElement *node) {
			QStringList classNameList = parseClassNames(node);
			CClassSynsetResult *classSynset = nullptr;
			if (!classNameList.isEmpty()) {
				classSynset = new CClassSynsetResult(classNameList);
			}
			return classSynset;
		}



	}; // end namespace Parser

}; // end namespace Konclude
