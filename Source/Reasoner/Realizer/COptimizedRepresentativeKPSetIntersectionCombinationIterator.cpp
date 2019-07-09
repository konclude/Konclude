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

#include "COptimizedRepresentativeKPSetIntersectionCombinationIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetIntersectionCombinationIterator::COptimizedRepresentativeKPSetIntersectionCombinationIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> iteratorList) {
				mIteratorList = iteratorList;

				for (CRealizationIndividualInstanceItemReferenceIterator* iterator : mIteratorList) {
					CRealizationIteratorData* iteratorData = new CRealizationIteratorData(iterator);
					mIteratorDataList.append(iteratorData);
				}

				mVisitedIterationCount = 0;
				mIntersectionIterationCount = 0;

				mIteratingIteratorData = nullptr;


				init();

				moveToValidPosition();
			}


			COptimizedRepresentativeKPSetIntersectionCombinationIterator::~COptimizedRepresentativeKPSetIntersectionCombinationIterator() {
				qDeleteAll(mIteratorList);
				qDeleteAll(mIteratorDataList);
			}


			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::init() {
				bool requiresInitialization = false;
				for (CRealizationIteratorData* iterationData : mIteratorDataList) {
					if (iterationData->mIterator->currentIndividualInstanceItemReference().getIndividualID() == -1) {
						if (iterationData->mIterator->requiresInitialization()) {
							requiresInitialization = true;
							break;
						}
					}
				}


				if (!requiresInitialization) {
					cint64 currentMinMaxCount = 0;
					for (CRealizationIteratorData* iterationData : mIteratorDataList) {
						cint64 itMaxCount = iterationData->mIterator->remainingInstancesEstimation().getMaximalCount();
						if (!mIteratingIteratorData || itMaxCount < currentMinMaxCount) {
							currentMinMaxCount = itMaxCount;
							mIteratingIteratorData = iterationData;
						}
					}
				}
				mIterateAscending = true;
				if (mIteratingIteratorData) {
					mIterateAscending = mIteratingIteratorData->mIterator->getSorting().getOrder() != CRealizationIndividualSorting::SORT_ORDER_DESCENDING;
					return true;
				}
				return false;
			}



			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::requiresInitialization() {
				if (mIteratingIteratorData) {
					if (!mIteratingIteratorData->mIterator->atEnd()) {
						if (mIteratingIteratorData->mIterator->requiresInitialization()) {
							return true;
						}
						cint64 currentId = mIteratingIteratorData->mIterator->currentIndividualInstanceItemReference().getIndividualID();
						for (CRealizationIteratorData* iteratorData : mIteratorDataList) {
							if (iteratorData != mIteratingIteratorData) {
								if (iteratorData->mIterator->currentIndividualInstanceItemReference().getIndividualID() == currentId) {
									if (iteratorData->mIterator->requiresInitialization()) {
										return true;
									}
								}
							}
						}
					}
				} else {
					for (CRealizationIteratorData* iterationData : mIteratorDataList) {
						if (iterationData->mIterator->requiresInitialization()) {
							return true;
						}
					}
				}
				return false;
			}

			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::begin() {

				bool requiredInitializiation = false;
				bool iteratorMoved = false;
				bool moreRequiredInitialization = false;

				if (requiresInitialization()) {
					for (CRealizationIteratorData* iteratorData : mIteratorDataList) {
						if (iteratorData->mIterator->requiresInitialization()) {
							cint64 prevIndiId = iteratorData->mIterator->currentIndividualInstanceItemReference().getIndividualID();
							requiredInitializiation = true;
							iteratorData->mIterator->begin();
							cint64 newIndiId = iteratorData->mIterator->currentIndividualInstanceItemReference().getIndividualID();
							if (newIndiId != prevIndiId) {
								iteratorMoved = true;
								if (newIndiId == -1 && !iteratorData->mIterator->atEnd()) {
									moreRequiredInitialization = true;
								}
							}
						}
					}
				}

				if (!mIteratingIteratorData) {
					init();
				}			

				if ((iteratorMoved && !moreRequiredInitialization ) || !requiresInitialization()) {
					if (moveToValidPosition()) {
						if (requiredInitializiation && !requiresInitialization()) {
							++mIntersectionIterationCount;
						}
					}
				}
				return true;
			}





			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::moveNext() {
				bool successfullyMoved = true;

				if (mIteratingIteratorData && !mIteratingIteratorData->mIterator->atEnd()) {
					bool moved = mIteratingIteratorData->mIterator->moveNext();
					if (moved) {
						++mVisitedIterationCount;
						CRealizationIndividualInstanceItemReference indiInstItemRef = mIteratingIteratorData->mIterator->currentIndividualInstanceItemReference();
						for (CRealizationIteratorData* iterationData : mIteratorDataList) {
							if (iterationData != mIteratingIteratorData) {
								iterationData->mIterator->moveTo(indiInstItemRef, false);
							}
						}

						successfullyMoved = moveToValidPosition();
						if (successfullyMoved) {
							++mIntersectionIterationCount;
						}

					} else {
						successfullyMoved = false;
					}
				}
				return successfullyMoved;
			}



			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::moveToValidPosition() {
				bool successfullyMoved = false;
				bool validIteratorPosition = false;
				bool positionRequiresInitialization = false;
				while ((!validIteratorPosition || !successfullyMoved) && !atEnd() && !positionRequiresInitialization) {
					successfullyMoved = true;
					positionRequiresInitialization = false;
					validIteratorPosition = true;


					cint64 minIndiId = CINT64_MAX;
					cint64 maxIndiId = CINT64_MIN;
					CRealizationIndividualInstanceItemReference minIndiItemRef;
					CRealizationIndividualInstanceItemReference maxIndiItemRef;

					CRealizationIndividualInstanceItemReference indiInstItemRef = mIteratingIteratorData->mIterator->currentIndividualInstanceItemReference();
					for (CRealizationIteratorData* iterationData : mIteratorDataList) {
						if (iterationData != mIteratingIteratorData) {
							CRealizationIndividualInstanceItemReference iterIndiInstItemRef = iterationData->mIterator->currentIndividualInstanceItemReference();
							if (mIterateAscending && iterIndiInstItemRef.getIndividualID() < indiInstItemRef.getIndividualID() || !mIterateAscending && iterIndiInstItemRef.getIndividualID() > indiInstItemRef.getIndividualID()) {
								if (!iterationData->mIterator->moveTo(indiInstItemRef, false)) {
									successfullyMoved = false;
								}
								cint64 movedIndiId = iterationData->mIterator->currentIndividualInstanceItemReference().getIndividualID();
								if (movedIndiId < minIndiId) {
									minIndiId = movedIndiId;
									minIndiItemRef = iterationData->mIterator->currentIndividualInstanceItemReference();
								}
								if (movedIndiId > maxIndiId) {
									maxIndiId = movedIndiId;
									maxIndiItemRef = iterationData->mIterator->currentIndividualInstanceItemReference();
								}
								if (movedIndiId != indiInstItemRef.getIndividualID()) {
									validIteratorPosition = false;
								} else if (iterationData->mIterator->requiresInitialization()) {
									positionRequiresInitialization = true;
								}
							} else if (mIterateAscending && iterIndiInstItemRef.getIndividualID() > indiInstItemRef.getIndividualID() || !mIterateAscending && iterIndiInstItemRef.getIndividualID() < indiInstItemRef.getIndividualID()) {
								if (iterIndiInstItemRef.getIndividualID() < minIndiId) {
									minIndiId = iterIndiInstItemRef.getIndividualID();
									minIndiItemRef = iterIndiInstItemRef;
								}
								if (iterIndiInstItemRef.getIndividualID() > maxIndiId) {
									maxIndiId = iterIndiInstItemRef.getIndividualID();
									maxIndiItemRef = iterIndiInstItemRef;
								}
								validIteratorPosition = false;
							}
						}
					}
					if (!validIteratorPosition) {
						if (minIndiId != CINT64_MAX && maxIndiId != CINT64_MIN && minIndiId != -1 && maxIndiId != -1) {
							cint64 moveToIndiId = maxIndiId;
							CRealizationIndividualInstanceItemReference moveToIndiItemRef = maxIndiItemRef;
							if (!mIterateAscending) {
								moveToIndiId = minIndiId;
								moveToIndiItemRef = minIndiItemRef;
							}
							++mVisitedIterationCount;
							mIteratingIteratorData->mIterator->moveTo(moveToIndiItemRef, false);
						} else {
							if (!mIteratingIteratorData->mIterator->moveNext()) {
								successfullyMoved = false;
							} else {
								++mVisitedIterationCount;
							}
						}
					} else {
						if (requiresInitialization()) {
							positionRequiresInitialization = true;
						}						
					}
				}
				return validIteratorPosition && successfullyMoved && !positionRequiresInitialization;
			}



			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver) {
				bool successfullyMoved = true;
				for (CRealizationIteratorData* iterationData : mIteratorDataList) {
					if (!iterationData->mIterator->moveTo(indiInstItemRef, moveOver)) {
						successfullyMoved = false;
					}
				}
				if (successfullyMoved) {
					++mVisitedIterationCount;
					if (!moveToValidPosition()) {
						successfullyMoved = false;
					}
				}
				return successfullyMoved;
			}



			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::atEnd() {
				return !mIteratingIteratorData || mIteratingIteratorData->mIterator->atEnd();
			}



			CRealizationIndividualInstanceItemReference COptimizedRepresentativeKPSetIntersectionCombinationIterator::currentIndividualInstanceItemReference() {
				if (mIteratingIteratorData) {
					return mIteratingIteratorData->mIterator->currentIndividualInstanceItemReference();
				} else {
					return CRealizationIndividualInstanceItemReference();
				}
			}



			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::isCurrentInstancePossible() {
				for (CRealizationIteratorData* iterationData : mIteratorDataList) {
					if (iterationData->mIterator->isCurrentInstancePossible()) {
						return true;
					}
				}
				return false;
			}

			bool COptimizedRepresentativeKPSetIntersectionCombinationIterator::isCurrentInstanceCertain() {
				for (CRealizationIteratorData* iterationData : mIteratorDataList) {
					if (!iterationData->mIterator->isCurrentInstanceCertain()) {
						return false;
					}
				}
				return true;
			}



			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetIntersectionCombinationIterator::getCopy() {
				QList<CRealizationIndividualInstanceItemReferenceIterator*> copiedIteratorList;
				for (CRealizationIndividualInstanceItemReferenceIterator* iterator : mIteratorList) {
					CRealizationIndividualInstanceItemReferenceIterator* copiedIterator = iterator->getCopy();
					copiedIteratorList.append(copiedIterator);
				}
				COptimizedRepresentativeKPSetIntersectionCombinationIterator* iteratorCopy = new COptimizedRepresentativeKPSetIntersectionCombinationIterator(copiedIteratorList);
				iteratorCopy->mVisitedIterationCount = mVisitedIterationCount;
				iteratorCopy->mIntersectionIterationCount = mIntersectionIterationCount;
				return iteratorCopy;
			}


			CRealizationRemainingInstancesEstimation COptimizedRepresentativeKPSetIntersectionCombinationIterator::remainingInstancesEstimation() {
				CRealizationRemainingInstancesEstimation estimation;

				cint64 minTotalCount = 0;
				cint64 maxTotalCount = -1;
				for (CRealizationIteratorData* iterationData : mIteratorDataList) {
					if (maxTotalCount == -1) {
						maxTotalCount = iterationData->mIterator->remainingInstancesEstimation().getMaximalCount();
					} else {
						maxTotalCount = qMin(maxTotalCount, iterationData->mIterator->remainingInstancesEstimation().getMaximalCount());
					}
				}


				estimation.setMaximalCount(maxTotalCount);
				estimation.setMinimalCount(minTotalCount);
				cint64 remainingEstimation = 0;
				if (!atEnd()) {
					remainingEstimation = mIteratingIteratorData->mIterator->remainingInstancesEstimation().getEstimatedCount() * ((double)mIntersectionIterationCount / mVisitedIterationCount);
					remainingEstimation = qMin(remainingEstimation, maxTotalCount);
					remainingEstimation = qMax(remainingEstimation, (cint64)0);
				}
				estimation.setEstimatedCount(remainingEstimation);
				return estimation;
			}




			CRealizationIndividualSorting COptimizedRepresentativeKPSetIntersectionCombinationIterator::getSorting() {
				if (mIteratingIteratorData) {
					return mIteratingIteratorData->mIterator->getSorting();
				} else {
					return CRealizationIndividualSorting();
				}
			}


			QList<CRealizationIndividualInstanceItemReferenceIterator*>* COptimizedRepresentativeKPSetIntersectionCombinationIterator::getCombinedIteratorList() {
				return &mIteratorList;
			}



		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
