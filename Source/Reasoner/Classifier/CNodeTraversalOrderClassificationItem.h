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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CNODETRAVERSALORDERCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CNODETRAVERSALORDERCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"

// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Taxonomy/CTaxonomy.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;
		using namespace Taxonomy;

		namespace Classifier {


			/*! 
			 *
			 *		\class		CNodeTraversalOrderClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNodeTraversalOrderClassificationItem {
				// public methods
				public:
					//! Constructor
					CNodeTraversalOrderClassificationItem(CHierarchyNode *node, CHierarchyNode *parentNode);

					//! Destructor
					virtual ~CNodeTraversalOrderClassificationItem();

					virtual CHierarchyNode *getTraversalNode();
					virtual CHierarchyNode *getParentTestingNode();
					virtual CHierarchyNode *getParentAgainstTestingNode();
					virtual CNodeTraversalOrderClassificationItem *setParentTestingNode(CHierarchyNode *node);
					virtual CNodeTraversalOrderClassificationItem *setParentAgainstTestingNode(CHierarchyNode *node);
					virtual bool isParentAgainstTesting();

					virtual QList<CHierarchyNode *> *getOrderTraversalList();
					virtual CNodeTraversalOrderClassificationItem *appendOrderTraversalList(QList<CHierarchyNode *> &nodeList);

					virtual QList<CHierarchyNode *> *getAgainstOrderTraversalList();
					virtual CNodeTraversalOrderClassificationItem *appendAgainstOrderTraversalList(QList<CHierarchyNode *> &nodeList);

				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode *traversalNode;
					CHierarchyNode *parentTestNode;
					CHierarchyNode *parentAgainstTestNode;
					bool againstTested;

					QList<CHierarchyNode *> orderTraversalList;
					QList<CHierarchyNode *> againstOrderTraversalList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CNODETRAVERSALORDERCLASSIFICATIONITEM_H
