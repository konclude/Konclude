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

#include "CIndividualConceptBatchProcessingQueue.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				CIndividualConceptBatchProcessingQueue::CIndividualConceptBatchProcessingQueue(CProcessContext* processContext) 
						: mProcessContext(processContext),mConceptIndiQueueMap(processContext),mBindCountIndiQueueMap(processContext) {

				}

				CIndividualConceptBatchProcessingQueue* CIndividualConceptBatchProcessingQueue::initProcessingQueue(CIndividualConceptBatchProcessingQueue* processingQueue) {
					if (processingQueue) {
						mConceptIndiQueueMap = processingQueue->mConceptIndiQueueMap;
						mBindCountIndiQueueMap = processingQueue->mBindCountIndiQueueMap;
						mUseCurrentProcessingQueue = processingQueue->mUseCurrentProcessingQueue;
						mCurrentProcessingTag = processingQueue->mCurrentProcessingTag;
						mLocCurrentProcessingQueue = nullptr;
						mBindindBasedQueuedCount = processingQueue->mBindindBasedQueuedCount;
					} else {
						mConceptIndiQueueMap.clear();
						mBindCountIndiQueueMap.clear();
						mLocCurrentProcessingQueue = nullptr;
						mUseCurrentProcessingQueue = nullptr;
						mCurrentProcessingTag = -1;
						mBindindBasedQueuedCount = 0;
					}
					return this;
				}



				bool CIndividualConceptBatchProcessingQueue::isEmpty() {
					if (mBindindBasedQueuedCount > 0) {
						return false;
					}
					if (mUseCurrentProcessingQueue) {
						if (!mUseCurrentProcessingQueue->getIndividualQueue()->isEmpty()) {
							return false;
						}
					}
					for (CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData>::const_iterator it = mConceptIndiQueueMap.constBegin(), itEnd = mConceptIndiQueueMap.constEnd(); it != itEnd; ++it) {
						const CIndividualConceptBatchProcessingQueueData& data = it.value();
						if (data.mUseProcData) {
							if (!data.mUseProcData->getIndividualQueue()->isEmpty()) {
								return false;
							}
						}
					}
					return true;
				}



				bool CIndividualConceptBatchProcessingQueue::takeNextConceptProcessIndividual(CConcept*& concept, CIndividualProcessNode*& indiNode, CConceptProcessDescriptor*& conProDes) {
					if (!mUseCurrentProcessingQueue) {

						if (mBindindBasedQueuedCount > 0) {
							for (CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData>::iterator it = mBindCountIndiQueueMap.begin(), itEnd = mBindCountIndiQueueMap.end(); it != itEnd; ++it) {
								CIndividualConceptBatchProcessingQueueData& data = it.value();
								if (!data.mUseProcData->getIndividualQueue()->isEmpty()) {
									CIndividualConceptBatchProcessingData* procData = nullptr;
									if (!data.mLocProcData) {
										procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
										procData->initConceptBatchProcessingData(data.mUseProcData);
										data.mLocProcData = procData;
										data.mUseProcData = procData;
									} else {
										procData = data.mUseProcData;
									}

									procData->getIndividualQueue()->takeNextProcessIndiviudalConceptProcessDescriptor(indiNode,conProDes);
									cint64 indiID = indiNode->getIndividualNodeID();
									cint64 conTag = conProDes->getConceptDescriptor()->getConceptTag();
									CIndividualConceptQueuedData& indConQueData = mBindCountIndiQueuedHash[QPair<cint64,cint64>(conTag,indiID)];
									indConQueData.mQueued = false;
									--mBindindBasedQueuedCount;
									return true;
								}
							}

							// TODO: check bug??
							mBindindBasedQueuedCount = 0;
						}


						//if (!mBindCountIndiQueueMap.isEmpty()) {
						//	CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData>::iterator it = mBindCountIndiQueueMap.begin();
						//	while (it != mBindCountIndiQueueMap.end() && (!mUseCurrentProcessingQueue || mUseCurrentProcessingQueue->getIndividualQueue()->isEmpty())) {
						//		CIndividualConceptBatchProcessingQueueData& data = it.value();
						//		mCurrentProcessingTag = it.key();
						//		mUseCurrentProcessingQueue = data.mUseProcData;
						//		mLocCurrentProcessingQueue = data.mLocProcData;
						//		it = mBindCountIndiQueueMap.erase(it);
						//	}
						//}
						if (!mConceptIndiQueueMap.isEmpty()) {
							CPROCESSMAP<cint64,CIndividualConceptBatchProcessingQueueData>::iterator it = mConceptIndiQueueMap.begin();
							while (it != mConceptIndiQueueMap.end() && (!mUseCurrentProcessingQueue || mUseCurrentProcessingQueue->getIndividualQueue()->isEmpty())) {
								CIndividualConceptBatchProcessingQueueData& data = it.value();
								mCurrentProcessingTag = it.key();
								mUseCurrentProcessingQueue = data.mUseProcData;
								mLocCurrentProcessingQueue = data.mLocProcData;
								it = mConceptIndiQueueMap.erase(it);
							}
						}
					}
					if (mUseCurrentProcessingQueue) {
						cint64 queuedIndiCount = mUseCurrentProcessingQueue->getIndividualQueue()->getQueuedIndividualCount();
						if (queuedIndiCount > 1) {
							if (!mLocCurrentProcessingQueue) {
								CIndividualConceptBatchProcessingData* procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
								procData->initConceptBatchProcessingData(mUseCurrentProcessingQueue);
								mLocCurrentProcessingQueue = procData;
								mUseCurrentProcessingQueue = procData;
							}
							mUseCurrentProcessingQueue->getIndividualQueue()->takeNextProcessIndiviudalConceptProcessDescriptor(indiNode,conProDes);
							//if (!mUseCurrentProcessingQueue->hasConcept()) {
							//	cint64 indiID = indiNode->getIndividualID();
							//	cint64 conTag = conProDes->getConceptDescriptor()->getConceptTag();
							//	CIndividualConceptQueuedData& indConQueData = mBindCountIndiQueuedHash[QPair<cint64,cint64>(conTag,indiID)];
							//	indConQueData.mQueued = false;
							//}
						} else if (queuedIndiCount == 1) {
							mUseCurrentProcessingQueue->getIndividualQueue()->getNextProcessIndiviudalConceptProcessDescriptor(indiNode,conProDes);
							//if (!mUseCurrentProcessingQueue->hasConcept()) {
							//	cint64 indiID = indiNode->getIndividualID();
							//	cint64 conTag = conProDes->getConceptDescriptor()->getConceptTag();
							//	CIndividualConceptQueuedData& indConQueData = mBindCountIndiQueuedHash[QPair<cint64,cint64>(conTag,indiID)];
							//	indConQueData.mQueued = false;
							//}
							mUseCurrentProcessingQueue = nullptr;
							mLocCurrentProcessingQueue = nullptr;
							mCurrentProcessingTag = -1;
						} else {
							mUseCurrentProcessingQueue = nullptr;
							mLocCurrentProcessingQueue = nullptr;
							mCurrentProcessingTag = -1;
						}
					}
					return true;
				}


				CIndividualConceptBatchProcessingQueue* CIndividualConceptBatchProcessingQueue::insertIndiviudalForConcept(CConcept* concept, CIndividualProcessNode* individual, CConceptProcessDescriptor* conProDes) {
					CIndividualConceptBatchProcessingData* procData = getBatchProcessingData(concept,true);
					procData->getIndividualQueue()->insertProcessIndiviudalConceptProcessDescriptor(individual,conProDes);
					return this;
				}

				CIndividualConceptBatchProcessingQueue* CIndividualConceptBatchProcessingQueue::insertIndiviudalForBindingCount(CConcept* concept, cint64 bindCount, CIndividualProcessNode* individual, CConceptProcessDescriptor* conProDes) {
					cint64 conTag = conProDes->getConceptDescriptor()->getConceptTag();
					cint64 indiID = individual->getIndividualNodeID();
					CIndividualConceptQueuedData& queuedData = mBindCountIndiQueuedHash[QPair<cint64,cint64>(conTag,indiID)];
					if (!queuedData.mQueued || bindCount > queuedData.mPrevTag) {
						CIndividualConceptBatchProcessingData* procData = getBatchProcessingData(concept, bindCount, true);
						if (queuedData.mQueued) {
							// remove previous 
							CIndividualConceptBatchProcessingData* prevProcData = getBatchProcessingData(concept,queuedData.mPrevTag,true);
							CConceptProcessDescriptor* prevConProDes = prevProcData->getIndividualQueue()->removeQueuedProcessIndiviudal(individual);
							while (prevConProDes) {
								if (prevConProDes->getProcessingRestrictionSpecification()) {
									procData->getIndividualQueue()->insertProcessIndiviudalConceptProcessDescriptor(individual, prevConProDes);
								}
								prevConProDes = prevProcData->getIndividualQueue()->removeQueuedProcessIndiviudal(individual);
							}
						} else {
							++mBindindBasedQueuedCount;
						}
						procData->getIndividualQueue()->insertProcessIndiviudalConceptProcessDescriptor(individual,conProDes);
						queuedData.mPrevTag = bindCount;
						queuedData.mQueued = true;
					}
					return this;
				}


				CIndividualConceptBatchProcessingData* CIndividualConceptBatchProcessingQueue::getBatchProcessingData(CConcept* concept, bool createAndLocalize) {
					CIndividualConceptBatchProcessingData* procData = nullptr;
					cint64 conTag = concept->getConceptTag();
					if (createAndLocalize) {
						//if (mCurrentProcessingTag == conTag) {
						//	if (!mLocCurrentProcessingQueue) {
						//		procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
						//		procData->initConceptBatchProcessingData(mUseCurrentProcessingQueue);
						//		mLocCurrentProcessingQueue = procData;
						//		mUseCurrentProcessingQueue = procData;
						//	} else {
						//		procData = mUseCurrentProcessingQueue;
						//	}
						//} else {
							CIndividualConceptBatchProcessingQueueData& data = mConceptIndiQueueMap[conTag];
							if (!data.mLocProcData) {
								procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
								procData->initConceptBatchProcessingData(data.mUseProcData);
								procData->setConcept(concept);
								data.mUseProcData = procData;
								data.mLocProcData = procData;
							}
							procData = data.mUseProcData;
						//}
					} else {
						//if (mCurrentProcessingTag == conTag) {
						//	procData = mUseCurrentProcessingQueue;
						//} else {
							const CIndividualConceptBatchProcessingQueueData& data = mConceptIndiQueueMap.value(conTag);
							procData = data.mUseProcData;
						//}
					}
					return procData;
				}

				CIndividualConceptBatchProcessingData* CIndividualConceptBatchProcessingQueue::getBatchProcessingData(CConcept* concept, cint64 bindCount, bool createAndLocalize) {
					CIndividualConceptBatchProcessingData* procData = nullptr;
					cint64 processingTag = -bindCount;
					if (createAndLocalize) {
						if (mCurrentProcessingTag == processingTag) {
							if (!mLocCurrentProcessingQueue) {
								procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
								procData->initConceptBatchProcessingData(mUseCurrentProcessingQueue);
								mLocCurrentProcessingQueue = procData;
								mUseCurrentProcessingQueue = procData;
							} else {
								procData = mUseCurrentProcessingQueue;
							}
						} else {
							CIndividualConceptBatchProcessingQueueData& data = mBindCountIndiQueueMap[processingTag];
							if (!data.mLocProcData) {
								procData = CObjectParameterizingAllocator< CIndividualConceptBatchProcessingData,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
								procData->initConceptBatchProcessingData(data.mUseProcData);
								data.mUseProcData = procData;
								data.mLocProcData = procData;
							}
							procData = data.mUseProcData;
						}
					} else {
						if (mCurrentProcessingTag == processingTag) {
							procData = mUseCurrentProcessingQueue;
						} else {
							const CIndividualConceptBatchProcessingQueueData& data = mBindCountIndiQueueMap.value(processingTag);
							procData = data.mUseProcData;
						}
					}
					return procData;
				}


			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
