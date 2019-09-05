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

#include "COptimizedComplexVariableCompositionItemDependence.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexVariableCompositionItemDependence::COptimizedComplexVariableCompositionItemDependence(COptimizedComplexVariableCompositionItem* baseItem) {
				mDependentItem = baseItem;
			
				mBatchStartBindingsCardinalityLinker = nullptr;
				mBatchStartUpdatedCardinalityLinker = nullptr;
				mBatchEndBindingsCardinalityLinker = nullptr;
				mBatchEndUpdatedCardinalityLinker = nullptr;
				mBatchCurrentBindingsCardinalityLinker = nullptr;
				mBatchCurrentUpdatedCardinalityLinker = nullptr;
				mBatchProcessedBindingsCardinalityLinkerCount = 0;
				mTotalProcessedBindingsCardinalityLinkerCount = 0;
				mBatchDependentItemMappingsCount = 0;
				mBatchDependentItemMappingsSize = 0;
				mLastRequestedMappingsComputationCount = 0;
			}


			COptimizedComplexVariableCompositionItemDependence::~COptimizedComplexVariableCompositionItemDependence() {
			}




			COptimizedComplexVariableCompositionItem* COptimizedComplexVariableCompositionItemDependence::getDependentItem() {
				return mDependentItem;
			}



			COptimizedComplexVariableIndividualBindingsCardinalityLinker* COptimizedComplexVariableCompositionItemDependence::getBatchCurrentBindingsCardinalityLinker(bool moveNext) {
				if (mBatchCurrentBindingsCardinalityLinker == mBatchEndBindingsCardinalityLinker) {
					return nullptr;
				}
				COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = mBatchCurrentBindingsCardinalityLinker;
				if (mBatchCurrentBindingsCardinalityLinker && moveNext) {
					mBatchCurrentBindingsCardinalityLinker = mBatchCurrentBindingsCardinalityLinker->getNext();
					++mBatchProcessedBindingsCardinalityLinkerCount;
					++mTotalProcessedBindingsCardinalityLinkerCount;
				}
				return linker;
			}

			COptimizedComplexVariableIndividualUpdateCardinalityLinker* COptimizedComplexVariableCompositionItemDependence::getBatchCurrentUpdatedCardinalityLinker(bool moveNext) {
				if (mBatchCurrentUpdatedCardinalityLinker == mBatchEndUpdatedCardinalityLinker) {
					return nullptr;
				}
				COptimizedComplexVariableIndividualUpdateCardinalityLinker* linker = mBatchCurrentUpdatedCardinalityLinker;
				if (mBatchCurrentUpdatedCardinalityLinker && moveNext) {
					mBatchCurrentUpdatedCardinalityLinker = mBatchCurrentUpdatedCardinalityLinker->getNext();
				}
				return linker;
			}



			cint64 COptimizedComplexVariableCompositionItemDependence::getBatchProcessedBindingsCardinalityLinkerCount() {
				return mBatchProcessedBindingsCardinalityLinkerCount;
			}

			cint64 COptimizedComplexVariableCompositionItemDependence::getTotalProcessedBindingsCardinalityLinkerCount() {
				return mTotalProcessedBindingsCardinalityLinkerCount;
			}


			bool COptimizedComplexVariableCompositionItemDependence::isBatchProcessed(bool considerCardinalityUpdates) {
				return !getBatchCurrentBindingsCardinalityLinker(false) && (!considerCardinalityUpdates || !getBatchCurrentUpdatedCardinalityLinker(false));
			}

			bool COptimizedComplexVariableCompositionItemDependence::loadNextBatch(bool considerCardinalityUpdates) {
				COptimizedComplexVariableIndividualMappings* depMappings = mDependentItem->getVariableMapping();
				if (mBatchCurrentBindingsCardinalityLinker == mBatchEndBindingsCardinalityLinker) {
					mBatchEndBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;

					mBatchStartBindingsCardinalityLinker = depMappings->getLastAddedBindingsCardinalityLinker();
					mBatchCurrentBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;

					cint64 prevDepItemMappingCount = mBatchDependentItemMappingsCount;
					mBatchDependentItemMappingsCount = depMappings->getBindingCount();
					mBatchDependentItemMappingsSize = mBatchDependentItemMappingsCount - prevDepItemMappingCount;

					mBatchProcessedBindingsCardinalityLinkerCount = 0;
				}

				if (considerCardinalityUpdates && mBatchCurrentUpdatedCardinalityLinker == mBatchEndUpdatedCardinalityLinker) {
					mBatchEndUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
					mBatchStartUpdatedCardinalityLinker = depMappings->getLastUpdatedCardinalityLinker();
					mBatchCurrentUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
				}


				return !isBatchProcessed(considerCardinalityUpdates);
			}


			bool COptimizedComplexVariableCompositionItemDependence::isProcessingFinished(bool considerCardinalityUpdates) {
				COptimizedComplexVariableIndividualMappings* depMappings = mDependentItem->getVariableMapping();
				return isBatchProcessed(considerCardinalityUpdates) && mDependentItem->isVariableMappingsComputed() && mBatchStartBindingsCardinalityLinker == depMappings->getLastAddedBindingsCardinalityLinker() && (!considerCardinalityUpdates || mBatchStartUpdatedCardinalityLinker == depMappings->getLastUpdatedCardinalityLinker());
			}


			cint64 COptimizedComplexVariableCompositionItemDependence::getLastRequestedMappingsComputationCount() {
				return mLastRequestedMappingsComputationCount;
			}

			COptimizedComplexVariableCompositionItemDependence* COptimizedComplexVariableCompositionItemDependence::setLastRequestedMappingsComputationCount(cint64 count) {
				mLastRequestedMappingsComputationCount = count;
				return this;
			}

			bool COptimizedComplexVariableCompositionItemDependence::hasMappingsCountComputationRequested(cint64 count) {				
				if (mLastRequestedMappingsComputationCount == -1) {
					return true;
				}
				if (count == -1) {
					return false;
				}
				return mLastRequestedMappingsComputationCount >= count;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
