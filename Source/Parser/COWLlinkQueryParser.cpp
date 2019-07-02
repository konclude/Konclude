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
			} else  if (nodeString == "GetSameIndividuals") {
				expression = parseGetSameIndividualsNode(node);
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
			} else  if (nodeString == "GetObjectPropertyTargets") {
				expression = parseGetObjectPropertyTargetsNode(node);
			} else  if (nodeString == "GetFlattenedObjectPropertyTargets") {
				expression = parseGetFlattenedObjectPropertyTargetsNode(node);
			} else  if (nodeString == "GetObjectPropertySources") {
				expression = parseGetObjectPropertySourcesNode(node);
			} else  if (nodeString == "GetFlattenedObjectPropertySources") {
				expression = parseGetFlattenedObjectPropertySourcesNode(node);


			} else  if (nodeString == "GetSubObjectProperties") {
				expression = parseGetSubObjectPropertiesNode(node);
			} else  if (nodeString == "GetSuperObjectProperties") {
				expression = parseGetSuperObjectPropertiesNode(node);
			} else  if (nodeString == "GetEquivalentObjectProperties") {
				expression = parseGetEquivalentObjectPropertiesNode(node);
			} else  if (nodeString == "GetSubObjectPropertyHierarchy") {
				expression = parseGetSubObjectPropertyHierarchyNode(node);
			} else  if (nodeString == "IsObjectPropertySatisfiable") {
				expression = parseIsObjectPropertySatisfiableNode(node);
			} else  if (nodeString == "GetSubDataProperties") {
				expression = parseGetSubDataPropertiesNode(node);
			} else  if (nodeString == "GetSuperDataProperties") {
				expression = parseGetSuperDataPropertiesNode(node);
			} else  if (nodeString == "GetEquivalentDataProperties") {
				expression = parseGetEquivalentDataPropertiesNode(node);
			} else  if (nodeString == "GetSubDataPropertyHierarchy") {
				expression = parseGetSubDataPropertyHierarchyNode(node);
			} else  if (nodeString == "IsDataPropertySatisfiable") {
				expression = parseIsDataPropertySatisfiableNode(node);

			} else  if (nodeString == "GetObjectPropertiesOfSource") {
				expression = parseGetObjectPropertiesOfSourceNode(node);
			} else  if (nodeString == "GetObjectPropertiesOfTarget") {
				expression = parseGetObjectPropertiesOfTargetNode(node);
			} else  if (nodeString == "GetObjectPropertiesBetween") {
				expression = parseGetObjectPropertiesOfBetweenNode(node);


			} else  if (nodeString == "GetDeterministicIndividuals") {
				expression = parseGetDeterministicIndividualsNode(node);
			} else  if (nodeString == "GetNondeterministicIndividuals") {
				expression = parseGetNondeterministicIndividualsNode(node);
			} else  if (nodeString == "GetDeterministicClassAssertions" || nodeString == "GetKnownClassAssertions") {
				expression = parseGetDeterministicClassAssertionsNode(node);
			} else  if (nodeString == "GetNondeterministicClassAssertions" || nodeString == "GetPossibleClassAssertions") {
				expression = parseGetNondeterministicClassAssertionsNode(node);

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
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);

			CQueryIsClassSatisfiableExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsClassSatisfiableQuery(builds,idName);
			return queryExpression;
		}



		CQueryAreClassesEquivalentExpression *COWLlinkQueryParser::parseAreClassesEquivalentNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryAreClassesEquivalentExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getAreClassesEquivalenceQuery(builds,idName);
			return queryExpression;
		}


		CQueryIsInstanceOfExpression *COWLlinkQueryParser::parseIsInstanceOfNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryIsInstanceOfExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsInstanceOfQuery(builds,idName);
			return queryExpression;
		}


		CQueryIsClassSubsumedByExpression *COWLlinkQueryParser::parseIsClassSubsumedByNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryIsClassSubsumedByExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsClassSubsumedByQuery(builds,idName);
			return queryExpression;
		}


		CQueryAreClassesDisjointExpression *COWLlinkQueryParser::parseAreClassesDisjointNode(QDomElement *node) {
			// parse <IsClassSatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression *> builds = parseOntologyChildNodes(node);
			CQueryAreClassesDisjointExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getAreClassesDisjointQuery(builds,idName);
			return queryExpression;
		}



		CQueryGetTypesExpression *COWLlinkQueryParser::parseGetTypesNode(QDomElement *node) {
			// parse <GetTypes id="xxxx"/>
			QString idName = getQueryIDNameString(node);
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
			QString idName = getQueryIDNameString(node);
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
			QString idName = getQueryIDNameString(node);
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

		CQueryGetSameIndividualsExpression *COWLlinkQueryParser::parseGetSameIndividualsNode(QDomElement *node) {
			// parse <GetSameIndividuals id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSameIndividualsExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSameIndividualsQuery(builds,idName);
			return queryExpression;
		}

		CQueryGetInstancesExpression *COWLlinkQueryParser::parseGetInstancesNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = getQueryIDNameString(node);
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
			// parse <GetSubClasses id="xxxx"/>
			QString idName = getQueryIDNameString(node);
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
			// parse <GetSuperClasses id="xxxx"/>
			QString idName = getQueryIDNameString(node);
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
			// parse <GetEquivalentClasses id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetEquivalentClassesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetEquivalentClassesQuery(builds,idName);
			return queryExpression;
		}







		CQueryGetSubObjectPropertiesExpression *COWLlinkQueryParser::parseGetSubObjectPropertiesNode(QDomElement *node) {
			// parse <GetSubObjectProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSubObjectPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSubObjectPropertiesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetSuperObjectPropertiesExpression *COWLlinkQueryParser::parseGetSuperObjectPropertiesNode(QDomElement *node) {
			// parse <GetSuperObjectProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSuperObjectPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSuperObjectPropertiesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetEquivalentObjectPropertiesExpression* COWLlinkQueryParser::parseGetEquivalentObjectPropertiesNode(QDomElement *node) {
			// parse <GetEquivalentObjectProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetEquivalentObjectPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetEquivalentObjectPropertiesQuery(builds,idName);
			return queryExpression;
		}



		CQueryGetSubObjectPropertyHierarchyExpression* COWLlinkQueryParser::parseGetSubObjectPropertyHierarchyNode(QDomElement *node) {
			// parse <GetSubObjectPropertyHierarchy id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSubObjectPropertyHierarchyExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSubObjectPropertyHierarchyQuery(builds,idName);
			return queryExpression;
		}




		CQueryIsObjectPropertySatisfiableExpression* COWLlinkQueryParser::parseIsObjectPropertySatisfiableNode(QDomElement *node) {
			// parse <IsObjectPropertySatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryIsObjectPropertySatisfiableExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsObjectPropertySatisfiableQuery(builds,idName);
			return queryExpression;
		}





		CQueryGetSubDataPropertiesExpression *COWLlinkQueryParser::parseGetSubDataPropertiesNode(QDomElement *node) {
			// parse <GetSubDataProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSubDataPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSubDataPropertiesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetSuperDataPropertiesExpression *COWLlinkQueryParser::parseGetSuperDataPropertiesNode(QDomElement *node) {
			// parse <GetSuperDataProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QString directString = node->attribute("direct");
			bool direct = false;
			if (directString.toUpper() == "TRUE") {
				direct = true;
			}
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSuperDataPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSuperDataPropertiesQuery(builds,direct,idName);
			return queryExpression;
		}


		CQueryGetEquivalentDataPropertiesExpression* COWLlinkQueryParser::parseGetEquivalentDataPropertiesNode(QDomElement *node) {
			// parse <GetEquivalentDataProperties id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetEquivalentDataPropertiesExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetEquivalentDataPropertiesQuery(builds,idName);
			return queryExpression;
		}



		CQueryGetSubDataPropertyHierarchyExpression* COWLlinkQueryParser::parseGetSubDataPropertyHierarchyNode(QDomElement *node) {
			// parse <GetSubDataPropertyHierarchy id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetSubDataPropertyHierarchyExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetSubDataPropertyHierarchyQuery(builds,idName);
			return queryExpression;
		}




		CQueryIsDataPropertySatisfiableExpression* COWLlinkQueryParser::parseIsDataPropertySatisfiableNode(QDomElement *node) {
			// parse <IsDataPropertySatisfiable id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryIsDataPropertySatisfiableExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsDataPropertySatisfiableQuery(builds,idName);
			return queryExpression;
		}




		CQueryIsEntailedExpression* COWLlinkQueryParser::parseIsEntailedNode(QDomElement *node) {
			// parse <GetFlattenedInstances id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryIsEntailedExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getIsEntailedQuery(builds,idName);
			return queryExpression;
		}


		QString COWLlinkQueryParser::getQueryIDNameString(QDomElement* node) {
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
			return idName;
		}




		CQueryExpression *COWLlinkQueryParser::parseGetObjectPropertyTargetsNode(QDomElement *node) {
			// parse <GetObjectPropertyTargets id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			bool inversedProperty = false;
			QList<CBuildExpression*> builds = parseOntologyChildNodesInverseHandling(node,inversedProperty);
			CQueryExpression *queryExpression = 0;
			if (!inversedProperty) {
				queryExpression = mQueryBuilder->getGetObjectPropertyTargetsQuery(builds,idName);
			} else {
				queryExpression = mQueryBuilder->getGetObjectPropertySourcesQuery(builds,idName);
			}
			return queryExpression;
		}

		CQueryExpression *COWLlinkQueryParser::parseGetFlattenedObjectPropertyTargetsNode(QDomElement *node) {
			// parse <GetFlattenedObjectPropertyTargets id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			bool inversedProperty = false;
			QList<CBuildExpression*> builds = parseOntologyChildNodesInverseHandling(node,inversedProperty);
			CQueryExpression *queryExpression = 0;
			if (!inversedProperty) {
				queryExpression = mQueryBuilder->getGetFlattenedObjectPropertyTargetsQuery(builds,idName);
			} else {
				queryExpression = mQueryBuilder->getGetFlattenedObjectPropertySourcesQuery(builds,idName);
			}
			return queryExpression;
		}




		CQueryExpression *COWLlinkQueryParser::parseGetObjectPropertySourcesNode(QDomElement *node) {
			// parse <GetObjectPropertySources id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			bool inversedProperty = false;
			QList<CBuildExpression*> builds = parseOntologyChildNodesInverseHandling(node,inversedProperty);
			CQueryExpression *queryExpression = 0;
			if (!inversedProperty) {
				queryExpression = mQueryBuilder->getGetObjectPropertySourcesQuery(builds,idName);
			} else {
				queryExpression = mQueryBuilder->getGetObjectPropertyTargetsQuery(builds,idName);
			}
			return queryExpression;
		}

		CQueryExpression *COWLlinkQueryParser::parseGetFlattenedObjectPropertySourcesNode(QDomElement *node) {
			// parse <GetFlattenedObjectPropertySources id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			bool inversedProperty = false;
			QList<CBuildExpression*> builds = parseOntologyChildNodesInverseHandling(node,inversedProperty);
			CQueryExpression *queryExpression = 0;
			if (!inversedProperty) {
				queryExpression = mQueryBuilder->getGetFlattenedObjectPropertySourcesQuery(builds,idName);
			} else {
				queryExpression = mQueryBuilder->getGetFlattenedObjectPropertyTargetsQuery(builds,idName);
			}
			return queryExpression;
		}






		CQueryGetObjectPropertiesOfSourceExpression *COWLlinkQueryParser::parseGetObjectPropertiesOfSourceNode(QDomElement *node) {
			// parse <GetObjectPropertiesOfSource id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetObjectPropertiesOfSourceExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetObjectPropertiesOfSourceQuery(builds,idName);
			return queryExpression;
		}


		CQueryGetObjectPropertiesOfTargetExpression *COWLlinkQueryParser::parseGetObjectPropertiesOfTargetNode(QDomElement *node) {
			// parse <GetObjectPropertiesOfTarget id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetObjectPropertiesOfTargetExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetObjectPropertiesOfTargetQuery(builds,idName);
			return queryExpression;
		}



		CQueryGetObjectPropertiesBetweenExpression *COWLlinkQueryParser::parseGetObjectPropertiesOfBetweenNode(QDomElement *node) {
			// parse <GetObjectPropertiesBetween id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetObjectPropertiesBetweenExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetObjectPropertiesBetweenQuery(builds,idName);
			return queryExpression;
		}













		CQueryGetDeterministicIndividualsExpression *COWLlinkQueryParser::parseGetDeterministicIndividualsNode(QDomElement *node) {
			// parse <GetDeterministicIndividuals id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetDeterministicIndividualsExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetDeterministicIndividualsQuery(builds,idName);
			return queryExpression;
		}


		CQueryGetNondeterministicIndividualsExpression *COWLlinkQueryParser::parseGetNondeterministicIndividualsNode(QDomElement *node) {
			// parse <GetNondeterministicIndividuals id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetNondeterministicIndividualsExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetNondeterministicIndividualsQuery(builds,idName);
			return queryExpression;
		}

		CQueryGetDeterministicClassAssertionsExpression *COWLlinkQueryParser::parseGetDeterministicClassAssertionsNode(QDomElement *node) {
			// parse <GetDeterministicClassAssertions id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetDeterministicClassAssertionsExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetDeterministicClassAssertionsQuery(builds,idName);
			return queryExpression;
		}

		CQueryGetNondeterministicClassAssertionsExpression *COWLlinkQueryParser::parseGetNondeterministicClassAssertionsNode(QDomElement *node) {
			// parse <GetNondeterministicClassAssertions id="xxxx"/>
			QString idName = getQueryIDNameString(node);
			QList<CBuildExpression*> builds = parseOntologyChildNodes(node);
			CQueryGetNondeterministicClassAssertionsExpression *queryExpression = 0;
			queryExpression = mQueryBuilder->getGetNondeterministicClassAssertionsQuery(builds,idName);
			return queryExpression;
		}

	}; // end namespace Parser

}; // end namespace Konclude
