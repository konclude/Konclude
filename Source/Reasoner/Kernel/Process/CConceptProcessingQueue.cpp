/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CConceptProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CConceptProcessingQueue::CConceptProcessingQueue(CProcessContext* processContext) : mProcessContext(processContext) {
					mMaxPriorityIndex = -1;
					mDesCount = 0;
				}


				CConceptProcessingQueue::~CConceptProcessingQueue() {
				}


				CConceptProcessingQueue* CConceptProcessingQueue::initProcessingQueue(CConceptProcessingQueue* processingQueue) {
					if (processingQueue) {
						mMaxPriorityIndex = processingQueue->mMaxPriorityIndex;
						mDesCount = processingQueue->mDesCount;
						for (cint64 i = 0; i < mMaxIndex; ++i) {
							mPriorityVec[i] = processingQueue->mPriorityVec[i];
							mPriorityVec[i].mPrevSortedPriorityDescriptorLinker = processingQueue->mPriorityVec[i].mSortedPriorityDescriptorLinker;
						}
					} else {
						mMaxPriorityIndex = -1;
						mDesCount = 0;
						for (cint64 i = 0; i < mMaxIndex; ++i) {
							CConceptProcessingPriorityQueueData& queueData = mPriorityVec[i];
							queueData.mDefaultPriorityDescriptorLinker = nullptr;
							queueData.mSortedPriorityDescriptorLinker = nullptr;
							queueData.mPrevSortedPriorityDescriptorLinker = nullptr;
						}
					}
					return this;
				}


				CConceptProcessingQueue* CConceptProcessingQueue::resetProcessingQueueModification() {
					for (cint64 i = 0; i < mMaxIndex; ++i) {
						CConceptProcessingPriorityQueueData& queueData = mPriorityVec[i];
						queueData.mPrevSortedPriorityDescriptorLinker = nullptr;
					}
					return this;
				}


				qint64 CConceptProcessingQueue::getDescriptorCount() {
					return mDesCount;
				}

				bool CConceptProcessingQueue::hasProcessDescriptor() {
					return mDesCount > 0;
				}

				bool CConceptProcessingQueue::isEmpty() {
					return mDesCount <= 0;
				}


				CConceptProcessingQueue *CConceptProcessingQueue::reinsertConceptProcessDescriptor(CConceptProcessDescriptor *conProDes) {
					CConceptProcessPriority conPriority = conProDes->getProcessPriority();
					double priority = conPriority.getPriority();
					cint64 priorityIndex = priority;
					++mDesCount;
					mMaxPriorityIndex = qMax(mMaxPriorityIndex,priorityIndex);
					CConceptProcessingPriorityQueueData& proPriQueData = mPriorityVec[priorityIndex];
					if (priority == (double)priorityIndex) {
						proPriQueData.mDefaultPriorityDescriptorLinker = conProDes;
					} else {
						proPriQueData.mSortedPriorityDescriptorLinker = conProDes;
						if (proPriQueData.mPrevSortedPriorityDescriptorLinker && proPriQueData.mPrevSortedPriorityDescriptorLinker == conProDes->getNext()) {
							proPriQueData.mPrevSortedPriorityDescriptorLinker = conProDes;
						}
					}
					return this;
				}


				CConceptProcessingQueue* CConceptProcessingQueue::insertConceptProcessDescriptor(CConceptProcessDescriptor *conProDes) {
					CConceptProcessPriority conPriority = conProDes->getProcessPriority();
					double priority = conPriority.getPriority();
					cint64 priorityIndex = priority;
					++mDesCount;
					mMaxPriorityIndex = qMax(mMaxPriorityIndex,priorityIndex);
					CConceptProcessingPriorityQueueData& proPriQueData = mPriorityVec[priorityIndex];
					if (priority == (double)priorityIndex) {
						proPriQueData.mDefaultPriorityDescriptorLinker = conProDes->append(proPriQueData.mDefaultPriorityDescriptorLinker);
					} else {
						CConceptProcessDescriptor* sortedDesLinker = proPriQueData.mSortedPriorityDescriptorLinker;
						if (!sortedDesLinker || *conProDes <= *sortedDesLinker) {
							// insert to begin
							proPriQueData.mSortedPriorityDescriptorLinker = conProDes->append(proPriQueData.mSortedPriorityDescriptorLinker);
						} else {
							CConceptProcessDescriptor* lastSortedDesLinker = nullptr;
							while (sortedDesLinker && !(*conProDes <= *sortedDesLinker)) {
								if (sortedDesLinker == proPriQueData.mPrevSortedPriorityDescriptorLinker) {
									// make descriptor local
									CConceptProcessDescriptor* newLocalDescriptor = CObjectAllocator<CConceptProcessDescriptor>::allocateAndConstruct(CContext::getMemoryAllocationManager(mProcessContext));
									newLocalDescriptor->initCopy(sortedDesLinker);

									proPriQueData.mPrevSortedPriorityDescriptorLinker = proPriQueData.mPrevSortedPriorityDescriptorLinker->getNext();
									sortedDesLinker = sortedDesLinker->getNext();

									if (lastSortedDesLinker) {
										lastSortedDesLinker->setNext(newLocalDescriptor);
									} else {
										proPriQueData.mSortedPriorityDescriptorLinker = newLocalDescriptor;
									}
									lastSortedDesLinker = newLocalDescriptor;
								} else {
									lastSortedDesLinker = sortedDesLinker;
									sortedDesLinker = sortedDesLinker->getNext();
								}
							}
							if (lastSortedDesLinker) {
								conProDes->append(sortedDesLinker);
								lastSortedDesLinker->setNext(conProDes);
							}
						}
					}
					return this;
				}


				CConceptProcessDescriptor* CConceptProcessingQueue::takeNextConceptDescriptorProcess() {
					CConceptProcessDescriptor* conProDes = nullptr;
					while (mDesCount > 0 && !conProDes && mMaxPriorityIndex >= 0) {
						CConceptProcessingPriorityQueueData& proPriQueData = mPriorityVec[mMaxPriorityIndex];
						conProDes = proPriQueData.mDefaultPriorityDescriptorLinker;
						if (conProDes) {
							proPriQueData.mDefaultPriorityDescriptorLinker = conProDes->getNext();
						} else {
							conProDes = proPriQueData.mSortedPriorityDescriptorLinker;
							if (conProDes) {
								if (conProDes == proPriQueData.mPrevSortedPriorityDescriptorLinker) {
									proPriQueData.mPrevSortedPriorityDescriptorLinker = proPriQueData.mPrevSortedPriorityDescriptorLinker->getNext();
								}
								proPriQueData.mSortedPriorityDescriptorLinker = conProDes->getNext();
							} else {
								--mMaxPriorityIndex;
							}
						}
					}
					if (conProDes) {
						--mDesCount;
					}
					return conProDes;
				}


				bool CConceptProcessingQueue::getNextConceptProcessPriority(CConceptProcessPriority* priority) {
					CConceptProcessDescriptor* conProDes = nullptr;
					if (mDesCount <= 0) {
						return false;
					}
					while (!conProDes && mMaxPriorityIndex >= 0) {
						CConceptProcessingPriorityQueueData& proPriQueData = mPriorityVec[mMaxPriorityIndex];
						conProDes = proPriQueData.mDefaultPriorityDescriptorLinker;
						if (!conProDes) {
							conProDes = proPriQueData.mSortedPriorityDescriptorLinker;
							if (!conProDes) {
								--mMaxPriorityIndex;
							}
						}
					}
					*priority = conProDes->getProcessPriority();
					return true;
				}

				CConceptProcessingQueueIterator CConceptProcessingQueue::getConceptProcessingQueueIterator() {
					return CConceptProcessingQueueIterator(mPriorityVec,mDesCount);
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
