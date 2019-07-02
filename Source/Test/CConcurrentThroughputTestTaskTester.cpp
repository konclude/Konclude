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

#include "CConcurrentThroughputTestTaskTester.h"


namespace Konclude {

	namespace Test {



		CConcurrentThroughputTestTaskTester::CConcurrentThroughputTestTaskTester() {
			mHandleAlg = new CThroughputTestTaskHandleAlgorithm();
		}


		CConcurrentThroughputTestTaskTester::~CConcurrentThroughputTestTaskTester() {
		}


		void CConcurrentThroughputTestTaskTester::generateTestingStructure(cint64 maxBranchDepth, cint64 branchingFactor) {
			mMaxBranchDepth = maxBranchDepth;
			mBranchingFactor = branchingFactor;
			mThreadTestCount = 1;
			mThreadMaxTestCount = 8;
			mScaleReferenceTime = 0;
			mLastEventProcessed = 0;
			mLastTaskProcessed = 0;
			mTotalTestTasks = qPow(mBranchingFactor,mMaxBranchDepth+1);
			mProcessUnit = new CSingleThreadTaskProcessorUnit(mHandleAlg);
			mProcessUnit->startProcessing();
			mCompletorUnit = new CTaskProcessorCompletorThread(mHandleAlg);
			mSchedulerUnit = new CTaskProcessorSchedulerThread(mHandleAlg,mCompletorUnit);
			mCompletorUnit->installScheduler(mSchedulerUnit);
			mSchedulerUnit->installScheduler(mSchedulerUnit);
			mCompletorUnit->startProcessing();
			mSchedulerUnit->startProcessing();
		}

		void CConcurrentThroughputTestTaskTester::startTesting() {
			CThroughputTestTask* rootTask = CObjectMemoryPoolAllocator<CThroughputTestTask>::allocateAndConstructWithMemroyPool();
			rootTask->initTestTask(mMaxBranchDepth,mBranchingFactor);
			rootTask->addCallbackLinker(this);
			mMeasurementTimer.start();
			CTaskEventCommunicator::postSendTaskScheduleEvent(mProcessUnit->getEventHandler(),rootTask,nullptr);
		}

		void CConcurrentThroughputTestTaskTester::stopTesting() {
		}

		void CConcurrentThroughputTestTaskTester::destroyTestingStructure() {
			mProcessUnit->stopProcessing();
			mProcessUnit->stopThread();
			delete mProcessUnit;
		}


		cint64 CConcurrentThroughputTestTaskTester::getTaskProcessedCount() {
			cint64 totalProcessedCount = mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
			return totalProcessedCount;
		}



		void CConcurrentThroughputTestTaskTester::doCallback() {
			cint64 elapsedTime = mMeasurementTimer.elapsed();
			if (mThreadTestCount == 1) {
				mScaleReferenceTime = elapsedTime;
			}
			if (mScaleReferenceTime == 0) {
				mScaleReferenceTime = 1;
			}
			cint64 eventProcessedCount = 0;
			cint64 taskProcessedCount = 0;
			if (mThreadTestCount == 1) {
				eventProcessedCount += mProcessUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
				taskProcessedCount += mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
			} else {
				eventProcessedCount += mSchedulerUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
				eventProcessedCount += mCompletorUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
				taskProcessedCount += mSchedulerUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
				taskProcessedCount += mCompletorUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
				foreach (CTaskProcessorThread* taskProcUnit, mProcessorUnitList) {
					eventProcessedCount += taskProcUnit->getTaskProcessingStatistics()->getStatisticEventsProcessedCount();
					taskProcessedCount += taskProcUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
				}
				cint64 tmpEventProcessedCount = eventProcessedCount;
				eventProcessedCount = tmpEventProcessedCount - mLastEventProcessed;
				mLastEventProcessed = tmpEventProcessedCount;
				cint64 tmpTaskProcessedCount = taskProcessedCount;
				taskProcessedCount = tmpTaskProcessedCount - mLastTaskProcessed;
				mLastTaskProcessed = tmpTaskProcessedCount;
			}
			cout<<mThreadTestCount<<" Threads: \t"<<taskProcessedCount<<" processed Test-Tasks, \t"<<eventProcessedCount<<" processed Events, \t"<<elapsedTime<<" ms, \t"<<(double)mScaleReferenceTime/(double)elapsedTime<<" x\n";
			mMeasurementTimer.restart();
			if (++mThreadTestCount <= mThreadMaxTestCount) {
				if (mThreadTestCount != 2) {
					// add task processor
					CTaskProcessorThread* taskProcessor = new CTaskProcessorThread(mHandleAlg,mCompletorUnit);
					mProcessorUnitList.append(taskProcessor);
					taskProcessor->installScheduler(mSchedulerUnit);
					taskProcessor->startProcessing();
				}

				CThroughputTestTask* rootTask = CObjectMemoryPoolAllocator<CThroughputTestTask>::allocateAndConstructWithMemroyPool();
				rootTask->initTestTask(mMaxBranchDepth,mBranchingFactor);
				rootTask->addCallbackLinker(this);
				CTaskEventCommunicator::postSendTaskScheduleEvent(mSchedulerUnit->getEventHandler(),rootTask,nullptr);
			}
		}


	}; // end namespace Test

}; // end namespace Konclude
