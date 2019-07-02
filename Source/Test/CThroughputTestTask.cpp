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

#include "CThroughputTestTask.h"


namespace Konclude {

	namespace Test {



		CThroughputTestTask::CThroughputTestTask(CMemoryPool* memoryPool) : CTask(memoryPool) {
			mMaxBranchDepth = 0;
			mCurBranchDepth = 0;
			mBranchingFactor = 0;
		}


		CThroughputTestTask::~CThroughputTestTask() {
		}


		cint64 CThroughputTestTask::getBranchingFactor() {
			return mBranchingFactor;
		}

		cint64 CThroughputTestTask::getCurrentBranchingDepth() {
			return mCurBranchDepth;
		}

		cint64 CThroughputTestTask::getMaximumBranchingDepth() {
			return mMaxBranchDepth;
		}



		CTask* CThroughputTestTask::makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext) {
			CTask::makeTaskReference(dependedTask,handlerContext);
			CThroughputTestTask* testDepTask = (CThroughputTestTask*)dependedTask;
			testDepTask->mMaxBranchDepth = mMaxBranchDepth;
			testDepTask->mBranchingFactor = mBranchingFactor;
			testDepTask->mCurBranchDepth = mCurBranchDepth+1;
			return this;
		}

		CTask* CThroughputTestTask::initTask(CTask* parentTask, CTaskHandleContext* context) {
			CTask::initTask(parentTask,context);
			return this;
		}

		CTask* CThroughputTestTask::initTestTask(cint64 maxBranchDepth, cint64 branchingFactor) {
			CTask::initTask(nullptr,nullptr);
			mBranchingFactor = branchingFactor;
			mMaxBranchDepth = maxBranchDepth;
			return this;
		}


		bool CThroughputTestTask::hasReachedMaxBranchingDepth() {
			return mCurBranchDepth >= mMaxBranchDepth;
		}

		CTaskResult* CThroughputTestTask::createTaskResult(CTaskHandleContext* handlerContext) {
			return &mBoolTaskResult;
		}

		CTaskStatus* CThroughputTestTask::createTaskStatus(CTaskHandleContext* handlerContext) {
			return &mDefaultTaskResult;
		}

	}; // end namespace Test

}; // end namespace Konclude
