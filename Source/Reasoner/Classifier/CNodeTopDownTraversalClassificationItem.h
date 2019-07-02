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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CNODETOPDOWNTRAVERSALCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CNODETOPDOWNTRAVERSALCLASSIFICATIONITEM_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassifierStatistics.h"
#include "CNodeBlockedClassificationItem.h"

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
			 *		\class		CNodeTopDownTraversalClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNodeTopDownTraversalClassificationItem : public CNodeBlockedClassificationItem {
				// public methods
				public:
					//! Constructor
					CNodeTopDownTraversalClassificationItem(CHierarchyNode *node, CHierarchyNode *topNode, QList<CNodeTopDownTraversalClassificationItem *> *lockedReleaseProcessingList);

					//! Destructor
					virtual ~CNodeTopDownTraversalClassificationItem();


					CNodeBlockedClassificationItem *releaseBlockedResumeProcessing();
					CNodeBlockedClassificationItem *releaseTestingBlockedResumeProcessing(CConcept *subsumerConcept, CConcept *subsumedConcept, bool testingResult);


					CHierarchyNode *getTraversalNode();

					QList<CHierarchyNode *> *getTestingNodeList();
					CNodeTopDownTraversalClassificationItem *appendTestingNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeTopDownTraversalClassificationItem *appendTestingNode(CHierarchyNode *node);

					QList<CHierarchyNode *> *getParentNodeList();
					CNodeTopDownTraversalClassificationItem *appendParentNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeTopDownTraversalClassificationItem *appendParentNode(CHierarchyNode *parentNode);

					QList<CHierarchyNode *> *getBranchLockedNodeList();
					CNodeTopDownTraversalClassificationItem *appendBranchLockedNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeTopDownTraversalClassificationItem *appendBranchLockedNode(CHierarchyNode *branchNode);

					bool hasNextTestingNode();
					CHierarchyNode *takeNextTestingNode();
					CHierarchyNode *getTestingNode();
					CNodeTopDownTraversalClassificationItem *clearTestingNode();
					bool getTestingResult();
					CHierarchyNode *getEquivalentTestingNode();
					CNodeTopDownTraversalClassificationItem *setEquivalentTestingNode(CHierarchyNode *equivalentTestingNode);

					bool hasDirectBranchLockedNode();
					CHierarchyNode *getDirectBranchLockedNode();
					CNodeTopDownTraversalClassificationItem *setDirectBranchLockedNode(CHierarchyNode *parentLockedNode);

					QList<CHierarchyNode *> *getNewBranchNodeList();
					CNodeTopDownTraversalClassificationItem *appendNewBranchNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeTopDownTraversalClassificationItem *appendNewBranchNode(CHierarchyNode *branchNode);

					CNodeTopDownTraversalClassificationItem* setAlreadyTraversedDirectBlockedSuccessorNode(bool alredyTraversed);
					bool hasAlreadyTraversedDirectBlockedSuccessorNode();

				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode *traversalNode;
					CHierarchyNode *parLockNode;
					CHierarchyNode *testNode;
					CHierarchyNode *eqTestNode;

					QList<CHierarchyNode *> testingNodeList;
					QList<CHierarchyNode *> branchLockedNodeList;
					QList<CHierarchyNode *> newBranchNodeList;
					QList<CHierarchyNode *> parentNodeList;

					QSet<CHierarchyNode *> branchLockedNodeSet;



					QList<CNodeTopDownTraversalClassificationItem *> *lockedReleaseProcList;


					bool subSumTestingResult;
					bool alredyTraversedDirectBlockedSuccessorNode;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CNODETOPDOWNTRAVERSALCLASSIFICATIONITEM_H
