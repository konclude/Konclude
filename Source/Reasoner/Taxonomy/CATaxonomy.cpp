/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_CPP
#define KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_CPP


#include "CATaxonomy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N, class E>
			CATaxonomy<T,N,E>::CATaxonomy() {
			}



			template<class T, class N, class E>
			bool CATaxonomy<T,N,E>::isElementSubsumption(const E& subsumerElement, const E& subsumedElement) {
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
			bool CATaxonomy<T,N,E>::isElementEquivalence(const E& element1, const E& element2) {
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
			bool CATaxonomy<T,N,E>::isElementSatisfiable(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isNodeSatisfiable(node);
			}

			template<class T, class N, class E>
			bool CATaxonomy<T,N,E>::isElementNotSatisfiable(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isElementNotSatisfiable(node);
			}



			template<class T, class N, class E>
			bool CATaxonomy<T,N,E>::isTopHierarchyElement(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isTopHierarchyNode(node);
			}

			template<class T, class N, class E>
			bool CATaxonomy<T,N,E>::isBottomHierarchyElement(const E& element) {
				N* node = getHierarchyNode(element);
				if (!node) {
					return false;
				}
				return isBottomHierarchyNode(node);
			}





		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CATAXONOMY_CPP
