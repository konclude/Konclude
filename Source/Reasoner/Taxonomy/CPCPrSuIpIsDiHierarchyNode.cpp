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

#include "CPCPrSuIpIsDiHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			CPCPrSuIpIsDiHierarchyNode::CPCPrSuIpIsDiHierarchyNode(CConcept *representiveConcept) {
				eqConList.append(representiveConcept);
				eqConCount = 1;
				active = true;
				mention = false;
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
			}


			CPCPrSuIpIsDiHierarchyNode::~CPCPrSuIpIsDiHierarchyNode() {
			}


			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addParentNode(CPCPrSuIpIsDiHierarchyNode *parentNode) {
				parentNodeSet.insert(parentNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addChildNode(CPCPrSuIpIsDiHierarchyNode *childNode) {
				childNodeSet.insert(childNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeParent(CPCPrSuIpIsDiHierarchyNode *parentNode) {
				parentNodeSet.insert(parentNode);
				parentNode->addChildNode(this);
				makePredecessor(parentNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeChild(CPCPrSuIpIsDiHierarchyNode *childNode) {
				childNodeSet.insert(childNode);
				childNode->addParentNode(this);
				makeSuccessor(childNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasChildNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return childNodeSet.contains(node);
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasParentNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return parentNodeSet.contains(node);
			}


			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addEquivalentConcept(CConcept *concept) {
				eqConList.append(concept);
				eqConCount++;
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addPredecessorNode(CPCPrSuIpIsDiHierarchyNode *preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addSuccessorNode(CPCPrSuIpIsDiHierarchyNode *succNode) {
				succNodeSet.insert(succNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makePredecessor(CPCPrSuIpIsDiHierarchyNode *preNode) {
				predNodeSet.insert(preNode);
				preNode->addSuccessorNode(this);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeSuccessor(CPCPrSuIpIsDiHierarchyNode *succNode) {
				succNodeSet.insert(succNode);
				succNode->addPredecessorNode(this);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}


			bool CPCPrSuIpIsDiHierarchyNode::hasSuccessorNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return succNodeSet.contains(node);
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasPredecessorNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return predNodeSet.contains(node);
			}



			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeParent(CPCPrSuIpIsDiHierarchyNode *parentNode) {
				parentNodeSet.remove(parentNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeChild(CPCPrSuIpIsDiHierarchyNode *childNode) {
				childNodeSet.remove(childNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getParentNodeSet() {
				return &parentNodeSet;
			}

			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getChildNodeSet() {
				return &childNodeSet;
			}

			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getPredecessorNodeSet() {
				return &predNodeSet;
			}

			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getSuccessorNodeSet() {
				return &succNodeSet;
			}

			QList<CConcept *> *CPCPrSuIpIsDiHierarchyNode::getEquivalentConceptList() {
				return &eqConList;
			}


			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removePredecessor(CPCPrSuIpIsDiHierarchyNode *predNode) {
				predNodeSet.remove(predNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeSuccessor(CPCPrSuIpIsDiHierarchyNode *succNode) {
				succNodeSet.remove(succNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getParentNodeList() {
				return parentNodeSet.values();
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getChildNodeList() {
				return childNodeSet.values();
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getPredecessorNodeList() {
				return predNodeSet.values();
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getSuccessorNodeList() {
				return succNodeSet.values();
			}



			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				incompPredNodeSet.insert(incompNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeIncompatiblePredecessor(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				addIncompatiblePredecessorNode(incompNode);
				incompNode->addIncompatibleSuccessorNode(this);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return incompPredNodeSet.contains(node);
			}


			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeIncompatiblePredecessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				incompPredNodeSet.remove(incompNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				incompSuccNodeSet.insert(incompNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeIncompatibleSuccessor(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				addIncompatibleSuccessorNode(incompNode);
				incompNode->addIncompatiblePredecessorNode(this);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return incompSuccNodeSet.contains(node);
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeIncompatibleSuccessorNode(CPCPrSuIpIsDiHierarchyNode *incompNode) {
				incompSuccNodeSet.remove(incompNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}



			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::addDisjointNode(CPCPrSuIpIsDiHierarchyNode *disjNode) {
				disjointNodeSet.insert(disjNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::makeDisjoint(CPCPrSuIpIsDiHierarchyNode *disjNode) {
				addDisjointNode(disjNode);
				disjNode->addDisjointNode(this);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}

			bool CPCPrSuIpIsDiHierarchyNode::hasDisjointNode(CPCPrSuIpIsDiHierarchyNode *node) {
				return disjointNodeSet.contains(node);
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::removeDisjointNode(CPCPrSuIpIsDiHierarchyNode *disjNode) {
				disjointNodeSet.remove(disjNode);
#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
				generateNodeStrings();
#endif
				return this;
			}




			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getIncompatibleSuccessorNodeSet() {
				return &incompSuccNodeSet;
			}

			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getIncompatiblePredecessorNodeSet() {
				return &incompPredNodeSet;
			}


			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getIncompatibleSuccessorNodeList() {
				return incompSuccNodeSet.values();
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getIncompatiblePredecessorNodeList() {
				return incompPredNodeSet.values();
			}



			QSet<CPCPrSuIpIsDiHierarchyNode *> *CPCPrSuIpIsDiHierarchyNode::getDisjointNodeSet() {
				return &disjointNodeSet;
			}

			QList<CPCPrSuIpIsDiHierarchyNode *> CPCPrSuIpIsDiHierarchyNode::getDisjointNodeList() {
				return disjointNodeSet.values();
			}


			CConcept *CPCPrSuIpIsDiHierarchyNode::getOneEquivalentConcept() {
				CConcept *concept = 0;
				if (!eqConList.isEmpty()) {
					concept = eqConList.first();
				}
				return concept;
			}


			bool CPCPrSuIpIsDiHierarchyNode::isActive() {
				return active;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::setActive(bool activeNode) {
				active = activeNode;
				return this;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::setInactive(bool inactiveNode) {
				active = !inactiveNode;
				return this;
			}


			bool CPCPrSuIpIsDiHierarchyNode::isNeedExplicitMentioned() {
				return mention;
			}

			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::setNeedExplicitMentioned(bool needsExplicitMentioned) {
				mention = needsExplicitMentioned;
				return this;
			}




			QString CPCPrSuIpIsDiHierarchyNode::getEquivalentConceptsNodeString() {
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


			QString CPCPrSuIpIsDiHierarchyNode::getHierarchyConceptsNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + getEquivalentConceptsNodeString() + QString(" >\n");
				foreach (CPCPrSuIpIsDiHierarchyNode *childNode, getChildNodeList()) {
					string += childNode->getHierarchyConceptsNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentConceptsNodeString() + QString(" >\n");
				return string;
			}


			QStringList CPCPrSuIpIsDiHierarchyNode::getEquivalentConceptStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				foreach (CConcept *con, eqConList) {
					QStringList iriClassNameStringList;
					if (preferAbbreviatedNames) {
						iriClassNameStringList = CAbbreviatedIRIName::getAbbreviatedPrefixWithAbbreviatedIRINames(con->getClassNameLinker());
					} 
					
					if (!preferAbbreviatedNames || iriClassNameStringList.isEmpty()) {
						iriClassNameStringList = CIRIName::getIRINames(con->getClassNameLinker());
					}
					stringList<<iriClassNameStringList;
				}
				return stringList;
			}


			qint64 CPCPrSuIpIsDiHierarchyNode::getEquivalentConceptCount() {
				return eqConCount;
			}


#ifdef DEBUGCPCPRSUIPISDIHIERARCHYNODECONCEPTSTRINGS
			CPCPrSuIpIsDiHierarchyNode *CPCPrSuIpIsDiHierarchyNode::generateNodeStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentConceptStringList(true);
				parentNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				childNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				precNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				succNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getSuccessorNodeList()) {
					succNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				incPrecNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getIncompatiblePredecessorNodeList()) {
					incPrecNodeStrings<<node->getEquivalentConceptsNodeString();
				}			
				incSuccNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getIncompatibleSuccessorNodeList()) {
					incSuccNodeStrings<<node->getEquivalentConceptsNodeString();
				}	
				disjointNodeStrings.clear();
				foreach (CPCPrSuIpIsDiHierarchyNode *node, getDisjointNodeList()) {
					disjointNodeStrings<<node->getEquivalentConceptsNodeString();
				}	
				return this;
			}
#endif


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude
