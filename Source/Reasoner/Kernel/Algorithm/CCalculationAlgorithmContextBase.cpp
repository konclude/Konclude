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

#include "CCalculationAlgorithmContextBase.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CCalculationAlgorithmContextBase::CCalculationAlgorithmContextBase() {

					mTempMemMan = nullptr;
					mConceptPriorityStrategy = nullptr;
					mIndividualPriorityStrategy = nullptr;
					mProcessingDataBox = nullptr;
					mProcessContext = nullptr;
					mPrTaskMemMan = nullptr;
					mProcessTagger = nullptr;
					mSatCalcTask = nullptr;
					mTaskProcessorContext = nullptr;
					mTaskPriorityStrategy = nullptr;
					mProcStatGath = nullptr;
					mBranchTreeNode = nullptr;
					mBaseDepNode = nullptr;
					mUnsatCacheHandler = nullptr;
					mIndiNodeManager = nullptr;
					mClashDescriptorFactory = nullptr;
					mUnsatCachRetStrategy = nullptr;

				}


				CCalculationAlgorithmContextBase::~CCalculationAlgorithmContextBase() {
				}


				CCalculationAlgorithmContextBase* CCalculationAlgorithmContextBase::initCalculationAlgorithmContext(CTaskProcessorContext* taskHandleContext) {
					initCalculationAlgorithmContext(taskHandleContext,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
					return this;
				}


				CCalculationAlgorithmContextBase* CCalculationAlgorithmContextBase::initCalculationAlgorithmContext(CTaskProcessorContext* taskProcessorContext, CConceptProcessingPriorityStrategy* conceptPriorityStrategy, 
							CIndividualProcessingPriorityStrategy* individualPriorityStrategy, CTaskProcessingPriorityStrategy* taskPriorityStrategy, CUnsatisfiableCacheRetrievalStrategy* unsatCacheRetrievalStrategy,
							CIndividualNodeManager* indiNodeManager, CClashDescriptorFactory* clashDesFactory, CDependencyFactory* depFactory, CUnsatisfiableCacheHandler* unsatCacheHandler, CSatisfiableExpanderCacheHandler* satExpCacheHandler, CSaturationNodeExpansionCacheHandler* satNodeExpCacheHandler) {
					mTaskProcessorContext = taskProcessorContext;
					mUsedTaskProcessorContext = mTaskProcessorContext;
					mTempMemMan = mTaskProcessorContext->getTaskHandleMemoryAllocationManager();
					mUsedTempMemMan = mTempMemMan;
					mIndividualPriorityStrategy = individualPriorityStrategy;
					mUsedIndividualPriorityStrategy = mIndividualPriorityStrategy;
					mConceptPriorityStrategy = conceptPriorityStrategy;
					mUsedConceptPriorityStrategy = mConceptPriorityStrategy;
					mTaskPriorityStrategy = taskPriorityStrategy;
					mUsedTaskPriorityStrategy = mTaskPriorityStrategy;
					mUnsatCachRetStrategy = unsatCacheRetrievalStrategy;
					mUsedUnsatCachRetStrategy = mUnsatCachRetStrategy;

					mUnsatCacheHandler = unsatCacheHandler;
					mUsedUnsatCacheHandler = mUnsatCacheHandler;
					mClashDescriptorFactory = clashDesFactory;
					mUsedClashDescriptorFactory = mClashDescriptorFactory;
					mIndiNodeManager = indiNodeManager;
					mUsedIndividualNodeManager = mIndiNodeManager;

					mDepFactory = depFactory;
					mUsedDepFactory = mDepFactory;

					mSatExpCacheHandler = satExpCacheHandler;
					mUsedSatExpCacheHandler = mSatExpCacheHandler;


					mSatNodeExpCacheHandler = satNodeExpCacheHandler;
					mUsedSatNodeExpCacheHandler = mSatNodeExpCacheHandler;

					return this;
				}

				CCalculationAlgorithmContextBase* CCalculationAlgorithmContextBase::initTaskProcessContext(CProcessContext* processContext, CSatisfiableCalculationTask* satCalcTask) {
					mProcessContext = processContext;
					mUsedProcessContext = mProcessContext;
					mSatCalcTask = satCalcTask;
					mUsedSatCalcTask = satCalcTask;
					mProcessingDataBox = mSatCalcTask->getProcessingDataBox();
					mUsedProcessingDataBox = mProcessingDataBox;

					mPrTaskMemMan = mProcessContext->getUsedMemoryAllocationManager();
					mUsedPrTaskMemMan = mPrTaskMemMan;
					mProcessTagger = mProcessContext->getUsedProcessTagger();
					mUsedProcessTagger = mProcessTagger;
					mProcStatGath = mProcessContext->getUsedProcessingStatisticsGathering();
					mUsedProcessStatGath = mProcStatGath;

					CBranchingTree* branchingTree = mProcessingDataBox->getBranchingTree(true);

					mBranchTreeNode = branchingTree->getBranchTreeNode(satCalcTask);
					mUsedBranchTreeNode = mBranchTreeNode;

					mBaseDepNode = branchingTree->getBaseDependencyNode(true);
					mUsedBaseDepNode = mBaseDepNode;

					mMaxCompletionGraphCachedIndiNodeID = -1;
					CConsistence* consistence = mProcessingDataBox->getOntology()->getConsistence();
					if (consistence) {
						CConsistenceData* consData = consistence->getConsistenceModelData();
						if (consData) {
							CConsistenceTaskData* consTaskData = dynamic_cast<CConsistenceTaskData*>(consData);
							if (consTaskData) {
								CSatisfiableCalculationTask* detSatCalcTask = consTaskData->getDeterministicSatisfiableTask();
								CSatisfiableCalculationTask* compGraphCachedCalcTask = consTaskData->getCompletionGraphCachedSatisfiableTask();
								if (detSatCalcTask && compGraphCachedCalcTask) {
									mMaxCompletionGraphCachedIndiNodeID = detSatCalcTask->getProcessingDataBox()->getIndividualProcessNodeVector()->getItemCount()-1;
									mCompletionGraphCachedLocalizationTag = detSatCalcTask->getProcessingDataBox()->getProcessContext()->getProcessTagger()->getCurrentLocalizationTag();
								}
							}
						}
					}

					return this;
				}



				CProcessTagger* CCalculationAlgorithmContextBase::getProcessTagger() {
					return mProcessTagger;
				}

				CProcessContext* CCalculationAlgorithmContextBase::getProcessContext() {
					return mProcessContext;
				}

				CMemoryAllocationManager* CCalculationAlgorithmContextBase::getProcessTaskMemoryAllocationManager() {
					return mPrTaskMemMan;
				}

				CMemoryAllocationManager* CCalculationAlgorithmContextBase::getMemoryAllocationManager() {
					return mPrTaskMemMan;
				}

				CTaskHandleMemoryAllocationManager* CCalculationAlgorithmContextBase::getTemporaryMemoryAllocationManager() {
					return mTempMemMan;
				}

				CConceptProcessingPriorityStrategy* CCalculationAlgorithmContextBase::getConceptPriorityStrategy() {
					return mConceptPriorityStrategy;
				}

				CIndividualProcessingPriorityStrategy* CCalculationAlgorithmContextBase::getIndividualPriorityStrategy() {
					return mIndividualPriorityStrategy;
				}

				CProcessingDataBox* CCalculationAlgorithmContextBase::getProcessingDataBox() {
					return mProcessingDataBox;
				}

				CSatisfiableCalculationTask* CCalculationAlgorithmContextBase::getSatisfiableCalculationTask() {
					return mSatCalcTask;
				}

				CTaskProcessorContext* CCalculationAlgorithmContextBase::getTaskProcessorContext() {
					return mTaskProcessorContext;
				}

				CTaskProcessingPriorityStrategy* CCalculationAlgorithmContextBase::getTaskPriorityStrategy() {
					return mTaskPriorityStrategy;
				}

				CProcessingStatisticGathering* CCalculationAlgorithmContextBase::getProcessingStatisticsGathering() {
					return mProcStatGath;
				}

				CBranchTreeNode* CCalculationAlgorithmContextBase::getBranchTreeNode() {
					return mBranchTreeNode;
				}


				CBranchTreeNode* CCalculationAlgorithmContextBase::getNewBranchTreeNode() {
					mBranchTreeNode = getUsedProcessingDataBox()->getBranchingTree(true)->getBranchTreeNode(mSatCalcTask,true);
					mUsedBranchTreeNode = mBranchTreeNode;
					return mBranchTreeNode;
				}

				
				CDependencyNode* CCalculationAlgorithmContextBase::getBaseDependencyNode() {
					return mBaseDepNode;
				}


				CUnsatisfiableCacheHandler* CCalculationAlgorithmContextBase::getUnsatisfiableCacheHandler() {
					return mUnsatCacheHandler;
				}

				CIndividualNodeManager* CCalculationAlgorithmContextBase::getIndividualNodeManager() {
					return mIndiNodeManager;
				}


				CClashDescriptorFactory* CCalculationAlgorithmContextBase::getClashDescriptorFactory() {
					return mClashDescriptorFactory;
				}
				
				CUnsatisfiableCacheRetrievalStrategy* CCalculationAlgorithmContextBase::getUnsatisfiableCacheRetrievalStrategy() {
					return mUnsatCachRetStrategy;
				}

				CDependencyFactory* CCalculationAlgorithmContextBase::getDependencyFactory() {
					return mDepFactory;
				}


				CSatisfiableExpanderCacheHandler* CCalculationAlgorithmContextBase::getSatisfiableExpanderCacheHandler() {
					return mSatExpCacheHandler;
				}


				CSaturationNodeExpansionCacheHandler* CCalculationAlgorithmContextBase::getSaturationNodeExpansionCacheHandler() {
					return mSatNodeExpCacheHandler;
				}


			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
