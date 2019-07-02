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
#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP

#include "CConceptClassesParentChildPredecessorHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T>
			CConceptClassesParentChildPredecessorHierarchyNode<T>::CConceptClassesParentChildPredecessorHierarchyNode(CConcept* representiveConcept) : CParentChildPredecessorHierarchyNode<T,CConcept*>(representiveConcept) {
			}


			template<class T>
			T* CConceptClassesParentChildPredecessorHierarchyNode<T>::addEquivalentConcept(CConcept* concept) {
				CParentChildPredecessorHierarchyNode<T,CConcept*>::addEquivalentElement(concept);
				return (T*)this;
			}


			template<class T>
			QList<CConcept*>* CConceptClassesParentChildPredecessorHierarchyNode<T>::getEquivalentConceptList() {
				return CParentChildPredecessorHierarchyNode<T,CConcept*>::getEquivalentElementList();
			}


			template<class T>
			CConcept* CConceptClassesParentChildPredecessorHierarchyNode<T>::getOneEquivalentConcept() {
				return CParentChildPredecessorHierarchyNode<T,CConcept*>::getOneEquivalentElement(0);
			}


			template<class T>
			qint64 CConceptClassesParentChildPredecessorHierarchyNode<T>::getEquivalentConceptCount() {
				return CParentChildPredecessorHierarchyNode<T,CConcept*>::getEquivalentElementCount();
			}


			template<class T>
			QString CConceptClassesParentChildPredecessorHierarchyNode<T>::getEquivalentConceptsNodeString() {
				QString string;
				QList<CConcept*>* eqList = CParentChildPredecessorHierarchyNode<T,CConcept*>::getEquivalentElementList();
				for (typename QList<CConcept*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
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
			QString CConceptClassesParentChildPredecessorHierarchyNode<T>::getHierarchyConceptsNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + CParentChildPredecessorHierarchyNode<T,CConcept*>::getEquivalentConceptsNodeString() + QString(" >\n");
				QSet<T*>* childNodeSet = CParentChildPredecessorHierarchyNode<T,CConcept*>::getChildNodeSet();
				for (typename QSet<CConcept*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
					CConceptClassesParentChildPredecessorHierarchyNode<T>* childNode = *it;
					string += childNode->getHierarchyConceptsNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentConceptsNodeString() + QString(" >\n");
				return string;
			}


			template<class T>
			QStringList CConceptClassesParentChildPredecessorHierarchyNode<T>::getEquivalentConceptStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				QList<CConcept*>* eqList = CParentChildPredecessorHierarchyNode<T,CConcept*>::getEquivalentElementList();
				for (typename QList<CConcept*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
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
			void CConceptClassesParentChildPredecessorHierarchyNode<T>::generateNodeRelationStrings() {
				parentNodeStrings.clear();
				foreach (CConceptClassesParentChildPredecessorHierarchyNode<T>* node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				childNodeStrings.clear();
				foreach (CConceptClassesParentChildPredecessorHierarchyNode<T>* node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentConceptsNodeString();
				}
				precNodeStrings.clear();
				foreach (CConceptClassesParentChildPredecessorHierarchyNode<T>* node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentConceptsNodeString();
				}
			}
#endif

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T>
			void CConceptClassesParentChildPredecessorHierarchyNode<T>::generateNodeElementStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentConceptStringList(true);
			}
#endif

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
