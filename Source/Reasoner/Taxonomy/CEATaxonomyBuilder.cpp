/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_CPP
#define KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_CPP


#include "CEATaxonomyBuilder.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N, class E>
			CEATaxonomyBuilder<T,N,E>::CEATaxonomyBuilder() {
			}

			template<class T, class N, class E>
			T* CEATaxonomyBuilder<T,N,E>::setElementNotSubsumption(const E& notSubsumerElement, const E& notSubsumedElement) {
				N* notSubsumerNode = getHierarchyNode(notSubsumerElement,true);
				N* notSubsumedNode = getHierarchyNode(notSubsumedElement,true);
				return setNodeNotSubsumption(notSubsumerNode,notSubsumedNode);
			}

			template<class T, class N, class E>
			T* CEATaxonomyBuilder<T,N,E>::setElementDisjoints(const E& disjointElement1, const E& disjointElement2) {
				N* dis1Node = getHierarchyNode(disjointElement1,true);
				N* dis2Node = getHierarchyNode(disjointElement2,true);
				return setNodeDisjoints(dis1Node,dis2Node);
			}


		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CEATAXONOMYBUILDER_CPP
