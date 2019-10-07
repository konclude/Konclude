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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTORDATA_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTORDATA_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"


// Other includes

#include "Utilities/Memory/CMemoryPoolContainer.h"
#include "Utilities/Memory/CMemoryPoolContainerAllocationManager.h"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"
#include "Context/CMemoryPoolNewAllocationContext.h"
#include "Context/CMemoryPoolReallocationIncreasingContext.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				template<class T>
				class CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData : public CMemoryPoolReallocationIncreasingContext {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData(CMemoryAllocationManager* reallocationMemMan);


						bool hasMoreBatchLinker();
						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData<T>* addBatchLinker(T* linker);
						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData<T>* mergeBatchLinkers(CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData<T>* vecData);
						T* takeNextBatchLinker();
						T* getNextBatchLinker();


						T* createBatchLinker();

					// protected methods
					protected:

					// protected variables
					protected:
						T* mBatchLinker;
						T* mLastBatchLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude


#include "CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData.cpp"


#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTORDATA_H
