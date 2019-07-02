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

#include "CTaxonomySatisfiableSubsumptionCalculationSponsor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CTaxonomySatisfiableSubsumptionCalculationSponsor::CTaxonomySatisfiableSubsumptionCalculationSponsor(CTaxonomy *takeTaxonomy) {
				tax = takeTaxonomy;
			}


			CTaxonomySatisfiableSubsumptionCalculationSponsor::~CTaxonomySatisfiableSubsumptionCalculationSponsor() {
				if (tax) {
					delete tax;
				}
			}

			bool CTaxonomySatisfiableSubsumptionCalculationSponsor::sponsoredSubsumptionCalculation(CConcreteOntology *ontology, CConcept *subsumerConcept, CConcept *subsumedConcept, CConceptSubsumptionRelationObserver *observer) {
				if (tax) {
					if (tax->hasHierarchyNode(subsumedConcept) && tax->hasHierarchyNode(subsumerConcept)) {
						bool isSubsuming = tax->isSubsumption(subsumerConcept,subsumedConcept);
						if (isSubsuming) {
							observer->tellConceptSupsumptionRelation(ontology,subsumerConcept,subsumedConcept,true);
							return true;
						} else {
							bool isNotSubsuming = tax->isNotSubsumption(subsumerConcept,subsumedConcept);
							if (isNotSubsuming) {
								observer->tellConceptSupsumptionRelation(ontology,subsumerConcept,subsumedConcept,false);
								return true;
							} else {
								return false;
							}
						}
					}
				}
				return false;
			}

			bool CTaxonomySatisfiableSubsumptionCalculationSponsor::sponsoredSatisfiableCalculation(CConcreteOntology *ontology, CConcept *concept, CConceptSatisfiableObserver *observer) {
				if (tax) {
					if (tax->hasHierarchyNode(concept)) {
						bool isSatis = tax->isSatisfiable(concept);
						if (isSatis) {
							observer->tellConceptSatisfiable(ontology,concept,true);
							return true;
						} else {
							bool isNotSatis = tax->isNotSatisfiable(concept);
							if (isNotSatis) {
								observer->tellConceptSatisfiable(ontology,concept,false);
								return true;
							} else {
								return false;
							}
						}
					}
				}
				return false;
			}


			CTaxonomy* CTaxonomySatisfiableSubsumptionCalculationSponsor::getTaxonomy() {
				return tax;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
