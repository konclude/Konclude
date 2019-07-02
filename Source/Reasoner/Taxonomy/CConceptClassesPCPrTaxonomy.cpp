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

#ifndef KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRTAXONOMY_CPP
#define KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRTAXONOMY_CPP


#include "CConceptClassesPCPrTaxonomy.h"


namespace Konclude {

	namespace Reasoner {

		namespace Taxonomy {


			template<class T, class N>
			CConceptClassesPCPrTaxonomy<T,N>::CConceptClassesPCPrTaxonomy(N* topHierarchyNode, N* bottomHierarchyNode) : CPCPrTaxonomy<T,N,CConcept*>(topHierarchyNode,bottomHierarchyNode) {
			}

			template<class T, class N>
			CConceptClassesPCPrTaxonomy<T,N>::CConceptClassesPCPrTaxonomy(CConcept* topConcept, CConcept* bottomConcept) : CPCPrTaxonomy<T,N,CConcept*>(topConcept,bottomConcept) {
			}

		}; // end namespace Taxonomy

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TAXONOMY_CCONCEPTCLASSESPCPRTAXONOMY_CPP
