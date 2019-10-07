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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGHASHMEMORYMANAGED_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGHASHMEMORYMANAGED_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableJoiningHasher.h"
#include "COptimizedComplexVariableJoiningData.h"


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
			class COptimizedComplexVariableJoiningHashMemoryManaged : public CMemoryPoolNewAllocationIncreasingContext, public CQtManagedRestrictedModificationHash<COptimizedComplexVariableJoiningHasher, COptimizedComplexVariableJoiningData> {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableJoiningHashMemoryManaged(CXLinker<cint64>* leftKeyBindingLinker, CXLinker<cint64>* rightKeyBindingLinker);

					CXLinker<cint64>* getLeftKeyBindingLinker();
					CXLinker<cint64>* getRightKeyBindingLinker();
					

					COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* createDataLinker();


					cint64 mJoinedCount = 0;
					cint64 mLeftAdded = 0;
					cint64 mRightAdded = 0;

					cint64 mInsertionCount = 0;
					cint64 mCheckingCount = 0;

					cint64 mProcessingTime = 0;

				// protected methods
				protected:

				// protected variables
				protected:
					CXLinker<cint64>* mLeftKeyBindingLinker;
					CXLinker<cint64>* mRightKeyBindingLinker;



				// private methods
				private:

				// private variables
				private:

			};


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEJOININGHASHMEMORYMANAGED_H
