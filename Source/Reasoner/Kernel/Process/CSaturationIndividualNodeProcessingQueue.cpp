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

#include "CSaturationIndividualNodeProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CSaturationIndividualNodeProcessingQueue::CSaturationIndividualNodeProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext) {

				}

				CSaturationIndividualNodeProcessingQueue* CSaturationIndividualNodeProcessingQueue::initProcessingQueue(CSaturationIndividualNodeProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
					} else {
						mPriorityIndiDesMap.clear();
					}
					return this;
				}


				CIndividualSaturationProcessNode* CSaturationIndividualNodeProcessingQueue::takeNextProcessIndividual() {
					CIndividualSaturationProcessNode* nextNode = nullptr;
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<cint64,CIndividualSaturationProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value();
						it = mPriorityIndiDesMap.erase(it);
					}
					return nextNode;
				}

				CIndividualSaturationProcessNode* CSaturationIndividualNodeProcessingQueue::getNextProcessIndividual() {
					CIndividualSaturationProcessNode* nextNode = nullptr;
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<cint64,CIndividualSaturationProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value();
					}
					return nextNode;
				}

				CSaturationIndividualNodeProcessingQueue* CSaturationIndividualNodeProcessingQueue::insertProcessIndiviudal(CIndividualSaturationProcessNode* individual) {
					mPriorityIndiDesMap.insert(-individual->getIndividualID(),individual);
					return this;
				}


				bool CSaturationIndividualNodeProcessingQueue::isEmpty() {
					return mPriorityIndiDesMap.count() <= 0;
				}

				cint64 CSaturationIndividualNodeProcessingQueue::getQueuedIndividualCount() {
					return mPriorityIndiDesMap.count();
				}

				bool CSaturationIndividualNodeProcessingQueue::hasQueuedIndividuals() {
					return mPriorityIndiDesMap.count() > 0;
				}

				bool CSaturationIndividualNodeProcessingQueue::isIndividualQueued(CIndividualSaturationProcessNode *individual) {
					return mPriorityIndiDesMap.contains(-individual->getIndividualID());
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
