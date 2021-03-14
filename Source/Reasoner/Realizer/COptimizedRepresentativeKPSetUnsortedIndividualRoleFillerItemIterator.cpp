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

#include "COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, bool considerSameMergedIndis) {
				mIndiInstItemRef = indiInstItemRef;
				mRoleInstancesItem = roleInstancesItem;
				mInversed = inversed;
				mIndividualInstantiatedItemHash = individualInstantiatedItemHash;
				mIndividualVector = individualVector;

				mAllowPossibleFillers = false;

				mConsiderSameMergedIndis = considerSameMergedIndis;

				mHasCurrentIndiId = false;
				mCurrentIndiId = -1;
				mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
				mInitializationRequired = false;
				mNeighbourPossibleInstanceDataIteratorsInitialized = false;
				mNeighbourPossibleInstanceDataIteratorsFinished = false;
				mNeighbourArrayIteratorsInitialized = false;
				mNeighbourArrayIteratorsFinished = false;
				mNeighbourComplexDataIteratorsInitialized = false;
				mNeighbourComplexDataIteratorsFinished = false;
				mCurrentNeighbourArrayVisitingPositionCursor = 0;

				mNeighbourPossibleInstanceDataIterationNewlyInitialized = false;
				mNeighbourComplexDataIterationNewlyInitialized = false;

				mBackendAssocCacheReader = backendAssocCacheReader;
				mSameMergedIndisInCache = mBackendAssocCacheReader->hasSameIndividualsMergings();

				mComplexRoleDataPointer = nullptr;
				mIndiAssData = nullptr;

				mNeighbourArrayIndiVisitedCount = 0;
				mNeighbourArrayIndiMergedCount = 0;
				mNeighbourArrayVisistedCount = 0;
				mNeighbourArrayCount = 0;
				mNeighbourArrayVisitedPosIndiCount = 0;

				moveToNextPossiblePosition(false);
			}




			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::moveToNextPossiblePosition(bool forceNextPosition) {
				if (forceNextPosition) {
					mHasCurrentIndiId = false;
					mCurrentIndiId = -1;
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
					mInitializationRequired = false;
				}

				if (!mHasCurrentIndiId && (!mNeighbourArrayIteratorsInitialized || !mNeighbourArrayIteratorsFinished)) {
					if (!mIndiAssData) {
						mIndiAssData = mBackendAssocCacheReader->getIndividualAssociationData(mIndiInstItemRef.getIndividualID());
					}
					if (!mIndiAssData) {
						mInitializationRequired = true;
					} else {
						if (!mNeighbourArrayIteratorsInitialized) {
							initializeNeighbourArrayIteration();
						}

						if (mNeighbourArrayIteratorsInitialized && !mNeighbourArrayIteratorsFinished) {
							CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = mIndiAssData->getRoleSetNeighbourArray();
							bool isSameMerged = false;
							while ((!mHasCurrentIndiId || isSameMerged) && mSingleNeighLabelDetDataHashIt != mSingleNeighLabelDetDataHashItEnd) {
								isSameMerged = false;
								mHasCurrentIndiId = false;

								if (!roleSetNeighbourArray->at(mSingleNeighLabelDetDataHashIt.value()->getLabelArrayIndex()).visitNeighbourIndividualIdsFromCursor([&](cint64 neighbourId, cint64 cursor)->bool {
									mHasCurrentIndiId = true;
									mCurrentIndiId = neighbourId;

									COptimizedKPSetIndividualItem* currentIndividualItem = mIndividualInstantiatedItemHash->getIndividualInstantiatedItem(mCurrentIndiId);
									CIndividual* curentIndividual = nullptr;
									if (!currentIndividualItem) {
										curentIndividual = mIndividualVector->getData(mCurrentIndiId);
									} else {
										curentIndividual = currentIndividualItem->getIndividual();
									}
									mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(CIndividualReference(curentIndividual, mCurrentIndiId), currentIndividualItem);

									if (!mConsiderSameMergedIndis && isSameIndividualsMerged(mCurrentIndiInstItemRef)) {
										isSameMerged = true;
									}

									++mNeighbourArrayIndiVisitedCount;
									if (isSameMerged) {
										++mNeighbourArrayIndiMergedCount;
									}
									mCurrentNeighbourArrayVisitingPositionCursor = cursor;
									return false;
								}, mCurrentNeighbourArrayVisitingPositionCursor)) {
									mCurrentNeighbourArrayVisitingPositionCursor = 0;
									++mSingleNeighLabelDetDataHashIt;

									if (mSingleNeighLabelDetDataHashIt != mSingleNeighLabelDetDataHashItEnd) {
										cint64 arrayPosIndiCount = roleSetNeighbourArray->at(mSingleNeighLabelDetDataHashIt.value()->getLabelArrayIndex()).getIndividualCount();
										mNeighbourArrayVisitedPosIndiCount += arrayPosIndiCount;
										mNeighbourArrayVisistedCount++;
									}
								}

							}

							if (!mHasCurrentIndiId) {
								mNeighbourArrayIteratorsFinished = true;
							}
						}

					}
				}
				if (mNeighbourArrayIteratorsFinished) {
					mHasCurrentIndiId = false;
					mCurrentIndiId = -1;
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
					mInitializationRequired = false;
				}


				if (!mHasCurrentIndiId && (!mNeighbourPossibleInstanceDataIteratorsInitialized || !mNeighbourPossibleInstanceDataIteratorsFinished)) {
					if (!mNeighbourPossibleInstanceDataIteratorsInitialized) {
						initializePossibleInstanceDataIteration();

					}


					if (!mNeighbourPossibleInstanceDataIteratorsFinished) {
						if (!mNeighbourPossibleInstanceDataIterationNewlyInitialized && forceNextPosition && mNeighbourPossibleInstanceDataIt != mNeighbourPossibleInstanceDataItEnd) {
							++mNeighbourPossibleInstanceDataIt;
						}
						mNeighbourPossibleInstanceDataIterationNewlyInitialized = false;
						while (!mHasCurrentIndiId && mNeighbourPossibleInstanceDataIt != mNeighbourPossibleInstanceDataItEnd) {
							mCurrentIndiId = mNeighbourPossibleInstanceDataIt.key();
							COptimizedKPSetIndividualItem* currentIndividualItem = mNeighbourPossibleInstanceDataIt->mNeighbourIndividualItem;
							mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(currentIndividualItem->getIndividualReference(), currentIndividualItem);

							if (!mConsiderSameMergedIndis && isSameIndividualsMerged(mCurrentIndiInstItemRef)) {
								++mNeighbourPossibleInstanceDataIt;
							} else {
								COptimizedKPSetRoleInstancesData* instanceData = mNeighbourPossibleInstanceDataIt->mInstanceItemData;
								if (instanceData->mKnownInstance || mAllowPossibleFillers && instanceData->mPossibleInstance && !instanceData->mTestedInstance) {
									mHasCurrentIndiId = true;
								} else if (!mAllowPossibleFillers && instanceData->mPossibleInstance && !instanceData->mKnownInstance && !instanceData->mTestedInstance) {
									mInitializationRequired = true;
									return false;
								} else {
									++mNeighbourPossibleInstanceDataIt;
								}
							}
						}

						if (!mHasCurrentIndiId) {
							mNeighbourPossibleInstanceDataIteratorsFinished = true;
						}
					}
				}



				if (!mHasCurrentIndiId && (!mNeighbourComplexDataIteratorsInitialized || !mNeighbourComplexDataIteratorsFinished)) {
					if (!mNeighbourComplexDataIteratorsInitialized) {
						initializeComplexDataIteration();
					}

					if (mNeighbourComplexDataIteratorsInitialized && !mNeighbourComplexDataIteratorsFinished) {

						if (!mNeighbourComplexDataIterationNewlyInitialized && forceNextPosition && mNeighbourComplexInstanceDataIt != mNeighbourComplexInstanceDataItEnd) {
							++mNeighbourComplexInstanceDataIt;
						}
						mNeighbourComplexDataIterationNewlyInitialized = false;
						while (!mHasCurrentIndiId && mNeighbourComplexInstanceDataIt != mNeighbourComplexInstanceDataItEnd) {

							mCurrentIndiId = mNeighbourComplexInstanceDataIt.key();
							COptimizedKPSetIndividualItem* currentIndividualItem = mNeighbourComplexInstanceDataIt->mNeighbourIndividualItem;
							CIndividual* curentIndividual = nullptr;
							if (!currentIndividualItem) {
								currentIndividualItem = mIndividualInstantiatedItemHash->getIndividualInstantiatedItem(mCurrentIndiId);
							}
							if (!currentIndividualItem) {
								curentIndividual = mIndividualVector->getData(mCurrentIndiId);
							} else {
								curentIndividual = currentIndividualItem->getIndividual();
							}
							mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(CIndividualReference(curentIndividual, mCurrentIndiId), currentIndividualItem);
							
							if (!mConsiderSameMergedIndis && isSameIndividualsMerged(mCurrentIndiInstItemRef)) {
								++mNeighbourComplexInstanceDataIt;
							} else {
								if (mNeighbourComplexInstanceDataIt->mInstanceItemData->mKnownInstance || mAllowPossibleFillers && mNeighbourComplexInstanceDataIt->mInstanceItemData->mPossibleInstance && !mNeighbourComplexInstanceDataIt->mInstanceItemData->mTestedInstance) {
									mHasCurrentIndiId = true;
								} else if (!mAllowPossibleFillers && mNeighbourComplexInstanceDataIt->mInstanceItemData->mPossibleInstance && !mNeighbourComplexInstanceDataIt->mInstanceItemData->mKnownInstance && !mNeighbourComplexInstanceDataIt->mInstanceItemData->mTestedInstance) {
									mInitializationRequired = true;
									return false;
								} else {
									++mNeighbourComplexInstanceDataIt;
								}
							}
						}

						if (!mHasCurrentIndiId) {
							mNeighbourComplexDataIteratorsFinished = true;
						}

					}
				}


				return mHasCurrentIndiId && !mInitializationRequired;

			}



			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::initializeComplexDataIteration() {
				if (mRoleInstancesItem->hasComplexRoleData() && mRoleInstancesItem->getIndividualIdComplexRoleDataHash()->contains(mIndiInstItemRef.getIndividualID())) {
					if (!mComplexRoleDataPointer) {
						mComplexRoleDataPointer = &mRoleInstancesItem->getIndividualIdComplexRoleDataHash()->operator[](mIndiInstItemRef.getIndividualID());
					}
					if (*mComplexRoleDataPointer == nullptr) {
						mInitializationRequired = true;
					} else if (!(*mComplexRoleDataPointer)->isInitialized(mInversed)) {
						mInitializationRequired = true;
					} else {
						mNeighbourComplexDataIterationNewlyInitialized = true;
						mNeighbourComplexDataIteratorsInitialized = true;
						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)(*mComplexRoleDataPointer);
						COptimizedKPSetRoleInstancesHash* neighbourHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(mInversed, false);
						if (neighbourHash) {
							mNeighbourComplexInstanceDataIt = neighbourHash->constBegin();
							mNeighbourComplexInstanceDataItEnd = neighbourHash->constEnd();
						}
					}
				} else {
					mNeighbourComplexDataIteratorsInitialized = true;
				}
				return mNeighbourComplexDataIterationNewlyInitialized;
			}


			void COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::initializePossibleInstanceDataIteration() {
				COptimizedKPSetIndividualItem* indiInstItem = (COptimizedKPSetIndividualItem*)mIndiInstItemRef.getRealizationInstanceItem();
				mNeighbourPossibleInstanceDataIteratorsInitialized = true;
				mNeighbourPossibleInstanceDataIterationNewlyInitialized = true;
				if (indiInstItem && indiInstItem->getKnownPossibleRoleNeighboursInstancesHash()->contains(mRoleInstancesItem)) {
					COptimizedKPSetRoleNeighbourInstancesHashData& neighbourPossibleInstanceHashData = indiInstItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](mRoleInstancesItem);
					COptimizedKPSetRoleInstancesHash* neighbourPossibleInstanceHash = neighbourPossibleInstanceHashData.getRoleNeighbourInstancesHash(mInversed, false);
					if (neighbourPossibleInstanceHash) {
						mNeighbourPossibleInstanceDataIt = neighbourPossibleInstanceHash->constBegin();
						mNeighbourPossibleInstanceDataItEnd = neighbourPossibleInstanceHash->constEnd();
					}
				}
			}

			void COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::initializeNeighbourArrayIteration() {
				mCurrentNeighbourArrayVisitingPositionCursor = 0;
				mNeighbourArrayIteratorsInitialized = true;
				CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = mIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
				CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = mIndiAssData->getRoleSetNeighbourArray();

				COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = mRoleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(mInversed)->value(combinedNeigRoleSetLabel);
				if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDetDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();
					mSingleNeighLabelDetDataHashIt = singleNeighLabelDetDataHash->constBegin();
					mSingleNeighLabelDetDataHashItEnd = singleNeighLabelDetDataHash->constEnd();
					mNeighbourArrayCount = singleNeighLabelDetDataHash->size();
				}

				if (mSingleNeighLabelDetDataHashIt != mSingleNeighLabelDetDataHashItEnd) {
					cint64 arrayPosIndiCount = roleSetNeighbourArray->at(mSingleNeighLabelDetDataHashIt.value()->getLabelArrayIndex()).getIndividualCount();
					mNeighbourArrayVisitedPosIndiCount += arrayPosIndiCount;
					mNeighbourArrayVisistedCount++;
				}


				//if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasPossibleInstancesLabelItems()) {
				//	QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelNonDetDataHash = roleItemCompNeighLabelData->getPossibleInstancesLabelItemDataHash();
				//	mSingleNeighLabelNonDetDataHashIt = singleNeighLabelNonDetDataHash->constBegin();
				//	mSingleNeighLabelNonDetDataHashItEnd = singleNeighLabelNonDetDataHash->constEnd();
				//}
			}




			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef) {
				if (indiInstItemRef.getRealizationInstanceItem()) {
					COptimizedKPSetIndividualItem* indiItem = (COptimizedKPSetIndividualItem*)indiInstItemRef.getRealizationInstanceItem();
					if (indiItem->getIndividualId() != indiInstItemRef.getIndividualID() || indiItem->isItemSameIndividualMerged()) {
						return true;
					}
					return false;
				} else {
					if (mSameMergedIndisInCache) {
						CBackendRepresentativeMemoryCacheIndividualAssociationData* neighbourIndiAssData = mBackendAssocCacheReader->getIndividualAssociationData(indiInstItemRef.getIndividualID());
						return neighbourIndiAssData->hasRepresentativeSameIndividualMerging();
					} else {
						return false;
					}
				}
			}


			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::requiresInitialization() {
				return mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::begin() {
				if (mInitializationRequired) {
					mInitializationRequired = false;
					moveToNextPossiblePosition(false);
				}
				return !mInitializationRequired;
			}



			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::atEnd() {
				return !mHasCurrentIndiId && !mInitializationRequired;
			}

			CRealizationRemainingInstancesEstimation COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::remainingInstancesEstimation() {
				CRealizationRemainingInstancesEstimation estimation;
				if (!mNeighbourArrayIteratorsInitialized) {
					initializeNeighbourArrayIteration();
				}
				double remainingIndiCount = 0;
				if (!mNeighbourArrayIteratorsFinished) {
					remainingIndiCount = mNeighbourArrayVisitedPosIndiCount - mNeighbourArrayIndiVisitedCount;
					double remainingNeighbourArrayCount = mNeighbourArrayCount - mNeighbourArrayVisistedCount;
					double remainingNeighbourArrayIndiCount = (double)mNeighbourArrayVisitedPosIndiCount / (double)mNeighbourArrayVisistedCount * remainingNeighbourArrayCount;
					remainingIndiCount += remainingNeighbourArrayIndiCount;
					if (mNeighbourArrayIndiMergedCount > 0) {
						remainingIndiCount = remainingIndiCount * (double)(mNeighbourArrayIndiVisitedCount - mNeighbourArrayIndiMergedCount) / (double)(mNeighbourArrayIndiVisitedCount);
					}
				}
				if (!mNeighbourPossibleInstanceDataIteratorsFinished) {
					// TODO: make more precise
					remainingIndiCount += 1.;
				}
				if (!mNeighbourComplexDataIteratorsFinished) {
					// TODO: make more precise
					remainingIndiCount += 1.;
				}
				estimation.setEstimatedCount(remainingIndiCount);
				return estimation;
			}


			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::moveNext() {
				moveToNextPossiblePosition(true);	
				return mHasCurrentIndiId && !mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver) {
				return false;
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::currentIndividualInstanceItemReference() {
				return mCurrentIndiInstItemRef;
			}

			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::isCurrentInstancePossible() {
				return mCurrentIndiInstanceOnlyPossible;
			}

			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::isCurrentInstanceCertain() {
				return !mCurrentIndiInstanceOnlyPossible;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::getCopy() {
				COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator* copiedIterator = new COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator(mIndiInstItemRef, mBackendAssocCacheReader, mRoleInstancesItem, mInversed, mIndividualInstantiatedItemHash, mIndividualVector, mConsiderSameMergedIndis);


				copiedIterator->mNeighbourPossibleInstanceDataIteratorsInitialized = mNeighbourPossibleInstanceDataIteratorsInitialized;
				copiedIterator->mNeighbourPossibleInstanceDataIteratorsFinished = mNeighbourPossibleInstanceDataIteratorsFinished;
				copiedIterator->mNeighbourPossibleInstanceDataIt = mNeighbourPossibleInstanceDataIt;
				copiedIterator->mNeighbourPossibleInstanceDataItEnd = mNeighbourPossibleInstanceDataItEnd;

				copiedIterator->mNeighbourArrayIteratorsInitialized = mNeighbourArrayIteratorsInitialized;
				copiedIterator->mNeighbourArrayIteratorsFinished = mNeighbourArrayIteratorsFinished;
				copiedIterator->mIndiAssData = mIndiAssData;
				copiedIterator->mSingleNeighLabelDetDataHashIt = mSingleNeighLabelDetDataHashIt;
				copiedIterator->mSingleNeighLabelDetDataHashItEnd = mSingleNeighLabelDetDataHashItEnd;
				copiedIterator->mCurrentNeighbourArrayVisitingPositionCursor = mCurrentNeighbourArrayVisitingPositionCursor;

				copiedIterator->mNeighbourComplexDataIteratorsInitialized = mNeighbourComplexDataIteratorsInitialized;
				copiedIterator->mNeighbourComplexDataIteratorsFinished = mNeighbourComplexDataIteratorsFinished;
				copiedIterator->mNeighbourComplexInstanceDataIt = mNeighbourComplexInstanceDataIt;
				copiedIterator->mNeighbourComplexInstanceDataItEnd = mNeighbourComplexInstanceDataItEnd;

				copiedIterator->mAllowPossibleFillers = mAllowPossibleFillers;
				copiedIterator->mComplexRoleDataPointer = mComplexRoleDataPointer;
				copiedIterator->mInitializationRequired = mInitializationRequired;

				copiedIterator->mCurrentIndiId = mCurrentIndiId;
				copiedIterator->mHasCurrentIndiId = mHasCurrentIndiId;
				copiedIterator->mCurrentIndiInstItemRef = mCurrentIndiInstItemRef;
				copiedIterator->mCurrentIndiInstanceOnlyPossible = mCurrentIndiInstanceOnlyPossible;

				copiedIterator->mNeighbourPossibleInstanceDataIterationNewlyInitialized = mNeighbourPossibleInstanceDataIterationNewlyInitialized;
				copiedIterator->mNeighbourComplexDataIterationNewlyInitialized = mNeighbourComplexDataIterationNewlyInitialized;

				copiedIterator->mNeighbourArrayIndiVisitedCount = mNeighbourArrayIndiVisitedCount;
				copiedIterator->mNeighbourArrayIndiMergedCount = mNeighbourArrayIndiMergedCount;
				copiedIterator->mNeighbourArrayVisistedCount = mNeighbourArrayVisistedCount;
				copiedIterator->mNeighbourArrayCount = mNeighbourArrayCount;
				copiedIterator->mNeighbourArrayVisitedPosIndiCount = mNeighbourArrayVisitedPosIndiCount;

				return copiedIterator;
			}


			CRealizationIndividualSorting COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::getSorting() {
				return CRealizationIndividualSorting(Realization::CRealizationIndividualSorting::SORT_TYPE_NOTHING, Realization::CRealizationIndividualSorting::SORT_ORDER_NOTHING, Realization::CRealizationIndividualSorting::SORT_GROUP_NOTHING);
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::getQueryingIndividualInstanceItemReference() {
				return mIndiInstItemRef;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::getQueryingRoleInstancesItem() {
				return mRoleInstancesItem;
			}

			bool COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator::getQueryingRoleInstancesItemInversion() {
				return mInversed;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
