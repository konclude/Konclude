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

#include "CRoleDependence.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			CRoleDependence::CRoleDependence() {
				subRoleLinker = 0;
				indirectSubRoleLinker = 0;
				rolDepTag = 0;
				conceptDepLinker = 0;
				indiDepsLinker = 0;
				indirectChainedSubRoleLinker = 0;
			}


			CRoleDependence::~CRoleDependence() {
			}


			CRoleDependence *CRoleDependence::init(CRole *role) {
				subRoleLinker = 0;
				indirectSubRoleLinker = 0;
				rolDepTag = role->getRoleTag();
				conceptDepLinker = 0;
				return this;
			}


			CRoleDependence *CRoleDependence::init(qint64 roleTag, CRoleDependence *roleDep, CMemoryManager<CSortedNegLinker<qint64> > *linkerMemMan) {
				rolDepTag = roleTag;
				subRoleLinker = 0;
				indirectSubRoleLinker = 0;
				conceptDepLinker = 0;
				indirectChainedSubRoleLinker = 0;

				if (linkerMemMan) {
					CSortedNegLinker<qint64> *linkIt = 0;
					if (roleDep) {
						linkIt = roleDep->subRoleLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *roleLink = linkerMemMan->allocate();
						roleLink->init(linkIt->getData(),linkIt->isNegated());
						addSubRoleLinker(roleLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}

					linkIt = 0;
					if (roleDep) {
						linkIt = roleDep->conceptDepLinker;
					}
					while (linkIt) {
						CSortedNegLinker<qint64> *conLink = linkerMemMan->allocate();
						conLink->init(linkIt->getData(),linkIt->isNegated());
						addConceptDependenceLinker(conLink);
						linkIt = (CSortedNegLinker<qint64> *)linkIt->getNext();
					}

					linkIt = 0;
					if (roleDep) {
						linkIt = roleDep->indiDepsLinker;
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



			CRoleDependence *CRoleDependence::setRole(CRole *role) {
				rolDepTag = role->getRoleTag();
				return this;
			}

			qint64 CRoleDependence::getRoleTag() {
				return rolDepTag;
			}




			CRoleDependence *CRoleDependence::setSubRoleList(CSortedNegLinker<qint64> *depSubRoleList) {
				subRoleLinker = depSubRoleList;
				return this;
			}


			CRoleDependence *CRoleDependence::addSubRoleLinker(CSortedNegLinker<qint64> *depSubRoleLinker) {
				if (subRoleLinker) {
					subRoleLinker = subRoleLinker->insertSortedNextSorted(depSubRoleLinker);
				} else {
					subRoleLinker = depSubRoleLinker;
				}
				return this;
			}


			CSortedNegLinker<qint64> *CRoleDependence::getSubRoleList() {
				return subRoleLinker;
			}




			CRoleDependence *CRoleDependence::setIndirectSubRoleList(CSortedNegLinker<qint64> *indirectDepSubRoleList) {
				indirectSubRoleLinker = indirectDepSubRoleList;
				return this;
			}

			CRoleDependence *CRoleDependence::addIndirectSubRoleLinker(CSortedNegLinker<qint64> *indirectDepSubRoleLinker) {
				if (indirectSubRoleLinker) {
					indirectSubRoleLinker = indirectSubRoleLinker->insertSortedNextSorted(indirectDepSubRoleLinker);
				} else {
					indirectSubRoleLinker = indirectDepSubRoleLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CRoleDependence::getIndirectSubRoleList() {
				return indirectSubRoleLinker;
			}



			CRoleDependence *CRoleDependence::setConceptDependenceList(CSortedNegLinker<qint64> *conceptDependenceList) {
				conceptDepLinker = conceptDependenceList;
				return this;
			}

			CRoleDependence *CRoleDependence::addConceptDependenceLinker(CSortedNegLinker<qint64> *conceptDependenceLinker) {
				if (conceptDepLinker) {
					conceptDepLinker = conceptDepLinker->insertSortedNextSorted(conceptDependenceLinker);
				} else {
					conceptDepLinker = conceptDependenceLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CRoleDependence::getConceptDependenceList() {
				return conceptDepLinker;
			}



			CRoleDependence *CRoleDependence::setIndividualDependenceList(CSortedNegLinker<qint64> *indiList) {
				indiDepsLinker = indiList;
				return this;
			}

			CRoleDependence *CRoleDependence::addIndividualDependenceLinker(CSortedNegLinker<qint64> *indiLinker) {
				if (indiDepsLinker) {
					indiDepsLinker = indiDepsLinker->insertSortedNextSorted(indiLinker);
				} else {
					indiDepsLinker = indiLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CRoleDependence::getIndividualDependenceList() {
				return indiDepsLinker;
			}


			CRoleDependence *CRoleDependence::setIndirectChainedSubRoleList(CSortedNegLinker<qint64> *indiList) {
				indirectChainedSubRoleLinker = indiList;
				return this;
			}

			CRoleDependence *CRoleDependence::addIndirectChainedSubRoleLinker(CSortedNegLinker<qint64> *indiLinker) {
				if (indirectChainedSubRoleLinker) {
					indirectChainedSubRoleLinker = indirectChainedSubRoleLinker->insertSortedNextSorted(indiLinker);
				} else {
					indirectChainedSubRoleLinker = indiLinker;
				}
				return this;
			}

			CSortedNegLinker<qint64> *CRoleDependence::getIndirectChainedSubRoleList() {
				return indirectChainedSubRoleLinker;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
