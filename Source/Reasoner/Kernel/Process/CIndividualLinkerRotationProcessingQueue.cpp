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

#include "CIndividualLinkerRotationProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualLinkerRotationProcessingQueue::CIndividualLinkerRotationProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext) {

					mMemMan = mProcessContext->getMemoryAllocationManager();
					mIndiProDesCount = 0;
					mProcessingIndiNodeLinker = nullptr;
					mRotationIndiNodeLinker = nullptr;
				}


				CIndividualLinkerRotationProcessingQueue* CIndividualLinkerRotationProcessingQueue::initProcessingQueue(CIndividualLinkerRotationProcessingQueue* processingQueue) {
					if (processingQueue) {
						mIndiProDesCount = processingQueue->mIndiProDesCount;
						mProcessingIndiNodeLinker = processingQueue->mProcessingIndiNodeLinker;
						mRotationIndiNodeLinker = processingQueue->mRotationIndiNodeLinker;
					} else {
						mRotationIndiNodeLinker = nullptr;
						mProcessingIndiNodeLinker = nullptr;
						mIndiProDesCount = 0;
					}
					return this;
				}

				CXLinker<CIndividualProcessNode*>* CIndividualLinkerRotationProcessingQueue::getProcessIndividualNodeLinker() {
					if (!mProcessingIndiNodeLinker) {
						mProcessingIndiNodeLinker = mRotationIndiNodeLinker;
						mRotationIndiNodeLinker = nullptr;
					}
					return mProcessingIndiNodeLinker;
				}

				CIndividualProcessNode* CIndividualLinkerRotationProcessingQueue::takeNextProcessIndividualNode() {
					if (!mProcessingIndiNodeLinker) {
						mProcessingIndiNodeLinker = mRotationIndiNodeLinker;
						mRotationIndiNodeLinker = nullptr;
					}
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mProcessingIndiNodeLinker) {
						--mIndiProDesCount;
						nextIndiNode = mProcessingIndiNodeLinker->getData();
						mProcessingIndiNodeLinker = mProcessingIndiNodeLinker->getNext();
					}
					return nextIndiNode;
				}

				CIndividualProcessNode* CIndividualLinkerRotationProcessingQueue::getNextProcessIndividualNode() {
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mProcessingIndiNodeLinker) {
						nextIndiNode = mProcessingIndiNodeLinker->getData();
					}
					return nextIndiNode;
				}

				CIndividualLinkerRotationProcessingQueue* CIndividualLinkerRotationProcessingQueue::insertIndiviudalProcessNode(CIndividualProcessNode* indiProNode) {
					CXLinker<CIndividualProcessNode*>* newIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(mMemMan);
					mRotationIndiNodeLinker = newIndiNodeLinker->initLinker(indiProNode, mRotationIndiNodeLinker);
					++mIndiProDesCount;
					return this;
				}

				bool CIndividualLinkerRotationProcessingQueue::isEmpty() {
					return mProcessingIndiNodeLinker == nullptr && mRotationIndiNodeLinker == nullptr;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
