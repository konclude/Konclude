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

#include "CIndividualDelayedBackendInitializationProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualDelayedBackendInitializationProcessingQueue::CIndividualDelayedBackendInitializationProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext) {

					mMemMan = mProcessContext->getMemoryAllocationManager();
					mIndiProDesCount = 0;
					mQueuedIndiNodeLinker = nullptr;
					mLastTakenIndiNodeQueuingLinker = nullptr;
					mIndiNodeQueuingLinkerPointer = nullptr;
					mRootQueue = this;
				}


				CIndividualDelayedBackendInitializationProcessingQueue* CIndividualDelayedBackendInitializationProcessingQueue::initProcessingQueue(CIndividualDelayedBackendInitializationProcessingQueue* processingQueue) {
					if (processingQueue) {
						mIndiProDesCount = processingQueue->mIndiProDesCount;
						mQueuedIndiNodeLinker = processingQueue->mQueuedIndiNodeLinker;
						mLastTakenIndiNodeQueuingLinker = processingQueue->mLastTakenIndiNodeQueuingLinker;
						mIndiNodeQueuingLinkerPointer = processingQueue->mIndiNodeQueuingLinkerPointer;
						mRootQueue = processingQueue->mRootQueue;
					} else {
						mQueuedIndiNodeLinker = nullptr;
						mIndiProDesCount = 0;
						mLastTakenIndiNodeQueuingLinker = nullptr;
						mIndiNodeQueuingLinkerPointer = nullptr;
						mRootQueue = this;
					}

					if (!mIndiNodeQueuingLinkerPointer) {
						mIndiNodeQueuingLinkerPointer = CObjectAllocator< QAtomicPointer<CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker> >::allocateAndConstruct(mMemMan);
						mLastIndiNodeQueuingLinker = CObjectAllocator< CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker >::allocateAndConstruct(mMemMan);
						mLastTakenIndiNodeQueuingLinker = mLastIndiNodeQueuingLinker;
						mIndiNodeQueuingLinkerPointer->store(mLastIndiNodeQueuingLinker);
					}

					return this;
				}

				CXLinker<CIndividualProcessNode*>* CIndividualDelayedBackendInitializationProcessingQueue::getProcessIndividualNodeLinker() {
					moveIndividualNodeLinkersToLocalQueue();
					return mQueuedIndiNodeLinker;
				}

				CIndividualProcessNode* CIndividualDelayedBackendInitializationProcessingQueue::takeNextProcessIndividualNode() {
					moveIndividualNodeLinkersToLocalQueue();
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mQueuedIndiNodeLinker) {
						--mIndiProDesCount;
						nextIndiNode = mQueuedIndiNodeLinker->getData();
						mQueuedIndiNodeLinker = mQueuedIndiNodeLinker->getNext();
					}
					return nextIndiNode;
				}

				CIndividualProcessNode* CIndividualDelayedBackendInitializationProcessingQueue::getNextProcessIndividualNode() {
					moveIndividualNodeLinkersToLocalQueue();
					CIndividualProcessNode* nextIndiNode = nullptr;
					if (mQueuedIndiNodeLinker) {
						nextIndiNode = mQueuedIndiNodeLinker->getData();
					}
					return nextIndiNode;
				}


				bool CIndividualDelayedBackendInitializationProcessingQueue::moveIndividualNodeLinkersToLocalQueue() {
					if (mIndiNodeQueuingLinkerPointer && mLastTakenIndiNodeQueuingLinker != *mIndiNodeQueuingLinkerPointer) {
						for (CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* lastTakenIndiNodeLinkerIt = *mIndiNodeQueuingLinkerPointer; lastTakenIndiNodeLinkerIt && lastTakenIndiNodeLinkerIt != mLastTakenIndiNodeQueuingLinker; lastTakenIndiNodeLinkerIt = *lastTakenIndiNodeLinkerIt->getNextIndividualNodeQueuingLinkerPointer()) {
							CIndividualProcessNode* indiNode = lastTakenIndiNodeLinkerIt->getIndividualProcessNode();
							insertIndiviudalProcessNode(indiNode);
						}
						mLastTakenIndiNodeQueuingLinker = *mIndiNodeQueuingLinkerPointer;
						return true;
					}
					return false;
				}



				bool CIndividualDelayedBackendInitializationProcessingQueue::addIndividualNodeQueuingLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeQueuingLinker) {
					//insertIndiviudalProcessNode(indiNodeQueuingLinker->getIndividualProcessNode());
					//return true;
					moveIndividualNodeLinkersToLocalQueue();
					CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* lastIndiNodeLinker = *mIndiNodeQueuingLinkerPointer;

					QAtomicPointer<CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker>* nextLinkerPointer = indiNodeQueuingLinker->getNextIndividualNodeQueuingLinkerPointer();

					if (nextLinkerPointer->testAndSetOrdered(nullptr, lastIndiNodeLinker)) {
						if (mIndiNodeQueuingLinkerPointer->testAndSetOrdered(lastIndiNodeLinker, indiNodeQueuingLinker)) {
							moveIndividualNodeLinkersToLocalQueue();
							return true;
						} else {
							nextLinkerPointer->store(nullptr);
							return false; 
						}
					} else {
						return false;
					}
				}


				CIndividualDelayedBackendInitializationProcessingQueue* CIndividualDelayedBackendInitializationProcessingQueue::insertIndiviudalProcessNode(CIndividualProcessNode* indiProNode) {
					CXLinker<CIndividualProcessNode*>* newIndiNodeLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(mMemMan);
					mQueuedIndiNodeLinker = newIndiNodeLinker->initLinker(indiProNode,mQueuedIndiNodeLinker);
					++mIndiProDesCount;
					return this;
				}

				bool CIndividualDelayedBackendInitializationProcessingQueue::isEmpty() {
					moveIndividualNodeLinkersToLocalQueue();
					return mQueuedIndiNodeLinker == nullptr;
				}

				bool CIndividualDelayedBackendInitializationProcessingQueue::isRoot() {
					return mRootQueue == this;
				}





			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
