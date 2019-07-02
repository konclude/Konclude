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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CNODEBOTTOMUPTRAVERSALCLASSIFICATIONITEM_H
#define KONCLUDE_REASONER_CLASSIFIER_CNODEBOTTOMUPTRAVERSALCLASSIFICATIONITEM_H

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
			 *		\class		CNodeBottomUpTraversalClassificationItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNodeBottomUpTraversalClassificationItem : public CNodeBlockedClassificationItem {
				// public methods
				public:
					//! Constructor
					CNodeBottomUpTraversalClassificationItem(CHierarchyNode *node, QList<CHierarchyNode *> &branchNodes, QList<CNodeBottomUpTraversalClassificationItem *> *lockedReleaseProcessingList);

					//! Destructor
					virtual ~CNodeBottomUpTraversalClassificationItem();


					CNodeBlockedClassificationItem *releaseBlockedResumeProcessing();
					CNodeBlockedClassificationItem *releaseTestingBlockedResumeProcessing(CConcept *subsumerConcept, CConcept *subsumedConcept, bool testingResult);


					CHierarchyNode *getTraversalNode();

					QList<CHierarchyNode *> *getTestingNodeList();
					CNodeBottomUpTraversalClassificationItem *appendTestingNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeBottomUpTraversalClassificationItem *appendTestingNode(CHierarchyNode *node);

					QList<CHierarchyNode *> *getChildNodeList();
					CNodeBottomUpTraversalClassificationItem *appendChildNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeBottomUpTraversalClassificationItem *appendChildNode(CHierarchyNode *parentNode);

					QList<CHierarchyNode *> *getBranchLockedNodeList();
					CNodeBottomUpTraversalClassificationItem *appendBranchLockedNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeBottomUpTraversalClassificationItem *appendBranchLockedNode(CHierarchyNode *branchNode);

					bool hasNextTestingNode();
					CHierarchyNode *takeNextTestingNode();
					CHierarchyNode *getTestingNode();
					CNodeBottomUpTraversalClassificationItem *clearTestingNode();
					bool getTestingResult();

					bool hasDirectBranchLockedNode();
					CHierarchyNode *getDirectBranchLockedNode();
					CNodeBottomUpTraversalClassificationItem *setDirectBranchLockedNode(CHierarchyNode *parentLockedNode);

					QList<CHierarchyNode *> *getNewBranchNodeList();
					CNodeBottomUpTraversalClassificationItem *appendNewBranchNodeList(QList<CHierarchyNode *> &nodeList);
					CNodeBottomUpTraversalClassificationItem *appendNewBranchNode(CHierarchyNode *branchNode);

					bool hasAllSilbingBranchTested();
					CNodeBottomUpTraversalClassificationItem *setAllSilbingBranchTested(bool allSilbingsTested = true);
					qint64 getSilbingBranchTestNodeCount();
					CNodeBottomUpTraversalClassificationItem *decSilbingBranchTestNodeCount();

					bool isChildNodeAddedContinueBlocking();
					QList<CHierarchyNode*> getChildNodeAddedContinueBlockingList();
					CNodeBottomUpTraversalClassificationItem* setChildNodeAddedContinueBlocking(bool addedChildNode);
					CNodeBottomUpTraversalClassificationItem* addChildNodeAddedContinueBlocking(CHierarchyNode* continueBlocking);


				// protected methods
				protected:

				// protected variables
				protected:
					CHierarchyNode *traversalNode;
					CHierarchyNode *parLockNode;
					CHierarchyNode *testNode;

					QList<CHierarchyNode *> testingNodeList;
					QList<CHierarchyNode *> branchLockedNodeList;
					QList<CHierarchyNode *> newBranchNodeList;
					QSet<CHierarchyNode *> newBranchNodeSet;
					QList<CHierarchyNode *> childNodeList;

					QList<CHierarchyNode *> childNodeContinueBlockingList;


					QList<CNodeBottomUpTraversalClassificationItem *> *lockedReleaseProcList;


					bool subSumTestingResult;
					bool mChildNodeAdded;

					qint64 silbingBranchTestNodeCount;
					bool allSilbingBranchTested;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CNODEBOTTOMUPTRAVERSALCLASSIFICATIONITEM_H
