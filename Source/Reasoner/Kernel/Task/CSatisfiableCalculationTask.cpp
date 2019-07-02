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

#include "CSatisfiableCalculationTask.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Task {


				CSatisfiableCalculationTask::CSatisfiableCalculationTask(CMemoryPool* memoryPool) : CTask(memoryPool) {
				}

				CSatisfiableCalculationTask::~CSatisfiableCalculationTask() {
				}

				CTaskResult* CSatisfiableCalculationTask::createTaskResult(CTaskHandleContext* handlerContext) {
					return &mBoolTaskResult;
				}

				CTaskStatus* CSatisfiableCalculationTask::createTaskStatus(CTaskHandleContext* handlerContext) {
					return &mDefaultTaskResult;
				}


				CTaskContext* CSatisfiableCalculationTask::getTaskContext(CTaskHandleContext* context) {
					return getProcessContext(context);
				}

				CProcessContext* CSatisfiableCalculationTask::getProcessContext(CTaskHandleContext* context) {
					if (!mProcessContext) {
						void* memManAdd = CTaskMemoryPoolAllocationManager::allocateMemoryToContainer(sizeof(CProcessContextBase),this,context->getTaskHandleMemoryAllocationManager());
						mProcessContext = new (memManAdd) CProcessContextBase(this,context);
						mTaskContext = mProcessContext;
					} else {
						mProcessContext->updateContext(context);
					}
					return mProcessContext;
				}

				CProcessingDataBox* CSatisfiableCalculationTask::getProcessingDataBox() {
					return mProcessingDataBox;
				}


				CTask* CSatisfiableCalculationTask::makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext) {
					CTask::makeTaskReference(dependedTask,handlerContext);
					CSatisfiableCalculationTask* depSatCalcTask = (CSatisfiableCalculationTask*)dependedTask;
					depSatCalcTask->mProcessingDataBox->initProcessingDataBox(mProcessingDataBox);
					depSatCalcTask->mProcessContext->referenceProcessContext(mProcessContext);
					depSatCalcTask->mConsAdapter = mConsAdapter;
					depSatCalcTask->mClassMessAdapter = mClassMessAdapter;
					depSatCalcTask->mIndiAnalAdapter = mIndiAnalAdapter;
					depSatCalcTask->mRealMessAdapter = mRealMessAdapter;
					depSatCalcTask->mSatIncConsTestingAdapter = mSatIncConsTestingAdapter;
					depSatCalcTask->mSatIndDepTrackAdapter = mSatIndDepTrackAdapter;
					depSatCalcTask->mPossAssCollAdapter = mPossAssCollAdapter;
					return this;
				}

				CTask* CSatisfiableCalculationTask::initTask(CTask* parentTask, CTaskHandleContext* context) {
					mProcessContext = nullptr;
					mProcessingDataBox = nullptr;
					mConsAdapter = nullptr;
					mIndiAnalAdapter = nullptr;
					mClassMessAdapter = nullptr;
					mRealMessAdapter = nullptr;
					mPossAssCollAdapter = nullptr;
					mSatIncConsTestingAdapter = nullptr;
					getProcessContext(context);
					mProcessingDataBox = CObjectParameterizingAllocator< CProcessingDataBox,CProcessContext* >::allocateAndConstructAndParameterize(mProcessContext->getUsedMemoryAllocationManager(),mProcessContext);
					CTask::initTask(parentTask,context);
					return this;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::initSatisfiableCalculationTask(CConcreteOntology* ontology, CCalculationConfigurationExtension* calculationConfig, CCalculationStatisticsCollector* calcStatCollector, CTaskHandleContext* context) {
					initTask(nullptr,context);
					mConsAdapter = nullptr;
					mIndiAnalAdapter = nullptr;
					mClassMessAdapter = nullptr;
					mRealMessAdapter = nullptr;
					mPossAssCollAdapter = nullptr;
					mSatIncConsTestingAdapter = nullptr;
					mSatIndDepTrackAdapter = nullptr;
					mCalcStatColl = calcStatCollector;
					mCalculationConfig = calculationConfig;
					mProcessingDataBox->initProcessingDataBox(ontology);
					return this;
				}


				CSatisfiableCalculationTask* CSatisfiableCalculationTask::initUndependedSatisfiableCalculationTask(CSatisfiableCalculationTask* baseTask, CCalculationConfigurationExtension* calculationConfig, CCalculationStatisticsCollector* calcStatCollector, CTaskHandleContext* context) {
					initTask(nullptr,context);
					mConsAdapter = nullptr;
					mIndiAnalAdapter = nullptr;
					mClassMessAdapter = nullptr;
					mRealMessAdapter = nullptr;
					mSatIncConsTestingAdapter = nullptr;
					mSatIndDepTrackAdapter = nullptr;
					mCalcStatColl = calcStatCollector;
					mCalculationConfig = calculationConfig;
					mProcessingDataBox->initProcessingDataBox(baseTask->mProcessingDataBox);
					mProcessContext->referenceProcessContext(baseTask->mProcessContext);
					mTaskType = baseTask->mTaskType;
					return this;
				}



				CSatisfiableCalculationTask* CSatisfiableCalculationTask::initBranchDependedSatisfiableCalculationTask(CSatisfiableCalculationTask* dependedTask, CTaskHandleContext* context) {
					initTask(dependedTask,context);
					mCalcStatColl = dependedTask->mCalcStatColl;
					mCalculationConfig = dependedTask->mCalculationConfig;
					mTaskType = dependedTask->mTaskType;
					return this;
				}


				CBooleanTaskResult* CSatisfiableCalculationTask::getSatisfiableCalculationTaskResult() {
					return &mBoolTaskResult;
				}

				CCalculationConfigurationExtension* CSatisfiableCalculationTask::getCalculationConfiguration() {
					return mCalculationConfig;
				}

				CTask* CSatisfiableCalculationTask::completeTask() {
#ifndef KONCLUDE_FORCE_STATISTIC_DEACTIVATED
					if (mProcessContext && mProcessContext->getUsedProcessingStatisticsGathering()) {
						if (mParentTask) {
							// complete/flush statistics
							CSatisfiableCalculationTask* parentSatTask = (CSatisfiableCalculationTask*)mParentTask;
							if (parentSatTask->mProcessContext) {
								mProcessContext->getUsedProcessingStatisticsGathering()->addGatheredProcessingStatisticsTo(parentSatTask->mProcessContext->getUsedProcessingStatisticsGathering());
							}
						} else if (mCalcStatColl) {
							CProcessingStatisticGathering* gatheredProcStat = mProcessContext->getUsedProcessingStatisticsGathering();
							CProcessingStatistics* processingStatistics = CProcessingStatistics::getProcessingStatistics();
							QList<CProcessingStatisticDescription*>* statList = processingStatistics->getProcessingStatisticDescriptionList();
							foreach (CProcessingStatisticDescription* statDesc, *statList) {
								mCalcStatColl->addProcessingStatistics(statDesc->getStatisticDescriptionString(),gatheredProcStat->getGatheredProcessingStatistic(statDesc->getStatisticGatheringIndex()));
							}
						}
					}
#endif
					return CTask::completeTask();
				}


				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setConsistenceAdapter(CTaskPreyingAdapter* consistenceAdapter) {
					mConsAdapter = consistenceAdapter;
					return this;
				}

				CTaskPreyingAdapter* CSatisfiableCalculationTask::getConsistenceAdapter() {
					return mConsAdapter;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setSaturationIndividualsAnalysationObserver(CSaturationIndividualsAnalysingAdapter* indiAnalAdapter) {
					mIndiAnalAdapter = indiAnalAdapter;
					return this;
				}

				CSaturationIndividualsAnalysingAdapter* CSatisfiableCalculationTask::getSaturationIndividualsAnalysationObserver() {
					return mIndiAnalAdapter;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setClassificationMessageAdapter(CSatisfiableTaskClassificationMessageAdapter* classMessAdapter) {
					mClassMessAdapter = classMessAdapter;
					return this;
				}

				CSatisfiableTaskClassificationMessageAdapter* CSatisfiableCalculationTask::getClassificationMessageAdapter() {
					return mClassMessAdapter;
				}


				bool CSatisfiableCalculationTask::isCalculationTableauCompletionTask() {
					return getTaskType() == CALCULATIONTABLEAUCOMPLETIONTASK;
				}

				bool CSatisfiableCalculationTask::isCalculationTableauSaturationTask() {
					return getTaskType() == CALCULATIONTABLEAUAPPROXIMATEDSATURATIONTASK;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setCalculationTaskType(cint64 taskType) {
					mTaskType = taskType;
					return this;
				}

				cint64 CSatisfiableCalculationTask::getCalculationTaskType() {
					return getTaskType();
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setRealizationMarkedCandidatesMessageAdapter(CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* realMessObserver) {
					mRealMessAdapter = realMessObserver;
					return this;
				}

				CSatisfiableTaskRealizationMarkedCandidatesMessageAdapter* CSatisfiableCalculationTask::getRealizationMarkedCandidatesMessageAdapter() {
					return mRealMessAdapter;
				}


				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setSatisfiableTaskIncrementalConsistencyTestingAdapter(CSatisfiableTaskIncrementalConsistencyTestingAdapter* incConsTestAdaptor) {
					mSatIncConsTestingAdapter = incConsTestAdaptor;
					return this;
				}

				CSatisfiableTaskIncrementalConsistencyTestingAdapter* CSatisfiableCalculationTask::getSatisfiableTaskIncrementalConsistencyTestingAdapter() {
					return mSatIncConsTestingAdapter;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setSatisfiableTaskIndividualDependenceTrackingAdapter(CSatisfiableTaskIndividualDependenceTrackingAdapter* indDepTrackAdaptor) {
					mSatIndDepTrackAdapter = indDepTrackAdaptor;
					return this;
				}

				CSatisfiableTaskIndividualDependenceTrackingAdapter* CSatisfiableCalculationTask::getSatisfiableTaskIndividualDependenceTrackingAdapter() {
					return mSatIndDepTrackAdapter;
				}

				CSatisfiableCalculationTask* CSatisfiableCalculationTask::setPossibleAssertionCollectionAdapter(CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* possAssCollAdapter) {
					mPossAssCollAdapter = possAssCollAdapter;
					return this;
				}

				CSatisfiableTaskRealizationPossibleAssertionCollectingAdapter* CSatisfiableCalculationTask::getPossibleAssertionCollectionAdapter() {
					return mPossAssCollAdapter;
				}

			}; // end namespace Task

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
