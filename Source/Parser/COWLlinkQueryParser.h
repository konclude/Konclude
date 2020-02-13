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

#ifndef KONCLUDE_PARSER_COWLlinkQueryParser_H
#define KONCLUDE_PARSER_COWLlinkQueryParser_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "CQueryParser.h"
#include "COntologyParser.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {




		/*! 
		 *
		 *		\class		COWLlinkQueryParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class COWLlinkQueryParser : public CQueryParser {
			// public methods
			public:
				//! Constructor
				COWLlinkQueryParser(CQueryBuilder *queryBuilder);

				//! Destructor
				virtual ~COWLlinkQueryParser();

				virtual bool parseQueryFile(const QString& filename);
				virtual CQueryExpression *parseQueryNode(QDomElement *queryNode);

			// protected methods
			protected:
				void parseRequestMessageNode(QDomElement *queryNode);

				QString getQueryIDNameString(QDomElement* node);

				CQueryIsClassSatisfiableExpression *parseIsClassSatisfiableNode(QDomElement *node);
				CQueryAreClassesEquivalentExpression *parseAreClassesEquivalentNode(QDomElement *node);
				CQueryIsInstanceOfExpression *parseIsInstanceOfNode(QDomElement *node);
				CQueryIsClassSubsumedByExpression *parseIsClassSubsumedByNode(QDomElement *node);
				CQueryAreClassesDisjointExpression *parseAreClassesDisjointNode(QDomElement *node);
				CQueryGetTypesExpression *parseGetTypesNode(QDomElement *node);
				CQueryGetFlattenedTypesExpression *parseGetFlattenedTypesNode(QDomElement *node);
				CQueryGetFlattenedInstancesExpression *parseGetFlattenedInstancesNode(QDomElement *node);
				CQueryGetSameIndividualsExpression* parseGetSameIndividualsNode(QDomElement *node);
				CQueryGetInstancesExpression *parseGetInstancesNode(QDomElement *node);
				CQueryGetSubClassesExpression *parseGetSubClassesNode(QDomElement *node);
				CQueryGetSuperClassesExpression *parseGetSuperClassesNode(QDomElement *node);
				CQueryGetEquivalentClassesExpression *parseGetEquivalentClassesNode(QDomElement *node);
				CQueryIsEntailedExpression* parseIsEntailedNode(QDomElement *node);
				CQueryExpression* parseGetObjectPropertyTargetsNode(QDomElement *node);
				CQueryExpression* parseGetFlattenedObjectPropertyTargetsNode(QDomElement *node);
				CQueryExpression* parseGetObjectPropertySourcesNode(QDomElement *node);
				CQueryExpression* parseGetFlattenedObjectPropertySourcesNode(QDomElement *node);


				CQueryGetObjectPropertiesOfSourceExpression* parseGetObjectPropertiesOfSourceNode(QDomElement *node);
				CQueryGetObjectPropertiesOfTargetExpression* parseGetObjectPropertiesOfTargetNode(QDomElement *node);
				CQueryGetObjectPropertiesBetweenExpression* parseGetObjectPropertiesOfBetweenNode(QDomElement *node);



				CQueryGetSubObjectPropertiesExpression *parseGetSubObjectPropertiesNode(QDomElement *node);
				CQueryGetSuperObjectPropertiesExpression *parseGetSuperObjectPropertiesNode(QDomElement *node);
				CQueryGetEquivalentObjectPropertiesExpression *parseGetEquivalentObjectPropertiesNode(QDomElement *node);
				CQueryGetSubObjectPropertyHierarchyExpression *parseGetSubObjectPropertyHierarchyNode(QDomElement *node);
				CQueryIsObjectPropertySatisfiableExpression *parseIsObjectPropertySatisfiableNode(QDomElement *node);
				CQueryGetSubDataPropertiesExpression *parseGetSubDataPropertiesNode(QDomElement *node);
				CQueryGetSuperDataPropertiesExpression *parseGetSuperDataPropertiesNode(QDomElement *node);
				CQueryGetEquivalentDataPropertiesExpression *parseGetEquivalentDataPropertiesNode(QDomElement *node);
				CQueryGetSubDataPropertyHierarchyExpression *parseGetSubDataPropertyHierarchyNode(QDomElement *node);
				CQueryIsDataPropertySatisfiableExpression *parseIsDataPropertySatisfiableNode(QDomElement *node);


				CQueryGetDeterministicIndividualsExpression* parseGetDeterministicIndividualsNode(QDomElement *node);
				CQueryGetNondeterministicIndividualsExpression* parseGetNondeterministicIndividualsNode(QDomElement *node);
				CQueryGetDeterministicClassAssertionsExpression* parseGetDeterministicClassAssertionsNode(QDomElement *node);
				CQueryGetNondeterministicClassAssertionsExpression* parseGetNondeterministicClassAssertionsNode(QDomElement *node);

				virtual QList<CBuildExpression *> parseOntologyChildNodes(QDomElement *node) = 0;

				virtual QList<CBuildExpression*> parseOntologyChildNodesInverseHandling(QDomElement *node, bool& inversed) = 0;


			// protected variables
			protected:
				CQueryBuilder *mQueryBuilder;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWLlinkQueryParser_H
