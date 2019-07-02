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

#ifndef KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHY_CPP
#define KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHY_CPP


#include "CAbstractHierarchy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N, class E>
			CAbstractHierarchy<T,N,E>::CAbstractHierarchy() {
			}



			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isElementSubsumption(const E& subsumerElement, const E& subsumedElement) {
				N* subsumerNode = getHierarchyNode(subsumerElement);
				if (!subsumerNode) {
					return false;
				}
				N* subsumedNode = getHierarchyNode(subsumedElement);
				if (!subsumedNode) {
					return false;
				}
				return isNodeSubsumption(subsumerNode,subsumedNode);
			}

			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isElementEquivalence(const E& element1, const E& element2) {
				N* node1 = getHierarchyNode(element1);
				if (!node1) {
					return false;
				}
				N* node2 = getHierarchyNode(element2);
				if (!node2) {
					return false;
				}
				return node1 == node2 || isNodeSubsumption(node1,node2) && isNodeSubsumption(node2,node1);
			}

			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isElementSatisfiable(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isNodeSatisfiable(node);
			}

			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isElementNotSatisfiable(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isElementNotSatisfiable(node);
			}



			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isTopHierarchyElement(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isTopHierarchyNode(node);
			}

			template<class T, class N, class E>
			bool CAbstractHierarchy<T,N,E>::isBottomHierarchyElement(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isBottomHierarchyNode(node);
			}





		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHY_CPP
