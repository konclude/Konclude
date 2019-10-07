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

#ifndef KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_CPP


#include "CParentChildPredecessorHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class E>
			CParentChildPredecessorHierarchyNode<T,E>::CParentChildPredecessorHierarchyNode(const E &representiveElement) : CAbstractHierarchyNode<T,E>(representiveElement) {
				addPredecessorNode(this);
			}


			template<class T, class E>
			CParentChildPredecessorHierarchyNode<T,E>::~CParentChildPredecessorHierarchyNode() {
			}


			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::addParentNode(CParentChildPredecessorHierarchyNode<T,E>* parentNode) {
				parentNodeSet.insert(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::addChildNode(CParentChildPredecessorHierarchyNode<T,E>* childNode) {
				childNodeSet.insert(childNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::makeParent(CParentChildPredecessorHierarchyNode<T,E>* parentNode) {
				parentNodeSet.insert(parentNode);
				parentNode->addChildNode(this);
				makePredecessor(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::makeChild(CParentChildPredecessorHierarchyNode<T,E>* childNode) {
				childNodeSet.insert(childNode);
				childNode->addParentNode(this);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			bool CParentChildPredecessorHierarchyNode<T,E>::hasChildNode(CParentChildPredecessorHierarchyNode<T,E>* node) {
				return childNodeSet.contains(node);
			}

			template<class T, class E>
			bool CParentChildPredecessorHierarchyNode<T,E>::hasParentNode(CParentChildPredecessorHierarchyNode<T,E>* node) {
				return parentNodeSet.contains(node);
			}



			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::addPredecessorNode(CParentChildPredecessorHierarchyNode<T,E>* preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::makePredecessor(CParentChildPredecessorHierarchyNode<T,E>* preNode) {
				predNodeSet.insert(preNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			bool CParentChildPredecessorHierarchyNode<T,E>::hasPredecessorNode(CParentChildPredecessorHierarchyNode<T,E>* node) {
				return predNodeSet.contains(node);
			}



			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::removeParent(CParentChildPredecessorHierarchyNode<T,E>* parentNode) {
				parentNodeSet.remove(parentNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::removeChild(CParentChildPredecessorHierarchyNode<T,E>* childNode) {
				childNodeSet.remove(childNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}



			template<class T, class E>
			QSet<T*>* CParentChildPredecessorHierarchyNode<T,E>::getParentNodeSet() {
				return (QSet<T*>* )&parentNodeSet;
			}

			template<class T, class E>
			QSet<T*>* CParentChildPredecessorHierarchyNode<T,E>::getChildNodeSet() {
				return (QSet<T*>* )&childNodeSet;
			}

			template<class T, class E>
			QSet<T*>* CParentChildPredecessorHierarchyNode<T,E>::getPredecessorNodeSet() {
				return (QSet<T*>* )&predNodeSet;
			}



			template<class T, class E>
			T* CParentChildPredecessorHierarchyNode<T,E>::removePredecessor(CParentChildPredecessorHierarchyNode<T,E>* predNode) {
				predNodeSet.remove(predNode);
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeRelationStrings();
#endif
				return (T*)this;
			}

			template<class T, class E>
			QList<T*> CParentChildPredecessorHierarchyNode<T,E>::getParentNodeList() {
				QList<CParentChildPredecessorHierarchyNode*> parentNodeList(parentNodeSet.values());
				return (*(QList<T*>* )(&parentNodeList));
			}

			template<class T, class E>
			QList<T*> CParentChildPredecessorHierarchyNode<T,E>::getChildNodeList() {
				QList<CParentChildPredecessorHierarchyNode*> childNodeList(childNodeSet.values());
				return (*(QList<T*>* )(&(childNodeList)));
			}

			template<class T, class E>
			QList<T*> CParentChildPredecessorHierarchyNode<T,E>::getPredecessorNodeList() {
				QList<CParentChildPredecessorHierarchyNode*> predNodeList(predNodeSet.values());
				return (*(QList<T*>* )(&(predNodeList)));
			}



#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T, class E>
			void CParentChildPredecessorHierarchyNode<T,E>::generateNodeRelationStrings() {
			}
#endif



		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CPARENTCHILDPREDECESSORHIERARCHYNODE_CPP
