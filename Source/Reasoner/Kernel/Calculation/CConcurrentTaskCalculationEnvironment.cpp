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

#include "CConcurrentTaskCalculationEnvironment.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Calculation {


				CConcurrentTaskCalculationEnvironment::CConcurrentTaskCalculationEnvironment() {
					mProcessUnit = nullptr;
					mSchedulerUnit = nullptr;
					mCompletorUnit = nullptr;
					mCallbackExecuter = nullptr;
					mStatusPropagator = nullptr;
					mAllocationLimitation = nullptr;

					mTaskCreatedDepthCountVec.resize(CTaskProcessingStatistics::VECCOUNTERSIZE);
					mTaskProcessedDepthCountVec.resize(CTaskProcessingStatistics::VECCOUNTERSIZE);
					mTaskRelativeIncreaseTaskPerDepthVec.resize(CTaskProcessingStatistics::VECCOUNTERSIZE);
					mTaskTotalIncreaseTaskPerDepthVec.resize(CTaskProcessingStatistics::VECCOUNTERSIZE);
				}

				CConcurrentTaskCalculationEnvironment::~CConcurrentTaskCalculationEnvironment() {
					qDeleteAll(mProcessorUnitList);
					mProcessorUnitList.clear();
				}


				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::initSingleTaskProcessor(CSingleThreadTaskProcessorUnit* processUnit) {
					mProcessUnit = processUnit;
					mSchedulerUnit = mProcessUnit;
					mCompletorUnit = mProcessUnit;
					mProcessorUnitList.append(mProcessUnit);
					return this;
				}

				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::initMultiTaskProcessor(CTaskProcessorSchedulerThread* schedulerUnit, CTaskProcessorCompletorThread* completorUnit) {
					mProcessUnit = nullptr;
					mSchedulerUnit = schedulerUnit;
					mCompletorUnit = completorUnit;
					mProcessorUnitList.append(schedulerUnit);
					mProcessorUnitList.append(completorUnit);
					mThreadUnitList.append(schedulerUnit);
					mThreadUnitList.append(completorUnit);
					return this;
				}

				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::appendMultiTaskProcessor(CTaskProcessorThread* processorUnit) {
					mProcessorUnitList.append(processorUnit);
					mThreadUnitList.append(processorUnit);
					return this;
				}

				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::initCallbackExecuter(CSatisfiableCalculationTaskJobCallbackExecuter* callbackExecuter) {
					mCallbackExecuter = callbackExecuter;
					return this;
				}

				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::initStatusPropagator(CSatisfiableCalculationTaskStatusPropagator* statusPropagator) {
					mStatusPropagator = statusPropagator;
					return this;
				}

				CConcurrentTaskCalculationEnvironment* CConcurrentTaskCalculationEnvironment::setAllocationLimitation(CCentralizedAllocationLimitation* allocationLimitation) {
					mAllocationLimitation = allocationLimitation;
					return this;
				}


				CSatisfiableCalculationTaskJobCallbackExecuter* CConcurrentTaskCalculationEnvironment::getSatisfiableCalculationTaskJobCallbackExecuter() {
					return mCallbackExecuter;
				}

				CSatisfiableCalculationTaskStatusPropagator* CConcurrentTaskCalculationEnvironment::getSatisfiableCalculationTaskStatusPropagator() {
					return mStatusPropagator;
				}

				CSingleThreadTaskProcessorUnit* CConcurrentTaskCalculationEnvironment::getSingleTaskProcessorUnit() {
					return mProcessUnit;
				}

				CTaskEventHandlerBasedScheduler* CConcurrentTaskCalculationEnvironment::getSchedulerTaskProcessorUnit() {
					return mSchedulerUnit;
				}

				CTaskEventHandlerBasedCompletor* CConcurrentTaskCalculationEnvironment::getCompletorTaskProcessorUnit() {
					return mCompletorUnit;
				}

				QList<CTaskEventHandlerBasedProcessor*> CConcurrentTaskCalculationEnvironment::getTaskProcessorUnitList() {
					return mProcessorUnitList;
				}

				CCentralizedAllocationLimitation* CConcurrentTaskCalculationEnvironment::getAllocationLimitation() {
					return mAllocationLimitation;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationComputionTime() {
					cint64 compTime = 0;
					if (mProcessUnit) {
						compTime += mProcessUnit->getStatisticComputionTime();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						compTime += threadUnit->getStatisticComputionTime();
					}
					return compTime;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationBlockingTime() {
					cint64 compTime = 0;
					if (mProcessUnit) {
						compTime += mProcessUnit->getStatisticBlockingTime();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						compTime += threadUnit->getStatisticBlockingTime();
					}
					return compTime;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationMemoryConsumption() {
					cint64 memComp = 0;
					memComp = mAllocationLimitation->getAllocatedMemorySizeMaximum();
					return memComp;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationMemoryReserved() {
					cint64 memComp = 0;
					memComp = mAllocationLimitation->getReservedMemorySizeMaximum();
					return memComp;
				}


				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksProcessedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksAddedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksAddedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksAddedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksCreatedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksCreatedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksCreatedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksCompletedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksCompletedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksCompletedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksUpdatedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksUpdatedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksUpdatedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticEventsProcessedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
					}
					return statVal;
				}

				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticTasksRequestedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksRequestedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticTasksRequestedCount();
					}
					return statVal;
				}

				
				cint64 CConcurrentTaskCalculationEnvironment::getCalculationStatisticThreadsBlockedCount() {
					cint64 statVal = 0;
					if (mProcessUnit) {
						statVal += mProcessUnit->getTaskProcessingStatistics()->getStatisticThreadsBlockedCount();
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						statVal += threadUnit->getTaskProcessingStatistics()->getStatisticThreadsBlockedCount();
					}
					return statVal;
				}


				double CConcurrentTaskCalculationEnvironment::getCalculationApproximatedRemainingTasksCount() {
					double remainingTasksCount = 0;
					for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE; ++i) {
						mTaskCreatedDepthCountVec[i] = 0;
						mTaskProcessedDepthCountVec[i] = 0;
						mTaskRelativeIncreaseTaskPerDepthVec[i] = 0.;
						mTaskTotalIncreaseTaskPerDepthVec[i] = 1.;
					}
					if (mProcessUnit) {
						QVector<cint64>* createdVec = mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksCreatedDepthCountVector();
						QVector<cint64>* processedVec = mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedDepthCountVector();
						for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE; ++i) {
							mTaskCreatedDepthCountVec[i] += (*createdVec)[i];
							mTaskProcessedDepthCountVec[i] += (*processedVec)[i];
						}
					}
					foreach (CTaskProcessorThreadBase* threadUnit, mThreadUnitList) {
						QVector<cint64>* createdVec = threadUnit->getTaskProcessingStatistics()->getStatisticTasksCreatedDepthCountVector();
						QVector<cint64>* processedVec = threadUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedDepthCountVector();
						for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE; ++i) {
							mTaskCreatedDepthCountVec[i] += (*createdVec)[i];
							mTaskProcessedDepthCountVec[i] += (*processedVec)[i];
						}
					}

					for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE-1; ++i) {
						cint64 processedTaskCount = mTaskProcessedDepthCountVec[i];
						cint64 nextDepthCreatedCount = mTaskCreatedDepthCountVec[i+1];
						double relIncTaskRate = 0;
						if (nextDepthCreatedCount) {
							if (processedTaskCount < 1) {
								processedTaskCount = 1;
								mTaskProcessedDepthCountVec[i] = processedTaskCount;
							}
							relIncTaskRate = (double)nextDepthCreatedCount/(double)processedTaskCount;
						}
						mTaskRelativeIncreaseTaskPerDepthVec[i] = relIncTaskRate;
					}

					for (cint64 i = CTaskProcessingStatistics::VECCOUNTERSIZE-1; i >= 0; --i) {
						double relIncValue = mTaskRelativeIncreaseTaskPerDepthVec[i];
						if (relIncValue > 0.) {
							double nextTotIncValue = mTaskTotalIncreaseTaskPerDepthVec[i+1];
							if (nextTotIncValue > 0.) {
								mTaskTotalIncreaseTaskPerDepthVec[i] = 1 + nextTotIncValue * relIncValue;
							} else {
								mTaskTotalIncreaseTaskPerDepthVec[i] = relIncValue;
							}
						}
					}

					for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE-1; ++i) {
						cint64 openTaskCount = mTaskCreatedDepthCountVec[i] - mTaskProcessedDepthCountVec[i];
						if (openTaskCount > 0) {
							remainingTasksCount += openTaskCount * mTaskTotalIncreaseTaskPerDepthVec[i];
						}
					}

					QStringList debugStringList;
					for (cint64 i = 0; i < CTaskProcessingStatistics::VECCOUNTERSIZE-1; ++i) {
						QString depthString = QString("Depth: %1, C:%2, P:%3, +:%4, *%5").arg(i).arg(mTaskCreatedDepthCountVec[i]).arg(mTaskProcessedDepthCountVec[i])
								.arg(mTaskRelativeIncreaseTaskPerDepthVec[i]).arg(mTaskTotalIncreaseTaskPerDepthVec[i]);
						debugStringList.append(depthString);
					}
					QString debugString = debugStringList.join("\r\n");

					return remainingTasksCount;
				}



			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
