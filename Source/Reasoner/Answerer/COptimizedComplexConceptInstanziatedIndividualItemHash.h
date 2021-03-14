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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASH_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASH_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexConceptInstanziatedIndividualItemHasher.h"
#include "COptimizedComplexConceptInstanziatedIndividualItemLinker.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"
#include "Utilities/Container/CLinker.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Container;
	using namespace Context;

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItemVariableExpressionBinding
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexConceptInstanziatedIndividualItemHash {
				// public methods
				public:
					//! Constructor
					COptimizedComplexConceptInstanziatedIndividualItemHash();
					~COptimizedComplexConceptInstanziatedIndividualItemHash();

					COptimizedComplexConceptInstanziatedIndividualItemHash* addRealizationIndividualInstanceItemReference(const CRealizationIndividualInstanceItemReference& itemRef);
					COptimizedComplexConceptInstanziatedIndividualItemLinker* getAddedRealizationIndividualInstanceItemReferenceLinker();
					COptimizedComplexConceptInstanziatedIndividualItemHash* remove(COptimizedComplexConceptInstanziatedIndividualItemLinker* linker);
					
					CMemoryAllocationManager* getInstancesMemoryAllocationManager();
					cint64 getMemoryConsumption();
					cint64 size();
					bool isEmpty();

					CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>* getIndividualInstanceItemReferenceLinkerHash();


				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexConceptInstanziatedIndividualItemLinker* mAddedLinker;
					COptimizedComplexConceptInstanziatedIndividualItemLinker* mTmpLinker;

					CQtManagedRestrictedModificationHash<COptimizedComplexConceptInstanziatedIndividualItemHasher, COptimizedComplexConceptInstanziatedIndividualItemLinker*>* mIndiRefLinkerHash;

					CMemoryPoolNewAllocationIncreasingContext* mMemManContext;

				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCONCEPTINSTANZIATEDINDIVIDUALITEMHASH_H
