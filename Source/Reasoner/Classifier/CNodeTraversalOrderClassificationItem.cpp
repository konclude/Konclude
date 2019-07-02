/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CNodeTraversalOrderClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CNodeTraversalOrderClassificationItem::CNodeTraversalOrderClassificationItem(CHierarchyNode *node, CHierarchyNode *parentNode) {
				parentTestNode = parentNode;
				traversalNode = node;
				parentAgainstTestNode = 0;
				againstTested = false;
			}


			CNodeTraversalOrderClassificationItem::~CNodeTraversalOrderClassificationItem() {
			}

			CHierarchyNode *CNodeTraversalOrderClassificationItem::getTraversalNode() {
				return traversalNode;
			}

			CHierarchyNode *CNodeTraversalOrderClassificationItem::getParentTestingNode() {
				return parentTestNode;
			}

			CHierarchyNode *CNodeTraversalOrderClassificationItem::getParentAgainstTestingNode() {
				return parentAgainstTestNode;
			}

			CNodeTraversalOrderClassificationItem *CNodeTraversalOrderClassificationItem::setParentTestingNode(CHierarchyNode *node) {
				parentTestNode = node;
				return this;
			}

			QList<CHierarchyNode *> *CNodeTraversalOrderClassificationItem::getOrderTraversalList() {
				return &orderTraversalList;
			}

			CNodeTraversalOrderClassificationItem *CNodeTraversalOrderClassificationItem::appendOrderTraversalList(QList<CHierarchyNode *> &nodeList) {
				orderTraversalList << nodeList;
				return this;
			}

			CNodeTraversalOrderClassificationItem *CNodeTraversalOrderClassificationItem::setParentAgainstTestingNode(CHierarchyNode *node) {
				parentAgainstTestNode = node;
				againstTested = true;
				return this;
			}
			
			bool CNodeTraversalOrderClassificationItem::isParentAgainstTesting() {
				return againstTested;
			}

			QList<CHierarchyNode *> *CNodeTraversalOrderClassificationItem::getAgainstOrderTraversalList() {
				return &againstOrderTraversalList;
			}

			CNodeTraversalOrderClassificationItem *CNodeTraversalOrderClassificationItem::appendAgainstOrderTraversalList(QList<CHierarchyNode *> &nodeList) {
				againstOrderTraversalList << nodeList;
				return this;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
