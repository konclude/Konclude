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
#ifndef KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP

#include "CRolePropertiesParentChildPredecessorHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T>
			CRolePropertiesParentChildPredecessorHierarchyNode<T>::CRolePropertiesParentChildPredecessorHierarchyNode(CRole* representiveRole) : CParentChildPredecessorHierarchyNode<T,CRole*>(representiveRole) {
			}


			template<class T>
			T* CRolePropertiesParentChildPredecessorHierarchyNode<T>::addEquivalentRole(CRole* role) {
				CParentChildPredecessorHierarchyNode<T,CRole*>::addEquivalentElement(role);
				return (T*)this;
			}


			template<class T>
			QList<CRole*>* CRolePropertiesParentChildPredecessorHierarchyNode<T>::getEquivalentRoleList() {
				return CParentChildPredecessorHierarchyNode<T,CRole*>::getEquivalentElementList();
			}


			template<class T>
			CRole* CRolePropertiesParentChildPredecessorHierarchyNode<T>::getOneEquivalentRole() {
				return CParentChildPredecessorHierarchyNode<T,CRole*>::getOneEquivalentElement(0);
			}


			template<class T>
			qint64 CRolePropertiesParentChildPredecessorHierarchyNode<T>::getEquivalentRoleCount() {
				return CParentChildPredecessorHierarchyNode<T,CRole*>::getEquivalentElementCount();
			}


			template<class T>
			QString CRolePropertiesParentChildPredecessorHierarchyNode<T>::getEquivalentRoleNodeString() {
				QString string;
				QList<CRole*>* eqList = CParentChildPredecessorHierarchyNode<T,CRole*>::getEquivalentElementList();
				for (typename QList<CRole*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
					CRole* role = *it;
					QString iriPropertyNameString = CIRIName::getRecentIRIName(role->getPropertyNameLinker());
					if (!string.isEmpty()) {
						string += QString(" <=> %1").arg(iriPropertyNameString);
					} else {
						string = iriPropertyNameString;
					}
				}
				return string;
			}


			template<class T>
			QString CRolePropertiesParentChildPredecessorHierarchyNode<T>::getHierarchyRolesNodeString(qint64 depth) {
				QString string(depth,'\t');
				string += QString("< ") + CParentChildPredecessorHierarchyNode<T,CRole*>::getEquivalentRoleNodeString() + QString(" >\n");
				QSet<T*>* childNodeSet = CParentChildPredecessorHierarchyNode<T,CRole*>::getChildNodeSet();
				for (typename QSet<CRole*>::const_iterator it = childNodeSet->constBegin(), itEnd = childNodeSet->constEnd(); it != itEnd; ++it) {
					CRolePropertiesParentChildPredecessorHierarchyNode<T>* childNode = *it;
					string += childNode->getHierarchyRolesNodeString(depth+1);
				}
				string += QString(depth,'\t')+QString("</ ") + getEquivalentRoleNodeString() + QString(" >\n");
				return string;
			}


			template<class T>
			QStringList CRolePropertiesParentChildPredecessorHierarchyNode<T>::getEquivalentRoleStringList(bool preferAbbreviatedNames) {
				QStringList stringList;
				QList<CRole*>* eqList = CParentChildPredecessorHierarchyNode<T,CRole*>::getEquivalentElementList();
				for (typename QList<CRole*>::const_iterator it = eqList->constBegin(), itEnd = eqList->constEnd(); it != itEnd; ++it) {
					CRole* role = *it;
					QStringList iriClassNameStringList;
					if (preferAbbreviatedNames) {
						iriClassNameStringList = CAbbreviatedIRIName::getAbbreviatedPrefixWithAbbreviatedIRINames(role->getPropertyNameLinker());
					} 

					if (!preferAbbreviatedNames || iriClassNameStringList.isEmpty()) {
						iriClassNameStringList = CIRIName::getIRINames(role->getPropertyNameLinker());
					}
					stringList<<iriClassNameStringList;
				}
				return stringList;
			}

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T>
			void CRolePropertiesParentChildPredecessorHierarchyNode<T>::generateNodeRelationStrings() {
				parentNodeStrings.clear();
				foreach (CRolePropertiesParentChildPredecessorHierarchyNode<T>* node, getParentNodeList()) {
					parentNodeStrings<<node->getEquivalentRoleNodeString();
				}
				childNodeStrings.clear();
				foreach (CRolePropertiesParentChildPredecessorHierarchyNode<T>* node, getChildNodeList()) {
					childNodeStrings<<node->getEquivalentRoleNodeString();
				}
				precNodeStrings.clear();
				foreach (CRolePropertiesParentChildPredecessorHierarchyNode<T>* node, getPredecessorNodeList()) {
					precNodeStrings<<node->getEquivalentRoleNodeString();
				}
			}
#endif

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T>
			void CRolePropertiesParentChildPredecessorHierarchyNode<T>::generateNodeElementStrings() {
				nodeStrings.clear();
				nodeStrings = getEquivalentRoleStringList(true);
			}
#endif

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CROLEPROPERTIESPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
