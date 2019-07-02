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

#include "COWLlinkQueryParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQueryParser::COWLlinkQueryParser(CQueryBuilder *queryBuilder) {
			mQueryBuilder = queryBuilder;

		}


		COWLlinkQueryParser::~COWLlinkQueryParser() {
		}



		bool COWLlinkQueryParser::parseQueryFile(QString filename) {
			bool successfullParsed = true;

			// parse file
			QFile file(filename);
			if (file.open(QIODevice::ReadOnly)) {
				QDomDocument document;
				QString errorMessage;
				int errorLine = 0, errorColumn = 0;
				if (document.setContent(file.readAll(),true,&errorMessage,&errorLine,&errorColumn)) {

					QDomElement documentElement(document.documentElement());
					QString docElemName = documentElement.tagName();

					if (docElemName == "RequestMessage") {
						LOG(INFO,"::Konclude::Parser::OWLlinkQueryParser",logTr("Parsing RequestMessage Node."),this);
						parseRequestMessageNode(&documentElement);
					} else {
						successfullParsed = false;
						LOG(ERROR,"::Konclude::Parser::OWLlinkQueryParser",logTr("Found '%1', but no known Node.").arg(docElemName),this);
					}

					LOG(INFO,"::Konclude::Parser::OWLlinkQueryParser",logTr("Parsing of '%1' completed.").arg(filename),this);
				} else {
					LOG(ERROR,"::Konclude::Parser::OWLlinkQueryParser",logTr("XML error: %1, file: %2, line: %3, column %4.").arg(errorMessage).arg(filename).arg(errorLine).arg(errorColumn),this);
				}
				file.close();

			} else {
				successfullParsed = false;
				LOG(ERROR,"::Konclude::Parser::OWLlinkQueryParser",logTr("File '%1' not found or couldn't be opened.").arg(filename),this);
			}

			return successfullParsed;
		}



		void COWLlinkQueryParser::parseRequestMessageNode(QDomElement *queryNode) {
			QDomElement node = queryNode->firstChildElement();
			while (!node.isNull()) {
				// build for each node an expression
				parseQueryNode(&node);

				node = node.nextSiblingElement();
			}
		}



		CQueryExpression *COWLlinkQueryParser::parseQueryNode(QDomElement *node) {
			CQueryExpression *expression = 0;

			QString nodeString(node->tagName());

			if (nodeString == "IsClassSatisfiable") {
				expression = parseIsClassSatisfiableNode(node);
			} else if (nodeString == "AreClassesEquivalent") {
				expression = parseAreClassesEquivalentNode(node);
			} else if (nodeString == "IsInstanceOf") {
				expression = parseIsInstanceOfNode(node);
			} else if (nodeString == "IsClassSubsumedBy") {
				expression = parseIsClassSubsumedByNode(node);
			} else if (nodeString == "AreClassesDisjoint") {
				expression = parseAreClassesDisjointNode(node);
			} else  if (nodeString == "GetFlattenedTypes") {
				expression = parseGetFlattenedTypesNode(node);
			} else  if (nodeString == "GetTypes") {
				expression = parseGetTypesNode(node);
			} else  if (nodeString == "GetFlattenedInstances") {
				expression = parseGetFlattenedInstancesNode(node);
			} else  if (nodeString == "GetInstances") {
				expression = parseGetInstancesNode(node);
			} else  if (nodeString == "GetSubClasses") {
				expression = parseGetSubClassesNode(node);
			} else  if (nodeString == "GetSuperClasses") {
				expression = parseGetSuperClassesNode(node);
			} else  if (nodeString == "GetEquivalentClasses") {
				expression = parseGetEquivalentClassesNode(node);
			} else  if (nodeString == "IsEntailed") {
				expression = parseIsEntailedNode(node);
			} else if (nodeString == "#text") {
				// ignoring text nodes
			} else {
				QString notSupportedNodeString(nodeString);
				LOG(WARNING,"::Konclude::Parser::OWLlinkQueryParser",logTr("Not supported XML node: '%1'. Continue with next node.").arg(notSupportedNodeString),this);
			}
			return expression;
		}



		CQueryIsClassSatisfiableExpression *COWLlinkQueryParser::parseIsClassSatisfiableNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}

			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);

			CQueryIsClassSatisfiableExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsClassSatisfiableQuery(builds,idName);
			return queryExpression;
		}



		CQueryAreClassesEquivalentExpression *COWLlinkQueryParser::parseAreClassesEquivalentNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryAreClassesEquivalentExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getAreClassesEquivalenceQuery(builds,idName);
			return queryExpression;
		}


		CQueryIsInstanceOfExpression *COWLlinkQueryParser::parseIsInstanceOfNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryIsInstanceOfExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsInstanceOfQuery(builds,idName);
			return queryExpression;
		}


		CQueryIsClassSubsumedByExpression *COWLlinkQueryParser::parseIsClassSubsumedByNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryIsClassSubsumedByExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsClassSubsumedByQuery(builds,idName);
			return queryExpression;
		}


		CQueryAreClassesDisjointExpression *COWLlinkQueryParser::parseAreClassesDisjointNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryAreClassesDisjointExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getAreClassesDisjointQuery(builds,idName);
			return queryExpression;
		}



		CQueryGetTypesExpression *COWLlinkQueryParser::parseGetTypesNode(QDomElement *node) {
			// parse <GetTypes id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetTypesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetTypesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetFlattenedTypesExpression *COWLlinkQueryParser::parseGetFlattenedTypesNode(QDomElement *node) {
			// parse <GetFlattenedTypes id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetFlattenedTypesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetFlattenedTypesQuery(builds,direct,idName);
			return queryExpression;
		}



		CQueryGetFlattenedInstancesExpression *COWLlinkQueryParser::parseGetFlattenedInstancesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetFlattenedInstancesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetFlattenedInstancesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetInstancesExpression *COWLlinkQueryParser::parseGetInstancesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetInstancesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetInstancesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetSubClassesExpression *COWLlinkQueryParser::parseGetSubClassesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSubClassesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSubClassesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetSuperClassesExpression *COWLlinkQueryParser::parseGetSuperClassesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSuperClassesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSuperClassesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetEquivalentClassesExpression* COWLlinkQueryParser::parseGetEquivalentClassesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetEquivalentClassesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetEquivalentClassesQuery(builds,idName);
			return queryExpression;
		}



		CQueryIsEntailedExpression* COWLlinkQueryParser::parseIsEntailedNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = node->attribute("id");
			if (idName.isEmpty()) {
				idName = node->attribute("name");
			}
			if (idName.isEmpty()) {
				idName = node->attribute("kb");
				if (!idName.isEmpty()) {
					idName = idName + " KB-Query";
				}
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryIsEntailedExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsEntailedQuery(builds,idName);
			return queryExpression;
		}

	}; // end namespace Parser

}; // end namespace Konclude
