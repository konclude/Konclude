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

#ifndef KONCLUDE_REASONER_TAXONOMY_CAHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CAHIERARCHYNODE_CPP


#include "CAHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class E>
			CAHierarchyNode<T,E>::CAHierarchyNode(const E &representiveElement) {
				eqElementList.append(representiveElement);
				eqElemCount = 1;
				active = true;
				mention = false;
				mNodeData = nullptr;
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeElementStrings();
#endif
			}


			template<class T, class E>
			CAHierarchyNode<T,E>::~CAHierarchyNode() {
			}


			template<class T, class E>
			T* CAHierarchyNode<T,E>::addEquivalentElement(const E &element) {
				eqElementList.append(element);
				eqElemCount++;
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeElementStrings();
#endif
				return (T*)this;
			}


			template<class T, class E>
			QList<E>* CAHierarchyNode<T,E>::getEquivalentElementList() {
				return &eqElementList;
			}


			template<class T, class E>
			E CAHierarchyNode<T,E>::getOneEquivalentElement(const E &defaultValue) {
				E element(defaultValue);
				if (!eqElementList.isEmpty()) {
					element = eqElementList.first();
				}
				return element;
			}

			template<class T, class E>
			E CAHierarchyNode<T,E>::getOneEquivalentElement() {
				return eqElementList.first();
			}

			template<class T, class E>
			bool CAHierarchyNode<T,E>::isActive() {
				return active;
			}

			template<class T, class E>
			T* CAHierarchyNode<T,E>::setActive(bool activeNode) {
				active = activeNode;
				return (T*)this;
			}

			template<class T, class E>
			T* CAHierarchyNode<T,E>::setInactive(bool inactiveNode) {
				active = !inactiveNode;
				return (T*)this;
			}


			template<class T, class E>
			bool CAHierarchyNode<T,E>::isNeedExplicitMentioned() {
				return mention;
			}

			template<class T, class E>
			T* CAHierarchyNode<T,E>::setNeedExplicitMentioned(bool needsExplicitMentioned) {
				mention = needsExplicitMentioned;
				return (T*)this;
			}


			template<class T, class E>
			qint64 CAHierarchyNode<T,E>::getEquivalentElementCount() {
				return eqElemCount;
			}

			template<class T, class E>
			CHierarchyNodeData* CAHierarchyNode<T,E>::getNodeData() {
				return mNodeData;
			}

			template<class T, class E>
			bool CAHierarchyNode<T,E>::hasNodeData() {
				return mNodeData != nullptr;
			}


#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T, class E>
			void CAHierarchyNode<T,E>::generateNodeElementStrings() {
			}
#endif

			template<class T, class E>
			CHierarchyNodeData* CAHierarchyNode<T,E>::setNodeData(CHierarchyNodeData* nodeData) {
				CHierarchyNodeData* tmpData = nodeData;
				mNodeData = nodeData;
				return tmpData;
			}



		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CAHIERARCHYNODE_CPP
