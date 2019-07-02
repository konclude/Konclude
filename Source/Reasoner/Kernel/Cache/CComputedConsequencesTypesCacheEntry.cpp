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

#include "CComputedConsequencesTypesCacheEntry.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				CComputedConsequencesTypesCacheEntry::CComputedConsequencesTypesCacheEntry(CComputedConsequencesCacheContext* context) : CComputedConsequencesCacheEntry(context) {
				}


				CComputedConsequencesTypesCacheEntry* CComputedConsequencesTypesCacheEntry::initCacheEntry(CIndividual* individual) {
					mIndividual = individual;
					mConceptLinker = nullptr;
					return this;
				}

				bool CComputedConsequencesTypesCacheEntry::hasConcept(CConcept* concept, bool negation) {
					for (CSortedNegLinker<CConcept*>* conIt = mConceptLinker; conIt; conIt = conIt->getNext()) {
						if (conIt->getData() == concept && conIt->isNegated() == negation) {
							return true;
						}
					}
					return false;
				}

				CSortedNegLinker<CConcept*>* CComputedConsequencesTypesCacheEntry::getConceptLinker() {
					return mConceptLinker;
				}

				CComputedConsequencesTypesCacheEntry* CComputedConsequencesTypesCacheEntry::addConceptLinker(CSortedNegLinker<CConcept*>* conceptLinker) {
					mConceptLinker = conceptLinker->append(mConceptLinker);
					return this;
				}

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
