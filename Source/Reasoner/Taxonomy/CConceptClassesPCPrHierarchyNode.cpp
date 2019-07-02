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
#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_CPP

#include "CConceptClassesPCPrHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T>
			CConceptClassesPCPrHierarchyNode<T>::CConceptClassesPCPrHierarchyNode(CConcept* representiveConcept) : CPCPrHierarchyNode<T,CConcept*>(representiveConcept) {
			}


			template<class T>
			T* CConceptClassesPCPrHierarchyNode<T>::addEquivalentConcept(CConcept* concept) {
				CPCPrHierarchyNode<T,CConcept*>::addEquivalentElement(concept);
				return (T*)this;
			}


			template<class T>
			QList<CConcept*>* CConceptClassesPCPrHierarchyNode<T>::getEquivalentConceptList() {
				return CPCPrHierarchyNode<T,CConcept*>::getEquivalentElementList();
			}


			template<class T>
			CConcept* CConceptClassesPCPrHierarchyNode<T>::getOneEquivalentConcept() {
				return CPCPrHierarchyNode<T,CConcept*>::getOneEquivalentElement(0);
			}


			template<class T>
			qint64 CConceptClassesPCPrHierarchyNode<T>::getEquivalentConceptCount() {
				return CPCPrHierarchyNode<T,CConcept*>::getEquivalentElementCount();
			}


			template<class T>
			QString CConceptClassesPCPrHierarchyNode<T>::getEquivalentConceptsNodeString() {
				QString string;
				QList<CConcept*>* eqList = CPCPrHierarchyNode<T,CConcept*>::getEquivalentElementList();
				for (typename QList<T*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
					CConcept* con = *it;
					QString iriClassNameString = CIRIName::getRecentIRIName(con->getClassNameLinker());
					if (!string.isEmpty()) {
						string += QString(" <=> %1").arg(iriClassNameString);
					} else {
						string = iriClassNameString;
					}
				}
				return string;
			}


			template<class T>
			QString CConceptClassesPCPrHierarchyNode<T>::getHierarchyConceptsNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + CPCPrHierarchyNode<T,CConcept*>::getEquivalentConceptsNodeString() + QString(" >\n");
				QSet<T*>* childNodeSet = CPCPrHierarchyNode<T,CConcept*>::getChildNodeSet();
				for (typename QSet<T*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
					CConceptClassesPCPrHierarchyNode<T>* childNode = *it;
					string += childNode->getHierarchyConceptsNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentConceptsNodeString() + QString(" >\n");
				return string;
			}


			template<class T>
			QStringList CConceptClassesPCPrHierarchyNode<T>::getEquivalentConceptStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				QList<CConcept*>* eqList = CPCPrHierarchyNode<T,CConcept*>::getEquivalentElementList();
				for (typename QList<T*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
					CConcept* con = *it;
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

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T>
			void CConceptClassesPCPrHierarchyNode<T>::generateNodeRelationStrings() {
				parentNodeStrings.clear();
				foreach (CConceptClassesPCPrHierarchyNode<T>* node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				childNodeStrings.clear();
				foreach (CConceptClassesPCPrHierarchyNode<T>* node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				precNodeStrings.clear();
				foreach (CConceptClassesPCPrHierarchyNode<T>* node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentConceptsNodeString();
				}
			}
#endif

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T>
			void CConceptClassesPCPrHierarchyNode<T>::generateNodeElementStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentConceptStringList(true);
			}
#endif

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRHIERARCHYNODE_CPP
