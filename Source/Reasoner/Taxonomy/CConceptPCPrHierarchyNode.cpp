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

#include "CConceptPCPrHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			CConceptPCPrHierarchyNode::CConceptPCPrHierarchyNode(CConcept *representiveConcept) : CPCPrHierarchyNode<CConceptPCPrHierarchyNode, CConcept *>(representiveConcept) {
			}


			CConceptPCPrHierarchyNode::~CConceptPCPrHierarchyNode() {
			}


			CConceptPCPrHierarchyNode *CConceptPCPrHierarchyNode::addEquivalentConcept(CConcept *concept) {
				CPCPrHierarchyNode<CConceptPCPrHierarchyNode, CConcept *>::addEquivalentElement(concept);
				return this;
			}


			QList<CConcept *> *CConceptPCPrHierarchyNode::getEquivalentConceptList() {
				return CPCPrHierarchyNode<CConceptPCPrHierarchyNode, CConcept *>::getEquivalentElementList();
			}


			CConcept *CConceptPCPrHierarchyNode::getOneEquivalentConcept() {
				return CPCPrHierarchyNode<CConceptPCPrHierarchyNode, CConcept *>::getOneEquivalentElement(0);
			}


			qint64 CConceptPCPrHierarchyNode::getEquivalentConceptCount() {
				return eqElemCount;
			}


			QString CConceptPCPrHierarchyNode::getEquivalentConceptsNodeString() {
				QString string;
				foreach (CConcept *con, eqElementList) {
					QString iriClassNameString = CIRIName::getRecentIRIName(con->getClassNameLinker());
					if (!string.isEmpty()) {
						string += QString(" <=> %1").arg(iriClassNameString);
					} else {
						string = iriClassNameString;
					}
				}
				return string;
			}


			QString CConceptPCPrHierarchyNode::getHierarchyConceptsNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + getEquivalentConceptsNodeString() + QString(" >\n");
				foreach (CConceptPCPrHierarchyNode *childNode, getChildNodeList()) {
					string += childNode->getHierarchyConceptsNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentConceptsNodeString() + QString(" >\n");
				return string;
			}


			QStringList CConceptPCPrHierarchyNode::getEquivalentConceptStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				foreach (CConcept *con, eqElementList) {
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

#ifdef DEBUGCPCPRHIERARCHYNODEELEMENTSTRINGS
			CConceptPCPrHierarchyNode *CConceptPCPrHierarchyNode::generateNodeStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentConceptStringList(true);
				parentNodeStrings.clear();
				foreach (CConceptPCPrHierarchyNode *node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				childNodeStrings.clear();
				foreach (CConceptPCPrHierarchyNode *node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				precNodeStrings.clear();
				foreach (CConceptPCPrHierarchyNode *node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				return this;
			}
#endif


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude
