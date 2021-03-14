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

#include "COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				mIndiInstItemRef = indiInstItemRef;
				mRoleInstancesItem = roleInstancesItem;
				mInversed = inversed;
				mIndividualInstantiatedItemHash = individualInstantiatedItemHash;
				mIndividualVector = individualVector;
				mBackendAssocCacheReader = backendAssocCacheReader;

				mAllowPossibleFillers = false;

				mSorting = sorting;
				mIterateAscending = mSorting.getOrder() != CRealizationIndividualSorting::SORT_ORDER_DESCENDING;
				mConsiderSameMergedIndis = mSorting.getGrouping() != CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE || mSorting.getType() != CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID;

				mIndiInstItemRefCursor = indiInstItemRefCursor;
				mMoveOverCursor = moveOverCursor;
				mInitializationRequired = false;
				mIndiAssData = mBackendAssocCacheReader->getIndividualAssociationData(mIndiInstItemRef.getIndividualID());
				mSameMergedIndisInCache = mBackendAssocCacheReader->hasSameIndividualsMergings();

				mIndiInstItem = (COptimizedKPSetIndividualItem*)mIndiInstItemRef.getRealizationInstanceItem();

				if (mIndiInstItem && mIndiInstItem->getKnownPossibleRoleNeighboursInstancesHash()->contains(mRoleInstancesItem)) {
					mNeighbourPossibleInstanceHashData = &mIndiInstItem->getKnownPossibleRoleNeighboursInstancesHash()->operator[](mRoleInstancesItem);
				} else {
					mNeighbourPossibleInstanceHashData = nullptr;
				}

				mComplexRoleDataPointer = nullptr;
				if (mRoleInstancesItem->hasComplexRoleData()) {
					QHash<cint64, COptimizedKPSetIndividualComplexRoleData*>* indiIdComplexRoleDataHash = mRoleInstancesItem->getIndividualIdComplexRoleDataHash();
					if (indiIdComplexRoleDataHash && indiIdComplexRoleDataHash->contains(mIndiInstItemRef.getIndividualID())) {
						mComplexRoleDataPointer = &indiIdComplexRoleDataHash->operator[](mIndiInstItemRef.getIndividualID());
						if (*mComplexRoleDataPointer == nullptr) {
							mInitializationRequired = true;
						} else if (!(*mComplexRoleDataPointer)->isInitialized(mInversed)) {
							mInitializationRequired = true;
						}
					}
				}
				if (!mIndiAssData) {
					mInitializationRequired = true;
				}

				mMinIndiId = CINT_MAX;
				mMaxIndiId = CINT_MIN;
				mLastIndiId = -1;
				mAverageIterationIncrementSize = 1;
				mIterationCount = 0;
				mIterationMoved = false;
				mPossibleIndiCount = 0;
				mConfirmedPossibleIndiCount = 0;
				mVisitedPossibleIndiCount = 0;
				mMapInitialized = false;


				begin();
			}


			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::requiresInitialization() {
				return mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::begin() {
				if (mInitializationRequired) {
					mInitializationRequired = false;
					if (mRoleInstancesItem->hasComplexRoleData()) {
						if (!(*mComplexRoleDataPointer) || !(*mComplexRoleDataPointer)->isInitialized(mInversed)) {
							mInitializationRequired = true;
						}
					}
					if (!mIndiAssData) {
						mInitializationRequired = true;
					}
				}


				if (!mInitializationRequired && !mMapInitialized) {
					mMapInitialized = true;
					CBackendRepresentativeMemoryLabelCacheItem* combinedNeigRoleSetLabel = mIndiAssData->getLabelCacheEntry(CBackendRepresentativeMemoryLabelCacheItem::NEIGHBOUR_INSTANTIATED_ROLE_SET_COMBINATION_LABEL);
					CBackendRepresentativeMemoryCacheIndividualRoleSetNeighbourArray* roleSetNeighbourArray = mIndiAssData->getRoleSetNeighbourArray();

					COptimizedKPSetRoleInstancesCombinedNeighbourRoleSetCacheLabelData* roleItemCompNeighLabelData = mRoleInstancesItem->getCombinedNeighbourCacheLabelItemDataHash(mInversed)->value(combinedNeigRoleSetLabel);
					if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasKnownInstancesLabelItems()) {
						QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelDetDataHash = roleItemCompNeighLabelData->getKnownInstancesLabelItemDataHash();

						for (auto it = singleNeighLabelDetDataHash->constBegin(), itEnd = singleNeighLabelDetDataHash->constEnd(); it != itEnd; ++it) {
							roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
								mMinIndiId = qMin(mMinIndiId, neighbourId);
								mMaxIndiId = qMax(mMaxIndiId, neighbourId);
								mNeighbourIndiIdsMap.insert(neighbourId, IndividualIdSourceData(true, false, true, false, nullptr));
								return true;
							});
						}
					}

					if (roleItemCompNeighLabelData && roleItemCompNeighLabelData->hasPossibleInstancesLabelItems()) {
						QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>* singleNeighLabelNonDetDataHash = roleItemCompNeighLabelData->getPossibleInstancesLabelItemDataHash();
						for (auto it = singleNeighLabelNonDetDataHash->constBegin(), itEnd = singleNeighLabelNonDetDataHash->constEnd(); it != itEnd; ++it) {
							roleSetNeighbourArray->at(it.value()->getLabelArrayIndex()).visitNeighbourIndividualIds([&](cint64 neighbourId)->bool {
								mMinIndiId = qMin(mMinIndiId, neighbourId);
								mMaxIndiId = qMax(mMaxIndiId, neighbourId);
								++mPossibleIndiCount;
								mNeighbourIndiIdsMap.insert(neighbourId, IndividualIdSourceData(true, false, false, true, nullptr));
								return true;
							});
						}

					}

					if (mComplexRoleDataPointer && *mComplexRoleDataPointer) {
						COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData* indiExplicitIndirectLinkComplexRepresentationData = (COptimizedKPSetIndividualComplexRoleExplicitIndirectLinksData*)(*mComplexRoleDataPointer);
						COptimizedKPSetRoleInstancesHash* neighbourHash = indiExplicitIndirectLinkComplexRepresentationData->getRoleNeighbourInstancesHash(mInversed, false);
						if (neighbourHash) {
							for (COptimizedKPSetRoleInstancesHash::const_iterator it = neighbourHash->constBegin(), itEnd = neighbourHash->constEnd(); it != itEnd; ++it) {
								cint64 neighbourId = it.key();
								const COptimizedKPSetRoleInstancesHashData& hashData = it.value();
								if (hashData.mInstanceItemData->mKnownInstance || !hashData.mInstanceItemData->mTestedInstance) {
									mMinIndiId = qMin(mMinIndiId, neighbourId);
									mMaxIndiId = qMax(mMaxIndiId, neighbourId);
									if (!hashData.mInstanceItemData->mKnownInstance) {
										++mPossibleIndiCount;
									}
									mNeighbourIndiIdsMap.insert(neighbourId, IndividualIdSourceData(false, true, hashData.mInstanceItemData->mKnownInstance, !hashData.mInstanceItemData->mKnownInstance, hashData.mInstanceItemData));
								}
							}
						}
					}


					mNeighbourIndiIdsMapItBegin = mNeighbourIndiIdsMap.constBegin();
					mNeighbourIndiIdsMapItEnd = mNeighbourIndiIdsMap.constEnd();
					if (mIndiInstItemRefCursor.isNonEmpty()) {
						cint64 cursorId = mIndiInstItemRefCursor.getIndividualID();

						const QMap<cint64, IndividualIdSourceData>& neighbourIndiIdsMap = mNeighbourIndiIdsMap;
						if (mIterateAscending) {
							if (mMoveOverCursor) {
								mNeighbourIndiIdsMapItBegin = neighbourIndiIdsMap.upperBound(cursorId);
							} else {
								mNeighbourIndiIdsMapItBegin = neighbourIndiIdsMap.lowerBound(cursorId);
							}
						} else {
							if (mMoveOverCursor) {
								mNeighbourIndiIdsMapItEnd = neighbourIndiIdsMap.lowerBound(cursorId);
							} else {
								mNeighbourIndiIdsMapItEnd = neighbourIndiIdsMap.upperBound(cursorId);
							}
						}
					}

					if (mIterateAscending) {
						mNeighbourIndiIdsMapIt = mNeighbourIndiIdsMapItBegin;
					} else {
						mNeighbourIndiIdsMapIt = mNeighbourIndiIdsMapItEnd;
						if (mNeighbourIndiIdsMapItBegin != mNeighbourIndiIdsMapItEnd) {
							--mNeighbourIndiIdsMapIt;
						}
					}

					mAverageIterationIncrementSize = (mMaxIndiId + 1 - mMinIndiId) / (double)mNeighbourIndiIdsMap.size();
					mIterationCount = mNeighbourIndiIdsMap.size();

					mHasCurrentIndiId = false;
					mCurrentIndiId = -1;
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();

				}

				if (mMapInitialized) {
					moveToValidPosition(mNeighbourIndiIdsMapIt);
				}

				return !mInitializationRequired;
			}



			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::atEnd() {
				return !mHasCurrentIndiId && mMapInitialized;
			}

			CRealizationRemainingInstancesEstimation COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::remainingInstancesEstimation() {
				CRealizationRemainingInstancesEstimation estimation;
				estimation.setMinimalCount(mNeighbourIndiIdsMap.size() - mPossibleIndiCount);
				estimation.setMaximalCount(mNeighbourIndiIdsMap.size());

				cint64 remainingEstimation = 0;
				if (mHasCurrentIndiId) {
					cint64 diff = mMaxIndiId - mCurrentIndiId;
					if (!mIterateAscending) {
						diff = mCurrentIndiId - mMinIndiId;
					}
					remainingEstimation = (double)diff / mAverageIterationIncrementSize;
					if (mVisitedPossibleIndiCount > 0) {
						remainingEstimation = remainingEstimation * mConfirmedPossibleIndiCount / mVisitedPossibleIndiCount;
					}

					remainingEstimation = qMin(remainingEstimation, (cint64)mNeighbourIndiIdsMap.size());
					remainingEstimation = qMax(remainingEstimation, (cint64)0);

				}

				estimation.setEstimatedCount(remainingEstimation);
				return estimation;
			}


			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::moveNext() {
				mHasCurrentIndiId = false;
				mCurrentIndiId = -1;
				mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
				if (mNeighbourIndiIdsMapIt != mNeighbourIndiIdsMapItEnd) {
					if (mIterateAscending) {
						++mNeighbourIndiIdsMapIt;
					} else {
						if (mNeighbourIndiIdsMapIt == mNeighbourIndiIdsMapItBegin) {
							mNeighbourIndiIdsMapIt = mNeighbourIndiIdsMapItEnd;
						} else {
							--mNeighbourIndiIdsMapIt;
						}
					}

					moveToValidPosition(mNeighbourIndiIdsMapIt);
				}

				if (mHasCurrentIndiId) {
					mLastIndiId = mCurrentIndiId;
				} else {
					mLastIndiId = -1;
				}


				return mHasCurrentIndiId && !mInitializationRequired;
			}


			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::moveToValidPosition(QMap<cint64, IndividualIdSourceData>::const_iterator& neighbourIndiIdsMapIt) {
				mInitializationRequired = false;
				mHasCurrentIndiId = false;
				while (!mInitializationRequired && !mHasCurrentIndiId && neighbourIndiIdsMapIt != mNeighbourIndiIdsMapItEnd) {
					cint64 indiId = neighbourIndiIdsMapIt.key();
					mCurrentIndiId = indiId;
					mCurrentIndiInstanceOnlyPossible = false;
					mCurrentSourceData = neighbourIndiIdsMapIt.value();
					mCurentIndividual = mIndividualVector->getData(indiId);
					mCurrentIndividualItem = mIndividualInstantiatedItemHash->getIndividualInstantiatedItem(indiId);
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(CIndividualReference(mCurentIndividual, mCurrentIndiId), mCurrentIndividualItem);

					if (isValidIteratorPosition(mHasCurrentIndiId, mCurrentSourceData)) {
						mHasCurrentIndiId = true;
					} else if (!mInitializationRequired) {

						if (mIterateAscending) {
							++neighbourIndiIdsMapIt;
						} else {
							if (neighbourIndiIdsMapIt == mNeighbourIndiIdsMapItBegin) {
								neighbourIndiIdsMapIt = mNeighbourIndiIdsMapItEnd;
							} else {
								--neighbourIndiIdsMapIt;
							}
						}

					}

				}
				
				return mHasCurrentIndiId && !mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::isValidIteratorPosition(cint64 indiId, IndividualIdSourceData& sourceData) {
				mCurrentIndiInstanceOnlyPossible = false;
				if (!mConsiderSameMergedIndis && isSameIndividualsMerged(mCurrentIndiInstItemRef)) {
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
					return false;
				}

				if (mCurrentIndividualItem && sourceData.mIsFromAssociationNeighbourArray && sourceData.mIsPossibleNeighbour && mNeighbourPossibleInstanceHashData) {
					++mVisitedPossibleIndiCount;
					COptimizedKPSetRoleInstancesData* instanceData = mNeighbourPossibleInstanceHashData->getRoleNeighbourInstanceItemData(mInversed, indiId, false);
					if (!instanceData || !instanceData->mKnownInstance) {
						if (!mAllowPossibleFillers) {
							if (!instanceData->mTestedInstance) {
								mInitializationRequired = true;
							}
							mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
							return false;
						} else {
							mCurrentIndiInstanceOnlyPossible = true;
						}
					} else {
						++mConfirmedPossibleIndiCount;
					}				
				}

				if (mRoleInstancesItem->hasComplexRoleData() && sourceData.mIsFromComplexRoleDataHash && sourceData.mIsPossibleNeighbour) {

					++mVisitedPossibleIndiCount;
					if (!sourceData.mComplexInstancesData || !sourceData.mComplexInstancesData->mKnownInstance) {
						if (!mAllowPossibleFillers) {
							if (!sourceData.mComplexInstancesData->mTestedInstance) {
								mInitializationRequired = true;
							}
							mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();
							return false;
						} else {
							mCurrentIndiInstanceOnlyPossible = true;
						}
					} else {
						++mConfirmedPossibleIndiCount;
					}
					
				}

				return true;
			}



			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef) {
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


			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOverCursor) {
				cint64 cursorId = indiInstItemRef.getIndividualID();

				if (!mHasCurrentIndiId || mIterateAscending && cursorId < mCurrentIndiId || !mIterateAscending && cursorId > mCurrentIndiId) {
					return false;
				}

				cint64 diff = qAbs(cursorId - mCurrentIndiId);
				cint64 iterationSteps = diff / mAverageIterationIncrementSize;

				while (mHasCurrentIndiId && iterationSteps < MAX_ITERTION_BEFORE_MOVE_LIMIT && (mIterateAscending && mCurrentIndiId < cursorId || !mIterateAscending && mCurrentIndiId > cursorId)) {
					moveNext();

					diff = qAbs(cursorId - mCurrentIndiId);
					iterationSteps = diff / mAverageIterationIncrementSize;
				}

				if (mHasCurrentIndiId && (mIterateAscending && mCurrentIndiId < cursorId || !mIterateAscending && mCurrentIndiId > cursorId)) {
					mLastIndiId = -1;
					mIterationMoved = true;
					const QMap<cint64, IndividualIdSourceData>& neighbourIndiIdsMap = mNeighbourIndiIdsMap;
					if (mIterateAscending) {
						mNeighbourIndiIdsMapIt = neighbourIndiIdsMap.lowerBound(cursorId);
						mNeighbourIndiIdsMapItEnd = neighbourIndiIdsMap.constEnd();
					} else {
						mNeighbourIndiIdsMapIt = neighbourIndiIdsMap.upperBound(cursorId);
						mNeighbourIndiIdsMapItEnd = neighbourIndiIdsMap.constEnd();
						if (mNeighbourIndiIdsMapIt != mNeighbourIndiIdsMapItEnd) {
							--mNeighbourIndiIdsMapIt;
						}
					}
					if (mNeighbourIndiIdsMapIt != mNeighbourIndiIdsMap.constEnd()) {
						moveToValidPosition(mNeighbourIndiIdsMapIt);
					} else {
						mHasCurrentIndiId = false;
						mCurentIndividual = nullptr;
					}
				}
				if (moveOverCursor) {
					moveNext();
				}
				return mHasCurrentIndiId;
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::currentIndividualInstanceItemReference() {
				return mCurrentIndiInstItemRef;
			}

			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::isCurrentInstancePossible() {
				return mCurrentIndiInstanceOnlyPossible;
			}

			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::isCurrentInstanceCertain() {
				return !mCurrentIndiInstanceOnlyPossible;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::getCopy() {
				COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator* copiedIterator = new COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator(mIndiInstItemRef, mBackendAssocCacheReader, mRoleInstancesItem, mInversed, mIndividualInstantiatedItemHash, mIndividualVector, mSorting, mIndiInstItemRefCursor, mMoveOverCursor);

				copiedIterator->mNeighbourIndiIdsMap = mNeighbourIndiIdsMap;
				copiedIterator->mNeighbourIndiIdsMapItBegin = mNeighbourIndiIdsMapItBegin;
				copiedIterator->mNeighbourIndiIdsMapItEnd = mNeighbourIndiIdsMapItEnd;
				copiedIterator->mNeighbourIndiIdsMapIt = mNeighbourIndiIdsMapIt;
				copiedIterator->mCurrentIndiId = mCurrentIndiId;
				copiedIterator->mHasCurrentIndiId = mHasCurrentIndiId;
				copiedIterator->mCurrentSourceData = mCurrentSourceData;
				copiedIterator->mCurrentIndividualItem = mCurrentIndividualItem;
				copiedIterator->mCurentIndividual = mCurentIndividual;
				copiedIterator->mCurrentIndiInstItemRef = mCurrentIndiInstItemRef;
				copiedIterator->mCurrentIndiInstanceOnlyPossible = mCurrentIndiInstanceOnlyPossible;

				copiedIterator->mMapInitialized = mMapInitialized;
				copiedIterator->mInitializationRequired = mInitializationRequired;
				copiedIterator->mMinIndiId = mMinIndiId;
				copiedIterator->mMaxIndiId = mMaxIndiId;
				copiedIterator->mLastIndiId = mLastIndiId;
				copiedIterator->mAverageIterationIncrementSize = mAverageIterationIncrementSize;
				copiedIterator->mIterationCount = mIterationCount;
				copiedIterator->mIterationMoved = mIterationMoved;
				copiedIterator->mPossibleIndiCount = mPossibleIndiCount;
				copiedIterator->mVisitedPossibleIndiCount = mVisitedPossibleIndiCount;
				copiedIterator->mConfirmedPossibleIndiCount = mConfirmedPossibleIndiCount;

				return copiedIterator;
			}


			CRealizationIndividualSorting COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::getSorting() {
				return mSorting;
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::getQueryingIndividualInstanceItemReference() {
				return mIndiInstItemRef;
			}

			COptimizedKPSetRoleInstancesItem* COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::getQueryingRoleInstancesItem() {
				return mRoleInstancesItem;
			}

			bool COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator::getQueryingRoleInstancesItemInversion() {
				return mInversed;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
