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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTOR_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTOR_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"
#include "CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData.h"


// Other includes

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
				 *		\class		CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				template<class T>
				class CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector : public QVector<CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData<T>*>, public CMemoryPoolNewAllocationIncreasingContext {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector(cint64 size);
						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector();


						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVectorData<T>* getVectorLinkerData(cint64 index, bool createIfNotExists = true);


						CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector* addBatchLinker(cint64 index, T* linker);


						T* createBatchLinker();

						cint64 getNextIndex(bool moveNext = true);

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mNextIndex;
						cint64 mSize;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#include "CQtConcurrentVariableMappingsCompositionJoinBatchLinkerVector.cpp"

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBATCHLINKERVECTOR_H
