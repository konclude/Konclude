/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASHPART_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASHPART_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableIndividualBindings.h"
#include "COptimizedComplexVariableIndividualBindingsHasher.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"
#include "COptimizedComplexVariableIndividualUpdateCardinalityLinker.h"
#include "COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker.h"


// Other includes
#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"



// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {



			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualMappingsMultiHashPart
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualMappingsMultiHashPart : public CMemoryPoolNewAllocationIncreasingContext {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualMappingsMultiHashPart(cint64 bindingSize);
					virtual ~COptimizedComplexVariableIndividualMappingsMultiHashPart();

					cint64 getBindingCount();

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getLastAddedBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualMappingsMultiHashPart* addLastAddedBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);


					COptimizedComplexVariableIndividualMappingsMultiHashPart* addLastUpdatedCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker);
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* getLastUpdatedCardinalityLinker();



					COptimizedComplexVariableIndividualMappingsMultiHashPart* setCurrentUpdateId(cint64 currUpdateId);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* incCurrentUpdateId(cint64 incUpdateCount = 1);
					cint64 getCurrentUpdateId();


					bool addBindingsCardinalitiesCopy(COptimizedComplexVariableIndividualBindings* bindings, COptimizedComplexVariableIndividualBindingsCardinality* addedCardinalites);

					bool addInsertingBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, cint64 hashValue, bool addCardinalitiesIfAlreadyPresent);
					bool addInsertingBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker, bool addCardinalitiesIfAlreadyPresent);


					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getFirstBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getLastBindingsCardinalityLinker();
					COptimizedComplexVariableIndividualMappingsMultiHashPart* setLastBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* clearBindingsCardinalityLinkers();

					COptimizedComplexVariableIndividualUpdateCardinalityLinker* getFirstUpdateCardinalityLinker();
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* getLastUpdateCardinalityLinker();
					COptimizedComplexVariableIndividualMappingsMultiHashPart* setLastUpdateCardinalityLinker(COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* clearUpdateCardinalityLinkers();


					COptimizedComplexVariableIndividualMappingsMultiHashPart* removeBindingsCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);

					cint64 getLastUpdatedBindingCount(bool update = true);



					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* getLastAddedBindingsCardinalityBatchLinker();
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* getFirstAddedBindingsCardinalityBatchLinker();
					COptimizedComplexVariableIndividualMappingsMultiHashPart* clearBindingsCardinalityBatchLinker();



					cint64 mStatInsertionCount = 0;
					cint64 mStatBindingsCopyCount = 0;
					cint64 mStatCardinalitiesCopyCount = 0;

					cint64 mStatsProcessingTime = 0;

					cint64 getMaximumCardinalitySameIndividualsJointlyConsidered();
					cint64 getMaximumCardinalitySameIndividualsSeparatelyConsidered();


				// protected methods
				protected:
					COptimizedComplexVariableIndividualMappingsMultiHashPart* addCreatingUpdateCardinalityLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsLinker, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalities);


					COptimizedComplexVariableIndividualBindingsCardinality* getBindingsCardinalityCopy(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* releaseBindingsCardinalityCopy(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities);

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* releaseBindingsCardinalityLinkerCopy(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);

					COptimizedComplexVariableIndividualBindings* getBindingsCopy(COptimizedComplexVariableIndividualBindings* bindings);
					COptimizedComplexVariableIndividualMappingsMultiHashPart* releaseBindingsCopy(COptimizedComplexVariableIndividualBindings* bindings);


					COptimizedComplexVariableIndividualMappingsMultiHashPart* extendAddedBindingsCardinalityBatchLinker(COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker);

				// protected variables
				protected:
					cint64 mBindingCount;
					cint64 mLastUpdatedBindingCount;
					cint64 mBindingSize;


					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mFreeBindCardLinkerCopy;
					COptimizedComplexVariableIndividualBindingsCardinality* mFreeCardCopy;
					COptimizedComplexVariableIndividualBindings* mFreeBindingsCopy;


					CQtManagedRestrictedModificationHash<COptimizedComplexVariableIndividualBindingsHasher, COptimizedComplexVariableIndividualBindingsCardinalityLinker*> mMappingCardinalityHash;


					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mLastAddedBindingsCardinalityLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mFirstAddedBindingsCardinalityLinker;

					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mLastUpdateCardinalityLinker;
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mFirstUpdateCardinalityLinker;
					cint64 mCurrentUpdateId;



					cint64 mCurrentLinkerBatchingSize;
					double mLinkerBatchingSizeIncreasingFactor;

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mLastAddedBindingsCardinalityBatchLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mFirstAddedBindingsCardinalityBatchLinker;
					cint64 mLastAddedBindingsCardinalityBatchLinkerUpdateId;

					cint64 mMaximumCardinalitySameIndividualsJointlyConsidered;
					cint64 mMaximumCardinalitySameIndividualsSeparatelyConsidered;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALMAPPINGSMULTIHASHPART_H
