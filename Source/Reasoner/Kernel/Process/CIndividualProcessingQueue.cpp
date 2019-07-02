/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CIndividualProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualProcessingQueue::CIndividualProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext),mIndiDesPriorityHash(processContext) {

					mIndiProDesCount = 0;
					mHasMaxIndiPriority = false;
					mLastCheckIndi = nullptr;
				}


				CIndividualProcessingQueue::~CIndividualProcessingQueue() {
				}

				CIndividualProcessingQueue* CIndividualProcessingQueue::initProcessingQueue(CIndividualProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
						mIndiDesPriorityHash = processingQueue->mIndiDesPriorityHash;
						mIndiProDesCount = processingQueue->mIndiProDesCount;
					} else {
						mPriorityIndiDesMap.clear();
						mIndiDesPriorityHash.clear();
						mIndiProDesCount = 0;
					}
					return this;
				}


				CIndividualProcessingQueue *CIndividualProcessingQueue::insertIndiviudalProcessDescriptor(CIndividualProcessNodeDescriptor *indiProDes) {
					CIndividualProcessNode *individual = indiProDes->getIndividual();
					CIndividualProcessNodePriority indiPriority = indiProDes->getProcessPriority();
					if (individual == mLastCheckIndi) {
						mLastCheckIndiPriority = qMax(indiPriority,mLastCheckIndiPriority);
					}
					cint64 indiID = individual->getIndividualID();
					if (mHasMaxIndiPriority) {
						mMaxIndiPriority = qMax(indiPriority,mMaxIndiPriority);
					}

					//cint64 prevCount = mPriorityIndiDesMap.count();

					mIndiDesPriorityHash.insert(indiID,indiPriority);
					mPriorityIndiDesMap.insertMulti(indiPriority,indiProDes);
					++mIndiProDesCount;

					//if (mPriorityIndiDesMap.count() != mIndiProDesCount) {
					//	mPriorityIndiDesMap.insertMulti(indiPriority,indiProDes);
					//	bool bug = true;
					//}

					return this;
				}


				bool CIndividualProcessingQueue::isIndividualQueued(CIndividualProcessNode *individual) {
					cint64 indiID = individual->getIndividualID();
					mLastCheckIndiPriority = mIndiDesPriorityHash.value(indiID,CIndividualProcessNodePriority());
					return !mLastCheckIndiPriority.isNullPriority();
				}


				bool CIndividualProcessingQueue::needsIndiviudalInsertion(CIndividualProcessNode *individual, CIndividualProcessNodePriority priority) {
					if (mLastCheckIndi == individual) {
						if (!mLastCheckIndiPriority.isNullPriority() && priority >= mLastCheckIndiPriority) {
							return false;
						} else {
							return true;
						}
					} else {
						cint64 indiID = individual->getIndividualID();
						mLastCheckIndi = individual;
						mLastCheckIndiPriority = mIndiDesPriorityHash.value(indiID,CIndividualProcessNodePriority());
						if (!mLastCheckIndiPriority.isNullPriority() && priority >= mLastCheckIndiPriority) {
							return false;
						}
						return true;
					}
				}


				bool CIndividualProcessingQueue::hasHigherPriorityIndividual(CIndividualProcessNodePriority priority) {
					if (!mHasMaxIndiPriority) {
						CIndividualProcessNodeDescriptor* nextIndiDes = getNextProcessIndividualDescriptor();
						if (nextIndiDes) {
							mMaxIndiPriority = nextIndiDes->getProcessPriority();
							mHasMaxIndiPriority = true;
						}
					}
					if (mHasMaxIndiPriority) {
						return mMaxIndiPriority < priority;
					}
					return false;
				}


				CIndividualProcessNodeDescriptor* CIndividualProcessingQueue::takeNextProcessIndividualDescriptor() {
					CIndividualProcessNodeDescriptor* indiProDes = nullptr;
					if (mIndiProDesCount > 0) {
						--mIndiProDesCount;
						CPROCESSMAP<CIndividualProcessNodePriority,CIndividualProcessNodeDescriptor*>::iterator it = mPriorityIndiDesMap.begin();
						indiProDes = it.value();
						mPriorityIndiDesMap.erase(it);
						cint64 indiID = indiProDes->getIndividual()->getIndividualID();
						mIndiDesPriorityHash.insert(indiID,0.);
						mLastCheckIndi = nullptr;

						//if (mPriorityIndiDesMap.count() != mIndiProDesCount) {
						//	bool bug = true;
						//}
					}
					mHasMaxIndiPriority = false;
					return indiProDes;
				}

				CIndividualProcessNodeDescriptor* CIndividualProcessingQueue::getNextProcessIndividualDescriptor() {
					CIndividualProcessNodeDescriptor* indiProDes = nullptr;
					if (mIndiProDesCount > 0) {
						CPROCESSMAP<CIndividualProcessNodePriority,CIndividualProcessNodeDescriptor*>::iterator it = mPriorityIndiDesMap.begin();
						indiProDes = it.value();
					}
					return indiProDes;
				}

				bool CIndividualProcessingQueue::isEmpty() {
					return mIndiProDesCount <= 0;
				}

				bool CIndividualProcessingQueue::hasIndividualProcessDescriptor() {
					return mIndiProDesCount > 0;
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
