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

#ifndef KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBINDINGSCARDINALITYLINKERBATCHLINKER_H
#define KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBINDINGSCARDINALITYLINKERBATCHLINKER_H

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
				 *		\class		CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker : public CLinkerBase<COptimizedComplexVariableIndividualBindingsCardinalityLinker*, CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> {
					// public methods
					public:
						//! Constructor
						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker();


						bool hasMoreLinkers();


						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* setBatchBindingsCardinalityLinkers(COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityStartLinker, COptimizedComplexVariableIndividualBindingsCardinalityLinker* batchBindingsCardinalityEndLinker);
						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* setBatchCardinalityUpdateLinkers(COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateStartLinker, COptimizedComplexVariableIndividualUpdateCardinalityLinker* batchCardinalityUpdateEndLinker);

						COptimizedComplexVariableIndividualBindingsCardinalityLinker* getNextBatchBindingsCardinalityLinker(bool moveNext = false);

						COptimizedComplexVariableIndividualUpdateCardinalityLinker* getNextCardinalityUpdateLinker(bool moveNext = false);

						bool isInserting();
						bool isLeftSide();

						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* setInserting(bool inserting);
						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* setLeftSide(bool leftSide);


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

						bool mInserting;
						bool mLeftSide;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COMPOSITION_CQTCONCURRENTVARIABLEMAPPINGSCOMPOSITIONJOINBINDINGSCARDINALITYLINKERBATCHLINKER_H
