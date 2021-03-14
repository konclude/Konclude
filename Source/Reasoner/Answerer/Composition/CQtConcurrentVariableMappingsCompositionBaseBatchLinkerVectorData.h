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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTORDATA_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTORDATA_H

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
				 *		\class		CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				template<class T>
				class CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData : public CMemoryPoolReallocationIncreasingContext {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData(CMemoryAllocationManager* reallocationMemMan);


						bool hasMoreBatchLinker();
						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>* addBatchLinker(T* linker);
						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>* mergeBatchLinkers(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<T>* vecData);
						T* takeNextBatchLinker();
						T* getNextBatchLinker() const;


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


#include "CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData.cpp"


#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBATCHLINKERVECTORDATA_H
