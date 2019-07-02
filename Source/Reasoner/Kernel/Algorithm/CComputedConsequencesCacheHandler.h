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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPUTEDCONSEQUENCESCACHEHANDLER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPUTEDCONSEQUENCESCACHEHANDLER_H

// Libraries includes


// Namespace includes
#include "AlgorithmSettings.h"
#include "CCalculationAlgorithmContext.h"

// Other includes
#include "Reasoner/Ontology/CIndividual.h"
#include "Reasoner/Ontology/CConcept.h"

#include "Reasoner/Kernel/Cache/CComputedConsequencesCacheReader.h"
#include "Reasoner/Kernel/Cache/CComputedConsequencesCacheWriter.h"
#include "Reasoner/Kernel/Cache/CComputedConsequencesCacheEntry.h"
#include "Reasoner/Kernel/Cache/CComputedConsequencesCacheWriteData.h"

#include "Context/CContextBase.h"

#include "Reasoner/Kernel/Task/CConsistenceTaskData.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			using namespace Cache;
			using namespace Task;

			namespace Algorithm {

				/*! 
				 *
				 *		\class		CComputedConsequencesCacheHandler
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CComputedConsequencesCacheHandler {
					// public methods
					public:
						//! Constructor
						CComputedConsequencesCacheHandler(CComputedConsequencesCacheReader* satCacheReader, CComputedConsequencesCacheWriter* satCacheWriter);

						bool tryCacheTypeConcept(CIndividual* individual, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);
						CSortedNegLinker<CConcept*>* getCachedTypesConceptLinker(CIndividual* individual, CCalculationAlgorithmContext* calcAlgContext);


						bool commitCacheMessages(CCalculationAlgorithmContext* calcAlgContext);

					// protected methods
					protected:
						bool prepareCacheMessages(CCalculationAlgorithmContext* calcAlgContext);
						bool addCacheMessages(CComputedConsequencesCacheWriteData* expandWriteData, CCalculationAlgorithmContext* calcAlgContext);

						bool canCacheTypeConcept(CIndividual* individual, CConcept* concept, bool negation, CCalculationAlgorithmContext* calcAlgContext);

					// protected variables
					protected:
						CComputedConsequencesCacheReader* mSatCacheReader;
						CComputedConsequencesCacheWriter* mSatCacheWriter;

						CComputedConsequencesCacheWriteData* mWriteData;
						CTaskMemoryPoolAllocationManager* mMemAllocMan;
						CMemoryPoolContainer mMemPoolCon;
						CContextBase* mTmpContext;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPUTEDCONSEQUENCESCACHEHANDLER_H
