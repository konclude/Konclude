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

#include "CHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			CHierarchyNode::CHierarchyNode(CConcept *representiveConcept) {
				eqConList.append(representiveConcept);
				eqConCount = 1;
				active = true;
				mention = false;
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
			}


			CHierarchyNode::~CHierarchyNode() {
			}


			CHierarchyNode *CHierarchyNode::addParentNode(CHierarchyNode *parentNode) {
				parentNodeSet.insert(parentNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::addChildNode(CHierarchyNode *childNode) {
				childNodeSet.insert(childNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeParent(CHierarchyNode *parentNode) {
				addParentNode(parentNode);
				parentNode->addChildNode(this);
				makePredecessor(parentNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeChild(CHierarchyNode *childNode) {
				childNodeSet.insert(childNode);
				childNode->addParentNode(this);
				makeSuccessor(childNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CHierarchyNode::hasChildNode(CHierarchyNode *node) {
				return childNodeSet.contains(node);
			}

			bool CHierarchyNode::hasParentNode(CHierarchyNode *node) {
				return parentNodeSet.contains(node);
			}


			CHierarchyNode *CHierarchyNode::addEquivalentConcept(CConcept *concept) {
				eqConList.append(concept);
				eqConCount++;
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::addPredecessorNode(CHierarchyNode *preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::addSuccessorNode(CHierarchyNode *succNode) {
				succNodeSet.insert(succNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


			CHierarchyNode *CHierarchyNode::makePredecessor(CHierarchyNode *preNode) {
				predNodeSet.insert(preNode);
				preNode->addSuccessorNode(this);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeSuccessor(CHierarchyNode *succNode) {
				succNodeSet.insert(succNode);
				succNode->addPredecessorNode(this);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


			bool CHierarchyNode::hasSuccessorNode(CHierarchyNode *node) {
				return succNodeSet.contains(node);
			}

			bool CHierarchyNode::hasPredecessorNode(CHierarchyNode *node) {
				return predNodeSet.contains(node);
			}



			CHierarchyNode *CHierarchyNode::removeParent(CHierarchyNode *parentNode) {
				parentNodeSet.remove(parentNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::removeChild(CHierarchyNode *childNode) {
				childNodeSet.remove(childNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			QSet<CHierarchyNode *> *CHierarchyNode::getParentNodeSet() {
				return &parentNodeSet;
			}

			QSet<CHierarchyNode *> *CHierarchyNode::getChildNodeSet() {
				return &childNodeSet;
			}

			QSet<CHierarchyNode *> *CHierarchyNode::getPredecessorNodeSet() {
				return &predNodeSet;
			}

			QSet<CHierarchyNode *> *CHierarchyNode::getSuccessorNodeSet() {
				return &succNodeSet;
			}

			QList<CConcept *> *CHierarchyNode::getEquivalentConceptList() {
				return &eqConList;
			}


			CHierarchyNode *CHierarchyNode::removePredecessor(CHierarchyNode *predNode) {
				predNodeSet.remove(predNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::removeSuccessor(CHierarchyNode *succNode) {
				succNodeSet.remove(succNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			QList<CHierarchyNode *> CHierarchyNode::getParentNodeList() {
				return parentNodeSet.values();
			}

			QList<CHierarchyNode *> CHierarchyNode::getChildNodeList() {
				return childNodeSet.values();
			}

			QList<CHierarchyNode *> CHierarchyNode::getPredecessorNodeList() {
				return predNodeSet.values();
			}

			QList<CHierarchyNode *> CHierarchyNode::getSuccessorNodeList() {
				return succNodeSet.values();
			}



			CHierarchyNode *CHierarchyNode::addIncompatiblePredecessorNode(CHierarchyNode *incompNode) {
				incompPredNodeSet.insert(incompNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeIncompatiblePredecessor(CHierarchyNode *incompNode) {
				addIncompatiblePredecessorNode(incompNode);
				incompNode->addIncompatibleSuccessorNode(this);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CHierarchyNode::hasIncompatiblePredecessorNode(CHierarchyNode *node) {
				return incompPredNodeSet.contains(node);
			}


			CHierarchyNode *CHierarchyNode::removeIncompatiblePredecessorNode(CHierarchyNode *incompNode) {
				incompPredNodeSet.remove(incompNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::addIncompatibleSuccessorNode(CHierarchyNode *incompNode) {
				incompSuccNodeSet.insert(incompNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeIncompatibleSuccessor(CHierarchyNode *incompNode) {
				addIncompatibleSuccessorNode(incompNode);
				incompNode->addIncompatiblePredecessorNode(this);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CHierarchyNode::hasIncompatibleSuccessorNode(CHierarchyNode *node) {
				return incompSuccNodeSet.contains(node);
			}

			CHierarchyNode *CHierarchyNode::removeIncompatibleSuccessorNode(CHierarchyNode *incompNode) {
				incompSuccNodeSet.remove(incompNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			CHierarchyNode *CHierarchyNode::addDisjointNode(CHierarchyNode *disjNode) {
				disjointNodeSet.insert(disjNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CHierarchyNode *CHierarchyNode::makeDisjoint(CHierarchyNode *disjNode) {
				addDisjointNode(disjNode);
				disjNode->addDisjointNode(this);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CHierarchyNode::hasDisjointNode(CHierarchyNode *node) {
				return disjointNodeSet.contains(node);
			}

			CHierarchyNode *CHierarchyNode::removeDisjointNode(CHierarchyNode *disjNode) {
				disjointNodeSet.remove(disjNode);
#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}




			QSet<CHierarchyNode *> *CHierarchyNode::getIncompatibleSuccessorNodeSet() {
				return &incompSuccNodeSet;
			}

			QSet<CHierarchyNode *> *CHierarchyNode::getIncompatiblePredecessorNodeSet() {
				return &incompPredNodeSet;
			}


			QList<CHierarchyNode *> CHierarchyNode::getIncompatibleSuccessorNodeList() {
				return incompSuccNodeSet.values();
			}

			QList<CHierarchyNode *> CHierarchyNode::getIncompatiblePredecessorNodeList() {
				return incompPredNodeSet.values();
			}



			QSet<CHierarchyNode *> *CHierarchyNode::getDisjointNodeSet() {
				return &disjointNodeSet;
			}

			QList<CHierarchyNode *> CHierarchyNode::getDisjointNodeList() {
				return disjointNodeSet.values();
			}


			CConcept *CHierarchyNode::getOneEquivalentConcept() {
				CConcept *concept = 0;
				if (!eqConList.isEmpty()) {
					concept = eqConList.first();
				}
				return concept;
			}


			bool CHierarchyNode::isActive() {
				return active;
			}

			CHierarchyNode *CHierarchyNode::setActive(bool activeNode) {
				active = activeNode;
				return this;
			}

			CHierarchyNode *CHierarchyNode::setInactive(bool inactiveNode) {
				active = !inactiveNode;
				return this;
			}


			bool CHierarchyNode::isNeedExplicitMentioned() {
				return mention;
			}

			CHierarchyNode *CHierarchyNode::setNeedExplicitMentioned(bool needsExplicitMentioned) {
				mention = needsExplicitMentioned;
				return this;
			}




			QString CHierarchyNode::getEquivalentConceptsNodeString() {
				QString string;
				foreach (CConcept *con, eqConList) {
					QString iriClassNameString = CIRIName::getRecentIRIName(con->getClassNameLinker());
					if (!string.isEmpty()) {
						string += QString(" <=> %1").arg(iriClassNameString);
					} else {
						string = iriClassNameString;
					}
				}
				return string;
			}


			QString CHierarchyNode::getHierarchyConceptsNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + getEquivalentConceptsNodeString() + QString(" >\n");
				foreach (CHierarchyNode *childNode, getChildNodeList()) {
					string += childNode->getHierarchyConceptsNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentConceptsNodeString() + QString(" >\n");
				return string;
			}


			QStringList CHierarchyNode::getEquivalentConceptStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				foreach (CConcept *con, eqConList) {
					QStringList iriClassNameStringList;
					if (preferAbbreviatedNames) {
						iriClassNameStringList.append(CAbbreviatedIRIName::getRecentAbbreviatedPrefixWithAbbreviatedIRIName(con->getClassNameLinker()));
					} 
					
					if (!preferAbbreviatedNames || iriClassNameStringList.isEmpty()) {
						iriClassNameStringList.append(CIRIName::getRecentIRIName(con->getClassNameLinker()));
					}
					stringList<<iriClassNameStringList;
				}
				return stringList;
			}


			qint64 CHierarchyNode::getEquivalentConceptCount() {
				return eqConCount;
			}


#ifdef DEBUGHIERARCHYNODECONCEPTSTRINGS
			CHierarchyNode *CHierarchyNode::generateNodeStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentConceptStringList(true);
				parentNodeStrings.clear();
				foreach (CHierarchyNode *node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				childNodeStrings.clear();
				foreach (CHierarchyNode *node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				precNodeStrings.clear();
				foreach (CHierarchyNode *node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				succNodeStrings.clear();
				foreach (CHierarchyNode *node, getSuccessorNodeList()) {
					succNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				incPrecNodeStrings.clear();
				foreach (CHierarchyNode *node, getIncompatiblePredecessorNodeList()) {
					incPrecNodeStrings<<node->getEquivalentConceptsNodeString();
				}			
				incSuccNodeStrings.clear();
				foreach (CHierarchyNode *node, getIncompatibleSuccessorNodeList()) {
					incSuccNodeStrings<<node->getEquivalentConceptsNodeString();
				}	
				disjointNodeStrings.clear();
				foreach (CHierarchyNode *node, getDisjointNodeList()) {
					disjointNodeStrings<<node->getEquivalentConceptsNodeString();
				}	
				return this;
			}
#endif


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude
