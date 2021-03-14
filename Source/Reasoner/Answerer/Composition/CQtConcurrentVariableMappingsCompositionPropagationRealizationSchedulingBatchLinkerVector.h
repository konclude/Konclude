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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"
#include "CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData.h"
#include "CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector.h"


// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged.h"

#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CNewAllocationMemoryPoolProvider.h"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Utilities/UtilitiesSettings.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Context;
	using namespace Utilities;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				template<class T>
				class CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector : public CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<T> {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector(cint64 size);
						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector();



						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector* addRealizationSchedulingIterationData(COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iterationData);

						CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* getFirstRealizationSchedulingIterationDataLinker() const;
						CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* getLastRealizationSchedulingIterationDataLinker() const;


						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector* mergeRealizationSchedulingIterationDataLinkers(const CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<T>& reduceVec);


					// protected methods
					protected:
						 
					// protected variables
					protected:
						CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* mFirstRealizationSchedulingIterationDataLinker;
						CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* mLastRealizationSchedulingIterationDataLinker;


					// private methods
					private:
						
					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector.cpp"

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONPROPAGATIONREALIZATIONSCHEDULINGBATCHLINKERVECTOR_H
