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

#ifndef KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_CPP
#define KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_CPP


#include "CEATaxonomy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N, class E>
			CEATaxonomy<T,N,E>::CEATaxonomy() {
			}


			template<class T, class N, class E>
			bool CEATaxonomy<T,N,E>::isElementDisjoint(const E& disjointElement1, const E& disjointElement2) {
				N* node1 = getHierarchyNode(disjointElement1);
				if (!node1) {
					return false;
				}
				N* node2 = getHierarchyNode(disjointElement2);
				if (!node2) {
					return false;
				}
				return isNodeDisjoint(node1,node2);
			}

			template<class T, class N, class E>
			bool CEATaxonomy<T,N,E>::isElementNotDisjoint(const E& disjointElement1, const E& disjointElement2) {
				N* node1 = getHierarchyNode(disjointElement1);
				if (!node1) {
					return false;
				}
				N* node2 = getHierarchyNode(disjointElement2);
				if (!node2) {
					return false;
				}
				return isNodeNotDisjoint(node1,node2);
			}


			template<class T, class N, class E>
			bool CEATaxonomy<T,N,E>::isElementNotSubsumption(const E& notSubsumerElement, const E& notSubsumedElement) {
				N* subsumerNode = getHierarchyNode(notSubsumerElement);
				if (!subsumerNode) {
					return false;
				}
				N* subsumedNode = getHierarchyNode(notSubsumedElement);
				if (!subsumedNode) {
					return false;
				}
				return isNodeNotSubsumption(subsumerNode,subsumedNode);
			}

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CEATAXONOMY_CPP
