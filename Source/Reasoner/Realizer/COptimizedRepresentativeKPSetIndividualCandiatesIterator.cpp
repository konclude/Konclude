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

#include "COptimizedRepresentativeKPSetIndividualCandiatesIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetIndividualCandiatesIterator::COptimizedRepresentativeKPSetIndividualCandiatesIterator(const QMap<cint64, IndividualIdCandidateData>& candidateIndiIdsMap, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				mIndividualInstantiatedItemHash = individualInstantiatedItemHash;
				mIndividualVector = individualVector;
				mBackendAssocCacheReader = backendAssocCacheReader;


				mSorting = sorting;
				mIterateAscending = mSorting.getOrder() != CRealizationIndividualSorting::SORT_ORDER_DESCENDING;

				mIndiInstItemRefCursor = indiInstItemRefCursor;
				mMoveOverCursor = moveOverCursor;
				mInitializationRequired = false;


				mComplexRoleDataPointer = nullptr;

				mMinIndiId = CINT_MAX;
				mMaxIndiId = CINT_MIN;
				mLastIndiId = -1;
				mAverageIterationIncrementSize = 1;
				mIterationCount = 0;
				mIterationMoved = false;
				mMapInitialized = false;

				mCandidateIndiIdsMap = candidateIndiIdsMap;

				begin();
			}


			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::requiresInitialization() {
				return mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::begin() {

				if (!mInitializationRequired && !mMapInitialized) {
					mMapInitialized = true;

					mCandidateIndiIdsMapItBegin = mCandidateIndiIdsMap.constBegin();
					mCandidateIndiIdsMapItEnd = mCandidateIndiIdsMap.constEnd();
					if (mIndiInstItemRefCursor.isNonEmpty()) {
						cint64 cursorId = mIndiInstItemRefCursor.getIndividualID();

						if (mIterateAscending) {
							if (mMoveOverCursor) {
								mCandidateIndiIdsMapItBegin = mCandidateIndiIdsMap.upperBound(cursorId);
							} else {
								mCandidateIndiIdsMapItBegin = mCandidateIndiIdsMap.lowerBound(cursorId);
							}
						} else {
							if (mMoveOverCursor) {
								mCandidateIndiIdsMapItEnd = mCandidateIndiIdsMap.lowerBound(cursorId);
							} else {
								mCandidateIndiIdsMapItEnd = mCandidateIndiIdsMap.upperBound(cursorId);
							}
						}
					}

					if (mIterateAscending) {
						mCandidateIndiIdsMapIt = mCandidateIndiIdsMapItBegin;
					} else {
						mCandidateIndiIdsMapIt = mCandidateIndiIdsMapItEnd;
						if (mCandidateIndiIdsMapItBegin != mCandidateIndiIdsMapItEnd) {
							--mCandidateIndiIdsMapIt;
						}
					}


					if (mCandidateIndiIdsMapItBegin != mCandidateIndiIdsMapItEnd) {
						mMinIndiId = mCandidateIndiIdsMapItBegin.key();
						QMap<cint64, IndividualIdCandidateData>::const_iterator lastIt = mCandidateIndiIdsMapItEnd;
						--lastIt;
						mMaxIndiId = lastIt.key();
					}

					mAverageIterationIncrementSize = (mMaxIndiId - mMinIndiId) / (double)mCandidateIndiIdsMap.size();
					mIterationCount = mCandidateIndiIdsMap.size();

					mHasCurrentIndiId = false;
					mCurrentIndiId = -1;
					mCurrentIndividualItem = nullptr;
					mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();

				}

				if (mMapInitialized) {
					loadIteratorIndividualItemData(mCandidateIndiIdsMapIt);
				}

				return !mInitializationRequired;
			}



			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::atEnd() {
				return !mHasCurrentIndiId && mMapInitialized;
			}

			CRealizationRemainingInstancesEstimation COptimizedRepresentativeKPSetIndividualCandiatesIterator::remainingInstancesEstimation() {
				CRealizationRemainingInstancesEstimation estimation;
				estimation.setMinimalCount(mCandidateIndiIdsMap.size());
				estimation.setMaximalCount(mCandidateIndiIdsMap.size());

				cint64 remainingEstimation = 0;
				if (mHasCurrentIndiId) {
					cint64 diff = mMaxIndiId - mCurrentIndiId;
					if (!mIterateAscending) {
						diff = mCurrentIndiId - mMinIndiId;
					}
					remainingEstimation = (double)diff / mAverageIterationIncrementSize;

					remainingEstimation = qMin(remainingEstimation, (cint64)mCandidateIndiIdsMap.size());
					remainingEstimation = qMax(remainingEstimation, (cint64)0);
				}

				estimation.setEstimatedCount(remainingEstimation);
				return estimation;
			}


			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::moveNext() {
				if (mCandidateIndiIdsMapIt != mCandidateIndiIdsMapItEnd) {
					if (mIterateAscending) {
						++mCandidateIndiIdsMapIt;
					} else {
						if (mCandidateIndiIdsMapIt == mCandidateIndiIdsMapItBegin) {
							mCandidateIndiIdsMapIt = mCandidateIndiIdsMapItEnd;
						} else {
							--mCandidateIndiIdsMapIt;
						}
					}

				}
				loadIteratorIndividualItemData(mCandidateIndiIdsMapIt);

				if (mHasCurrentIndiId) {
					mLastIndiId = mCurrentIndiId;
				} else {
					mLastIndiId = -1;
				}


				return mHasCurrentIndiId && !mInitializationRequired;
			}



			COptimizedRepresentativeKPSetIndividualCandiatesIterator* COptimizedRepresentativeKPSetIndividualCandiatesIterator::loadIteratorIndividualItemData(QMap<cint64, IndividualIdCandidateData>::const_iterator currentIndiIdsMapIt) {
				mHasCurrentIndiId = false;
				mCurrentIndiId = -1;
				mCurrentIndividualItem = nullptr;
				mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference();

				if (currentIndiIdsMapIt != mCandidateIndiIdsMapItEnd) {
					mCurrentIndiId = currentIndiIdsMapIt.key();
					mHasCurrentIndiId = true;

					mCurrentIndividualItem = mIndividualInstantiatedItemHash->value(mCurrentIndiId);
					if (mCurrentIndividualItem) {
						mCurentIndividual = mCurrentIndividualItem->getIndividual();
						mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(mCurentIndividual, mCurrentIndiId, mCurrentIndividualItem);
					} else {
						mCurentIndividual = mIndividualVector->getData(mCurrentIndiId);
						mCurrentIndiInstItemRef = CRealizationIndividualInstanceItemReference(mCurentIndividual, mCurrentIndiId, mCurrentIndividualItem);
					}

				}
				return this;

			}




			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOverCursor) {
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

				if (mHasCurrentIndiId && iterationSteps > MAX_ITERTION_BEFORE_MOVE_LIMIT && (mIterateAscending && mCurrentIndiId < cursorId || !mIterateAscending && mCurrentIndiId > cursorId)) {
					mLastIndiId = -1;
					mIterationMoved = true;
					if (mIterateAscending) {
						mCandidateIndiIdsMapIt = mCandidateIndiIdsMap.lowerBound(cursorId);
					} else {
						mCandidateIndiIdsMapIt = mCandidateIndiIdsMap.upperBound(cursorId);
						if (mCandidateIndiIdsMapIt != mCandidateIndiIdsMapItEnd) {
							--mCandidateIndiIdsMapIt;
						}
					}
				}
				if (moveOverCursor) {
					moveNext();
				} else {
					loadIteratorIndividualItemData(mCandidateIndiIdsMapIt);
				}
				return mHasCurrentIndiId;
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetIndividualCandiatesIterator::currentIndividualInstanceItemReference() {
				return mCurrentIndiInstItemRef;
			}

			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::isCurrentInstancePossible() {
				return false;
			}

			bool COptimizedRepresentativeKPSetIndividualCandiatesIterator::isCurrentInstanceCertain() {
				return true;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetIndividualCandiatesIterator::getCopy() {
				COptimizedRepresentativeKPSetIndividualCandiatesIterator* copiedIterator = new COptimizedRepresentativeKPSetIndividualCandiatesIterator(mCandidateIndiIdsMap, mBackendAssocCacheReader, mIndividualInstantiatedItemHash, mIndividualVector, mSorting, mIndiInstItemRefCursor, mMoveOverCursor);

				copiedIterator->mCandidateIndiIdsMap = mCandidateIndiIdsMap;
				copiedIterator->mCandidateIndiIdsMapItBegin = mCandidateIndiIdsMapItBegin;
				copiedIterator->mCandidateIndiIdsMapItEnd = mCandidateIndiIdsMapItEnd;
				copiedIterator->mCandidateIndiIdsMapIt = mCandidateIndiIdsMapIt;
				copiedIterator->mCurrentIndiId = mCurrentIndiId;
				copiedIterator->mHasCurrentIndiId = mHasCurrentIndiId;
				copiedIterator->mCurrentIndividualItem = mCurrentIndividualItem;
				copiedIterator->mCurentIndividual = mCurentIndividual;
				copiedIterator->mCurrentIndiInstItemRef = mCurrentIndiInstItemRef;

				copiedIterator->mMapInitialized = mMapInitialized;
				copiedIterator->mInitializationRequired = mInitializationRequired;
				copiedIterator->mMinIndiId = mMinIndiId;
				copiedIterator->mMaxIndiId = mMaxIndiId;
				copiedIterator->mLastIndiId = mLastIndiId;
				copiedIterator->mAverageIterationIncrementSize = mAverageIterationIncrementSize;
				copiedIterator->mIterationCount = mIterationCount;
				copiedIterator->mIterationMoved = mIterationMoved;

				return copiedIterator;
			}


			CRealizationIndividualSorting COptimizedRepresentativeKPSetIndividualCandiatesIterator::getSorting() {
				return mSorting;
			}





		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
