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

#ifndef KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_CPP
#define KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_CPP


#include "CAbstractHierarchyBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N, class E>
			CAbstractHierarchyBuilder<T,N,E>::CAbstractHierarchyBuilder() {
			}

			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setElementSatisfiable(const E& element, bool satisfiable) {
				if (satisfiable) {
					return setElementSatisfiable(element);
				} else {
					return setElementUnsatisfiable(element);
				}
			}


			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setElementSatisfiable(const E& element) {
				N* node = getHierarchyNode(element,true);
				return setNodeSatisfiable(node);
			}


			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setElementUnsatisfiable(const E& element) {
				N* node = getHierarchyNode(element,true);
				return setNodeUnsatisfiable(node);
			}


			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setElementSubsumption(const E& subsumerElement, const E& subsumedElement) {
				N* subsumerNode = getHierarchyNode(subsumerElement,true);
				N* subsumedNode = getHierarchyNode(subsumedElement,true);
				return setNodeSubsumption(subsumerNode,subsumedNode);
			}


			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setElementEquivalence(const E& element1, const E& element2) {
				N* eqNode1 = getHierarchyNode(element1,true);
				N* eqNode2 = getHierarchyNode(element2,true);
				return setNodeEquivalence(eqNode1,eqNode2);
			}

			template<class T, class N, class E>
			T* CAbstractHierarchyBuilder<T,N,E>::setNodeSatisfiable(N* node, bool satisfiable) {
				if (satisfiable) {
					return setNodeSatisfiable(node);
				} else {
					return setNodeUnsatisfiable(node);
				}
			}


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CABSTRACTHIERARCHYBUILDER_CPP
