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

#include "CIndividualDepthProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualDepthProcessingQueue::CIndividualDepthProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext) {

				}

				CIndividualDepthProcessingQueue* CIndividualDepthProcessingQueue::initProcessingQueue(CIndividualDepthProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
						mHasMaxIndiPriority = processingQueue->mHasMaxIndiPriority;
						mMaxIndiPriority = processingQueue->mMaxIndiPriority;
						mLastCheckIndi = processingQueue->mLastCheckIndi;
					} else {
						mHasMaxIndiPriority = false;
						mPriorityIndiDesMap.clear();
						mLastCheckIndi = nullptr;
						mMaxIndiPriority = CIndividualDepthPriority();
					}
					return this;
				}


				CIndividualProcessNode* CIndividualDepthProcessingQueue::takeNextProcessIndividual() {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value();
						it = mPriorityIndiDesMap.erase(it);
						if (it != mPriorityIndiDesMap.end()) {
							mHasMaxIndiPriority = true;
							mMaxIndiPriority = it.key();
						} else {
							mHasMaxIndiPriority = false;
						}
					}
					return nextNode;
				}

				CIndividualProcessNode* CIndividualDepthProcessingQueue::getNextProcessIndividual() {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value();
					}
					return nextNode;
				}

				CIndividualDepthProcessingQueue* CIndividualDepthProcessingQueue::insertProcessIndiviudal(CIndividualProcessNode* individual) {
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualID());
					if (mHasMaxIndiPriority) {
						if (priority < mMaxIndiPriority) {
							mMaxIndiPriority = priority;
						}
					} else {
						mMaxIndiPriority = priority;
						mHasMaxIndiPriority = true;
					}
					mPriorityIndiDesMap.insert(priority,individual);
					return this;
				}


				bool CIndividualDepthProcessingQueue::isEmpty() {
					return mPriorityIndiDesMap.count() <= 0;
				}

				cint64 CIndividualDepthProcessingQueue::getQueuedIndividualCount() {
					return mPriorityIndiDesMap.count();
				}

				bool CIndividualDepthProcessingQueue::hasQueuedIndividuals() {
					return mPriorityIndiDesMap.count() > 0;
				}

				bool CIndividualDepthProcessingQueue::isIndividualQueued(CIndividualProcessNode *individual) {
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualID());
					return mPriorityIndiDesMap.contains(priority);
				}

				bool CIndividualDepthProcessingQueue::hasHigherPriorityIndividual(CIndividualProcessNode *individual) {
					if (mHasMaxIndiPriority) {
						CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualID());
						return mMaxIndiPriority < priority;
					}
					return false;
				}

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
