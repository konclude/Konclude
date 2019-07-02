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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_CPP


#include "CPCPrHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class E>
			CPCPrHierarchyNode<T,E>::CPCPrHierarchyNode(const E &representiveElement) : CAHierarchyNode<T,E>(representiveElement) {
				addPredecessorNode(this);
			}


			template<class T, class E>
			CPCPrHierarchyNode<T,E>::~CPCPrHierarchyNode() {
			}


			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::addParentNode(CPCPrHierarchyNode<T,E>* parentNode) {
				parentNodeSet.insert(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::addChildNode(CPCPrHierarchyNode<T,E>* childNode) {
				childNodeSet.insert(childNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::makeParent(CPCPrHierarchyNode<T,E>* parentNode) {
				parentNodeSet.insert(parentNode);
				parentNode->addChildNode(this);
				makePredecessor(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::makeChild(CPCPrHierarchyNode<T,E>* childNode) {
				childNodeSet.insert(childNode);
				childNode->addParentNode(this);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			bool CPCPrHierarchyNode<T,E>::hasChildNode(CPCPrHierarchyNode<T,E>* node) {
				return childNodeSet.contains(node);
			}

			template<class T, class E>
			bool CPCPrHierarchyNode<T,E>::hasParentNode(CPCPrHierarchyNode<T,E>* node) {
				return parentNodeSet.contains(node);
			}



			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::addPredecessorNode(CPCPrHierarchyNode<T,E>* preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::makePredecessor(CPCPrHierarchyNode<T,E>* preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			bool CPCPrHierarchyNode<T,E>::hasPredecessorNode(CPCPrHierarchyNode<T,E>* node) {
				return predNodeSet.contains(node);
			}



			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::removeParent(CPCPrHierarchyNode<T,E>* parentNode) {
				parentNodeSet.remove(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::removeChild(CPCPrHierarchyNode<T,E>* childNode) {
				childNodeSet.remove(childNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			QSet<T*>* CPCPrHierarchyNode<T,E>::getParentNodeSet() {
				return (QSet<T*>* )&parentNodeSet;
			}

			template<class T, class E>
			QSet<T*>* CPCPrHierarchyNode<T,E>::getChildNodeSet() {
				return (QSet<T*>* )&childNodeSet;
			}

			template<class T, class E>
			QSet<T*>* CPCPrHierarchyNode<T,E>::getPredecessorNodeSet() {
				return (QSet<T*>* )&predNodeSet;
			}



			template<class T, class E>
			T* CPCPrHierarchyNode<T,E>::removePredecessor(CPCPrHierarchyNode<T,E>* predNode) {
				predNodeSet.remove(predNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			QList<T*> CPCPrHierarchyNode<T,E>::getParentNodeList() {
				QList<CPCPrHierarchyNode*> parentNodeList(parentNodeSet.values());
				return (*(QList<T*>* )(&parentNodeList));
			}

			template<class T, class E>
			QList<T*> CPCPrHierarchyNode<T,E>::getChildNodeList() {
				QList<CPCPrHierarchyNode*> childNodeList(childNodeSet.values());
				return (*(QList<T*>* )(&(childNodeList)));
			}

			template<class T, class E>
			QList<T*> CPCPrHierarchyNode<T,E>::getPredecessorNodeList() {
				QList<CPCPrHierarchyNode*> predNodeList(predNodeSet.values());
				return (*(QList<T*>* )(&(predNodeList)));
			}



#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T, class E>
			void CPCPrHierarchyNode<T,E>::generateNodeRelationStrings() {
			}
#endif



		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CPCPRHIERARCHYNODE_CPP
