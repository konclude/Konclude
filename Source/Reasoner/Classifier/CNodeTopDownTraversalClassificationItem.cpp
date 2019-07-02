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

#include "CNodeTopDownTraversalClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CNodeTopDownTraversalClassificationItem::CNodeTopDownTraversalClassificationItem(CHierarchyNode *node, CHierarchyNode *topNode, QList<CNodeTopDownTraversalClassificationItem *> *lockedReleaseProcessingList) {
				traversalNode = node;
				newBranchNodeList << topNode;
				lockedReleaseProcList = lockedReleaseProcessingList;
				testNode = 0;
				parLockNode = 0;
				eqTestNode = 0;
				subSumTestingResult = false;
				alredyTraversedDirectBlockedSuccessorNode = false;
			}


			CNodeTopDownTraversalClassificationItem::~CNodeTopDownTraversalClassificationItem() {
			}



			CNodeBlockedClassificationItem *CNodeTopDownTraversalClassificationItem::releaseBlockedResumeProcessing() {
				lockedReleaseProcList->append(this);
				return this;
			}

			CNodeBlockedClassificationItem *CNodeTopDownTraversalClassificationItem::releaseTestingBlockedResumeProcessing(CConcept *subsumerConcept, CConcept *subsumedConcept, bool testingResult) {
				subSumTestingResult = testingResult;
				lockedReleaseProcList->append(this);
				return this;
			}



			CHierarchyNode *CNodeTopDownTraversalClassificationItem::getTraversalNode() {
				return traversalNode;
			}

			QList<CHierarchyNode *> *CNodeTopDownTraversalClassificationItem::getTestingNodeList() {
				return &testingNodeList;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendTestingNodeList(QList<CHierarchyNode *> &nodeList) {
				testingNodeList << nodeList;
				return this;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendTestingNode(CHierarchyNode *node) {
				testingNodeList.append(node);
				return this;
			}


			QList<CHierarchyNode *> *CNodeTopDownTraversalClassificationItem::getParentNodeList() {
				return &parentNodeList;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendParentNodeList(QList<CHierarchyNode *> &nodeList) {
				parentNodeList << nodeList;
				return this;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendParentNode(CHierarchyNode *node) {
				parentNodeList.append(node);
				return this;
			}

			bool CNodeTopDownTraversalClassificationItem::hasNextTestingNode() {
				return !testingNodeList.isEmpty();
			}

			CHierarchyNode *CNodeTopDownTraversalClassificationItem::takeNextTestingNode() {
				testNode = testingNodeList.takeFirst();
				return testNode;
			}

			CHierarchyNode *CNodeTopDownTraversalClassificationItem::getTestingNode() {
				return testNode;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::clearTestingNode() {
				testNode = 0;
				return this;
			}

			bool CNodeTopDownTraversalClassificationItem::getTestingResult() {
				return subSumTestingResult;
			}


			QList<CHierarchyNode *> *CNodeTopDownTraversalClassificationItem::getBranchLockedNodeList() {
				return &branchLockedNodeList;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendBranchLockedNodeList(QList<CHierarchyNode *> &nodeList) {
				foreach (CHierarchyNode *node, nodeList) {
					appendBranchLockedNode(node);
				}
				return this;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendBranchLockedNode(CHierarchyNode *branchNode) {
				if (!branchLockedNodeSet.contains(branchNode)) {
					branchLockedNodeSet.insert(branchNode);
					branchLockedNodeList.append(branchNode);
				}
				return this;
			}

			bool CNodeTopDownTraversalClassificationItem::hasDirectBranchLockedNode() {
				return parLockNode != 0;
			}

			CHierarchyNode *CNodeTopDownTraversalClassificationItem::getDirectBranchLockedNode() {
				return parLockNode;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::setDirectBranchLockedNode(CHierarchyNode *parentLockedNode) {
				parLockNode = parentLockedNode;
				return this;
			}

			QList<CHierarchyNode *> *CNodeTopDownTraversalClassificationItem::getNewBranchNodeList() {
				return &newBranchNodeList;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendNewBranchNodeList(QList<CHierarchyNode *> &nodeList) {
				newBranchNodeList << nodeList;
				return this;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::appendNewBranchNode(CHierarchyNode *branchNode) {
				newBranchNodeList.append(branchNode);
				return this;
			}

			CHierarchyNode *CNodeTopDownTraversalClassificationItem::getEquivalentTestingNode() {
				return eqTestNode;
			}

			CNodeTopDownTraversalClassificationItem *CNodeTopDownTraversalClassificationItem::setEquivalentTestingNode(CHierarchyNode *equivalentTestingNode) {
				eqTestNode = equivalentTestingNode;
				return this;
			}

			CNodeTopDownTraversalClassificationItem* CNodeTopDownTraversalClassificationItem::setAlreadyTraversedDirectBlockedSuccessorNode(bool alredyTraversed) {
				alredyTraversedDirectBlockedSuccessorNode = alredyTraversed;
				return this;
			}

			bool CNodeTopDownTraversalClassificationItem::hasAlreadyTraversedDirectBlockedSuccessorNode() {
				return alredyTraversedDirectBlockedSuccessorNode;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
