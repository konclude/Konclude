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

#include "CIndividualDepthProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualDepthProcessingQueue::CIndividualDepthProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mPriorityIndiDesMap(processContext),mRemainingDepthIndiLinkerMap(processContext) {

					mNextRemainingDepth = 1;
					mAdditionalPriorityIndiDesMap = nullptr;
				}

				CIndividualDepthProcessingQueue* CIndividualDepthProcessingQueue::initProcessingQueue(CIndividualDepthProcessingQueue* processingQueue) {
					if (processingQueue) {
						if (!processingQueue->mAdditionalPriorityIndiDesMap && processingQueue->mPriorityIndiDesMap.size() <= 100 || processingQueue->mAdditionalPriorityIndiDesMap && processingQueue->mPriorityIndiDesMap.size()*5 < processingQueue->mAdditionalPriorityIndiDesMap->size()) {
							mPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
							mAdditionalPriorityIndiDesMap = processingQueue->mAdditionalPriorityIndiDesMap;
							mAdditionalPriIndiDesMapIt = processingQueue->mAdditionalPriIndiDesMapIt;
						} else {
							if (processingQueue->mAdditionalPriorityIndiDesMap) {
								mAdditionalPriorityIndiDesMap = CObjectParameterizingAllocator< CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getMemoryAllocationManager(),mProcessContext);
								if (processingQueue->mAdditionalPriIndiDesMapIt == processingQueue->mAdditionalPriorityIndiDesMap->constBegin()) {
									*mAdditionalPriorityIndiDesMap = *processingQueue->mAdditionalPriorityIndiDesMap;
									for (CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::const_iterator it = processingQueue->mPriorityIndiDesMap.constBegin(), itEnd = processingQueue->mPriorityIndiDesMap.constEnd(); it != itEnd; ++it) {
										mAdditionalPriorityIndiDesMap->insert(it.key(),it.value());
									}
								} else {
									*mAdditionalPriorityIndiDesMap = processingQueue->mPriorityIndiDesMap;
									for (CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::const_iterator it = processingQueue->mAdditionalPriIndiDesMapIt, itEnd = processingQueue->mAdditionalPriorityIndiDesMap->constEnd(); it != itEnd; ++it) {
										mAdditionalPriorityIndiDesMap->tryInsert(it.key(),it.value());
									}
								}
							} else {
								mAdditionalPriorityIndiDesMap = &processingQueue->mPriorityIndiDesMap;
							}
							mAdditionalPriIndiDesMapIt = mAdditionalPriorityIndiDesMap->constBegin();
							mPriorityIndiDesMap.clear();
						}
						mRemainingDepthIndiLinkerMap = processingQueue->mRemainingDepthIndiLinkerMap;
						mNextRemainingDepth = processingQueue->mNextRemainingDepth;
					} else {
						mPriorityIndiDesMap.clear();
						mRemainingDepthIndiLinkerMap.clear();
						mNextRemainingDepth = 1;
						mAdditionalPriIndiDesMapIt = mPriorityIndiDesMap.constEnd();
					}
					return this;
				}


				CIndividualProcessNode* CIndividualDepthProcessingQueue::takeNextProcessIndividual() {
					CIndividualProcessNode* nextNode = nullptr;

					if (mPriorityIndiDesMap.isEmpty() && (!mAdditionalPriorityIndiDesMap || mAdditionalPriIndiDesMapIt == mAdditionalPriorityIndiDesMap->constEnd())) {
						CPROCESSMAP<cint64,CXLinker<CIndividualProcessNode*>*>::iterator it = mRemainingDepthIndiLinkerMap.begin();
						mNextRemainingDepth = it.key()+1;
						for (CXLinker<CIndividualProcessNode*>* indiLinker = it.value(); indiLinker; indiLinker = indiLinker->getNext()) {
							CIndividualProcessNode* individual = indiLinker->getData();
							CIndividualDepthPriority priority(individual->getIndividualNominalLevelOrAncestorDepth(),individual->getIndividualNodeID());
							mPriorityIndiDesMap.tryInsert(priority,individual);
						}
						mRemainingDepthIndiLinkerMap.erase(it);
					}

					if (!mPriorityIndiDesMap.isEmpty() || mAdditionalPriorityIndiDesMap && mAdditionalPriIndiDesMapIt != mAdditionalPriorityIndiDesMap->constEnd()) {
						if (!mAdditionalPriorityIndiDesMap || mAdditionalPriIndiDesMapIt == mAdditionalPriorityIndiDesMap->constEnd()) {
							CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
							nextNode = it.value();
							mPriorityIndiDesMap.erase(it);
						} else if (mPriorityIndiDesMap.isEmpty()) {
							nextNode = mAdditionalPriIndiDesMapIt.value();
							++mAdditionalPriIndiDesMapIt;
						} else {
							CPROCESSMAP<CIndividualDepthPriority,CIndividualProcessNode*>::iterator it = mPriorityIndiDesMap.begin();
							if (it.key() == mAdditionalPriIndiDesMapIt.key()) {
								nextNode = it.value();
								++mAdditionalPriIndiDesMapIt;
								mPriorityIndiDesMap.erase(it);
							} else if (it.key() < mAdditionalPriIndiDesMapIt.key()) {
								nextNode = it.value();
								mPriorityIndiDesMap.erase(it);
							} else {
								nextNode = mAdditionalPriIndiDesMapIt.value();
								++mAdditionalPriIndiDesMapIt;
							}
						}
					}
					return nextNode;
				}


				CIndividualDepthProcessingQueue* CIndividualDepthProcessingQueue::insertProcessIndiviudal(CIndividualProcessNode* individual) {
					cint64 depth = individual->getIndividualNominalLevelOrAncestorDepth();
					CIndividualDepthPriority priority(depth,individual->getIndividualNodeID());
					if (depth >= mNextRemainingDepth) {
						CXLinker<CIndividualProcessNode*>* indiLinker = CObjectAllocator< CXLinker<CIndividualProcessNode*> >::allocateAndConstruct(mProcessContext->getMemoryAllocationManager());
						indiLinker->initLinker(individual);
						CXLinker<CIndividualProcessNode*>*& storedIndiLinker = mRemainingDepthIndiLinkerMap[depth];
						storedIndiLinker = indiLinker->append(storedIndiLinker);
					} else {
						mPriorityIndiDesMap.insert(priority,individual);
					}
					return this;
				}


				bool CIndividualDepthProcessingQueue::isEmpty() {
					return mPriorityIndiDesMap.isEmpty() && (!mAdditionalPriorityIndiDesMap || mAdditionalPriIndiDesMapIt == mAdditionalPriorityIndiDesMap->constEnd()) && mRemainingDepthIndiLinkerMap.isEmpty();
				}



			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
