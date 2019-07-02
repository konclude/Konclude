/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CThroughputTestTaskTester.h"


namespace Konclude {

	namespace Test {



		CThroughputTestTaskTester::CThroughputTestTaskTester() {
			handleAlg = new CThroughputTestTaskHandleAlgorithm();
		}


		CThroughputTestTaskTester::~CThroughputTestTaskTester() {
		}


		void CThroughputTestTaskTester::generateTestingStructure(cint64 maxBranchDepth, cint64 branchingFactor) {
			mMaxBranchDepth = maxBranchDepth;
			mBranchingFactor = branchingFactor;
			mProcessUnit = new CSingleThreadTaskProcessorUnit(handleAlg);
			mProcessUnit->startProcessing();
		}

		void CThroughputTestTaskTester::startTesting() {
			CThroughputTestTask* rootTask = CObjectMemoryPoolAllocator<CThroughputTestTask>::allocateAndConstructWithMemroyPool();
			rootTask->initTestTask(mMaxBranchDepth,mBranchingFactor);
			CTaskEventCommunicator::postSendTaskProcessEvent(mProcessUnit->getEventHandler(),rootTask,false,nullptr);
		}

		void CThroughputTestTaskTester::stopTesting() {
		}

		void CThroughputTestTaskTester::destroyTestingStructure() {
			mProcessUnit->stopProcessing();
			mProcessUnit->stopThread();
			delete mProcessUnit;
		}


		cint64 CThroughputTestTaskTester::getTaskProcessedCount() {
			cint64 totalProcessedCount = mProcessUnit->getTaskProcessingStatistics()->getStatisticTasksProcessedCount();
			return totalProcessedCount;
		}



	}; // end namespace Test

}; // end namespace Konclude
