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

#include "CIndividualDependence.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CIndividualDependence::CIndividualDependence() {
				indiDepTag = 0;
				conceptDepsLinker = 0;
			}


			CIndividualDependence::~CIndividualDependence() {
			}


			CIndividualDependence *CIndividualDependence::init(qint64 indiTag, CIndividualDependence *indiDep, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan) {
				indiDepTag = indiTag;
				conceptDepsLinker = 0;
				if (linkerMemMan) {
					CSortedNegLinker<qint64> *linkIt = 0;
					if (indiDep) {
						linkIt = indiDep->conceptDepsLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *conLink = linkerMemMan->allocate();
						conLink->init(linkIt->getData(),linkIt->isNegated());
						addConceptDependenceLinker(conLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}
				}
				return this;
			}

			CIndividualDependence *CIndividualDependence::init(CIndividual *indi) {
				indiDepTag = indi->getIndividualID();
				conceptDepsLinker = 0;
				return this;
			}


			qint64 CIndividualDependence::getIndividualDependenceTag() {
				return indiDepTag;
			}

			CIndividualDependence *CIndividualDependence::setIndividualDependenceTag(qint64 tag) {
				indiDepTag = tag;
				return this;
			}

			CIndividualDependence *CIndividualDependence::setConceptDependenceList(CSortedNegLinker<qint64> *conceptList) {
				conceptDepsLinker = conceptList;
				return this;
			}

			CIndividualDependence *CIndividualDependence::addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptLinker) {
				if (conceptDepsLinker) {
					conceptDepsLinker = conceptDepsLinker->insertSortedNextSorted(conceptLinker);
				} else {
					conceptDepsLinker = conceptLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CIndividualDependence::getConceptDependenceList() {
				return conceptDepsLinker;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
