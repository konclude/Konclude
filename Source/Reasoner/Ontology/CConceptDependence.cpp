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

#include "CConceptDependence.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CConceptDependence::CConceptDependence() {
				conceptDepTag = 0;
				conceptDepsLinker = 0;
				roleDepsLinker = 0;
				indiDepsLinker = 0;
			}


			CConceptDependence::~CConceptDependence() {
			}


			CConceptDependence *CConceptDependence::init(qint64 conTag, CConceptDependence *conceptDep, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan) {
				conceptDepTag = conTag;
				conceptDepsLinker = 0;
				if (linkerMemMan) {
					CSortedNegLinker<qint64> *linkIt = 0;
					if (conceptDep) {
						linkIt = conceptDep->conceptDepsLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *conLink = linkerMemMan->allocate();
						conLink->init(linkIt->getData(),linkIt->isNegated());
						addConceptDependenceLinker(conLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}

					linkIt = 0;
					if (conceptDep) {
						linkIt = conceptDep->roleDepsLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *roleLink = linkerMemMan->allocate();
						roleLink->init(linkIt->getData(),linkIt->isNegated());
						addRoleDependenceLinker(roleLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}

					linkIt = 0;
					if (conceptDep) {
						linkIt = conceptDep->indiDepsLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *indiLink = linkerMemMan->allocate();
						indiLink->init(linkIt->getData(),linkIt->isNegated());
						addIndividualDependenceLinker(indiLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}
				}
				return this;
			}

			CConceptDependence *CConceptDependence::init(CConcept *concept) {
				conceptDepTag = concept->getConceptTag();
				conceptDepsLinker = 0;
				roleDepsLinker = 0;
				indiDepsLinker = 0;
				return this;
			}

			CConceptDependence *CConceptDependence::setConceptDependenceList(CSortedNegLinker<qint64> *conceptList) {
				conceptDepsLinker = conceptList;
				return this;
			}

			CConceptDependence *CConceptDependence::addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptLinker) {
				if (conceptDepsLinker) {
					conceptDepsLinker = conceptDepsLinker->insertSortedNextSorted(conceptLinker);
				} else {
					conceptDepsLinker = conceptLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CConceptDependence::getConceptDependenceList() {
				return conceptDepsLinker;
			}


			CConceptDependence *CConceptDependence::setRoleDependenceList(CSortedNegLinker<qint64> *roleList) {
				roleDepsLinker = roleList;
				return this;
			}

			CConceptDependence *CConceptDependence::addRoleDependenceLinker(CSortedNegLinker<qint64> *roleLinker) {
				if (roleDepsLinker) {
					roleDepsLinker = roleDepsLinker->insertSortedNextSorted(roleLinker);
				} else {
					roleDepsLinker = roleLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CConceptDependence::getRoleDependenceList() {
				return roleDepsLinker;
			}

			qint64 CConceptDependence::getConceptDependenceTag() {
				return conceptDepTag;
			}

			CConceptDependence *CConceptDependence::getConceptDependenceTag(qint64 tag) {
				conceptDepTag = tag;
				return this;
			}


			CConceptDependence *CConceptDependence::setIndividualDependenceList(CSortedNegLinker<qint64> *indiList) {
				indiDepsLinker = indiList;
				return this;
			}

			CConceptDependence *CConceptDependence::addIndividualDependenceLinker(CSortedNegLinker<qint64> *indiLinker) {
				if (indiDepsLinker) {
					indiDepsLinker = indiDepsLinker->insertSortedNextSorted(indiLinker);
				} else {
					indiDepsLinker = indiLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CConceptDependence::getIndividualDependenceList() {
				return indiDepsLinker;
			}

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
