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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALRESOLVINGHASHMEMORYMANAGED_H
#define KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALRESOLVINGHASHMEMORYMANAGED_H

// Libraries includes


// Namespace includes
#include "TriplesSettings.h"
#include "CRedlandNodeHasher.h"
#include "CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"

#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CObjectAllocator.h"


#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"

#include "Reasoner/Ontology/CIndividual.h"

#include "Reasoner/Preprocess/CDataLiteralNormalizerPreProcess.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Preprocess;

		namespace Triples {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItemVariableExpressionBinding
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRedlandStoredTriplesIndividualResolvingHashMemoryManaged : public CMemoryPoolNewAllocationIncreasingContext, public CQtManagedRestrictedModificationHash<CRedlandNodeHasher, CIndividual*> {
				// public methods
				public:
					//! Constructor
					CRedlandStoredTriplesIndividualResolvingHashMemoryManaged(CConcreteOntology* ontology, CConfiguration* configuration, const QString& anonymousOntologyIdentifier);

					CIndividual*& getIndividual(librdf_node* node);
					CIndividual*& getIndividual(const CRedlandNodeHasher& indiNode);
					CRedlandStoredTriplesIndividualResolvingHashMemoryManaged* insertIndividual(const CRedlandNodeHasher& indiNode, CIndividual* indi);

					CConceptAssertionLinker* createAssertionConceptLinker();
					CRoleAssertionLinker* createAssertionRoleLinker();
					CDataAssertionLinker* createAssertionDataLinker();
					CReverseRoleAssertionLinker* createReverseAssertionRoleLinker();

					CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext* getIndividualDataMemoryManager();
					CDataLiteralNormalizerPreProcess* getIndividualDataLiteralNormalizer();

					CMemoryPool* takeIndividualDataMemoryPools();

					cint64 getConceptAssertionCount();
					cint64 getObjectPropertyAssertionCount();
					cint64 getDataPropertyAssertionCount();


				// protected methods
				protected:

				// protected variables
				protected:
					QString mAnonymousOntologyIdentifier;
					CDataLiteralNormalizerPreProcess mIndiDataLiteralNormalizer;
					CRedlandStoredTriplesIndividualPreProcessMemoryManagingContext mIndiDataMemAllocManager;
					CPreProcessContext* mPreprocessContext;

					cint64 mConceptAssertionCount = 0;
					cint64 mObjectPropertyAssertionCount = 0;
					cint64 mDataPropertyAssertionCount = 0;


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Triples

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_TRIPLES_CREDLANDSTOREDTRIPLESINDIVIDUALRESOLVINGHASHMEMORYMANAGED_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
