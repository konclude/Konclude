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

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALPREPROCESSMEMORYMANAGINGCONTEXT_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALPREPROCESSMEMORYMANAGINGCONTEXT_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"


// Other includes
#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"

#include "Reasoner/Preprocess/CPreProcessContext.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Preprocess;

		namespace Triples {



			/*! 
			 *
			 *		\class		CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext : public CPreProcessContext {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext(CConcreteOntology* ontology, CConfiguration* configuration);

					virtual CConfiguration* getConfiguration();
					virtual CConcreteOntology* getOntology();
					virtual CMemoryAllocationManager* getMemoryAllocationManager();

					virtual CMemoryPool* takeMemoryPools();


				// protected methods
				protected:

				// protected variables
				protected:
					CMemoryPoolNewAllocationIncreasingContext mMemManContext;
					CConcreteOntology* mOntology;
					CConfiguration* mConfiguration;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALPREPROCESSMEMORYMANAGINGCONTEXT_H
