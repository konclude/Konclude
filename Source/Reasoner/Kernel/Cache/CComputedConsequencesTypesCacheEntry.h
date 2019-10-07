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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CComputedConsequencesTypesCacheEntry_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CComputedConsequencesTypesCacheEntry_H

// Libraries includes


// Namespace includes
#include "CacheSettings.h"
#include "CComputedConsequencesCacheContext.h"
#include "CComputedConsequencesCacheEntry.h"


// Other includes
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CComputedConsequencesCachingData.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Cache {

				/*! 
				 *
				 *		\class		CComputedConsequencesTypesCacheEntry
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CComputedConsequencesTypesCacheEntry : public CComputedConsequencesCacheEntry, public CComputedConsequencesCachingData {
					// public methods
					public:
						//! Constructor
						CComputedConsequencesTypesCacheEntry(CComputedConsequencesCacheContext* context);

						CComputedConsequencesTypesCacheEntry* initCacheEntry(CIndividual* individual);

						bool hasConcept(CConcept* concept, bool negation);

						CSortedNegLinker<CConcept*>* getConceptLinker();
						CComputedConsequencesTypesCacheEntry* addConceptLinker(CSortedNegLinker<CConcept*>* conceptLinker);

					// protected methods
					protected:

					// protected variables
					protected:
						CIndividual* mIndividual;
						CSortedNegLinker<CConcept*>* mConceptLinker;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CComputedConsequencesTypesCacheEntry_H
