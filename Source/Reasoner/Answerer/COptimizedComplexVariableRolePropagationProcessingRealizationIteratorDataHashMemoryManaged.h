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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAHASHMEMORYMANAGED_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAHASHMEMORYMANAGED_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData.h"
#include "COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"

#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CObjectAllocator.h"


#include "Utilities/Container/CQtManagedRestrictedModificationHash.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;

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
			class COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged : public CMemoryPoolNewAllocationIncreasingContext, public CQtManagedRestrictedModificationHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*> {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged();

					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* getRolePropagationProcessingRealizationIteratorData(CRealizationIndividualInstanceItemReference instItemRef);

					cint64 mProcessingTime = 0;
					cint64 mPropagatedCount = 0;


					cint64 getFillerInstanceItemCount();
					cint64 getPropagationInstanceItemCount();
					cint64 getRealizationFinishedFillerInstanceItemCount();
					cint64 getPropagationHandledInstanceItemCount();
					cint64 getPropagatedInstanceItemCount();



					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* setFillerInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* setRealizationFinishedFillerInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* setPropagationHandledInstanceItemCount(cint64 count);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* setPropagatedInstanceItemCount(cint64 count);


					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* incFillerInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* incRealizationFinishedFillerInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* incPropagationHandledInstanceItemCount(cint64 count = 1);
					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* incPropagatedInstanceItemCount(cint64 count = 1);


					COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* incExpectedFillerAllPropagationItemCount(double count = 1);
					double getExpectedFillerAllPropagationItemCount();
					double getExpectedFillerPerPropagationItemCount();


				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mFillerInstanceItemCount;

					cint64 mRealizationFinishedFillerInstanceItemCount;
					cint64 mPropagationHandledInstanceItemCount;

					double mExpectedFillerAllPropagationItemCount;

					cint64 mPropagatedInstanceItemCount;
						


				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEROLEPROPAGATIONPROCESSINGREALIZATIONITERATORDATAHASHMEMORYMANAGED_H
