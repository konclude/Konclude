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

#include "COWLlinkQueryExtensionParser.h"


namespace Konclude {

	namespace Parser {



		COWLlinkQueryExtensionParser::COWLlinkQueryExtensionParser() {
		}


		COWLlinkQueryExtensionParser::~COWLlinkQueryExtensionParser() {
		}



		CTaxonomyPremisingQuerySupport *COWLlinkQueryExtensionParser::parseTaxonomyQuerySupport(CConcreteOntology *ontology, QDomElement *queryNode) {
			CTaxonomyPremisingQuerySupport *taxSupport = 0;
			QDomElement calcSupportNode = queryNode->firstChildElement("TaxonomyCalculationSupport");
			if (!calcSupportNode.isNull()) {
				CClassificationCalculationSupport *calcSupp = parseTaxonomyCalculationSupport(ontology,&calcSupportNode);
				if (calcSupp && !taxSupport) {
					taxSupport = new CTaxonomyPremisingQuerySupport(calcSupp);
				}
			}
			return taxSupport;
		}

		CClassificationCalculationSupport *COWLlinkQueryExtensionParser::parseTaxonomyCalculationSupport(CConcreteOntology *ontology, QDomElement *node) {
			CConcreteOntology* conOnto = dynamic_cast<CConcreteOntology*>(ontology);
			CClassificationCalculationSupport *calcSupp = 0;
			COWLlinkQtXMLResultParser restultParser;
			CQueryResult *queryResult = restultParser.parseQueryResult(node);
			if (queryResult) {
				CClassHierarchyResult *classHierarchyResult = dynamic_cast<CClassHierarchyResult *>(queryResult);
				if (classHierarchyResult) {
					QString completeTaxAttString = node->attribute("completeness");
					bool x = node->hasAttribute("completeness");
					QString name = node->tagName();
					bool completeTax = completeTaxAttString.toUpper() == "ALLEXISTINGSUBSUMPTIONSLISTED";
					calcSupp = new CClassificationCalculationSupport();
					CPartialPruningTaxonomy *tax = new CPartialPruningTaxonomy(conOnto->getDataBoxes()->getTBox()->getTopConcept(),conOnto->getDataBoxes()->getTBox()->getBottomConcept());
					classHierarchyResult->fillTaxonomy(ontology,tax);
					if (completeTax) {
						tax->completeUnknownAsNonSubsumptions();
					}
					CTaxonomySatisfiableSubsumptionCalculationSponsor *satisSubsumSponsor = new CTaxonomySatisfiableSubsumptionCalculationSponsor(tax);
					calcSupp->setSubsumptionCalculationSponsor(satisSubsumSponsor);
					calcSupp->useSatisfiableCalculationSponsor(satisSubsumSponsor);
				}
			}
			return calcSupp;
		}



	}; // end namespace Parser

}; // end namespace Konclude
