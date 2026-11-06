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

#include "CTaxonomy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			CTaxonomy::CTaxonomy(CHierarchyNode *topHierarchyNode, CHierarchyNode *bottomHierarchyNode) {
				topNode = topHierarchyNode;
				bottomNode = bottomHierarchyNode;
				topNode->makeChild(bottomNode);
				nodeContainer.append(topNode);
				nodeContainer.append(bottomNode);
				topIsUnsatisfiable = false;
				foreach (CConcept *con, *topNode->getEquivalentConceptList()) {
					conNodeHash.insert(con,topNode);
				}
				foreach (CConcept *con, *bottomNode->getEquivalentConceptList()) {
					conNodeHash.insert(con,bottomNode);
				}
				taxonomyComplete = false;
			}

			CTaxonomy::CTaxonomy(CConcept *topConcept, CConcept *bottomConcept) {
				topNode = createHierarchyNode(topConcept);
				bottomNode = createHierarchyNode(bottomConcept);
				topNode->makeChild(bottomNode);
				nodeContainer.append(topNode);
				nodeContainer.append(bottomNode);
				topIsUnsatisfiable = false;
				foreach (CConcept *con, *topNode->getEquivalentConceptList()) {
					conNodeHash.insert(con,topNode);
				}
				foreach (CConcept *con, *bottomNode->getEquivalentConceptList()) {
					conNodeHash.insert(con,bottomNode);
				}
				taxonomyComplete = false;
			}



			CTaxonomy::~CTaxonomy() {
				qDeleteAll(nodeContainer);
			}

			bool CTaxonomy::isSubsumption(CConcept *subsumerConcept, CConcept *subsumedConcept) {
				if (topIsUnsatisfiable) {
					return true;
				}
				bool subsumed = false;
				CHierarchyNode *subsumerNode = conNodeHash.value(subsumerConcept);
				if (subsumerNode) {
					CHierarchyNode *subsumedNode = conNodeHash.value(subsumedConcept);
					if (subsumedNode) {
						return subsumerNode == subsumedNode || subsumerNode->hasPredecessorNode(subsumedNode);
					}
				}
				return subsumed;
			}



			bool CTaxonomy::isSatisfiable(CConcept *concept) {
				if (topIsUnsatisfiable) {
					return false;
				}
				CHierarchyNode *node = conNodeHash.value(concept);
				if (node) {
					if (node == topNode) {
						return topNode->hasIncompatiblePredecessorNode(bottomNode);
					}
					return node != bottomNode;
				}
				return false;
			}

			bool CTaxonomy::isNotSatisfiable(CConcept *concept) {
				if (topIsUnsatisfiable) {
					return true;
				}
				CHierarchyNode *node = conNodeHash.value(concept);
				if (node) {
					return node == bottomNode;
				}
				return false;
			}

			bool CTaxonomy::isTopHierarchyNode(CConcept *concept) {
				if (topIsUnsatisfiable) {
					return true;
				}
				CHierarchyNode *node = conNodeHash.value(concept);
				if (node) {
					return node == topNode;
				}
				return false;
			}

			bool CTaxonomy::isBottomHierarchyNode(CConcept *concept) {
				if (topIsUnsatisfiable) {
					return true;
				}
				CHierarchyNode *node = conNodeHash.value(concept);
				if (node) {
					return node == bottomNode;
				}
				return false;
			}

			bool CTaxonomy::isEquivalence(CConcept *concept1, CConcept *concept2) {
				if (topIsUnsatisfiable) {
					return true;
				}
				CHierarchyNode *node1 = conNodeHash.value(concept1);
				CHierarchyNode *node2 = conNodeHash.value(concept2);
				if (node1 && node2) {
					return node1 == node2;
				} else {
					return false;
				}
			}


			bool CTaxonomy::isDisjoint(CConcept *disjointConcept1, CConcept *disjointConcept2) {
				if (topIsUnsatisfiable) {
					return false;
				}
				CHierarchyNode *node1 = conNodeHash.value(disjointConcept1);
				CHierarchyNode *node2 = conNodeHash.value(disjointConcept2);
				if (node1 && node2) {
					if (node1 == node2) {
						return false;
					} else {
						return node1->hasDisjointNode(node2);
					}
				} else {
					return false;
				}
			}

			bool CTaxonomy::isNotDisjoint(CConcept *disjointConcept1, CConcept *disjointConcept2) {
				if (topIsUnsatisfiable) {
					return true;
				}
				CHierarchyNode *node1 = conNodeHash.value(disjointConcept1);
				CHierarchyNode *node2 = conNodeHash.value(disjointConcept2);
				if (node1 && node2) {
					if (node1 == node2) {
						return true;
					} else if (node1->hasPredecessorNode(node2)) {
						return true;
					} else if (node2->hasPredecessorNode(node1)) {
						return true;
					}
				}
				return false;
			}


			CHierarchyNode *CTaxonomy::getTopHierarchyNode() {
				return topNode;
			}

			CHierarchyNode *CTaxonomy::getBottomHierarchyNode() {
				return bottomNode;
			}

			CTaxonomy *CTaxonomy::removeBypassedTopBottomLink() {
				if (topNode != bottomNode && bottomNode->hasParentNode(topNode) && bottomNode->getParentNodeSet()->count() > 1) {
					// the top node remains a predecessor of the bottom node, only the direct link goes
					bottomNode->removeParent(topNode);
					topNode->removeChild(bottomNode);
				}
				return this;
			}



			CHierarchyNode *CTaxonomy::createHierarchyNode(CConcept *concept) {
				return new CHierarchyNode(concept);
			}


			CHierarchyNode *CTaxonomy::getHierarchyNode(CConcept *concept, bool create) {
				CHierarchyNode *node = 0;
				if (concept) {
					node = conNodeHash.value(concept,0);
					if (topIsUnsatisfiable) {
						if (node != bottomNode && (node || create)) {
							conNodeHash.insert(concept,bottomNode);
							bottomNode->addEquivalentConcept(concept);
						}
					}
					if (!node && create) {
						node = createHierarchyNode(concept);
						conNodeHash.insert(concept,node);
						nodeContainer.append(node);
					}
				}
				return node;
			}

			bool CTaxonomy::hasHierarchyNode(CConcept *concept) {
				return conNodeHash.contains(concept);
			}

			bool CTaxonomy::isNotSubsumption(CConcept *notSubsumerConcept, CConcept *notSubsumedConcept) {
				if (topIsUnsatisfiable) {
					return false;
				}
				bool notSubsumed = false;
				CHierarchyNode *notSubsumerNode = conNodeHash.value(notSubsumerConcept);
				if (notSubsumerNode) {
					CHierarchyNode *notSubsumedNode = conNodeHash.value(notSubsumedConcept);
					if (notSubsumedNode) {
						return notSubsumerNode != notSubsumedNode && notSubsumerNode->hasIncompatiblePredecessorNode(notSubsumedNode);
					}
				}
				return notSubsumed;
				
			}

			QString CTaxonomy::getConceptHierarchyString() {
				QString string = topNode->getHierarchyConceptsNodeString(0);
				return string;
			}

			bool CTaxonomy::isComplete() {
				return taxonomyComplete;
			}

			CTaxonomy *CTaxonomy::setTaxonomyComplete(bool completed) {
				taxonomyComplete = completed;
				if (completed) {
					orderEquivalentConcepts();
				}
				return this;
			}


			bool CTaxonomy::orderEquivalentConcepts() {
				// the equivalent concepts of a node are collected in the order the classification
				// merged them, which differs from run to run; the writers and the queries name a node
				// by its first concept, so the lists are ordered by name, and two results of one
				// hierarchy come out the same
				QSet<CHierarchyNode*> orderedNodeSet;
				QHash<CConcept*,CHierarchyNode*>* nodeHash = getConceptHierarchyNodeHash();
				for (QHash<CConcept*,CHierarchyNode*>::const_iterator it = nodeHash->constBegin(), itEnd = nodeHash->constEnd(); it != itEnd; ++it) {
					CHierarchyNode* node = it.value();
					if (!node || orderedNodeSet.contains(node)) {
						continue;
					}
					orderedNodeSet.insert(node);
					QList<CConcept*>* eqConList = node->getEquivalentConceptList();
					if (eqConList->size() > 1) {
						qSort(eqConList->begin(), eqConList->end(), conceptNameLessThan);
					}
				}
				return true;
			}


			bool CTaxonomy::conceptNameLessThan(CConcept* concept1, CConcept* concept2) {
				// the built-in bottom (tag 0) and top (tag 1) concepts stay the representatives of their nodes
				cint64 tag1 = concept1->getConceptTag();
				cint64 tag2 = concept2->getConceptTag();
				if (tag1 <= 1 || tag2 <= 1) {
					return tag1 < tag2;
				}
				QString name1 = CIRIName::getRecentIRIName(concept1->getClassNameLinker());
				QString name2 = CIRIName::getRecentIRIName(concept2->getClassNameLinker());
				if (name1 != name2) {
					return name1 < name2;
				}
				return concept1->getConceptTag() < concept2->getConceptTag();
			}

			bool CTaxonomy::isActiveNode(CHierarchyNode *node) {
				return node->isActive();
			}

			CTaxonomy *CTaxonomy::setActiveNode(CHierarchyNode *node, bool nodeActive) {
				node->setActive(nodeActive);
				if (nodeActive) {
					activeNodeSet.insert(node);
				} else {
					activeNodeSet.remove(node);
				}
				return this;
			}

			CTaxonomy *CTaxonomy::setInactiveNode(CHierarchyNode *node, bool nodeInactive) {
				node->setInactive(nodeInactive);
				if (!nodeInactive) {
					activeNodeSet.insert(node);
				} else {
					activeNodeSet.remove(node);
				}
				return this;
			}

			CHierarchyNode *CTaxonomy::getActiveNode(CHierarchyNode *node) {
				if (node->isActive()) {
					return node;
				} else {
					CConcept *con = node->getOneEquivalentConcept();
					return getHierarchyNode(con,false);
				}
			}

			CTaxonomy *CTaxonomy::updateNodeEquivalences(CHierarchyNode *node) {
				foreach (CConcept *con, *node->getEquivalentConceptList()) {
					conNodeHash.insert(con,node);
				}
				return this;
			}

			QHash<CConcept*,CHierarchyNode*>* CTaxonomy::getConceptHierarchyNodeHash() {
				return &conNodeHash;
			}

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude
