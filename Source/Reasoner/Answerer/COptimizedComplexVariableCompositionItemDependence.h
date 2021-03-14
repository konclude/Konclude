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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEMDEPENDENCE_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEMDEPENDENCE_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COptimizedComplexVariableCompositionItem.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {


	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableCompositionItemDependence
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableCompositionItemDependence {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableCompositionItemDependence(COptimizedComplexVariableCompositionItem* baseItem);

					virtual ~COptimizedComplexVariableCompositionItemDependence();

					
					COptimizedComplexVariableCompositionItem* getDependentItem();

					bool reset();

					COptimizedComplexVariableIndividualBindingsCardinalityLinker* getBatchCurrentBindingsCardinalityLinker(bool moveNext);
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* getBatchCurrentUpdatedCardinalityLinker(bool moveNext);

					cint64 getBatchProcessedBindingsCardinalityLinkerCount();
					cint64 getTotalProcessedBindingsCardinalityLinkerCount();

					bool isBatchProcessed(bool considerCardinalityUpdates = true);
					bool loadNextBatch(bool considerCardinalityUpdates = true);
					bool loadNextBatchProvidedBatch(bool considerCardinalityUpdates = true);


					bool isProcessingFinished(bool considerCardinalityUpdates = true);


					cint64 getLastRequestedMappingsComputationCount();
					COptimizedComplexVariableCompositionItemDependence* setLastRequestedMappingsComputationCount(cint64 count);
					bool hasMappingsCountComputationRequested(cint64 count);

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* getBatchProvidedBindingCardinalityBatchLinker(bool moveNext);
					bool setBatchProvidedBindingCardinalityLinkersProcessed(COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* batchLinker);



				// protected methods
				protected:

				// protected variables
				protected:
					COptimizedComplexVariableCompositionItem* mDependentItem;

					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mStartLoadedBindingsCardinalityBatchLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mCurrentLoadedBindingsCardinalityBatchLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* mEndLoadedBindingsCardinalityBatchLinker;

					bool mSingleLoadedBindingsCardinalityLinker;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchStartBindingsCardinalityLinker;
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchStartUpdatedCardinalityLinker;


					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchEndBindingsCardinalityLinker;
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchEndUpdatedCardinalityLinker;


					COptimizedComplexVariableIndividualBindingsCardinalityLinker* mBatchCurrentBindingsCardinalityLinker;
					COptimizedComplexVariableIndividualUpdateCardinalityLinker* mBatchCurrentUpdatedCardinalityLinker;

					cint64 mBatchDependentItemMappingsCount;
					cint64 mBatchDependentItemMappingsSize;
					cint64 mBatchProcessedBindingsCardinalityLinkerCount;
					cint64 mTotalProcessedBindingsCardinalityLinkerCount;

					cint64 mLastRequestedMappingsComputationCount;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLECOMPOSITIONITEMDEPENDENCE_H
