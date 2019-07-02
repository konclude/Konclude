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

#include "CSaturationSuccessorExtensionIndividualNodeProcessingQueue.h"
#include "CIndividualSaturationProcessNode.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationSuccessorExtensionIndividualNodeProcessingQueue::CSaturationSuccessorExtensionIndividualNodeProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext) {

					mCurrentIndividual = nullptr;
				}

				CSaturationSuccessorExtensionIndividualNodeProcessingQueue* CSaturationSuccessorExtensionIndividualNodeProcessingQueue::initProcessingQueue(CSaturationSuccessorExtensionIndividualNodeProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
						mCurrentIndividual = processingQueue->mCurrentIndividual;
					} else {
						mPriorityIndiDesMap.clear();
						mCurrentIndividual = nullptr;
					}
					return this;
				}


				CIndividualSaturationProcessNode* CSaturationSuccessorExtensionIndividualNodeProcessingQueue::takeNextToCurrentProcessIndividual() {
					if (!mCurrentIndividual) {
						CIndividualSaturationProcessNode* nextNode = nullptr;
						if (!mPriorityIndiDesMap.isEmpty()) {
							CPROCESSMAP<cint64,CIndividualSaturationProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
							nextNode = it.value();
							it = mPriorityIndiDesMap.erase(it);
						}
						mCurrentIndividual = nextNode;
					}
					return mCurrentIndividual;
				}

				CSaturationSuccessorExtensionIndividualNodeProcessingQueue* CSaturationSuccessorExtensionIndividualNodeProcessingQueue::insertProcessIndiviudal(CIndividualSaturationProcessNode* individual) {
					if (mCurrentIndividual) {
						if (mCurrentIndividual->getIndividualID() == individual->getIndividualID()) {
							return this;
						}
					}
					mPriorityIndiDesMap.insert(-individual->getIndividualID(),individual);
					return this;
				}


				bool CSaturationSuccessorExtensionIndividualNodeProcessingQueue::isEmpty() {
					return !mCurrentIndividual && mPriorityIndiDesMap.count() <= 0;
				}

				cint64 CSaturationSuccessorExtensionIndividualNodeProcessingQueue::getQueuedIndividualCount() {
					cint64 processingCount = mPriorityIndiDesMap.count();
					if (mCurrentIndividual) {
						processingCount += 1;
					}
					return processingCount;
				}

				bool CSaturationSuccessorExtensionIndividualNodeProcessingQueue::hasQueuedIndividuals() {
					return mCurrentIndividual || mPriorityIndiDesMap.count() > 0;
				}

				bool CSaturationSuccessorExtensionIndividualNodeProcessingQueue::isIndividualQueued(CIndividualSaturationProcessNode *individual) {
					if (mCurrentIndividual) {
						if (mCurrentIndividual->getIndividualID() == individual->getIndividualID()) {
							return true;
						}
					}
					return mPriorityIndiDesMap.contains(-individual->getIndividualID());
				}

				CIndividualSaturationProcessNode* CSaturationSuccessorExtensionIndividualNodeProcessingQueue::getCurrentProcessIndividual() {
					return mCurrentIndividual;
				}

				CSaturationSuccessorExtensionIndividualNodeProcessingQueue* CSaturationSuccessorExtensionIndividualNodeProcessingQueue::clearCurrentProcessIndividual() {
					mCurrentIndividual = nullptr;
					return this;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
