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

#include "CNodeBottomUpTraversalClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CNodeBottomUpTraversalClassificationItem::CNodeBottomUpTraversalClassificationItem(CHierarchyNode *node, QList<CHierarchyNode *> &branchNodes, QList<CNodeBottomUpTraversalClassificationItem *> *lockedReleaseProcessingList) {
				traversalNode = node;
				lockedReleaseProcList = lockedReleaseProcessingList;
				testNode = 0;
				parLockNode = 0;
				subSumTestingResult = false;
				allSilbingBranchTested = false;
				branchLockedNodeList << branchNodes;
				foreach (CHierarchyNode* node, branchNodes) {
					newBranchNodeSet.insert(node);
				}

				silbingBranchTestNodeCount = branchLockedNodeList.count();
				mChildNodeAdded = false;
			}


			CNodeBottomUpTraversalClassificationItem::~CNodeBottomUpTraversalClassificationItem() {
			}



			CNodeBlockedClassificationItem *CNodeBottomUpTraversalClassificationItem::releaseBlockedResumeProcessing() {
				lockedReleaseProcList->append(this);
				return this;
			}

			CNodeBlockedClassificationItem *CNodeBottomUpTraversalClassificationItem::releaseTestingBlockedResumeProcessing(CConcept *subsumerConcept, CConcept *subsumedConcept, bool testingResult) {
				subSumTestingResult = testingResult;
				lockedReleaseProcList->append(this);
				return this;
			}



			CHierarchyNode *CNodeBottomUpTraversalClassificationItem::getTraversalNode() {
				return traversalNode;
			}

			QList<CHierarchyNode *> *CNodeBottomUpTraversalClassificationItem::getTestingNodeList() {
				return &testingNodeList;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendTestingNodeList(QList<CHierarchyNode *> &nodeList) {
				testingNodeList << nodeList;
				return this;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendTestingNode(CHierarchyNode *node) {
				testingNodeList.append(node);
				return this;
			}


			QList<CHierarchyNode *> *CNodeBottomUpTraversalClassificationItem::getChildNodeList() {
				return &childNodeList;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendChildNodeList(QList<CHierarchyNode *> &nodeList) {
				childNodeList << nodeList;
				return this;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendChildNode(CHierarchyNode *node) {
				childNodeList.append(node);
				return this;
			}

			bool CNodeBottomUpTraversalClassificationItem::hasNextTestingNode() {
				return !testingNodeList.isEmpty();
			}

			CHierarchyNode *CNodeBottomUpTraversalClassificationItem::takeNextTestingNode() {
				testNode = testingNodeList.takeFirst();
				return testNode;
			}

			CHierarchyNode *CNodeBottomUpTraversalClassificationItem::getTestingNode() {
				return testNode;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::clearTestingNode() {
				testNode = 0;
				return this;
			}

			bool CNodeBottomUpTraversalClassificationItem::getTestingResult() {
				return subSumTestingResult;
			}


			QList<CHierarchyNode *> *CNodeBottomUpTraversalClassificationItem::getBranchLockedNodeList() {
				return &branchLockedNodeList;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendBranchLockedNodeList(QList<CHierarchyNode *> &nodeList) {
				branchLockedNodeList << nodeList;
				return this;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendBranchLockedNode(CHierarchyNode *branchNode) {
				branchLockedNodeList.append(branchNode);
				return this;
			}

			bool CNodeBottomUpTraversalClassificationItem::hasDirectBranchLockedNode() {
				return parLockNode != 0;
			}

			CHierarchyNode *CNodeBottomUpTraversalClassificationItem::getDirectBranchLockedNode() {
				return parLockNode;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::setDirectBranchLockedNode(CHierarchyNode *parentLockedNode) {
				parLockNode = parentLockedNode;
				return this;
			}

			QList<CHierarchyNode *> *CNodeBottomUpTraversalClassificationItem::getNewBranchNodeList() {
				return &newBranchNodeList;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendNewBranchNodeList(QList<CHierarchyNode *> &nodeList) {
				foreach (CHierarchyNode* node, nodeList) {
					appendNewBranchNode(node);
				}
				return this;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::appendNewBranchNode(CHierarchyNode *branchNode) {
				if (!newBranchNodeSet.contains(branchNode)) {
					newBranchNodeList.append(branchNode);
					newBranchNodeSet.insert(branchNode);
				}
				return this;
			}


			bool CNodeBottomUpTraversalClassificationItem::hasAllSilbingBranchTested() {
				return allSilbingBranchTested;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::setAllSilbingBranchTested(bool allSilbingsTested) {
				allSilbingBranchTested = allSilbingsTested;
				return this;
			}

			qint64 CNodeBottomUpTraversalClassificationItem::getSilbingBranchTestNodeCount() {
				return silbingBranchTestNodeCount;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::decSilbingBranchTestNodeCount() {
				--silbingBranchTestNodeCount;
				return this;
			}


			bool CNodeBottomUpTraversalClassificationItem::isChildNodeAddedContinueBlocking() {
				return mChildNodeAdded;
			}

			CNodeBottomUpTraversalClassificationItem *CNodeBottomUpTraversalClassificationItem::setChildNodeAddedContinueBlocking(bool addedChildNode) {
				mChildNodeAdded = addedChildNode;
				return this;
			}

			QList<CHierarchyNode*> CNodeBottomUpTraversalClassificationItem::getChildNodeAddedContinueBlockingList() {
				return childNodeContinueBlockingList;
			}

			CNodeBottomUpTraversalClassificationItem* CNodeBottomUpTraversalClassificationItem::addChildNodeAddedContinueBlocking(CHierarchyNode* continueBlocking) {
				childNodeContinueBlockingList.append(continueBlocking);
				return this;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
