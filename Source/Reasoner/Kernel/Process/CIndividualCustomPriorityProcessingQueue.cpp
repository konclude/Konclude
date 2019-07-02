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

#include "CIndividualCustomPriorityProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualCustomPriorityProcessingQueue::CIndividualCustomPriorityProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiMap(processContext) {

				}

				CIndividualCustomPriorityProcessingQueue* CIndividualCustomPriorityProcessingQueue::initProcessingQueue(CIndividualCustomPriorityProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiMap = processingQueue->mPriorityIndiMap;
					} else {
						mPriorityIndiMap.clear();
					}
					return this;
				}


				CIndividualProcessNode* CIndividualCustomPriorityProcessingQueue::takeNextProcessIndividual() {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiMap.isEmpty()) {
						CPROCESSMAP<double,CIndividualProcessNode*>::iterator it = mPriorityIndiMap.begin();
						nextNode = it.value();
						it = mPriorityIndiMap.erase(it);
					}
					return nextNode;
				}

				CIndividualProcessNode* CIndividualCustomPriorityProcessingQueue::getNextProcessIndividual() {
					CIndividualProcessNode* nextNode = nullptr;
					if (!mPriorityIndiMap.isEmpty()) {
						CPROCESSMAP<double,CIndividualProcessNode*>::iterator it = mPriorityIndiMap.begin();
						nextNode = it.value();
					}
					return nextNode;
				}

				CIndividualCustomPriorityProcessingQueue* CIndividualCustomPriorityProcessingQueue::insertIndiviudal(double priority, CIndividualProcessNode* individual) {
					mPriorityIndiMap.insertMulti(priority,individual);
					return this;
				}


				bool CIndividualCustomPriorityProcessingQueue::isEmpty() {
					return mPriorityIndiMap.count() <= 0;
				}

				cint64 CIndividualCustomPriorityProcessingQueue::getQueuedIndividualCount() {
					return mPriorityIndiMap.count();
				}

				bool CIndividualCustomPriorityProcessingQueue::hasQueuedIndividuals() {
					return mPriorityIndiMap.count() > 0;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
