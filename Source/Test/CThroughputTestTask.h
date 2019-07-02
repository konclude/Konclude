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

#ifndef KONCLUDE_TEST_CTHROUGHPUTTESTTASK_H
#define KONCLUDE_TEST_CTHROUGHPUTTESTTASK_H

// Libraries includes


// Namespace includes


// Other includes
#include "Scheduler/CTask.h"
#include "Scheduler/CBooleanTaskResult.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Scheduler;

	namespace Test {


		/*! 
		 *
		 *		\class		CThroughputTestTask
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CThroughputTestTask : public CTask {
			// public methods
			public:
				//! Constructor
				CThroughputTestTask(CMemoryPool* memoryPool = nullptr);

				//! Destructor
				virtual ~CThroughputTestTask();

				virtual CTask* makeTaskReference(CTask* dependedTask, CTaskHandleContext* handlerContext);

				virtual CTask* initTask(CTask* parentTask, CTaskHandleContext* context);
				CTask* initTestTask(cint64 maxBranchDepth, cint64 branchingFactor);

				cint64 getBranchingFactor();
				cint64 getCurrentBranchingDepth();
				cint64 getMaximumBranchingDepth();

				bool hasReachedMaxBranchingDepth();

			// protected methods
			protected:
				virtual CTaskResult* createTaskResult(CTaskHandleContext* handlerContext);
				virtual CTaskStatus* createTaskStatus(CTaskHandleContext* handlerContext);

			// protected variables
			protected:
				cint64 mMaxBranchDepth;
				cint64 mCurBranchDepth;
				cint64 mBranchingFactor;

				CBooleanTaskResult mBoolTaskResult;
				CTaskStatus mDefaultTaskResult;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CThroughputTestTask_H
