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

#include "CIndividualDepthConceptProcessDescriptorProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualDepthConceptProcessDescriptorProcessingQueue::CIndividualDepthConceptProcessDescriptorProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext) {

				}

				CIndividualDepthConceptProcessDescriptorProcessingQueue* CIndividualDepthConceptProcessDescriptorProcessingQueue::initProcessingQueue(CIndividualDepthConceptProcessDescriptorProcessingQueue* processingQueue) {
					if (processingQueue) {
						mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
					} else {
						mPriorityIndiDesMap.clear();
					}
					return this;
				}


				bool CIndividualDepthConceptProcessDescriptorProcessingQueue::takeNextProcessIndiviudalConceptProcessDescriptor(CIndividualProcessNode*& nextNode, CConceptProcessDescriptor*& conProDes) {
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority,CIndividualDepthConceptProcessDescriptorProcessingQueueData>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value().mIndiNode;
						conProDes = it.value().mConProDes;
						it = mPriorityIndiDesMap.erase(it);
						return true;
					}
					return false;
				}


				bool CIndividualDepthConceptProcessDescriptorProcessingQueue::getNextProcessIndiviudalConceptProcessDescriptor(CIndividualProcessNode*& nextNode, CConceptProcessDescriptor*& conProDes) {
					if (!mPriorityIndiDesMap.isEmpty()) {
						CPROCESSMAP<CIndividualDepthPriority, CIndividualDepthConceptProcessDescriptorProcessingQueueData>::iterator it = mPriorityIndiDesMap.begin();
						nextNode = it.value().mIndiNode;
						conProDes = it.value().mConProDes;
						return true;
					}
					return false;
				}

				CIndividualDepthConceptProcessDescriptorProcessingQueue* CIndividualDepthConceptProcessDescriptorProcessingQueue::insertProcessIndiviudalConceptProcessDescriptor(CIndividualProcessNode* individual, CConceptProcessDescriptor* conProDes) {
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualNodeID());
					if (conProDes->getProcessingRestrictionSpecification()) {
						mPriorityIndiDesMap.insertMulti(priority, CIndividualDepthConceptProcessDescriptorProcessingQueueData(conProDes, individual));
					} else {
						mPriorityIndiDesMap.insert(priority, CIndividualDepthConceptProcessDescriptorProcessingQueueData(conProDes, individual));
					}
					return this;
				}


				CConceptProcessDescriptor* CIndividualDepthConceptProcessDescriptorProcessingQueue::removeQueuedProcessIndiviudal(CIndividualProcessNode* individual) {
					CConceptProcessDescriptor* conProDes = nullptr;
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualNodeID());
					CPROCESSMAP<CIndividualDepthPriority, CIndividualDepthConceptProcessDescriptorProcessingQueueData>::iterator it = mPriorityIndiDesMap.find(priority);
					if (it != mPriorityIndiDesMap.end()) {
						conProDes = it.value().mConProDes;
						it = mPriorityIndiDesMap.erase(it);
					}
					return conProDes;
				}



				bool CIndividualDepthConceptProcessDescriptorProcessingQueue::isEmpty() {
					return mPriorityIndiDesMap.count() <= 0;
				}

				cint64 CIndividualDepthConceptProcessDescriptorProcessingQueue::getQueuedIndividualCount() {
					return mPriorityIndiDesMap.count();
				}

				bool CIndividualDepthConceptProcessDescriptorProcessingQueue::hasQueuedIndividuals() {
					return mPriorityIndiDesMap.count() > 0;
				}

				bool CIndividualDepthConceptProcessDescriptorProcessingQueue::isIndividualQueued(CIndividualProcessNode *individual) {
					CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualNodeID());
					return mPriorityIndiDesMap.contains(priority);
				}


				CPROCESSMAP<CIndividualDepthPriority,CIndividualDepthConceptProcessDescriptorProcessingQueueData>* CIndividualDepthConceptProcessDescriptorProcessingQueue::getIndividualDepthConceptProcessDescriptorProcessingQueue() {
					return &mPriorityIndiDesMap;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
