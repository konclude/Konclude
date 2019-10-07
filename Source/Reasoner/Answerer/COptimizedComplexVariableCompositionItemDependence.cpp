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

				mSingleLoadedBindingsCardinalityLinker = false;

				mCurrentLoadedBindingsCardinalityBatchLinker = nullptr;
				mStartLoadedBindingsCardinalityBatchLinker = nullptr;
				mEndLoadedBindingsCardinalityBatchLinker = nullptr;
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
				return !getBatchCurrentBindingsCardinalityLinker(false) && mCurrentLoadedBindingsCardinalityBatchLinker == nullptr && (!considerCardinalityUpdates || !getBatchCurrentUpdatedCardinalityLinker(false));
			}

			bool COptimizedComplexVariableCompositionItemDependence::loadNextBatch(bool considerCardinalityUpdates) {
				if (mStartLoadedBindingsCardinalityBatchLinker) {
					return loadNextBatchProvidedBatch(considerCardinalityUpdates);
				} else {
					COptimizedComplexVariableIndividualMappings* depMappings = mDependentItem->getVariableMapping();
					if (mBatchCurrentBindingsCardinalityLinker == mBatchEndBindingsCardinalityLinker) {
						mSingleLoadedBindingsCardinalityLinker = true;
						mBatchEndBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;

						mBatchStartBindingsCardinalityLinker = depMappings->getLastAddedBindingsCardinalityLinker();
						mBatchCurrentBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;

						cint64 prevDepItemMappingCount = mBatchDependentItemMappingsCount;
						mBatchDependentItemMappingsCount = depMappings->getBindingCount();
						mBatchDependentItemMappingsSize = mBatchDependentItemMappingsCount - prevDepItemMappingCount;

						mBatchProcessedBindingsCardinalityLinkerCount = 0;
					}

					if (considerCardinalityUpdates && mBatchCurrentUpdatedCardinalityLinker == mBatchEndUpdatedCardinalityLinker) {
						mSingleLoadedBindingsCardinalityLinker = true;
						mBatchEndUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
						mBatchStartUpdatedCardinalityLinker = depMappings->getLastUpdatedCardinalityLinker();
						mBatchCurrentUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
					}


					return !isBatchProcessed(considerCardinalityUpdates);
				}
			}



			COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* COptimizedComplexVariableCompositionItemDependence::getBatchProvidedBindingCardinalityBatchLinker(bool moveNext) {
				if (!mCurrentLoadedBindingsCardinalityBatchLinker) {
					return nullptr;
				}
				COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* linker = mCurrentLoadedBindingsCardinalityBatchLinker;

				if (mBatchCurrentBindingsCardinalityLinker == mBatchEndBindingsCardinalityLinker && moveNext) {
					mBatchStartBindingsCardinalityLinker = mCurrentLoadedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker();
					mBatchCurrentBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;
					mBatchEndBindingsCardinalityLinker = mCurrentLoadedBindingsCardinalityBatchLinker->getEndBindingsCardinalityLinker();
					if (mBatchEndBindingsCardinalityLinker) {
						mBatchEndBindingsCardinalityLinker = mBatchEndBindingsCardinalityLinker->getNext();
					}

					cint64 prevDepItemMappingCount = mBatchDependentItemMappingsCount;
					mBatchDependentItemMappingsCount = prevDepItemMappingCount + mCurrentLoadedBindingsCardinalityBatchLinker->getLinkerCount();
					mBatchDependentItemMappingsSize = mCurrentLoadedBindingsCardinalityBatchLinker->getLinkerCount();


					mBatchProcessedBindingsCardinalityLinkerCount = 0;
				}

				if (mCurrentLoadedBindingsCardinalityBatchLinker && moveNext) {
					if (mCurrentLoadedBindingsCardinalityBatchLinker == mEndLoadedBindingsCardinalityBatchLinker) {
						mCurrentLoadedBindingsCardinalityBatchLinker = nullptr;
					} else {
						mCurrentLoadedBindingsCardinalityBatchLinker = mCurrentLoadedBindingsCardinalityBatchLinker->getNext();
					}
				}


				return linker;
			}


			bool COptimizedComplexVariableCompositionItemDependence::setBatchProvidedBindingCardinalityLinkersProcessed(COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* batchLinker) {
				if (batchLinker->getStartBindingsCardinalityLinker() == mBatchCurrentBindingsCardinalityLinker) {
					mBatchCurrentBindingsCardinalityLinker = mBatchEndBindingsCardinalityLinker;

					mBatchProcessedBindingsCardinalityLinkerCount += batchLinker->getLinkerCount();
					mTotalProcessedBindingsCardinalityLinkerCount += batchLinker->getLinkerCount();

					return true;
				}
				return false;
			}




			bool COptimizedComplexVariableCompositionItemDependence::loadNextBatchProvidedBatch(bool considerCardinalityUpdates) {
				COptimizedComplexVariableIndividualMappings* depMappings = mDependentItem->getVariableMapping();
				COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* lastAddedBindingsCardinalityBatchLinker = depMappings->getLastAddedBindingsCardinalityBatchLinker();
				COptimizedComplexVariableIndividualBindingsCardinalityBatchLinker* firstAddedBindingsCardinalityBatchLinker = depMappings->getFirstAddedBindingsCardinalityBatchLinker();
				if (lastAddedBindingsCardinalityBatchLinker && !mSingleLoadedBindingsCardinalityLinker) {

					if (mCurrentLoadedBindingsCardinalityBatchLinker == nullptr && lastAddedBindingsCardinalityBatchLinker != mEndLoadedBindingsCardinalityBatchLinker) {

						if (!mEndLoadedBindingsCardinalityBatchLinker) {
							mStartLoadedBindingsCardinalityBatchLinker = firstAddedBindingsCardinalityBatchLinker;
						} else {
							mStartLoadedBindingsCardinalityBatchLinker = mEndLoadedBindingsCardinalityBatchLinker->getNext();
						}

						mEndLoadedBindingsCardinalityBatchLinker = lastAddedBindingsCardinalityBatchLinker;
						mCurrentLoadedBindingsCardinalityBatchLinker = mStartLoadedBindingsCardinalityBatchLinker;
					}


					if (mBatchCurrentBindingsCardinalityLinker == mBatchEndBindingsCardinalityLinker && mCurrentLoadedBindingsCardinalityBatchLinker != nullptr) {
						mBatchStartBindingsCardinalityLinker = mCurrentLoadedBindingsCardinalityBatchLinker->getStartBindingsCardinalityLinker();
						mBatchCurrentBindingsCardinalityLinker = mBatchStartBindingsCardinalityLinker;
						mBatchEndBindingsCardinalityLinker = mCurrentLoadedBindingsCardinalityBatchLinker->getEndBindingsCardinalityLinker();
						if (mBatchEndBindingsCardinalityLinker) {
							mBatchEndBindingsCardinalityLinker = mBatchEndBindingsCardinalityLinker->getNext();
						}

						cint64 prevDepItemMappingCount = mBatchDependentItemMappingsCount;
						mBatchDependentItemMappingsCount = prevDepItemMappingCount + mCurrentLoadedBindingsCardinalityBatchLinker->getLinkerCount();
						mBatchDependentItemMappingsSize = mCurrentLoadedBindingsCardinalityBatchLinker->getLinkerCount();


						mBatchProcessedBindingsCardinalityLinkerCount = 0;
					}

					if (considerCardinalityUpdates && mBatchCurrentUpdatedCardinalityLinker == mBatchEndUpdatedCardinalityLinker) {
						mBatchEndUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
						mBatchStartUpdatedCardinalityLinker = depMappings->getLastUpdatedCardinalityLinker();
						mBatchCurrentUpdatedCardinalityLinker = mBatchStartUpdatedCardinalityLinker;
					}

					return !isBatchProcessed(considerCardinalityUpdates);
				} else {
					return loadNextBatch(considerCardinalityUpdates);
				}
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
