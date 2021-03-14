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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBINDINGSCARDINALITYLINKERBATCHLINKER_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBINDINGSCARDINALITYLINKERBATCHLINKER_H

// Libraries includes


// Namespace includes
#include "CompositionSettings.h"


// Other includes
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualBindingsCardinalityLinker.h"
#include "Reasoner/Answerer/COptimizedComplexVariableIndividualUpdateCardinalityLinker.h"


#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {


				/*! 
				 *
				 *		\class		CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker : public CLinkerBase<COptimizedComplexVariableIndividualBindingsCardinalityLinker*, CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker> {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker();


						bool hasMoreLinkers();


						CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker* setBatchBindingsCardinalityLinkers(COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityStartLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityEndLinker);
						CQtConcurrentVariableMappingsCompositionBaseBindingsCardinalityLinkerBatchLinker* setBatchCardinalityUpdateLinkers(COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateStartLinker, COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateEndLinker);

						COptimizedComplexVariableIndividualBindingsCardinalityLinker* getNextBatchBindingsCardinalityLinker(bool moveNext = false);

						COptimizedComplexVariableIndividualUpdateCardinalityLinker* getNextCardinalityUpdateLinker(bool moveNext = false);



					// protected methods
					protected:

					// protected variables
					protected:
						//COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchBindingsCardinalityCurrentLinker;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchBindingsCardinalityStartLinker;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchBindingsCardinalityEndLinker;

						COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchCardinalityUpdateCurrentLinker;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchCardinalityUpdateStartLinker;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchCardinalityUpdateEndLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONBASEBINDINGSCARDINALITYLINKERBATCHLINKER_H
