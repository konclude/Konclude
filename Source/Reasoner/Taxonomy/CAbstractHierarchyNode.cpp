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

#ifndef KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_CPP
#define KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_CPP


#include "CAbstractHierarchyNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class E>
			CAbstractHierarchyNode<T,E>::CAbstractHierarchyNode(const E &representiveElement) {
				eqElementList.append(representiveElement);
				eqElemCount = 1;
				mNodeData = nullptr;
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeElementStrings();
#endif
			}


			template<class T, class E>
			CAbstractHierarchyNode<T,E>::~CAbstractHierarchyNode() {
			}


			template<class T, class E>
			T* CAbstractHierarchyNode<T,E>::addEquivalentElement(const E &element) {
				eqElementList.append(element);
				eqElemCount++;
#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
				generateNodeElementStrings();
#endif
				return (T*)this;
			}


			template<class T, class E>
			QList<E>* CAbstractHierarchyNode<T,E>::getEquivalentElementList() {
				return &eqElementList;
			}


			template<class T, class E>
			E CAbstractHierarchyNode<T,E>::getOneEquivalentElement(const E &defaultValue) {
				E element(defaultValue);
				if (!eqElementList.isEmpty()) {
					element = eqElementList.first();
				}
				return element;
			}

			template<class T, class E>
			E CAbstractHierarchyNode<T,E>::getOneEquivalentElement() {
				return eqElementList.first();
			}


			template<class T, class E>
			qint64 CAbstractHierarchyNode<T,E>::getEquivalentElementCount() {
				return eqElemCount;
			}

			template<class T, class E>
			CHierarchyNodeData* CAbstractHierarchyNode<T,E>::getNodeData() {
				return mNodeData;
			}

			template<class T, class E>
			bool CAbstractHierarchyNode<T,E>::hasNodeData() {
				return mNodeData != nullptr;
			}

#ifdef DEBUGHIERARCHYNODEELEMENTSTRINGS
			template<class T, class E>
			void CAbstractHierarchyNode<T,E>::generateNodeElementStrings() {
			}
#endif

			template<class T, class E>
			CHierarchyNodeData* CAbstractHierarchyNode<T,E>::setNodeData(CHierarchyNodeData* nodeData) {
				CHierarchyNodeData* tmpData = nodeData;
				mNodeData = nodeData;
				return tmpData;
			}



		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYNODE_CPP
