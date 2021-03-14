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

#include "COptimizedRepresentativeKPSetCacheLabelItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			bool operator<(const COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorKey& key1, const COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorKey& key2) {
				if (key1.mIndiId != key2.mIndiId) {
					return key1.mIndiId < key2.mIndiId;
				}
				if (key1.mPossibleInstance != key2.mPossibleInstance) {
					return !key1.mPossibleInstance;
				}
				return false;
			}

			COptimizedRepresentativeKPSetCacheLabelItemIterator::COptimizedRepresentativeKPSetCacheLabelItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, 
				COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {

				mLabelCacheItemKnwonInstancesList = labelCacheItemKnownInstancesList;
				mLabelCacheItemPossibleInstancesList = labelCacheItemPossibleInstancesList;
				mIndividualInstantiatedItemHash = individualInstantiatedItemHash;

				mIndiInstItemRefCursor = indiInstItemRefCursor;
				mPreventSameIndisMultipleOccurences = false;
				mMoveOverCursor = moveOverCursor;

				mAverageIterationIncrementSize = 1;
				mIterationCount = 0;
				mLastIndiId = -1;

				mAssociatedIndiPossibleCount = 0;
				mAssociatedIndiTotalCount = 0;
				mAssociatedIndiPossibleVisistedCount = 0;
				mAssociatedIndiPossibleConfirmedCount = 0;

				mSorting = sorting;
				mIterateAscending = mSorting.getOrder() != CRealizationIndividualSorting::SORT_ORDER_DESCENDING;
				mConsiderSameMergedIndis = mSorting.getGrouping() != CRealizationIndividualSorting::SORT_GROUP_SAME_UNIQUE || mSorting.getType() != CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID;

				mCurrentIteratorData = nullptr;
				mIndividualVector = individualVector;

				mAllowPossibleInstances = false;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelItemIterator::init() {
				mInitializationRequired = false;
				mCurrentIteratorData = nullptr;
				mFirstIteratorIndiId = -1;

				for (CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem : mLabelCacheItemKnwonInstancesList) {
					createIndividualAssociationsIteratorData(cacheLabelItem, false, mIndiInstItemRefCursor, mMoveOverCursor);
				}
				for (CBackendRepresentativeMemoryLabelCacheItem* cacheLabelItem : mLabelCacheItemPossibleInstancesList) {
					createIndividualAssociationsIteratorData(cacheLabelItem, true, mIndiInstItemRefCursor, mMoveOverCursor);
				}

				moveNext();
				return this;
			}

			COptimizedRepresentativeKPSetCacheLabelItemIterator::~COptimizedRepresentativeKPSetCacheLabelItemIterator() {
				qDeleteAll(mIteratorDataContainer);
			}



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelItemIterator::createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) {
				return new CAssociationMapIteratorData(labelItem, iterator, possibleInstances);
			}


			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetCacheLabelItemIterator::getIndividualInstanceItemReferenceCursor() {
				return mIndiInstItemRefCursor;
			}

			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::hasMoveOverCursor() {
				return mMoveOverCursor;
			}



			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedRepresentativeKPSetCacheLabelItemIterator::getKnownInstancesLabelCacheItemList() {
				return &mLabelCacheItemKnwonInstancesList;
			}

			QList<CBackendRepresentativeMemoryLabelCacheItem*>* COptimizedRepresentativeKPSetCacheLabelItemIterator::getPossibleInstancesLabelCacheItemList() {
				return &mLabelCacheItemPossibleInstancesList;
			}



			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::requiresInitialization() {
				return mInitializationRequired;
			}

			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::begin() {
				if (mInitializationRequired) {
					mInitializationRequired = false;

					++mCheckingInitializationRequiredId;

					QList<CAssociationMapIteratorData*> tmpUninitializedList(mUninitializedList);
					mUninitializedList.clear();
					while (!tmpUninitializedList.isEmpty()) {
						CAssociationMapIteratorData* iteratorData = tmpUninitializedList.takeFirst();
						moveToValidPosition(iteratorData);
						iteratorData->mLastCheckedInitializationRequiredId = mCheckingInitializationRequiredId;
						cint64 itIndiId = iteratorData->mIterator.currentAssociatedIndividualId();
						if (itIndiId != -1) {
							mCurrentIndiIdIterationDataMap.insertMulti((itIndiId, iteratorData), iteratorData);
						} else if (iteratorData->mInitializationRequired && !iteratorData->mIterator.atEnd()) {
							mUninitializedList.append(iteratorData);
							mInitializationRequired = true;
						}
					}


					if (mCurrentIteratorData) {
						moveToValidPosition(mCurrentIteratorData);
						mCurrentIteratorData->mLastCheckedInitializationRequiredId = mCheckingInitializationRequiredId;
						cint64 itIndiId = mCurrentIteratorData->mIterator.currentAssociatedIndividualId();
						if (itIndiId != -1) {
							mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(itIndiId, mCurrentIteratorData->mPossibleInstances), mCurrentIteratorData);
						} else if (mCurrentIteratorData->mInitializationRequired && !mCurrentIteratorData->mIterator.atEnd()) {
							mUninitializedList.append(mCurrentIteratorData);
							mInitializationRequired = true;
						}
						mCurrentIteratorData = nullptr;
					}

					if (!mInitializationRequired) {
						moveNext();
					}
				}
				return !mInitializationRequired;
			}



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelItemIterator::createIndividualAssociationsIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool possibleInstances, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)labelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
				if (extensionData) {
					mAssociatedIndiTotalCount += extensionData->getIndividualIdAssociationCount();
					if (possibleInstances) {
						mAssociatedIndiPossibleCount += extensionData->getIndividualIdAssociationCount();
					}
					CAssociationMapIteratorData* iteratorData = nullptr;
					if (indiInstItemRefCursor.isNonEmpty()) {
						iteratorData = createAssociationMapIteratorData(labelItem, extensionData->getIterator(indiInstItemRefCursor.getIndividualID(), moveOverCursor, mIterateAscending, true, mConsiderSameMergedIndis), possibleInstances);
					} else {
						iteratorData = createAssociationMapIteratorData(labelItem, extensionData->getIterator(mIterateAscending, true, mConsiderSameMergedIndis), possibleInstances);
					}
					mIteratorDataContainer.append(iteratorData);
					if (!iteratorData->mIterator.atEnd()) {
						cint64 itIndiId = iteratorData->mIterator.currentAssociatedIndividualId();
						if (itIndiId == -1 || mCurrentIndiIdIterationDataMap.isEmpty() || !possibleInstances) {
							iteratorData->mLastCheckedInitializationRequiredId = mCheckingInitializationRequiredId;
							moveToValidPosition(iteratorData);
							itIndiId = iteratorData->mIterator.currentAssociatedIndividualId();
							if (itIndiId != -1) {
								mCurrentIndiIdIterationDataMap.insertMulti((itIndiId, iteratorData), iteratorData);
							} else if (iteratorData->mInitializationRequired && !iteratorData->mIterator.atEnd()) {
								mUninitializedList.append(iteratorData);
								mInitializationRequired = true;
							}
						} else {
							mCurrentIndiIdIterationDataMap.insertMulti((itIndiId, iteratorData), iteratorData);
							iteratorData->mInitializationRequired = true;
						}
					}
					return iteratorData;
				}
				return nullptr;
			}



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelItemIterator::updateIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor) {
				CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData* extensionData = (CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData*)associationIteratorData->mLabelItem->getExtensionData(CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData::INDIVIDUAL_ASSOCIATION_MAP);
				if (extensionData) {
					associationIteratorData->mIterator = extensionData->getIterator(indiInstItemRefCursor.getIndividualID(), moveOverCursor, mIterateAscending, true, mConsiderSameMergedIndis);
					clearInstanceIndividualAssociationsIteratorData(associationIteratorData);
					if (!associationIteratorData->mIterator.atEnd()) {
						moveToValidPosition(associationIteratorData);
						cint64 itIndiId = associationIteratorData->mIterator.currentAssociatedIndividualId();
						if (itIndiId != -1) {
							mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(itIndiId, associationIteratorData->mPossibleInstances), associationIteratorData);
						} else if (associationIteratorData->mInitializationRequired && !associationIteratorData->mIterator.atEnd()) {
							mUninitializedList.append(associationIteratorData);
							mInitializationRequired = true;
						}
					}
				}
				return associationIteratorData;
			}



			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId) {
				++mAssociatedIndiPossibleVisistedCount;
				++mAssociatedIndiPossibleConfirmedCount;
				return true;
			}


			void COptimizedRepresentativeKPSetCacheLabelItemIterator::clearInstanceIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData) {
				associationIteratorData->mCurrentIndiRealizationItem = nullptr;
				associationIteratorData->mCurrentInstanceOnlyPossible = false;
				associationIteratorData->mIndividual = nullptr;
				associationIteratorData->mAdditionalData = nullptr;
			}

			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::loadInstanceIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData) {
				cint64 indiId = associationIteratorData->mIterator.currentAssociatedIndividualId();
				if (!associationIteratorData->mCurrentIndiRealizationItem) {
					associationIteratorData->mCurrentIndiRealizationItem = mIndividualInstantiatedItemHash->getIndividualInstantiatedItem(indiId);
				}
				if (mIndividualVector && !associationIteratorData->mIndividual) {
					associationIteratorData->mIndividual = mIndividualVector->getData(indiId);
				}

				return false;
			}



			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::moveToValidPosition(CAssociationMapIteratorData* iteratorData) {
				if (iteratorData->mIterator.atEnd()) {
					return false;
				}
				clearInstanceIndividualAssociationsIteratorData(iteratorData);
				if (iteratorData->mPossibleInstances || !mConsiderSameMergedIndis) {
					bool foundNext = false;
					while (!iteratorData->mIterator.atEnd() && !foundNext) {

						iteratorData->mInitializationRequired = loadInstanceIndividualAssociationsIteratorData(iteratorData);
						if (iteratorData->mInitializationRequired) {
							return false;
						}

						cint64 indiId = iteratorData->mIterator.currentAssociatedIndividualId();
						iteratorData->mCurrentInstanceOnlyPossible = false;
						foundNext = isValidIteratorPosition(iteratorData, indiId);

						if (!foundNext && iteratorData->mInitializationRequired) {
							return false;
						}

						if (!foundNext) {
							iteratorData->mIterator.moveNext();
							clearInstanceIndividualAssociationsIteratorData(iteratorData);
						}
					}
					if (foundNext) {
						return true;
					}

				} else {
					// TODO: avoid individual item hash lookup with separate possible item individual associations
					loadInstanceIndividualAssociationsIteratorData(iteratorData);
					return true;
				}
				return false;
			}



			COptimizedRepresentativeKPSetCacheLabelItemIterator::CAssociationMapIteratorData* COptimizedRepresentativeKPSetCacheLabelItemIterator::getNextIteratorDataFromMap() {
				CAssociationMapIteratorData* iterationData = nullptr;
				while (!iterationData && !mCurrentIndiIdIterationDataMap.isEmpty()) {
					if (mIterateAscending) {
						QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::iterator it = mCurrentIndiIdIterationDataMap.begin();
						iterationData = *it;
						mCurrentIndiIdIterationDataMap.erase(it);
					} else {
						QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::iterator it = mCurrentIndiIdIterationDataMap.end();
						--it;
						iterationData = *it;
						mCurrentIndiIdIterationDataMap.erase(it);
					}


					if (iterationData && iterationData->mInitializationRequired) {

						cint64 prevIndiId = iterationData->mIterator.currentAssociatedIndividualId();
						if (iterationData->mLastCheckedInitializationRequiredId <= mCheckingInitializationRequiredId) {
							moveToValidPosition(iterationData);
							iterationData->mLastCheckedInitializationRequiredId = mCheckingInitializationRequiredId;
						}

						if (iterationData->mIterator.atEnd()) {
							iterationData = nullptr;
						} else {
							cint64 newIndiId = iterationData->mIterator.currentAssociatedIndividualId();
							if (prevIndiId != newIndiId) {
								if (mIterateAscending) {
									QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::const_iterator it = mCurrentIndiIdIterationDataMap.constBegin();
									if (it.key().mIndiId < newIndiId) {
										mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(newIndiId, iterationData->mPossibleInstances), iterationData);
										iterationData = nullptr;
									}
								} else {
									QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::const_iterator it = mCurrentIndiIdIterationDataMap.constEnd();
									--it;
									if (it.key().mIndiId > newIndiId) {
										mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(newIndiId, iterationData->mPossibleInstances), iterationData);
										iterationData = nullptr;
									}
								}
							}
						}
						
					}
				}
				return iterationData;
			}


			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::moveNext() {
				do {
					if (mCurrentIteratorData) {
						mInitializationRequired = false;
						bool sucessfullyMovedToValidPosition = true;
						do {
							mCurrentIteratorData->mIterator.moveNext();
							sucessfullyMovedToValidPosition = moveToValidPosition(mCurrentIteratorData);
						} while (mPreventSameIndisMultipleOccurences && !mCurrentIteratorData->mIterator.atEnd() && mCurrentIteratorData->mIterator.currentAssociatedIndividualId() == mLastIndiId);

						if (mCurrentIteratorData->mIterator.atEnd()) {
							mCurrentIteratorData = nullptr;
						}


						if (mCurrentIteratorData && mSorting.getType() == CRealizationIndividualSorting::SORT_TYPE_INDIVIDUAL_ID && !mCurrentIndiIdIterationDataMap.isEmpty()) {
							cint64 currentIndiId = mCurrentIteratorData->mIterator.currentAssociatedIndividualId();
							if (mIterateAscending) {
								QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::const_iterator it = mCurrentIndiIdIterationDataMap.constBegin();
								if (it.key().mIndiId < currentIndiId) {
									mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(currentIndiId, mCurrentIteratorData->mPossibleInstances), mCurrentIteratorData);
									mCurrentIteratorData = nullptr;
								}
							} else {
								QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*>::const_iterator it = mCurrentIndiIdIterationDataMap.constEnd();
								--it;
								if (it.key().mIndiId > currentIndiId) {
									mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(currentIndiId, mCurrentIteratorData->mPossibleInstances), mCurrentIteratorData);
									mCurrentIteratorData = nullptr;
								}
							}
						}

						if (mCurrentIteratorData && !sucessfullyMovedToValidPosition) {
							if (mCurrentIteratorData->mInitializationRequired) {
								mInitializationRequired = true;
								return false;
							} else {
								mCurrentIteratorData = nullptr;
							}
						}
					}
					if (!mCurrentIteratorData) {
						if (!mUninitializedList.isEmpty()) {
							mInitializationRequired = true;
						} else {
							mCurrentIteratorData = getNextIteratorDataFromMap();
						}
					}
				} while (mCurrentIteratorData && mPreventSameIndisMultipleOccurences && mLastIndiId == currentIndividualInstanceItemReference().getIndividualID());

				if (mCurrentIteratorData && mCurrentIteratorData->mInitializationRequired) {
					mInitializationRequired = true;
				}

				if (!mInitializationRequired) {
					if (mCurrentIteratorData) {
						if (mFirstIteratorIndiId == -1) {
							mFirstIteratorIndiId = currentIndividualInstanceItemReference().getIndividualID();
						}

						if (mLastIndiId >= 0) {
							mAverageIterationIncrementSize = (mAverageIterationIncrementSize * mIterationCount + getDifference(currentIndividualInstanceItemReference().getIndividualID(), mLastIndiId)) / (mIterationCount + 1);
							mIterationCount++;
						}

						mLastIndiId = currentIndividualInstanceItemReference().getIndividualID();
					} else {
						mLastIndiId = -1;
					}
				}

				return mCurrentIteratorData != nullptr && !mInitializationRequired;
			}


			cint64 COptimizedRepresentativeKPSetCacheLabelItemIterator::getDifference(cint64 number1, cint64 number2) {
				if (number1 > number2) {
					return qAbs(number2 - number1);
				} else {
					return qAbs(number1 - number2);
				}
			}


			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver) {

				function<bool(CAssociationMapIteratorData* iteratorData)> reachedCursorCheckFunc = [&](CAssociationMapIteratorData* iteratorData)->bool {
					if (moveOver) {
						return (mIterateAscending && iteratorData->mIterator.currentAssociatedIndividualId() > indiInstItemRef.getIndividualID()) || (!mIterateAscending && iteratorData->mIterator.currentAssociatedIndividualId() < indiInstItemRef.getIndividualID());
					} else {
						return (mIterateAscending && iteratorData->mIterator.currentAssociatedIndividualId() >= indiInstItemRef.getIndividualID()) || (!mIterateAscending && iteratorData->mIterator.currentAssociatedIndividualId() <= indiInstItemRef.getIndividualID());
					}
				};

				if (atEnd() || reachedCursorCheckFunc(mCurrentIteratorData)) {
					return !atEnd();
				} else {

					cint64 diff = getDifference(currentIndividualInstanceItemReference().getIndividualID(), indiInstItemRef.getIndividualID());
					double estimatedRequiredIterations = diff / (double)mAverageIterationIncrementSize;

					cint64 maxIterationWithoutMove = MAX_ITERATION_WITHOUT_MOVE_PER_LABEL* (mCurrentIndiIdIterationDataMap.size() + 1);
					while (!mInitializationRequired && estimatedRequiredIterations <= maxIterationWithoutMove && mCurrentIteratorData && !reachedCursorCheckFunc(mCurrentIteratorData)) {
						moveNext();

						if (mCurrentIteratorData && !mInitializationRequired) {
							diff = getDifference(currentIndividualInstanceItemReference().getIndividualID(), indiInstItemRef.getIndividualID());
							estimatedRequiredIterations = diff / (double)mAverageIterationIncrementSize;
						}
					}

					if (!atEnd() || mInitializationRequired) {
						cint64 prevLastIndiId = mLastIndiId;
						mLastIndiId = -1;
						if (mInitializationRequired || !reachedCursorCheckFunc(mCurrentIteratorData)) {
							mInitializationRequired = false;

							QList<CAssociationMapIteratorData*> tmpUninitializedList(mUninitializedList);
							mUninitializedList.clear();
							while (!tmpUninitializedList.isEmpty()) {
								CAssociationMapIteratorData* iteratorData = tmpUninitializedList.takeFirst();
								if (!reachedCursorCheckFunc(iteratorData)) {
									updateIndividualAssociationsIteratorData(iteratorData, indiInstItemRef, moveOver);
								}
							}


							while (mCurrentIteratorData && !reachedCursorCheckFunc(mCurrentIteratorData)) {
								// create new iteration data for those that are not at the cursor
								if (!reachedCursorCheckFunc(mCurrentIteratorData)) {
									updateIndividualAssociationsIteratorData(mCurrentIteratorData, indiInstItemRef, moveOver);
								}
								mCurrentIteratorData = nullptr;

								mCurrentIteratorData = getNextIteratorDataFromMap();
							}

							if (mCurrentIteratorData && mCurrentIteratorData->mInitializationRequired) {
								mInitializationRequired = true;
							}
						}
						if (mCurrentIteratorData) {
							if (!mInitializationRequired) {
								mLastIndiId = currentIndividualInstanceItemReference().getIndividualID();
							} else {
								mLastIndiId = prevLastIndiId;
							}
						}
					}
					

				}
				return !mInitializationRequired && !atEnd();
			}



			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::atEnd() {
				return mCurrentIteratorData == nullptr && mUninitializedList.isEmpty();
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetCacheLabelItemIterator::currentIndividualInstanceItemReference() {
				if (mCurrentIteratorData) {
					cint64 currentIndiId = mCurrentIteratorData->mIterator.currentAssociatedIndividualId();
					return CRealizationIndividualInstanceItemReference(CIndividualReference(mCurrentIteratorData->mIndividual, currentIndiId), mCurrentIteratorData->mCurrentIndiRealizationItem);
				}
				return CRealizationIndividualInstanceItemReference();
			}



			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::isCurrentInstancePossible() {
				if (mCurrentIteratorData) {
					return mCurrentIteratorData->mCurrentInstanceOnlyPossible;
				} else {
					return false;
				}
			}

			bool COptimizedRepresentativeKPSetCacheLabelItemIterator::isCurrentInstanceCertain() {
				if (mCurrentIteratorData) {
					return !mCurrentIteratorData->mCurrentInstanceOnlyPossible;
				} else {
					return false;
				}
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelItemIterator::getCopy() {
				COptimizedRepresentativeKPSetCacheLabelItemIterator* iteratorCopy = new COptimizedRepresentativeKPSetCacheLabelItemIterator(QList<CBackendRepresentativeMemoryLabelCacheItem*>(), QList<CBackendRepresentativeMemoryLabelCacheItem*>(), mIndividualInstantiatedItemHash, mIndividualVector, mSorting, mIndiInstItemRefCursor, mMoveOverCursor);
				iteratorCopy->init();
				this->initCopy(iteratorCopy);
				return iteratorCopy;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelItemIterator::initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				COptimizedRepresentativeKPSetCacheLabelItemIterator* cacheLabelItemIterator = (COptimizedRepresentativeKPSetCacheLabelItemIterator*)iterator;
				for (CAssociationMapIteratorData* iteratorData : mIteratorDataContainer) {
					CAssociationMapIteratorData* copiedIteratorData = createAssociationMapIteratorData(iteratorData->mLabelItem, iteratorData->mIterator, iteratorData->mPossibleInstances);
					copiedIteratorData->mLastCheckedInitializationRequiredId = iteratorData->mLastCheckedInitializationRequiredId;
					copiedIteratorData->mCurrentIndiRealizationItem = iteratorData->mCurrentIndiRealizationItem;
					copiedIteratorData->mIndividual = iteratorData->mIndividual;
					copiedIteratorData->mAdditionalData = iteratorData->mAdditionalData;
					copiedIteratorData->mInitializationRequired = iteratorData->mInitializationRequired;
					copiedIteratorData->mCurrentInstanceOnlyPossible = iteratorData->mCurrentInstanceOnlyPossible;
					cacheLabelItemIterator->mIteratorDataContainer.append(copiedIteratorData);
					if (mCurrentIteratorData == iteratorData) {
						cacheLabelItemIterator->mCurrentIteratorData = copiedIteratorData;
					} else if (!copiedIteratorData->mIterator.atEnd()) {
						if (copiedIteratorData->mInitializationRequired && copiedIteratorData->mIterator.currentAssociatedIndividualId() == -1) {
							cacheLabelItemIterator->mUninitializedList.append(copiedIteratorData);
						} else {
							cacheLabelItemIterator->mCurrentIndiIdIterationDataMap.insertMulti(CAssociationMapIteratorKey(copiedIteratorData), copiedIteratorData);
						}
					}
				}
				cacheLabelItemIterator->mInitializationRequired = mInitializationRequired;
				cacheLabelItemIterator->mFirstIteratorIndiId = mFirstIteratorIndiId;
				cacheLabelItemIterator->mAllowPossibleInstances = mAllowPossibleInstances;
				cacheLabelItemIterator->mPreventSameIndisMultipleOccurences = mPreventSameIndisMultipleOccurences;
				cacheLabelItemIterator->mAverageIterationIncrementSize = mAverageIterationIncrementSize;
				cacheLabelItemIterator->mIterationCount = mIterationCount;
				cacheLabelItemIterator->mLastIndiId = mLastIndiId;
				cacheLabelItemIterator->mLabelCacheItemKnwonInstancesList = mLabelCacheItemKnwonInstancesList;
				cacheLabelItemIterator->mLabelCacheItemPossibleInstancesList = mLabelCacheItemPossibleInstancesList;

				cacheLabelItemIterator->mAssociatedIndiPossibleVisistedCount = mAssociatedIndiPossibleVisistedCount;
				cacheLabelItemIterator->mAssociatedIndiPossibleConfirmedCount = mAssociatedIndiPossibleConfirmedCount;
				cacheLabelItemIterator->mAssociatedIndiTotalCount = mAssociatedIndiTotalCount;
				cacheLabelItemIterator->mAssociatedIndiPossibleCount = mAssociatedIndiPossibleCount;
				cacheLabelItemIterator->mCheckingInitializationRequiredId = mCheckingInitializationRequiredId;
				return this;
			}

			CRealizationRemainingInstancesEstimation COptimizedRepresentativeKPSetCacheLabelItemIterator::remainingInstancesEstimation() {
				CRealizationRemainingInstancesEstimation estimation;
				estimation.setMaximalCount(mAssociatedIndiTotalCount);
				estimation.setMinimalCount(mAssociatedIndiTotalCount - mAssociatedIndiPossibleCount);
				cint64 remainingEstimation = 0;
				if (!atEnd()) {
					if (mIterateAscending) {
						cint64 difference = getDifference(currentIndividualInstanceItemReference().getIndividualID(), mFirstIteratorIndiId);
						remainingEstimation = mAssociatedIndiTotalCount - (difference / mAverageIterationIncrementSize);
					} else {
						cint64 difference = getDifference(currentIndividualInstanceItemReference().getIndividualID(), 0);
						remainingEstimation = mAssociatedIndiTotalCount / mAverageIterationIncrementSize;
					}
					remainingEstimation = qMin(mAssociatedIndiTotalCount, remainingEstimation);
					remainingEstimation = qMax((cint64)0, remainingEstimation);
				}
				estimation.setEstimatedCount(remainingEstimation);
				return estimation;
			}



			CRealizationIndividualSorting COptimizedRepresentativeKPSetCacheLabelItemIterator::getSorting() {
				return mSorting;
			}

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
