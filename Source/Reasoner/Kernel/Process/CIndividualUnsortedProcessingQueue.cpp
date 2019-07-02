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

#include "CIndividualUnsortedProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualUnsortedProcessingQueue::CIndividualUnsortedProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext) {

					mMemMan = mProcessContext->getMemoryAllocationManager();
					mIndiProDesCount = 0;
					mProcessingIndiNodeLinker = nullptr;
				}


				CIndividualUnsortedProcessingQueue* CIndividualUnsortedProcessingQueue::initProcessingQueue(CIndividualUnsortedProcessingQueue* processingQueue) {
					if (processingQueue) {
						mIndiProDesCount = processingQueue->mIndiProDesCount;
						mProcessingIndiNodeLinker = processingQueue->mProcessingIndiNodeLinker;
					} else {
						mProcessingIndiNodeLinker = nullptr;
						mIndiProDesCount = 0;
					}
					return this;
				}

				CXLinker<CIndividualProcessNode*>* CIndividualUnsortedProcessingQueue::getProcessIndividualNodeLinker() {
					return mProcessingIndiNodeLinker;
				}

				CIndividualProcessNode* CIndividualUnsortedProcessingQueue::takeNextProcessIndividualNode() {
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mProcessingIndiNodeLinker) {
						--mIndiProDesCount;
						nextIndiNode = mProcessingIndiNodeLinker->getData();
						mProcessingIndiNodeLinker = mProcessingIndiNodeLinker->getNext();
					}
					return nextIndiNode;
				}

				CIndividualProcessNode* CIndividualUnsortedProcessingQueue::getNextProcessIndividualNode() {
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mProcessingIndiNodeLinker) {
						nextIndiNode = mProcessingIndiNodeLinker->getData();
					}
					return nextIndiNode;
				}

				CIndividualUnsortedProcessingQueue* CIndividualUnsortedProcessingQueue::insertIndiviudalProcessNode(CIndividualProcessNode* indiProNode) {
					CXLinker<CIndividualProcessNode*>* newIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(mMemMan);
					mProcessingIndiNodeLinker = newIndiNodeLinker->initLinker(indiProNode,mProcessingIndiNodeLinker);
					++mIndiProDesCount;
					return this;
				}

				bool CIndividualUnsortedProcessingQueue::isEmpty() {
					return mProcessingIndiNodeLinker == nullptr;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
